# WT32-ETH01 Ethernet Checker

A standalone tool to verify network connectivity. It checks DHCP, Internet (ICMP), and Web (HTTP) access.

When you plug it in, it will test...

- **DHCP:** Verifies network layer and cable integrity.
- **PING:** Verifies routing to the wider internet.
- **HTTP:** Verifies that port 80 traffic isn't blocked by firewalls.

If any of those things fail, it'll tell you which one.

- [Parts](#parts)
- [Hardware Wiring (Production)](#hardware-wiring-production)
- [Hardware Wiring (Programming)](#hardware-wiring-programming)
- [Programming](#programming)
- [Breadboard and Testing](#breadboard-and-testing)
  - [The "Combined" Breadboard Strategy](#the-combined-breadboard-strategy)
  - [Step-by-Step Programming Process](#step-by-step-programming-process)
  - [Transitioning to Testing](#transitioning-to-testing)
  - [Testing the "Production" Power](#testing-the-production-power)
  - [Summary Checklist for Success](#summary-checklist-for-success)
- [References](#references)

## Parts

- [WT32-ETH01](https://amzn.to/4bPwoAS) - $17
- [0.91" I2C IIC 128x32 OLED display](https://amzn.to/3NF82zR) - $12 for 3
- [USB 3.1 female breakout board](https://amzn.to/4mrg90i) - $9 for 10
- [2-pin normally open button](https://amzn.to/4cDuVxz) - $6.50 for 12
- [10K resistor](https://amzn.to/4svy7jM) - $5.50 for a variety pack of 1000

Prices listed are as of this writing. Basically, while there are devices out there that do this same function, they're way more expensive than the parts for this one.

## Hardware Wiring (Production)

| Component      | Component Pin | WT32-ETH01 Pin | Note                                   |
| -------------- | ------------- | -------------- | -------------------------------------- |
| OLED           | SDA           | IO14           | Data                                   |
| OLED           | SCL           | IO15           | Clock                                  |
| OLED           | VCC           | 3V3            | Power for display                      |
| OLED           | GND           | GND            | Ground for display                     |
| Button         | One Pin       | IO35           | Push to reset testing                  |
| Button         | One Pin       | GND            | Push to reset testing                  |
| 10k Resistor   | One Pin       | IO35           | Pull-up to keep signal HIGH by default |
| 10k Resistor   | One Pin       | 3V3            | Pull-up to keep signal HIGH by default |
| USB-C Breakout | VBUS          | 5V             | Production power input                 |
| USB-C Breakout | GND           | GND            | Production power input                 |

## Hardware Wiring (Programming)

I used [this ESP32 downloader kit from M5Stack](https://shop.m5stack.com/products/esp32-downloader-kit).

Pin connections for USB programmer:

| USB Programmer | WT32-ETH01 |
| -------------- | ---------- |
| GND            | GND        |
| G0             | IO0        |
| EN             | EN         |
| TXD            | RXD        |
| RXD            | TXD        |
| 3.3V           | 3V3        |

## Programming

This project is done with [VS Code and PlatformIO](https://docs.platformio.org/en/latest/integration/ide/vscode.html).

1. Set up the ESP32 downloader with your WT32-ETH01.
2. Connect it via USB to your computer.
3. Open in VS Code/PlatformIO and click **Upload**.
4. Remove the downloader and connect the components for standalone USB-C operation.

## Breadboard and Testing

When you're going from "trying things out" to "wired for production" here are some instructions to help get there.

### The "Combined" Breadboard Strategy

All of the components - the production components and the downloader - should all be wired up to the appropriate pins on the WT32 via the breadboard. To keep everything connected safely, follow these rules:

- Shared ground: All components (WT32, OLED, Button, USB-C, and Downloader) must share the same GND rail on your breadboard.
- The power rule: Only connect one 5V source at a time.
- While programming: Plug in the ESP 32 downloader. Leave the USB-C cable unplugged.
- While testing "Production": Unplug the Downloader (or its USB cable) and plug in the USB-C breakout.

### Step-by-Step Programming Process

1. Clone the code from the repo.
2. Plug the ESP32 downloader into your computer (this powers the board).
3. In VS Code, click the "PlatformIO: Upload" icon (the right-pointing arrow) in the bottom status bar.

Watch the terminal. You should see Writing at 0x00001000... and a percentage climb. At the end you should see a "success" message.

### Transitioning to Testing

The WT32-ETH01 does not have a reset button. To run testing, you will have to cycle power (unplug/plug it back into USB power) or push the button to start the test again.

The OLED should light up and display "ETHERNET: Negotiating DHCP..."

Plug in an Ethernet cable.

Watch the progress bar. If it hits "SYSTEMS OK," your hardware and software are verified.

### Testing the "Production" Power

Now, let's verify your USB-C breakout works for the final build:

1. Unplug the ESP32 downloader's USB cable from your computer.
2. Ensure your USB-C breakout is wired to the 5V and GND rails.
3. Plug a USB-C cable into the breakout.
4. The device should spring to life exactly as it did with the downloader.

Press your physical button on IO35. The screen should clear and restart the test sequence.

### Summary Checklist for Success

- Flash Mode: IO0 -> GND + Reset.
- Run Mode: IO0 -> Disconnected + Reset.
- Wiring Check: Is the 10k resistor definitely going to 3.3V (not 5V)? IO35 is not 5V tolerant for long periods.
- Ethernet: Ensure your WT32-ETH01 is getting enough current. If the OLED flickers when Ethernet initializes, your USB port might not be outputting enough milliamps (Ethernet is power-hungry on ESP32).

## References

- [Unofficial Guide to the WT32-ETH01](https://github.com/egnor/wt32-eth01)
- [How to Use wt32-eth01: Examples, Pinouts, and Specs](https://docs.cirkitdesigner.com/component/19888585-1942-4683-b24c-de1a4e53a293/wt32-eth01)
- [Getting Started with VS Code and PlatformIO IDE for ESP32 and ESP8266](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)
