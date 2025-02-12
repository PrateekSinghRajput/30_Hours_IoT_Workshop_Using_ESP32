#define TRIG_PIN 5
#define ECHO_PIN 18
#define ALERT_PIN 25

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ALERT_PIN, OUTPUT);
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 10) {
    digitalWrite(ALERT_PIN, HIGH);
    Serial.println("⚠️ Object Too Close! ⚠️");
  } else {
    digitalWrite(ALERT_PIN, LOW);
  }

  delay(500);
}

