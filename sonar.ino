#define SONAR_ECHO_PIN WEMOS_D7 // Echo Pin
#define SONAR_TRIG_PIN WEMOS_D6 // Trigger Pin

float distance;

void sonarSetup() {
  pinMode(SONAR_TRIG_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);
}


void sonarLoop() {
  /* The following trigPin/echoPin cycle is used to determine the
    distance of the nearest object by bouncing soundwaves off of it. */
  digitalWrite(SONAR_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);
  long duration = pulseIn(SONAR_ECHO_PIN, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = float(duration) / 58.2f;
  dbg(2, "Distance: ");
  dbgLn(2, distance);
}

float sonarDistance() {
  return distance;
}
