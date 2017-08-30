#include <EEPROM.h>
#include <HardwareSerial.h>

#include "App.h"

const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

App::App()
{
}

void App::setup()
{
    EEPROM.begin(512);

    Serial.begin(115200);
    //Serial.begin(57600);
}
