# Project Context: WT32-ETH01 Ethernet Checker

This project is an ESP32-based Ethernet testing tool using the WT32-ETH01 board and PlatformIO.

## Hardware Specifications

- **Board:** WT32-ETH01 (Internal PHY: LAN8720)
- **Display:** 0.91" I2C OLED (SSD1306, 128x32)
- **OLED I2C Pins:** SDA = IO14, SCL = IO15 (Important: IO32/33 are not broken out on this board)
- **Input Pin:** IO35 (Manual Button, 10k external pull-up to 3.3V)
- **Ethernet:** LAN8720 (PHY Addr 1, Power Pin 16, MDC 23, MDIO 18)
- **Power:** Production via USB-C (5V), Programming via M5Stack Downloader.

## Software Logic Flow

1. **DHCP:** Acquire IP from network. Timeout after 12s.
2. **Ping:** Ping Google DNS (8.8.8.8).
3. **HTTP:** GET request to `http://connectivitycheck.gstatic.com/generate_204`.
4. **Manual Trigger:** If IO35 is pulled LOW, re-run the test sequence.

## UI Style

- Resolution is 128x32.
- Display a progress bar (4px height) at the bottom during tests.
- Show "Success" or specific "Error" messages clearly on screen.
