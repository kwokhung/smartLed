#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "MASON-IT";
const char *password = "22182830";
const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    Serial.begin(115200);

    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

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
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    if ((char)payload[0] == 'o' && (char)payload[1] == 'n') //on
        digitalWrite(4, LOW);
    else if ((char)payload[0] == 'o' && (char)payload[1] == 'f' && (char)payload[2] == 'f') //off
        digitalWrite(4, HIGH);

    Serial.println();
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ad7cad07680c47ff80677b3c19bbe6dc", "mbltest01/nodemcu01", "e61m/mza6z5HY0eD4n/sbagP6mkDZeFfmmxSh5KER0w="))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("letv1s01", "1023");
            // ... and resubscribe
            client.subscribe("nodemcu01");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
