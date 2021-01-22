#define SETTINGS_SIZE 512

struct Settings {
  int mode; // m
  int rotationMin; // rm
  int rotationMax; // rx
  int rotationStep; // rs
  int beepMode; // bm
  int laserMode; // lm
  float distanceDivider; // dd
  float securityPerimeter; // sp
  int minimalTargeSize; // mts
};

Settings s6s;

void settingsSetup() {
  settingsLoadFile();
  //settingsSave();
}

String settingsSave() {
  StaticJsonDocument<SETTINGS_SIZE> doc;
  doc["m"] = s6s.mode;
  doc["rm"] = s6s.rotationMin;
  doc["rx"] = s6s.rotationMax;
  doc["rs"] = s6s.rotationStep;
  doc["bm"] = s6s.beepMode;
  doc["lm"] = s6s.laserMode;
  doc["dd"] = s6s.distanceDivider;
  doc["sp"] = s6s.securityPerimeter;
  doc["mts"] = s6s.minimalTargeSize;
  String jsonString;
  int jsonLenght = serializeJson(doc, jsonString);
  if (jsonLenght == 0 ) {
    dbgLn(1, "Error saving settings");
  }
  File file = SPIFFS.open("/settings.json", "w");
  int bytesWritten = file.print(jsonString);
  file.close();
  if (bytesWritten == 0 ) {
    dbgLn(1, "Error saving settings");
  }
  dbg(1, "Saving settings: ");
  dbgLn(1, jsonString);
}

void settingsLoadFile() {
  File file = SPIFFS.open("/settings.json", "r");
  String jsonString = file.readString();
  file.close();
  settingsLoadString(jsonString);
}

void settingsLoadString(String jsonString) {
  StaticJsonDocument<SETTINGS_SIZE> doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  if (error) {
    dbg(1, "Error deserialization settings: ");
    dbgLn(1, jsonString);
    return;
  }
  dbg(1, "Load settings: ");
  dbgLn(1, jsonString);

  if (doc.containsKey("m")) {
    s6s.mode = doc["m"];
  } else {
    s6s.mode = 1;
  }
  if (doc.containsKey("rm")) {
    s6s.rotationMin = doc["rm"];
  } else {
    s6s.rotationMin = 0;
  }
  if (doc.containsKey("rx")) {
    s6s.rotationMax = doc["rx"];
  } else {
    s6s.rotationMax = 180;
  }
  if (doc.containsKey("rs")) {
    s6s.rotationStep = doc["rs"];
  } else {
    s6s.rotationStep = 50;
  }
  if (doc.containsKey("bm")) {
    s6s.beepMode = doc["bm"];
  } else {
    s6s.beepMode = 1;
  }
  if (doc.containsKey("lm")) {
    s6s.laserMode = doc["lm"];
  } else {
    s6s.laserMode = 1;
  }
  if (doc.containsKey("dd")) {
    s6s.distanceDivider = doc["dd"];
  } else {
    s6s.distanceDivider = 2.0f;
  }
  if (doc.containsKey("sp")) {
    s6s.securityPerimeter = doc["sp"];
  } else {
    s6s.securityPerimeter = 50.0f;
  }
  if (doc.containsKey("mts")) {
    s6s.minimalTargeSize = doc["mts"];
  } else {
    s6s.minimalTargeSize = 5;
  }
}

struct Settings settings() {
  return s6s;
}
