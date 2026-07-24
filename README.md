# 🛸 Comet ESP32 Flight Controller

An experimental, open-source quadcopter flight controller built on the **ESP32** microcontroller. Features dual-loop PID stabilisation, FlySky IBUS receiver support, automatic sensor detection, a modular C++ architecture, and a serial command interface.

> **Author:** Bhushan Patil  
> **Field:** Electronics & Telecommunication Engineering — Embedded Systems · UAVs · Robotics · AI · Space-Tech

---

## ⚠️ Warning / Disclaimer

This is an **experimental DIY project** and is **NOT** as stable, reliable, or fully tested as professional flight controller firmware such as INAV, Betaflight, ArduPilot, or PX4.

**Use this code completely at your own risk.**

The developer is **NOT responsible** for:
- Drone crashes or flyaways
- Hardware or property damage
- Injuries, accidents, or any loss caused by using this project

This software may contain bugs, unstable behaviour, PID tuning issues, sensor drift, or unexpected flight failures.

### Always:
- ✅ Test **without propellers** first
- ✅ Perform safe ground testing
- ✅ Fly in **open areas** only
- ✅ Keep away from people and animals
- ✅ Use a properly calibrated RC transmitter and ESCs

**By using or modifying this code, you accept full responsibility for all risks involved.**

---

## 🚀 Features

| Category | Details |
|---|---|
| **MCU** | ESP32 (dual-core, 240 MHz) |
| **Loop Rate** | 250 Hz (4 ms fixed-step with overrun resync guard) |
| **Architecture** | Clean modular C++ layout (`Config`, `Storage`, `Sensors`, `Receiver`, `PID`, `Motors`, `CLI`) |
| **IMU Support** | MPU6050 · MPU6500 · MPU9250 · ICM20689 (auto-detected with I2C data validation) |
| **Barometer** | BMP280 · BME280 · BMP388 (auto-detected, optional) |
| **Magnetometer** | HMC5883L · QMC5883L (auto-detected, optional) |
| **RC Protocol** | FlySky IBUS (14 channels via Serial2 at 115200 baud) |
| **ESC Output** | 4× PWM at configurable frequency (default 500 Hz, 1000–2000 µs) |
| **Stabilisation** | Cascaded Angle → Rate PID (Roll, Pitch) + Rate PID (Yaw) with smooth Derivative-on-Measurement |
| **Angle Estimation** | Complementary filter (gyro + accelerometer fusion) |
| **Configuration** | Persistent NVS/EEPROM storage for PID, calibration, and ESC settings |
| **Interface** | Full Serial CLI (`HELP`, `DUMP`, `DUMP_JSON`, `SET`, `SAVE`, `RESET`) |
| **Safety** | Power-on arming guard · Zero-throttle AirMode spin · Failsafe (500 ms timeout) · I2C read validation |

---

## 📁 Project Structure

```
ESP Flight Controller/
├── README.md                              ← project documentation
└── src/
    ├── Comet_ESP32_flightcontroller/
    │   ├── Comet_ESP32_flightcontroller.ino   ← main sketch entry point & 250 Hz loop
    │   ├── Config.h / Config.cpp             ← constants, pin definitions & global state
    │   ├── Storage.h / Storage.cpp           ← EEPROM / NVS preferences storage
    │   ├── Sensors.h / Sensors.cpp           ← I2C sensor detection, IMU reads & attitude filter
    │   ├── Receiver.h / Receiver.cpp         ← IBUS parser, failsafe & arming safety guard
    │   ├── PID.h / PID.cpp                   # Angle & Rate PID controllers with smooth D-term
    │   ├── Motors.h / Motors.cpp             ← ESC frequency, X-quad mixing & motor outputs
    │   └── CLI.h / CLI.cpp                   ← Serial command interpreter, LED status & logging
    └── Gyro_accelerometer_calibration/
        └── Gyro_accelerometer_calibration.ino ← standalone IMU calibration tool
```

---

## 🛠 Hardware Requirements

| Component | Description |
|---|---|
| **ESP32 DevKit** | Main flight controller (any ESP32 with enough GPIOs) |
| **IMU Sensor** | MPU6050, MPU6500, MPU9250, or ICM20689 (I²C) |
| **ESCs** | 4× Brushless ESC (supporting 500 Hz PWM, 1000–2000 µs) |
| **Motors** | 4× Brushless DC motors |
| **RC Receiver** | FlySky IBUS-compatible (e.g. FS-iA6B, FS-iA10B) |
| **Battery** | LiPo (3S or 4S depending on motors/ESCs) |
| **Frame** | Any X-configuration quadcopter frame |
| **Barometer** *(optional)* | BMP280, BME280, or BMP388 |
| **Magnetometer** *(optional)* | HMC5883L or QMC5883L |

---

## 📌 Pin Configuration

### Motor Output Pins

| Motor | Position | GPIO | Rotation |
|---|---|---|---|
| Motor 1 | Front Right | GPIO 13 | CCW |
| Motor 2 | Rear Right | GPIO 12 | CW |
| Motor 3 | Rear Left | GPIO 14 | CCW |
| Motor 4 | Front Left | GPIO 27 | CW |

### System Pins

| Function | GPIO | Notes |
|---|---|---|
| IBUS RX | GPIO 16 | Serial2 RX (connect to receiver IBUS out) |
| I²C SDA | GPIO 21 | Default ESP32 I²C (to IMU/Baro/Mag) |
| I²C SCL | GPIO 22 | Default ESP32 I²C (to IMU/Baro/Mag) |
| Status LED | GPIO 15 | Arming/failsafe indicator |

### Motor Layout (X-Configuration, top view)

```
    Front
  4 (CW)   1 (CCW)
     \     /
      \   /
       \ /
        X
       / \
      /   \
     /     \
  3 (CCW)  2 (CW)
     Rear
```

---

## 📡 RC Channel Mapping & Safety Guards

| Channel | Function | Range | Center |
|---|---|---|---|
| CH1 (`rcChannels[0]`) | Roll | 1000–2000 | 1500 |
| CH2 (`rcChannels[1]`) | Pitch | 1000–2000 | 1500 |
| CH3 (`rcChannels[2]`) | Throttle | 1000–2000 | — (non-self-centering) |
| CH4 (`rcChannels[3]`) | Yaw | 1000–2000 | 1500 |
| CH5 (`rcChannels[4]`) | AUX1 (Arm) | < 1300 = Disarm · > 1700 = Arm | — |
| CH6 (`rcChannels[5]`) | AUX2 | Available for future use | — |

### Safety Guard Mechanisms
- **Power-On Arming Guard**: If the quad is powered up with AUX1 switch HIGH (>1700), arming is blocked until the pilot toggles AUX1 to DISARMED (<1300) at least once.
- **Arming Conditions**: AUX1 > 1700 **AND** Throttle < 1050 **AND** Arming guard cleared.
- **In-Flight Zero Throttle (AirMode)**: Dropping throttle to zero while **ARMED** keeps motors spinning at `ThrottleIdle` (1170 µs) and preserves attitude stabilization. Setting motors to `ThrottleCutOff` (1000 µs) and resetting PID state occurs **only when DISARMED**.
- **Receiver Failsafe**: If no valid IBUS frame is received for **500 ms**, failsafe triggers: motors cut to `ThrottleCutOff` and the FC disarms.
- **I2C Data Validation**: Verifies 14-byte packet arrival before parsing sensor data to prevent corrupted `-1` byte readings from I2C bus noise or disconnections.

---

## 🧠 Control System

### Architecture

```
RC Sticks ──► Angle PID ──► Desired Rate ──► Rate PID ──► Motor Mixing ──► ESCs
                 ▲                               ▲
                 │                               │
          Complementary                    Gyroscope
            Filter                         (deg/s)
          (Acc + Gyro)
```

### Angle PID (Outer Loop)
Computes target rotation **rate** from the error between the desired angle (from RC sticks, ±50°) and estimated angle from complementary filter.

### Rate PID (Inner Loop)
Computes motor stabilization correction using **Derivative-on-Measurement** (`-D * d(GyroRate)/dt`). This smooths D-term response and eliminates derivative voltage spikes during quick stick movements.

### Motor Mixing (X-Quad)

```
Motor1 (FR) = Throttle − Roll − Pitch − Yaw
Motor2 (RR) = Throttle − Roll + Pitch + Yaw
Motor3 (RL) = Throttle + Roll + Pitch − Yaw
Motor4 (FL) = Throttle + Roll − Pitch + Yaw
```

All motor outputs are clamped between `ThrottleIdle` (default 1170 µs) and 1999 µs when armed.

---

## ⚙️ Default Configuration Values

### Rate PID

| Axis | P | I | D |
|---|---|---|---|
| Roll | 0.625 | 2.1 | 0.0088 |
| Pitch | 0.625 | 2.1 | 0.0088 |
| Yaw | 4.0 | 3.0 | 0.0 |

### Angle PID

| Axis | P | I | D |
|---|---|---|---|
| Roll | 2.0 | 0.5 | 0.007 |
| Pitch | 2.0 | 0.5 | 0.007 |

### System Config

| Parameter | Default | Description |
|---|---|---|
| `escFreq` | 500 Hz | ESC PWM frequency |
| `thrIdle` | 1170 µs | Minimum armed motor speed |
| `thrCut` | 1000 µs | Motor off / disarmed value |

All values are stored in **ESP32 NVS (Non-Volatile Storage)** and persist across reboots.

---

## 💻 Serial Command Interface

Connect via Serial Monitor at **115200 baud**. Available commands:

| Command | Description |
|---|---|
| `HELP` | Show all available commands |
| `DUMP` | Print all current config values (human-readable) |
| `DUMP_JSON` | Print all config as JSON |
| `SAVE` | Persist current values to NVS/EEPROM |
| `RESET` | Factory reset — clear NVS and reload defaults |
| `SET <param> <value>` | Change a parameter at runtime |

### Settable Parameters

- **Rate PID:** `PRateRoll`, `IRateRoll`, `DRateRoll`, `PRatePitch`, `IRatePitch`, `DRatePitch`, `PRateYaw`, `IRateYaw`, `DRateYaw`
- **Angle PID:** `PAngleRoll`, `IAngleRoll`, `DAngleRoll`, `PAnglePitch`, `IAnglePitch`, `DAnglePitch`
- **Calibration:** `GyroCalR`, `GyroCalP`, `GyroCalY`, `AccCalX`, `AccCalY`, `AccCalZ`
- **System Config:** `escFreq`, `thrIdle`, `thrCut`

### Example CLI Session

```
SET PRateRoll 0.75
SET IRateRoll 2.5
SET thrIdle 1180
SAVE
DUMP
```

> **Note:** `SET` modifies values in RAM instantly. Use `SAVE` to persist to NVS storage across reboots.

---

## 🔧 Calibration Tool

A standalone calibration sketch is provided at `src/Gyro_accelerometer_calibration/Gyro_accelerometer_calibration.ino`.

### What It Does
1. **Gyroscope calibration** — averages 2000 samples to compute zero-rate offsets for Roll, Pitch, and Yaw
2. **Accelerometer calibration** — averages 2000 samples to compute offset for X, Y, Z axes (Z is normalised to 1g)
3. Outputs calibration values in copy-paste format

### How to Use
1. Upload the calibration sketch to your ESP32
2. Place the quadcopter **flat on a level surface**
3. Open Serial Monitor (115200 baud)
4. Press **Space** then **Enter** to start calibration
5. Copy the printed calibration values
6. Enter them into the flight controller via `SET` commands, then `SAVE`

---

## 📥 Installation & Upload

### Prerequisites
1. **Arduino IDE** — [Download](https://www.arduino.cc/en/software)
2. **ESP32 Board Package** — Add this URL in *File → Preferences → Additional Board Manager URLs*:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
   Then install **"ESP32 by Espressif Systems"** from Board Manager.

### Required Libraries
Install via *Sketch → Include Library → Manage Libraries*:

| Library | Purpose |
|---|---|
| **ESP32Servo** | PWM servo/ESC output on ESP32 |
| **Wire** | I²C communication (built-in) |
| **Preferences** | NVS storage (built-in) |

### Upload Steps
1. Open `Comet_ESP32_flightcontroller.ino` in Arduino IDE (all header/source files will open as tabs automatically)
2. Select your ESP32 board (*Tools → Board → ESP32 Dev Module*)
3. Select the correct COM port (*Tools → Port*)
4. Click **Upload**
5. Open Serial Monitor at 115200 baud to verify `=== Comet FC Ready ===`

---

## 🧪 Testing Procedure

### Phase 1: Bench Testing (No Propellers!)
1. Upload firmware and verify serial output
2. Check sensor detection messages (`IMU: MPU6050 at 0x68`, etc.)
3. Verify RC channel values in serial log (`RC:1500 1500 1000 1500 ...`)
4. Test arming/disarming with AUX1 switch
5. Observe motor values change with throttle while armed

### Phase 2: Motor Testing (No Propellers!)
1. Arm the drone with throttle at minimum
2. Slowly increase throttle — all 4 motors should spin up
3. Tilt the drone and observe PID corrections in motor values
4. Verify motor directions match the X-quad layout

### Phase 3: First Flight
- ⚠️ Attach propellers only after bench testing is complete
- Use a **low altitude** hover in an **open space**
- Keep throttle low and be ready to disarm (AUX1 switch)
- Tune PID values incrementally — start with Rate P, then Rate D, then Rate I

---

## 🔍 LED Status Indicators

| LED Behaviour | Meaning |
|---|---|
| **Solid ON** | Armed — motors active |
| **Slow blink** (500 ms) | Disarmed — ready, receiver connected |
| **Rapid blink** (100 ms) | Failsafe — no receiver signal |
| **5 rapid blinks** on boot | Boot sequence / initialisation |

---

## 📊 Serial Log Format

At ~5 Hz, the firmware prints a status line:

```
RC:1500 1500 1050 1500 1800 1000 M:1170 1170 1170 1170 A:0.03 0.01 0.93 G:0.1 -0.2 0.0 ARMED
```

| Field | Meaning |
|---|---|
| `RC:` | CH1–CH6 raw values |
| `M:` | Motor 1–4 output (µs) |
| `A:` | Accelerometer X, Y, Z (g) |
| `G:` | Gyroscope Roll, Pitch, Yaw (°/s) |
| Status | `ARMED`, `DISARMED`, or `FAILSAFE` |

---

## 📜 License

This project is open-source and intended for **educational and research purposes**.

Use responsibly.

---

## ✈️ Fly Safe!

Always prioritise safety while testing UAV systems. **Never fly over people, animals, or property you can't afford to damage.** Start small, test thoroughly, and iterate carefully.
