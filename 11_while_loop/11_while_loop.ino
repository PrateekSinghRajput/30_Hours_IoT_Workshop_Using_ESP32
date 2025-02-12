

void setup() {
  Serial.begin(115200);
}

void loop() {
  int count = 1;
  while (count <= 5) {
    Serial.print("Count: ");
    Serial.println(count);
    count++;
    delay(500);
  }
  delay(5000);
}

