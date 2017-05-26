#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RPIN 5
#define GPIN 4
#define BPIN 0
const int LED = BPIN;
int ledValue = 0;

const char *ssid = "MASON-IT";
const char *password = "22182830";
const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    Serial.begin(115200);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    //ledValue = map(1023, 0, 1023, 0, 255);
    //analogWrite(LED, ledValue);

    setup_wifi();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    reconnect();
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }

    client.loop();
}

void setup_wifi()
{
    delay(10);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
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

    long payloadLong = payloadString.toInt();

    Serial.print('*');
    Serial.print(payloadString);
    Serial.print('*');
    Serial.print(payloadLong);
    Serial.print('*');

    if (payloadString == "on")
    {
        analogWrite(LED, 0);
        digitalWrite(LED, LOW);
    }
    else if (payloadString == "off")
    {
        analogWrite(LED, 0);
        digitalWrite(LED, HIGH);
    }
    else if (payloadLong > 0)
    {
        analogWrite(LED, 1023 - payloadLong);
    }

    Serial.println();
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        if (client.connect("ad7cad07680c47ff80677b3c19bbe6dc", "mbltest01/nodemcu01", "e61m/mza6z5HY0eD4n/sbagP6mkDZeFfmmxSh5KER0w="))
        {
            Serial.println("connected");

            client.publish("letv1s01", "1023");
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
