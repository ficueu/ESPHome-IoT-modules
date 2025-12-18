# ESP-MBUS-v1.0
ESP32C3-MBUS: wM-BUS based on CC1101 gateway

Thanks to SzczepanLeon for ESPHome wmbus component https://github.com/SzczepanLeon/esphome-components

Features:
* ESPHome compatible,
* BTproxy,
* high quality CC1101 module with shielding,
* onboard RP-SMA connector,
* USBC for flashing and powering,
* LED: status (green).


### DO NOT POWER ON MODULE WITHOUT ANTENNAS

## ESPHome yaml config file

Example ESPHome yaml file: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-MBUS/esp-mbus-v10.yaml

Compiled binary: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-MBUS/esp-mbus-v10.bin

### HOW TO USE?

1. Install and configure ESPHome addon for Home Assistant: https://esphome.io/guides/getting_started_hassio.html#installing-esphome-dashboard

2. Connect the device to PC/power adapter via USB cable, find network named ESP-MBUS and connect to it with password: 12345678.

3. Open the web browser and go to 192.168.4.1 and fill your SSID and password, click on save.

4. Your device should be visible on ESPHome addon with name and green button "TAKE CONTROL" - click on this button, install device and flash it.
If the device can not be taken control of, please create a new device (name and device type doesn't matter, you can choose esp32 with recommended settings), next edit the device and replace config with Example ESPHome yaml file, next upload configuration via usb cable.

5. This device is based on SzczepanLeon wmbus component, you can find example configuration here: https://github.com/SzczepanLeon/esphome-components



