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
