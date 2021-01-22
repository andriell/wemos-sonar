/* Настройки IP адреса */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void wsSetup() {
  WiFi.softAP(CONFIG_WEBSERVER_SSID, CONFIG_WEBSERVER_PASSWORD);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/sonar.svg", ws_svg);
  server.on("/settings.json", HTTP_POST, ws_settings_post);
  server.onNotFound(ws_default);

  server.begin();
  dbgLn(1, "Server started");
}

void wsLoop() {
  server.handleClient();
}

void ws_settings_post() {
  String jsonStr = server.arg("plain");
  settingsLoadString(jsonStr);
  settingsSave();
  server.send(200, "application/json", "{'sucsess': true}");
}

void ws_svg() {
  String r = "<svg width='128' height='64' xmlns='http://www.w3.org/2000/svg'>";
  r += "<rect width='100%' height='100%' fill='black' stroke='none' />";
  r += "<polyline points='64 64 0 0' stroke='white' stroke-width='1' fill='none' />";
  r += "<circle cx='64' cy='64' r='50' stroke='white' stroke-width='1' fill='none' stroke-dasharray='2, 5' />";
  r += "<circle cx='64' cy='64' r='25' stroke='white' stroke-width='1' fill='none' stroke-dasharray='2, 5' />";
  r += "<polyline points='0 20 10 10 16 10 32 15 33 50 34 20 35 15 40 10 50 20' stroke='white' stroke-width='1' fill='none' />";
  r += "<rect width='25' height='8' fill='black' stroke='none' />";
  r += "<text x='0' y='8' fill='white' font-size='8' font-family='Arial'>123.56</text>";
  r += "<rect x='108' width='25' height='8' fill='black' stroke='none' />";
  r += "<text x='108' y='8' fill='white' font-size='8' font-family='Arial'>100%</text>";
  r += "</svg>";
  server.send(200, "image/svg+xml", r);
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
