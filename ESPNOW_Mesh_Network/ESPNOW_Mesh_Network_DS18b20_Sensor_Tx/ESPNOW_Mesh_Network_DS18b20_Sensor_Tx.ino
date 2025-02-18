#include <esp_now.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 Sensor Setup
#define ONE_WIRE_BUS 4  // DS18B20 data pin (adjust as needed)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Define number of peers
#define NUM_PEERS 5

// List of peer MAC addresses (Replace these with actual MAC addresses)
uint8_t peerAddresses[NUM_PEERS][6] = {
  { 0xAC, 0x15, 0x18, 0xD5, 0x53, 0x54 },
  { 0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC },
  { 0x30, 0xAE, 0xA4, 0xDD, 0xEE, 0xFF },
  { 0x34, 0x12, 0xA1, 0x55, 0x66, 0x77 },
  { 0x38, 0x2C, 0xA2, 0x88, 0x99, 0x00 }
};

// Message structure that includes DS18B20 sensor data and an optional text message
typedef struct struct_message {
  float temperature;
  char message[100];
} struct_message;

struct_message outgoingMessage;
struct_message incomingMessage;

// Callback function executed when data is received
void onDataRecv(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  Serial.print("Received from ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", macAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  Serial.print("Received Temperature: ");
  Serial.print(incomingMessage.temperature);
  Serial.println(" °C");

  Serial.print("Received Message: ");
  Serial.println(incomingMessage.message);
}

// Callback function executed when data is sent
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("Message sent to ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", macAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" - Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  // Initialize DS18B20 sensor
  sensors.begin();

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callbacks
  esp_now_register_recv_cb(onDataRecv);
  esp_now_register_send_cb(onDataSent);

  // Add each peer to ESP-NOW
  for (int i = 0; i < NUM_PEERS; i++) {
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerAddresses[i], 6);
    peerInfo.channel = 0;  // use the current WiFi channel
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.print("Failed to add peer: ");
      for (int j = 0; j < 6; j++) {
        Serial.printf("%02X", peerAddresses[i][j]);
        if (j < 5) Serial.print(":");
      }
      Serial.println();
    } else {
      Serial.print("Added peer: ");
      for (int j = 0; j < 6; j++) {
        Serial.printf("%02X", peerAddresses[i][j]);
        if (j < 5) Serial.print(":");
      }
      Serial.println();
    }
  }
}

void loop() {
  // Request temperature from DS18B20
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  outgoingMessage.temperature = temperature;

  // Ask user for a message to send
  Serial.println("Type a message to send:");
  while (Serial.available() == 0) {
    // Wait for input
    delay(10);
  }
  String inputMessage = Serial.readStringUntil('\n');
  inputMessage.trim();

  // Prepare outgoing message
  if (inputMessage.length() > 0) {
    inputMessage.toCharArray(outgoingMessage.message, sizeof(outgoingMessage.message));
  } else {
    strcpy(outgoingMessage.message, "No text provided");
  }

  // Send message to all peers
  for (int i = 0; i < NUM_PEERS; i++) {
    esp_now_send(peerAddresses[i], (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));
    Serial.print("Sent to peer ");
    Serial.print(i+1);
    Serial.print(": Temperature ");
    Serial.print(temperature);
    Serial.println(" °C");
  }
  
  delay(1000);
}
