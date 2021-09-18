#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5,6);
 
void setup() {
s.begin(9600);
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
  root["soil_humidity"] = 40;
  root["air_humidity"] = 43;
  root["air_temperature"] = 37;
  root["ph_indicator"] = 8.3;
  root["nitrogen"] = 18;
  root["phosphorous"] = 20;
  root["potassium"] = 24;
 
if(s.available()>0)
  {
    root.printTo(s);
  }
}
