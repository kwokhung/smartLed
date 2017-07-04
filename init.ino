#include "OLed.h"
#include "Led.h"
#include <EEPROM.h>
#include <SSD1306.h>

#define RX0 3 // GPIO3 / RX / D0
#define TX0 1 // GPIO1 / TX / D1

#define SCL 14 // GPIO14 / E5 / D5
#define SDA 12 // GPIO12 / E6 / D6

#define OnboardLED 2 // GPIO2 / E4 / D9
#define RLED 5       // GPIO5 / E1 / D3
#define GLED 4       // GPIO4 / E2 / D4
#define BLED 0       // GPIO0 / E3 / D8

Led led(RLED, GLED, BLED);

SSD1306 display(0x3c, SDA, SCL);
QRcode qrcode(&display);
OLed oLed(&display, &qrcode);

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

//const char *ssid = "MASON-IT";
//const char *password = "22182830";
//const char *ssid = "mxjk";
//const char *password = "mxjk2015";
const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

WiFiClient espClient;
//WiFiClientSecure espClient;
ESP8266WebServer server(80);
PubSubClient client(espClient);
String connectInfo;

void setup()
{
    EEPROM.begin(512);

    //Serial.begin(115200);
    Serial.begin(57600);

    oLed.setup();

    led.setup();

    setupWifi();

    setupMqtt();
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }

    client.loop();
}

void setupWifi()
{
    String ssid = "";

    for (int i = 0; i < 32; i++)
    {
        ssid += char(EEPROM.read(i));
    }
    //ssid = "xxx";

    String password = "";

    for (int i = 32; i < 96; i++)
    {
        password += char(EEPROM.read(i));
    }
    //ssid = "yyy";

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(const_cast<char *>(ssid.c_str()), const_cast<char *>(password.c_str()));

    int percentage = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        oLed.connecting(ssid, password, percentage);

        percentage += 5;

        if (percentage > 100)
        {
            break;
        }
    }

    if (percentage > 100)
    {
        randomSeed(analogRead(0));
        long randNumber = random(100, 1000);
        String mySsid = "ESP" + String(random(100, 1000), DEC);
        //String myPassword = String(random(10000000, 100000000), DEC);
        String myPassword = "12345678";

        Serial.println("");
        Serial.println("Configuring access point...");
        Serial.println(mySsid);
        Serial.println(myPassword);

        WiFi.softAP(const_cast<char *>(mySsid.c_str()), const_cast<char *>(myPassword.c_str()));

        IPAddress myIP = WiFi.softAPIP();

        connectInfo = mySsid + "/" + myPassword + "/" + myIP.toString();

        Serial.print("Connect Info: ");
        Serial.println(connectInfo);

        oLed.beAccessPoint(mySsid, myPassword, &myIP);

        server.on("/", HTTP_GET, []() {
            server.send(200, "text/html", "<h1>You are connected</h1><p>" + connectInfo + "</p>");
        });

        server.on("/", HTTP_POST, []() {
            DynamicJsonBuffer jsonBuffer;
            JsonObject &payloadJson = jsonBuffer.parseObject(server.arg("plain"));

            //String payloadString;
            //payloadJson.printTo(payloadString);
            //server.send(200, "text/json", payloadString);

            String newSsid = payloadJson["ssid"].asString();
            String newPassword = payloadJson["password"].asString();

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

            server.send(200, "application/json", "{success:true}");
            delay(1000);
            //server.stop();

            ESP.reset();
        });

        server.begin();

        while (true)
        {
            server.handleClient();
        }
    }
    else
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        oLed.connected(ssid, WiFi.localIP().toString());
    }
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
