#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22
#define LED_PIN 2
#define BUZZER_PIN 4

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte validUID[] = { 0xA1, 0xB2, 0xC3, 0xD4 };

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
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
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    Serial.println("Invalid Card! Buzzer Beeps");
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

