int count = 0;

void setup() {
  Serial.begin(115200);
  pinMode(26, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  int val = digitalRead(26);

  if (val == 0) {
    count = count + 1;
    Serial.print("Button Pressed! Counter: ");
    Serial.println(count);
    delay(300);
  }

  switch (count) {
    case 1:
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      break;

    case 2:
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      break;

    case 3:
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      break;

    default:
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
      count = 0;
      Serial.println("Reset Counter to 0");
  }

  delay(100);
}

