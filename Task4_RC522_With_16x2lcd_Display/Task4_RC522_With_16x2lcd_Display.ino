#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22
#define LED_PIN 2
#define BUZZER_PIN 4


MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte validUID[] = { 0xA1, 0xB2, 0xC3, 0xD4 };

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("RFID System Init");
  delay(2000);
  lcd.clear();

  Serial.println("Scan an RFID card...");
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Card");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Card UID: ");

  bool validCard = true;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");

    lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    lcd.print(mfrc522.uid.uidByte[i], HEX);
    lcd.print(" ");

    if (mfrc522.uid.uidByte[i] != validUID[i]) {
      validCard = false;
    }
  }
  Serial.println();

  if (validCard) {
    Serial.println("Valid Card! LED ON");
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    
    lcd.setCursor(0, 1);
    lcd.print("Access Granted!");
  } else {
    Serial.println("Invalid Card! Buzzer Beeps");
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Access Denied!");
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Card");

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
