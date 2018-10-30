#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
#include<TinyGPS.h>
char auth[] = "auth";
char ssid[] = "name";
char pass[] = "********";
int IrPin=11;
int sendFlag=0;
long lat,lon;
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX
SoftwareSerial GPS(4,5);
TinyGPS gps;
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);
SimpleTimer timer;
void irSensor()
{
  int val;
  Serial.println("Detecting Nearby objects");
  val=digitalRead(IrPin);
  if(val==HIGH)
  {
    if(sendFlag==0)
    {
      Serial.println("Detected Movement");
      digitalWrite(7,HIGH);
      delay(1000);
      digitalWrite(7,LOW);
      delay(1000);
      digitalWrite(7,HIGH);
      delay(1000);
      digitalWrite(7,LOW);
      delay(1000);
      Blynk.notify("Someone is approaching.");
      sendFlag=1;
    }    
  }
  else
    Serial.println("No movement detected");
}
void gpsSend()
{
 Serial.println("Sending Location");
  String loc="The location is ";
  Serial.print(loc);
  Serial.print((lat*0.000001),8);
  Serial.print(",");
  Serial.print((lon*0.000001),8);
  Serial.print("\n");
  Blynk.virtualWrite(5,lat*0.000001);
  Blynk.virtualWrite(6,lon*0.000001);
}
void setup()
{
  Serial.begin(9600);
  delay(10);
  GPS.begin(9600);
  delay(10);
  EspSerial.begin(9600);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(20000, irSensor);
  timer.setInterval(30000,gpsSend);
  pinMode(IrPin,INPUT);
  pinMode(8,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(8,LOW);
  sendFlag=0;
}
void loop()
{
  byte a;
  if(GPS.available())  
  {
    GPS.listen();
    a=GPS.read();
    while(gps.encode(a))
      gps.get_position(&lat,&lon);
  }
  if(!Blynk.connected())
    Blynk.begin(auth,wifi,ssid,pass);
  EspSerial.listen();
  Blynk.run();
  timer.run();
}
