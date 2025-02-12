#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Prateek");
  Serial.println("Bluetooth Started! Waiting for connection...");
}

void loop() {
  if (SerialBT.hasClient()) {
    SerialBT.println("Hello World");
    Serial.println("Sent: Hello World");
  } else {
    Serial.println("Waiting for Bluetooth connection...");
  }
  delay(1000);
}
