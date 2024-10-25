
void blinking() {
  static byte led_counter;
  static bool led_status;
  static unsigned long led_change;
  const int durationLEDon[] = { 80, 200, 1000 };
  const int durationLEDoff[] = { 1000, 1200, 5000 };
  if (LED_blinking_times2 == 0) {
    digitalWrite(BuiltinLED, LOW);  //when connected LED off
    return;
  } else if (LED_blinking_times2 >= 1) {  // blinking
    if (led_status && millis() >= led_change) {
      digitalWrite(BuiltinLED, LOW);  //when connected LED off
      led_status = 0;
      led_counter++;
      if (led_counter >= LED_blinking_times2) {
        led_change = millis() + durationLEDoff[LED_blinking_duration2];  //off time Led long
        led_counter = 0;
      } else {
        led_change = millis() + durationLEDon[LED_blinking_duration2]*2;  //off time Led short
      }
    } else if (!led_status && millis() >= led_change) {
      digitalWrite(BuiltinLED, HIGH);  //when connected LED off
      led_status = 1;
      led_change = millis() + durationLEDon[LED_blinking_duration2];  //on time Led
    }
  }
}