#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "RELAY_CONTROL"
#define APPSK  "12345678"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

#define LED 2
u_int8_t LED_status = 1; // Начальный статус светодиода
u_int8_t RELAY_status = 0; // Начальный статус реле

const String HtmlHtml = "<html><head><meta charset=\"utf-8\">"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlTitle = "<h1>Умный сетевой фильтр с ДУ - Панель управления</h1><br/>\n";
const String HtmlLedStateLow = "<big>Питание вход 1 </big><br/><br/>\n";
const String HtmlLedStateHigh = "<big>Питание вход 1 </big><br/><br/>\n";
const String HtmlButtons = 
//"<a href=\"LEDOff\"><button style=\"background-color:red;color:white;width:20%; height:5%;\">Выключить </button></a>\n"
"<a href=\"SWITCH\"><button style=\"background-color:green;color:white;width:20%; height:5%;\">Переключить </button></a><br/>";
const String HtmlHtmlClose = "</html>";

void response(){
  String htmlRes = HtmlHtml + HtmlTitle;
  htmlRes += HtmlLedStateLow;
  htmlRes += HtmlButtons;
  htmlRes += HtmlHtmlClose; 
  server.send(200, "text/html", htmlRes);
}

void handleSwitch() {
  if(LED_status) LED_status = 0;
  else LED_status = 1;

  digitalWrite(LED, LED_status);
  digitalWrite(4, ~RELAY_status);
  //delay(100);
  //digitalWrite(4, RELAY_status);

  response();
}

//void handleLedOff() {
//  statusRELAY = 0; // conditions to make the LED turn off

//  digitalWrite(LED, statusRELAY);

//  response();
//}

void setup() {

    delay(1000); 
    WiFi.softAP(ssid, password); 
    IPAddress apip = WiFi.softAPIP();     
    server.on("/", response); 
    server.on("/SWITCH", handleSwitch);
    //server.on("/LEDOn", handleLedOff); 

    server.begin();    
    pinMode(LED, OUTPUT);
    pinMode(4,  OUTPUT);
    digitalWrite(LED, LED_status);
    digitalWrite(4, RELAY_status);
}
 
void loop() {
    server.handleClient();
    digitalWrite(4, RELAY_status);
}
