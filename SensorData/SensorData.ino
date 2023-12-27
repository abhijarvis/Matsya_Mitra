#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#define SD_CS 5
#include "RTClib.h"
RTC_DS3231 RTC;

const char* ssid     = "JARVIS";//"NETGEAR";//"belkin.36c9";//"Intramex_Airport_1";//"belkin.36c9";//"GUESTHOUSE";
const char* password = "a1ucimg8";//"acpl@123";//"3eb4669f";//"#115#4#b@b";//"3eb4669f";//"guesthouse";

// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0xC0, 0x38, 0x57, 0x4, 0xE1, 0x3D, 0x9 };
DeviceAddress sensor2 = { 0x28, 0x74, 0x69, 0x57, 0x4, 0xE1, 0x3D, 0x8A };
DeviceAddress sensor3= { 0x28, 0x3D, 0x2E, 0x57, 0x4, 0xE1, 0x3D, 0xD1 };
DeviceAddress sensor4= { 0x28, 0x73, 0xCD, 0x57, 0x4, 0xE1, 0x3D, 0x23 };


#define WifiConnect 2
#define internetFlag 16
#define buzzer 15

void setup(void){
  Serial.begin(115200);
  sensors.begin();
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
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed"); 
    delay(1000); 
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!"); 
    return; 
  }

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 
  rtc.adjust(DateTime(__DATE__, __TIME__));
 

}

void loop(void){ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Sensor 1(*C): ");
  Serial.print(sensors.getTempC(sensor1)); 
  Serial.print(" Sensor 1(*F): ");
  Serial.println(sensors.getTempF(sensor1)); 
 
  Serial.print("Sensor 2(*C): ");
  Serial.print(sensors.getTempC(sensor2)); 
  Serial.print(" Sensor 2(*F): ");
  Serial.println(sensors.getTempF(sensor2)); 
  
  Serial.print("Sensor 3(*C): ");
  Serial.print(sensors.getTempC(sensor3)); 
  Serial.print(" Sensor 3(*F): ");
  Serial.println(sensors.getTempF(sensor3)); 
  
  Serial.print("Sensor 4(*C): ");
  Serial.print(sensors.getTempC(sensor4)); 
  Serial.print(" Sensor 4(*F): ");
  Serial.println(sensors.getTempF(sensor4)); 

  delay(2000);
}