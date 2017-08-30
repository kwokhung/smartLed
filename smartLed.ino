#include "App.h"
#include "Wifi.h"
#include "Mqtt.h"

App app;
Wifi wifi(*app.oLed);
Mqtt mqtt(*app.led, *app.oLed);

void setup()
{
    app.setup();
    wifi.setup();
    mqtt.setup();
}

void loop()
{
    mqtt.loop();
}
