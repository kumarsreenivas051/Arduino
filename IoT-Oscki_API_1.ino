

/**
 * Authorization.ino
 *
 *  Created on: 09.12.2015
 * Modified on:23.01.2018 
 * By:Kumaraswamy V S
 
 */

#include <Arduino.h>

#define DHTTYPE DHT11   // DHT 11
#include "DHT.h"

#define DHTPIN D3
//#define pirpin 5
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <SPI.h>
#include <Wire.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
DHT dht(DHTPIN, DHTTYPE);
ESP8266WiFiMulti WiFiMulti;
String temp();
int ledPin=D4;
void setup() {
pinMode(A0,INPUT);
Serial.begin(115200);
pinMode(ledPin,OUTPUT);

  dht.begin();  
  

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println(); 
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("ssid", "password");

}
 void loop() {
    // wait for WiFi connection
   
    delay(100);
    Serial.println("started");
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url


        http.begin("server_path"+temp());
        //http.begin("http://172.0.0.100/iot/1.php?"+pir());



        /*
          // or
          http.begin("http://192.168.1.12/test.html");
          http.setAuthorization("user", "password");

          // or
          http.begin("http://192.168.1.12/test.html");
          http.setAuthorization("dXNlcjpwYXN3b3Jk");
         */


        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    delay(100);
}


/*
String pir()
{
  String ret="";
  int input = analogRead(pirpin);
  ret="a=" + (String)input + "&username=***&key=***&tab=pir";
  Serial.println(ret);
  return ret;
}

*/
String temp()
{
 
 String ret="";
  float coVal=analogRead(A0);
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f) || isnan(coVal)){
    Serial.println("Failed to read from DHT sensor!");
    return "";
  }
 
   if((map(analogRead(A0),700,1023,0,10)>4))
  {
    digitalWrite(ledPin,HIGH);
    delay(500);
     digitalWrite(ledPin,LOW);
     delay(100);
  }
  else{
    digitalWrite(ledPin,LOW);
  }
  

//  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  ret="Humidity="+(String)h+"%"+"   Temperature="+(String)t+"°C"+"   Intensity of CO="+(String)(map(analogRead(A0),700,1023,0,10))+"   username=****&key=****&tab=DHT";
  Serial.println(ret); 
  delay(1000);
  return ret;
} 
