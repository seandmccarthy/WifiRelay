#include <ESP8266WiFi.h>

#define GPIO_0 0
#define RELAY_PIN 0
#define ON LOW
#define OFF HIGH

# STASSID and STAPSK passed in as -D compile time defines
const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);
IPAddress local_IP(10, 0, 1, 102);
IPAddress gateway(10, 0, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, OFF);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  digitalWrite(LED_BUILTIN, ON);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  client.setTimeout(5000); // default is 1000

  String req = client.readStringUntil('\r');

  if (req.indexOf(F("/button/momentary")) != -1) {
    digitalWrite(RELAY_PIN, ON);
    delay(1000);
    digitalWrite(RELAY_PIN, OFF);
  } else if (req.indexOf(F("/button/on")) != -1) {
    digitalWrite(LED_BUILTIN, OFF);
    digitalWrite(RELAY_PIN, OFF);
  } else if (req.indexOf(F("/button/off")) != -1) {
    digitalWrite(RELAY_PIN, ON);
    digitalWrite(LED_BUILTIN, ON);
  }

  while (client.available()) {
    client.read();
  }

  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n"));
  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/button/on'>ON</a> - <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/button/off'>OFF</a> - <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/button/momentary'>MOMENTARY</a>"));
  client.print(F("</html>"));
}
