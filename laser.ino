const int LASER_PIN = WEMOS_D0;

void laserSetup() {
  pinMode(LASER_PIN, OUTPUT);
}

void laserOn() {
  digitalWrite(LASER_PIN, HIGH);
}

void laserOff() {
  digitalWrite(LASER_PIN, LOW);
}
