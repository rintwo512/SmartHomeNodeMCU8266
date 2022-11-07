/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL1ov1xbJZ"
#define BLYNK_DEVICE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "bG6ROuRQ4qPRS_3TOy3l5__kOvtGR7-4"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
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
  // Debug console
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


BLYNK_WRITE(V0) // Executes when the value of virtual pin 0 changes
{
    if (param.asInt() == 1)
    {        
        digitalWrite(2, LOW); // Set digital pin 2 LOW
    }
    else
    {        
        digitalWrite(2, HIGH); // Set digital pin 2 HIGH
    }
}

BLYNK_WRITE(V1)
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

BLYNK_WRITE(V4)
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
    
    //Set the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    //Read the echoPin, return to sound wave travel time to microseconds
    duration = pulseIn(echoPin, HIGH);
  
    //Calculating the distance
    distance = duration*0.034/2; //s=t*v
  
    //kirim data ke blynk
    Blynk.virtualWrite(V5, distance);   

    //LCD Di Blynk    
    
    lcd.print (3, 0, "Makassar012");
    // lcd.print (0, 1, "Jarak : " + String(distance) + " cm");
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
  }
  else  {
    digitalWrite(LEDsystem, LOW); //pembaruan
    Serial.println("Sistem OFF");
    delay(1000);
  }

}


//====================== Logika Jarak ==============================
void logic() {
  if ( distance <= 5) {

    Blynk.setProperty(V6, "color", "#FF0000"); //warna merah
    Blynk.setProperty(V5, "color", "#FF0000"); //warna merah
    led1.on();
      digitalWrite(BUZZ, HIGH);
      digitalWrite(LEDalarm, HIGH);
      delay(60);
      digitalWrite(BUZZ, LOW);
      digitalWrite(LEDalarm, LOW);
      delay(30);
    //}
    Serial.println(" MEPET");
  }

  else if (( distance > 5 ) && ( distance < 15)) {

    Blynk.setProperty(V6, "color", "#FFE600"); // warna kuning
    Blynk.setProperty(V5, "color", "#FFE600"); // warna kuning
    led1.on();
      digitalWrite(BUZZ, HIGH);
      digitalWrite(LEDalarm, HIGH);
      delay(400);
      digitalWrite(BUZZ, LOW);
      digitalWrite(LEDalarm, LOW);
      delay(400);
    //}
    Serial.println ( " DEKAT");

  }

  else if ( distance >= 15) {
    Blynk.setProperty(V6, "color", "#2EFF26"); //warna hijau
    Blynk.setProperty(V5, "color", "#2EFF26"); //warna hijau
    led1.on();
    delay(200);
    Serial.println ( " JAUH");

  }
}
//====================================================================

//======================= Kirim Data Sensor ==========================
void sendSensor() {
  //clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  //Set the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //Read the echoPin, return to sound wave travel time to microseconds
  duration = pulseIn(echoPin, HIGH);
  
  //Calculating the distance
  distance = duration*0.034/2; //s=t*v
  
  //kirim data ke blynk
  Blynk.virtualWrite(V5, distance);   

  //LCD Di Blynk
  // lcd.print (0, 0, "Makassar012");
  // lcd.print (0, 1, "Jarak : " + String(distance) + " cm");
  // lcd.print (0, 1, "Modul IoT 101");
}
//====================================================================

//================== Bunyi Awal Saat Tersambung ======================
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


