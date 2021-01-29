#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Wire.h>
#include "SH1106Wire.h" // Library: ESP8266 and ESP32 OLED driver for SSD1306 displays
#include <ArduinoJson.h> // Library: ArduinoJson
#include <Servo.h>
#include "const.h"
#include "config.h"

#define MAX_UL       0xFFFFFFFFUL

#define WEMOS_A0     17
#define WEMOS_D0     16
#define WEMOS_D1     5
#define WEMOS_D2     4
#define WEMOS_D3     0
#define WEMOS_D4_LED 2
#define WEMOS_D5     14
#define WEMOS_D6     12
#define WEMOS_D7     13
#define WEMOS_D8     15
#define WEMOS_TX     1
#define WEMOS_RX     3



struct Settings {
  int mode; // m
  int rotationMin; // rm
  int rotationMax; // rx
  int rotationPause; // rp
  int adjustment; // a
  int beepMode; // bm
  int laserMode; // lm
  float distanceDivider; // dd
  float securityPerimeter; // sp
  int minimalTargeSize; // mts
};

// s6s is settings like i18n is internationalization
Settings s6s;

unsigned long mainMillis() {
  return  millis() + (MAX_UL - 30000UL);
}

void setup() {
  wemosSetup();
  dbgSetup();
  dbgLn(1, "Setup");
  if (!SPIFFS.begin()) {
    dbgLn(1, "An Error has occurred while mounting SPIFFS");
    return;
  }
  beepSetup();
  ledSetup();
  liionSetup();
  rotationSetup();
  triggerSetup();
  sonarSetup();
  laserSetup();
  displaySetup();
  targetSetup();
  wsSetup();
  laserOn();

  settingsLoadFile();
  rotationSetPosition(s6s.rotationMin);
}

int alarmCount = 0;

void loop() {
  unsigned long startMillis = mainMillis();
  srLoop();
  dbgLoop();
  wsLoop();
  if (s6s.mode < 1) {
    return;
  }
  sonarLoop();
  displayLoop();
  targetLoop();
  triggerLoop();
  if (targetGet() >= 0) {
    rotationSetPosition(targetGet());
    laserOff();
    beep();
    delay(1000);
    laserOn();
    if (alarmCount >= 5) {
      triggerFire(targetGet());
      rotationSetPosition(targetGetEnd());
      targetReset();
      alarmCount = 0;
    }
    alarmCount++;
  } else {
    alarmCount = 0;
    rotationLoop();
  }

  unsigned long endMillis = mainMillis();
  unsigned long executionTime;
  if (endMillis < startMillis) {
    executionTime = MAX_UL - startMillis;
    executionTime += endMillis + 1;
  } else {
    executionTime = endMillis - startMillis;
  }
  if (executionTime < s6s.rotationPause) {
    delay(s6s.rotationPause - executionTime);
  }
}
