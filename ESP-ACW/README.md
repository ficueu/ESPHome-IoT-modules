# ESP-ACW

## ESP-ACW: bezprzewodowy moduł do sterowania klimatyzacją bez chmury producenta (USB/iR)

![alt text](https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-ACW/images/img1.jpg)

### Funkcje i cechy urządzenia:
* zgodny z ESPHome,
* zasilanie bezpośrednio ze złącza klimatyzatora,
* zgodny z większością klimatyzatorów wyposażonych w złącze USB (pochodne Midea i Tuya oraz Haier, Kaisai, Rotenso, Beko, Electrolux, Samsung) oraz ze złączem 4 pin (nowe Haier Flexis itp.),
* możliwość podłączenia diody iR oraz odbiornika iR do sterowania starszych klimatyzatorów nie wyposażonych w złącze USB,
* złącze USB-C do programowania,
* możliwość uruchomienia BTProxy (ESPHome),
* LED statusu urządzenia.

> [!TIP]
> Domyślna konfiguracja WiFi **SSID: ESP-ACW PASS: 12345678**
>
> Urządzenie jest wstępnie zaprogramowane. Po połączeniu się z urządzeniem przez WiFi można podać dane naszej sieci WiFi abu urządzenie się z nią połączyło. Jeśli wszystko przebiegnie pomyślnie to urządzenie powinno być widoczne w dodatku ESPHome z opcją **ADOPT**, po wybraniu tej opcji automatycznie zostanie pobrane oprogramowanie i sterownik zostanie skonfigurowany do działania.

Domyślny yaml dla urządzeń typu Midea: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-ACW/esp-acw-midea.yaml

Domyślny yaml dla urządzeń typu Tuya: https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-ACW/esp-acw-tuya.yaml

Domyślny yaml dla urządzeń Haier (smartair, zazwyczaj złącze USB): https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-ACW/esp-acw-haier-smartair.yaml

Domyślny yaml dla urządzeń Haier (hOn, złącze 4 pin): https://github.com/ficueu/ESPHome-IoT-modules/blob/main/ESP-ACW/esp-acw-haier.yaml
