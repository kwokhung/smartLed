#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Mqtt.h"
#include "OLed.h"
#include "Led.h"

extern char *mqtt_server;

Mqtt::Mqtt(Led& led, OLed& oLed)
{
    //WiFiClient* espClient = new WiFiClient();
    //WiFiClientSecure* espClient = new WiFiClientSecure;
    
    this->client = new PubSubClient(*new WiFiClient());
    this->led = &led;
    this->oLed = &oLed;
}

void Mqtt::setup()
{
    client->setServer(mqtt_server, 1883);
    //client->setServer(mqtt_server, 1884);

    client->setCallback([&](char *topic, byte *payload, unsigned int length) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");

        String payloadString = "";

        for (int i = 0; i < length; i++)
        {
            payloadString += (char)payload[i];
        }

        DynamicJsonBuffer jsonBuffer;
        JsonObject &payloadJson = jsonBuffer.parseObject(payloadString);

        Serial.print('*');
        Serial.print(payloadString);

        String toDo = payloadJson["what"]["toDo"].asString();

        if (toDo == "reset")
        {
            String newSsid = payloadJson["what"]["details"]["ssid"].asString();
            String newPassword = payloadJson["what"]["details"]["password"].asString();

            oLed->reset(newSsid, newPassword);

            for (int i = 0; i < 96; i++)
            {
                EEPROM.write(i, 0);
            }

            for (int i = 0; i < newSsid.length(); i++)
            {
                EEPROM.write(i, newSsid[i]);
            }

            for (int i = 0; i < newPassword.length(); i++)
            {
                EEPROM.write(32 + i, newPassword[i]);
            }

            EEPROM.commit();

            ESP.reset();
        }
        else
        {
            led->lightR(payloadJson["RVALUE"].as<int>());
            led->lightG(payloadJson["GVALUE"].as<int>());
            led->lightB(payloadJson["BVALUE"].as<int>());
        }

        Serial.println('*');
    });

    reconnect();
}

void Mqtt::loop()
{
    if (!client->connected())
    {
        reconnect();
    }

    client->loop();
}

void Mqtt::reconnect()
{
    while (!client->connected())
    {
        Serial.print("Attempting MQTT connection...");

        if (client->connect("ad7cad07680c47ff80677b3c19bbe6dc", "mbltest01/nodemcu01", "e61m/mza6z5HY0eD4n/sbagP6mkDZeFfmmxSh5KER0w="))
        {
            Serial.println("connected");

            client->publish("letv1s01", "{\"RVALUE\":0,\"GVALUE\":0,\"BVALUE\":0}");
            client->subscribe("nodemcu01");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client->state());
            Serial.println(" try again in 5 seconds");

            delay(5000);
        }
    }
}
