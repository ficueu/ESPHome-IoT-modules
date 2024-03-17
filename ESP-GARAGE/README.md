## ESP-GARAGE: Moduł nadzorujący/wyzwalający sterownik bramy garażowej

 <img src="https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-GARAGE/Images/esp-garage1.jpg" width=30% height=30%>


### Specyfikacja:
* ESP32C3,
* zasilanie DC: 7-24V,
* zasilanie AC: 7-20V,
* 2x przekaźniki NO 1A@24VDC/0,5A@125VAC,
* 2x wejścia izolowane 3-24VDC,
* wyjście LED adresowalnych WS2812B i podobnych,
* zasilanie 5V 500mA dla LEDów,
* złącze USBC do programowania,
* działa pod kontrolą ESPHome,
* obudowa wykonana w technologii druku 3D.


## Przykłady zastosowań:
* rozszerzenie możliwości fabrycznych sterowników bram garażowych o sterowanie oraz raportowanie zamknięcia przez WiFi z poziomu HA (np. FAAC),
* integracja z centralami alarmowymi przez wejścia/wyjścia binarne (np. SATEL CA6/CA10/Perfecta).


Pinout:
```
1: AC/DC (zasilanie)
2: AC/DC (zasilanie)
3: LED (wyjście dla LED adresowalnych)
4: 5V (zasilanie dla LED)
5: GND (masa dla LED)
6: IN1 (wejście 3-24VDC)
7: IN2 (wejście 3-24VDC)
8: GND IN (masa dla wejść IN1/IN2)
9: RL1 (przekaźnik 1)
10: COM1 (przekaźnik 1)
11: RL2 (przekaźnik 2)
12: COM2 (przekaźnik 2)
```
