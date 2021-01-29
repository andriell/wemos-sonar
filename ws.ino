// Web Server
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void wsSetup() {
  WiFi.softAP(CONST_WEBSERVER_SSID, CONST_WEBSERVER_PASSWORD);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/sonar.svg", ws_svg);
  server.on("/sonar.txt", ws_svgTxt);
  server.on("/fire", HTTP_POST, ws_fire);
  server.on("/angle", HTTP_POST, ws_angle);
  server.on("/settings.json", HTTP_POST, ws_settings_post);
  server.onNotFound(ws_default);

  server.begin();
  dbgLn(1, "Server started");
}

void wsLoop() {
  server.handleClient();
}

void ws_angle() {
  int a = server.arg("a").toInt();
  rotationSetPosition(a);
  server.send(200, "application/json", "{'sucsess': true}");
}

void ws_fire() {
  triggerFire(rotationPosition());
  server.send(200, "application/json", "{'sucsess': true}");
}

void ws_settings_post() {
  String jsonStr = server.arg("plain");
  settingsLoadString(jsonStr);
  settingsSave();
  laserOff();
  laserOn();
  server.send(200, "application/json", "{'sucsess': true}");
}

void ws_svg() {
  server.send(200, "image/svg+xml", displaySvg());
}

void ws_svgTxt() {
  server.send(200, "text/plain", displaySvg());
}

void ws_default() {
  if (!wsFileRead(server.uri())) {
    settingsSave();
    server.send(404, "text/html", "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>404 page not found</title></head><body><h1>404 page not found</h1></body></html>");
  }
}

bool wsFileRead(String path) {
  if (path.endsWith("/")) path += "index.html";
  String contentType = wsContentType(path);
  if (SPIFFS.exists(path)) {
    dbg(1, "Web server. File read: ");
    dbgLn(1, path);
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  dbg(1, "Web server. File not found: ");
  dbgLn(1, path);
  return false;
}

String wsContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  else if(filename.endsWith(".svg")) return "image/svg+xml";
  else if(filename.endsWith(".json")) return "application/json";
  return "text/plain";
}
