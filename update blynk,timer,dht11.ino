/* 

By : Rinto Harahap

Modul IoT 101
NodeMCU ESP8266

Date : 16/01/2023

*/

#define BLYNK_TEMPLATE_ID "TMPLcCYoLodH"
#define BLYNK_DEVICE_NAME "Smart Building 2"
#define BLYNK_AUTH_TOKEN "xUbK57JzIQG1gUzg_TURrciufEqIpLE_"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <time.h>



char auth[] = BLYNK_AUTH_TOKEN;

const char* ALL_SSID[] = {"RH", "realme", "ENGINEERING.TR 7", "PUBLIC_TR7", "Makassar"};
const char* ALL_PASS[] = {"makassar92", "makassar", "TIDAKKUTAUKI", "indihom3nkri", "Makassar1992"};

const int SSID_TERBACA = sizeof(ALL_SSID) / sizeof(ALL_SSID[0]);
#define trigPin D1
#define echoPin D2
#define LED_Merah D6
#define Alarm_LED D7
#define LED_Start D8
#define BUZZ D0
// #define DHT_Sens D5
#define RelayA D3
#define RelayB D4
#define DHTPIN D5

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);




BlynkTimer timer;

// WidgetLED led1(V6);
WidgetLCD lcd(V5);


long durasi;
long jarak;





void sendSensorDHT()
{
  int h = dht.readHumidity();
  int t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  if(t >= 40){
    Blynk.setProperty(V2, "color", "#FF0000");
    Blynk.logEvent("notifku2","Suhu diatas 35 Derajat celcius");
    digitalWrite(BUZZ, HIGH);
    delay(60);
    digitalWrite(BUZZ, LOW);
    delay(30);
  }else{
     Blynk.setProperty(V2, "color", "#2EFF26");
  }
  Blynk.virtualWrite(V3, h);
  Blynk.virtualWrite(V2, t);
  Serial.println(t);
  Serial.println(h);
}

void setTime(int timezone)
{
  
  Serial.print("[TIME] : Setting time using SNTP \n");

  configTime(timezone * 3600, 0, "id.pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 1000)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.printf("\n[TIME] : Current time: %s \n", asctime(&timeinfo));
}


void setup()
{

  

  boolean wifiFound = false;
  int i, n;

  // Debug console
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Penyiapan selesai");

  Serial.println(F("Pindai mulai"));
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("Pemindaian Selesai"));
  if (nbVisibleNetworks == 0) {
  Serial.println(F("Tidak ada jaringan yang ditemukan. Setel ulang untuk mencoba lagi!"));
  while (true);
  }

  Serial.print(nbVisibleNetworks);
  Serial.println(" jaringan(s) ditemukan!");


  for (i = 0; i < nbVisibleNetworks; ++i) {
  Serial.println(WiFi.SSID(i));
  for (n = 0; n < SSID_TERBACA; n++) {
  if (strcmp(ALL_SSID[n], WiFi.SSID(i).c_str())) {
  Serial.print(F("\tTidak cocok! "));
  Serial.println(ALL_SSID[n]);
  } else {
    wifiFound = true;
    break;
    }
  }
  if (wifiFound) break;
  }

  if (!wifiFound) {
  Serial.println(F("Tidak ada Jaringan Dikenal yang teridentifikasi. Setel ulang untuk mencoba lagi !"));
  while (true);

  }

  const char* ssid = (ALL_SSID[n]);
  const char* pass = (ALL_PASS[n]);
  Serial.println(WiFi.localIP());
  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk Connected");
  
  pinMode(RelayB, OUTPUT);
  pinMode(RelayA, OUTPUT);  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  pinMode(BUZZ, OUTPUT);
  pinMode(LED_Merah, OUTPUT);
  pinMode(Alarm_LED, OUTPUT);
  pinMode(LED_Start, OUTPUT);

  Startup();
  timer.setInterval(1000L, sendSensorDHT);
  setTime(8);

}


BLYNK_WRITE(V0) {
    if (param.asInt() == 1)
    {        
        digitalWrite(RelayA, LOW);
    }
    else
    {        
        digitalWrite(RelayA, HIGH);
    }
}

BLYNK_WRITE(V1)
{
    if (param.asInt() == 1)
    {      
        digitalWrite(RelayB, LOW);   
    }
    else
    {
        digitalWrite(RelayB, HIGH);        
    }
}

BLYNK_WRITE(V6)
{
    if (param.asInt() == 1)
    {    
        digitalWrite(LED_Merah, HIGH);
                
    }
    else
    {        
        digitalWrite(LED_Merah, LOW);   
    }
}

BLYNK_WRITE(V4)
{
    if (param.asInt() == 1)
    {    
        digitalWrite(BUZZ, HIGH);
                
    }
    else
    {        
        digitalWrite(BUZZ, LOW);   
    }
}




//  BLYNK_WRITE(V2)
// {
//   int pinValue = param.asInt();
//   Serial.println(pinValue);

//   if(pinValue >= 20 ){
    
    
//     Blynk.setProperty(V2, "color", "#FF0000");
//     Blynk.logEvent("notifku2","Suhu diatas 40 Derajat celcius");

//   }else{
    
//     Blynk.setProperty(V2, "color", "#2EFF26");
    
//   }
    

// }


void loop() {

  lcd.print (2, 0, "Makassar.012");
  lcd.print (1, 1, "Smart Home 1.0");

  

  
  if (digitalRead(LED_Merah) == HIGH)  {
 
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    
    durasi = pulseIn(echoPin, HIGH);
  
    
    jarak = durasi*0.034/2;
  
    
    Blynk.virtualWrite(V7, jarak);   

    
    
    
    logic();

    Serial.print("Durasi: ");
    Serial.println(durasi);
    Serial.print("Jarak: ");
    Serial.print(jarak);
    Serial.print (" cm");
    Serial.println (" ");
    delay(100);
  }
  else  {
    digitalWrite(LED_Merah, LOW);
    Serial.println("Sistem OFF");
    delay(1000);
  }


   time_t now = time(nullptr);
  struct tm* p_timeinfo = localtime(&now);
  Serial.printf("[NOW]  : Time = %s \n", asctime(p_timeinfo));
  
  if((p_timeinfo->tm_hour == 17) && (p_timeinfo->tm_min == 59)) {
    Blynk.virtualWrite(V0, 1);
    digitalWrite(RelayA, LOW);
  }

  if((p_timeinfo->tm_hour == 5) && (p_timeinfo->tm_min == 59)) {
   
    Blynk.virtualWrite(V0, 0);
    digitalWrite(RelayA, HIGH);
  }

  if((p_timeinfo->tm_hour == 18) && (p_timeinfo->tm_min == 59)) {
    Blynk.virtualWrite(V1, 1);
    digitalWrite(RelayB, LOW);
  }

  if((p_timeinfo->tm_hour == 19) && (p_timeinfo->tm_min == 59)) {
   
    Blynk.virtualWrite(V1, 0);
    digitalWrite(RelayB, HIGH);
  }

  delay(1000);

   Blynk.run();
   timer.run();
   

}

void logic() {
  if ( jarak <= 5) {
   
    Blynk.setProperty(V7, "color", "#FF0000");      
    // led1.on();
      digitalWrite(BUZZ, HIGH);
      digitalWrite(Alarm_LED, HIGH);
      delay(60);
      digitalWrite(BUZZ, LOW);
      digitalWrite(Alarm_LED, LOW);
      delay(30);

    Serial.println(" -5cm");
  }

  else if (( jarak > 5 ) && ( jarak < 15)) {

    Blynk.setProperty(V7, "color", "#FFE600");   
      digitalWrite(BUZZ, HIGH);
      digitalWrite(Alarm_LED, HIGH);
      delay(400);
      digitalWrite(BUZZ, LOW);
      digitalWrite(Alarm_LED, LOW);
      delay(400);
    
    Serial.println ( " 6 - 15cm");

  }

  else if ( jarak >= 15) {
    Blynk.setProperty(V7, "color", "#2EFF26");   
    delay(200);
    Serial.println ( " 15cm keatas");

  }
}


void sendSensor() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
    
  durasi = pulseIn(echoPin, HIGH);
    
  jarak = durasi*0.034/2;
  
  Blynk.virtualWrite(V7, jarak);     
}



void Startup()  {

  for(int i = 0; i <= 3; i++){
    digitalWrite(LED_Start, HIGH);
    digitalWrite(BUZZ, HIGH);
    delay(500);
    digitalWrite(LED_Start, LOW);
    digitalWrite(BUZZ, LOW);
    delay(500);
  }
}








