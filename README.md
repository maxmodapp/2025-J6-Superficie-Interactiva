# 🤖 Project Plan J6: Multi-Touch Platform with Load Cells
(README under construction, it have the "Plan de Poryecto Info")
### 👨‍💻 Development Team
- Dappiano, Máximo - [03210/8]
- Restucha, Tiago - [03398/7]
- Schwindt, Ignacio Andres - [03229/0]

***

## 1. 📝 Introduction

The project consists of developing a tactile platform based on load cells, designed to simultaneously detect the pulses and movements of multiple objects. While inspired by MIT's SCALE system, which uses distributed sensor networks to capture the magnitude and shape of applied force, our approach will focus on a reduced number of sensors and two-dimensional detection. The final goal is to create a device similar to a touchpad or *mousepad* that can recognize multiple interactions, such as dragging multiple fingers (drag) and pushing (push).

***

## 2. 🎯 Objective and Scope

The platform will be designed to detect the force and position of multiple simultaneous contacts. It will also allow interaction through gestures like dragging and pressing, opening new possibilities for large-scale user interfaces.

### Primary Objectives (Fundamentals)
-   **🏗️ Structure:** Develop a basic physical structure that houses three evenly distributed load cells.
-   **🔌 Electronics:** Implement the necessary electronics to read the outputs of the load cells using HX711 modules.
-   **🧠 Algorithms:** Design algorithms that, based on the combined readings, can calculate the 2D position of a load, detect displacement (drag), and distinguish pressure gestures (push).
-   **🖥️ Interface:** Develop a web interface that displays the object's position in real-time and visualizes detected gestures.
-   **🧪 Validation:** Validate the prototype's performance through systematic testing and document the design to ensure its future reproducibility.

### Secondary Objectives
-   **📈 Management:** Carry out structured project management and analyze the feasibility of future improvements.
-   **📦 Design:** Improve the structural design to favor potential commercial adoption.

***

## 3. 🏗️ Project Outline

The platform consists of three load cells ($s_{i}$) placed at strategic points on a base. Each cell is connected to an HX711 module, which amplifies and digitizes the signal. An --------- microcontroller reads these values, applies filters, and calculates the Center of Pressure (CoP) to estimate the object's position. Finally, the data is sent to a web server that presents it on a visual interface, showing the trajectory and detected gestures.


*Figure 1: Project Outline*

***

## 4. 🛠️ Component Identification

#### 4.1. Hardware
The main hardware includes **three load cells**, which convert force into an electrical signal. Each is connected to an **HX711 module**, a 24-bit analog-to-digital converter. The brain of the system is an **----------** microcontroller or similar.

#### 4.2. Power Supply
-   The load cells and HX711 modules are powered by a regulated 5V supply (max consumption 50mA).
-   The ESP32 microcontroller is powered by 3.3V.
-   An external **5V power supply with a capacity of at least 2A** is recommended to prevent voltage drops.

#### 4.3. Software
The firmware will be responsible for reading the signals from the HX711 modules, applying filtering, and calculating the CoP to estimate position. It will also identify gestures such as dragging (continuous tracking of the CoP) and pressing (a sharp increase in total force). A web server implemented in HTML will receive this data to display it on a real-time graphical interface.

#### 4.4. Web System
The web interface will present the object's position on a 2D plane, indicating the magnitude of the force with a visual indicator (e.g., change in color or size). It will also include functionalities to calibrate the cells and adjust parameters like sensitivity.

***

## 5. 📅 Timeline & Milestones

#### 5.1. October Progress Report
For this delivery, the design and construction of the physical platform are planned to be completed. A single load cell scale will be implemented and calibrated to verify accuracy. Additionally, the microcontroller will be able to read an HX711 and send the data to a local server.

#### 5.2. November Progress Report
In November, the installation of the three load cells will be completed, and the triangulation algorithm to calculate the 2D position will be implemented. The logic for detecting drag and press gestures will be developed. The web interface will be reactive, and all modules will be integrated for comprehensive testing.

***

## 6. 🎥 Video Documentation

-   **Video Link:** [)

***

## 7. ✅ Conclusion

This project offers a valuable opportunity to explore low-cost tactile interfaces, combining hardware and software for gesture detection. Although its scope is more limited than the SCALE project, it will serve to strengthen our technical skills and demonstrate our capabilities to future employers.

***

## 📚 References

1.  **Media Lab, MIT.** (n.d.). *SCALE*. Retrieved from https://tangible.media.mit.edu/project/scale/
2.  **Dwyer Omega.** (n.d.). *What is a Load Cell?*. Retrieved from https://www.dwyeromega.com/en-us/resources/load-cells
3.  **SparkFun Electronics.** (2017). *HX711 Datasheet*. Retrieved from https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf
4.  **SparkFun Electronics.** (2017). *Load cell amplifier HX711 breakout hookup guide*. Retrieved from https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide

