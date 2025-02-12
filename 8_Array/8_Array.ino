
const int ledPins[] = { 13, 12, 14, 27 };
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

void setup() {
  for (int i = 0; i < numLeds; i++) {  
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);  
    delay(500);
    digitalWrite(ledPins[i], LOW); 
    delay(500);
  }
}

