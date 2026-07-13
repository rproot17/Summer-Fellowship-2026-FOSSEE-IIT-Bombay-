#include <WiFi.h>
#include <WebServer.h>
#include <AccelStepper.h>

// --- Configuration ---
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Your Original Hardwired Pins
#define STEP_PIN 2
#define DIR_PIN 3
#define BTN_CW 4
#define BTN_CCW 5

// Motor Setup (1 = external stepper driver)
AccelStepper stepper(1, STEP_PIN, DIR_PIN);
float targetSpeed = 800.0; // Steps per second

// Web Server on port 80
WebServer server(80);

// Global state for web commands
enum MotorState { STOPPED, MOVING_CW, MOVING_CCW };
MotorState webState = STOPPED;

// --- HTML for iOS Web App ---
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
  <title>Motor Control</title>
  <style>
    body { font-family: -apple-system, BlinkMacSystemFont, sans-serif; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; margin: 0; background-color: #1c1c1e; color: white; user-select: none; }
    h1 { margin-bottom: 40px; font-weight: 600; }
    .btn { background-color: #007aff; color: white; border: none; padding: 20px 40px; font-size: 24px; border-radius: 14px; margin: 15px; width: 80%; max-width: 300px; touch-action: manipulation; cursor: pointer; }
    .btn:active { background-color: #005bb5; }
    .stop-btn { background-color: #ff3b30; }
    .stop-btn:active { background-color: #c92a20; }
  </style>
</head>
<body>
  <h1>Stepper Control</h1>
  <button class="btn" onpointerdown="sendCommand('/cw')" onpointerup="sendCommand('/stop')" onpointerleave="sendCommand('/stop')">Clockwise</button>
  <button class="btn" onpointerdown="sendCommand('/ccw')" onpointerup="sendCommand('/stop')" onpointerleave="sendCommand('/stop')">Anti-Clockwise</button>
  <button class="btn stop-btn" onclick="sendCommand('/stop')">Emergency Stop</button>

  <script>
    function sendCommand(cmd) {
      fetch(cmd).catch(err => console.error(err));
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // Configure Buttons to use internal Pull-Up resistors for GND wiring
  pinMode(BTN_CW, INPUT_PULLUP);
  pinMode(BTN_CCW, INPUT_PULLUP);

  // Configure Stepper Max Speed and Acceleration limits
  stepper.setMaxSpeed(2000.0);
  stepper.setAcceleration(1000.0);

  // Non-blocking background Wi-Fi initiation
  WiFi.begin(ssid, password);
  Serial.println("Wi-Fi connecting in background... Buttons are active now!");

  // Web Server Routes
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });
  
  server.on("/cw", []() {
    webState = MOVING_CW;
    server.send(200, "text/plain", "CW");
  });
  
  server.on("/ccw", []() {
    webState = MOVING_CCW;
    server.send(200, "text/plain", "CCW");
  });
  
  server.on("/stop", []() {
    webState = STOPPED;
    server.send(200, "text/plain", "STOP");
  });

  server.begin();
}

void loop() {
  // Only handle network traffic if Wi-Fi is actively connected
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  // Read physical buttons (LOW means pressed because completing circuit to GND)
  bool cwPressed = (digitalRead(BTN_CW) == LOW);
  bool ccwPressed = (digitalRead(BTN_CCW) == LOW);

  // Determine targeted speed based on inputs (Physical overrides Web command)
  if (cwPressed || webState == MOVING_CW) {
    stepper.setSpeed(targetSpeed);
  } 
  else if (ccwPressed || webState == MOVING_CCW) {
    stepper.setSpeed(-targetSpeed);
  } 
  else {
    stepper.setSpeed(0);
  }

  // Generate steps cleanly and smoothly every single CPU cycle
  stepper.runSpeed();
}
