#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4  // DS18B20 data pin

const char *ssid = "Prateek";
const char *password = "justdoelectronics@#12345";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
AsyncWebServer server(80);
AsyncEventSource events("/events");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>ESP32 DS18B20 Sensor</title>
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
      <h1>ESP32 DS18B20 Sensor</h1>
      <p>Temperature: <span id='temperature_ds' class='data'>--</span> &#8457;</p>
    </div>
  </div>
  <script>
    if (!!window.EventSource) {
      var source = new EventSource('/events');
      source.addEventListener('sensorData', function(e) {
        let data = JSON.parse(e.data);
        document.getElementById('temperature_ds').innerText = data.temperature_ds;
      }, false);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  ds18b20.begin();

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
  ds18b20.requestTemperatures();
  float temp_ds = ds18b20.getTempFByIndex(0);  // Get DS18B20 temp in Fahrenheit

  if (!isnan(temp_ds)) {
    char data[100];
    snprintf(data, sizeof(data), "{\"temperature_ds\": %.2f}", temp_ds);
    events.send(data, "sensorData", millis());
  }
  delay(2000);
}
