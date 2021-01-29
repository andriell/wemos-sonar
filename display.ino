SH1106Wire display(0x3C, WEMOS_D1, WEMOS_D2);

float distanceArray[181];
float sinArray[181];
float cosArray[181];

void displaySetup() {
  for (int i = 0; i <= 180; i++) {
    distanceArray[i] = -1.0f;
    sinArray[i] = sin(M_PI / 180.0f * float(i));
    cosArray[i] = cos(M_PI / 180.0f * float(i));
  }
  display.init();
  //display.flipScreenVertically();
  displayLog(String("Loading..."));
}

void displayLoop() {
  int i = rotationPosition();
  distanceArray[i] = sonarDistance();

  display.clear();
  int x, y, p;
  int prevX = -1;
  int prevY = -1;
  for (int i = 0; i <= 180; i++) {
    if (distanceArray[i] > 0) {
      x = round(64.0f + cosArray[i] * distanceArray[i] / s6s.distanceDivider);
      y = round(64.0f - sinArray[i] * distanceArray[i] / s6s.distanceDivider);

      //if (!(y < 10 && (x < 20 && x > 110)) && prevX > 0 && prevY > 0) {
      if (prevX > -1 && prevY > -1) {
        display.drawLine(dm(prevX, 0, 128), dm(prevY, 0, 64), dm(x, 0, 128), dm(y, 0, 64));
      }
      prevX = x;
      prevY = y;
    }

    if (i % 10 == 0) {
      x = float(64.0f + cosArray[i] * s6s.securityPerimeter / s6s.distanceDivider);
      y = float(64.0f - sinArray[i] * s6s.securityPerimeter / s6s.distanceDivider);
      display.setPixel(x, y);
      x = float(64.0f + cosArray[i] * 50.0f);
      y = float(64.0f - sinArray[i] * 50.0f);
      display.setPixel(x, y);
    }
  }

  p = targetGetStart();
  if (p >= 0) {
    for (float i = 0; i <= s6s.securityPerimeter / s6s.distanceDivider; i = i + 5.0f) {
      x = round(64.0f + cosArray[p] * i);
      y = round(64.0f - sinArray[p] * i);
      display.setPixel(x, y);
    }
  }

  p = targetGetEnd();
  if (p >= 0) {
    for (float i = 0; i <= s6s.securityPerimeter / s6s.distanceDivider; i = i + 5.0f) {
      x = round(64.0f + cosArray[p] * i);
      y = round(64.0f - sinArray[p] * i);
      display.setPixel(x, y);
    }
  }

  p = rotationPosition();
  for (float i = 0; i <= 64.0f; i = i + 2.0f) {
    x = round(64.0f + cosArray[p] * i);
    y = round(64.0f - sinArray[p] * i);
    display.setPixel(x, y);
  }

  display.setFont(Dialog_plain_8);
  display.drawString(0, 0, String(sonarDistance()));
  display.drawString(112, 0, String(liionP()) + "%");
  //display.drawCircle(display.getWidth() / 2, display.getHeight(), 25);
  //display.drawCircle(display.getWidth() / 2, display.getHeight(), 50);
  display.display();
}

int dm(int v, int _min, int _max) {
  if (v < _min) {
    return _min;
  }
  if (v > _max) {
    return _max;
  }
  return v;
}

String displaySvg() {
  int x, y, p;
  String r = "<svg width='128' height='64' xmlns='http://www.w3.org/2000/svg'>";
  r += "<rect width='100%' height='100%' fill='black' stroke='none' />";

  p = rotationPosition();
  x = round(64.0f + cosArray[p] * 64.0f);
  y = round(64.0f - sinArray[p] * 64.0f);
  r += "<polyline points='64 64 " + String(x) + " " + String(y) + "' stroke='white' stroke-width='1' stroke-dasharray='1, 1' fill='none' />";

  p = targetGetStart();
  if (p >= 0) {
    x = round(64.0f + cosArray[p] * s6s.securityPerimeter / s6s.distanceDivider);
    y = round(64.0f - sinArray[p] * s6s.securityPerimeter / s6s.distanceDivider);
    r += "<polyline points='64 64 " + String(x) + " " + String(y) + "' stroke='blue' stroke-width='1' stroke-dasharray='1, 1' fill='none' />";
  }

  p = targetGetEnd();
  if (p >= 0) {
    x = round(64.0f + cosArray[p] * s6s.securityPerimeter / s6s.distanceDivider);
    y = round(64.0f - sinArray[p] * s6s.securityPerimeter / s6s.distanceDivider);
    r += "<polyline points='64 64 " + String(x) + " " + String(y) + "' stroke='green' stroke-width='1' stroke-dasharray='1, 1' fill='none' />";
  }

  r += "<circle cx='64' cy='64' r='" + String(int(100.0f / s6s.distanceDivider)) + "' stroke='white' stroke-width='1' fill='none' stroke-dasharray='2, 5' />";
  r += "<circle cx='64' cy='64' r='" + String(int(s6s.securityPerimeter / s6s.distanceDivider)) + "' stroke='red' stroke-width='1' fill='none' stroke-dasharray='2, 5' />";
  r += "<polyline points='";
  for (int i = 0; i <= 180; i++) {
    if (distanceArray[i] > 0) {
      x = round(64.0f + cosArray[i] * distanceArray[i] / s6s.distanceDivider);
      y = round(64.0f - sinArray[i] * distanceArray[i] / s6s.distanceDivider);
      r += String(x) + " " + String(y) + " ";
    }
  }
  r += "' stroke='white' stroke-width='1' fill='none' />";
  r += "<rect width='25' height='8' fill='black' stroke='none' />";
  r += "<text x='0' y='8' fill='white' font-size='8' font-family='Arial'>" + String(sonarDistance()) + "</text>";
  r += "<rect x='108' width='25' height='8' fill='black' stroke='none' />";
  r += "<text x='108' y='8' fill='white' font-size='8' font-family='Arial'>" + String(liionP()) + "%</text>";
  r += "</svg>";
  return r;
}

int displayLogPosition = 0;

// Вывод строки лога на экран
// Displaying the log line on the screen
void displayLog(String s) {
  dbgLn(2, s);
  if (displayLogPosition > 5) {
    displayLogPosition = 0;
    display.clear();
  }
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, displayLogPosition * 10, s);
  displayLogPosition++;

  display.display();
}
