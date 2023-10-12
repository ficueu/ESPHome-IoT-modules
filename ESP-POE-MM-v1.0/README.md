# ESP-POE-MM-v1.0
ESP32 POE: ZigBee + BLE gateway + 2xRS485 + CAN


 <img src="https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/Images/esp-poe-mm1.jpg" width=30% height=30%>


Features:
* ESPHome compatible,
* ESP32-S (with U.FL connector) used as BLE receiver, main gateway controller,
* Ebyte E72-2G4M20S1E (CC2652p with U.FL connector) as ZigBee coordinator (on external board),
* POE 802.3af/802.3at (36-57 VDC) with LAN8720,
* passive PoE 12-57V (experimental, need to solder jumper on PCB),
* USBC for flashing, based on CH340C (not recommended for powering),
* power connector: 10-57 VDC (abs max 70V - needs to replace capacitor),
* external connectors: 10xSCREW TERMINALS (3 fixed for: GND, 5V and 3.3V, others for GPIO or RS485/CAN),
* internal connectors: 10xGPIO (4, 5, 13, 14, 15, 16, 32, 33, 34, 35)
* support for 2xRS485/MODBUS (extended version),
* support for CAN (extended version),
* galvanic separation from PoE or screw terminal voltage input,
* LEDs: power (green) and status (amber),
* external antennas for WiFi/BLE and ZigBee




### DO NOT POWER ON MODULE WITHOUT ANTENNAS


## USAGE / CONFIGURATION
Module is ready to use, you can simply plug in into PoE, configure Z2M or ZHA with right IP address and add device to HA via ESPHome integration (device should be automatically recognized by Home Assistant).

Extended setup:

1. Add ESPHome addon to your Home Assistant.
2. Plug device via PoE to network.
3. Click [b]Adopt[/b] in your ESPHome addon (device should be named ESP32 PoE Multi Gateway).
4. Click [b]Install[/b] and wait until flashing is done.
5. Click [b]LOGS[/b] and search for IP address for this device.
6. Add Zigbee2MQTT addon to your Home Assistant.
7. Change config for Zigbee2MQTT addon in [b]serial:[/b] to (note! change only YOUR-IP, ports should be always :1234):
```
  port: tcp://YOUR-IP:1234
```
8. Restart Zigbee2MQTT addon.
9. Add device via Home Assistant integration to turn on bluetooth proxy.


## ESPHome yaml config file


Example ESPHome yaml file (RS485 and CAN): https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/esp-poe-mm-v10.yaml


Example ESPHome yaml file (ZigBee): https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/esp-poe-mm-v10-zb.yaml


Latest ESPHome ZigBee yaml: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/esp-poe-mm-v12-zb.yaml
Changelog:
v12:
- changed framework to esp-idf for better performance;
- to flash this device with this framework you need to connect device via USB to PC (OTA update might brick device);
v11:
- configuration cleanup;
v10:
- initial release.


![alt text](https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/images/ESP-POE-MM-v1.0-DESC.png)


ORANGE: solder jumpers with 3 pads, connect middle with left or right pad to use right signal (all jumpers has description eg. A|32 - if you want to use signal 32 - solder middle and right pad, to use A signal - solder middle and left pad).


BLUE: termination jumpers - solder jumper to enable 120 Ohm termination on bus.


BROWN: enable jumpers - if you want to use GPIO pins which are shared with transceivers - please disconnect the jumpers.


YELLOW: passive PoE enable jumper.


Pinout (top screw terminal connector):
```
1: GND
2: +3.3V
3: +5V
4: GPIO13/GPIO14
5: GPIO5/CAN H
6: CAN L/GPIO4
7: GPIO16/RS B (RS1)
8: RS A (RS1)/GPIO15
9: GPIO33/RS B (RS2)
10: RS A (RS2)/GPIO32
```


Pinout (bottom screw terminal connector):
```
1: VCC (INPUT: 10-57 VDC)
2: GND
```


Pinout (ESP32 side):
```
status_led:
  pin:
    number: 2
    inverted: true
   
ethernet:
  type: LAN8720
  mdc_pin: GPIO23
  mdio_pin: GPIO18
  clk_mode: GPIO17_OUT
  phy_addr: 1
  power_pin: GPIO12
 
 
#UART Settings
uart:
  - id: RS1
    rx_pin: GPIO34
    tx_pin: GPIO15
    baud_rate: 9600


  - id: RS2
    rx_pin: GPIO39
    tx_pin: GPIO14
    baud_rate: 9600      


  - id: ZIGBEE
    rx_pin: GPIO34
    tx_pin: GPIO15
    baud_rate: 115200


modbus:
  - id: MODBUS1
    flow_control_pin: 13  
    uart_id: RS1
  - id: MODBUS2
    flow_control_pin: 16  
    uart_id: RS2


canbus:
  - platform: esp32_can
    tx_pin: GPIO5
    rx_pin: GPIO4
    can_id: 1
    bit_rate: 500kbps
 
```


ZigBee2MQTT config:


```
serial:
  port: tcp://YOUR-IP:1234
```


Updating ZigBee firmware:
1. Read https://www.zigbee2mqtt.io/guide/adapters/flashing/flashing_via_cc2538-bsl.html
2. Prepare cc2538-bsl tool.
3. Turn on "Zigbee BSL" on your EespHome device integration.
4. Wait 10s.
5. Run cc2538-bsl:
```
python cc2538-bsl.py -b 115200 -p socket://IP-OF-DEVICE:1234 -evw FILENAME.HEX"
```
eg (for IP: 192.168.0.126 and filename: CC1352P2_CC2652P_other_coordinator_20220219.hex):
```
python cc2538-bsl.py -b 115200 -p socket://192.168.0.126:1234 -evw CC1352P2_CC2652P_other_coordinator_20220219.hex
```
6. Wait until firmware was upload and verified successfully, it can take few minutes.


Successfully flashing process:
```
Opening port socket://192.168.0.126:1234, baud 115200
Reading data from CC1352P2_CC2652P_other_coordinator_20220219.hex
Your firmware looks like an Intel Hex file
Connecting to target...
CC1350 PG2.0 (7x7mm): 352KB Flash, 20KB SRAM, CCFG.BL_CONFIG at 0x00057FD8
Primary IEEE Address: XX:XX:XX:XX:XX:XX:XX:XX
    Performing mass erase
Erasing all main bank flash sectors
    Erase done
Writing 360448 bytes starting at address 0x00000000
Write 104 bytes at 0x00057F988
    Write done
Verifying by comparing CRC32 calculations.
    Verified (match: 0x9f4c5825)
```


If error is occured "ERROR: Timeout waiting for ACK/NACK after 'Send data (0x24)'" back to step 3. and try again.








