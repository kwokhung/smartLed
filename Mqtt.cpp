#include "Mqtt.h"
#include <PubSubClient.h>

Mqtt::Mqtt(PubSubClient *client)
{
    this->client = client;
}

void Mqtt::setup()
{
}
