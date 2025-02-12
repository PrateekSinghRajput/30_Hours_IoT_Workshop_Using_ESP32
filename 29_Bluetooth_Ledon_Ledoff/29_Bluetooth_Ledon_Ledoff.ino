#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
const int ledPin = 13;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Prateek");
  pinMode(ledPin, OUTPUT);
  Serial.println("Bluetooth Started! Waiting for connection...");
}

void loop() {
  if (SerialBT.hasClient()) {
    if (SerialBT.available()) {
      char receivedChar = SerialBT.read();

      if (receivedChar == 'A') {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED ON");
        SerialBT.println("LED is ON");
      } else if (receivedChar == 'B') {
        digitalWrite(ledPin, LOW);
        Serial.println("LED OFF");
        SerialBT.println("LED is OFF");
      }
    }
  } else {
    Serial.println("Waiting for Bluetooth connection...");
  }
  delay(100);
}
