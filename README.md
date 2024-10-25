# Casolar
Solar car charging regulator for an ESP32, Fenecon PV System and Go-e Charger or as a basis for different system. 

You have a Go-e charger and have a Fenecon Energy Management System (FEMS / EMS) and want to connect them to charge your car with surplus solar power? This code gives you a good solution! If you don't have exactly these Systems, then the code may help you as a basis for your systems. 

What you need:
- ESP32 (no Pins are needed, so every board layout should work)
- Energy Management System with API functionality (Fenecon preferable)
- Electic Car Charger with API functionality (Go-E Charger preferable)

Setup:
- All three mentioned system should be in one Network system and reachable to each other

Code
- download code and extract the zip file
- Open Code in the Arduino IDE
  Settings for Arduino IDE:
  - ESP32 board manager URL: https://espressif.github.io/arduino-esp32/package_esp32_index.json
  - Select FireBeetle-ESP32 as board (or your own if you have other board)
- Put your wifi ssid and password into the code
- Lookup both IP adresses (EMS and charger) and enter them into the code
- Modify the code further to your personal wishes (optional)
- Use www or AI (for example ChatGPT) to debug errors on compiling

If you modify the code further please let me and the community benifit from them two. 
If you have wishes for further or other functionality please also let me know. 

contact me via GitHub or mail: petereinbanschbach@gmail.com

Tags:
#Überschussladen
#Go-e Charger
#Fenecon
#EMS
#ESP32
