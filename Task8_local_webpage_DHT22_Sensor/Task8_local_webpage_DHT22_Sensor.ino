#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT22

const char *ssid = "Prateek";
const char *password = "justdoelectronics@#12345";

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);
AsyncEventSource events("/events");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>ESP32 DHT22 Sensor</title>
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
      <h1>ESP32 DHT22 Sensor</h1>
      <p>Temperature: <span id='temperature' class='data'>--</span> &#8451;</p>
      <p>Humidity: <span id='humidity' class='data'>--</span> %</p>
    </div>
  </div>
  <script>
    if (!!window.EventSource) {
      var source = new EventSource('/events');
      source.addEventListener('sensorData', function(e) {
        let data = JSON.parse(e.data);
        document.getElementById('temperature').innerText = data.temperature;
        document.getElementById('humidity').innerText = data.humidity;
      }, false);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  dht.begin();

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
  server.addHandler(&events);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    char data[100];
    snprintf(data, sizeof(data), "{\"temperature\": %.2f, \"humidity\": %.2f}", temp, hum);
    events.send(data, "sensorData", millis());
  }
  delay(2000);
}
