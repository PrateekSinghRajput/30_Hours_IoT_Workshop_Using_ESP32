#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define POT_PIN 35

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Pot Value:");
  lcd.setCursor(0, 1);
  lcd.print("Percentage:");
}

void loop() {
  int potValue = analogRead(POT_PIN);
  int percentage = map(potValue, 0, 1023, 0, 100);

  lcd.setCursor(10, 0);
  lcd.print("    ");
  lcd.setCursor(10, 0);
  lcd.print(potValue);

  lcd.setCursor(12, 1);
  lcd.print("   ");
  lcd.setCursor(12, 1);
  lcd.print(percentage);
  lcd.print("%");

  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(" | Percentage: ");
  Serial.print(percentage);
  Serial.println("%");

  delay(500);
}

