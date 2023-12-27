#include <WiFi.h>
#include <WiFiClient.h> 
#include <Wire.h>
#include "RTClib.h"
#include <HTTPClient.h>        
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;


RTC_DS3231 rtc;
WiFiClient client;
const int httpPort = 80;
const char* host2 = "www.google.com";
int google = 0;
int wififlag =0;
int lenght =0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


int trigger = 19;

const char* ssid     = "JARVIS";//"NETGEAR";//"belkin.36c9";//"Intramex_Airport_1";//"belkin.36c9";//"GUESTHOUSE";
const char* password = "a1ucimg8";//"acpl@123";//"3eb4669f";//"#115#4#b@b";//"3eb4669f";//"guesthouse";


void setup() {
  pinMode(trigger, OUTPUT);     //trigger pin action
  digitalWrite(trigger, LOW);
  Serial.begin(115200);
  rtc.begin();

  

  for (uint8_t t = 4; t > 0; t--) {
     Serial.printf("[SETUP] WAIT %d...\n", t);
     Serial.flush();
     delay(100);
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print("...");
    if (WiFi.status() != WL_CONNECTED) { 
      loop();
    } 
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(19800);
}

void loop() {
getTime();
delay(5000);
}

void getDataFromNtp(){
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);
}
void getTimeFromRtc(){
   DateTime now = rtc.now();
  Serial.print(now.hour(), DEC);       // uncommnet to see time in serial.
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print("  ");
  Serial.print(now.day(), DEC);
  Serial.println(" ");
  Serial.print(" DATE:");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.println();

  delay(5000);
}



void getTime() {
        if(WiFi.status() == WL_CONNECTED) {
          while(!timeClient.update()) {
            timeClient.forceUpdate();
          }
          // The formattedDate comes with the following format:
          // 2018-05-28T16:00:13Z
          // We need to extract date and time
          formattedDate = timeClient.getFormattedDate();
          Serial.println(formattedDate);

          // Extract date
          int splitT = formattedDate.indexOf("T");
          dayStamp = formattedDate.substring(0, splitT);
          Serial.print("DATE: ");
          Serial.println(dayStamp);
          // Extract time
          timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
          Serial.print("HOUR: ");
          Serial.println(timeStamp);
          delay(1000);
           
      } else {
        DateTime now = rtc.now();
        Serial.print(" TIME:");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
        Serial.print(" DATE:");
        Serial.print(now.day(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.year(), DEC);
        delay(500);  
      }
  }

  void getWiFiConnected(){
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(100);
      Serial.println("WiFi NOT FOUND"); 
      wififlag=0;
      delay(10);
    } 
  }