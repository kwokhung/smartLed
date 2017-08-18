#ifndef Mqtt_h
#define Mqtt_h

#include <PubSubClient.h>
#include "OLed.h"
#include "Led.h"

class Mqtt
{
  public:
    Mqtt(PubSubClient *client, Led *led, OLed *oLed);
    void setup();
    void loop();
    void reconnect();
    
  private:
    PubSubClient *client;
    Led *led;
    OLed *oLed;
};

#endif