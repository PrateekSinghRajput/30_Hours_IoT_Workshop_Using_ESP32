int ledPin = 13;
int analogPin = 35;
int val = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  val = analogRead(analogPin);
  Serial.print("val Value: ");
  Serial.println(val);
  analogWrite(ledPin, val / 16);
}

