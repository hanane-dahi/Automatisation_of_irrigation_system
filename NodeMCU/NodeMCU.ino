#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>  
#include "RTClib.h"


LiquidCrystal_I2C lcd(0x27, 16, 2);

RTC_DS1307 RTC; 

#define FIREBASE_HOST "highagriv6-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "761zrw0dHrfaDdgTHqtiPsszjtPOHcTEDGNkctjq"
#define WIFI_SSID "ORANGE-DIGITAL-CENTER"
#define WIFI_PASSWORD "ODCxxxx"

int moisture_pin = A0;
float soil_moisture;
float air_temperature;
float air_humidity;
float soil_ph;
float nitrogen_value, phosphorous_value, potassium_value;
int WaterLevel = 0; 

SoftwareSerial s(D6,D5);
 

void setup() {
  
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  
  Wire.begin(D2, D1);
  lcd.begin();
  lcd.backlight();
  lcd.print("AgriTech Group");


  // wait for serial communication ...
  s.begin(9600);
  while (!Serial) continue;



  RTC.begin();  
  RTC.adjust(DateTime(__DATE__, __TIME__)); 
  
}

void loop() {
   StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(s);
  if (data == JsonObject::invalid())
    return;

  

  send_data("soil_moisture", data["soil_moisture"]);
  delay(1000);
  send_data("air_temperature", data["air_temperature"]);
  delay(1000);
  send_data("air_humidity", data["air_humidity"]);
  delay(1000);
  send_data("soil_ph", data["soil_ph"]);
  delay(1000);
  send_data("nitrogen_value", data["nitrogen_value"]);
  delay(1000);
  send_data("phosphorous_value", data["phosphorous_value"]);
  delay(1000);
  send_data("potassium_value", data["potassium_value"]);
  delay(10000);
  time now = RTC.now();
  send_data(" time", data["time"]);
  WaterLevel = data["WaterLevel"];

  
  affichage();
  
}

void send_data(String indicator, float sensor_data){
    Firebase.pushFloat(indicator, sensor_data);  
     if (Firebase.failed()) {
      Serial.print("probleme avec l'ajout de");
      Serial.println(indicator);
      Serial.println(Firebase.error());  
      return;
  }
}

void affichage(){
  lcd.clear();
     String irrigation_decision = Firebase.getString("irrigation/decision");
    Serial.print("irrigation_decision : ");
   Serial.println(irrigation_decision);

  if(Firebase.failed()){
      Serial.println("probleme de lecture de irrigation");
    }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Level : ");
  lcd.setCursor(8,0);
  if (WaterLevel<=100){ 
    lcd.println("Vide     ");
    
      
        
  } 
  
  else if (WaterLevel>100 && WaterLevel<=300){
    lcd.println("Faible"); 
        Serial.println("Faible      "); 
    
  } 
    
  else if (WaterLevel>300 && WaterLevel<=340){ 
    lcd.println("Moyen"); 
        Serial.println("Moyen      "); 
        
  }
  
  else if (WaterLevel>340){ 
    lcd.println("Rempli"); 
        Serial.println("Rempli      "); 
     
  }
  
  
  

  
  lcd.setCursor(0,1);
  
  if(irrigation_decision == "on"){
      lcd.print("irrigation");
      Firebase.setString("irrigation/decision", "off");
     
    }else{
        lcd.print("pas d'irrigation.");
      }

  delay(5000);
  
  
  
  }
