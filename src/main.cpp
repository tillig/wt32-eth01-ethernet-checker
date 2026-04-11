#include <Arduino.h>
#include <ETH.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define BUTTON_PIN 35

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/**
 * @brief Logs status updates to Serial Monitor and OLED display.
 * @param title The title of the status update.
 * @param detail The details of the status update.
 * @param progress The progress percentage.
 * @param isEndState Whether this is the final state of the test (and we're waiting for a button press to restart).
 */
void logStatus(String title, String detail, int progress, bool isEndState = false)
{
  // Output to Serial Monitor for debugging
  Serial.println("-------------------------");
  Serial.println("STATUS: " + title);
  Serial.println("DETAIL: " + detail);
  Serial.println("PROGRESS: " + String(progress) + "%");

  // Output to OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println(title);
  display.setCursor(0, 12);
  display.println(detail);

  if (isEndState)
  {
    Serial.println("WAITING FOR BUTTON PRESS...");
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

/**
 * @brief Runs the Ethernet connectivity check.
 */
void runEthernetCheck()
{
  Serial.println("\n*** STARTING ETHERNET TEST ***");

  // Stage 1: DHCP
  logStatus("ETHERNET", "Negotiating DHCP...", 20);
  unsigned long start = millis();
  while (!ETH.linkUp() || ETH.localIP().toString() == "0.0.0.0")
  {
    if (millis() - start > 12000)
    {
      logStatus("DHCP FAIL", "No IP received.", 0, true);
      return;
    }

    delay(100);
  }

  // Stage 2: Ping
  logStatus("IP: " + ETH.localIP().toString(), "Pinging 8.8.8.8...", 60);
  if (!Ping.ping("8.8.8.8", 2))
  {
    logStatus("PING FAIL", "Google Unreachable", 0, true);
    return;
  }

  // Stage 3: HTTP
  logStatus("PING OK", "Checking HTTP...", 85);
  HTTPClient http;
  http.begin("http://connectivitycheck.gstatic.com/generate_204");
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    logStatus("SYSTEMS OK", "Full Connectivity!", 100, true);
  }
  else
  {
    logStatus("HTTP FAIL", "Error: " + String(httpCode), 0, true);
  }

  http.end();
}

/**
 * @brief Initializes the WT32-ETH01 checker.
 */
void setup()
{
  Serial.begin(115200);
  delay(500); // Wait for serial monitor to open
  Serial.println("\n[BOOT] WT32-ETH01 checker starting...");

  pinMode(BUTTON_PIN, INPUT); // External 10k Pull-up used
  Serial.println("[BOOT] GPIO 35 input initialized.");
  // Wire.begin(SDA, SCL);
  Wire.begin(14, 15);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("[WARN] OLED not found. Check IO14/IO15. Continuing in Serial-only mode.");
  }
  else
  {
    Serial.println("[INFO] OLED initialized.");
  }

  Serial.println("[INFO] Starting Ethernet PHY...");
  ETH.begin();
  runEthernetCheck();
}

/**
 * @brief Main loop checks for button press to restart the test.
 */
void loop()
{
  // Manual Trigger: Check if IO35 is pulled LOW (button pressed)
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      Serial.println("[USER] Button pressed. Restarting test.");
      while (digitalRead(BUTTON_PIN) == LOW)
        ; // Wait for release
      runEthernetCheck();
    }
  }
}
