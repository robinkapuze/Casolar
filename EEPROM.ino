void saveEEPROM() {
  bool bool_temp;
  byte byte_temp;
  float float_temp;
  int int_temp;
  EEPROM.begin(512);
  EEPROM.get(0, bool_temp);
  bool_temp != loadmanual ? EEPROM.put(0, loadmanual) : 1;
  EEPROM.get(1, bool_temp);
  bool_temp != prio_auto ? EEPROM.put(1, prio_auto) : 1;
  EEPROM.get(2, bool_temp);
  bool_temp != overwrite ? EEPROM.put(2, overwrite) : 1;
  EEPROM.get(3, byte_temp);
  byte_temp != only_x_phasig ? EEPROM.put(3, only_x_phasig) : 1;
  EEPROM.get(4, bool_temp);
  bool_temp != loadminimal ? EEPROM.put(4, loadminimal) : 1;
  EEPROM.get(5, bool_temp);
  bool_temp != goeueberschuss ? EEPROM.put(5, goeueberschuss) : 1;
  EEPROM.get(7, byte_temp);
  byte_temp != retrysPv ? EEPROM.put(7, retrysGo) : 1;
  EEPROM.get(8, byte_temp);
  byte_temp != retrysPv ? EEPROM.put(8, retrysPv) : 1;
  EEPROM.get(9, byte_temp);
  byte_temp != ladestop_prozent ? EEPROM.put(9, ladestop_prozent) : 1;
  EEPROM.get(10, byte_temp);
  byte_temp != today ? EEPROM.put(10, today) : 1;
  for (int i = 1; i < 14; i++) {
    EEPROM.get(i + 10, byte_temp);
    (maxtoday[i] <= (byte_temp / 4.) - 0.25 || maxtoday[i] >= (byte_temp / 4.) + 0.25) ? EEPROM.put(i + 10, byte(maxtoday[i] * 4)) : 1;
    EEPROM.get(i + 25, byte_temp);
    (realtoday[i] <= (byte_temp / 4.) - 0.25 || realtoday[i] >= (byte_temp / 4.) + 0.25) ? EEPROM.put(i + 25, byte(realtoday[i] * 4)) : 1;
    EEPROM.get(i + 40, byte_temp);
    (uebertoday[i] <= (byte_temp / 4.) - 0.25 || uebertoday[i] >= (byte_temp / 4.) + 0.25) ? EEPROM.put(i + 40, byte(uebertoday[i] * 4)) : 1;
  }
  EEPROM.get(60, float_temp);
  float_temp != maxtoday[0] ? EEPROM.put(60, maxtoday[0]) : 1;
  EEPROM.get(64, float_temp);
  float_temp != realtoday[0] ? EEPROM.put(64, realtoday[0]) : 1;
  EEPROM.get(68, float_temp);
  float_temp != uebertoday[0] ? EEPROM.put(68, uebertoday[0]) : 1;
  EEPROM.commit();
  EEPROM.end();
  Serial.println("Saved settings ");
  //if (retrysPv >= 10 && retrysPv < 50) deepsleep();
  //if (retrysGo >= 10 && retrysGo < 50) deepsleep();
  changed = 0;
  return;
}
void loadEEPROM() {
  EEPROM.begin(512);
  EEPROM.get(0, loadmanual);
  EEPROM.get(1, prio_auto);
  EEPROM.get(2, overwrite);
  EEPROM.get(3, only_x_phasig);
  EEPROM.get(4, loadminimal);
  EEPROM.get(5, goeueberschuss);
  EEPROM.get(7, retrysGo);
  EEPROM.get(8, retrysPv);
  EEPROM.get(9, ladestop_prozent);
  byte temp;
  for (int i = 1; i < 14; i++) {
    EEPROM.get(i + 10, temp);
    maxtoday[i] = temp / 4.;
    EEPROM.get(i + 25, temp);
    realtoday[i] = temp / 4.;
    EEPROM.get(i + 40, temp);
    uebertoday[i] = temp / 4.;
  }
  EEPROM.get(60, maxtoday[0]);
  EEPROM.get(64, realtoday[0]);
  EEPROM.get(68, uebertoday[0]);
  if (ladestop_prozent >= 100 || ladestop_prozent < 0) {
    ladestop_prozent = 30;
    changed = 1;
  }
  EEPROM.get(10, today);
  if (today == 0) {
    today = day();
    Serial.println("no day find in eeprom");
  } else if (today != byte(day())) {
    Serial.print("Tag aenderung: ");
    Serial.print(today);
    Serial.print("--->");
    Serial.println(day());
    movetoday();
  } else {
    Serial.print(today);
    Serial.print("-all good->");
    Serial.println(day());
  }
  EEPROM.end();
  Serial.println("Loaded settings ");
  return;
}