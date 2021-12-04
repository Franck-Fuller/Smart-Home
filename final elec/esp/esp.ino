#include <ESP8266WiFi.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#include <ArduinoJson.h>
/***************** Déclaration des variables ****************/

const char* ssid = "Eloctro-lab";
const char* password = "12345678";

SoftwareSerial Arduino(5,4);
//SoftwareSerial AlarmeArduino(13,15);
Servo porte; 
int led1=02;
int led2=13;
int led3=12; 
int pirPin = 15;
int pos =0;
const int speaker = 16;
//int *speakerP = &speaker;

boolean stateL1 =true;
boolean stateL2 =true;
boolean stateL3 =true;
boolean statePorte = false;
float temp=0.0, humi=0.0 ;
int flame=0, gaz=0,presence=0;
int *presenceP = &presence;
int stateAlarme = 0;//Faux
int *stateAlarmeP=&stateAlarme;

int Pirr;


ESP8266WebServer server(80);

void Pir_test() {
  Pirr = digitalRead(pirPin);
  Serial.println("test1");
  Serial.print(digitalRead(pirPin));
  Serial.println("stateAlarme");
   Serial.println(stateAlarme);
  Serial.println("fin test1");
  //delay(100);
  if (digitalRead(pirPin) == HIGH) {
    Serial.println("Detecte");
    *presenceP = 1;
    Serial.printf("%d\n", &presence);
     Serial.println(presence);
    if(*stateAlarmeP){
      //tone(speaker,500);
    digitalWrite(speaker, HIGH);
    Serial.println("je dois sonner");
    }
    else
    {   noTone(speaker);
       //digitalWrite(*speakerP, LOW);
        Serial.println("je ne dois pas sonner");
    }
  }
  else{
     Serial.println("ne Detecte pas");
    *presenceP = 0;
    Serial.println(presence);
  }

}

void getStateLed(int state, int led)
{
  switch (led)
  {
    case 1:if(state){
        server.send(200, "text/html", "1");}
        else{
           server.send(200, "text/html", "0");
        };break;

        case 2:if(state){
        server.send(200, "text/html", "1");}
        else{
           server.send(200, "text/html", "0");
        };break;
        case 3:if(state){
        server.send(200, "text/html", "1");}
        else{
           server.send(200, "text/html", "0");
        };break;

        case 4:if(state){
        server.send(200, "text/html", "1");}
        else{
           server.send(200, "text/html", "0");
        };break;

        case 5:if(state){
        server.send(200, "text/html", "1");}
        else{
           server.send(200, "text/html", "0");
        };break;
  }
}

void statL1()
{
  getStateLed( stateL1,1);
}
void statL2()
{
  getStateLed( stateL2,2);
}
void statL3()
{
  getStateLed( stateL3,3);
}


void getTemperatur () {
  //envoie de la température de la pièce 
     
    //******* temperature ************
  server.send(200, "text/html", (String)temp);
    
}
void getHumidite ()
{
//envoie de l'humidité de la pièce 
      
  server.send(200, "text/plain", (String)humi);
    
}

void getFlamme ()
{
//envoie de la présence de flamme de la pièce 
      
    //******* flamme ************
  server.send(200, "text/plain", (String)flame);
    
}
void getPresence ()
{
  //envoie de la présence 
      
    //******* flamme ************
    if(*stateAlarmeP)
    {
  server.send(200, "text/plain", (String)presence);
    }
    else
    {
      server.send(200, "text/plain", "0");
    }
    
}

void getGaz ()
{
//envoie de la présence de gaz de la pièce 
  
    //******* gaz ************
  server.send(200, "text/plain", (String)gaz);
    
}

void EteindreLampe1()
{
    stateL1=false;
    digitalWrite(led1, LOW);
    server.send(200, "text/plain", "Off");
   
}
 void AllumerLampe1 ()
 {
  stateL1=true;
   digitalWrite(led1, HIGH);
  server.send(200, "text/plain", "On1");
 }
 void EteindreLampe2()
{
  stateL2 =false;
  digitalWrite(led2, LOW);
    server.send(200, "text/plain", "Off");
   
}


 void AllumerLampe2 ()
 { stateL2=true;
   digitalWrite(led2, HIGH);
  server.send(200, "text/plain", "On");
 }
 void EteindreLampe3()
{
  stateL3=false;
  digitalWrite(led3, LOW);
    server.send(200, "text/plain", "Off");
   
}
 void AllumerLampe3 () {
  stateL3=true;
   digitalWrite(led3, HIGH);
  server.send(200, "text/plain", "On");
 }


 void OuverturePorte(){
  Serial.print("Ouvrir porte");
  Serial.println(statePorte);
      for (int pos = 0; pos <= 90; pos += 1) { 
          porte.write(pos);   
          delay(5);         
      }
      Serial.println("En train d'ouvrir la porte");
      statePorte=true;
      server.send(200, "text/plain", "OK");
  
}

 void FermeturePorte(){
    Serial.print("Fermer porte");
    Serial.print(statePorte);
      for (int pos = 90; pos >= 0; pos -= 1){ 
        porte.write(pos);  
        delay(5);            
      }
      porte.write(0);
      Serial.println("En train de fermer la porte");
      statePorte=false;
  server.send(200, "text/plain", "OK");
  
  
 }

  void onAlarme ()
 {
  server.send(200, "text/plain", "On");
  //AlarmeArduino.print("On");
  Serial.print("En train d'allumer l'alarme");
  *stateAlarmeP = 1;
  Serial.println(stateAlarme);
 }

  void offAlarme ()





  
 {
  server.send(200, "text/plain", "Off");
  Serial.print("En train d'eteindre l'alarme");
  //AlarmeArduino.print("Off");
  *stateAlarmeP = 0;
   Serial.println(stateAlarme);
 }

  void onAllLED ()
 {
   digitalWrite(led1, HIGH);
   digitalWrite(led2, HIGH);
   digitalWrite(led3, HIGH);
   /*digitalWrite(led4, HIGH);
   digitalWrite(led5, HIGH);*/
   stateL1=true;
   stateL2=true;   
   stateL3=true;   
  // stateL4=true;   
  // stateL5=true;      
  server.send(200, "text/plain", "OK");
 }
  void offAllLED ()
 {
   digitalWrite(led1, LOW);
   digitalWrite(led2, LOW);
   digitalWrite(led3, LOW);
   /*digitalWrite(led4, LOW);
   digitalWrite(led5, LOW);*/
   stateL1=false;
   stateL2=false;   
   stateL3=false;   
//   stateL4=false;   
   //stateL5=false;      
  server.send(200, "text/plain", "OK");
 }
        
void setup(){
  
delay(10);
Serial.begin(115200);
Arduino.begin(2400);
//AlarmeArduino.begin(2400);

pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(led3,OUTPUT);
/*pinMode(led4,OUTPUT);
pinMode(led5,OUTPUT);*/
pinMode(pirPin, INPUT);
pinMode(speaker, OUTPUT);
digitalWrite(led1, HIGH);
digitalWrite(led2, HIGH);
digitalWrite(led3, HIGH);
//digitalWrite(led4, HIGH);
//digitalWrite(led5, HIGH);


//digitalWrite(*speakerP,LOW);

porte.attach(14);  
porte.write(0);


Serial.print("|");
Serial.print(ssid);

//Trying to connect to the WiFi
WiFi.begin(ssid, password);



while (WiFi.status() != WL_CONNECTED) {
  delay(100);
   Serial.print("*");
  }



  
//Il est possible de configurer IP address comme pour le premier code mais bon
  Serial.print("|");
  Serial.print(WiFi.localIP());
  Serial.print("|");
  IPAddress ip(192, 168, 43, 200);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);



  
// construction des routes 
  
   server.on("/temperature", getTemperatur);
   server.on("/humidite", getHumidite);
   server.on("/gaz", getGaz);
   server.on("/flamme", getFlamme);
   server.on("/presence", getPresence);
   server.on("/on/led1", AllumerLampe1);
   server.on("/off/led1", EteindreLampe1);
   server.on("/on/led2", AllumerLampe2);
   server.on("/off/led2", EteindreLampe2);
   server.on("/on/led3", AllumerLampe3);
   server.on("/off/led3", EteindreLampe3);
   /*server.on("/on/led4", AllumerLampe4);
   server.on("/off/led4", EteindreLampe4);
   server.on("/on/led5", AllumerLampe5);
   server.on("/off/led5", EteindreLampe5);*/
   server.on("/fermutePorte", FermeturePorte);
   server.on("/ouverturePorte", OuverturePorte);
   server.on("/state/led1",statL1);
   server.on("/state/led2", statL2);
   server.on("/state/led3", statL3);
   /*server.on("/state/led4", statL4);
   server.on("/state/led5", statL5);*/
   server.on("/alarme/activer",onAlarme);
   server.on("/alarme/desactiver", offAlarme);
   server.on("/led/offAll",offAllLED);
    server.on("/led/onAll",onAllLED);
   
   server.begin();
   Serial.println("HTTP server started");
   // AlarmeArduino.print(stateAlarme);


  // noTone(speaker);
  }


  
  void loop() {
   
   server.handleClient();
   
       if (Arduino.available()> 0 )
    {
        String incomingString = Arduino.readString();
        Serial.println(incomingString);
        DynamicJsonDocument root(1024);
        DeserializationError error = deserializeJson(root, incomingString);
                  if ( !error == DeserializationError::Ok)
                   Serial.println(error.c_str());
                    else{
         //Print the data in the serial monitor

         Serial.println("nouveau\n");
         
          Serial.println("");
          Serial.print("Temperature ");
          temp=root["temp"];
          Serial.println(temp);

          
          Serial.println("");
          Serial.print("Humidité ");
          humi=root["humi"];
          Serial.println(humi);

          Serial.println("");
          Serial.print("Flamme ");
          flame=root["flamme"];
          Serial.println(flame);

          Serial.println("");
          Serial.print("Gaz");
          gaz=root["gaz"];
          Serial.println(gaz);

          Pir_test();
          Serial.println("");
          Serial.print("Presence");
          Serial.printf("%d", &presence);
          
          Serial.println("fin\n\n\n");
          
    }
      while(Arduino.available()>0){
          Arduino.read();   
      }
    }
   
    
  }
     
     
      
