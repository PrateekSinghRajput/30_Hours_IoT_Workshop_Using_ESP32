#include <esp_now.h>
#include <WiFi.h>

// Define the structure for the incoming message
typedef struct struct_message {
  float temperature;   // DS18B20 Temperature Data
  char message[100];   // Optional text message
} struct_message;

struct_message incomingMessage;

// Callback when data is received
void onDataRecv(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
  // Copy incoming data into our struct
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  
  // Print the sender's MAC address
  Serial.print("Received from: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", macAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  // Print the received temperature and message
  Serial.print("Temperature: ");
  Serial.print(incomingMessage.temperature);
  Serial.println(" °C");
  
  Serial.print("Message: ");
  Serial.println(incomingMessage.message);
  Serial.println("---------------------------");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(onDataRecv);
  
  Serial.println("Receiver is ready and listening for data...");
}

void loop() {
  // The receiver doesn't need to do anything in the main loop.
  // ESP-NOW callbacks will handle incoming data.
  delay(100);
}
