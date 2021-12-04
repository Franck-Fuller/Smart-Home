#include <Servo.h>

#include <SoftwareSerial.h>

#include <ArduinoJson.h>

#include <DHT.h>
#include <DHT_U.h>

#include <Servo.h>
#define DHTTYPE DHT11
//#include <Wire.h>
//#include "RTClib.h"





//************** Buffer Json *********

//StaticJsonBuffer<1000> jsonBuffer;
//JsonObject&root = jsonBuffer.createObject();

//******************** declaration des variables ***************************

//RTC_DS3231 rtc;
int dhtPin      = 7;
int pirPin      =  8;
int speaker     = 13;
Servo porte;
int positionPorte;
boolean statePorte = false;
int Pirr           ;
float t, h         ;
//String recu="" , ip="" , ssid=""  ;
boolean stateAlarme = false;
const unsigned long interval = 3000;
//const uint32_t interval = 3;
unsigned long previousTime = 0;
//uint32_t previousTime = 0;
DHT dht(dhtPin, DHTTYPE);
//DateTime now ;
DynamicJsonDocument root(1024);
SoftwareSerial Esp8266(6, 5);
SoftwareSerial AlarmeESP( 3, 2);

void setup() {
  Serial.begin(115200);
  Esp8266.begin(2400);
  AlarmeESP.begin(2400);
  dht.begin();  
  pinMode(pirPin, INPUT);
  pinMode(dhtPin, INPUT);
  porte.attach(9);
  porte.write(90);
  pinMode(speaker,  OUTPUT);
  /*if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }*/
  delay(4000);

}

/**Capteur de Temperature* ****/
void dht_test() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  //** add data to root
  root["temp"] = t;
  root["humi"] = h;

}



/*********   Detecteur de mouvement* *********/
void Pir_test() {
  Pirr = digitalRead(pirPin);
  //delay(100);
  if (Pirr == HIGH && stateAlarme) {
    root["presence"] = 1;
    digitalWrite(speaker, HIGH);
  }
  if (Pirr == LOW) {
    //Serial.println("Non");
    root["presence"] = 0;
  }

}

void ouvrirPorte(){
   for (positionPorte = 0; positionPorte <= 180; positionPorte += 1) { 
        porte.write(positionPorte);   
        delay(15);
   }
   Serial.println("Entreeee");
   statePorte = true;
   root["porte"] = statePorte;
}


void fermerPorte(){
   for (positionPorte = 180; positionPorte >= 0; positionPorte -= 1) { 
        porte.write(positionPorte);   
        delay(15);
   }
   statePorte = false;
   root["porte"] = statePorte;
}

void capteurs() {
  dht_test();
  Pir_test();
}

void loop() {
  //now = rtc.now();
  unsigned long currentTime = millis();
  //uint32_t currentTime = now.unixtime();
  /*if ( now.hour() >= 5 && now.minute() >= 38){
    digitalWrite(speaker,HIGH);
    delay(2000);
    digitalWrite(speaker,LOW);
  }*/
  /*Serial.println("Previous" + previousTime);
  Serial.println("Current" + currentTime);*/
  capteurs();
  if( currentTime - previousTime >= interval){
    Serial.println("Ok");
  serializeJson(root, Esp8266);
  serializeJson(root, Serial);
    previousTime = currentTime;
    
  }

  /*ouvrirPorte();
  delay(5000);
  fermerPorte();
  delay(5000);*/
 
  if(AlarmeESP.available()>0){
    String incomingString = AlarmeESP.readString();
    Serial.println("Etat alarme : " + incomingString);
    if( incomingString == "On"){
      stateAlarme = true;
    }
    else if (incomingString == "Off"){
      stateAlarme = false;
    }
    else if( incomingString == "OuvrirPorte" && !statePorte){
      Serial.println("Entré");
      ouvrirPorte();
    }
    else if( incomingString == "fermerPorte" && statePorte){
      Serial.println("Sortie");
      fermerPorte();
    }
  }
}
