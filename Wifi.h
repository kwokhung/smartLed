#ifndef Wifi_h
#define Wifi_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "OLed.h"

//const char *ssid = "MASON-IT";
//const char *password = "22182830";
//const char *ssid = "mxjk";
//const char *password = "mxjk2015";

class Wifi
{
public:
  Wifi(ESP8266WebServer *server, OLed *oLed);
  void setup();

private:
  ESP8266WebServer *server;
  OLed *oLed;
  String connectInfo;
};

#endif