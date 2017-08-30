#include <EEPROM.h>
#include <HardwareSerial.h>

#include "Led.h"
#include "OLed.h"
#include "Wifi.h"
#include "Mqtt.h"
#include "App.h"

//const char *ssid = "MASON-IT";
//const char *password = "22182830";
//const char *ssid = "mxjk";
//const char *password = "mxjk2015";

const char *mqttUrl = "mbltest01.mqtt.iot.gz.baidubce.com";

App::App()
    : led(new Led(LED_R_PIN, LED_G_PIN, LED_B_Pin)),
      oLed(new OLed(OLed_ADDRESS, OLed_SDA_PIN, OLed_SCL_PIN)),
      wifi(new Wifi(*oLed)),
      mqtt(new Mqtt(*led, *oLed))
{
}

void App::setup()
{
    EEPROM.begin(EEPROM_SIZE);

    Serial.begin(SERIAL_RATE);

    led->setup();
    oLed->setup();
    wifi->setup();
    mqtt->setup();
}

Mqtt *App::getMqtt()
{
    return mqtt;
}
