#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BluetoothSerial.h"

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 16, 2);
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Prateek");

  sensors.begin();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp Sensor Init");
  delay(2000);
  lcd.clear();
}

void loop() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  SerialBT.print("Temperature: ");
  SerialBT.print(temperature);
  SerialBT.println(" °C");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  delay(1000);
}