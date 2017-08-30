#include "App.h"
#include "Led.h"
#include "OLed.h"
#include "Wifi.h"
#include "Mqtt.h"

App app;
Led led(LED_R_PIN, LED_G_PIN, LED_B_Pin);
OLed oLed(OLed_ADDRESS, OLed_SDA_PIN, OLed_SCL_PIN);
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
