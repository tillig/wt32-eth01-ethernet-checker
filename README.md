# WT32-ETH01 Ethernet Checker

A standalone tool to verify network connectivity. It checks DHCP, Internet (ICMP), and Web (HTTP) access.

When you plug it in, it will test...

- **DHCP:** Verifies network layer and cable integrity.
- **PING:** Verifies routing to the wider internet.
- **HTTP:** Verifies that port 80 traffic isn't blocked by firewalls.

If any of those things fail, it'll tell you which one.

## Parts

- [WT32-ETH01](https://amzn.to/4bPwoAS) - $17
- [0.91" I2C IIC 128x32 OLED display](https://amzn.to/3NF82zR) - $12 for 3
- [USB 3.1 female breakout board](https://amzn.to/4mrg90i) - $9 for 10
- [2-pin normally open button](https://amzn.to/4cDuVxz) - $6.50 for 12
- [10K resistor](https://amzn.to/4svy7jM) - $5.50 for a variety pack of 1000

Prices listed are as of this writing. Basically, while there are devices out there that do this same function, they're way more expensive than the parts for this one.

## Hardware Wiring (Production)

| Component    | Pin A    | Pin B   | Notes               |
| :----------- | :------- | :------ | :------------------ |
| USB-C Power  | VCC (5V) | 5V Pin  | Main Power          |
| USB-C Power  | GND      | GND Pin | Ground              |
| 0.91" OLED   | SDA      | IO33    | I2C Data            |
| 0.91" OLED   | SCL      | IO32    | I2C Clock           |
| Button       | Pin      | IO35    | Signal (Active LOW) |
| 10k Resistor | IO35     | 3V3 Pin | Pull-up Resistor    |

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

## Programming Instructions

This project is done with [VS Code and PlatformIO](https://docs.platformio.org/en/latest/integration/ide/vscode.html).

1. Set up the ESP32 downloader with your WT32-ETH01.
2. Connect it via USB to your computer.
3. Open in VS Code/PlatformIO and click **Upload**.
4. Remove the downloader and connect the components for standalone USB-C operation.

## References

- [Unofficial Guide to the WT32-ETH01](https://github.com/egnor/wt32-eth01)
- [How to Use wt32-eth01: Examples, Pinouts, and Specs](https://docs.cirkitdesigner.com/component/19888585-1942-4683-b24c-de1a4e53a293/wt32-eth01)
- [Getting Started with VS Code and PlatformIO IDE for ESP32 and ESP8266](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)
