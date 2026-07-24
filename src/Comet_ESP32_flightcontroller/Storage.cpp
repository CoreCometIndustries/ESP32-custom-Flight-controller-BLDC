#include "Config.h"
#include "Storage.h"

void loadConfig() {
  // --- Rate PID ---
  prefs.begin("pid-rate", true);    // read-only
  PRateRoll  = prefs.getFloat("pRoll",  0.625f);
  IRateRoll  = prefs.getFloat("iRoll",  2.1f);
  DRateRoll  = prefs.getFloat("dRoll",  0.0088f);
  PRatePitch = prefs.getFloat("pPitch", 0.625f);
  IRatePitch = prefs.getFloat("iPitch", 2.1f);
  DRatePitch = prefs.getFloat("dPitch", 0.0088f);
  PRateYaw   = prefs.getFloat("pYaw",   4.0f);
  IRateYaw   = prefs.getFloat("iYaw",   3.0f);
  DRateYaw   = prefs.getFloat("dYaw",   0.0f);
  prefs.end();

  // --- Angle PID ---
  prefs.begin("pid-angle", true);
  PAngleRoll  = prefs.getFloat("pRoll",  2.0f);
  IAngleRoll  = prefs.getFloat("iRoll",  0.5f);
  DAngleRoll  = prefs.getFloat("dRoll",  0.007f);
  PAnglePitch = prefs.getFloat("pPitch", 2.0f);
  IAnglePitch = prefs.getFloat("iPitch", 0.5f);
  DAnglePitch = prefs.getFloat("dPitch", 0.007f);
  prefs.end();

  // --- Calibration ---
  prefs.begin("calib", true);
  RateCalibrationRoll  = prefs.getFloat("gyroR",  0.27f);
  RateCalibrationPitch = prefs.getFloat("gyroP", -0.85f);
  RateCalibrationYaw   = prefs.getFloat("gyroY", -2.09f);
  AccXCalibration      = prefs.getFloat("accX",   0.03f);
  AccYCalibration      = prefs.getFloat("accY",   0.01f);
  AccZCalibration      = prefs.getFloat("accZ",  -0.07f);
  prefs.end();

  // --- Config ---
  prefs.begin("config", true);
  ESCfreq        = prefs.getInt("escFreq", 500);
  ThrottleIdle   = prefs.getInt("thrIdle", 1170);
  ThrottleCutOff = prefs.getInt("thrCut",  1000);
  prefs.end();

  Serial.println("Config loaded from EEPROM");
}

void saveConfig() {
  prefs.begin("pid-rate", false);   // read-write
  prefs.putFloat("pRoll",  PRateRoll);
  prefs.putFloat("iRoll",  IRateRoll);
  prefs.putFloat("dRoll",  DRateRoll);
  prefs.putFloat("pPitch", PRatePitch);
  prefs.putFloat("iPitch", IRatePitch);
  prefs.putFloat("dPitch", DRatePitch);
  prefs.putFloat("pYaw",   PRateYaw);
  prefs.putFloat("iYaw",   IRateYaw);
  prefs.putFloat("dYaw",   DRateYaw);
  prefs.end();

  prefs.begin("pid-angle", false);
  prefs.putFloat("pRoll",  PAngleRoll);
  prefs.putFloat("iRoll",  IAngleRoll);
  prefs.putFloat("dRoll",  DAngleRoll);
  prefs.putFloat("pPitch", PAnglePitch);
  prefs.putFloat("iPitch", IAnglePitch);
  prefs.putFloat("dPitch", DAnglePitch);
  prefs.end();

  prefs.begin("calib", false);
  prefs.putFloat("gyroR", RateCalibrationRoll);
  prefs.putFloat("gyroP", RateCalibrationPitch);
  prefs.putFloat("gyroY", RateCalibrationYaw);
  prefs.putFloat("accX",  AccXCalibration);
  prefs.putFloat("accY",  AccYCalibration);
  prefs.putFloat("accZ",  AccZCalibration);
  prefs.end();

  prefs.begin("config", false);
  prefs.putInt("escFreq", ESCfreq);
  prefs.putInt("thrIdle", ThrottleIdle);
  prefs.putInt("thrCut",  ThrottleCutOff);
  prefs.end();

  Serial.println(">> Config saved to EEPROM");
}

void resetConfig() {
  prefs.begin("pid-rate",  false); prefs.clear(); prefs.end();
  prefs.begin("pid-angle", false); prefs.clear(); prefs.end();
  prefs.begin("calib",     false); prefs.clear(); prefs.end();
  prefs.begin("config",    false); prefs.clear(); prefs.end();

  // Reload hardcoded defaults
  PRateRoll = 0.625f; IRateRoll = 2.1f;  DRateRoll = 0.0088f;
  PRatePitch= 0.625f; IRatePitch= 2.1f;  DRatePitch= 0.0088f;
  PRateYaw  = 4.0f;   IRateYaw  = 3.0f;  DRateYaw  = 0.0f;

  PAngleRoll  = 2.0f; IAngleRoll  = 0.5f; DAngleRoll  = 0.007f;
  PAnglePitch = 2.0f; IAnglePitch = 0.5f; DAnglePitch = 0.007f;

  RateCalibrationRoll  =  0.27f;
  RateCalibrationPitch = -0.85f;
  RateCalibrationYaw   = -2.09f;
  AccXCalibration =  0.03f;
  AccYCalibration =  0.01f;
  AccZCalibration = -0.07f;

  ESCfreq = 500; ThrottleIdle = 1170; ThrottleCutOff = 1000;

  Serial.println(">> Factory reset — defaults loaded");
}
