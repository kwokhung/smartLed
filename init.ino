#include <EEPROM.h>
#include <SSD1306.h>
#include <qrcode.h>

#define RX0 3 // GPIO3 / RX / D0
#define TX0 1 // GPIO1 / TX / D1

#define SCL 14 // GPIO14 / E5 / D5
#define SDA 12 // GPIO12 / E6 / D6

SSD1306 display(0x3c, SDA, SCL);
QRcode qrcode(&display);

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

//const char *mySsid = "AAA407";
//const char *myPassword = "12345678";
//const char *ssid = "aMASON-IT";
//const char *password = "22182830";
//const char *ssid = "mxjk";
//const char *password = "mxjk2015";
const char *mqtt_server = "mbltest01.mqtt.iot.gz.baidubce.com";

//StaticJsonBuffer<512> jsonBuffer;
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

    setupDisplay();

    setupLed();

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

void setupDisplay()
{
    display.init();
    display.clear();
    display.display();

    // qrcode.debug();
    qrcode.init();
    //qrcode.create("Hello world.");
}

void setupWifi()
{
    String ssid = "";

    for (int i = 0; i < 32; i++)
    {
        ssid += char(EEPROM.read(i));
    }

    String password = "";

    for (int i = 32; i < 96; i++)
    {
        password += char(EEPROM.read(i));
    }

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //WiFi.begin(ssid, password);
    WiFi.begin(const_cast<char *>(ssid.c_str()), const_cast<char *>(password.c_str()));

    int percentage = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        //display.drawString(64, 8, F("Connecting"));
        //display.drawString(64, 28, ssid);
        display.drawString(64, 8, ssid);
        display.drawString(64, 28, password);
        display.drawProgressBar(10, 48, 108, 12, percentage);
        display.display();

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

        //WiFi.softAP(mySsid, myPassword);
        WiFi.softAP(const_cast<char *>(mySsid.c_str()), const_cast<char *>(myPassword.c_str()));

        IPAddress myIP = WiFi.softAPIP();

        connectInfo = mySsid + "/" + myPassword + "/" + myIP.toString();

        Serial.print("Connect Info: ");
        Serial.println(connectInfo);

        //qrcode.create(connectInfo);
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 8, "SSID: " + mySsid);
        display.drawString(64, 28, "Password: " + myPassword);
        display.drawString(64, 48, "IP: " + myIP.toString());
        display.display();

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
            server.send(200, "text/json", "{success:true}");
            server.stop();
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
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 8, F("Connected"));
        display.drawString(64, 28, ssid);
        display.drawString(64, 48, WiFi.localIP().toString());
        display.display();
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

    lightIt(RLED, payloadJson["RVALUE"].as<int>());
    lightIt(GLED, payloadJson["GVALUE"].as<int>());
    lightIt(BLED, payloadJson["BVALUE"].as<int>());

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

/*void lightIt(int led, int brightness)
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
}*/
