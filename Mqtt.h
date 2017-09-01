#ifndef Mqtt_h
#define Mqtt_h

#include <PubSubClient.h>

#include "Led.h"
#include "OLed.h"

class Mqtt
{
public:
  Mqtt(char *mqttUrl, Led &led, OLed &oLed);

  void setup();
  void loop();
  void reconnect();

private:
  PubSubClient *client;
  char *mqttUrl;
  Led *led;
  OLed *oLed;
};

#endif