#include "Arduino.h"
#include "Control_Server.h"

IPAddress local_IP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

/* AP Credentials and Server Port */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void controlSetup() {
  Serial.println();
  
  Serial.print("Configuring access point...");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  WiFi.softAP(ssid, password);
  
  /* Start AP and Server */
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.begin();
  
  Serial.println("HTTP server started");
}

void controlLoop() {
  server.handleClient();
}
