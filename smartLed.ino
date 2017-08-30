#include <ArduinoJson.h>
#include <EEPROM.h>
#include <SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

#include "init.h"
#include "Led.h"
#include "OLed.h"
#include "Wifi.h"
#include "Mqtt.h"

Led led(RLED, GLED, BLED);

OLed oLed;

Wifi wifi(oLed);

Mqtt mqtt(led, oLed);

void setup()
{
    EEPROM.begin(512);

    //Serial.begin(115200);
    Serial.begin(57600);

    led.setup();

    oLed.setup();

    wifi.setup();

    mqtt.setup();
}

void loop()
{
    mqtt.loop();
}
