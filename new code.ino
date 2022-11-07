
#define BLYNK_TEMPLATE_ID "TMPL1ov1xbJZ"
#define BLYNK_DEVICE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "bG6ROuRQ4qPRS_3TOy3l5__kOvtGR7-4"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "RH";
char pass[] = "makassar92";

#define trigPin D1
#define echoPin D2
#define LEDsystem D6
#define LEDalarm D7
#define LEDH D8
#define BUZZ D0

BlynkTimer timer;

WidgetLED led1(V6);
WidgetLCD lcd(V7);

long duration;
long distance;

void setup()
{
  Serial.begin(115200);
 
  pinMode(2, OUTPUT); // Initialise digital pin 2 as an output pin
  pinMode(0, OUTPUT); // Initialise digital pin 2 as an output pin

  Blynk.begin(auth, ssid, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(BUZZ, OUTPUT);
  pinMode(LEDsystem, OUTPUT);
  pinMode(LEDalarm, OUTPUT);
  pinMode(LEDH, OUTPUT);

  Startup();

}


BLYNK_WRITE(V0) // PIN LAMPU BLYNK
    if (param.asInt() == 1)
    {        
        digitalWrite(2, LOW);
    }
    else
    {        
        digitalWrite(2, HIGH);
    }
}

BLYNK_WRITE(V1) //PIN STOP KONTAK BLYNK 
{
    if (param.asInt() == 1)
    {      
        digitalWrite(0, LOW);   
    }
    else
    {
        digitalWrite(0, HIGH);        
    }
}

BLYNK_WRITE(V4) // PIN TOMBOL MONITOR JARAK BLYNK
{
    if (param.asInt() == 1)
    {    
        digitalWrite(LEDsystem, HIGH);
                
    }
    else
    {        
        digitalWrite(LEDsystem, LOW);   
    }
}

void loop() {
  
  sendDHT();

  Blynk.run();
  if (digitalRead(LEDsystem) == HIGH)  {
    
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);   
        duration = pulseIn(echoPin, HIGH);
        //KALKULASI JARAK
        distance = duration*0.034/2;  
        //KIRIM DATA KE BLYNK
        Blynk.virtualWrite(V5, distance);   
        //KIRIM DATA KE LCD BLYNK
        lcd.print (3, 0, "Makassar012");    
        lcd.print (1, 1, "Smart Home 101");
        timer.run();
        logic();
        Serial.print("Durasi: ");
        Serial.println(duration);
        Serial.print("Jarak: ");
        Serial.print(distance);
        Serial.print (" cm");
        Serial.println (" ");
        delay(100);
  }else  {
        digitalWrite(LEDsystem, LOW); //pembaruan
        Serial.println("Sistem OFF");
        delay(1000);
  }

}


void logic() {
  if ( distance <= 5) {

    Blynk.setProperty(V6, "color", "#FF0000");
    Blynk.setProperty(V5, "color", "#FF0000");
    led1.on()
    digitalWrite(BUZZ, HIGH);
    digitalWrite(LEDalarm, HIGH);
    delay(60);
    digitalWrite(BUZZ, LOW);
    digitalWrite(LEDalarm, LOW);
    delay(30);    
    Serial.println(" BAHAYA");
  }

  else if (( distance > 5 ) && ( distance < 15)) {

    Blynk.setProperty(V6, "color", "#FFE600");
    Blynk.setProperty(V5, "color", "#FFE600");
    led1.on();
    digitalWrite(BUZZ, HIGH);
    digitalWrite(LEDalarm, HIGH);
    delay(400);
    digitalWrite(BUZZ, LOW);
    digitalWrite(LEDalarm, LOW);
    delay(400);
    Serial.println ( " KURANG AMAN");

  }

  else if ( distance >= 15) {
    Blynk.setProperty(V6, "color", "#2EFF26");
    Blynk.setProperty(V5, "color", "#2EFF26");
    led1.on();
    delay(200);
    Serial.println ( " JAUH");

  }
}


void sendSensor() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);    
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);    
  duration = pulseIn(echoPin, HIGH);    
  distance = duration*0.034/2;      
}


void Startup()  {

  for(int i = 0; i <= 3; i++){
    digitalWrite(LEDH, HIGH);
    digitalWrite(BUZZ, HIGH);
    delay(500);
    digitalWrite(LEDH, LOW);
    digitalWrite(BUZZ, LOW);
    delay(500);
  }
}

void sendDHT(){
  
  Blynk.virtualWrite(V2, 22);
  Blynk.virtualWrite(V3, 70);

}


