#include <WiFi.h>
#include <RTClib.h>
RTC_DS3231 rtc;
// Need to check again
unsigned long MyTestTimer = 0;                   // variables MUST be of type unsigned long
const byte    OnBoard_LED = 2;

//const char *ssid     = "WLANBuero_EXT"; 
const   char *ssid     = "JARVIS";

const char *password = "a1ucimg8";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 19800;

#include <time.h>                   // time() ctime()
time_t now;                         // this is the epoch
tm timeinfo;                      // the structure tm holds time information in a more convient way

void showTime() {
  time(&now);                       // read the current time
  localtime_r(&now, &timeinfo);           // update the structure tm with the current time
  Serial.print("year:");
  Serial.print(timeinfo.tm_year + 1900);  // years since 1900
  Serial.print("\tmonth:");
  Serial.print(timeinfo.tm_mon + 1);      // January = 0 (!)
  Serial.print("\tday:");
  Serial.print(timeinfo.tm_mday);         // day of month
  Serial.print("\thour:");
  Serial.print(timeinfo.tm_hour);         // hours since midnight  0-23
  Serial.print("\tmin:");
  Serial.print(timeinfo.tm_min);          // minutes after the hour  0-59
  Serial.print("\tsec:");
  Serial.print(timeinfo.tm_sec);          // seconds after the minute  0-61*
  Serial.print("\twday");
  Serial.print(timeinfo.tm_wday);         // days since Sunday 0-6
  if (timeinfo.tm_isdst == 1)             // Daylight Saving Time flag
    Serial.print("\tDST");
  else
    Serial.print("\tstandard");
    
  Serial.println();
}

void connectToWifi() {
  Serial.print("Connecting to "); 
  Serial.println(ssid);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    BlinkHeartBeatLED(OnBoard_LED, 333);
    delay(2000);
    Serial.print(".");
  }
  Serial.print("\n connected.");
  Serial.println(WiFi.localIP() );

}

void synchroniseWith_NTP_Time() {
  Serial.print("configTime uses ntpServer ");
  Serial.println(ntpServer);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("synchronising time");
  
  while (timeinfo.tm_year + 1900 < 2000 ) {
    time(&now);                       // read the current time
    localtime_r(&now, &timeinfo);
    BlinkHeartBeatLED(OnBoard_LED, 100);
    delay(100);
    Serial.print(".");
  }
  Serial.print("\n time synchronsized \n");
  showTime();    
}


void PrintFileNameDateTime() {
  Serial.println( F("Code running comes from file ") );
  Serial.println(__FILE__);
  Serial.print( F("  compiled ") );
  Serial.print(__DATE__);
  Serial.print( F(" ") );
  Serial.println(__TIME__);  
}

boolean TimePeriodIsOver (unsigned long &periodStartTime, unsigned long TimePeriod) {
  unsigned long currentMillis  = millis();  
  if ( currentMillis - periodStartTime >= TimePeriod )
  {
    periodStartTime = currentMillis; // set new expireTime
    return true;                // more time than TimePeriod) has elapsed since last time if-condition was true
  } 
  else return false;            // not expired
}


void BlinkHeartBeatLED(int IO_Pin, int BlinkPeriod) {
  static unsigned long MyBlinkTimer;
  pinMode(IO_Pin, OUTPUT);
  
  if ( TimePeriodIsOver(MyBlinkTimer,BlinkPeriod) ) {
    digitalWrite(IO_Pin,!digitalRead(IO_Pin) ); 
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("\n Setup-Start \n");
  PrintFileNameDateTime();
  
  //connectToWifi();
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
  if (WiFi.status() == WL_CONNECTED) {
    synchroniseWith_NTP_Time();
  }
  if (! rtc.begin()) {
    Serial.println("RTC module is NOT found");
    Serial.flush();
    while (1);
  }
  rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
}

void loop() {
  BlinkHeartBeatLED(OnBoard_LED,100);
  if(WiFi.status()!= WL_CONNECTED) {
    WiFi.begin(ssid, password);
    GetRTCdata();
  } else {
    Serial.println("Wifi Connected ");
  }
  
}

void GetRTCdata(){
  DateTime now = rtc.now();
  Serial.print("ESP32 RTC Date Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" "); 
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

  delay(1000); // delay 1 seconds
}