#include "Config.h"
#include "Sensors.h"

// --- IMU (MPU6050 / MPU6500 / ICM20689 / MPU9250) ---
void detectIMU() {
  const uint8_t addrs[] = { 0x68, 0x69 };
  for (uint8_t i = 0; i < 2; i++) {
    Wire.beginTransmission(addrs[i]);
    if (Wire.endTransmission() == 0) {
      // Read WHO_AM_I register (0x75)
      Wire.beginTransmission(addrs[i]);
      Wire.write(0x75);
      Wire.endTransmission(false);
      Wire.requestFrom(addrs[i], (uint8_t)1);
      if (Wire.available()) {
        imuType = Wire.read();
        imuAddr = addrs[i];
        const char* name = "Unknown";
        if      (imuType == 0x68) name = "MPU6050";
        else if (imuType == 0x70) name = "MPU6500";
        else if (imuType == 0x98) name = "ICM20689";
        else if (imuType == 0x71) name = "MPU9250";
        Serial.printf("IMU : %s at 0x%02X (WHO_AM_I=0x%02X)\n", name, addrs[i], imuType);
        return;
      }
    }
  }
  Serial.println("ERROR: No IMU detected on I2C bus!");
}

// --- Barometer (BMP280 / BME280 / BMP388) ---
void detectBaro() {
  const uint8_t addrs[] = { 0x76, 0x77 };
  for (uint8_t i = 0; i < 2; i++) {
    Wire.beginTransmission(addrs[i]);
    if (Wire.endTransmission() == 0) {
      // Try BMP280 / BME280 chip-ID at register 0xD0
      Wire.beginTransmission(addrs[i]);
      Wire.write(0xD0);
      Wire.endTransmission(false);
      Wire.requestFrom(addrs[i], (uint8_t)1);
      if (Wire.available()) {
        uint8_t id = Wire.read();
        if (id == 0x58) { baroAddr = addrs[i]; baroType = id; Serial.printf("Baro: BMP280 at 0x%02X\n", addrs[i]); return; }
        if (id == 0x60) { baroAddr = addrs[i]; baroType = id; Serial.printf("Baro: BME280 at 0x%02X\n", addrs[i]); return; }
      }
      // Try BMP388 chip-ID at register 0x00
      Wire.beginTransmission(addrs[i]);
      Wire.write(0x00);
      Wire.endTransmission(false);
      Wire.requestFrom(addrs[i], (uint8_t)1);
      if (Wire.available()) {
        uint8_t id = Wire.read();
        if (id == 0x50) { baroAddr = addrs[i]; baroType = id; Serial.printf("Baro: BMP388 at 0x%02X\n", addrs[i]); return; }
      }
    }
  }
  Serial.println("Baro: not found (optional)");
}

// --- Magnetometer (HMC5883L / QMC5883L) ---
void detectMag() {
  // HMC5883L at 0x1E — verify ID register A (0x0A) returns 0x48 ('H')
  Wire.beginTransmission(0x1E);
  if (Wire.endTransmission() == 0) {
    Wire.beginTransmission(0x1E);
    Wire.write(0x0A);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)0x1E, (uint8_t)1);
    if (Wire.available()) {
      uint8_t id = Wire.read();
      if (id == 0x48) { magAddr = 0x1E; magType = id; Serial.println("Mag : HMC5883L at 0x1E"); return; }
    }
  }
  // QMC5883L at 0x0D
  Wire.beginTransmission(0x0D);
  if (Wire.endTransmission() == 0) {
    magAddr = 0x0D; magType = 0x0D;
    Serial.println("Mag : QMC5883L at 0x0D");
    return;
  }
  Serial.println("Mag : not found (optional)");
}

// --- Configure IMU registers ---
void configureIMU() {
  // Wake up — clear sleep bit
  Wire.beginTransmission(imuAddr);
  Wire.write(0x6B); Wire.write(0x00);
  Wire.endTransmission();
  delay(100);

  // DLPF config: bandwidth ~10 Hz (reg 0x1A = 0x05)
  Wire.beginTransmission(imuAddr);
  Wire.write(0x1A); Wire.write(0x05);
  Wire.endTransmission();

  // Gyro range: ±500°/s  → divisor 65.5 (reg 0x1B = 0x08)
  Wire.beginTransmission(imuAddr);
  Wire.write(0x1B); Wire.write(0x08);
  Wire.endTransmission();

  // Accel range: ±8 g    → divisor 4096 (reg 0x1C = 0x10)
  Wire.beginTransmission(imuAddr);
  Wire.write(0x1C); Wire.write(0x10);
  Wire.endTransmission();

  Serial.println("IMU configured: DLPF=5, Gyro=±500°/s, Accel=±8g");
}

// --- Read raw sensor data from IMU ---
void readIMU() {
  if (!imuAddr) return;
  
  Wire.beginTransmission(imuAddr);
  Wire.write(0x3B);                          // start at ACCEL_XOUT_H
  Wire.endTransmission(false);               // repeated START
  if (Wire.requestFrom(imuAddr, (uint8_t)14) == 14 && Wire.available() >= 14) {
    int16_t AccXLSB = (int16_t)(Wire.read() << 8 | Wire.read());
    int16_t AccYLSB = (int16_t)(Wire.read() << 8 | Wire.read());
    int16_t AccZLSB = (int16_t)(Wire.read() << 8 | Wire.read());
    Wire.read(); Wire.read();   // skip temperature bytes
    int16_t GyroX   = (int16_t)(Wire.read() << 8 | Wire.read());
    int16_t GyroY   = (int16_t)(Wire.read() << 8 | Wire.read());
    int16_t GyroZ   = (int16_t)(Wire.read() << 8 | Wire.read());

    RateRoll  = (float)GyroX   / 65.5f;
    RatePitch = (float)GyroY   / 65.5f;
    RateYaw   = (float)GyroZ   / 65.5f;
    AccX      = (float)AccXLSB / 4096.0f;
    AccY      = (float)AccYLSB / 4096.0f;
    AccZ      = (float)AccZLSB / 4096.0f;
  }
}

// --- Apply Sensor Calibration Offsets ---
void applyCalibration() {
  RateRoll  -= RateCalibrationRoll;
  RatePitch -= RateCalibrationPitch;
  RateYaw   -= RateCalibrationYaw;
  AccX      -= AccXCalibration;
  AccY      -= AccYCalibration;
  AccZ      -= AccZCalibration;
}

// --- Attitude Estimation (Complementary Filter) ---
void updateAttitudeFilter() {
  AngleRoll  =  atanf(AccY / sqrtf(AccX * AccX + AccZ * AccZ)) * RAD_TO_DEG_F;
  AnglePitch = -atanf(AccX / sqrtf(AccY * AccY + AccZ * AccZ)) * RAD_TO_DEG_F;

  complementaryAngleRoll  = 0.991f * (complementaryAngleRoll  + RateRoll  * dt) + 0.009f * AngleRoll;
  complementaryAnglePitch = 0.991f * (complementaryAnglePitch + RatePitch * dt) + 0.009f * AnglePitch;
}
