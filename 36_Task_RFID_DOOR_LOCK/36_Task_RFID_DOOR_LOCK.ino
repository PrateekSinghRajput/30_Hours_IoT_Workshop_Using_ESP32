#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 2
#define RST_PIN 15
#define LED_PIN 13

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte validUID[] = { 0x43, 0x23, 0xB3, 0x29 }; //43 23 B3 29

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Scan an RFID card...");
}

void loop() {

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("Card UID: ");
  bool validCard = true;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
    if (mfrc522.uid.uidByte[i] != validUID[i]) {
      validCard = false;
    }
  }
  Serial.println();

  if (validCard) {
    Serial.println("Valid Card! LED ON");
    digitalWrite(LED_PIN, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("  Welcome To");
    lcd.setCursor(0, 1);
    lcd.print("  Door Lock");
    delay(5000);
    lcd.clear();
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("Invalid Card! Buzzer Beeps");
    digitalWrite(LED_PIN, LOW);
    lcd.clear();
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
