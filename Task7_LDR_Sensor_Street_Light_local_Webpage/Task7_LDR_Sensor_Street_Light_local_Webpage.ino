#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define LDR_PIN 34
#define LED_PIN 13

const char *ssid = "Prateek";
const char *password = "justdoelectronics@#12345";

AsyncWebServer server(80);
AsyncEventSource events("/events");
int threshold = 2000;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>ESP32 LDR Street Light</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 0; background-color: #f4f4f4; }
    .container { display: flex; justify-content: center; align-items: center; height: 100vh; }
    .card {
      background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
      width: 300px; text-align: center;
    }
    .data { font-size: 24px; font-weight: bold; color: #333; }
    h1 { font-size: 22px; color: #444; }
  </style>
</head>
<body>
  <div class='container'>
    <div class='card'>
      <h1>ESP32 LDR Street Light</h1>
      <p>Light Intensity: <span id='ldr_value' class='data'>--</span></p>
      <p>Street Light: <span id='led_status' class='data'>--</span></p>
      <p>Threshold: <input type='number' id='threshold' value='2000' onchange='updateThreshold()'></p>
    </div>
  </div>
  <script>
    if (!!window.EventSource) {
      var source = new EventSource('/events');
      source.addEventListener('sensorData', function(e) {
        let data = JSON.parse(e.data);
        document.getElementById('ldr_value').innerText = data.ldr_value;
        document.getElementById('led_status').innerText = data.led_status;
      }, false);
    }
    function updateThreshold() {
      let newThreshold = document.getElementById('threshold').value;
      fetch('/setThreshold?value=' + newThreshold);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/setThreshold", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      threshold = request->getParam("value")->value().toInt();
      Serial.print("New Threshold: ");
      Serial.println(threshold);
    }
    request->send(200, "text/plain", "Threshold updated");
  });

  server.addHandler(&events);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  int ldr_value = analogRead(LDR_PIN);
  String led_status;

  if (ldr_value < threshold) {
    digitalWrite(LED_PIN, HIGH);
    led_status = "ON";
  } else {
    digitalWrite(LED_PIN, LOW);
    led_status = "OFF";
  }

  char data[100];
  snprintf(data, sizeof(data), "{\"ldr_value\": %d, \"led_status\": \"%s\"}", ldr_value, led_status.c_str());
  events.send(data, "sensorData", millis());
  delay(2000);
}
