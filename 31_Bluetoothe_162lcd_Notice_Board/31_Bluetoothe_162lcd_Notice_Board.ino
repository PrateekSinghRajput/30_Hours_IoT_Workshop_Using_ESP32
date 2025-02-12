#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Prateek");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for Msg...");
}

void loop() {
  if (SerialBT.available()) {
    String message = SerialBT.readString();
    Serial.println("Received: " + message);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Msg: ");
    lcd.setCursor(0, 1);
    lcd.print(message);
  }
}
