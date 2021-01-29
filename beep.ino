const int BUZZER_PIN = WEMOS_D3;

void beepSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 2000, 100);
}

void beep(byte n) {
  if(s6s.beepMode < 1) {
    return;
  }
  for (byte i = 1; i < n; i++) {
    tone(BUZZER_PIN, 2000, 50);
    delay(100);
  }
}

void beep() {
  if(s6s.beepMode < 1) {
    return;
  }
  tone(BUZZER_PIN, 2000, 100);
}

void beepTime(unsigned long t) {
  if(s6s.beepMode < 1) {
    return;
  }
  tone(BUZZER_PIN, 2000, t);
}
