#include <Wire.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 2
#define RST_PIN 15
#define LED_PIN 14
#define BUZZER_PIN 33
#define RELAY_PIN 13

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
MFRC522 mfrc522(SS_PIN, RST_PIN);

byte validUID[] = { 0x66, 0x76, 0x7D, 0x3D };

unsigned long relayOnTime = 0;
bool relayTriggered = false;

bool cardPresent = false;

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

  displayMessage("Scan an RFID card...");
  Serial.println("Scan an RFID card...");
}

void loop() {

  if (relayTriggered) {
    unsigned long timeLeft = 6000 - (millis() - relayOnTime);
    if ((long)timeLeft <= 0) {
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      relayTriggered = false;
      Serial.println("Relay OFF");
      displayMessage("Scan an RFID card...");
    } else {
      displayRelayCountdown(timeLeft);
    }
  }

  if (!mfrc522.PICC_IsNewCardPresent()) {

    cardPresent = false;
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) return;

  if (!cardPresent) {
    cardPresent = true;

    bool validCard = checkUID(mfrc522.uid.uidByte, mfrc522.uid.size);

    if (validCard) {
      Serial.println("Valid Card! Relay ON");
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, LOW);
      relayOnTime = millis();
      relayTriggered = true;
    } else {
      Serial.println("Invalid Card! Buzzer Beeps");
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      buzzerBeep();
      displayAccessDenied();
    }
  } else {
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

bool checkUID(byte* uid, byte uidLength) {
  if (uidLength != sizeof(validUID)) return false;
  for (byte i = 0; i < uidLength; i++) {
    if (uid[i] != validUID[i]) return false;
  }
  return true;
}

void displayMessage(const char* msg) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(2, 30, msg);
  u8g2.sendBuffer();
}

void displayRelayCountdown(unsigned long timeLeft) {
  char buf[30];
  sprintf(buf, "Time Left: %lu sec", (timeLeft + 999) / 1000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(2, 20, "Access Granted!");
  u8g2.drawStr(2, 40, "Relay ON");
  u8g2.drawStr(2, 60, buf);
  u8g2.sendBuffer();
}

void displayAccessDenied() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(2, 20, "Access Denied!");
  u8g2.drawStr(2, 40, "Try Again...");
  u8g2.sendBuffer();
}

void buzzerBeep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
  delay(100);
}
