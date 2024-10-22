## ESP-INTF: Moduł komunikacyjny CAN, RS485(MODBUS), 2xBIN

 <img src="https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-INTF/Images/esp-intf1.jpg" width=30% height=30%>


### Specyfikacja:
* ESP32C3,
* zasilanie 5-24VDC,
* 1x RS485 (MODBUS),
* 1x CAN,
* 2x wejścia izolowane 3-24VDC,
* złącze USBC do programowania,
* działa pod kontrolą ESPHome,
* obudowa wykonana w technologii druku 3D.


## Przykłady zastosowań:
* bramka MODBUS (falowniki, pompy ciepła, rekuperatory, itp.),
* bramka CAN (falowniki, pompy ciepła, rekuperatory, samochody, itp.).


Pinout:
```
1: VIN (5-24VDC)
2: GND (masa zasilania)
3: IN1 (wejście 3-24VDC)
4: IN2 (wejście 3-24VDC)
5: GND IN (masa dla wejść IN1/IN2)
6: CAN H
7: CAN L
8: GND 
9: RS485 A 
10: RS485 B
```

## ESPHome yaml config file

Example ESPHome yaml file: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-INTF/esp-intf-v10.yaml

Compiled binary: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-INTF/esp-intf-v10-factory-2024.10.0.bin

### HOW TO USE?

1. Install and configure ESPHome addon for Home Assistant: https://esphome.io/guides/getting_started_hassio.html#installing-esphome-dashboard

2. Connect the device to PC/power adapter via USB cable, find network named ESP-INTF and connect to it with password: 12345678.

3. Open the web browser and go to 192.168.4.1 and fill your SSID and password, click on save.

4. Your device should be visible on ESPHome addon with name and green button "ADOPT" - click on this button, install device and flash it.
If the device can not be adopted please create a new device (name and device type doesn't matter, you can choose esp32 with recommended settings), next edit the device and replace config with Example ESPHome yaml file, next upload configuration via usb cable.



