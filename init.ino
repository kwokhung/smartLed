#include "Mqtt.h"
#include "OLed.h"
#include "Led.h"
#include <EEPROM.h>
#include <SSD1306.h>

#define RX0 3 // GPIO3 / RX / D0
#define TX0 1 // GPIO1 / TX / D1

#define SCL 14 // GPIO14 / E5 / D5
#define SDA 12 // GPIO12 / E6 / D6

#define OnboardLED 2 // GPIO2 / E4 / D9

Led led(RLED, GLED, BLED);

SSD1306 display(0x3c, SDA, SCL);
QRcode qrcode(&display);
OLed oLed(&display, &qrcode);

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Wifi.h"
#include <PubSubClient.h>

const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

ESP8266WebServer server(80);
Wifi wifi(&server, &oLed);
WiFiClient espClient;
//WiFiClientSecure espClient;
PubSubClient client(espClient);
Mqtt mqtt(&client, &led, &oLed);

void setup()
{
    EEPROM.begin(512);

    //Serial.begin(115200);
    Serial.begin(57600);

    oLed.setup();

    led.setup();

    wifi.setup();

    //setupMqtt();
    mqtt.setup();
}

void loop()
{
    /*if (!client.connected())
    {
        mqtt.reconnect();
    }

    client.loop();*/
    mqtt.loop();
}

void setupMqtt()
{
    client.setServer(mqtt_server, 1883);
    //client.setServer(mqtt_server, 1884);
    client.setCallback(callback);

    reconnect();
}

void callback(char *topic, byte *payload, unsigned int length)
{
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

        oLed.reset(newSsid, newPassword);

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
        led.lightIt(RLED, payloadJson["RVALUE"].as<int>());
        led.lightIt(GLED, payloadJson["GVALUE"].as<int>());
        led.lightIt(BLED, payloadJson["BVALUE"].as<int>());
    }

    Serial.println('*');
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        if (client.connect("ad7cad07680c47ff80677b3c19bbe6dc", "mbltest01/nodemcu01", "e61m/mza6z5HY0eD4n/sbagP6mkDZeFfmmxSh5KER0w="))
        {
            Serial.println("connected");

            client.publish("letv1s01", "{\"RVALUE\":0,\"GVALUE\":0,\"BVALUE\":0}");
            client.subscribe("nodemcu01");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");

            delay(5000);
        }
    }
}
