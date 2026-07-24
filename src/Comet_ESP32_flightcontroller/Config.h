#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <Preferences.h>

// ======== Constants ========
constexpr uint32_t LOOP_PERIOD_US = 4000;       // 4ms = 250 Hz
constexpr float    dt             = 0.004f;      // loop period in seconds
constexpr float    RAD_TO_DEG_F   = 57.2957795f;
constexpr uint32_t RX_TIMEOUT_US  = 500000;      // 500ms receiver failsafe
constexpr uint16_t LOG_DIVIDER    = 50;          // log every 50 loops ≈ 5 Hz
constexpr uint8_t  LED_PIN        = 15;

// ======== IBUS Configuration ========
#define IBUS_RX_PIN       16
#define IBUS_FRAME_SIZE   32
#define IBUS_NUM_CHANNELS 14
#define IBUS_HEADER1      0x20
#define IBUS_HEADER2      0x40

// ======== Motor Pins ========
const int mot1_pin = 13;
const int mot2_pin = 12;
const int mot3_pin = 14;
const int mot4_pin = 27;

// ======== Sensor Auto-Detect State ========
extern uint8_t imuAddr;
extern uint8_t imuType;
extern uint8_t baroAddr;
extern uint8_t baroType;
extern uint8_t magAddr;
extern uint8_t magType;

// ======== PID Gains ========
extern float PAngleRoll,  PAnglePitch;
extern float IAngleRoll,  IAnglePitch;
extern float DAngleRoll,  DAnglePitch;

extern float PRateRoll,   IRateRoll,   DRateRoll;
extern float PRatePitch,  IRatePitch,  DRatePitch;
extern float PRateYaw,    IRateYaw,    DRateYaw;

// ======== Calibration Offsets ========
extern float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
extern float AccXCalibration,      AccYCalibration,      AccZCalibration;

// ======== Config ========
extern int ESCfreq;
extern int ThrottleIdle;
extern int ThrottleCutOff;

// ======== IMU / Angle State ========
extern float RateRoll, RatePitch, RateYaw;
extern float AccX, AccY, AccZ;
extern float AngleRoll, AnglePitch;
extern float complementaryAngleRoll;
extern float complementaryAnglePitch;

// ======== PID Working Variables ========
extern float PtermRoll,  ItermRoll,  DtermRoll,  PIDOutputRoll;
extern float PtermPitch, ItermPitch, DtermPitch, PIDOutputPitch;
extern float PtermYaw,   ItermYaw,   DtermYaw,   PIDOutputYaw;

extern float DesiredRateRoll, DesiredRatePitch, DesiredRateYaw;
extern float ErrorRateRoll,   ErrorRatePitch,   ErrorRateYaw;
extern float InputRoll, InputThrottle, InputPitch, InputYaw;

extern float PrevErrorRateRoll,  PrevErrorRatePitch,  PrevErrorRateYaw;
extern float PrevItermRateRoll,  PrevItermRatePitch,  PrevItermRateYaw;
extern float PrevRateRoll,       PrevRatePitch,       PrevRateYaw;

extern float DesiredAngleRoll, DesiredAnglePitch;
extern float ErrorAngleRoll,   ErrorAnglePitch;
extern float PrevErrorAngleRoll, PrevErrorAnglePitch;
extern float PrevItermAngleRoll, PrevItermAnglePitch;

// ======== Motor Outputs & Servo Objects ========
extern float MotorInput1, MotorInput2, MotorInput3, MotorInput4;
extern Servo mot1, mot2, mot3, mot4;

// ======== IBUS State ========
extern uint16_t rcChannels[IBUS_NUM_CHANNELS];
extern uint8_t  ibusBuffer[IBUS_FRAME_SIZE];
extern uint8_t  ibusIndex;
extern uint32_t lastIbusFrameTime;
extern uint32_t lastIbusByteTime;

// ======== Arming & Failsafe ========
extern bool armed;
extern bool failsafe;
extern bool armPreventedOnBoot;

// ======== Loop Timing ========
extern uint32_t LoopTimer;

// ======== EEPROM Preferences ========
extern Preferences prefs;

// ======== Serial Command Buffer ========
extern char    cmdBuffer[64];
extern uint8_t cmdIndex;

#endif // CONFIG_H
