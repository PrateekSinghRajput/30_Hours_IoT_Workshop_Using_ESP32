//Prateek
//www.justdoelectronics.com

#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define LED 12

#define BLYNK_AUTH_TOKEN "B0RoRRUcGziNPOyh7j7YUUf093u3g63H"
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Prateek";
char pass[] = "justdoelectronics@#12345";

BLYNK_WRITE(V0) {
  bool value1 = param.asInt();

  if (value1 == 1) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();
}
