int count = 0;

void setup() {
  Serial.begin(115200);
  pinMode(26, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {
  int val = digitalRead(26);

  if (val == 0) {
    count = count + 1;
    Serial.print("Button Pressed! Counter: ");
    Serial.println(count);
    delay(300);
  }

  if (count == 1) {
    analogWrite(13, 30);
  } else if (count == 2) {
    analogWrite(13, 95);
  } else if (count == 3) {
    analogWrite(13, 255);
  } else {
    analogWrite(13, 0);
    count = 0;
  }

  delay(100);
}

