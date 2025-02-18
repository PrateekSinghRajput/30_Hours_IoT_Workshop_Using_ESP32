#include <WiFi.h>
#include <esp_now.h>

#define MY_NAME "SLAVE_NODE"
const int led = 2; // ESP32 onboard LED

// Define the structure to receive data
typedef struct {
    int state;
} dataPacket;

// Callback function when data is received
void dataReceived(const uint8_t *mac, const uint8_t *data, int len) {
    char macStr[18];
    dataPacket packet;
  
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", 
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    Serial.println();
    Serial.print("Received data from: ");
    Serial.println(macStr);

    memcpy(&packet, data, sizeof(packet));

    Serial.print("Button state: ");
    Serial.println(packet.state);

    digitalWrite(led, packet.state);
}

void setup() {
    pinMode(led, OUTPUT);
    Serial.begin(115200);

    Serial.println("\nInitializing...");
    Serial.println(MY_NAME);
    Serial.print("My MAC address is: ");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_STA); // Set ESP32 in Station mode

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed");
        return;
    }

    esp_now_register_recv_cb(dataReceived);  // Register the callback function

    Serial.println("ESP-NOW Initialized.");
}

void loop() {
    // No need for anything here
}
