/* Настройки IP адреса */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void wsSetup() {
  if (!SPIFFS.begin()) {
    dbgLn(1, "An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.softAP(CONFIG_WEBSERVER_SSID, CONFIG_WEBSERVER_PASSWORD);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  //server.on("/", ws_index);
  server.onNotFound(ws_default);

  server.begin();
  dbgLn(1, "Server started");
}

void wsLoop() {
  server.handleClient();
}

void ws_index() {
  server.send(200, "text/html", "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>/</title></head><body><h1>/</h1></body></html>");
}

void ws_default() {
  if (!wsFileRead(server.uri())) {
    server.send(404, "text/html", "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>404 page not found</title></head><body><h1>404 page not found</h1></body></html>");
  }
}

bool wsFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = wsContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

String wsContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}
