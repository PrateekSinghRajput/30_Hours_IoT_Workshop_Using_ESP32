#define ENA 5
#define IN1 18
#define IN2 19

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  // Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200);
  delay(3000);

  // Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(1000);

  // Reverse
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200);
  delay(3000);

  // Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(1000);
}

