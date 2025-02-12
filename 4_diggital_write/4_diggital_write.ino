#define Button 26
#define led 13
int val;

void setup() {
  Serial.begin(115200);
  pinMode(Button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}

void loop() {
  val = digitalRead(Button);
  Serial.println(val);
  digitalWrite(13, val);
  delay(200);
}

