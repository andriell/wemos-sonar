Servo servoR;
int servoRPos = 0;
boolean servoRP = true;

void rotationSetup()
{
  servoR.attach(WEMOS_D8);
}

void rotationLoop()
{
  if (servoRP) {
    servoRPos += CONFIG_ROTATION_STEP;
    if (servoRPos >= 180) {
      servoRP = false;
      servoRPos = 180;
    }
  } else {
    servoRPos -= CONFIG_ROTATION_STEP;
    if (servoRPos <= 0) {
      servoRP = true;
      servoRPos = 0;
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
