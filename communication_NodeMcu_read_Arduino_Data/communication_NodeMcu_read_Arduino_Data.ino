#include <SoftwareSerial.h>


SoftwareSerial s(D6,D5);
#include <ArduinoJson.h>
 
void setup() {
  
  Serial.begin(9600);
  s.begin(9600);
  while (!Serial) continue;
 
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.print("soil_humidity : ");
  Serial.print("");
  float data1=root["soil_humidity"];
  Serial.println(data1);
  Serial.print("air_humidity");
  float data2=root["air_humidity"];
  Serial.println(data2);
  Serial.println("");
   Serial.print("air_temperature");
  float data3=root["air_temperature"];
  Serial.println(data3);
  Serial.println("--------------------------------------------");
 
}
