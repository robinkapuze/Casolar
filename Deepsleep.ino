#define uS_TO_S_FACTOR 1000000ULL  // Faktor, um Sekunden in Mikrosekunden zu konvertieren

void deepsleep() {
  //Go to sleep now
  Serial.print("Going to sleep for ");
  Serial.print(interval);
  Serial.println(" minutes.");
  saveEEPROM();
  delay(500);
  while (!xSemaphoreTake(xMutex, (200 * portTICK_PERIOD_MS))) {
    delay(100);
  }
  LED_blinking_times1 = 1;
  LED_blinking_duration1 = 2;
  xSemaphoreGive(xMutex);
  delay(200);
  esp_sleep_enable_timer_wakeup(interval * 60 * uS_TO_S_FACTOR);
  esp_deep_sleep_start(); //starts deepsleep and come back as a restart
}