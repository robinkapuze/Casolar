
bool manuellerEingriff;      //if 1 nicht laden; wenn 0 nicht laden
byte phase = 1, amp1, amp3;  // 6.....16A
int ueberschuss, status_phase, status_amp, status_auto, status_sell, status_loaddata, status_alw, status_discharge, status_batterie, status_solar;
const char* msg;
unsigned long timer_changephase;
unsigned long timer_changeload;
unsigned long timer_stopload;
unsigned long last_loadstop;
byte ladestop_prozent = 30;
unsigned long phasechangemillis = 300000;  //Phasenumstellung maximale Änderung 600000ms = 10min
unsigned long loadchangemillis = 120000;   //Phasenumstellung maximale Änderung 1mins = 10min
bool first_load_set = 1, overwrite;
const int numElements = 16;
byte voltage = 230;
int real_load_data[numElements];
byte only_x_phasig;
WiFiClient client;
int HTTP_PORT = 80;
String HTTP_METHOD = "GET";
const char GO_mqttADRESS[] = "/mqtt?payload=";
const char GO_setADRESS[] = "/api/set?";
const char GO_getADRESS[] = "/api/status?filter=";

void movetoday() {
  if (year() < 2024 || (uebertoday[0] <= 0.01 && realtoday[0] <= 0.01 && maxtoday[0] <= 0.01)) {
    Serial.println("Nein, mach ich nicht, da ...today == 0");
    today = day();
    return;
  }
  Serial.println("moving");
  for (int i = 13; i >= 1; i--) {
    maxtoday[i] = maxtoday[i - 1];
    realtoday[i] = realtoday[i - 1];
    uebertoday[i] = uebertoday[i - 1];
  }
  maxtoday[0] = 0.;
  realtoday[0] = 0.;
  uebertoday[0] = 0.;
  today = day();
  saveEEPROM();
}
void setGo(String queryString1, byte mqtt = 0) {
  static int failcounter;
  String ans = "";
  if (mqtt == 0) {  //für fast alle anfragen
    HTTPClient httpGo;
    String serverPath = String(GO_ADRESS) + String(GO_setADRESS) + queryString1;
    //Serial.println(String(GO_ADRESS) + String(GO_setADRESS) + queryString1);
    httpGo.begin(serverPath.c_str());
    int httpResponseCode = httpGo.GET();
    if (httpResponseCode > 0) {
      ans = httpGo.getString();
      Serial.print("ans: ");
      Serial.println(ans);
      failcounter = 0;
      retrysGo = 0;
    } else {
      Serial.print("No Respond from Go-e Charger; Try to Set nbr: ");
      Serial.println(queryString1);
      vTaskDelay(1);
      failcounter++;
      LED_blinking_times = 2;
      LED_blinking_duration = 1;
      httpGo.end();
      if (failcounter > 15) {
        retrysGo++;
        saveEEPROM();
        delay(50);
        Serial.println("restart due to connection error");
        ESP.restart();
      }
    }
    vTaskDelay(1);

  } else if (mqtt == 1) {  //für wahrscheinlich nichts
    client.connect(GO_ADRESS, HTTP_PORT);
    //Serial.print("setGo: ");
    client.println(HTTP_METHOD + " " + String(GO_getADRESS) + queryString1 + " HTTP/1.1");
    //Serial.println(HTTP_METHOD + " " + String(GO_getADRESS) + queryString1 + " HTTP/1.1");
    client.println("Connection: close");
    client.stop();
    vTaskDelay(1);
  } else if (mqtt = 2) {
    HTTPClient httpGo;
    String serverPath = String(GO_ADRESS) + String(GO_mqttADRESS) + queryString1;
    //Serial.println(String(GO_ADRESS) + String(GO_mqttADRESS) + queryString1);
    vTaskDelay(1);
    httpGo.begin(serverPath.c_str());
    int httpResponseCode = httpGo.GET();
    if (httpResponseCode > 0) {
      ans = httpGo.getString();
      //Serial.print("Respond: ");
      //Serial.println(httpResponseCode);
    }
    httpGo.end();
    vTaskDelay(1);
    LED_blinking_times = 2;
    LED_blinking_duration = 1;
  }
  return;
}
int getGonbr(String queryString1) {
  static int failcounter;
  HTTPClient httpGo;
  String serverPath = String(GO_ADRESS) + String(GO_getADRESS) + queryString1;
  httpGo.begin(serverPath.c_str());
  String ans = "";
  vTaskDelay(1);
  int httpResponseCode = httpGo.GET();
  if (httpResponseCode > 0) {
    ans = httpGo.getString();
    failcounter = 0;
    retrysGo = 0;
  } else {
    Serial.print("No Respond from Go-e Charger; Try to Get nbr: ");
    Serial.println(queryString1);
    httpGo.end();
    LED_blinking_times = 2;
    LED_blinking_duration = 1;
    failcounter++;
    httpGo.end();
    if (failcounter > 15) {
      retrysGo++;
      saveEEPROM();
      delay(50);
      Serial.println("restart due to connection error");
      ESP.restart();
    }
    return -1;
  }
  if (ans.lastIndexOf("false") != -1) return 0;
  if (ans.lastIndexOf("true") != -1) return 1;
  int q = ans.lastIndexOf(":");
  ans.remove(0, q + 1);
  //ans.remove(ans.length() - 1, 1);
  return ans.toInt();  //sell_energie: wenn negativ wird aus dem Netztbezogen; wenn positiv wird Strom eingespeist
}
String getGo(String queryString1) {
  static int failcounter;
  HTTPClient httpGo;
  String serverPath = String(GO_ADRESS) + String(GO_getADRESS) + queryString1;
  httpGo.begin(serverPath.c_str());
  String ans = "";
  vTaskDelay(1);
  int httpResponseCode = httpGo.GET();
  if (httpResponseCode > 0) {
    ans = httpGo.getString();
    failcounter = 0;
    retrysGo = 0;
  } else {
    Serial.print("No Respond from Go-e Charger; Try to Get: ");
    Serial.println(queryString1);
    httpGo.end();
    failcounter++;
    LED_blinking_times = 2;
    LED_blinking_duration = 1;
    httpGo.end();
    if (failcounter > 15) {
      retrysGo++;
      saveEEPROM();
      delay(50);
      Serial.println("restart due to connection error");
      ESP.restart();
    }
    return "";
  }
  vTaskDelay(1);
  return ans;
}
int getPV(String queryString1) {
  static int failcounter;
  HTTPClient httpPV;
  String serverPath = String(PV_ADRESS) + queryString1;
  httpPV.begin(serverPath.c_str());
  httpPV.setAuthorization("", "user");
  int httpResponseCode = httpPV.GET();
  vTaskDelay(1);
  if (httpResponseCode > 0) {
    failcounter = 0;
    String payload = httpPV.getString();
    httpPV.end();
    int q = payload.lastIndexOf("\"value\":");
    payload.remove(0, q + 8);
    payload.remove(payload.length() - 1, 1);
    retrysPv = 0;
    return -payload.toInt();  //sell_energie: wenn negativ wird aus dem Netztbezogen; wenn positiv wird Strom eingespeist
  } else {
    vTaskDelay(1);
    httpPV.end();
    Serial.println("PV did not respond");
    LED_blinking_times = 2;
    failcounter++;
    LED_blinking_times = 3;
    LED_blinking_duration = 1;
    if (failcounter > 15) {
      retrysPv++;
      saveEEPROM();
      delay(50);
      Serial.println("restart due to connection error");
      ESP.restart();
    }
    return -1;
  }
}
int getGoload(int data = 11) {
  String msg_all = getGo("nrg");  //Tatsächliche Phasen Leistung erfragen (array mit Voltage, Ampere, Leistung,...)
  const size_t capacity = JSON_OBJECT_SIZE(10) + 300;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, msg_all);
  if (error) {
    vTaskDelay(1);
    retrysGo++;
    saveEEPROM();
    delay(50);
    Serial.print("No respond from Go-e Charger; Try to get Loadrates: ");
    Serial.println(error.c_str());
    LED_blinking_times = 1;
    LED_blinking_duration = 1;
    return -1;
  }
  vTaskDelay(1);
  retrysGo = 0;
  JsonArray nrgArray = doc["nrg"];
  for (int i = 0; i < numElements; i++) {
    float value = nrgArray[i];
    real_load_data[i] = round(value);
    vTaskDelay(1);
  }
  return real_load_data[data];
}

int getData() {
  int q = 0, ans, retgD = 0;
  for (q = 0; q <= 5; q++) {
    vTaskDelay(1);
    switch (q) {
      case 0:
        ans = getGonbr("car");
        ans == -1 ? Serial.print("car: ") : status_auto = ans;
        break;
      case 1:
        ans = getGonbr("psm");
        ans == -1 ? Serial.print("phase: ") : status_phase = ans;
        //Serial.print("phase: ");Serial.println(ans);
        break;
      case 2:
        ans = getGonbr("amp");
        ans == -1 ? Serial.print("amp: ") : status_amp = ans;
        //Serial.print("amp: ");Serial.println(ans);
        break;
      case 3:
        ans = getGoload();
        ans == -1 ? Serial.print("loaddata: ") : status_loaddata = ans;
        //Serial.print("loaddata: ");Serial.println(ans);
        ans = 0;
        break;
      case 4:
        ans = getPV("_sum/GridActivePower");
        ans == -1 ? Serial.print("sell: ") : status_sell = ans;
        ans = getPV("_sum/EssDischargePower");
        ans == -1 ? Serial.print("discharge: ") : status_discharge = ans;  //status_discharge positiv = akku wird geladen;
        ans = getPV("ess0/Soc");
        ans == -1 ? Serial.print("batteriestand: ") : status_batterie = -ans;  //status_discharge positiv = akku wird geladen;
        ans = getPV("_sum/ProductionDcActualPower");
        ans == -1 ? Serial.print("discharge: ") : status_solar = ans;  //status_discharge positiv = akku wird geladen;
        //Serial.print("sell: ");Serial.println(ans);
        break;
      case 5:
        break;
        ans = getGonbr("alw");
        ans == -1 ? Serial.print("alw: ") : status_alw = ans;
        //Serial.print("alw: ");Serial.println(ans);
        break;
    }
    if (ans == -1) {
      Serial.print("No data from: ");
      Serial.println(q);
      return -1;
    }
  }
  if (init_loop && retgD == 0) {
    Serial.println("Ersterdurchlauf - Daten abgeglichen");
    init_loop = 0;
    load = status_loaddata > 0 ? 1 : 0;
    phase = status_phase;
    amp1 = amp3 = status_amp;
  }
  /*Serial.print("getData() = ");
  Serial.println(retgD);*/
  return retgD;
}

int manuellereingriff() {
  if (getData() == -1) return -1;
  vTaskDelay(1);
  if (manuellerEingriff) return 1;
  if (init_loop) {
    Serial.println("Da erster durchlauf kein Manueller test");
    return 0;
  }
  boolean ret = 0;
  if (status_phase == 0) {
    ret = 1;
    Serial.println("Phase Automatisch");
  } /* else if (status_phase != phase) {
    ret = 1;
    Serial.println("Phasenunterschied");
  }
  else if ((status_phase == 1 && amp1 == status_amp) || (status_phase == 2 && amp3 == status_amp)) {
    ret = 0;
  }else{
    ret = 1;
  }*/
  if (ret) {
    manuellerEingriff = 1;
    Serial.println("Manueller Eingriff wurde entdeckt");
    LED_blinking_times = 4;
    LED_blinking_duration = 1;
  }
  return ret;
}

boolean calcLoad() {
  ueberschuss = status_sell + status_loaddata;
  if ((prio_auto == 1 && status_batterie > ladestop_prozent) || status_batterie >= 80) ueberschuss += status_discharge;  // Solarbatterie nicht mehr laden
  if (status_discharge > 900) ueberschuss += status_discharge / 2;
  if (ueberschuss > 0) uebertoday[0] += ueberschuss / 3600000. * takt;
  if (loadminimal) {
    if (status_batterie <= ladestop_prozent && load == 1) {
      timer_changeload = 0;
      return 0;
    }
    if (timer_changeload > 0) timer_changeload = 0;
    LED_blinking_times = 2;
    LED_blinking_duration = 0;
    return 1;
  }
  if (status_auto == 1 && overwrite == 0) {
    Serial.println("Kein Auto angeschlossen");
    LED_blinking_times = 1;
    LED_blinking_duration = 0;
    return 0;
  }
  //Batterie maximal mit 900w laden
  if (ueberschuss < -200) {
    last_loadstop = 500000;
    sign = -1;
  }
  if (ueberschuss < 1400) {
    if (load) {
      if (timer_changeload == 0) {  // laden beenden initiieren
        timer_changeload = millis() + loadchangemillis / 2;
        Serial.println("Laden beenden wird initiiert.");
        LED_blinking_times = 3;
        LED_blinking_duration = 0;
        return 1;
      } else if (timer_changeload <= millis()) {  // laden beenden
        Serial.println("Laden wird beendet.");
        LED_blinking_times = 1;
        LED_blinking_duration = 1;
        timer_changeload = 0;
        return 0;
      }
    } else {
      timer_changeload = 0;  // weiterhin nicht laden
      Serial.println("Keine Aenderung - Nicht laden.");
      LED_blinking_times = 1;
      LED_blinking_duration = 1;
      return 0;
    }
  } else if (ueberschuss >= 1400) {
    if (load) {
      Serial.println("Keine Aenderung - Laden aktiv.");
      LED_blinking_times = 2;
      LED_blinking_duration = 1;
      timer_changeload = 0;
      return 1;
    } else {
      if (timer_changeload == 0) {
        if (millis() <= (120000 + last_loadstop * sign)) {  // sofort laden
          Serial.println("Laden wird sofort beginnen.");
          timer_changeload = 0;
          LED_blinking_times = 1;
          LED_blinking_duration = 0;
          first_load_set = 1;
          return 1;
        }
        timer_changeload = millis() + loadchangemillis;  //laden initiieren
        Serial.print("Laden ist initiiert worden.");
        LED_blinking_times = 3;
        LED_blinking_duration = 0;
        return 0;
      } else if (timer_changeload <= millis()) {  //laden nun beginnen
        Serial.println("Laden wird beginnen.");
        LED_blinking_times = 1;
        LED_blinking_duration = 0;
        timer_changeload = 0;
        first_load_set = 1;
        return 1;
      }
    }
  }
  return load;
}

int calcLoading() {
  ueberschuss = status_sell + status_loaddata;
  if (prio_auto == 1 && status_batterie > ladestop_prozent) ueberschuss += status_discharge;
  if (status_discharge > 900) ueberschuss += status_discharge / 2;
  if (loadminimal) {
    amp1 = 6;
    amp3 = 6;
    return 0;
  }
  if (ueberschuss < 1420) {
    LED_blinking_times = 1;
    LED_blinking_duration = 0;
    amp1 = 6;
    amp3 = 6;
    return 0;
  }
  amp3 = ueberschuss / (voltage * 3);
  amp1 = ueberschuss / voltage;
  amp3 > 16 ? amp3 = 16 : 1;
  amp3 < 6 ? amp3 = 6 : 1;
  amp1 > 16 ? amp1 = 16 : 1;
  amp1 < 6 ? amp1 = 6 : 1;
  return 1;
}


int calcPhase() {
  if (loadminimal) {
    phase = 1;
    if (timer_changephase > 0) timer_changephase = 0;
    return status_phase == 1 ? 0 : 1;
  }
  if (only_x_phasig > 0) {
    phase = only_x_phasig;
    return phase;
  }
  if (status_loaddata <= 100 && ueberschuss >= 1420) {  // wenn nicht geladen wird, phase sofort anpassen
    ueberschuss >= 4200 ? phase = 2 : phase = 1;
    return phase;
  }
  //return 0 ==> keine Änderung
  //return 1 ==> zu 1er phase wechseln
  //return 2 ==> zu 3er phase wechseln

  if (status_phase == ((ueberschuss > 4140) + 1)) {  // bei keiner änderung nichts tun
    if (timer_changephase > 0) {
      timer_changephase = 0;
      Serial.println("Phase bleibt unveraendert.");
    }
    return 0;
  }

  if (timer_changephase == 0) {
    if (status_phase == 2 && status_batterie <= 50) {
      timer_changephase = millis() + phasechangemillis / 2;

    } else {
      timer_changephase = millis() + phasechangemillis;
    }
    Serial.println("Phasen aenderung initiiert.");
    return 0;
  } else if (timer_changephase <= millis()) {
    Serial.println("Phasen wird geaenderung.");
    timer_changephase = 0;
    phase = status_phase == 1 ? 2 : 1;
    return phase;
  }
  return 0;
}



bool Regulator() {
  byte m_temp = manuellereingriff();
  if (m_temp == 1) {
    Serial.println("Manueller Eingriff - Regler deaktiviert.");
    return 0;
  } else if (m_temp == -1) return 1;

  if (init_loop) {
    Serial.println("Initalloop, kein Senden von Daten");
    return 0;
  }
  if (loadmanual) {
    if (timer_changeload > 0) timer_changeload = 0;
    if (timer_changephase > 0) timer_changephase = 0;
    Serial.println("Manuelles Laden aktiv - Regler inaktiv");
    return 0;
  }
  vTaskDelay(1);
  bool load_calculation = calcLoad();
  vTaskDelay(1);

  if (load_calculation != load) {
    load = load_calculation;
    setGo("alw=" + String(load_calculation), 2);
    if (!load) {
      last_loadstop = millis();
      sign = 1;
    }
  }

  int Loading = calcLoading();
  int Phase = calcPhase();

  Serial.print("Ueberschuss: ");
  Serial.print(ueberschuss);
  Serial.print("W vs. real: ");
  Serial.print(status_loaddata);
  Serial.print("W - Laden: ");
  Serial.print(load);
  Serial.print(" - Phase: ");
  Serial.print(phase);
  Serial.print(" - Amp: ");
  Serial.println(phase == 1 ? amp1 : amp3);

  if (load) {
    if (Phase > 0 && status_phase != phase) setGo("psm=" + String(phase));
    if (status_amp != (phase == 1 ? amp1 : amp3)) {  // nur bei änderung senden
      phase == 1 ? setGo("amp=" + String(amp1), 2) : setGo("amp=" + String(amp3), 2);
    }
    realtoday[0] += real_load_data[11] / 3600000. * takt;
  }
  if (ueberschuss > 1400) maxtoday[0] += (phase == 1 ? voltage : 660) * (phase == 1 ? amp1 : amp3) / 3600000. * takt;
  return 0;
}
int pGrid, pPV, pAkku;

void sendueberschuss() {
  int ans;
  if (goeueberschuss) {
    ans = getPV("_sum/GridActivePower");
    ans == -1 ? Serial.print("sell: ") : pGrid = -ans;
    if (ans == -1) {
      Serial.println("No PV connection");
      return;
    }
    vTaskDelay(1);

    ans = getPV("_sum/EssDischargePower");
    ans == -1 ? Serial.print("discharge: ") : pAkku = -ans;  //status_discharge positiv = akku wird geladen;
    if (ans == -1) {
      Serial.println("No PV connection");
      return;
    }
    vTaskDelay(1);
    ans = getPV("_sum/ProductionDcActualPower");
    ans == -1 ? Serial.print("discharge: ") : pPV = -ans;
    if (ans == -1) {
      Serial.println("No PV connection");
      return;
    }
  } else {
    pGrid = -status_sell;
    pAkku = -status_discharge;
    pPV = -status_solar;
  }
  vTaskDelay(1);

  HTTPClient httpGo;
  String postData = "{\"pGrid\":" + String(pGrid) + ".,\"pPv\":" + String(pPV) + ".,\"pAkku\":" + String(pAkku) + ".}";
  String serverPath = String(GO_ADRESS) + String(GO_setADRESS) + "ids=" + postData;
  //Serial.println(serverPath);
  httpGo.begin(serverPath.c_str());
  vTaskDelay(1);
  int httpResponseCode = httpGo.GET();
  if (httpResponseCode > 0) {
    String ansS = httpGo.getString();
    //Serial.print("ans: ");
    //Serial.println(ansS);
    retrysGo = 0;
  } else {
    Serial.print("No Respond from Go-e Charger; Try to Set nbr: ");
    Serial.println(serverPath);
    vTaskDelay(1);
    LED_blinking_times = 2;
    LED_blinking_duration = 1;
  }
  httpGo.end();
  vTaskDelay(5);
  if (goeueberschuss) {
    ans = getGonbr("psm");
    vTaskDelay(1);
    ans == -1 ? Serial.print("phase: ") : status_phase = ans;
    ans = getGonbr("amp");
    vTaskDelay(1);
    ans == -1 ? Serial.print("amp: ") : status_amp = ans;
    ans = getGoload();
    vTaskDelay(1);
    ans == -1 ? Serial.print("loaddata: ") : status_loaddata = ans;
  }
}