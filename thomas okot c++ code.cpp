#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Thomas";
const char* password = "okot655";

// LED Pins
#define LED1 23
#define LED2 22
#define LED3 21

WebServer server(80);

// HTML Dashboard with Branding
String dashboard = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Thomas Smart Home Systems - LED Dashboard</title>
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background-color: #f4f6f8;
      text-align: center;
      padding: 40px;
    }

    h2 {
      color: #333;
      margin-bottom: 10px;
    }

    p {
      color: #555;
      margin-bottom: 30px;
    }

    .btn {
      padding: 12px 30px;
      margin: 10px;
      font-size: 16px;
      color: white;
      background-color: #007BFF;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: background-color 0.3s ease;
    }

    .btn:hover {
      background-color: #0c00b3;
    }

    footer {
      margin-top: 50px;
      color: #777;
      font-size: 14px;
      border-top: 1px solid #ccc;
      padding-top: 10px;
    }

    @media (max-width: 600px) {
      .btn {
        width: 80%;
        font-size: 18px;
      }
    }
    {
    .on { background-color: #00e676; color: white; }
    .off { background-color: #ff1744; color: white; }
    }
  </style>
</head>
<body>
  <h2>Thomas Smart Home Systems - LED Lighting Control</h2>
  <p>Click buttons to control your lights</p>

  <button onclick="location.href='/led1/on'" class="btn">Turn ON LED 1</button>
  <button onclick="location.href='/led1/off'" class="btn">Turn OFF LED 1</button><br>

  <button onclick="location.href='/led2/on'" class="btn">Turn ON LED 2</button>
  <button onclick="location.href='/led2/off'" class="btn">Turn OFF LED 2</button><br>

  <button onclick="location.href='/led3/on'" class="btn">Turn ON LED 3</button>
  <button onclick="location.href='/led3/off'" class="btn">Turn OFF LED 3</button>

  <footer>Thomas Smart Home Systems | Team Innovate 2025</footer>
</body>
</html>

)rawliteral";

void handleRoot() {
  server.send(200, "text/html", dashboard);
}

// LED Control Endpoints
void handleLED(String led, bool state) {
  int pin = (led == "1") ? LED1 : (led == "2") ? LED2 : LED3;
  digitalWrite(pin, state ? HIGH : LOW);
  server.send(200, "text/html", "<h3>LED " + led + (state ? " ON" : " OFF") + "</h3><a href='/'>Go Back</a>");
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/led1/on", [](){ handleLED("1", true); });
  server.on("/led1/off", [](){ handleLED("1", false); });
  server.on("/led2/on", [](){ handleLED("2", true); });
  server.on("/led2/off", [](){ handleLED("2", false); });
  server.on("/led3/on", [](){ handleLED("3", true); });
  server.on("/led3/off", [](){ handleLED("3", false); });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
