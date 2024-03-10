# ESP-POE-MM-v1.0

## ESP-POE-MM: Bramka multiprotokołowa (wM-BUS/BLE)/koordynator ZigBee LAN z PoE.

 <img src="https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/Images/esp-poe-mm1.jpg" width=30% height=30%>


### Wersje sprzętowe:

<details>
<summary> ESP-POE-MM-v1.3 [PL] (aktualna)</summary>


### Funkcje:
* ESP32 z modułem LAN8720,
* zasilanie PoE 802.3af/802.3at (36-57 VDC),
* zasilanie PoE pasywne 12-35V (po zlutowaniu zworki),
* zasilanie DC (złącze śrubowe) 10-57V,
* Złącze USBC do programowania,
* BT proxy lub natywna obsługa urządzeń BT/BLE przez komponenty esphome,
* koordynator ZigBee na układzie CC2652p (do działania wymagany dodatek Z2M/ZHA),
* odbiornik wM-BUS CC1101 (na specjalne zamówienie, wyłącza możliwość używania BT proxy),
* separacja galwaniczna POE <-> peryferia,
* możliwość skonfigurowania 2x MODBUS + 1xCAN (na specjalne zamówienie),
* dedykowane zewnętrzne anteny dla ZigBee, BLE/wM-BUS,
* diody zasilania (czerwona), aktywności (niebieska),
* zewnętrzne złącza śrubowe do podłączenia zewnętrznych czujników itp.,
* wewnętrzne złacza: 10xGPIO (4, 5, 13, 14, 15, 16, 32, 33, 34, 35)
* obudowa wykonana w technologii druku 3D.

Wersja 1.3. posiada wlutowany konwerter RS485 (MODBUS), który może być wykorzystany w przypadku konfiguracji urządzenia jako BLE + ZigBee.

Dla wersji BLE + ZigBee + RS485, na złączu śrubowym, dostępne dla użytkownika są GND, 3.3V, 5V, GPIO4, GPIO5 oraz sygnały A i B dla magistrali RS485.

### NIE NALEŻY URUCHAMIAĆ MODUŁU BEZ PRZYKRECONYCH ANTEN!

## Pliki konfiguracyjne yaml dla ESPHome:

Najnowsza konfiguracja: (BLE+ZigBee+RS485): https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/esp-poe-mm-v13-zb-rs.yaml

Plik bin: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/esp-poe-mm-v13-zb-rs-factory-2024.2.2.bin

<img src=https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/images/ESP-POE-MM-v1.0-DESC.png width=50% height=50%>


NIEBIESKI: zworki terminatorów 120Ohm - zlutowanie zworki aktywuje terminację linii.

ŻÓŁTY: zworka do aktywacji pasywnego PoE (JP3)


Pinout (górne złącze śrubowe):
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


Pinout (dolne złącze śrubowe):
```
1: VCC (INPUT: 10-57 VDC)
2: GND
```


Pinout (konfiguracyjny):
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
    rx_pin: GPIO39
    tx_pin: GPIO14
    baud_rate: 9600

  - id: RS2
    rx_pin: GPIO34
    tx_pin: GPIO15
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


Konfiguracja Z2M:


```
serial:
  port: tcp://YOUR-IP:1234
```

</details>

<details>
<summary> ESP-POE-MM-v1.2 [ENG]</summary>


### Features:
* ESPHome compatible,
* ESP32-S (with U.FL connector) used as BLE receiver, main gateway controller,
* Ebyte E72-2G4M20S1E (CC2652p with U.FL connector) as ZigBee coordinator (on external board),
* POE 802.3af/802.3at (36-57 VDC) with LAN8720,
* passive PoE 12-57V (experimental, need to solder jumper on PCB),
* USBC for flashing, based on CH340C (not recommended for powering),
* power connector: 10-57 VDC (abs max 70V - needs to replace capacitor),
* external connectors: 10xSCREW TERMINALS (3 fixed for: GND, 5V and 3.3V, others for GPIO - they can be shared with Zigbee module or RS485/CAN),
* internal connectors: 10xGPIO (4, 5, 13, 14, 15, 16, 32, 33, 34, 35)
* support for 2xRS485/MODBUS (extended version),
* support for CAN (extended version),
* galvanic separation from PoE or screw terminal voltage input,
* LEDs: power (green) and status (amber),
* external antennas for WiFi/BLE and ZigBee

### DO NOT POWER ON MODULE WITHOUT ANTENNAS

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

<img src=https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-POE-MM-v1.0/images/ESP-POE-MM-v1.0-DESC.png width=50% height=50%>


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
    rx_pin: GPIO39
    tx_pin: GPIO14
    baud_rate: 9600

  - id: RS2
    rx_pin: GPIO34
    tx_pin: GPIO15
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

</details>

### Instrukcje

<details>
<summary>INSTRUKCJA [PL]</summary>


Moduł jest gotowy do działania, domyślnie włączony jest klient DHCP. Wystarczy podłączyć moduł do switcha PoE, podać adres modułu w konfiguracja Z2M lub ZHA wraz z portem **1234** oraz dodać urządzenie przez integrację ESPHome (urządzenie zostanie wykryte automatycznie).

Dodatkowo można dodać urządzenie do listy urządzeń w dodatku ESPHome - umożliwi to dokładną konfigurację modułu oraz aktualizowanie jego oprogramowania.
1. Zainstaluj dodatek ESPHome: https://my.home-assistant.io/redirect/supervisor_addon/?addon=5c53de3b_esphome&repository_url=https%3A%2F%2Fgithub.com%2Fesphome%2Fhome-assistant-addon
2. Uruchom dodatek i przejdź do jego interfejsu.
3. Kliknij **Adopt** przy nowo wykrytym urządzeniu [ESP32 PoE Multi Gateway v1.3 (ZIGBEE+RS485)].
4. Kliknij **Install** i poczekaj aż oprogramowanie zostanie zainstalowane na urządzeniu.

Konfiguracja dla Z2M (zamień IP-URZADZENIA na adres IP):

```
  port: tcp://IP-URZADZENIA:1234
```

Alternatywnie możesz skorzystać z aliasu adresu IP (nie zawsze ta metoda będzie działała):

```
  port: tcp://esp-poe-mm-v13-zb-rs.local:1234
```


</details>

<details>
<summary>MANUAL [EN]</summary>


Module is ready to use, you can simply plug in into PoE, configure Z2M or ZHA with right IP address and add device to HA via ESPHome integration (device should be automatically recognized by Home Assistant).

Extended setup:

1. Add ESPHome addon to your Home Assistant.
2. Plug device via PoE to network.
3. Click **Adopt** in your ESPHome addon (device should be named ESP32 PoE Multi Gateway).
4. Click **Install** and wait until flashing is done.
5. Click **LOGS** and search for IP address for this device.
6. Add Zigbee2MQTT addon to your Home Assistant.
7. Change config for Zigbee2MQTT addon in **serial:** to:
(note! change only YOUR-IP, port should be always :1234)
```
  port: tcp://YOUR-IP:1234
```
8. Restart Zigbee2MQTT addon.
9. Add device via Home Assistant integration to turn on bluetooth proxy.



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

</details>






