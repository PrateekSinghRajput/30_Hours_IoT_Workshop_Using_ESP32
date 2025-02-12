int sensorPin = 35;
int digitalValue = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  digitalValue = analogRead(sensorPin);
  Serial.print("digital value = ");
  Serial.println(digitalValue);
  delay(1000);
}

