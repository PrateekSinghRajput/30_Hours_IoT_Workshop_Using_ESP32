#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 2
#define RST_PIN 15
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
Serial.begin(115200);
SPI.begin();
mfrc522.PCD_Init();
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
for (byte i = 0; i < mfrc522.uid.size; i++) {
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
Serial.print(mfrc522.uid.uidByte[i], HEX);
Serial.print(" ");
}

Serial.println();
mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1();
}