#include <esp_now.h>
#include <WiFi.h>

uint8_t peerAddress[] = { 0xAC, 0x15, 0x18, 0xD5, 0x53, 0x54 };

typedef struct struct_message {
  char message[100];
} struct_message;

struct_message outgoingMessage;
struct_message incomingMessage;

// Callback when data is received
void onDataRecv(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  Serial.print("Received: ");
  Serial.println(incomingMessage.message);
}

// Callback when data is sent
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("Message Sent Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  Serial.println("Type a message to send:");
  while (Serial.available() == 0) {}

  String inputMessage = Serial.readStringUntil('\n');
  inputMessage.trim();

  if (inputMessage.length() > 0) {
    inputMessage.toCharArray(outgoingMessage.message, 100);
    esp_now_send(peerAddress, (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));
    Serial.print("You: ");
    Serial.println(inputMessage);
  }

  delay(500);
}