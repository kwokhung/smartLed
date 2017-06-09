#include <SSD1306.h>
#include <qrcode.h>

SSD1306 display(0x3c, D6, D5);
QRcode qrcode(&display);

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RLED 5
#define GLED 4
#define BLED 0

const char *ssid = "MASON-IT";
const char *password = "22182830";
const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

//StaticJsonBuffer<512> jsonBuffer;
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    Serial.begin(115200);

    display.init();
    display.clear();
    display.display();

    // enable debug qrcode
    qrcode.debug();

    // Initialize QRcode display using library
    qrcode.init();
    // create qrcode
    qrcode.create("Hello world.");

    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, HIGH);
    digitalWrite(BLED, HIGH);

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

    DynamicJsonBuffer jsonBuffer;
    JsonObject &payloadJson = jsonBuffer.parseObject(payloadString);

    Serial.print('*');
    Serial.print(payloadString);

    lightIt(RLED, payloadJson["RVALUE"].as<int>());
    lightIt(GLED, payloadJson["GVALUE"].as<int>());
    lightIt(BLED, payloadJson["BVALUE"].as<int>());

    Serial.println('*');
}

void lightIt(int led, int brightness)
{
    Serial.print('*');
    Serial.print(brightness);

    if (brightness > 0)
    {
        analogWrite(led, 1023 - brightness);
    }
    else if (brightness == 0)
    {
        analogWrite(led, 1023);
        digitalWrite(led, HIGH);
    }
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
