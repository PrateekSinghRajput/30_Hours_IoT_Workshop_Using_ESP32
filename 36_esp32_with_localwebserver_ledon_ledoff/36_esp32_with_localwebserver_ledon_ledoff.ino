#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ABCD";
const char* password = "12345678";

WebServer server(80);

const int ledPin = 13;
bool ledState = false;

void handleRoot() {
    String html = "<!DOCTYPE html>";
    html += "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>body{text-align:center;font-family:Arial;margin:0;padding:20px;background:#f4f4f4;}";
    html += "h1{color:#333;} .container{background:white;padding:20px;border-radius:10px;box-shadow:0 0 10px rgba(0,0,0,0.1);}";
    html += "button{font-size:20px;padding:10px 20px;margin:10px;border:none;border-radius:5px;cursor:pointer;}";
    html += "#on{background:#28a745;color:white;} #off{background:#dc3545;color:white;}";
    html += "</style></head><body>";
    html += "<div class='container'><h1>ESP32 LED Control</h1>";
    html += "<p><strong>LED State: " + String(ledState ? "ON" : "OFF") + "</strong></p>";
    html += "<p><a href='/on'><button id='on'>LED ON</button></a>";
    html += "<a href='/off'><button id='off'>LED OFF</button></a></p>";
    html += "</div></body></html>";
    server.send(200, "text/html", html);
}

void handleLEDOn() {
    digitalWrite(ledPin, HIGH);
    ledState = true;
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}

void handleLEDOff() {
    digitalWrite(ledPin, LOW);
    ledState = false;
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}

void setup() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/on", handleLEDOn);
    server.on("/off", handleLEDOff);

    server.begin();
    Serial.println("HTTP Server Started");
}

void loop() {
    server.handleClient();
}
