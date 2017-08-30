#include "App.h"
#include "Led.h"
#include "OLed.h"
#include "Wifi.h"
#include "Mqtt.h"

App app;
Led led(RLED, GLED, BLED);
OLed oLed(OLed_ADDRESS, OLed_SDA, OLed_SCL);
Wifi wifi(oLed);
Mqtt mqtt(led, oLed);

void setup()
{
    app.setup();
    led.setup();
    oLed.setup();
    wifi.setup();
    mqtt.setup();
}

void loop()
{
    mqtt.loop();
}
