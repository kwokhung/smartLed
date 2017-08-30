#include <EEPROM.h>

#include "App.h"
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
