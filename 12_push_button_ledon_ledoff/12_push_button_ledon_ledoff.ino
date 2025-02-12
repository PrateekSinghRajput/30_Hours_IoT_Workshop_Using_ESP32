int pushButton = 12;
int LED = 14;

void setup() {
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {

  int buttonState = digitalRead(pushButton);
  digitalWrite(LED, (!(buttonState)));
  delay(1);
}
