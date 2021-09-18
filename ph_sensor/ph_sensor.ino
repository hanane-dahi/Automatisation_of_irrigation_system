#include <SoftwareSerial.h>
#include <Wire.h>

 
#define RE 8
#define DE 7
 
const byte ph[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
byte values[11];
SoftwareSerial mod(2, 3);

 
void setup()
{
  Serial.begin(9600);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
 
}
 
void loop()
{
  byte val;
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(ph, sizeof(ph)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 11; i++)
    {
      values[i] = mod.read();
      Serial.print(values[i], HEX); 
    }
    Serial.println();
  }
  float soil_ph = float(values[4]) / 10;
  Serial.print("Soil Ph: ");
  Serial.println(soil_ph, 1);
 
 
  
  delay(3000);
}
