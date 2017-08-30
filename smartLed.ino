#include "App.h"
#include "OLed.h"
#include "Wifi.h"
#include "Mqtt.h"

App app;
OLed oLed(OLed_ADDRESS, OLed_SDA_PIN, OLed_SCL_PIN);
Wifi wifi(oLed);
Mqtt mqtt(*app.led, oLed);

void setup()
{
    app.setup();
    oLed.setup();
    wifi.setup();
    mqtt.setup();
}

void loop()
{
    mqtt.loop();
}
