#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> 
#include "env.h"
#define bedswitch 23
#define lrswitch 22
#define hwswitch 21


char * endpoint = "https://ecse-three-led-api.onrender.com/api/state";

void switchonoff(bool L1, bool L2, bool L3)
{
if (L1==1)
    {
      digitalWrite(bedswitch,HIGH);
      Serial.println("Bed Room Switch is ON ");
    }
    else
    {
      digitalWrite(bedswitch,LOW);
      Serial.println("Bed Room Switch is OFF ");
    }

    if (L2==1)
    {
      digitalWrite(lrswitch,HIGH);
      Serial.println("Living Room Switch is ON ");
    }
    else
    {
      digitalWrite(lrswitch,LOW);
      Serial.println("Living Room Switch is OFF ");
    }

    if (L3==1)
    {
      digitalWrite(hwswitch,HIGH);
      Serial.println("Hall Way Switch is ON ");
    }
    else
    {
      digitalWrite(hwswitch,LOW);
      Serial.println("Hall Way Switch is OFF ");
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(bedswitch, OUTPUT);
  pinMode(lrswitch, OUTPUT);
  pinMode(hwswitch, OUTPUT);

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS,6);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    String http_response;
    http.begin(endpoint);
    http.addHeader("X-API-KEY","Emma#4451");
   

    int httpResponseCode = http.GET();

    if (httpResponseCode>0){
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.print(http_response);
    }
  else{
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    //return;
  }
  http.end();

  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, http_response);
  if (error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
      }

    const char* username = doc["username"];//Emma#4451
    const char* id = doc["_id"];//63fb859377ef4b58d4d8751a
    bool light_switch_1 = doc["light_switch_1"];
    bool light_switch_2 = doc["light_switch_2"];
    bool light_switch_3 = doc["light_switch_3"];

    Serial.println("");

    Serial.print("Bed Room Switch status: ");
    Serial.println(light_switch_1);

    Serial.print("Living Room Switch status: ");
    Serial.println(light_switch_2);

    Serial.print("Hall Way Switch status: ");
    Serial.println(light_switch_3);
    Serial.println("");

    switchonoff(light_switch_1,light_switch_2,light_switch_3);

  }
  else{
    return;
  }
}
