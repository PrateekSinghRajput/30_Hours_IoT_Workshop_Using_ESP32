#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void loop() {
  setColor(255, 0, 0);  // Red
  delay(1000);
  setColor(0, 255, 0);  // Green
  delay(1000);
  setColor(0, 0, 255);  // Blue
  delay(1000);
  setColor(255, 255, 0);  // Yellow
  delay(1000);
  setColor(0, 255, 255);  // Cyan
  delay(1000);
  setColor(255, 0, 255);  // Magenta
  delay(1000);
  setColor(255, 255, 255);  // White
  delay(1000);
}

