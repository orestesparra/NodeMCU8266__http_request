/*
      NodeMCU8266 client
      This is a client for the Halloween Project server.
      ////////////////////////
      Title: Halloween Project
      Developer: Orestes Parra (orestesparra.com)
      License: MIT
      Version: 1.0      
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "halloween"
#define STAPSK  "scare1234"
#endif

#define ledValue1 2
#define ledConnected 16

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.4.1";
const uint16_t port = 80;

//this is for select the client: 
// 0 => client 1, 
// 1=> client 2, 
// 2=> client 3,
// 3=> client 4
int clientNumber = 0; 

void setup() {
  Serial.begin(115200);
 
  pinMode(ledValue1, OUTPUT);
  pinMode(ledConnected, OUTPUT);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  digitalWrite(ledConnected, LOW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("WiFi connected");
  digitalWrite(ledConnected, HIGH);
  
  Serial.println("Got IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}
WiFiClient client;
void loop() {
  HTTPClient http;
  static bool wait = false;
  
  String url = "/buttons";
  Serial.println("Asking value of buttons");
  http.begin(client,"http://192.168.4.1/buttons");
  int httpCode = http.GET();
  Serial.println("Reading...");
  String payload = http.getString();
  Serial.println(httpCode);   
  Serial.println(payload);   
  char option = payload[clientNumber];
  if(option=='1'){
     digitalWrite(ledValue1, LOW);
   }else{
     digitalWrite(ledValue1, HIGH);
   }
   
  if (wait) {
    delay(100);
  }
  wait = true;
}
