#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

const char *ssid = "Prateek";
const char *password = "justdoelectronics@#12345";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

struct device {
  const int id;
  const int pin;
  const int btnPin;
  int prevBtnState;
  int status;  // 0:off, 1:on
};
struct device d1 = { 1, 14, 26, 0, 0 };
struct device d2 = { 2, 27, 25, 0, 0 };

String processor(const String &var) {
  if (var == "btn1txt") {
    return d1.status == 0 ? "ON" : "OFF";
  } else if (var == "btn2txt") {
    return d2.status == 0 ? "ON" : "OFF";
  } else if (var == "btn1class") {
    return d1.status == 0 ? "button" : "button2";
  } else if (var == "btn2class") {
    return d2.status == 0 ? "button" : "button2";
  }
  return String();
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>ESP32 Home Automation</title>
  <style>
    html { font-family: arial; display: inline-block; margin: 0px auto; text-align: center;}
    .button { background-color: mediumseagreen; border: none; color: white; padding: 10px 15px; font-size: 24px; cursor: pointer; margin: 3px; }
    .button2 { background-color: gray; border: none; color: white; padding: 10px 15px; font-size: 24px; cursor: pointer; margin: 3px; }
    .button3 { background-color: crimson; border: none; color: white; padding: 10px 15px; font-size: 24px; cursor: pointer; margin: 3px; }
  </style>
</head>
<body>
  <h1>ESP32 SMART Home!</h1>
  <h3 style='color: red;'>Switch 1 State</h3>
  <p><a href='/set?button_id=1'><button id='btn1' class='%btn1class%'>%btn1txt%</button></a></p>
  <h3 style='color: green;'>Switch 2 State</h3>
  <p><a href='/set?button_id=2'><button id='btn2' class='%btn2class%'>%btn2txt%</button></a></p>
  
  <p>
    <a href='/allon'><button class='button'>All ON</button></a>
    <a href='/alloff'><button class='button3'>All OFF</button></a>
  </p>
  <script>
    if (!!window.EventSource) {
      var source = new EventSource('/events');
      source.addEventListener('toggleState', function(e) {
        let jsonData = JSON.parse(e.data);
        const element = document.getElementById(jsonData.id);
        if (jsonData.status == 1) {
          element.innerHTML = 'OFF';
          element.className = "button2";
        } else {
          element.innerHTML = 'ON';
          element.className = "button";
        }
      }, false);
    }
  </script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(d1.pin, OUTPUT);
  pinMode(d2.pin, OUTPUT);

  pinMode(d1.btnPin, INPUT_PULLUP);
  pinMode(d2.btnPin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp32")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) delay(1000);
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("button_id")) {
      int id = request->getParam("button_id")->value().toInt();
      if (id == d1.id) togglePinState(&d1);
      else if (id == d2.id) togglePinState(&d2);
    }
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/allon", HTTP_GET, [](AsyncWebServerRequest *request) {
    toggleAll(1);
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/alloff", HTTP_GET, [](AsyncWebServerRequest *request) {
    toggleAll(0);
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    resetAll();
    request->send_P(200, "text/html", index_html, processor);
  });

  server.addHandler(&events);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  int newBtn1State = digitalRead(d1.btnPin);
  int newBtn2State = digitalRead(d2.btnPin);


  if (d1.prevBtnState == 1 && newBtn1State == 0) togglePinState(&d1);
  if (d2.prevBtnState == 1 && newBtn2State == 0) togglePinState(&d2);

  d1.prevBtnState = newBtn1State;
  d2.prevBtnState = newBtn2State;

  delay(10);
}

void togglePinState(struct device *d) {
  d->status = !d->status;
  digitalWrite(d->pin, d->status);
  char data[100];
  snprintf(data, sizeof(data), "{\"id\":\"btn%d\", \"status\":%d}", d->id, d->status);
  events.send(data, "toggleState", millis());
}

void toggleAll(int state) {
  if (d1.status != state) togglePinState(&d1);
  if (d2.status != state) togglePinState(&d2);
}

void resetAll() {
  toggleAll(0);
}
