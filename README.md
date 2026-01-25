# Ethernet Checker

Simple client device using a WT32-ETH01 to check whether a given Ethernet connection is working.

When you plug it in, it will...

- Get a DHCP connection.
- Try to get the Google home page.

If any of those things fail, it'll tell you which one.

I made this because I was trying to troubleshoot my parents' network and, depending on the location, we could _maybe_ get a DHCP connection but not get out of the network; or maybe we wouldn't even get DHCP. It was tiring to lug a laptop around just to try the network so I thought a little checker like this could be handy.

## Parts

- [WT32-ETH01](https://amzn.to/4bPwoAS) - $17
- [0.91" I2C IIC 127x32 OLED display](https://amzn.to/3NF82zR) - $12 for 3

Prices listed are as of this writing. The point is that, while there are devices that do a far more robust job checking network connectivity, they're usually far more expensive than what I'm interested in paying for this.

## Programming

I used [this ESP32 downloader kit](https://shop.m5stack.com/products/esp32-downloader-kit).

Pin connections for USB programmer:

| USB Programmer | WT32-ETH01                 |
| -------------- | -------------------------- |
| GND            | GND (fourth down on right) |
| G0             | IO0 (third down on right)  |
| EN             | EN (first on left)         |
| TXD            | RXD (second down on right) |
| RXD            | TXD (first on left)        |
| 3.3V           | (10th down on left)        |

This project is done with [VS Code and PlatformIO](https://docs.platformio.org/en/latest/integration/ide/vscode.html)

## References

- [Unofficial Guide to the WT32-ETH01](https://github.com/egnor/wt32-eth01)
- [How to Use wt32-eth01: Examples, Pinouts, and Specs](https://docs.cirkitdesigner.com/component/19888585-1942-4683-b24c-de1a4e53a293/wt32-eth01)
- [Getting Started with VS Code and PlatformIO IDE for ESP32 and ESP8266](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)
