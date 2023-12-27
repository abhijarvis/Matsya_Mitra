#include <Keypad.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
//#include <ESP32Ping.h>
// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#define SD_CS 5
int lenght = 0;
int secureKey = 0;
int lcdColumns = 20;//16
int lcdRows = 4;//2
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
const char* ssid     = "JARVIS";//"NETGEAR";//"belkin.36c9";//"Intramex_Airport_1";//"belkin.36c9";//"GUESTHOUSE";
const char* password = "a1ucimg8";//"acpl@123";//"3eb4669f";//"#115#4#b@b";//"3eb4669f";//"guesthouse";
const char* host = "213.157.12.84";
const char* host2 = "www.google.com";
HardwareSerial Serial32(2);
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
#include <FPM.h>
const byte ROWS = 4; 
const byte COLS = 4; 
/*char keys[ROWS][COLS] = 
{
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};*/
//Membrane Keypad
/*
char keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};*/
//PCB KeypadLayout

char keys[ROWS][COLS] = 
{
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};
byte rowPins[ROWS] = {27, 14, 12, 13};
byte colPins[COLS] = {32, 33, 25, 26};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;
int i1,i2,i3,i4;
char c1,c2,c3,c4;
DateTime now;
WiFiClient client;
const int httpPort = 80;
int result;
int tempresult;
#define match 2
#define indFinger 4
#define buzzer 15
int32_t test;
int addr = 0;
int countoff = 0;
int google = 0;
File file;
String results[200];
int Machine_id = 45;
int wififlag,Empflag,internetFlag,serverFlag;
String dataMessage;
String total;
String resultd;
const int row_1 = 27;
const int row_2 = 14;
const int row_3 = 12;
const int row_4 = 13;
const int col_1 = 32;
const int col_2 = 33;
const int col_3 = 25;
const int col_4 = 26;
FPM finger(&Serial32);
FPM_System_Params params;
int32_t fid;
uint32_t template_cnt;
uint32_t templatecount;

void setup() {
  delay(1000);
  lcd.begin(20,4);
  Serial.begin(9600);
  /* all colums inout high */
  pinMode(col_1, INPUT_PULLUP);
  pinMode(col_2, INPUT_PULLUP);
  pinMode(col_3, INPUT_PULLUP);
  pinMode(col_4, INPUT_PULLUP);
  /* all rows output HIGH */
  pinMode(row_1, OUTPUT);
  pinMode(row_2, OUTPUT);
  pinMode(row_3, OUTPUT);
  pinMode(row_4, OUTPUT);
  lcd.init();                  
  lcd.backlight();
  delay(100);
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  pinMode(match, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(indFinger, OUTPUT);
  digitalWrite(buzzer, LOW);
  Serial.print("Machine Id : ");
  Serial.println(Machine_id);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(" ATTENDANCE SYSTEM ");
  Serial.println("ATTENDANCE SYSTEM");
  lcd.setCursor(0,2);
  lcd.print("ACCENTUS CONSULTING");
  Serial.println("ACCENTUS CONSULTING");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(" PRIME  HR SOLUTION ");
  Serial.println("PRIME  HR SOLUTION");
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("PRIME ERP");
  Serial.println(" PRIME ERP ");
  lcd.setCursor(0,2);
  lcd.print("Enterprise Solution");
  Serial.println("Enterprise Solution");
  delay(5000);
  Serial32.begin(57600);
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("  FINDING MODULE  ");
  Serial.println("Finding Module");
  lcd.setCursor(0,1);
  delay(5000);
   if (finger.begin()) {
        finger.readParams(&params);
        Serial.println("Found fingerprint sensor!");
        Serial.print("Capacity: "); Serial.println(params.capacity);
        Serial.print("Packet length: "); Serial.println(FPM::packet_lengths[params.packet_len]);
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("    MODULE FOUND   ");
        delay(1000);
        lcd.clear();
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("  MODULE NOT FOUND  ");
        lcd.setCursor(0,2);
        lcd.print(" CHECK CONNECTIONS  ");
        Serial.println("Check Connections");
        while (1) yield();
    }
  lcd.setCursor(0,1);
  lcd.print("INITIALIZING SYSTEM");
  Serial.println("Start System");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(" INITIALIZING WiFi ");
  delay(1000);
  lcd.clear();
  Serial.print("Connecting to WiFi ");
  Serial.println(ssid);
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
  lcd.clear();
  lcd.print("WiFi CONNECTED TO   ");
  lcd.setCursor(0,1);
  lcd.print(WiFi.SSID());
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("INITIALIZING SD CARD");
  delay(2000);
  lcd.clear();
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(" No SD Card Found ");
    delay(1000);
    lcd.clear();
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
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(" SD CARD INIT. FAIL ");
    return; 
  }
  delay(1000);
}

void loop() {
      Serial.println("Machine ID - > "+String(Machine_id));
      Serial.print("Scan Finger - ");
      lcd.setCursor(0,0);
      lcd.print(" ATTENDANCE  SYSTEM ");
      lcd.setCursor(0,1);
      lcd.setCursor(0,2);
      getTime();

      if(google==0 && wififlag == 1){
            lcd.clear();
            lcd.setCursor(0,2);
            lcd.print("   Internet Down !");
            lcd.setCursor(0,3);
            lcd.print("   Contact IT !");
            Serial.println("Google Value - > "+String(google));
      }
      //Serial.println("Length -> "+String(lenght));
      if ((lenght < 18) || (lenght > 18)){
         if (WiFi.status() == WL_CONNECTED) { 
            Serial.println("Server Not Avilable");
            lcd.clear();
            lcd.setCursor(0,2);
            lcd.print("   Server Down !");
            lcd.setCursor(0,3);
            lcd.print("   Contact IT !");
            internetFlag=0;
            Serial.println("Internet value = "+String(internetFlag));
            delay(1000);
            lcd.clear();
            DateTime now = RTC.now();
            lcd.setCursor(0,2);
            lcd.print(" TIME:");
            lcd.print(now.hour(), DEC);
            lcd.print(':');
            lcd.print(now.minute(), DEC);
            lcd.print(':');
            lcd.print(now.second(), DEC);
            lcd.setCursor(0,3);
            lcd.print(" DATE:");
            lcd.print(now.day(), DEC);
            lcd.print('/');
            lcd.print(now.month(), DEC);
            lcd.print('/');
            lcd.print(now.year(), DEC);
            delay(500);
          }
        } else {
          internetFlag=1;
          Serial.println("Internet values = "+String(internetFlag));
        }
    if (key == '*') { 
        lcd.clear();
        lcd.print("      SETUP     ");
        delay(1000);
        lcd.clear();
        lcd.print("   SETUP YEAR   ");
        char keypressed2 = keypad.waitForKey();  
        if (keypressed2 != NO_KEY && keypressed2 !='*' && keypressed2 !='#' && keypressed2 !='A' && keypressed2 !='B' && keypressed2 !='C' && keypressed2 !='D' ) {
           c1 = keypressed2;
           lcd.setCursor(0, 1);
           lcd.print(c1);
        }
        char  keypressed3 = keypad.waitForKey();
        if (keypressed3 != NO_KEY && keypressed3 !='*' && keypressed3 !='#' && keypressed3 !='A' && keypressed3 !='B' && keypressed3 !='C' && keypressed3 !='D' ) {
           c2 = keypressed3;
           lcd.setCursor(1, 1);
           lcd.print(c2);
        }
       char  keypressed4 = keypad.waitForKey();
       if (keypressed4 != NO_KEY && keypressed4 !='*' && keypressed4 !='#' && keypressed4 !='A' && keypressed4 !='B' && keypressed4 !='C' && keypressed4 !='D' ) {
           c3 = keypressed4;
           lcd.setCursor(2, 1);
           lcd.print(c3);
       }
       char   keypressed5 = keypad.waitForKey();
       if (keypressed5 != NO_KEY && keypressed5 !='*' && keypressed5 !='#' && keypressed5 !='A' && keypressed5 !='B' && keypressed5 !='C' && keypressed5 !='D' ) {
           c4 = keypressed5;
           lcd.setCursor(3, 1);
           lcd.print(c4);
       }
       i1=(c1-48)*1000;       
       i2=(c2-48)*100;
       i3=(c3-48)*10;
       i4=c4-48;
       int N_year=i1+i2+i3+i4;
       delay(500);
       lcd.clear();
       lcd.print("   SETUP MONTH   ");
      char keypressed6 = keypad.waitForKey();
      if (keypressed6 != NO_KEY && keypressed6 !='*' && keypressed6 !='#' && keypressed6 !='A' && keypressed6 !='B' && keypressed6 !='C' && keypressed6 !='D' ) {
         c1 = keypressed6;
         lcd.setCursor(0, 1);
         lcd.print(c1);
      }
      char   keypressed7 = keypad.waitForKey();
      if (keypressed7 != NO_KEY && keypressed7 !='*' && keypressed7 !='#' && keypressed7 !='A' && keypressed7 !='B' && keypressed7 !='C' && keypressed7 !='D' ) {
         c2 = keypressed7;
         lcd.setCursor(1, 1);
         lcd.print(c2);
       }
       i1=(c1-48)*10;
       i2=c2-48;
       int N_month=i1+i2;
       delay(500);
       lcd.clear();
       lcd.print("    SETUP DAY    ");                  
      char keypressed8 = keypad.waitForKey(); 
      if (keypressed8 != NO_KEY && keypressed8 !='*' && keypressed8 !='#' && keypressed8 !='A' && keypressed8 !='B' && keypressed8 !='C' && keypressed8 !='D' ) {
          c1 = keypressed8;
         lcd.setCursor(0, 1);
         lcd.print(c1);
       }
      char keypressed9 = keypad.waitForKey();
      if (keypressed9 != NO_KEY && keypressed9 !='*' && keypressed9 !='#' && keypressed9 !='A' && keypressed9 !='B' && keypressed9 !='C' && keypressed9 !='D' ) {
          c2 = keypressed9;
         lcd.setCursor(1, 1);
         lcd.print(c2);
       }
       i1=(c1-48)*10;
       i2=c2-48;
       int N_day=i1+i2;
       delay(500);
       lcd.clear();
       lcd.print("Setup hour");                
        char keypressed10 = keypad.waitForKey();
        if (keypressed10 != NO_KEY && keypressed10 !='*' && keypressed10 !='#' && keypressed10 !='A' && keypressed10 !='B' && keypressed10 !='C' && keypressed10 !='D' ) {
           c1 = keypressed10;
           lcd.setCursor(0, 1);
           lcd.print(c1);
        }
        char   keypressed11 = keypad.waitForKey();
        if (keypressed11 != NO_KEY && keypressed11 !='*' && keypressed11 !='#' && keypressed11 !='A' && keypressed11 !='B' && keypressed11 !='C' && keypressed11 !='D' ) {
            c2 = keypressed11;
           lcd.setCursor(1, 1);
           lcd.print(c2);
         }
         i1=(c1-48)*10;
         i2=c2-48;
         int N_hour=i1+i2;
         delay(500);
         lcd.clear();
         lcd.print("Setup minutes");
        char keypressed12 = keypad.waitForKey();
        if (keypressed12 != NO_KEY && keypressed12 !='*' && keypressed12 !='#' && keypressed12 !='A' && keypressed12 !='B' && keypressed12 !='C' && keypressed12 !='D' ) {
            c1 = keypressed12;
           lcd.setCursor(0, 1);
           lcd.print(c1);
         }
        char    keypressed13 = keypad.waitForKey();
        if (keypressed13 != NO_KEY && keypressed13 !='*' && keypressed13 !='#' && keypressed13 !='A' && keypressed13 !='B' && keypressed13 !='C' && keypressed13 !='D' ) {
           c2 = keypressed13;
           lcd.setCursor(1, 1);
           lcd.print(c2);
        }
         i1=(c1-48)*10;
         i2=c2-48;
         int N_minutes=i1+i2;
         delay(500);
         lcd.clear();
        RTC.adjust(DateTime(N_year, N_month, N_day, N_hour, N_minutes, 22));
        key=NO_KEY;
    }
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(100);
      Serial.println("WiFi NOT FOUND");
      lcd.clear();
      lcd.print("   WiFi NOT FOUND   ");
      wififlag=0;
      delay(10);
    } 
    result= getFingerprintIDez();
    if(result>0) {
        digitalWrite(indFinger, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(800);
        digitalWrite(buzzer, LOW);
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("   ");
        lcd.print("ID:");
        Serial.print("ID:");
        lcd.print(result);
        delay(1000);
        lcd.clear();
        Serial.println(result);
        if (WiFi.status() == WL_CONNECTED)  {
          if (internetFlag == 1) {
            Serial.println("Internet Avilable"); 
            HTTPClient http;
            http.begin("https://wk.accentusprime.com/user/index/registerAttendance/"+String(result)+"/"+String(Machine_id));
            int httpCode = http.GET();  
            Serial.print("HTTP CODE IS :-  "); 
            Serial.println(httpCode);
            if (httpCode > 0) { 
                String payload = http.getString();
                Serial.println(payload);
                delay(450);
                payload.trim();
                String Response = payload;
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("    ");
                lcd.print(Response);
                delay(2000);
                lcd.clear(); 
                http.end(); 
            } 
          
          } else {
              WriteDataMemoryCard();
              Empflag=1;
          }
        } else {
          
            WriteDataMemoryCard();
            Empflag=1;
        }  
        digitalWrite(indFinger, LOW);
        return;
    } 
    if(wififlag==1 && Empflag==1 && internetFlag==1) {
       ReadMemoryCardData();
    }
    if(internetFlag==1 && wififlag==1){
        checkKeys(); 
        delay(300);
    }
    
}


void checkKeys() {
  key = keypad.getKey();
  Serial.println(key);
  if(key=='A') {
      lcd.clear(); 
      lcd.print("ENTER EMP ID:");
      delay(500);
      int number= GetNumber();
      Serial.println("The EMP num is :");
      Serial.print(number);
      lcd.setCursor(0,2);  
      lcd.print("EMPLOYEE ID:");
      lcd.setCursor(0,3);  
      lcd.print(number);
      delay(1000);
      lcd.clear();
      if(client.connect(host, httpPort)) {
          HTTPClient http;
          http.begin("https://wk.accentusprime.com/user/index/checkFingerToMachine/"+String(number)+"/"+String(Machine_id));
          int httpCode = http.GET();                                         
          if (httpCode > 0) { 
              String payload = http.getString();
              payload.trim();
              String Response = payload;
              Serial.print(Response);
              if(Response=="BOTH_BLANK") {
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("  READY FOR ENROLL  ");
                delay(2000);
                while(key == 'A');
                Serial.println("Going to Enroll");
                enroll();
                Serial.print("Finger Id : ");
                Serial.println(fid);
                Serial.print("Employee Code : ");
                Serial.println(number);
                Serial.print("Machine Id : ");
                Serial.println(Machine_id);
                if(result>=0) {
                  lcd.clear();
                  lcd.print("FINGER ALREADY EXIST");
                  delay(3000);
                  return;
                }
                Serial.print("Test Print :");
                Serial.println(test);
                if(test==10) {
                  Serial.println("Going to break"); 
                  return;
                }  
                if (client.connect(host, httpPort)) {       
                  HTTPClient http;
                  http.begin("https://wk.accentusprime.com/user/index/enrollFingerToMachine/"+String(fid)+"/"+String(number)+"/"+String(Machine_id));
                  int httpCode = http.GET();                                           
                  if (httpCode > 0) { 
                      String payload = http.getString();
                      payload.trim();
                      String Response = payload;
                      Serial.print(Response);
                      lcd.clear();
                      lcd.setCursor(0,2);
                      lcd.print(Response);
                      delay(2000);
                      lcd.clear();
                      //Serial.println("Debug 44");
                      if (Response == "Enrolment Successful") {
                          lcd.clear();
                          lcd.print(" Place 2nd Finger !");
                          delay(1000);
                          enroll();
                          if(result>=0) {
                            lcd.clear();
                            lcd.print("FINGER ALREADY EXIST");
                            delay(3000);
                            return;
                          }
                          if(test==10) {
                            Serial.println("Going to break"); 
                            return;
                          }  
                         Serial.print("Finger Id : ");
                        Serial.println(fid);
                        Serial.print("Employee Code : ");
                        Serial.println(number);
                        Serial.print("Machine Id : ");
                        Serial.println(Machine_id);
                          HTTPClient http;
                          http.begin("https://wk.accentusprime.com/index/enrollFingerToMachine/"+String(fid)+"/"+String(number)+"/"+String(Machine_id));
                          int httpCode = http.GET();                                        
                          if (httpCode > 0) { 
                              String payload = http.getString();
                              payload.trim();
                              String Response = payload;
                              Serial.print(Response);
                              lcd.clear();
                              lcd.setCursor(0,2);
                              lcd.print(Response);
                              delay(2000);
                              lcd.clear();
                          }
                      }
                  }      
                }
            } else if(Response == "ONE_FING_EXT") {
                enroll();
                if(result>=0) {
                    lcd.clear();
                    lcd.print("FINGER ALREADY EXIST");
                    delay(3000);
                    return;
                }
                if(test==10) {
                  Serial.println("Going to break"); 
                  return;
                }  
                Serial.print("Finger Id : ");
                Serial.println(fid);
                Serial.print("Employee Code : ");
                Serial.println(number);
                Serial.print("Machine Id : ");
                Serial.println(Machine_id);
               if (client.connect(host, httpPort)) {       
                  HTTPClient http;
                  http.begin("https://wk.accentusprime.com/user/index/enrollFingerToMachine/"+String(fid)+"/"+String(number)+"/"+String(Machine_id)); 
                  int httpCode = http.GET();                                        
                  if (httpCode > 0) { 
                      String payload = http.getString();
                      payload.trim();
                      String Response = payload;
                      lcd.clear();
                      lcd.setCursor(0,2);
                      lcd.print(Response);
                      delay(2000);
                      lcd.clear();
                  }
                }
            } else if (Response == "BOTH_FIG_EXT") {
              Serial.println("Fing found");
              lcd.clear();
              lcd.setCursor(0,1);
              lcd.print(" ");
              lcd.print("EMP ALREADY EXIST");
              Serial.println("EMP.ID  EXIST");
              delay(2000);
              lcd.clear();
            } else if(Response == "EMP_ID_DEXT") {
              Serial.println("Fing found");
              lcd.clear();
              lcd.setCursor(0,1);
              lcd.print("   ");
              lcd.print("WRONG EMP ID");
              Serial.println("Wrong Emp Id");;
              delay(2000);
              lcd.clear();
            } 
          } 
          http.end();
      }
   }
   if(key=='D')  {
      lcd.clear();
      lcd.print("Enter Employee Code ");
      delay(500);
      int number= GetNumber();
      lcd.setCursor(0,2);
      lcd.print("Employee Code ");
      lcd.print(number);
      delay(2000);
      if (client.connect(host, httpPort)) {       
            HTTPClient http;
            http.begin("https://wk.accentusprime.com/user/index/deleteFingerPrintData/"+String(number)+"/"+String(Machine_id)+"/0/0");
            int httpCode = http.GET();                                        
            if (httpCode > 0) { 
                String payload = http.getString();
                payload.trim();
                String Response = payload;
                lenght= Response.length();
                Serial.print("Length = ");
                Serial.println(lenght);
                if(Response == "EMP_ID_DEXT"){
                    Serial.println("Employee Id dext");
                    lcd.clear();
                    lcd.setCursor(0,2);
                    lcd.print("  Wrong Emp Code");
                    delay(2000);
                    lcd.clear();
                } else if(lenght < 1){
                    Serial.println("Blank Response For DELETE!!!!");  
                    lcd.clear();
                    lcd.setCursor(0,1);
                    lcd.print("Finger Not Registerd");
                    delay(5000);
                    lcd.clear();
                    return;
                } else {
                    HTTPClient http;
                    int GetSecureKey = 1;
                    http.begin("https://wk.accentusprime.com/user/index/deleteFingerPrintData/0/0/0/"+String(GetSecureKey)); 
                    int httCode = http.GET();                                        
                    if (httCode > 0) { 
                        String payload = http.getString();
                        payload.trim();
                        String GetKey = payload;
                        Serial.print("The Key Is  = ");
                        Serial.println(GetKey); 
                        secureKey = GetKey.toInt();
                     }
                     http.end();
                     Serial.print("After htTP clinet THE SECURE KEY IS = ");
                     Serial.println(secureKey);
                     lcd.clear();
                      lcd.print("Enter Code ");
                      delay(500);
                      int SecureNumber = GetNumber();
                      if (SecureNumber==secureKey) {
                          fid = Response.toInt();
                          deleteFingerprint(fid);                   
                          http.begin("https://wk.accentusprime.com/user/index/deleteFingerPrintData/0/"+String(Machine_id)+"/"+Response+"/0");
                          int httpCode = http.GET();                                        
                          if (httpCode > 0) { 
                              String payload = http.getString();
                              payload.trim();
                              String Response = payload;
                              Serial.println("Finger deleteted SuccessFully!!");
                              if (Response == "NXT_EXT"){
                                  http.begin("https://wk.accentusprime.com/user/index/deleteFingerPrintData/"+String(number)+"/"+String(Machine_id)+"/0/0"); 
                                  int httpCode = http.GET();                                        
                                  if (httpCode > 0) { 
                                      String payload = http.getString();
                                      payload.trim();
                                      String Response = payload;
                                      Serial.println(payload);
                                      fid = Response.toInt();
                                      deleteFingerprint(fid);
                                      http.begin("https://wk.accentusprime.com/user/index/deleteFingerPrintData/0/"+String(Machine_id)+"/"+Response+"/0"); 
                                      int httpCode = http.GET();                                        
                                      if (httpCode > 0) { 
                                          String payload = http.getString();
                                          Serial.print(payload);
                                          lcd.clear();
                                          lcd.print(payload);
                                          delay(2000);
                                          lcd.clear();
                                      }
                                  }
                              } else if(Response == "ALL_DEL") {
                                  Serial.print("All Finger Deleted !!!");
                              }
                          }
                      }  else {
                         lcd.clear();
                         lcd.setCursor(0,1);
                         lcd.print("Wrong Key :(");
                         delay(1000);
                         lcd.clear();
                      }
                }
            }
            http.end();
          }

      
   } else if(key=='#')  {
   Serial.println("Returning.......");
   return; 
   } else if(key == 'B') {
    Serial.println(" Deleting sdcard... ");
    delay(1000);
    deleteFile(SD, "/data.txt");
   }
}

/*====================Write Data To MemoryCard============================*/
void WriteDataMemoryCard() {
  Serial.println("Writing Data TO MemoryCard");
    DateTime now = RTC.now();
    lcd.setCursor(0,1);
    lcd.print("ATTENDANCE REGISTERD");
    Serial.println("ATTENDANCE REGISTERD ");
    delay(1000);
    lcd.clear();
    now = RTC.now();
    Serial.print("Internet Empty");
    Serial.print("NO INTERNET: ");
    Serial.println("Write to sdcard: ");
    file = SD.open("/data.txt");
    if(!file) {
      Serial.println("File doens't exist");
      Serial.println("Creating file...");
      writeFile(SD, "/data.txt", "F_id, M_Id, TIME, DATE\n");
    } else {
      Serial.println("File already exists");  
    }
    file.close();
    dataMessage=String(result)+","+String(Machine_id)+","+String(now.hour(), DEC)+":"+String(now.minute(), DEC)+":"+String(now.second(), DEC)+","+String(now.day(), DEC)+"-"+String(now.month(), DEC)+"-"+String(now.year(), DEC)+"|-|";
    Serial.print("Save data: ");
    Serial.println(dataMessage);
    appendFile(SD, "/data.txt", dataMessage.c_str());
    addr++;
    Serial.println("NO OF EMP ATTENDED");
    Serial.println(addr);
}
/*====================Read Data From MemoryCard============================*/
void ReadMemoryCardData() {
  Serial.println("Reading Data From MemoryCard");
    if (WiFi.status() == WL_CONNECTED) {
        delay(1000);
        Serial.println("Read: ");  
        file = SD.open("/data.txt");
        if (file) {
          int lines=0;
          Serial.println("data.txt:");
          while (file.available()) {
            results[lines] = file.readStringUntil('\n');
            Serial.print("lines:");
            Serial.println(lines);
            Serial.println(results[lines]);
            lines++;
            countoff=lines;
          }
          file.close();
        } else {
          Serial.println("error opening data.txt");
        }
        for(int i=1;i<=addr;i++) {          
          total+=results[i];  
        }
        Serial.println(total);
        String  postData;
        postData = "TOTAL_STRING=" + total;
        HTTPClient http;
        http.begin("https://wk.accentusprime.com/user/index/getOfflineData");              //Specify request destination
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
        int httpCode = http.POST(postData);   //Send the request
        String payload = http.getString();    //Get the response payload
        Serial.println(httpCode);   //Print HTTP return code
        Serial.println(payload);    //Print request response payload
        payload.trim();
        String Res = payload;
        delay(1000);
        if(Res == "SUCCESS") {
          total="";
          addr=0;
          deleteFile(SD, "/data.txt");
          Empflag=0;
        }
        http.end();  //Close connection
        delay(1000);
        
    }
}

/*Enroll Finger*/
void enroll() {
  int p= -1;
 get_free_id(&fid);
 Serial.print("Count For Finger Id : ");
 Serial.println(fid);
  delay(1000);
  while(1) {
        p = finger.getImage();
        if(p != FPM_OK) {
          lcd.clear();
          lcd.print("Place Finger");
          delay(1000);
          lcd.clear();
        } else {
          Serial.print("Inside Enroll");
          result=getFingerprintIDez1();
          Serial.println(result);
          if(result>0) {
            Serial.println("Fingerprint already exist:1");
            Serial.print("result:");
            Serial.println(result);
            lcd.clear();
            lcd.print("Finger Exist!");
            delay(1000);
            return;
          } else {
            test = enroll_finger(fid);
            Serial.print(test);
            return;
          }
       }
    }
  }

/*Enroll The Finger*/  
int32_t enroll_finger(int32_t fid) {
    int32_t p = -1;
    Serial.println("Inside enroll");
    while (p != FPM_OK) {
        p = finger.getImage();
        switch (p) {
            case FPM_OK:
                Serial.println("Image taken");
                lcd.clear();
                lcd.print("Image taken");
                break;
            case FPM_NOFINGER:
                Serial.println("Place Finger");
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("NO FINGER");
                break;
            case FPM_PACKETRECIEVEERR:
                Serial.println("Communication error");
                lcd.clear();
                lcd.print("Comm Error");
                break;
            case FPM_IMAGEFAIL:
                Serial.println("Imaging error");
                lcd.clear();
                lcd.print("Imaging Error");
                break;
            case FPM_TIMEOUT:
                Serial.println("Timeout!");
                lcd.clear();
                lcd.print("Timeout!");
                break;
            case FPM_READ_ERROR:
                Serial.println("Got wrong PID or length!");
                lcd.clear();
                lcd.print("wrong PID or length!");
                break;
            default:
                Serial.println("Unknown error");
                lcd.clear();
                lcd.print("Unknown Error");
                break;
        }
         yield();
    }
    // OK success!
    p = finger.image2Tz(1);
    switch (p) {
        case FPM_OK:
            Serial.println("Image converted");
             lcd.clear();
              lcd.print("Image converted");
            break;
        case FPM_IMAGEMESS:
            Serial.println("Image too messy");
                   lcd.clear();
       lcd.print("Image too messy");
            return p;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Communication error");
                  lcd.clear();
      lcd.print("Comm Error");
            return p;
        case FPM_FEATUREFAIL:
            Serial.println("Could not find fingerprint features");
                  lcd.clear();
      lcd.print("Feature Not Found");
            return p;
        case FPM_INVALIDIMAGE:
            Serial.println("Could not find Valid IMage");
                  lcd.clear();
          lcd.print("Invalid Image");
            return p;
        case FPM_TIMEOUT:
            Serial.println("Timeout!");
                  lcd.clear();
      lcd.print("TimeOut");
            return p;
        case FPM_READ_ERROR:
            Serial.println("Got wrong PID or length!");
                  lcd.clear();
      lcd.print("wrong PID or length");
            return p;
        default:
            Serial.println("Unknown error");
              lcd.clear();
      lcd.print("Unknown error");
            return p;
    }
    Serial.println("Remove finger");
    lcd.clear();
    lcd.print("Remove Finger!");
    delay(2000);
    p = 0;
    while (p != FPM_NOFINGER) {
        p = finger.getImage();
        yield();
    }
    p = -1;
    Serial.println("Place same finger again");
    lcd.clear();
    lcd.print("PLACE FINGER AGAIN");
    delay(1000);
    while (p != FPM_OK) {
        p = finger.getImage();
        switch (p) {
            case FPM_OK:
                Serial.println("Image taken");
                break;
            case FPM_NOFINGER:
                Serial.println(".");
                break;
            case FPM_PACKETRECIEVEERR:
                Serial.println("Communication error");
                break;
            case FPM_IMAGEFAIL:
                Serial.println("Imaging error");
                break;
            case FPM_TIMEOUT:
                Serial.println("Timeout!");
                break;
            case FPM_READ_ERROR:
                Serial.println("Got wrong PID or length!");
                break;
            default:
                Serial.println("Unknown error");
                break;
        }
        yield();
    }
    // OK success!
    p = finger.image2Tz(2);
    switch (p) {
        case FPM_OK:
            Serial.println("Image converted");
            break;
        case FPM_IMAGEMESS:
            Serial.println("Image too messy");
            return p;
        case FPM_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return p;
        case FPM_FEATUREFAIL:
            Serial.println("Could not find fingerprint features");
            return p;
        case FPM_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            return p;
        case FPM_TIMEOUT:
            Serial.println("Timeout!");
            return false;
        case FPM_READ_ERROR:
            Serial.println("Got wrong PID or length!");
            return false;
        default:
            Serial.println("Unknown error");
            return p;
    }
    // OK converted!
    p = finger.createModel();
    if (p == FPM_OK) {
        Serial.println("Prints matched!");
    } else if (p == FPM_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
    } else if (p == FPM_ENROLLMISMATCH) {
         Serial.println("Fingerprints did not match");
          lcd.clear();
          lcd.print("   FINGERPRINTS    ");
          lcd.setCursor(0,1);
          lcd.print("   DID NOT MATCH   ");
          delay(3000);
          lcd.clear();
        return p;
    } else if (p == FPM_TIMEOUT) {
        Serial.println("Timeout!");
        return p;
    } else if (p == FPM_READ_ERROR) {
        Serial.println("Got wrong PID or length!");
        return p;
    } else {
        Serial.println("Unknown error");
        return p;
    }
    Serial.print("ID "); Serial.println(fid);
    p = finger.storeModel(fid);
    if (p == FPM_OK) {
        Serial.println("Stored!");
      lcd.clear();
      lcd.print("Finger Registerd!");
       delay(2000);
    } else if (p == FPM_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
    } else if (p == FPM_BADLOCATION) {
        Serial.println("Could not store in that location");
        return p;
    } else if (p == FPM_FLASHERR) {
        Serial.println("Error writing to flash");
        return p;
    } else if (p == FPM_TIMEOUT) {
        Serial.println("Timeout!");
        return p;
    } else if (p == FPM_READ_ERROR) {
        Serial.println("Got wrong PID or length!");
        return p;
    } else {
        Serial.println("Unknown error");
        return p;
    }
    return fid;
}


void getTime() {
  if(WiFi.status() == WL_CONNECTED) {
            if (client.connect(host2, httpPort)) {
              google = 1;
            } else {
              google = 0;
            }
             HTTPClient http;
            //http.begin("http://wk.accentusprime.com/user/index/getTime");
            http.begin("https://wk.accentusprime.com/user/index/getTime");
            //http://06ede142.ngrok.io/whims/
            int httpCode = http.GET();                                                
            if (httpCode > 0) {
                wififlag=1;
                Serial.println(lenght);
                String payload = http.getString();
                lenght =  payload.length();
                payload.trim();
                String Response = payload;
                if(lenght==18){
                  String Time = Response.substring(0,8);
                  lcd.print(" Time : ");
                  lcd.print(Time);
                  lcd.setCursor(0,3);
                  String Date = Response.substring(8);
                  lcd.print(" Date : ");
                  lcd.print(Date);
                }
             } else {
                 Serial.print("GetTime Http Code :- "+String(httpCode));
                 lenght=0;
             }
              http.end();
           
      } else {
        DateTime now = RTC.now();
        lcd.print(" TIME:");
        lcd.print(now.hour(), DEC);
        lcd.print(':');
        lcd.print(now.minute(), DEC);
        lcd.print(':');
        lcd.print(now.second(), DEC);
        lcd.setCursor(0,3);
        lcd.print(" DATE:");
        lcd.print(now.day(), DEC);
        lcd.print('/');
        lcd.print(now.month(), DEC);
        lcd.print('/');
        lcd.print(now.year(), DEC);
        delay(500);  
      }
  }

/*Search Finger Id*/
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FPM_OK)
  return -1;
  p = finger.image2Tz();
  if (p != FPM_OK)
  return -1;
  uint16_t fid, score;
  p = finger.searchDatabase(&fid, &score);
  if (p != FPM_OK) {
  lcd.clear();
   /*Work Here*/
   lcd.print("Finger Not Found");
   Serial.println("Finger Not Found");
   //Buzzer 
   digitalWrite(buzzer, HIGH);
   delay(200);
   digitalWrite(buzzer, LOW);
   delay(150);
   digitalWrite(buzzer, HIGH);
   delay(200);
   digitalWrite(buzzer, LOW);
   delay(150);
   digitalWrite(buzzer, HIGH);
   delay(200);
   digitalWrite(buzzer, LOW);
   //Buzzer End
   lcd.setCursor(0,1);
   lcd.print("Try Later");
   Serial.println("Try Later");
   delay(2000);
   lcd.clear();
   return -1;
  }
  Serial.print("Found ID #");
  Serial.println(fid);
  return fid;
}

int getFingerprintIDez1() {
  uint8_t p = finger.getImage();
  if (p != FPM_OK)
  return -1;
  p = finger.image2Tz();
  if (p != FPM_OK)
  return -1;
  uint16_t fid, score;
  p = finger.searchDatabase(&fid, &score);
  if (p != FPM_OK) {
  lcd.clear();
   /*Work Here*/
   lcd.print("Please Wait..");
   Serial.println("Finger Not Found");
   delay(2000);
   lcd.clear();
   return -1;
  }
  Serial.print("Found ID #");
  Serial.println(fid);
  return fid;
}

/*Get Last Finger Id*/
bool get_free_id(int32_t * fid) {
    int16_t p = -1;
    for (int page = 0; page < (params.capacity / FPM_TEMPLATES_PER_PAGE) + 1; page++) {
        p = finger.getFreeIndex(page, fid);
        switch (p) {
            case FPM_OK:
                if (*fid != FPM_NOFREEINDEX) {
                    Serial.print("Free slot at ID ");
                    Serial.println(*fid);
                    return true;
                }
                break;
            case FPM_PACKETRECIEVEERR:
                Serial.println("Communication error!");
                return false;
            case FPM_TIMEOUT:
                Serial.println("Timeout!");
                return false;
            case FPM_READ_ERROR:
                Serial.println("Got wrong PID or length!");
                return false;
            default:
                Serial.println("Unknown error!");
                return false;
        }
        yield();
    }
    
    Serial.println("No free slots!");
    return false;
}
/*Delete Finger id*/
int deleteFingerprint(int fid) {
    int p = -1;

    p = finger.deleteModel(fid);

    if (p == FPM_OK) {
        Serial.println("Deleted!");
        lcd.clear();
        lcd.print("Finger Deleted !");
        delay(2000);
        lcd.clear();
    } else if (p == FPM_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
    } else if (p == FPM_BADLOCATION) {
        Serial.println("Could not delete in that location");
        return p;
    } else if (p == FPM_FLASHERR) {
        Serial.println("Error writing to flash");
        return p;
    } else if (p == FPM_TIMEOUT) {
        Serial.println("Timeout!");
        return p;
    } else if (p == FPM_READ_ERROR) {
        Serial.println("Got wrong PID or length!");
        return p;
    } else {
        Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
        return p;
    }
}
/*Get Number From Keypad*/
int GetNumber() {
   int num = 0;
   int i=0;
   key = keypad.getKey();
   while(key != '#') {
      switch (key) {
         case NO_KEY:
            break;
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            num = num * 10 + (key - '0');
            lcd.setCursor(i,1);
            lcd.print(key);
            i++;
            break;
         case '*':
            num = 0;
            lcd.clear();
            break;
      }
      Serial.println(key);
      key = keypad.getKey();
   }
   return num;
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message))  {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
    Serial.printf("Appending to file: %s\n", path);
    File file = fs.open(path, FILE_APPEND);
    if(!file) {
      Serial.println("Failed to open file for appending");
      return;
    }
    if(file.print(message)) {
      Serial.println("Message appended");
    } else {
      Serial.println("Append failed");
    }
    file.close();
 }

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
