const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <style>
      body {
        background-color: #2e2e2e;
        color: white;
        font-family: Arial;
        text-align: center;
        max-width: 100vw;
        margin: 0;
        padding: 0;
      }
      .container {
        display: flex;
        flex-direction: column;
        align-items: center;
        flex-wrap: wrap;
      }
      .row {
        display: flex;
        justify-content: space-between;
        width: 50%;
        padding: 5px;
      }
      .button_row{
        display: flex;
        flex-direction_row;
        flex-wrap: wrap;
        align-items: center;
      }
      button{
        margin: 0.5rem 1rem;
        border-radius: 0.5rem;
        box-shadow: 0 0 3px gray;
      }
      button:hover{
        filter: brightness(85%);
      }
      .rowtable {
        display: flex;
        justify-content: space-between;
        width: 100%;
      }
      .row span {
        flex: 1;
        text-align: right;
      }
      .row div {
        flex: 1;
        text-align: left;
      }
      table {
        margin-top: 20px;
        border-collapse: collapse;
        width: 70%;
        margin-left: auto;
        margin-right: auto;
      }
      th,
      td {
        border: 1px solid white;
        padding: 10px;
      }
      th {
        background-color: #444;
      }
      .grid-container {
        display: grid;
        grid-template-columns: auto 4rem 4rem 4rem auto;
        gap: 10px;
        margin: 20px;
      }
      .grid-container > span {
        grid-column-start: 1;
        grid-column: 1;
        text-align: right;
      }
      .grid-container > span + div {
        text-align: right;
      }
      .grid-container .div {
        white-space: nowrap;
        display: inline-block;
      }
      .row-highlight {
        background-color: #555;
      }
      button {
        padding: 10px;
        margin: 5px;
        border: none;
        cursor: pointer;
        background-color: red;
        color: white;
      }
      .activ {
        background-color: green;
      }
      .activ_clr {
        color: #71d152;
        text-decoration-line: underline;
      }
      .activ_click{
        text-decoration-line: underline;
        cursor: pointer;
      }
      .cell {
        display: inline;
      }
      .cell div {
        display: inline;
      }
      .comment {
        justify-content: left;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="button_row">
        <div><button id="loadmanualBtn">Mit Go-e App laden</button></div>
        <div><button id="prio_autoBtn">Prio Auto</button></div>
        <div><button id="loadminimalBtn">Minimal Beladung</button></div>
        <div>
          <button id="goeueberschussBtn">Nur per Go-e Ueberschuss laden</button>
        </div>
        <!--<div><button id="manuellerEingriffBtn">Manueller Eingriff</button></div>-->
        <!--<div><button id="overwriteBtn">Overwrite</button></div>-->
        <div><button id="otaonBtn">Updatable</button></div>
      </div>

      <div class="button_row">
        <span>Phasen:</span>
        <div><button id="only_x_phasigBtn0">Automatisch</button></div>
        <div><button id="only_x_phasigBtn1">1 Phasig</button></div>
        <div><button id="only_x_phasigBtn2">3 Phasig</button></div>
      </div>

      <div class="grid-container">
        <span>Ladestop durch zu niedrigen Batteriestand:</span>
        <div class="cell">
          <div id="ladestop_prozent"></div>
          <div>%</div>
        </div>
        <span>Power auf Phasen 1,2,3:</span>
        <div class="cell">
          <div id="real_load_data_7"></div>
          <div>W</div>
        </div>
        <div class="cell">
          <div id="real_load_data_8"></div>
          <div>W</div>
        </div>
        <div class="cell">
          <div id="real_load_data_9"></div>
          <div>W</div>
        </div>
        <span>Power Total:</span>
        <div class="cell">
          <div id="real_load_data_11"></div>
          <div>W</div>
        </div>
        <span>Netzbezug:</span>
        <div class="cell">
          <div id="pGrid"></div>
          <div>W</div>
        </div>
        <div></div>
        <div></div>
        <div class="comment">negativ = Verkauf; positiv = Einkauf</div>
        <span>Solarakku Beldadung:</span>
        <div class="cell">
          <div id="pAkku"></div>
          <div>W</div>
        </div>
        <div></div>
        <div></div>
        <div class="comment">negativ = Beladung; positiv = Entladung</div>
        <span>PV Produktion:</span>
        <div class="cell">
          <div id="pPV"></div>
          <div>W</div>
        </div>
        <span>Ueberschuss berechnet:</span>
        <div class="cell">
          <div id="ueberschuss"></div>
          <div>W</div>
        </div>
        <span>Solarakku Stand:</span>
        <div class="cell">
          <div id="status_batterie"></div>
          <div>%</div>
        </div>
        <span>Laden freigegeben und Phase</span>
        <div id="load"></div>
        <div id="phase"></div>
        <span>Timer Ladeaenderung:</span>
        <div class="cell">
          <div id="timer_changeload"></div>
          <div>sek</div>
        </div>
        <div></div>
        <div></div>
        <div class="comment">wenn 0 keine Aenderung erwartet</div>
        <span>Timer Phasenaenderung:</span>
        <div class="cell">
          <div id="timer_changephase"></div>
          <div>sek</div>
        </div>
        <div></div>
        <div></div>
        <div class="comment">wenn 0 keine Aenderung erwartet</div>
        <span>Amper 1-/3-Phasig:</span>
        <div class="cell">
          <div id="amp1"></div>
          <div>A</div>
        </div>
        <div class="cell">
          <div id="amp3"></div>
          <div>A</div>
        </div>
      </div>
    </div>
    <div class="rowtable">
      <table>
        <thead>
          <tr>
            <th></th>
            <th>Today</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
          </tr>
        </thead>
        <tbody id="dataTable">
          <tr id="realtodayRow">
            <td>Real geladen</td>
          </tr>
          <tr id="maxtodayRow">
            <td>Laden freigegeben</td>
          </tr>
          <tr id="uebertodayRow">
            <td>Ueberschuss</td>
          </tr>
        </tbody>
      </table>
    </div>
    <script>
      const yesno = ["Nein", "Ja"];
      let loadmanual = 0,
        prio_auto = 0,
        overwrite = 0,
        loadminimal = 0,
        goeueberschuss = 0,
        manuellerEingriff = 0,
        otaon = 0;
        ueberschuss = 0;
        amp1 = 0;
        amp3 = 0;
      let realtoday = new Array(14).fill("-");
      let maxtoday = new Array(14).fill("-");
      let uebertoday = new Array(14).fill("-");

      function fetchData() {
        var xhr = new XMLHttpRequest();
        xhr.open("POST", "/getData", true);
        xhr.setRequestHeader(
          "Content-Type",
          "application/x-www-form-urlencoded"
        );
        xhr.onload = function () {
          if (xhr.status === 200) {
            var data = JSON.parse(xhr.responseText);

            document.getElementById("ladestop_prozent").innerHTML =
              data.ladestop_prozent;
            document.getElementById("real_load_data_7").innerHTML =
              data.real_load_data_7;
            document.getElementById("real_load_data_8").innerHTML =
              data.real_load_data_8;
            document.getElementById("real_load_data_9").innerHTML =
              data.real_load_data_9;
            document.getElementById("real_load_data_11").innerHTML =
              data.real_load_data_11;
            document.getElementById("pGrid").innerHTML = data.pGrid;
            document.getElementById("pAkku").innerHTML = data.pAkku;
            document.getElementById("pPV").innerHTML = data.pPV;
            document.getElementById("status_batterie").innerHTML =
              data.status_batterie;
            document.getElementById("load").innerHTML = yesno[data.load];
            document.getElementById("timer_changeload").innerHTML =
              data.timer_changeload / 1000;
            document.getElementById("timer_changephase").innerHTML =
              data.timer_changephase / 1000;
            document.getElementById("amp1").innerHTML = data.amp1;
            document.getElementById("amp3").innerHTML = data.amp3;
            document.getElementById("ueberschuss").innerHTML = data.ueberschuss;
            document.getElementById("phase").innerHTML = data.phase;

            loadmanual = data.loadmanual;
            prio_auto = data.prio_auto;
            //overwrite = data.overwrite;
            loadminimal = data.loadminimal;
            goeueberschuss = data.goeueberschuss;
            //manuellerEingriff = data.manuellerEingriff;
            otaon = data.otaon;
            only_x_phasig = data.only_x_phasig;

            // Aktualisiere Knopf
            //updateButton("overwriteBtn", overwrite);
            //updateButton("manuellerEingriffBtn", manuellerEingriff);
            updateButton("loadmanualBtn", loadmanual);
            updateButton("prio_autoBtn", prio_auto);
            updateButton("loadminimalBtn", loadminimal);
            updateButton("goeueberschussBtn", goeueberschuss);
            updateButton("otaonBtn", otaon);
            updateButton("only_x_phasigBtn0", only_x_phasig == 0 ? 1 : 0);
            updateButton("only_x_phasigBtn1", only_x_phasig == 1 ? 1 : 0);
            updateButton("only_x_phasigBtn2", only_x_phasig == 2 ? 1 : 0);
            updateClickBtn("timer_changephase", data.timer_changephase);
            updateClickBtn("timer_changeload", data.timer_changeload);
            updateStyle("amp1", data.phase == 1 ? 1 : 0);
            updateStyle("amp3", data.phase == 3 ? 1 : 0);
            realtoday = data.realtoday;
            maxtoday = data.maxtoday;
            uebertoday = data.uebertoday;
            updateTable();
          }
        };
        xhr.send();
      }
      function updateStyle(buttonId, state) {
        var button = document.getElementById(buttonId);
        if (state == 1) {
          button.classList.add("activ_clr"); // F\'fcge die Klasse .activ hinzu
        } else {
          button.classList.remove("activ_clr"); // Entferne die Klasse .activ
        }
      }
      function updateButton(buttonId, state) {
        var button = document.getElementById(buttonId);
        if (state == 1) {
          button.classList.add("activ"); // F\'fcge die Klasse .activ hinzu
        } else {
          button.classList.remove("activ"); // Entferne die Klasse .activ
        }
      }
      function updateClickBtn(buttonId, state) {
        var button = document.getElementById(buttonId);
        if (state > 0) {
          button.classList.add("activ_click"); // F\'fcge die Klasse .activ hinzu
        } else {
          button.classList.remove("activ_click"); // Entferne die Klasse .activ
        }
      }

      function initializeTableCells() {
        const realtodayRow = document.getElementById("realtodayRow");
        const maxtodayRow = document.getElementById("maxtodayRow");
        const uebertodayRow = document.getElementById("uebertodayRow");

        for (let i = 0; i < 14; i++) {
          if (!realtodayRow.cells[i + 1]) {
            const realtodayCell = document.createElement("td");
            realtodayRow.appendChild(realtodayCell);
          }
          if (!maxtodayRow.cells[i + 1]) {
            const maxtodayCell = document.createElement("td");
            maxtodayRow.appendChild(maxtodayCell);
          }
          if (!uebertodayRow.cells[i + 1]) {
            const uebertodayCell = document.createElement("td");
            uebertodayRow.appendChild(uebertodayCell);
          }
        }
      }

      function updateTable() {
        const realtodayRow = document.getElementById("realtodayRow");
        const maxtodayRow = document.getElementById("maxtodayRow");
        const uebertodayRow = document.getElementById("uebertodayRow");

        for (let i = 0; i < 14; i++) {
          // Aktualisiere die realtoday-Zelle
          realtodayRow.cells[i + 1].textContent = realtoday[i] || "0";
          // Aktualisiere die maxtoday-Zelle
          maxtodayRow.cells[i + 1].textContent = maxtoday[i] || "0";
          // Aktualisiere die uebertoday-Zelle
          uebertodayRow.cells[i + 1].textContent = uebertoday[i] || "0";
        }
      }
      function toggleState(name, variable) {
        var newValue = variable == 1 ? 0 : 1;
        var xhr = new XMLHttpRequest();
        xhr.open("POST", "/setData", true);
        xhr.setRequestHeader(
          "Content-Type",
          "application/x-www-form-urlencoded"
        );
        xhr.send(name + "=" + newValue);
        variable = newValue;
        updateButton(name + "Btn", newValue);
      }

      function sendState(variable, value) {
        var xhr = new XMLHttpRequest();
        xhr.open("POST", "/setData", true);
        xhr.setRequestHeader(
          "Content-Type",
          "application/x-www-form-urlencoded"
        );
        xhr.send(variable + "=" + value);
      }
      function sendClick(variable) {
        sendState(variable, 0);
      }

      // Initialer Datenabruf
      initializeTableCells();
      fetchData();
      // Datenabruf alle 5 Sekunden
      setInterval(fetchData, 5000);

      document.getElementById("loadmanualBtn").onclick = function () {
        toggleState("loadmanual", loadmanual);
      };
      document.getElementById("prio_autoBtn").onclick = function () {
        toggleState("prio_auto", prio_auto);
      };
      document.getElementById("loadminimalBtn").onclick = function () {
        toggleState("loadminimal", loadminimal);
      };
      document.getElementById("goeueberschussBtn").onclick = function () {
        toggleState("goeueberschuss", goeueberschuss);
      };
      document.getElementById("otaonBtn").onclick = function () {
        toggleState("otaon", otaon);
      };
      
      document.getElementById("timer_changephase").onclick = function () {
        sendClick("timer_changephase");
      };
      document.getElementById("timer_changeload").onclick = function () {
        sendClick("timer_changeload");
      };

      document.getElementById("only_x_phasigBtn0").onclick = function () {
        sendState("only_x_phasig", only_x_phasig);
      };
      document.getElementById("only_x_phasigBtn1").onclick = function () {
        sendState("only_x_phasig", only_x_phasig);
      };
      document.getElementById("only_x_phasigBtn2").onclick = function () {
        sendState("only_x_phasig", only_x_phasig);
      };

    </script>
  </body>
</html>
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
void ini_webserver() {
  if (MDNS.begin("Pumpensteuerung")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Route für das Empfangen von POST-Requests für Variablen
  server.on("/getData", HTTP_POST, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"ladestop_prozent\":" + String(ladestop_prozent) + ",";
    json += "\"today\":" + String(today) + ",";
    json += "\"real_load_data_7\":" + String(real_load_data[7]) + ",";
    json += "\"real_load_data_8\":" + String(real_load_data[8]) + ",";
    json += "\"real_load_data_9\":" + String(real_load_data[9]) + ",";
    json += "\"real_load_data_11\":" + String(real_load_data[11]) + ",";
    json += "\"pGrid\":" + String(pGrid) + ",";
    json += "\"pAkku\":" + String(pAkku) + ",";
    json += "\"pPV\":" + String(pPV) + ",";
    json += "\"status_discharge\":" + String(status_discharge) + ",";
    json += "\"status_batterie\":" + String(status_batterie) + ",";
    json += "\"load\":" + String(load) + ",";
    json += "\"timer_changeload\":" + String(timer_changeload) + ",";
    json += "\"timer_changephase\":" + String(timer_changephase) + ",";
    json += "\"amp1\":" + String(amp1) + ",";
    json += "\"amp3\":" + String(amp3) + ",";
    json += "\"phasechangemillis\":" + String(phasechangemillis) + ",";
    json += "\"loadchangemillis\":" + String(loadchangemillis) + ",";
    json += "\"loadmanual\":" + String(loadmanual) + ",";
    json += "\"prio_auto\":" + String(prio_auto) + ",";
    json += "\"overwrite\":" + String(overwrite) + ",";
    json += "\"loadminimal\":" + String(loadminimal) + ",";
    json += "\"goeueberschuss\":" + String(goeueberschuss) + ",";
    json += "\"manuellerEingriff\":" + String(manuellerEingriff) + ",";
    json += "\"otaon\":" + String(otaon) + ",";
    json += "\"only_x_phasig\":" + String(only_x_phasig) + ",";
    json += "\"phase\":" + String(phase == 2 ? 3 : phase) + ",";
    json += "\"ueberschuss\":" + String(ueberschuss) + ",";

    json += "\"realtoday\":[";
    for (int i = 0; i < 14; i++) {
      json += String(realtoday[i]);
      if (i < 13) json += " ,";  // Komma nach jedem Wert außer dem letzten
    }
    json += " ],";

    // Sende maxtoday[] als JSON
    json += "\"maxtoday\":[";
    for (int i = 0; i < 14; i++) {
      json += String(maxtoday[i]);
      if (i < 13) json += " ,";
    }
    json += " ],";

    // Sende uebertoday[] als JSON
    json += "\"uebertoday\":[";
    for (int i = 0; i < 14; i++) {
      json += String(uebertoday[i]);
      if (i < 13) json += " ,";
    }
    json += " ]}";
    request->send(200, "application/json", json);
  });
  server.on("/clrval", HTTP_GET, [](AsyncWebServerRequest *request) {
    for (int i = 0; i < 14; i++) {
      maxtoday[i] = 0.;
      realtoday[i] = 0.;
      uebertoday[i] = 0.;
    }
    request->send(200, "text/plain", "OK");
  });
  server.on("/extoday", HTTP_GET, [](AsyncWebServerRequest *request) {
    maxtoday[0] = 3.3333;
    realtoday[0] = 2.2222;
    uebertoday[0] = 1.1111;
    request->send(200, "text/plain", "OK");
  });
  server.on("/phasenow", HTTP_GET, [](AsyncWebServerRequest *request) {
    timer_changephase == millis();
    request->send(200, "text/plain", "OK");
  });
  server.on("/loadnow", HTTP_GET, [](AsyncWebServerRequest *request) {
    timer_changeload == millis();
    request->send(200, "text/plain", "OK");
  });
  server.on("/nextday", HTTP_GET, [](AsyncWebServerRequest *request) {
    movetoday();
    Serial.println("nexday");
    request->send(200, "text/plain", "OK");
  });
  server.on("/sleep", HTTP_GET, [](AsyncWebServerRequest *request) {
    interval = 1;
    deepsleep();
    request->send(200, "text/plain", "OK");
  });
  // POST zum Setzen der 3-Zustands-Variablen
  server.on("/setData", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("loadmanual", true)) {
      loadmanual = request->getParam("loadmanual", true)->value().toInt();
      Serial.println("setted Data");
    }
    if (request->hasParam("prio_auto", true)) {
      prio_auto = request->getParam("prio_auto", true)->value().toInt();
      Serial.println("setted Data");
    }
    if (request->hasParam("overwrite", true)) {
      overwrite = request->getParam("overwrite", true)->value().toInt();
      Serial.println("setted Data");
    }
    if (request->hasParam("loadminimal", true)) {
      loadminimal = request->getParam("loadminimal", true)->value().toInt();
      Serial.println("setted Data");
    }
    if (request->hasParam("goeueberschuss", true)) {
      goeueberschuss = request->getParam("goeueberschuss", true)->value().toInt();
      Serial.println("setted Data");
    }
    if (request->hasParam("manuellerEingriff", true)) {
      manuellerEingriff = request->getParam("manuellerEingriff", true)->value().toInt();
      Serial.println("setted Data");
    }
    if (request->hasParam("otaon", true)) {
      otaon = request->getParam("otaon", true)->value().toInt();
      Serial.print("setted ota:");Serial.print(otaon);
    }
    if (request->hasParam("only_x_phasig", true)) {
      only_x_phasig = request->getParam("only_x_phasig", true)->value().toInt();
      Serial.println("setted Data");
    }
    if (request->hasParam("timer_changephase", true)) {
      timer_changephase = 0;
      Serial.println("setted Data");
    }
    if (request->hasParam("timer_changeload", true)) {
      timer_changeload = 0;
      Serial.println("setted Data");
    }
    request->send(200, "text/plain", "OK");
  });
  server.onNotFound(notFound);
  server.begin();
  Serial.println("HTTP server started");
}
