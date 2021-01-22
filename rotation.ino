Servo servoR;
int servoRPos = 0;
boolean servoRP = true;

void rotationSetup()
{
  servoR.attach(WEMOS_D8);
}

void rotationLoop() {
  if (s6s.mode < 2) {
    return;
  }
  if (servoRP) {
    servoRPos += CONFIG_ROTATION_STEP;
    if (servoRPos >= s6s.rotationMax) {
      servoRP = false;
      servoRPos = s6s.rotationMax;
    }
  } else {
    servoRPos -= CONFIG_ROTATION_STEP;
    if (servoRPos <= s6s.rotationMin) {
      servoRP = true;
      servoRPos = s6s.rotationMin;
    }
  }
  servoR.write(servoRPos);
}

void rotationSetPosition(int p) {
  if (p < 0) {
    p = 0;
  } else if (p > 180) {
    p = 180;
  }
  if (p > servoRPos) {
    for (int pos = servoRPos; pos <= p; pos++) {
      delay(15);
      servoR.write(pos);
    }
  } else {
    for (int pos = servoRPos; pos >= p; pos--) {
      delay(15);
      servoR.write(pos);
    }
  }
  servoRPos = p;
}

int rotationPosition() {
  return servoRPos;
}
