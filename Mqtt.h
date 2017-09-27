#ifndef Mqtt_h
#define Mqtt_h

#define TINY_GSM_MODEM_A6

#include "TinyGsmClient.h"
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
  TinyGsm *modem;
  TinyGsmClient *gsmClient;
  PubSubClient *client;
  char *mqttUrl;
  Led *led;
  OLed *oLed;
};

#endif