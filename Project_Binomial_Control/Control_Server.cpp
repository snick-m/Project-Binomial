#include "Arduino.h"
#include "Control_Server.h"

IPAddress local_IP(192, 168, 5, 1);
IPAddress gateway(192, 168, 5, 1);
IPAddress subnet(255, 255, 255, 0);

/* AP Credentials and Server Port */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);
WebSocketsServer wsServer(81);

void webSocketEvent(uint8_t host, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[%u] Disconnected!\n", host);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = wsServer.remoteIP(host);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", host, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        wsServer.sendTXT(host, "Connected");
      }
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[%u] get Text: %s\n", host, payload);

      // send message to client
      // wsServer.sendTXT(host, "message here");

      // send data to all connected clients
      // wsServer.broadcastTXT("message here");
      break;
    case WStype_BIN:
      USE_SERIAL.printf("[%u] get binary length: %u\n", host, length);
      hexdump(payload, length);

      // send message to client
      // wsServer.sendBIN(host, payload, length);
      break;
  }

}

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

  wsServer.begin();
  wsServer.onEvent(webSocketEvent);

  Serial.println("HTTP server started");
}

void controlLoop() {
  server.handleClient();
  wsServer.loop();
}
