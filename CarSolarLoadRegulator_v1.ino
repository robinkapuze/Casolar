//robinkapuze petereinbanschbach@gmail.com
#include <WiFiClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "esp_sleep.h"
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <RestClient.h>
#include <time.h>
#include <TimeLib.h>

AsyncWebServer server(80);
//-----------------------------------------First Setup Data Start------------------------------------------------------
const char* ssid = "Fritzbox XYZZ";                                    //Wifi ssid name
const char* password = "put your password here";                          //Wifi password
const char* DNSname = "Casolar";                                    //Rename of ESP32 standard naming (inside router/network name)
const char PV_ADRESS[] = "http://192.168.178.45:80/rest/channel/";  //Fenecon FEMS IP Adress + /rest/channel/
const char GO_ADRESS[] = "http://192.168.178.74:80";                // Go-e Charger IP Adress & PORT
byte takt = 5;                                                      //frequenz of regulator in seconds (max. "5" for go-e internal solarcharging) 

//check also the InitWebServices.ino tab to make sure your arduino gets the current time from Fritzbox time server or out from the www

//-----------------------------------------First Setup Data End--------------------------------------------------------

SemaphoreHandle_t xMutex = NULL; //mutex for led transfer data
uint64_t interval;  //lengt of deepsleep in minutes
bool load;  //loading allowed
byte LED_blinking_times = 1;      //primär core variable
byte LED_blinking_times1 = 1;     //chared Variable
byte LED_blinking_times2 = 1;     //LED core variable
byte LED_blinking_duration = 2;   //primär core variable
byte LED_blinking_duration1 = 2;  //chared Variable
byte LED_blinking_duration2 = 2;  //LED core variable
const byte BuiltinLED = 2;   //Pin for LED
boolean otaon; //Update over the air on/off wenn on it keeps alive for 10min.
boolean changed; //if changed = 1 save to EEPROM is initiated
boolean init_loop = 1; //in first loop no check for manual interference 
boolean loadmanual; //wenn 1 you can change settings inside Go-e App
boolean loadminimal; // wenn 1 loading 1 phase with 6 Amp
boolean prio_auto; //priorise car before solar batterie
unsigned long next_check, next_check_settings, nextsave = 60000; //timers for diff. check loops
boolean goeueberschuss; // if 1 this go algorithmus does not take aktion; only the Go-e Internal Solarloading algorithmus  
boolean r_flag = 0;
byte retrysPv, retrysGo; //counter of failed requests with PV and Go-e; resets wenn one request is positive
float uebertoday[14]; //summed up ueberschuss; power (kW) tracking of the last days. 
float maxtoday[14];   // summed up maximal possible and allowed loading power;power (kW) tracking of the last days. 
float realtoday[14]; // summed up maximal real loading power; power (kW) tracking of the last days. 
byte today; //day 1....31
String IPaddress; //self IP adress of esp31

void ini_webserver();
void setup() {
  xMutex = xSemaphoreCreateMutex();  // crete a mutex object
  xTaskCreatePinnedToCore(
    taskwifi,    // Function to implement the task
    "taskwifi",  // Name of the task
    32 * 1000,   // Stack size in words
    NULL,        // Task input parameter
    1,           // Priority of the task
    NULL,        // Task handle.
    1            // Core 1 for WIFI; Core 0 for evererything else
  );

  xTaskCreatePinnedToCore(
    task2,     // Function to implement the task
    "task2",   // Name of the task
    4 * 1000,  // Stack size in words
    NULL,      // Task input parameter
    2,         // Priority of the task
    NULL,      // Task handle.
    0          // Core 1 for WIFI; Core 0 for evererything else
  );
}

void task2(void* pvParameters) { // LED core, let the led blink
  pinMode(BuiltinLED, OUTPUT);     //Builtin LED
  digitalWrite(BuiltinLED, HIGH);  //when connected LED off
  unsigned long next_ledupdate = millis() + 1000;
  while (1) {
    vTaskDelay(10);
    if (next_ledupdate <= millis()) {
      next_ledupdate = millis() + 1000;
      if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
        LED_blinking_times2 = LED_blinking_times1;
        LED_blinking_duration2 = LED_blinking_duration1;
        xSemaphoreGive(xMutex);
      }
    }
    blinking();  //erst nach dem initiieren soll geblinkt werden
  }
}

void taskwifi(void* pvParameters) { //Primary core
  Serial.begin(115200);
  Serial.println(__FILE__);
  wifi();  //connect to wifi
  ini_ota();  //initiate Update over the air
  ini_webserver(); 
  otaon = 1; //first 10min. the ota alway should be on to update imeditaly again
  ota();
  ArduinoOTA.begin();  //be ready for update over the air
  NTP_gettime();       //get Time over Fritz.box request; check InitWebServices.ino for IP or change it to a global website request
  loadEEPROM();    //load last saved data
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.print("  ");
  Serial.print(today);
  Serial.print(" / ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
  next_check = millis() + 100;
  Serial.println("Starting...");
  delay(500);
  unsigned long next_ledupdate1 = millis() + 1100;
  while (1) {
    vTaskDelay(1); //short delay to trigger the wachdog timer
    if (otaon) ota();  // handle OTA request and stop after 10min
    if (next_ledupdate1 < millis() && xSemaphoreTake(xMutex, (200 * portTICK_PERIOD_MS))) { //LED blinking exchange
      next_ledupdate1 = millis() + 1000;
      LED_blinking_times1 = LED_blinking_times;
      LED_blinking_duration1 = LED_blinking_duration;
      xSemaphoreGive(xMutex);
    } else if (otaon == 0 && millis() > 4294000000) {  //neustarten - millis overflow verhindern max: 4294967296 millis
      Serial.println("Restart due to overflow");
      saveEEPROM();
      delay(100);
      ESP.restart();
    } else if (load == 0 && otaon == 0 && (hour() >= 22 || hour() < 6) && year() >= 2024) {  //Abends schlafen gehen
      Serial.print("Ich schlafe bis morgen früh: ");
      delay(100);
      int sethour = 6;  //Aufwachzeit
      int setminu = 0;
      hour() < sethour ? interval = (sethour - hour())* 60 : interval = (24 - hour() + sethour) * 60;
      interval += setminu - minute();
      deepsleep();
    }
    vTaskDelay(1);
    if (millis() >= next_check) {  //jede 5sekunden Ladeleistung aktualisieren
      next_check = millis() + takt * 1000;
      vTaskDelay(1);
      if (WiFi.waitForConnectResult() != 3) wifi();  //reconnect to wifi, if no connection longer exist
      vTaskDelay(1);
      if (!goeueberschuss) {
        if (Regulator() == 0) {
          sendueberschuss();
        };
      } else {
        sendueberschuss();
      }
      vTaskDelay(1);
    }
    if (millis() >= nextsave) {
      if (today != day()) {
        Serial.print("Tagaenderung: ");
        Serial.print(today);
        Serial.print("->");
        Serial.print(day());
        movetoday();
      }
      saveEEPROM();
      nextsave += 60000 * 10;  
    }
    if(changed)saveEEPROM(); //save directly if 'requested' a byte of eeprom have a life expectancy of 100.000 overwrite times
  }
}
void loop() {
}