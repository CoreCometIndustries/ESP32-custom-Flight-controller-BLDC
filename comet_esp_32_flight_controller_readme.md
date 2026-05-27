# Comet ESP32 Flight Controller

An experimental DIY quadcopter flight controller built using an ESP32 and MPU6050 IMU sensor.

This project was developed as a custom embedded flight control system focused on learning UAV stabilization, sensor fusion, and PID-based flight control.

---

## ⚠️ Warning / Disclaimer

This project is **experimental** and is **NOT** as stable, reliable, or fully tested as professional flight controller firmware such as:

- INAV
- Betaflight
- ArduPilot
- PX4

Use this code completely at your own risk.

The developer is **NOT responsible** for:

- Drone crashes
- Flyaways
- Hardware damage
- Property damage
- Injuries or accidents
- Any loss caused by using this project

This software may contain:

- Bugs
- Unstable behavior
- PID tuning issues
- Sensor drift
- Unexpected flight failures

### Always:

- Test without propellers first
- Perform safe ground testing
- Fly in open areas only
- Keep away from people and animals
- Use a properly calibrated RC transmitter and ESCs

By using or modifying this code, you accept full responsibility for all risks involved.

---

# 🚀 Features

- ESP32-based flight controller
- MPU6050 IMU integration
- PID stabilization system
- Roll, Pitch, and Yaw rate control
- Complementary filter for angle estimation
- PWM ESC output control
- RC receiver input using interrupts
- Adjustable PID parameters
- 500Hz ESC refresh rate
- Lightweight custom firmware

---

# 🛠 Hardware Used

| Component | Description |
|---|---|
| ESP32 | Main flight controller |
| MPU6050 | Gyroscope + Accelerometer |
| ESCs | Brushless motor controllers |
| Brushless Motors | Quadcopter propulsion |
| RC Receiver | PWM input channels |
| LiPo Battery | Power source |

---

# 📌 Pin Configuration

## Motor Output Pins

| Motor | GPIO |
|---|---|
| Motor 1 | GPIO 13 |
| Motor 2 | GPIO 12 |
| Motor 3 | GPIO 14 |
| Motor 4 | GPIO 27 |

## Receiver Input Pins

| Channel | GPIO |
|---|---|
| CH1 | GPIO 34 |
| CH2 | GPIO 35 |
| CH3 | GPIO 32 |
| CH4 | GPIO 33 |
| CH5 | GPIO 25 |
| CH6 | GPIO 26 |

---

# 📡 Sensor Used

## MPU6050

Communication: I2C

Default Address:

```cpp
0x68
```

Used for:

- Gyroscope readings
- Accelerometer readings
- Angle estimation
- Stabilization feedback

---

# 🧠 Control System

The firmware uses:

- PID stabilization loops
- Complementary filter sensor fusion
- Real-time gyro + accelerometer processing

### PID Loops Included

- Roll PID
- Pitch PID
- Yaw PID
- Angle stabilization PID

---

# ⚙️ ESC Configuration

```cpp
ESC Frequency = 500Hz
```

PWM Range:

```cpp
1000us - 2000us
```

---

# 🔧 Libraries Required

Install the following Arduino libraries:

## Required Libraries

- ESP32Servo
- Wire

---

# 📥 Installation

## 1. Install Arduino IDE

Download:

https://www.arduino.cc/en/software

---

## 2. Install ESP32 Board Package

Add this URL in Arduino IDE:

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Then install:

```text
ESP32 by Espressif Systems
```

---

## 3. Install Required Libraries

Using Library Manager install:

- ESP32Servo

---

## 4. Upload Code

- Select your ESP32 board
- Select COM port
- Upload the `.ino` file

---

# 📊 Current Firmware Features

✅ Basic quadcopter stabilization

✅ Receiver PWM reading

✅ Sensor fusion

✅ PID motor mixing

✅ Complementary filter angle estimation

---

# ⚠️ Important Notes

This project is still under development.

It does NOT currently include:

- GPS hold
- Altitude hold
- Return to home
- Failsafe protection
- Advanced filtering
- Auto tuning
- Position hold
- Optical flow
- Mission planning

This is primarily a:

- Learning project
- Experimental flight controller
- Research platform

---

# 🧪 Recommended Testing Procedure

## Before Flight

- Verify motor directions
- Check propeller orientation
- Calibrate IMU
- Verify receiver mapping
- Tune PID values carefully
- Remove propellers during initial tests

## First Flight

- Use low altitude
- Fly in open space
- Keep throttle low
- Be prepared for instability

---

# 📈 Future Improvements

Planned upgrades may include:

- Kalman filter
- Altitude hold
- GPS navigation
- ESP-NOW telemetry
- OLED telemetry display
- Web tuning interface
- Blackbox logging
- OTA updates
- Advanced failsafe system
- Autonomous flight modes

---

# 👨‍💻 Developer

Bhushan Patil

Electronics & Telecommunication Engineering

Embedded Systems | UAVs | Robotics | AI | Space-Tech

---

# 📜 License

This project is open-source and intended for educational and research purposes.

Use responsibly.

---

# ⭐ Support

If you like this project:

- Star the repository
- Share your improvements
- Contribute new features
- Help improve flight stability

---

# ✈️ Fly Safe

Always prioritize safety while testing UAV systems.

