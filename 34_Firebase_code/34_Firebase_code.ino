// https://attendance-ed3eb-default-rtdb.firebaseio.com/
// PQbHd2LvThsAr7bAi2Cj4KSbbIJlA59xPvkC6WwU

#include <WiFi.h>
#include <FirebaseESP32.h>

// WiFi Credentials
#define WIFI_SSID "Prateek"
#define WIFI_PASSWORD "kejujdjdj@#12345"

// Firebase Credentials
#define FIREBASE_HOST "attendance-ed3eb-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "PQbHd2LvThsAr7bAi2Cj4KSbbIJlA59xPvkC6WwU"

// Firebase Instance
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

#define GAS_SENSOR_PIN 35

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi");
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  int gasValue = analogRead(GAS_SENSOR_PIN);
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);

  if (Firebase.setInt(firebaseData, "/gas_sensor/value", gasValue)) {
    Serial.println("Data sent to Firebase");
  } else {
    Serial.print("Firebase Error: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(5000);
}
