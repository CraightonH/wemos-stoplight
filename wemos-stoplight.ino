#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#import "html.h"

bool CYCLE = true;
long startOfCycleTime = millis();

ESP8266WebServer server(80);

const int led = 13;
const int RED_LIGHT = D4;
const int YEL_LIGHT = D3;
const int GRE_LIGHT = D2;

void handleRoot() {
  digitalWrite(led, 1);
  String page = INDEX_PAGE;
  server.send(200, "text/html", page);
  digitalWrite(led, 0);
}

void setLED(String light) {
  digitalWrite(RED_LIGHT, (light == "red"));
  digitalWrite(YEL_LIGHT, (light == "yellow"));
  digitalWrite(GRE_LIGHT, (light == "green"));
}

String getLEDStatus() {
  if (digitalRead(RED_LIGHT)) return "red";
  if (digitalRead(YEL_LIGHT)) return "yellow";
  if (digitalRead(GRE_LIGHT)) return "green";
  return "off";
}

void getStatus() {
    server.send(200, "text/plain", getLEDStatus());
}

void redOn() {
    CYCLE = false;
    setLED("red");
    server.send(200, "text/plain", getLEDStatus());
}

void yellowOn() {
    CYCLE = false;
    setLED("yellow");
    server.send(200, "text/plain", getLEDStatus());
}

void greenOn() {
    CYCLE = false;
    setLED("green");
    server.send(200, "text/plain", getLEDStatus());
}

void off() {
    CYCLE = false;
    setLED("off");
    server.send(200, "text/plain", getLEDStatus());
}

void cycle() {
    CYCLE = true;
    startOfCycleTime = millis();
}

void findKnownWiFiNetworks() {
  ESP8266WiFiMulti wifiMulti;
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("BYU-WiFi", "");
  wifiMulti.addAP("Hancock2.4G", "Arohanui");
  Serial.println("");
  Serial.print("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    wifiMulti.run();
    delay(1000);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID().c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  
  findKnownWiFiNetworks();  

  pinMode(RED_LIGHT, OUTPUT);
  pinMode(YEL_LIGHT, OUTPUT);
  pinMode(GRE_LIGHT, OUTPUT);
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/stoplight/status", getStatus);
  server.on("/stoplight/red", redOn);
  server.on("/stoplight/yellow", yellowOn);
  server.on("/stoplight/green", greenOn);
  server.on("/stoplight/cycle", cycle);
  server.on("/stoplight/off", off);

  server.onNotFound(handleRoot);

  server.begin();
  Serial.println("HTTP server started");
  cycle();
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  if (CYCLE) {
    if (millis() < startOfCycleTime + 4000) setLED("red");
    if (millis() > startOfCycleTime + 4000 && millis() < startOfCycleTime + 8000) setLED("green");
    if (millis() > startOfCycleTime + 8000 && millis() < startOfCycleTime + 12000) setLED("yellow");
    if (millis() > startOfCycleTime + 12000) cycle();
  }
}
