#include "BluetoothSerial.h"
#define TRIG_PIN  5   
#define ECHO_PIN  18  

BluetoothSerial SerialBT; 

void setup() {
    Serial.begin(115200);  
    SerialBT.begin("Prateek"); 
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.println("Bluetooth Started! Connect to ESP32_BT");
}

float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = (duration * 0.0343) / 2;
    return distance;
}

void loop() {
    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    SerialBT.print("Distance: ");
    SerialBT.print(distance);
    SerialBT.println(" cm");
    
    delay(1000); 
}
