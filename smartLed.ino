#include "App.h"
#include "Mqtt.h"

App app;
Mqtt mqtt(*app.led, *app.oLed);

void setup()
{
    app.setup();
    mqtt.setup();
}

void loop()
{
    mqtt.loop();
}
