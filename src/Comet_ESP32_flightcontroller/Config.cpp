#include "Config.h"

// ======== Sensor Auto-Detect State ========
uint8_t imuAddr  = 0;
uint8_t imuType  = 0;
uint8_t baroAddr = 0;
uint8_t baroType = 0;
uint8_t magAddr  = 0;
uint8_t magType  = 0;

// ======== PID Gains ========
float PAngleRoll  = 2.0f;    float PAnglePitch = 2.0f;
float IAngleRoll  = 0.5f;    float IAnglePitch = 0.5f;
float DAngleRoll  = 0.007f;  float DAnglePitch = 0.007f;

float PRateRoll  = 0.625f;   float IRateRoll  = 2.1f;   float DRateRoll  = 0.0088f;
float PRatePitch = 0.625f;   float IRatePitch = 2.1f;   float DRatePitch = 0.0088f;
float PRateYaw   = 4.0f;     float IRateYaw   = 3.0f;   float DRateYaw   = 0.0f;

// ======== Calibration Offsets ========
float RateCalibrationRoll  =  0.27f;
float RateCalibrationPitch = -0.85f;
float RateCalibrationYaw   = -2.09f;
float AccXCalibration      =  0.03f;
float AccYCalibration      =  0.01f;
float AccZCalibration      = -0.07f;

// ======== Config ========
int ESCfreq        = 500;
int ThrottleIdle   = 1170;
int ThrottleCutOff = 1000;

// ======== IMU / Angle State ========
float RateRoll = 0.0f, RatePitch = 0.0f, RateYaw = 0.0f;
float AccX = 0.0f, AccY = 0.0f, AccZ = 0.0f;
float AngleRoll = 0.0f, AnglePitch = 0.0f;
float complementaryAngleRoll  = 0.0f;
float complementaryAnglePitch = 0.0f;

// ======== PID Working Variables ========
float PtermRoll = 0.0f,  ItermRoll = 0.0f,  DtermRoll = 0.0f,  PIDOutputRoll = 0.0f;
float PtermPitch = 0.0f, ItermPitch = 0.0f, DtermPitch = 0.0f, PIDOutputPitch = 0.0f;
float PtermYaw = 0.0f,   ItermYaw = 0.0f,   DtermYaw = 0.0f,   PIDOutputYaw = 0.0f;

float DesiredRateRoll = 0.0f, DesiredRatePitch = 0.0f, DesiredRateYaw = 0.0f;
float ErrorRateRoll = 0.0f,   ErrorRatePitch = 0.0f,   ErrorRateYaw = 0.0f;
float InputRoll = 0.0f, InputThrottle = 1000.0f, InputPitch = 0.0f, InputYaw = 0.0f;

float PrevErrorRateRoll  = 0.0f, PrevErrorRatePitch  = 0.0f, PrevErrorRateYaw  = 0.0f;
float PrevItermRateRoll  = 0.0f, PrevItermRatePitch  = 0.0f, PrevItermRateYaw  = 0.0f;
float PrevRateRoll       = 0.0f, PrevRatePitch       = 0.0f, PrevRateYaw       = 0.0f;

float DesiredAngleRoll = 0.0f, DesiredAnglePitch = 0.0f;
float ErrorAngleRoll = 0.0f,   ErrorAnglePitch = 0.0f;
float PrevErrorAngleRoll = 0.0f, PrevErrorAnglePitch = 0.0f;
float PrevItermAngleRoll = 0.0f, PrevItermAnglePitch = 0.0f;

// ======== Motor Outputs & Servo Objects ========
float MotorInput1 = 1000.0f, MotorInput2 = 1000.0f, MotorInput3 = 1000.0f, MotorInput4 = 1000.0f;
Servo mot1, mot2, mot3, mot4;

// ======== IBUS State ========
uint16_t rcChannels[IBUS_NUM_CHANNELS];
uint8_t  ibusBuffer[IBUS_FRAME_SIZE];
uint8_t  ibusIndex       = 0;
uint32_t lastIbusFrameTime = 0;
uint32_t lastIbusByteTime  = 0;

// ======== Arming & Failsafe ========
bool armed              = false;
bool failsafe           = true;   // start in failsafe until first valid IBUS frame
bool armPreventedOnBoot = true;   // arming guard: require switch LOW once after boot

// ======== Loop Timing ========
uint32_t LoopTimer = 0;

// ======== EEPROM Preferences ========
Preferences prefs;

// ======== Serial Command Buffer ========
char    cmdBuffer[64];
uint8_t cmdIndex = 0;
