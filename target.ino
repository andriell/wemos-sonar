// Цель
// Target
int targetStart = -1;
int targetEnd = -1;

void targetSetup() {
  targetStart = -1;
  targetEnd = -1;
}

void targetLoop() {
  if (sonarDistance() < CONFIG_SECURITY_PERIMETER) {
    ledOn();
    beep();
    if (targetStart <= 0) {
      targetStart = rotationPosition();
      dbg(1, "Target start: ");
      dbgLn(1, targetStart);
    }
  } else if (targetStart >= 0) {
    ledOff();
    if (abs(rotationPosition() - targetStart) > CONFIG_MINIMAL_TARGET_SIZE) {
      targetEnd = rotationPosition();
      dbg(1, "Target end: ");
      dbgLn(1, targetEnd);
    } else {
      dbg(1, "Target is too small. Start: ");
      dbg(1, targetStart);
      dbg(1, " end: ");
      dbgLn(1, rotationPosition());
      targetStart = -1;
    }
  } else {
    targetEnd = -1;
  }
}

int targetGet() {
  if (targetStart > 0 && targetEnd > 0) {
    int r = (targetStart + targetEnd) / 2;
    dbg(1, "Target start: ");
    dbg(1, targetStart);
    dbg(1, " end: ");
    dbg(1, targetEnd);
    dbg(1, " center: ");
    dbgLn(1, r);
    return r;
  }
  return -1;
}

int targetGetStart() {
  return targetStart;
}

int targetGetEnd() {
  return targetEnd;
}

void targetReset() {
  dbgLn(1, "Target reset");
  targetStart = -1;
  targetEnd = -1;
}
