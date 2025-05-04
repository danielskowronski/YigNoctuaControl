# YigNoctuaControl

Custom overkill 5V fan controller for active cooling of various electronic devices (like OrangePi). It's based on ESP32 and Arduino core. Features two independent channels, each controls one PWM (4-pin) fan and gets data from two DS18B20 temperature sensors. Additionally, it has an ambient temperature sensor (AHT22) and an OLED display.

This project is not affiliated with Noctua, but uses their fans due to superb quality (controllable directly from GPIO and using USB 5V rail) and low noise.

**This is work in progress.**

This repository includes source code (Platform.io), hardware description with boards layout (DIYLC) and 3D-printed case.
