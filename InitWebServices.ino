


const char* ntpServer = "192.168.178.1";  // Standard IP für Fritzbox
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, 3600, 60000);  // UTC+1 (3600s) und Aktualisierungsintervall von 60s
//NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 3600, 60000);  //europe.pool.ntp.org

boolean NTP_gettime() {
  timeClient.begin();
  timeClient.update();
  setTime(timeClient.getEpochTime() + 3600);
  Serial.println(timeClient.getFormattedTime());
  timeClient.end();
  if (hour() != 0 && minute() != 0 && second() != 0 && year() != 0) {
    return 0;  //Zeit gefunden
  }
  return 1;
}
void ota() {
  static unsigned long ota_stop;
  static boolean ota_flag = 1;
  ArduinoOTA.handle();
  if (ota_flag) {
    ota_flag = 0;
    otaon = 1;
    ota_stop = millis() + 600000;  //für 10 min an
    Serial.println("OTA on");
  } else if (!ota_flag && ota_stop < millis()) {
    otaon = 0;
    ota_flag = 1;
    Serial.println("OTA off");
  }
}

void ini_ota() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else  // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    otaon = 0;
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
}

void wifi() {
  LED_blinking_times = 1;
  LED_blinking_duration = 1;
  Serial.print("wifistatus: ");
  byte statuswifi = WiFi.waitForConnectResult();
  Serial.println(statuswifi);
  if (statuswifi == WL_CONNECTED) return;
  int counter = 0;
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA);
  delay(1);
  WiFi.setHostname(DNSname);
  WiFi.begin(ssid, password);
  delay(500);
  while (statuswifi != WL_CONNECTED) {
    Serial.print(statuswifi);
    Serial.print(" - ");
    Serial.println(counter);
    delay(500);
    counter++;
    if (counter == 5 && statuswifi == 6) {
      WiFi.mode(WIFI_STA);
      WiFi.setHostname(DNSname);
      WiFi.begin(ssid, password);
    } else if (counter == 15 || statuswifi == 4) {
      Serial.println("Verbindungsprobleme --> WiFi Neustart");
      WiFi.disconnect(true);
      delay(1000);
      WiFi.mode(WIFI_OFF);
      delay(1000);
      WiFi.setAutoReconnect(true);
      WiFi.persistent(true);
      WiFi.mode(WIFI_STA);
      WiFi.setHostname(DNSname);
      WiFi.begin(ssid, password);
    } else if (counter >= 20) {
      Serial.println("Verbindungsprobleme ==> Neustart");
      saveEEPROM();
      delay(200);
      ESP.restart();
    }
    statuswifi = WiFi.status();
  }
  IPaddress = WiFi.localIP().toString();
  Serial.println(IPaddress);
  return;
}
