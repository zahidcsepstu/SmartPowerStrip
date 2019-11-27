#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "oRN6n6WyTnZ9r6XwCdnubT9wvtfv9-rF";
char ssid[] = "PSTU-Students";
char pass[] = "PSTU#2019";

 

WidgetLED led1(V0);
WidgetLED led2(V3);

long prevtime1;
long currenttime;
long TimeinSec1;
bool flag1=false;

long prevtime2;
long TimeinSec2;
bool flag2=false;

//port 1 scheduled on/off...........

BLYNK_WRITE(V2)
{
  if(param.asInt() == 0)
     digitalWrite(D0, HIGH);
  if(param.asInt() == 1)
     digitalWrite(D0, LOW);
}



//port 2 scheduled on/off...........

BLYNK_WRITE(V5)
{
  if(param.asInt() == 0)
     digitalWrite(D1, HIGH);
  if(param.asInt() == 1)
     digitalWrite(D1, LOW);
}

//port1 timer
BLYNK_WRITE(V1) {
  TimeinSec1 = param[0].asLong()/60;
  if(TimeinSec1>0){
    Blynk.notify("Timer is set for port 1");
    digitalWrite(D0,LOW);
    prevtime1=millis()/1000; 
    flag1=true;
  }
  else
    digitalWrite(D0,HIGH);
}

//port2 timer
BLYNK_WRITE(V4) {
  TimeinSec2 = param[0].asLong()/60;
  if(TimeinSec2>0){
    Blynk.notify("Timer is set for port 2");
    digitalWrite(D1,LOW);
    prevtime2=millis()/1000; 
    flag2=true;
  }
  else
    digitalWrite(D1,HIGH);
}




void checkTimer1()
{
  currenttime=millis()/1000;
  if(currenttime-prevtime1>=TimeinSec1)
   {
    digitalWrite(D0,HIGH);
    Blynk.notify("Port 1 is Turned off");
    flag1=false;
   }
}


void checkTimer2()
{
  currenttime=millis()/1000;
  if(currenttime-prevtime2>=TimeinSec2)
   {
    digitalWrite(D1,HIGH);
    Blynk.notify("Port 2 is Turned off");
    flag2=false;
   }
}





void setup()
{

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  // Debug console
  Serial.begin(9600);
  //pinMode(D4,OUTPUT);
 
  Blynk.begin(auth, ssid, pass);
  Blynk.notify("Smart power strip is connected");
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

bool wi;
void loop()
{
  Blynk.run();
  if(!WL_CONNECTED)
  wi=true;

  if(WL_CONNECTED&&wi)
  {
    Blynk.notify("online");
    wi=false;
  }
  Serial.println(WL_CONNECTED);
  Serial.println(WL_IDLE_STATUS);
  
  delay(1000);
  
  int state1 = digitalRead(D0);
  if(state1==0)
  led1.on();
  else
  led1.off();

  int state2 = digitalRead(D1);
  if(state2==0)
  led2.on();
  else
  led2.off();
 
  if(flag1)
  checkTimer1();
  if(flag2)
  checkTimer2();
}
