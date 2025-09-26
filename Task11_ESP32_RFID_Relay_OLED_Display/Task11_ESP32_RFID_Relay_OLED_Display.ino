#include <Wire.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 2
#define RST_PIN 15
#define LED_PIN 27
#define BUZZER_PIN 33
#define RELAY_PIN 13

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte validUID[] = { 0x66, 0x76, 0x7D, 0x3D };

unsigned long relayOnTime = 0;
bool relayTriggered = false;

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(2, 20, "Scan an RFID card...");
  u8g2.sendBuffer();

  Serial.println("Scan an RFID card...");
}

void loop() {

  if (relayTriggered) {
    unsigned long timeLeft = 6000 - (millis() - relayOnTime);
    if ((long)timeLeft <= 0) {
      digitalWrite(RELAY_PIN, LOW);
      relayTriggered = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("Relay OFF");
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(2, 20, "Scan an RFID card...");
      u8g2.sendBuffer();
    } else {

      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(2, 20, "Access Granted!");
      u8g2.drawStr(2, 40, "Relay ON");
      char countdownStr[20];
      sprintf(countdownStr, "Time Left: %lu", (timeLeft + 999) / 1000);
      u8g2.drawStr(2, 60, countdownStr);
      u8g2.sendBuffer();
    }
    return;
  }

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

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

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  if (validCard) {
    Serial.println("Valid Card! Relay ON");
    digitalWrite(RELAY_PIN, HIGH);
    relayOnTime = millis();
    relayTriggered = true;

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);

  } else {
    Serial.println("Invalid Card! Buzzer Beeps");
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    u8g2.drawStr(2, 20, "Access Denied!");
    u8g2.drawStr(2, 40, "Try Again...");
    u8g2.sendBuffer();
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(2, 20, "Scan an RFID card...");
    u8g2.sendBuffer();
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
