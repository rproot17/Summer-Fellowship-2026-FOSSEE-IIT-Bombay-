# Dobot Magician Go: Internal Battery-Powered Stepper Scissor Lift

![Project Status](https://img.shields.io/badge/Status-Completed-success)
![Platform](https://img.shields.io/badge/Platform-ESP32--C3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino-00979D)
![Institution](https://img.shields.io/badge/FOSSEE-IIT_Bombay-orange)

## 📌 Project Overview
This repository contains the mechanical design, control firmware, and integration details for a custom **Stepper-Actuated Scissor Lift** designed specifically for the **Dobot Magician Go** mobile robotic platform. 

The project bridges the gap between horizontal mobility and vertical manipulation in industrial automation simulations. By adding controlled vertical Z-axis actuation, the Dobot arm can access elevated storage bays, shelves, and conveyors without sacrificing its untethered mobility.

**Developed by:** Raj Prajapati (SVNIT)  
**Under the Guidance of:** Prof. Jayendran Venkateswaran (IEOR, IIT Bombay)  
**Program:** FOSSEE Internship, IIT Bombay (2026)

---

## ✨ Key Features
* **Power Independence:** Eliminates the need for external battery packs. The lift draws power directly from the Dobot Magician Go's internal battery via a highly efficient DC-DC Buck Converter.
* **Dual-Channel Control:**
  * **Wired Mode:** Direct serial UART integration with the Dobot's internal communication bus for autonomous coordination.
  * **Wireless Mode:** An onboard ESP32-C3 hosts an asynchronous Web Server for low-latency manual control via a mobile browser.
* **Ultra-Low Profile:** The scissor lift linkage folds entirely flat within the footprint of the AGV chassis, maintaining a low center of gravity during transit.
* **Frictionless Joints:** Incorporates 3D-printed linkages with pressed-in miniature deep groove radial ball bearings.

---

## 🧰 Hardware Requirements
* **Microcontroller:** ESP32-C3 RISC-V Development Board
* **Actuator:** NEMA 17 Bipolar Stepper Motor
* **Motor Driver:** Two-Phase H-Bridge Stepper Driver (e.g., A4988, TB6600, or similar)
* **Power Regulation:** Adjustable DC-DC Step-Down Buck Converter
* **Mechanical Components:**
  * Custom 3D Printed PLA+/PETG parts (Base plate, Top plate, Scissor links)
  * Metric Threaded Power Rod (Lead Screw) & Nut
  * Deep Groove Radial Ball Bearings
  * Tactile Push Buttons (for physical manual overrides)

---

## 💻 Software & Libraries
The firmware is written in C++ using the Arduino IDE framework.
Ensure you have the following libraries installed via the Arduino Library Manager:
* `WiFi.h` (Standard with ESP32 Core)
* `WebServer.h` (Standard with ESP32 Core)
* [`AccelStepper`](https://www.airspayce.com/mikem/arduino/AccelStepper/) by Mike McCauley

---

## 🚀 Setup and Installation

### 1. Mechanical Assembly
1. 3D print the provided CAD files (recommend 3+ perimeters for structural rigidity).
2. Press-fit the deep groove ball bearings into the central and sliding pivot joints.
3. Assemble the pantograph linkages and attach them to the base carriage and top tracking guides.
4. Mount the NEMA 17 motor to the base pocket and attach the threaded lead screw.

### 2. Electronics Wiring
* **Stepper Driver:** Connect `STEP_PIN` to GPIO 2 and `DIR_PIN` to GPIO 3 on the ESP32-C3.
* **Physical Buttons:** Connect the Clockwise (CW) button to GPIO 4 and Counter-Clockwise (CCW) to GPIO 5 (wired to GND using internal pull-ups).
* **Power:** Tap the Dobot's internal battery, route it through the Buck Converter (tuned to step down to 5V/3.3V), and power the ESP32 and motor driver logic.

### 3. Flashing the Firmware
1. Open the `firmware.ino` file in the Arduino IDE.
2. Update the Wi-Fi credentials in the configuration section:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
