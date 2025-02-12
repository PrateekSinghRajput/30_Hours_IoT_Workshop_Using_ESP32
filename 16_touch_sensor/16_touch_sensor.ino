const int Touch = 4;
const int led = 13;
int TouchVal, TouchThreshold = 50;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  delay(500);
}

void loop() {
  TouchVal = touchRead(Touch);
  Serial.println(TouchVal);

  if (TouchVal < TouchThreshold)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  delay(100);
}

