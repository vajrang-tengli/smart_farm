#define BLYNK_TEMPLATE_ID "TMPLVbgkloQv"
#define BLYNK_DEVICE_NAME "SMARTFARMING"
#define BLYNK_AUTH_TOKEN "uaLsQu5xWWlJlWyXMY-hK-i5RQ22etfD"
#define BLYNK_PRINT Serial
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#include<ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define APP_DEBUG
#define ssid "vajrang"
#define password "chimmi@2009"
#include <MQ135.h>
#include <DHTesp.h>
#define DHTpin 14 //DHT11 at Digital Pin D5
DHTesp dht;
int pump=15;
// mq135 at A0
int val1= 4; // valve 1 Indicator at Digital PIN D2
int val2=5; // valve 2 Indicator at Digital PIN D1
int Sense_pin1=16; // Soil Sensor input at Digital Pin D0
int Sense_pin2=2; // Soil Sensor input at Digital Pin D4
int value1;
int value2;
/*********************************************************************************/
void setup () {
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(15,OUTPUT);
  Serial. begin (9600) ;
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP ADDRESS: ");
  Serial.println(WiFi.localIP()); 
  pinMode (val1, OUTPUT) ;
  pinMode (val2, OUTPUT) ;
  Serial.println ("SMART FARMING") ;
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  dht.setup(DHTpin, DHTesp::DHT11);
  delay (2000) ;
}
/****************************************************************/
void loop() {
  Blynk.run();
  value1= digitalRead (Sense_pin1) ;
  value2= digitalRead (Sense_pin2) ;
   Serial . println (value1) ;
  Serial . println (value2) ;
  if (value1!=0)
  {
    //digitalWrite (val1, LOW);
    Blynk.virtualWrite(V5,"Field 1 DRY"); 
  }
  else
  {
    //digitalWrite (val1, HIGH);
    Blynk.virtualWrite(V5,"Field 1 WET");
  }
  if (value2!=0)
  {
    //digitalWrite (val2, LOW);
    Blynk.virtualWrite(V6,"Field 2 DRY"); 
  }
  else
  {
    //digitalWrite (val2, HIGH);
    Blynk.virtualWrite(V6,"Field 2 WET");
  }
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
  MQ135 gasSensor = MQ135(0);
  float air_quality = gasSensor.getPPM();
  Serial.print("Air Quality: ");  
  Serial.print(air_quality);
  Serial.println("  PPM");   
  Serial.println();
  //delay(dht.getMinimumSamplingPeriod());
  Blynk.virtualWrite(V2,temperature);
  Blynk.virtualWrite(V3,humidity);
  Blynk.virtualWrite(V4,air_quality);
  delay(1000);
}
/**********************************************************************/
BLYNK_WRITE(V0)//valve 1
{
  if(param.asInt()==1){
    digitalWrite(4,LOW);
  }
  else{
    digitalWrite(4,HIGH);
 }
}
/*********************************************************************/
BLYNK_WRITE(V1)// valve 2
{
  if(param.asInt()==1){
    digitalWrite(5,LOW);
  }
  else{
    digitalWrite(5,HIGH);
  }
}
/*****************************************/
BLYNK_WRITE(V7)//PUMP
{
  if(param.asInt()==1){
    digitalWrite(15,LOW);
  }
  else{
    digitalWrite(15,HIGH);
  }
}
/******************************************/
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6); 
  Blynk.syncVirtual(V7);
}
