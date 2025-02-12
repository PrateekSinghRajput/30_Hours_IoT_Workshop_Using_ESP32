#define MQ_PIN 32
#define ALERT_PIN 13

void setup() {
  Serial.begin(115200);
  pinMode(ALERT_PIN, OUTPUT);
}

void loop() {
  int gasLevel = analogRead(MQ_PIN);
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);

  if (gasLevel > 1400) {
    digitalWrite(ALERT_PIN, HIGH);
    Serial.println("⚠️ High Gas Level Detected! ⚠️");
  } else {
    digitalWrite(ALERT_PIN, LOW);
  }

  delay(1000);
}

