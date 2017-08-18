#ifndef Mqtt_h
#define Mqtt_h

#include <PubSubClient.h>

class Mqtt
{
  public:
    Mqtt(PubSubClient *client);
    void setup();

  private:
    PubSubClient *client;
};

#endif