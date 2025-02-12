#define Button 26
int val;

void setup() {
  Serial.begin(115200);
  pinMode(Button, INPUT_PULLUP);
}
void loop() {
  val = digitalRead(Button);
  Serial.println(val);
  delay(200);
}

