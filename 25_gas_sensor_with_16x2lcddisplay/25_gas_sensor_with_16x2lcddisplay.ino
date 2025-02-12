#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define MQ_PIN 32

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Welcome To");
  lcd.setCursor(0, 1);
  lcd.print("Our Projects");
  delay(1000);
  lcd.clear();
}

void loop() {
  int gasLevel = analogRead(MQ_PIN);
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);
  lcd.setCursor(0, 0);
  lcd.print("GAS Level :-");
  lcd.setCursor(0, 1);
  lcd.print(gasLevel);
  delay(1000);
}
