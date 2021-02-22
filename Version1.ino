#include <ArduinoOTA.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "" //Enter your SSID
#define STAPSK  "" //Enter your Password
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
uint16_t relayPin;
uint16_t relayAction;
ESP8266WebServer server(80);

const char* www_username = "admin";
const char* www_password = "esp8266";
String message = "Number of args received:";
void setup() {
  Serial.begin(115200);
  pinMode(4,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(1,OUTPUT); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  //ArduinoOTA.begin();

  server.on("/", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    char temp[2000];
    snprintf(temp, 2000,
             "<html>\
<head>\
<style>\
.row{margin:auto;position:inline-flex; }\
.lab{width:100px;display:inline-block;text-align:center;font-size:16pt}\
.but{ border: 1px solid black; height:75px;width:75px;margin-left:10px;display:inline-block;text-align:center;}\
.red{ background-color:red;color:red;}\
.green { background-color:green;color:green;}\
</style>\
<title>\
Control Panel\
</title>\
</head>\
<body>\
<div class='container'>\
 <div class='row'>\
    <label class='lab'>Switch 1</label><br>\
    <a href='/control?relay=4&action=1&loc=cpanel'> <span class='but green'> ON </span></a>\
    <a href='/control?relay=4&action=0&loc=cpanel'><span class='but red'> OFF </span>\
  </div><br>\
  <div class='row'>\
    <label class='lab'>Switch 2</label><br>\
    <a href='/control?relay=14&action=1&loc=cpanel'> <span class='but green'> ON </span></a>\
    <a href='/control?relay=14&action=0&loc=cpanel'><span class='but red'> OFF </span>\
  </div><br>\
  <div class='row'>\
    <label class='lab'>Switch 3</label><br>\
    <a href='/control?relay=12&action=1&loc=cpanel'> <span class='but green'> ON </span></a>\
    <a href='/control?relay=12&action=0&loc=cpanel'><span class='but red'> OFF </span>\
  </div><br>\
  <div class='row'>\
    <label class='lab'>Switch 4</label><br>\
    <a href='/control?relay=5&action=1&loc=cpanel'> <span class='but green'> ON </span></a>\
    <a href='/control?relay=5&action=0&loc=cpanel'><span class='but red'> OFF </span>\
  </div><br>\
  <div class='row'>\
    <label class='lab'>Switch 5</label><br>\
    <a href='/control?relay=13&action=1&loc=cpanel'> <span class='but green'> ON </span></a>\
    <a href='/control?relay=13&action=0&loc=cpanel'><span class='but red'> OFF </span>\
  </div><br>\
  <div class='row'>\
    <label class='lab'>Switch 6</label><br>\
    <a href='/control?relay=1&action=1&loc=cpanel'> <span class='but green'> ON </span></a>\
    <a href='/control?relay=1&action=0&loc=cpanel'><span class='but red'> OFF </span>\
  </div>\
</div>\
</body>\
</html>");
    server.send(200, "text/html", temp);
  });
  server.on("/control", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    relayPin = (uint16_t)server.arg(0).toInt();
    relayAction = (uint16_t)server.arg(1).toInt();

    if (server.arg(2) == "cpanel") {
      server.send(200,"text/html","<html><head><script>window.location='/';</script></head></html>");
    }
    else{
      server.send(200,"text/plain","okay");
    }
    //Serial.println(server.args());
    digitalWrite(relayPin, relayAction);
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
