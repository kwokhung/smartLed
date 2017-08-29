#include "Mqtt.h"
#include "OLed.h"
#include "Led.h"
#include <EEPROM.h>
#include <SSD1306.h>

#define RX0 3 // GPIO3 / RX / D0
#define TX0 1 // GPIO1 / TX / D1

#define SCL 14 // GPIO14 / E5 / D5
#define SDA 12 // GPIO12 / E6 / D6

#define OnboardLED 2 // GPIO2 / E4 / D9

Led led(RLED, GLED, BLED);

SSD1306 display(0x3c, SDA, SCL);
QRcode qrcode(&display);
OLed oLed(&display, &qrcode);

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Wifi.h"
#include <PubSubClient.h>

const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

ESP8266WebServer server(80);
Wifi wifi(&server, &oLed);
WiFiClient espClient;
//WiFiClientSecure espClient;
PubSubClient client(espClient);
Mqtt mqtt(&client, &led, &oLed);

void setup()
{
    EEPROM.begin(512);

    //Serial.begin(115200);
    Serial.begin(57600);

    oLed.setup();

    led.setup();

    wifi.setup();

    mqtt.setup();
}

void loop()
{
    mqtt.loop();
}
