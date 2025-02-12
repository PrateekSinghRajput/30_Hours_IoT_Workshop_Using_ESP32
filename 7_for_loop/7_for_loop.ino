
void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 1; i <= 10; i++) {
    Serial.print("Count: ");
    Serial.println(i);
    delay(500);
  }
}

