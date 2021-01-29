void ledSetup() {
  // инициализировать встроенный светодиод как выход
  // initialize onboard LED as output
  pinMode(WEMOS_D4_LED, OUTPUT);
}

void ledBlink(int n, unsigned long d) {
  for (int i = 0; i <= n; i++) {
    digitalWrite(WEMOS_D4_LED, LOW);
    delay(d);
    digitalWrite(WEMOS_D4_LED, HIGH);
    delay(d);
  }
}

void ledOn() {
  digitalWrite(WEMOS_D4_LED, LOW);
}

void ledOff() {
  digitalWrite(WEMOS_D4_LED, HIGH);
}
