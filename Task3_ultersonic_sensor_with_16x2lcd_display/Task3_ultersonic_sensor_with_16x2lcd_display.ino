#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define ALERT_PIN 25

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ALERT_PIN, OUTPUT);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Ultrasonic Init");
  delay(2000);
  lcd.clear();
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");

  if (distance < 10) {
    digitalWrite(ALERT_PIN, HIGH);
    Serial.println("⚠️ Object Too Close! ⚠️");
    lcd.setCursor(0, 1);
    lcd.print("!! ALERT !!");
  } else {
    digitalWrite(ALERT_PIN, LOW);
  }

  delay(500);
}
