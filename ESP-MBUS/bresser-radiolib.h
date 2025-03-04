/*
* na podstaw https://github.com/matthias-bs/Bresser_Weather_Sensor_CC1101_MQTT
* dodatek (inccule) do ESPHome
* odbioru danych z staji pogodowej Bresser
* 
*/

// Comment out BRESSER_6_IN_1 to use decodeBresser5In1Payload()
// Uncomment   BRESSER_6_IN_1 to use decodeBresser6In1Payload()
#define BRESSER_5_IN_1

// Print misc debug output
#define _DEBUG_MODE_

#undef delay(x)
#undef yield(x)
#undef  millis(x) 
#undef micros(x) 
#undef delayMicroseconds(x) 

// Enable RadioLib internal debug messages
//#define RADIOLIB_DEBUG

#include "esphome.h"
//#include "Arduino.h"

#include "RadioLib.h"

//#define RADIOLIB_BUILD_ARDUINO
//#define xstr(s) str(s)
//#define str(s) #s


// https://docs.openmqttgateway.com/setitup/rf.html#pinout
// Board   Receiver Pin(GDO2)  Emitter Pin(GDO0)   SCK   VCC   MOSI  MISO  CSN   GND
// ESP8266   D2/D3/D1/D8            RX/D2          D5    3V3   D7    D6    D8    GND
// ESP32         D27                 D12           D18   3V3   D23   D19   D5    GND

// Note: GDO2 does not seem to be used.

#if defined(ESP32)
    #define PIN_CC1101_CS   7
    #define PIN_CC1101_GDO0 3
    #define PIN_CC1101_GDO2 1
#elif defined(ESP8266)
    #define PIN_CC1101_CS   2
    #define PIN_CC1101_GDO0 4
    #define PIN_CC1101_GDO2 5
#endif


// Generate CC1101 radio module instance
CC1101 radio = new Module(PIN_CC1101_CS, PIN_CC1101_GDO0, RADIOLIB_NC, PIN_CC1101_GDO2);

static const char *TAG = "Bresser";

class Bresser : public PollingComponent  {
    public:

    Sensor *temperature_sensor = new Sensor();
    Sensor *humidity_sensor = new Sensor();
    Sensor *wind_avg_sensor = new Sensor();
    Sensor *wind_gust_sensor = new Sensor();
    Sensor *wind_dir_sensor = new Sensor();
    Sensor *rain_sensor = new Sensor();
    Sensor *battery_sensor = new Sensor();
    Bresser() : PollingComponent(5000) { }

//    uint32_t t1s = 0;
//    uint32_t sec = 0;
    bool decode_ok = false;

    
    void setup() override {
        ESP_LOGD(TAG, "setup");
        SPI.begin(6, 4, 5, 7);
        SPI.setFrequency(4000000);
        int state = radio.begin(868.35, 8.22, 57.136417, 270.0, 10, 32);
        if (state == RADIOLIB_ERR_NONE) {
            ESP_LOGD(TAG,"success!");
            state = radio.setCrcFiltering(false);
            if (state != RADIOLIB_ERR_NONE) {
                ESP_LOGD(TAG, "Error disabling crc filtering: [%d]", state);
    //            while (true) ;
            }
            state = radio.fixedPacketLengthMode(27);
            if (state != RADIOLIB_ERR_NONE) {
                ESP_LOGD(TAG, "Error setting fixed packet length: [%d]", state);
    //            while (true)  ;
            }
            // Preamble: AA AA AA AA AA
            // Sync is: 2D D4 
            // Preamble 40 bits but the CC1101 doesn't allow us to set that
            // so we use a preamble of 32 bits and then use the sync as AA 2D
            // which then uses the last byte of the preamble - we recieve the last sync byte
            // as the 1st byte of the payload.
            state = radio.setSyncWord(0xAA, 0x2D, 0, false);
            if (state != RADIOLIB_ERR_NONE) {
                ESP_LOGD(TAG, "Error setting sync words: [%d]", state);
    //            while (true)  ;
            }
        } else {
            ESP_LOGD(TAG, "Error initialising: [%d]", state);
            //while (true) ;
        }
        ESP_LOGD(TAG, "Setup complete - awaiting incoming messages...");
    }

    void loop() override {
//            uint32_t mil = esphome::millis();
//            if (mil - t1s > 1000){
//                t1s = mil; sec++;  ESP_LOGD(TAG,"loop %d", sec);
//            }            
        decode_ok = decode_ok or getWeatherdata();
    }

    void update() override {

        if (decode_ok) {
            if (weatherData.temp_ok){
                temperature_sensor->publish_state(weatherData.temp_c);
                humidity_sensor->publish_state(weatherData.humidity);
            }
            if (weatherData.wind_ok){
                wind_avg_sensor->publish_state(weatherData.wind_avg_meter_sec);
                wind_gust_sensor->publish_state(weatherData.wind_gust_meter_sec);
                wind_dir_sensor->publish_state(weatherData.wind_direction_deg);
            }
            if (weatherData.rain_ok)
                rain_sensor->publish_state(weatherData.rain_mm);
            battery_sensor->publish_state(weatherData.battery_ok);
            decode_ok = false;
        }
    }    

    typedef enum DecodeStatus {
        DECODE_OK, DECODE_PAR_ERR, DECODE_CHK_ERR, DECODE_DIG_ERR
    } DecodeStatus;

    struct WeatherData_S {
        uint8_t  s_type;               // only 6-in1
        uint32_t sensor_id;            // 5-in-1: 1 byte / 6-in-1: 4 bytes
        uint8_t  chan;                 // only 6-in-1
        bool     temp_ok;              // only 6-in-1
        float    temp_c;
        int      humidity;
        bool     uv_ok;                // only 6-in-1
        float    uv;                   // only 6-in-1
        bool     wind_ok;              // only 6-in-1
        float    wind_direction_deg;
        float    wind_gust_meter_sec;
        float    wind_avg_meter_sec;
        bool     rain_ok;              // only 6-in-1
        float    rain_mm;
        bool     battery_ok;
        bool     moisture_ok;          // only 6-in-1
        int      moisture;             // only 6-in-1
    };

    typedef struct WeatherData_S WeatherData_t;

    // Weather data object
    WeatherData_t weatherData;



    //////////////////////////////////////////////////////

    #if (defined(CHECK_PUB_KEY) and defined(CHECK_CA_ROOT)) or (defined(CHECK_PUB_KEY) and defined(CHECK_FINGERPRINT)) or (defined(CHECK_FINGERPRINT) and defined(CHECK_CA_ROOT)) or (defined(CHECK_PUB_KEY) and defined(CHECK_CA_ROOT) and defined(CHECK_FINGERPRINT))
    #error "cant have both CHECK_CA_ROOT and CHECK_PUB_KEY enabled"
    #endif


    //
    // Generate sample data for testing MQTT publishing -
    // use with #define _DEBUG_MQTT_
    //
    void genData(WeatherData_t *pOut)
    {
        pOut->sensor_id = 0xff;
        pOut->temp_c = 22.2f;
        pOut->humidity = 55;
        pOut->wind_direction_deg = 333;
        pOut->wind_gust_meter_sec = 44.4f;
        pOut->wind_avg_meter_sec = 11.1f;
        pOut->rain_mm = 9.9f;
        pOut->battery_ok = true;
    }


    //
    // From from rtl_433 project - https://github.com/merbanan/rtl_433/blob/master/src/util.c
    //
    uint16_t lfsr_digest16(uint8_t const message[], unsigned bytes, uint16_t gen, uint16_t key)
    {
        uint16_t sum = 0;
        for (unsigned k = 0; k < bytes; ++k) {
            uint8_t data = message[k];
            for (int i = 7; i >= 0; --i) {
                // fprintf(stderr, "key at bit %d : %04x\n", i, key);
                // if data bit is set then xor with key
                if ((data >> i) & 1)
                    sum ^= key;

                // roll the key right (actually the lsb is dropped here)
                // and apply the gen (needs to include the dropped lsb as msb)
                if (key & 1)
                    key = (key >> 1) ^ gen;
                else
                    key = (key >> 1);
            }
        }
        return sum;
    }


    //
    // From from rtl_433 project - https://github.com/merbanan/rtl_433/blob/master/src/util.c
    //
    int add_bytes(uint8_t const message[], unsigned num_bytes)
    {
        int result = 0;
        for (unsigned i = 0; i < num_bytes; ++i) {
            result += message[i];
        }
        return result;
    }

    /*
    // Cribbed from rtl_433 project - but added extra checksum to verify uu
    //
    // Example input data:
    //   EA EC 7F EB 5F EE EF FA FE 76 BB FA FF 15 13 80 14 A0 11 10 05 01 89 44 05 00
    //   CC CC CC CC CC CC CC CC CC CC CC CC CC uu II SS GG DG WW  W TT  T HH RR  R Bt
    // - C = Check, inverted data of 13 byte further
    // - uu = checksum (number/count of set bits within bytes 14-25)
    // - I = station ID (maybe)
    // - G = wind gust in 1/10 m/s, normal binary coded, GGxG = 0x76D1 => 0x0176 = 256 + 118 = 374 => 37.4 m/s.  MSB is out of sequence.
    // - D = wind direction 0..F = N..NNE..E..S..W..NNW
    // - W = wind speed in 1/10 m/s, BCD coded, WWxW = 0x7512 => 0x0275 = 275 => 27.5 m/s. MSB is out of sequence.
    // - T = temperature in 1/10 °C, BCD coded, TTxT = 1203 => 31.2 °C
    // - t = temperature sign, minus if unequal 0
    // - H = humidity in percent, BCD coded, HH = 23 => 23 %
    // - R = rain in mm, BCD coded, RRxR = 1203 => 31.2 mm
    // - B = Battery. 0=Ok, 8=Low.
    // - S = sensor type, only low nibble used, 0x9 for Bresser Professional Rain Gauge
    //
    // Parameters:
    //
    // msg     - Pointer to message
    // msgSize - Size of message
    // pOut    - Pointer to WeatherData
    //
    // Returns:
    //
    // DECODE_OK      - OK - WeatherData will contain the updated information
    // DECODE_PAR_ERR - Parity Error
    // DECODE_CHK_ERR - Checksum Error
    //
    */
    DecodeStatus decodeBresser5In1Payload(uint8_t *msg, uint8_t msgSize, WeatherData_t *pOut) { 
        // First 13 bytes need to match inverse of last 13 bytes
        for (unsigned col = 0; col < msgSize / 2; ++col) {
            if ((msg[col] ^ msg[col + 13]) != 0xff) {
                ESP_LOGD(TAG,"%s: Parity wrong at %u\n", __func__, col);
                return DECODE_PAR_ERR;
            }
        }

        // Verify checksum (number number bits set in bytes 14-25)
        uint8_t bitsSet = 0;
        uint8_t expectedBitsSet = msg[13];

        for(uint8_t p = 14 ; p < msgSize ; p++) {
        uint8_t currentByte = msg[p];
        while(currentByte) {
            bitsSet += (currentByte & 1);
            currentByte >>= 1;
        }
        }

        if (bitsSet != expectedBitsSet) {
        ESP_LOGD(TAG,"%s: Checksum wrong actual [%02X] != expected [%02X]\n", __func__, bitsSet, expectedBitsSet);
        return DECODE_CHK_ERR;
        }

        pOut->sensor_id = msg[14];

        int temp_raw = (msg[20] & 0x0f) + ((msg[20] & 0xf0) >> 4) * 10 + (msg[21] &0x0f) * 100;
        if (msg[25] & 0x0f) {
            temp_raw = -temp_raw;
        }
        pOut->temp_c = temp_raw * 0.1f;

        pOut->humidity = (msg[22] & 0x0f) + ((msg[22] & 0xf0) >> 4) * 10;

        pOut->wind_direction_deg = ((msg[17] & 0xf0) >> 4) * 22.5f;

        int gust_raw = ((msg[17] & 0x0f) << 8) + msg[16];
        pOut->wind_gust_meter_sec = gust_raw * 0.1f;

        int wind_raw = (msg[18] & 0x0f) + ((msg[18] & 0xf0) >> 4) * 10 + (msg[19] & 0x0f) * 100;
        pOut->wind_avg_meter_sec = wind_raw * 0.1f;

        int rain_raw = (msg[23] & 0x0f) + ((msg[23] & 0xf0) >> 4) * 10 + (msg[24] & 0x0f) * 100;
        pOut->rain_mm = rain_raw * 0.1f;

        pOut->battery_ok = (msg[25] & 0x80) ? false : true;

        return DECODE_OK;
    }


    //
    // From from rtl_433 project - https://github.com/merbanan/rtl_433/blob/master/src/devices/bresser_6in1.c
    //
    /**
    Decoder for Bresser Weather Center 6-in-1.
    - also Bresser Weather Center 7-in-1 indoor sensor.
    - also Bresser new 5-in-1 sensors.
    - also Froggit WH6000 sensors.
    - also rebranded as Ventus C8488A (W835)
    - also Bresser 3-in-1 Professional Wind Gauge / Anemometer PN 7002531
    There are at least two different message types:
    - 24 seconds interval for temperature, hum, uv and rain (alternating messages)
    - 12 seconds interval for wind data (every message)
    Also Bresser Explore Scientific SM60020 Soil moisture Sensor.
    https://www.bresser.de/en/Weather-Time/Accessories/EXPLORE-SCIENTIFIC-Soil-Moisture-and-Soil-Temperature-Sensor.html
    Moisture:
        f16e 187000e34 7 ffffff0000 252 2 16 fff 004 000 [25,2, 99%, CH 7]
        DIGEST:8h8h ID?8h8h8h8h FLAGS:4h BATT:1b CH:3d 8h 8h8h 8h8h TEMP:12h 4h MOIST:8h TRAILER:8h8h8h8h4h
    Moisture is transmitted in the humidity field as index 1-16: 0, 7, 13, 20, 27, 33, 40, 47, 53, 60, 67, 73, 80, 87, 93, 99.
    {206}55555555545ba83e803100058631ff11fe6611ffffffff01cc00 [Hum 96% Temp 3.8 C Wind 0.7 m/s]
    {205}55555555545ba999263100058631fffffe66d006092bffe0cff8 [Hum 95% Temp 3.0 C Wind 0.0 m/s]
    {199}55555555545ba840523100058631ff77fe668000495fff0bbe [Hum 95% Temp 3.0 C Wind 0.4 m/s]
    {205}55555555545ba94d063100058631fffffe665006092bffe14ff8
    {206}55555555545ba860703100058631fffffe6651ffffffff0135fc [Hum 95% Temp 3.0 C Wind 0.0 m/s]
    {205}55555555545ba924d23100058631ff99fe68b004e92dffe073f8 [Hum 96% Temp 2.7 C Wind 0.4 m/s]
    {202}55555555545ba813403100058631ff77fe6810050929ffe1180 [Hum 94% Temp 2.8 C Wind 0.4 m/s]
    {205}55555555545ba98be83100058631fffffe6130050929ffe17800 [Hum 95% Temp 2.8 C Wind 0.8 m/s]
                                            TEMP  HUM
    2dd4  1f 40 18 80 02 c3 18 ff 88 ff 33 08 ff ff ff ff 80 e6 00 [Hum 96% Temp 3.8 C Wind 0.7 m/s]
    2dd4  cc 93 18 80 02 c3 18 ff ff ff 33 68 03 04 95 ff f0 67 3f [Hum 95% Temp 3.0 C Wind 0.0 m/s]
    2dd4  20 29 18 80 02 c3 18 ff bb ff 33 40 00 24 af ff 85 df    [Hum 95% Temp 3.0 C Wind 0.4 m/s]
    2dd4  a6 83 18 80 02 c3 18 ff ff ff 33 28 03 04 95 ff f0 a7 3f
    2dd4  30 38 18 80 02 c3 18 ff ff ff 33 28 ff ff ff ff 80 9a 7f [Hum 95% Temp 3.0 C Wind 0.0 m/s]
    2dd4  92 69 18 80 02 c3 18 ff cc ff 34 58 02 74 96 ff f0 39 3f [Hum 96% Temp 2.7 C Wind 0.4 m/s]
    2dd4  09 a0 18 80 02 c3 18 ff bb ff 34 08 02 84 94 ff f0 8c 0  [Hum 94% Temp 2.8 C Wind 0.4 m/s]
    2dd4  c5 f4 18 80 02 c3 18 ff ff ff 30 98 02 84 94 ff f0 bc 00 [Hum 95% Temp 2.8 C Wind 0.8 m/s]
    {147} 5e aa 18 80 02 c3 18 fa 8f fb 27 68 11 84 81 ff f0 72 00 [Temp 11.8 C  Hum 81%]
    {149} ae d1 18 80 02 c3 18 fa 8d fb 26 78 ff ff ff fe 02 db f0
    {150} f8 2e 18 80 02 c3 18 fc c6 fd 26 38 11 84 81 ff f0 68 00 [Temp 11.8 C  Hum 81%]
    {149} c4 7d 18 80 02 c3 18 fc 78 fd 29 28 ff ff ff fe 03 97 f0
    {149} 28 1e 18 80 02 c3 18 fb b7 fc 26 58 ff ff ff fe 02 c3 f0
    {150} 21 e8 18 80 02 c3 18 fb 9c fc 33 08 11 84 81 ff f0 b7 f8 [Temp 11.8 C  Hum 81%]
    {149} 83 ae 18 80 02 c3 18 fc 78 fc 29 28 ff ff ff fe 03 98 00
    {150} 5c e4 18 80 02 c3 18 fb ba fc 26 98 11 84 81 ff f0 16 00 [Temp 11.8 C  Hum 81%]
    {148} d0 bd 18 80 02 c3 18 f9 ad fa 26 48 ff ff ff fe 02 ff f0
    Wind and Temperature/Humidity or Rain:
        DIGEST:8h8h ID:8h8h8h8h FLAGS:4h BATT:1b CH:3d WSPEED:~8h~4h ~4h~8h WDIR:12h ?4h TEMP:8h.4h ?4h HUM:8h UV?~12h ?4h CHKSUM:8h
        DIGEST:8h8h ID:8h8h8h8h FLAGS:4h BATT:1b CH:3d WSPEED:~8h~4h ~4h~8h WDIR:12h ?4h RAINFLAG:8h RAIN:8h8h UV:8h8h CHKSUM:8h
    Digest is LFSR-16 gen 0x8810 key 0x5412, excluding the add-checksum and trailer.
    Checksum is 8-bit add (with carry) to 0xff.
    Notes on different sensors:
    - 1910 084d 18 : RebeckaJohansson, VENTUS W835
    - 2030 088d 10 : mvdgrift, Wi-Fi Colour Weather Station with 5in1 Sensor, Art.No.: 7002580, ff 01 in the UV field is (obviously) invalid.
    - 1970 0d57 18 : danrhjones, bresser 5-in-1 model 7002580, no UV
    - 18b0 0301 18 : konserninjohtaja 6-in-1 outdoor sensor
    - 18c0 0f10 18 : rege245 BRESSER-PC-Weather-station-with-6-in-1-outdoor-sensor
    - 1880 02c3 18 : f4gqk 6-in-1
    - 18b0 0887 18 : npkap

    Parameters:

    msg     - Pointer to message
    msgSize - Size of message
    pOut    - Pointer to WeatherData

    Returns:

    DECODE_OK      - OK - WeatherData will contain the updated information
    DECODE_DIG_ERR - Digest Check Error
    DECODE_CHK_ERR - Checksum Error

    */
    DecodeStatus decodeBresser6In1Payload(uint8_t *msg, uint8_t msgSize, WeatherData_t *pOut) {
        int const moisture_map[] = {0, 7, 13, 20, 27, 33, 40, 47, 53, 60, 67, 73, 80, 87, 93, 99}; // scale is 20/3
        
        // LFSR-16 digest, generator 0x8810 init 0x5412
        int chkdgst = (msg[0] << 8) | msg[1];
        int digest  = lfsr_digest16(&msg[2], 15, 0x8810, 0x5412);
        if (chkdgst != digest) {
            //decoder_logf(decoder, 2, __func__, "Digest check failed %04x vs %04x", chkdgst, digest);
            ESP_LOGD(TAG,"Digest check failed - %02X vs %02X",chkdgst,digest);
    //        ESP_LOGD(TAG, chkdgst, HEX);
    //        ESP_LOGD(TAG,F(" vs "));
    //        ESP_LOGD(TAG,digest, HEX);
            return DECODE_DIG_ERR;
        }
        // Checksum, add with carry
        int chksum = msg[17];
        int sum    = add_bytes(&msg[2], 16); // msg[2] to msg[17]
        if ((sum & 0xff) != 0xff) {
            //decoder_logf(decoder, 2, __func__, "Checksum failed %04x vs %04x", chksum, sum);
            ESP_LOGD(TAG,"Checksum failed -  %02X vs %02X",chksum,HEX);
    //        ESP_LOGD(TAG,chksum, HEX);
    //        ESP_LOGD(TAG,F(" vs "));
    //        ESP_LOGD(TAG,sum, HEX);
            return DECODE_CHK_ERR;
        }

        pOut->sensor_id  = ((uint32_t)msg[2] << 24) | (msg[3] << 16) | (msg[4] << 8) | (msg[5]);
        pOut->s_type     = (msg[6] >> 4); // 1: weather station, 2: indoor?, 4: soil probe
        pOut->battery_ok = (msg[6] >> 3) & 1;
        pOut->chan       = (msg[6] & 0x7);

        // temperature, humidity, shared with rain counter, only if valid BCD digits
        pOut->temp_ok  = msg[12] <= 0x99 && (msg[13] & 0xf0) <= 0x90 && msg[14] <= 0xA0 && (msg[14] & 0xf0) <= 0xA0;
        int temp_raw   = (msg[12] >> 4) * 100 + (msg[12] & 0x0f) * 10 + (msg[13] >> 4);
        float temp_c   = temp_raw * 0.1f;
        if (temp_raw > 600)
            temp_c = (temp_raw - 1000) * 0.1f;
        pOut->temp_c   = temp_c;
        pOut->humidity = (msg[14] >> 4) * 10 + (msg[14] & 0x0f);

        // apparently ff0(1) if not available
        pOut->uv_ok  = msg[15] <= 0x99 && (msg[16] & 0xf0) <= 0x90;
        int uv_raw = ((msg[15] & 0xf0) >> 4) * 100 + (msg[15] & 0x0f) * 10 + ((msg[16] & 0xf0) >> 4);
        pOut->uv   = uv_raw * 0.1f;
        int flags  = (msg[16] & 0x0f); // looks like some flags, not sure

        //int unk_ok  = (msg[16] & 0xf0) == 0xf0;
        //int unk_raw = ((msg[15] & 0xf0) >> 4) * 10 + (msg[15] & 0x0f);

        // invert 3 bytes wind speeds
        msg[7] ^= 0xff;
        msg[8] ^= 0xff;
        msg[9] ^= 0xff;
        pOut->wind_ok = (msg[7] <= 0x99) && (msg[8] <= 0x99) && (msg[9] <= 0x99);

        int gust_raw              = (msg[7] >> 4) * 100 + (msg[7] & 0x0f) * 10 + (msg[8] >> 4);
        pOut->wind_gust_meter_sec = gust_raw * 0.1f;
        int wavg_raw              = (msg[9] >> 4) * 100 + (msg[9] & 0x0f) * 10 + (msg[8] & 0x0f);
        pOut->wind_avg_meter_sec  = wavg_raw * 0.1f;
        pOut->wind_direction_deg  = (((msg[10] & 0xf0) >> 4) * 100 + (msg[10] & 0x0f) * 10 + ((msg[11] & 0xf0) >> 4)) * 1.0f;

        // rain counter, inverted 3 bytes BCD, shared with temp/hum, only if valid digits
        msg[12] ^= 0xff;
        msg[13] ^= 0xff;
        msg[14] ^= 0xff;
        pOut->rain_ok   = msg[12] <= 0x99 && msg[13] <= 0x99 && msg[14] <= 0x99;
        int rain_raw    = (msg[12] >> 4) * 100000 + (msg[12] & 0x0f) * 10000
                + (msg[13] >> 4) * 1000 + (msg[13] & 0x0f) * 100
                + (msg[14] >> 4) * 10 + (msg[14] & 0x0f);
        pOut->rain_mm   = rain_raw * 0.1f;

        pOut->moisture_ok = false;
        if (pOut->s_type == 4 && pOut->temp_ok && pOut->humidity >= 1 && pOut->humidity <= 16) {
            pOut->moisture_ok = true;
            pOut->moisture = moisture_map[pOut->humidity - 1];
        }
        return DECODE_OK;
    }

    #ifdef BFT_EN
    //
    // Convert wind speed from meters per second to Beaufort
    // [https://en.wikipedia.org/wiki/Beaufort_scale]
    //
    uint8_t windspeed_ms_to_bft(float ms)
    {
    if (ms < 5.5) {
        // 0..3 Bft
        if (ms < 0.9) {
        return 0;
        } else if (ms < 1.6) {
        return 1;
        } else if (ms < 3.4) {
        return 2;
        } else {
        return 3;
        }
    } else if (ms < 17.2) { 
        // 4..7 Bft
        if (ms < 8) {
        return 4;
        } else if (ms < 10.8) {
        return 5;
        } else if (ms < 13.9) {
        return 6;
        } else {
        return 7;
        }
    } else {
        // 8..12 Bft
        if (ms < 20.8) {
        return 8;
        } else if (ms < 24.5) {
        return 9;
        } else if (ms < 28.5) {
        return 10;
        } else if (ms < 32.7) {
        return 11;
        } else {
        return 12;
        }
    }
    }
    #endif


    //
    // Get weather data from CC1101 receiver and decode it
    //
    bool getWeatherdata(void)
    {
        uint8_t recvData[27];
        bool decode_ok = false;

        // Receive data
        // 1. flush RX buffer
        // 2. switch to RX mode
        // 3. wait for expected RX packet or timeout [~500us in this configuration]
        // 4. flush RX buffer
        // 5. switch to standby 
        int state = radio.receive(recvData, 27);
        

        if (state == RADIOLIB_ERR_NONE) {
            // Verify last syncword is 1st byte of payload (see above)
            if (recvData[0] == 0xD4) {
                #ifdef _DEBUG_MODE_
                    // print the data of the packet
                    ESP_LOGD(TAG," Data:");
                    printRawdata(recvData, sizeof(recvData));
    //                for(int i = 0 ; i < sizeof(recvData) ; i++) {
    //                    ESP_LOGD(TAG," %02X", recvData[i]);
    //                }
    //                ESP_LOGD(TAG,);

                    ESP_LOGD(TAG," R [0x%02X] RSSI: %f LQI: %d\n", recvData[0], radio.getRSSI(), radio.getLQI());
                #endif

                #ifdef _DEBUG_MODE_
                    //printRawdata(&recvData[1], sizeof(recvData));
                #endif
                
                // Decode the information - skip the last sync byte we used to check the data is OK
                #ifdef BRESSER_6_IN_1
                    decode_ok = (decodeBresser6In1Payload(&recvData[1], sizeof(recvData) - 1, &weatherData) == DECODE_OK);
                    ESP_LOGD(TAG,"decode_ok: %d, temp_ok: %d, wind_ok: %d, rain_ok: %d\n",
                        decode_ok, weatherData.temp_ok, weatherData.wind_ok, weatherData.rain_ok);
                #else
                    decode_ok = (decodeBresser5In1Payload(&recvData[1], sizeof(recvData) - 1, &weatherData) == DECODE_OK);
            
                    // Fixed set of data for 5-in-1 sensor
                    weatherData.temp_ok     = true;
                    weatherData.uv_ok       = false;
                    weatherData.wind_ok     = true;
                    weatherData.rain_ok     = true;
                    weatherData.moisture_ok = false;
                #endif
                
                if (decode_ok) {
                    const float METERS_SEC_TO_MPH = 2.237;
                    ESP_LOGD(TAG,"Id: [%8X] Battery: [%s] ",
                        weatherData.sensor_id,
                        weatherData.battery_ok ? "OK " : "Low");
                    #ifdef BRESSER_6_IN_1
                        ESP_LOGD(TAG,"Ch: [%d] ", weatherData.chan);
                    #endif
                    if (weatherData.temp_ok) {
                        ESP_LOGD(TAG,"Temp: [%3.1fC] Hum: [%3d%%] ",
                            weatherData.temp_c,
                            weatherData.humidity);
                    } else {
                        ESP_LOGD(TAG,"Temp: [---.-C] Hum: [---%] ");
                    }
                    if (weatherData.wind_ok) {
                        ESP_LOGD(TAG,"Wind max: [%3.1fm/s] Wind avg: [%3.1fm/s] Wind dir: [%4.1fdeg] ",
                            weatherData.wind_gust_meter_sec,
                            weatherData.wind_avg_meter_sec,
                            weatherData.wind_direction_deg);
                    } else {
                        ESP_LOGD(TAG,"Wind max: [--.-m/s] Wind avg: [--.-m/s] ");
                    }
                    if (weatherData.rain_ok) {
                        ESP_LOGD(TAG,"Rain: [%6.1fmm] ",  
                            weatherData.rain_mm);
                    } else {
                        ESP_LOGD(TAG,"Rain: [-----.-mm] "); 
                    }
                    if (weatherData.moisture_ok) {
                        ESP_LOGD(TAG,"Moisture: [%2d%%]",
                            weatherData.moisture);
                    }
                    ESP_LOGD(TAG,"\n");
                    //printf("{\"sensor_type\": \"bresser-5-in-1\", \"sensor_id\": %d, \"battery\": \"%s\", \"temp_c\": %.1f, \"hum_pc\": %d, \"wind_gust_ms\": %.1f, \"wind_speed_ms\": %.1f, \"wind_dir\": %.1f, \"rain_mm\": %.1f}\n",
                    //       sensor_id, !battery_low ? "OK" : "Low",
                    //       temperature, humidity, wind_gust, wind_avg, wind_direction_deg, rain);
                } // if (decode_ok)
                else {
                    #ifdef _DEBUG_MODE_
                        ESP_LOGD(TAG,"[CC1101] R [0x%02X] RSSI: %f LQI: %d\n", recvData[0], radio.getRSSI(), radio.getLQI());
                    #endif
                }
            } // if (recvData[0] == 0xD4)
            else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
                #ifdef _DEBUG_MODE_
                    ESP_LOGD(TAG,"T");
                #endif
            } // if (state == RADIOLIB_ERR_RX_TIMEOUT)
            else {
                // some other error occurred
                ESP_LOGD(TAG,"[CC1101] Receive failed - failed, code %d\n", state);
            }
        } // if (state == RADIOLIB_ERR_NONE)

        return decode_ok;
    }



    //
    // Print raw payload
    //


    #ifdef _DEBUG_MODE_
    void printRawdata(uint8_t *msg, uint8_t msgSize) {
        char s[120];
        uint8_t ii = 0;
        s[0] = 0;
        ESP_LOGD(TAG,"Raw Data:");
        for (uint8_t p = 0 ; p < msgSize ; p++) {
            sprintf(s, "%s %02X",s,msg[p]);
            ii++;
            if (ii>40) {
                ESP_LOGD(TAG, "%s", s);
                ii = 0;
                s[0] = 0;
            };        
        }
        ESP_LOGD(TAG,"%s", s);
    }
    #endif

};