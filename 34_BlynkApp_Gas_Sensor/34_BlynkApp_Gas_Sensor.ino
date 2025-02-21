
#define MQ_PIN 32
#define ALERT_PIN 13
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_AUTH_TOKEN "B0RoRRUcGziNPOyh7j7YUUf093u3g63H"
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Prateek";
char pass[] = "justdoelectronics@#12345";

void setup() {
  Serial.begin(115200);
   Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  pinMode(ALERT_PIN, OUTPUT);
}

void loop() {
    Blynk.run();
    gas();
}

void gas() {
  int gasLevel = analogRead(MQ_PIN);
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);
  Blynk.virtualWrite(V3, gasLevel);

  if (gasLevel > 1400) {
    digitalWrite(ALERT_PIN, HIGH);
    Serial.println("⚠️ High Gas Level Detected! ⚠️");
  } else {
    digitalWrite(ALERT_PIN, LOW);
  }

  delay(1000);
}
