# 🤖 J6: Multi-Touch Interactive Surface

[![Platform](https://img.shields.io/badge/Platform-ESP8266%20(NodeMCU)-blueviolet.svg)](https://github.com/esp8266/Arduino)
[![License](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE.txt)

> **An interactive tactile platform based on force distribution analysis, controlled by the ESP8266 SoC.**

---

## 👨‍💻 Development Team

**Group J6 - 2025**

| Name | Student ID | GitHub Profile |
| :--- | :--- | :--- |
| **Dappiano, Máximo** | 03210/8 | [@maxmodapp](https://github.com/maxmodapp) |
| **Restucha, Tiago** | 03398/7 | [@TiagoRestucha](https://github.com/TiagoRestucha) |
| **Schwindt, Ignacio Andres** | 03229/0 | [@igna-s](https://github.com/igna-s) |

---

## 1. 📝 Introduction

This project involves the development of a tactile platform based on load cells, designed to simultaneously detect the pulses and movements of objects.

Inspired by the **MIT Media Lab SCALE system**, which utilizes distributed sensor networks to capture the magnitude and shape of applied force, our approach focuses on a reduced sensor array (triangulation) to achieve two-dimensional detection. The ultimate goal is to create a device analogous to a large-scale **touchpad** or mousepad capable of recognizing multiple interaction types, such as:
* **Tracking:** Real-time position estimation.
* **Drag:** Moving objects across the surface.
* **Push:** Detecting pressure intensity gestures (Z-axis).

---

## 2. 🎯 Objectives and Scope

The platform is designed to detect the force and position of contacts on a rigid surface. It enables interaction through gestures, opening new possibilities for tangible user interfaces.

### Primary Objectives (Fundamentals)
* **🏗️ Structure:** Develop a robust physical structure housing three load cells distributed in an equilateral triangle configuration. ✅
* **🔌 Electronics:** Implement the necessary circuitry to read analog outputs from the load cells using **HX711** (24-bit ADC) modules.✅
* **🧠 Algorithms:** Design firmware algorithms that, based on combined sensor readings, calculate the **2D position (X, Y)** of a load and detect **displacement**.✅
* **🖥️ Interface:** Develop a reactive **Web Interface** that visualizes the object's position and force intensity in real-time.✅
* **🧪 Validation:** Validate the prototype's performance through systematic testing and document the design to ensure reproducibility.✅

### Secondary Objectives
* **📈 Management:** Execute structured project management and analyze the feasibility of future feature expansions.✅
* **📦 Design:** Refine the industrial design to favor potential commercial adoption or academic demonstration.✅

---

## 3. 🏗️ Technical Architecture

The system operates on a **Barycentric Coordinate** principle.

The platform consists of **three load cells ($S_{1}, S_{2}, S_{3}$)** placed at strategic points (vertices) of a base. Each cell is connected to an **HX711 amplifier**, which digitizes the weight signal.

The core processing unit is an **ESP8266 (NodeMCU/Wemos)** microcontroller. It performs the following tasks:
1.  **Acquisition:** Reads raw data from the 3 ADCs.
2.  **Filtering:** Applies moving average filters to reduce mechanical noise.
3.  **Calculation:** Computes the Center of Pressure (CoP).
4.  **Transmission:** Sends telemetry (X, Y, Force) via **WebSockets** over Wi-Fi to a connected client.

Finally, a web server (hosted on the ESP8266) presents a visual HTML5 Canvas interface showing the trajectory and gestures.

---

## 4. 🛠️ Hardware Specifications

To replicate this system, the following hardware configuration is used:

| Component | Description | Function |
| :--- | :--- | :--- |
| **ESP8266 (NodeMCU)** | 32-bit MCU, 80MHz, Wi-Fi | Central processing and Web Server. |
| **Load Cells** | 3x Beam Type (5kg) | Force transduction. |
| **HX711** | 3x 24-bit ADCs | Signal amplification and digitization. |
| **Power Supply** | 5V 2A (MicroUSB) | System power. |

### Pinout Configuration
Since the ESP8266 has limited GPIOs, the following mapping is used to interface with the three HX711 modules:

* **SCK (Clock):** Shared across all modules on **GPIO 14 (D5)**.
* **DAT (Data) 1:** Connected to **GPIO 5 (D1)**.
* **DAT (Data) 2:** Connected to **GPIO 4 (D2)**.
* **DAT (Data) 3:** Connected to **GPIO 12 (D6)**.

---

## 5. 🔬 Theory of Operation

The position of the object is determined by calculating the **Center of Pressure (CoP)**. Assuming the sensors are arranged in a triangle, the coordinates $(X, Y)$ are derived from the weighted average of the forces $F_i$ measured at each vertex $(x_i, y_i)$.

### Mathematical Model

$$
F_{total} = \sum_{i=1}^{3} F_i
$$

$$
X_{cop} = \frac{\sum (F_i \cdot x_i)}{F_{total}} \quad , \quad Y_{cop} = \frac{\sum (F_i \cdot y_i)}{F_{total}}
$$

Where:
* $F_i$: Force measured by sensor $i$ (after tare).
* $x_i, y_i$: Physical coordinates of sensor $i$ relative to the center of the plate.

---

## 6. 🚀 Installation & Usage

1.  **Hardware Assembly:** Print the parts located in `/Estructura Fisica` and assemble the sensors in a triangular layout.
2.  **Firmware Upload:**
    * Open the project in `/Codigos` using **Arduino IDE** or **PlatformIO**.
    * Install the `HX711` and `WebSockets` libraries.
    * Upload the code to the ESP8266.
3.  **Calibration:**
    * Run the calibration sketch first to determine the scaling factor for your specific load cells.
    * Update the `calibration_factor` variable in the main firmware.
4.  **Run:**
    * Connect to the Wi-Fi network generated by the ESP8266 (or the network specified in code).
    * Open your browser and navigate to the IP address printed in the Serial Monitor.
    * Interact with the surface and observe the visual feedback.

---

## 📚 References

1.  **MIT Media Lab.** (n.d.). *SCALE: Distributed Sensing*. [Link](https://tangible.media.mit.edu/project/scale/)
2.  **Dwyer Omega.** *What is a Load Cell?* [Link](https://www.dwyeromega.com/en-us/resources/load-cells)
3.  **SparkFun Electronics.** *HX711 Datasheet & Hookup Guide*. [Link](https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide)

---
*© 2025 Group J6. Released under the GPL-3.0 License.*
