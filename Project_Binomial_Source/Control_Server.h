#ifndef CONTROL_SERVER_H
#define CONTROL_SERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "Project Binomial"
#define APPSK  "haatihaati5a5a"
#endif

void handleRoot();
void controlSetup();
void controlLoop();

#endif
