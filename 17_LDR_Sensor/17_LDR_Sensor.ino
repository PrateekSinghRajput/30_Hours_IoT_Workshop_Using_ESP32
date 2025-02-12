#define LDR_PIN 34
#define LED_PIN 12

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Intensity: ");
  Serial.println(lightLevel);

  if (lightLevel < 1500) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);
}

