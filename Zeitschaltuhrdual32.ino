
// ****************************************************************
// Sketch Esp32 Webserver Modular(Tab)
// created: Jens Fleischer, 2018-07-06
// last mod: Jens Fleischer, 2019-01-04
// For more information visit: https://fipsok.de
// ****************************************************************
// Hardware: Esp32
// Software: Esp32 Arduino Core 1.0.0 - 1.0.2 / 1.0.4
// Getestet auf: ESP32 NodeMCU-32s
/******************************************************************
  Copyright (c) 2018 Jens Fleischer. All rights reserved.

  This file is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This file is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*******************************************************************/
// Der WebServermodular stellt den Haupt Tab dar.
// #include "SPIFFS.h" #include <WebServer.h> müssen im Haupttab aufgerufen werden
// Die Funktionalität des ESP32 Webservers ist erforderlich.
// "server.onNotFound()" darf nicht im Setup des ESP32 Webserver stehen.
// Inklusive Arduino OTA-Updates
/**************************************************************************************/

#include <WebServer.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include "time.h"
#include <Preferences.h>

#define DEBUGGING             // Auskommentieren wenn keine Serielle Ausgabe erforderlich ist

#ifdef DEBUGGING
#define DEBUG(...) Serial.println(__VA_ARGS__)
#define DEBUG_F(...) Serial.printf("Funktion: %s meldet in Zeile: %d -> ", __PRETTY_FUNCTION__, __LINE__); Serial.println(__VA_ARGS__)
#else
#define DEBUG(...)
#define DEBUG_F(...)
#endif

struct tm tm;
Preferences preferences;
WebServer server(80);

void setup() {
  Serial.begin(115200);
  preferences.begin("config", false);
  DEBUG((String)"\nSketchname: " + (__FILE__) + "\nBuild: " + (__TIMESTAMP__) + "\n" );
  spiffs();
  admin();
  Connect();
  setupTime();
  setupSchaltUhr();
  setupHobbsMeter();
  ArduinoOTA.onStart([]() {
    save();                  // vor dem Sketch Update Betriebsstunden speichern
  });
  ArduinoOTA.begin();
  server.begin();
  DEBUG("HTTP Server gestartet\n\n");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  if (millis() < 0x2FFF || millis() > 0xFFFFF0FF) runtime();
  getLocalTime(&tm);        // Aktuallisiert die Uhrzeit
  dualSchaltuhr();
  reStation();
}
