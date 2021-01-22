// Скусковой ключек
// Trigger
Servo servoT;
boolean first = true;

void triggerSetup() {
  servoT.attach(WEMOS_D5);
  delay(20);
  servoT.write(90);
}

void triggerLoop() {
  servoT.write(90);
}

void triggerFire(int t) {
  if (!(s6s.mode == 1 || s6s.mode == 4)) {
    return;
  }
  float d = sonarDistance();
  int a = targetAmendment(d) + s6s.adjustment;
  dbg(1, "Fire. Is first: ");
  dbg(1, first);
  dbg(1, " Target: ");
  dbg(1, t);
  dbg(1, " Distance: ");
  dbg(1, d);
  dbg(1, " Amendment: ");
  dbgLn(1, a);

  if (first) {
    rotationSetPosition(t - a);
    trigger1();
  } else {
    rotationSetPosition(t + a);
    trigger2();
  }
  first = !first;
}

int targetAmendment(float distance) {
  if (distance > 77.3) {
    return 0;
  } else if (distance > 38.7) {
    return 1;
  } else if (distance > 25.8) {
    return 2;
  } else if (distance > 19.3) {
    return 3;
  } else if (distance > 15.4) {
    return 4;
  } else if (distance > 12.8) {
    return 5;
  } else if (distance > 11.0) {
    return 6;
  } else if (distance > 9.6) {
    return 7;
  } else if (distance > 8.5) {
    return 8;
  } else if (distance > 7.7) {
    return 9;
  }
  return 10;
}

void trigger1() {
  dbgLn(1, "Trigger 1");
  for (int pos = 90; pos >= 0; pos--) {
    dbg(2, "Trigger: ");
    dbgLn(2, pos);
    servoT.write(pos);
    delay(15);
    if (pos % 7 == 0) {
      beepTime(50ul);
    }
  }
}

void trigger2() {
  dbgLn(1, "Trigger 2");
  for (int pos = 90; pos <= 180; pos++) {
    dbg(2, "Trigger: ");
    dbgLn(2, pos);
    servoT.write(pos);
    delay(15);
    if (pos % 7 == 0) {
      beepTime(50ul);
    }
  }
}
