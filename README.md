# Automatic-Garbage-Collector-with-Live-Image-Detection-using-ESP32-and-Laptop
The Automatic Garbage Collector is a robotic system using an ESP32 microcontroller for trash detection and collection. It features AI-based object detection, a robotic arm, environmental sensors, and remote control via the Blynk app. Developed by Misbah, Rakib, Mahathir, Noman, and Kazi Neyamul Hasan in 4 months.
# Automatic Garbage Collector

The **Automatic Garbage Collector** is a smart robotic system designed to detect and collect trash autonomously. It integrates AI-based object detection, environmental monitoring, and remote control capabilities. The system uses an ESP32 microcontroller for control, a laptop webcam for AI-based object detection, and various sensors for navigation and monitoring.

## Project Images

- **Give in repositorie file**

---

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Components](#components)
4. [Circuit Diagram](#circuit-diagram)
5. [Installation](#installation)
6. [Usage](#usage)
7. [Future Work](#future-work)
8. [Contributing](#contributing)
9. [License](#license)

---

## Introduction

The **Automatic Garbage Collector** project is designed to help with waste management by detecting trash, collecting it, and disposing of it autonomously. The system combines a laptop running object detection algorithms with an ESP32 microcontroller that controls the movement and operation of the robotic arm and other components.

This project was developed by a team of four members: **Misbah**, **Rakib**, **Abir**, and **Noman**, with a project duration of 4 months. Most of the time was spent on developing the hardware part.

---

## Visual Representation

Here are some screenshots showcasing the Project Kairos interface.

### Index Page

![Shot 1](https://github.com/neyamulhasan/Automatic-Garbage-Collector-with-Live-Image-Detection-using-ESP32/blob/main/20241007_125039~2.jpg)
![Shot 2](https://github.com/neyamulhasan/Automatic-Garbage-Collector-with-Live-Image-Detection-using-ESP32/blob/main/PXL_20241007_064821503.NIGHT.jpg)
![Shot 3](https://github.com/neyamulhasan/Automatic-Garbage-Collector-with-Live-Image-Detection-using-ESP32/blob/main/PXL_20241007_064722111.NIGHT.jpg)
![Shot 4](https://github.com/neyamulhasan/Automatic-Garbage-Collector-with-Live-Image-Detection-using-ESP32/blob/main/PXL_20241007_064713685.NIGHT.jpg)
![Shot 5](https://github.com/neyamulhasan/Automatic-Garbage-Collector-with-Live-Image-Detection-using-ESP32/blob/main/PXL_20241008_152057928.NIGHT~2.jpg)


## Features

- **AI-based Object Detection**: Uses a laptop's webcam and YOLO algorithm to detect trash.
- **Autonomous Movement**: Uses an ESP32 microcontroller to navigate towards detected objects.
- **Trash Collection**: Equipped with a robotic arm to pick up trash and drop it in a designated bin.
- **Environmental Monitoring**: Monitors temperature, humidity, and smoke levels using sensors.
- **Remote Control via Blynk**: Control and monitor the system through the Blynk app.
- **Failsafe Mechanism**: Stops the robot if communication is lost or errors are detected.

The system can detect trash like bottles, tissues, papers, and more. Note that the trash picture dataset is not fully included in this repository. If you want to use this code for your project, you can build your own dataset based on this code. Feel free to update and use this code.

---

## Components

1. **ESP32 Microcontroller**: Controls the motors, sensors, and communication with Blynk.
2. **Laptop**: Used for AI-based object detection using a webcam.
3. **Webcam**: Captures live video feed for object detection.
4. **DC Motors**: Used for the movement of the robot.
5. **Motor Driver (L298N)**: Controls the DC motors.
6. **Robotic Arm with Servo Motors**: Picks up detected objects.
7. **DHT11 Sensor**: Measures temperature and humidity.
8. **Smoke Sensor**: Detects the presence of smoke.
9. **Ultrasonic Sensors**: Detects obstacles and measures distance.
10. **Power Supply**: Provides power to the motors and sensors.

---

## Circuit Diagram

*(Include a circuit diagram showing connections between the ESP32, motor driver, sensors, laptop, and other components.)*

---

## Installation

### Prerequisites

1. Install **Arduino IDE** or **PlatformIO** for programming the ESP32.
2. Set up **Python** on your laptop.
3. Install **YOLOv8** or a similar object detection framework.
4. Set up the **Blynk app** on your mobile device.

### Steps

1. **ESP32 Setup**:
   - Connect the ESP32 to your computer and upload the code using the Arduino IDE.
   - Configure the ESP32 for Wi-Fi and Blynk communication.

2. **Laptop Setup**:
   - Set up the Python environment with required libraries (e.g., `opencv`, `ultralytics`).
   - Install YOLOv8 or your preferred object detection framework.

3. **Connecting Components**:
   - Follow the circuit diagram to connect the motors, sensors, and other components to the ESP32.
   - Connect the laptop to the ESP32 via serial communication.

4. **Blynk Setup**:
   - Set up a project on the Blynk app with buttons to start/stop the robot and display sensor readings.
   - Update the Blynk authentication token in the ESP32 code.

---

## Usage

1. Power on the system and ensure that the ESP32 is connected to Wi-Fi.
2. Use the Blynk app to start the garbage collection process.
3. The robot will autonomously detect objects using the webcam and move towards them.
4. The robotic arm will pick up detected trash and place it in the bin.
5. Monitor environmental data (temperature, humidity, smoke) through the Blynk app.

---

## Future Work

- **Enhanced AI Model**: Improve object detection accuracy by training a custom YOLO model.
- **Advanced Path Planning**: Integrate A* or Dijkstra's algorithm for better navigation.
- **Metal Detection**: Add a metal detection feature to separate metallic waste.
- **Voice Commands**: Control the robot using voice recognition.
- **GPS Tracking**: Incorporate GPS for outdoor navigation and geo-fencing.
- **Battery Management System**: Implement battery status monitoring and low-power warnings.
- **Additional Sensors**: Add more sensors for better obstacle avoidance and functionality.

---

## Contributing

Contributions are welcome! Please fork this repository and submit a pull request with your changes.

### How to Contribute

1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature-name
   ```
3. Make your changes and commit them:
   ```bash
   git commit -m "Add new feature"
   ```
4. Push to the branch:
   ```bash
   git push origin feature-name
   ```
5. Open a pull request.

---

## License

**GNU GENERAL PUBLIC LICENSE**. See the [`LICENSE`](./LICENSE) file for details.


---
