#include <EEPROM.h>
#include <HardwareSerial.h>

#include "Led.h"
#include "OLed.h"
#include "Wifi.h"
#include "App.h"

//const char *ssid = "MASON-IT";
//const char *password = "22182830";
//const char *ssid = "mxjk";
//const char *password = "mxjk2015";

const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

App::App() : led(new Led(LED_R_PIN, LED_G_PIN, LED_B_Pin)),
             oLed(new OLed(OLed_ADDRESS, OLed_SDA_PIN, OLed_SCL_PIN)),
             wifi(new Wifi(*oLed))
{
}

void App::setup()
{
    EEPROM.begin(512);

    Serial.begin(115200);
    //Serial.begin(57600);

    led->setup();
    oLed->setup();
    wifi->setup();
}
