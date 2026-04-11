#include <Arduino.h>
#include <ETH.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define BUTTON_PIN 35

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void showStatus(String title, String detail, int progress, bool isEndState = false)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println(title);
  display.setCursor(0, 12);
  display.println(detail);

  if (isEndState)
  {
    display.setCursor(0, 24);
    display.print("-> Press for RETEST");
  }
  else
  {
    int barWidth = map(progress, 0, 100, 0, 128);
    display.fillRect(0, 28, barWidth, 4, SSD1306_WHITE);
  }
  display.display();
}

void runEthernetCheck()
{
  // Stage 1: DHCP
  showStatus("ETHERNET", "Negotiating DHCP...", 20);
  unsigned long start = millis();
  while (!ETH.linkUp() || ETH.localIP().toString() == "0.0.0.0")
  {
    if (millis() - start > 12000)
    {
      showStatus("DHCP FAIL", "No IP received.", 0, true);
      return;
    }
    delay(100);
  }

  // Stage 2: Ping
  showStatus("IP: " + ETH.localIP().toString(), "Pinging 8.8.8.8...", 60);
  if (!Ping.ping("8.8.8.8", 2))
  {
    showStatus("PING FAIL", "Google Unreachable", 0, true);
    return;
  }

  // Stage 3: HTTP
  showStatus("PING OK", "Checking HTTP...", 85);
  HTTPClient http;
  http.begin("http://connectivitycheck.gstatic.com/generate_204");
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    showStatus("SYSTEMS OK", "Full Connectivity!", 100, true);
  }
  else
  {
    showStatus("HTTP FAIL", "Firewall/Proxy?", 0, true);
  }
  http.end();
}

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT); // External 10k Pull-up used
  Wire.begin(33, 32);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 failed");
    for (;;)
      ;
  }

  ETH.begin();
  runEthernetCheck();
}

void loop()
{
  // Manual Trigger: Check if IO35 is pulled LOW (button pressed)
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      while (digitalRead(BUTTON_PIN) == LOW)
        ; // Wait for release
      runEthernetCheck();
    }
  }
}
