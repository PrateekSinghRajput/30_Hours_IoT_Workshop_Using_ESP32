#include <WiFi.h>
#include <esp_now.h>

const int button = 26;
#define MY_NAME "CONTROLLER_NODE"
uint8_t receiverAddress[] = {0x3C, 0x8A, 0x1F, 0x7F, 0x36, 0x30};  // Update with receiver's MAC address

struct dataPacket {
  int state;
};

dataPacket packet;

void transmissionComplete(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Transmission Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  pinMode(button, INPUT_PULLUP);
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.print("My MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_register_send_cb(transmissionComplete);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("ESP-NOW Initialized");
}

void loop() {
  packet.state = digitalRead(button);
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&packet, sizeof(packet));
  if (result != ESP_OK) {
    Serial.println("Send failed");
  }
  delay(30);
}
