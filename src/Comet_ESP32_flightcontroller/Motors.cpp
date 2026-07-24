#include "Config.h"
#include "Motors.h"
#include "PID.h"

void initMotors() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  mot1.setPeriodHertz(ESCfreq);  mot1.attach(mot1_pin, 1000, 2000);
  mot2.setPeriodHertz(ESCfreq);  mot2.attach(mot2_pin, 1000, 2000);
  mot3.setPeriodHertz(ESCfreq);  mot3.attach(mot3_pin, 1000, 2000);
  mot4.setPeriodHertz(ESCfreq);  mot4.attach(mot4_pin, 1000, 2000);
  delay(100);

  mot1.writeMicroseconds(1000);
  mot2.writeMicroseconds(1000);
  mot3.writeMicroseconds(1000);
  mot4.writeMicroseconds(1000);
  delay(2000);  // let ESCs complete power-on at minimum throttle
}

void updateMotors() {
  // -------- 1. Throttle Cap --------
  if (InputThrottle > 1800.0f) InputThrottle = 1800.0f;

  // -------- 2. Motor Mixing (X-quad) --------
  MotorInput1 = InputThrottle - InputRoll - InputPitch - InputYaw;  // front right  CCW
  MotorInput2 = InputThrottle - InputRoll + InputPitch + InputYaw;  // rear  right  CW
  MotorInput3 = InputThrottle + InputRoll + InputPitch - InputYaw;  // rear  left   CCW
  MotorInput4 = InputThrottle + InputRoll - InputPitch + InputYaw;  // front left   CW

  // Clamp upper
  if (MotorInput1 > 2000.0f) MotorInput1 = 1999.0f;
  if (MotorInput2 > 2000.0f) MotorInput2 = 1999.0f;
  if (MotorInput3 > 2000.0f) MotorInput3 = 1999.0f;
  if (MotorInput4 > 2000.0f) MotorInput4 = 1999.0f;

  // Clamp lower (idle spin while armed)
  if (MotorInput1 < (float)ThrottleIdle) MotorInput1 = (float)ThrottleIdle;
  if (MotorInput2 < (float)ThrottleIdle) MotorInput2 = (float)ThrottleIdle;
  if (MotorInput3 < (float)ThrottleIdle) MotorInput3 = (float)ThrottleIdle;
  if (MotorInput4 < (float)ThrottleIdle) MotorInput4 = (float)ThrottleIdle;

  // -------- 3. Disarm Override --------
  if (!armed) {
    MotorInput1 = (float)ThrottleCutOff;
    MotorInput2 = (float)ThrottleCutOff;
    MotorInput3 = (float)ThrottleCutOff;
    MotorInput4 = (float)ThrottleCutOff;
    resetPIDState();
  }

  // -------- 4. Write Motors --------
  mot1.writeMicroseconds((int)MotorInput1);
  mot2.writeMicroseconds((int)MotorInput2);
  mot3.writeMicroseconds((int)MotorInput3);
  mot4.writeMicroseconds((int)MotorInput4);
}
