# ESP-RGBWW

## ESP33-RGBWW: 5 kanałowy sterownik LED działający pod kontrolą ESPHome/WLED (WiFi).

![alt text](https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-RGBWW/images/img2.jpg)

### Moduły są na bieżąco aktualizowane i modyfikowane przez co zyskują nowe funkcjonalności, poniżej dostępna jest rozwijana lista z opisami poszczególnych rewizji HW.

<details>
<summary> RGBWW v.1.1 (aktualna)</summary>

### Funkcje i cechy sterownika:
* zgodny z ESPHome/WLED,
* 5 wyjść wysokoprądowych na złączach śrubowych (mosfet typu n),
* 4 wejścia/wyjścia GPIO na listwie kołkowej (przyciski, sensory, czujniki ruchu, LEDy adresowalne),
* GND, 3.3V (200mA), 5V (200mA) na listwie kołkowej (do zasilania czujników),
* możliwość zasilenia logiki z zewnętrznego zasilacza o dużej sprawności (7-24V),
* złącze USB-C do programowania,
* możliwość uruchomienia BTProxy (ESPHome),
* LED statusu urządzenia,
* maksymalne obciążenie sumaryczne 12A, ale nie więcej niż 4A na kanał,
* zasilanie 7-24V,
* pomiar prądu i napięcia,
* wymiary: 44x48x15 [mm].

Domyślny yaml: https://github.com/ficueu/ESPHome-IoT-modules/tree/main/ESP-RGBWW/esp-rgbww-v11.yaml

Domyślny plik bin: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-RGBWW/esp-rgbww-v11-factory-2024.2.2.bin

### Pinout:

![alt text](https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-RGBWW/images/pcb1.png)


```
OUT1 - GPIO07
OUT2 - GPIO03
OUT3 - GPIO10
OUT4 - GPIO05
OUT5 - GPIO04

IN1 - GPIO06
IN2 - GPIO21
IN3 - GPIO20
IN4 - GPIO02*

LED - GPIO10
```

</details>
<details>
<summary> RGBWW v.1.0</summary>

### Funkcje i cechy sterownika:
* zgodny z ESPHome/WLED,
* 5 wyjść wysokoprądowych na złączach śrubowych (mosfet typu n),
* 4 wejścia/wyjścia GPIO na listwie kołkowej (przyciski, sensory, czujniki ruchu, LEDy adresowalne),
* GND, 3.3V (200mA), 5V (200mA) na listwie kołkowej (do zasilania czujników),
* możliwość zasilenia logiki z zewnętrznego zasilacza o dużej sprawności (7-24V),
* złącze USB-C do programowania,
* możliwość uruchomienia BTProxy (ESPHome),
* LED statusu urządzenia,
* maksymalne obciążenie sumaryczne 12A, ale nie więcej niż 4A na kanał,
* zasilanie 7-24V,
* wymiary: 44x48x15 [mm].

Domyślny yaml: https://github.com/ficueu/ESPHome-IoT-modules/tree/main/ESP-RGBWW/esp-rgbww-v10.yaml

Domyślny plik bin: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-RGBWW/esp-rgbww-v10-factory-20231102.bin

### Pinout:

![alt text](https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-RGBWW/images/pcb1.png)


```
OUT1 - GPIO00
OUT2 - GPIO01
OUT3 - GPIO03
OUT4 - GPIO04
OUT5 - GPIO05

IN1 - GPIO06
IN2 - GPIO21
IN3 - GPIO20
IN4 - GPIO07

LED - GPIO10
```

</details>

### Przykładowa konfiguracja sterownika:

1. Przykład 1:
    - 1x LED 12V RGB (OUT1-3),
    - 2x LED 12V W (OUT4-5),
    - 1x LED 5V WS2812B (IN4),
    - 1x LD2410 (IN1-2).
2. Przykład 2:
    - 5x LED 24V W (OUT1-5),
    - 1x LED 5V WS2812B (IN4),
    - 1x DS18B20 (IN3),
    - 1x TTP223 (IN2).

> [!TIP]
> Domyślna konfiguracja WiFi **SSID: ESP-RGBWW PASS: 12345678**
>
> Urządzenie jest wstępnie zaprogramowane. Po połączeniu się z urządzeniem przez WiFi można podać dane naszej sieci WiFi abu urządzenie się z nią połączyło. Jeśli wszystko przebiegnie pomyślnie to urządzenie powinno być widoczne w dodatku ESPHome z opcją **ADOPT**, po wybraniu tej opcji automatycznie zostanie pobrane oprogramowanie i sterownik zostanie skonfigurowany do działania.

Plik **.bin** można wgrać przez https://web.esphome.io



