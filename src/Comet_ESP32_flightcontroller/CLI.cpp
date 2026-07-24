#include "Config.h"
#include "CLI.h"
#include "Storage.h"

void processSerialCommand() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (cmdIndex > 0) {
        cmdBuffer[cmdIndex] = '\0';

        // ---- SAVE ----
        if (strcmp(cmdBuffer, "SAVE") == 0) {
          saveConfig();
        }
        // ---- RESET ----
        else if (strcmp(cmdBuffer, "RESET") == 0) {
          resetConfig();
        }
        // ---- DUMP ----
        else if (strcmp(cmdBuffer, "DUMP") == 0) {
          Serial.println("=== Current Config ===");
          Serial.printf("Rate  Roll : P=%.4f  I=%.4f  D=%.5f\n", PRateRoll, IRateRoll, DRateRoll);
          Serial.printf("Rate  Pitch: P=%.4f  I=%.4f  D=%.5f\n", PRatePitch, IRatePitch, DRatePitch);
          Serial.printf("Rate  Yaw  : P=%.4f  I=%.4f  D=%.5f\n", PRateYaw, IRateYaw, DRateYaw);
          Serial.printf("Angle Roll : P=%.4f  I=%.4f  D=%.5f\n", PAngleRoll, IAngleRoll, DAngleRoll);
          Serial.printf("Angle Pitch: P=%.4f  I=%.4f  D=%.5f\n", PAnglePitch, IAnglePitch, DAnglePitch);
          Serial.printf("Gyro Cal   : R=%.4f  P=%.4f  Y=%.4f\n", RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw);
          Serial.printf("Acc  Cal   : X=%.4f  Y=%.4f  Z=%.4f\n", AccXCalibration, AccYCalibration, AccZCalibration);
          Serial.printf("ESC=%d  Idle=%d  Cut=%d\n", ESCfreq, ThrottleIdle, ThrottleCutOff);
          Serial.printf("IMU=0x%02X  Baro=0x%02X  Mag=0x%02X\n", imuAddr, baroAddr, magAddr);
          Serial.printf("Armed=%d  Failsafe=%d\n", armed, failsafe);
          Serial.println("======================");
        }
        // ---- DUMP_JSON ----
        else if (strcmp(cmdBuffer, "DUMP_JSON") == 0) {
          char json[512];
          snprintf(json, sizeof(json),
            "{\"ratePID\":{\"roll\":{\"p\":%.4f,\"i\":%.4f,\"d\":%.5f},"
            "\"pitch\":{\"p\":%.4f,\"i\":%.4f,\"d\":%.5f},"
            "\"yaw\":{\"p\":%.4f,\"i\":%.4f,\"d\":%.5f}},"
            "\"anglePID\":{\"roll\":{\"p\":%.4f,\"i\":%.4f,\"d\":%.5f},"
            "\"pitch\":{\"p\":%.4f,\"i\":%.4f,\"d\":%.5f}},"
            "\"calib\":{\"gyroR\":%.4f,\"gyroP\":%.4f,\"gyroY\":%.4f,"
            "\"accX\":%.4f,\"accY\":%.4f,\"accZ\":%.4f},"
            "\"config\":{\"escFreq\":%d,\"thrIdle\":%d,\"thrCut\":%d},"
            "\"sensors\":{\"imu\":\"0x%02X\",\"baro\":\"0x%02X\",\"mag\":\"0x%02X\"},"
            "\"status\":{\"armed\":%s,\"failsafe\":%s}}",
            PRateRoll, IRateRoll, DRateRoll,
            PRatePitch, IRatePitch, DRatePitch,
            PRateYaw, IRateYaw, DRateYaw,
            PAngleRoll, IAngleRoll, DAngleRoll,
            PAnglePitch, IAnglePitch, DAnglePitch,
            RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw,
            AccXCalibration, AccYCalibration, AccZCalibration,
            ESCfreq, ThrottleIdle, ThrottleCutOff,
            imuAddr, baroAddr, magAddr,
            armed ? "true" : "false", failsafe ? "true" : "false");
          Serial.println(json);
        }
        // ---- SET <param> <value> ----
        else if (strncmp(cmdBuffer, "SET ", 4) == 0) {
          char param[24];
          float value;
          if (sscanf(cmdBuffer + 4, "%23s %f", param, &value) == 2) {
            bool found = true;
            // Rate PID
            if      (strcmp(param, "PRateRoll")  == 0) PRateRoll  = value;
            else if (strcmp(param, "IRateRoll")  == 0) IRateRoll  = value;
            else if (strcmp(param, "DRateRoll")  == 0) DRateRoll  = value;
            else if (strcmp(param, "PRatePitch") == 0) PRatePitch = value;
            else if (strcmp(param, "IRatePitch") == 0) IRatePitch = value;
            else if (strcmp(param, "DRatePitch") == 0) DRatePitch = value;
            else if (strcmp(param, "PRateYaw")   == 0) PRateYaw   = value;
            else if (strcmp(param, "IRateYaw")   == 0) IRateYaw   = value;
            else if (strcmp(param, "DRateYaw")   == 0) DRateYaw   = value;
            // Angle PID
            else if (strcmp(param, "PAngleRoll")  == 0) PAngleRoll  = value;
            else if (strcmp(param, "IAngleRoll")  == 0) IAngleRoll  = value;
            else if (strcmp(param, "DAngleRoll")  == 0) DAngleRoll  = value;
            else if (strcmp(param, "PAnglePitch") == 0) PAnglePitch = value;
            else if (strcmp(param, "IAnglePitch") == 0) IAnglePitch = value;
            else if (strcmp(param, "DAnglePitch") == 0) DAnglePitch = value;
            // Calibration
            else if (strcmp(param, "GyroCalR") == 0) RateCalibrationRoll  = value;
            else if (strcmp(param, "GyroCalP") == 0) RateCalibrationPitch = value;
            else if (strcmp(param, "GyroCalY") == 0) RateCalibrationYaw   = value;
            else if (strcmp(param, "AccCalX")  == 0) AccXCalibration      = value;
            else if (strcmp(param, "AccCalY")  == 0) AccYCalibration      = value;
            else if (strcmp(param, "AccCalZ")  == 0) AccZCalibration      = value;
            // Config
            else if (strcmp(param, "escFreq")  == 0) ESCfreq        = (int)value;
            else if (strcmp(param, "thrIdle")  == 0) ThrottleIdle   = (int)value;
            else if (strcmp(param, "thrCut")   == 0) ThrottleCutOff = (int)value;
            else { found = false; Serial.printf("Unknown param: %s\n", param); }

            if (found) Serial.printf("SET %s = %.5f  (use SAVE to persist)\n", param, value);
          } else {
            Serial.println("Usage: SET <param> <value>");
          }
        }
        // ---- HELP ----
        else if (strcmp(cmdBuffer, "HELP") == 0) {
          Serial.println("=== Comet FC Serial Commands ===");
          Serial.println("DUMP              — show all current values");
          Serial.println("SAVE              — persist to EEPROM");
          Serial.println("RESET             — factory reset (clear EEPROM)");
          Serial.println("SET <param> <val> — change value at runtime");
          Serial.println("  Rate PID  : PRateRoll  IRateRoll  DRateRoll");
          Serial.println("              PRatePitch IRatePitch DRatePitch");
          Serial.println("              PRateYaw   IRateYaw   DRateYaw");
          Serial.println("  Angle PID : PAngleRoll IAngleRoll DAngleRoll");
          Serial.println("              PAnglePitch IAnglePitch DAnglePitch");
          Serial.println("  Calibration: GyroCalR GyroCalP GyroCalY");
          Serial.println("               AccCalX  AccCalY  AccCalZ");
          Serial.println("  Config     : escFreq  thrIdle  thrCut");
          Serial.println("================================");
        }
        else {
          Serial.printf("Unknown cmd: %s  (type HELP)\n", cmdBuffer);
        }

        cmdIndex = 0;
      }
    } else if (cmdIndex < sizeof(cmdBuffer) - 1) {
      cmdBuffer[cmdIndex++] = c;
    }
  }
}

void updateLED() {
  uint32_t now = millis();
  if (armed) {
    digitalWrite(LED_PIN, HIGH);               // solid ON
  } else if (failsafe) {
    digitalWrite(LED_PIN, (now / 100) % 2);    // rapid blink 100 ms
  } else {
    digitalWrite(LED_PIN, (now / 500) % 2);    // slow blink  500 ms
  }
}

void logTelemetry() {
  static uint16_t logCounter = 0;
  if (++logCounter >= LOG_DIVIDER) {
    logCounter = 0;
    if (Serial.availableForWrite() > 256) {
      char buf[256];
      int len = snprintf(buf, sizeof(buf),
        "RC:%d %d %d %d %d %d M:%d %d %d %d A:%.2f %.2f %.2f G:%.1f %.1f %.1f %s\n",
        rcChannels[0], rcChannels[1], rcChannels[2],
        rcChannels[3], rcChannels[4], rcChannels[5],
        (int)MotorInput1, (int)MotorInput2, (int)MotorInput3, (int)MotorInput4,
        AccX, AccY, AccZ, RateRoll, RatePitch, RateYaw,
        armed ? "ARMED" : (failsafe ? "FAILSAFE" : "DISARMED"));
      Serial.write(buf, len);
    }
  }
}
