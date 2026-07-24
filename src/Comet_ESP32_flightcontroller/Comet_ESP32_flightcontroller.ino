/*
  ============================================
  WARNING / DISCLAIMER
  ============================================

  This ESP32 flight controller code is an
  experimental DIY project and is NOT as
  stable, reliable, or fully tested as
  professional flight controller firmware
  such as INAV, Betaflight, ArduPilot, or PX4.

  Use this code completely at your own risk.

  The developer is NOT responsible for:
   - Drone crashes
   - Flyaways
   - Hardware damage
   - Property damage
   - Injuries or accidents
   - Any loss caused by using this code

  This software may contain bugs, unstable
  behavior, tuning problems, or unexpected
  failures during flight.

  Always:
   - Test without propellers first
   - Perform safe ground testing
   - Fly in open areas only
   - Keep away from people and animals

  By using or modifying this code, you accept
  full responsibility for all risks involved.
  
  Fly safe.
  ============================================
*/

#include "Config.h"
#include "Storage.h"
#include "Sensors.h"
#include "Receiver.h"
#include "PID.h"
#include "Motors.h"
#include "CLI.h"

// ============================================================
//  SETUP
// ============================================================
void setup() {
  // --- Serial with enlarged TX buffer ---
  Serial.setTxBufferSize(1024);
  Serial.begin(115200);

  // --- LED boot pattern ---
  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, LOW);  delay(100);
    digitalWrite(LED_PIN, HIGH); delay(100);
  }
  digitalWrite(LED_PIN, LOW);

  // --- Load all config from EEPROM (NVS) ---
  loadConfig();

  // --- I2C at 400 kHz fast-mode ---
  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  // --- Auto-detect & configure sensors ---
  detectIMU();
  detectBaro();
  detectMag();
  if (imuAddr) {
    configureIMU();
  }

  // --- IBUS Receiver Initialization ---
  initReceiver();

  // --- ESC & Motor Initialization ---
  initMotors();

  // --- Ready indication ---
  digitalWrite(LED_PIN, HIGH); delay(500);
  digitalWrite(LED_PIN, LOW);  delay(500);

  Serial.println("=== Comet FC Ready ===");
  Serial.println("Type HELP for serial commands");

  LoopTimer = micros();
}


// ============================================================
//  MAIN LOOP  (250 Hz)
// ============================================================
void loop() {
  // 1. Read IBUS RC Input
  readIBUS();

  // 2. Failsafe Check
  checkReceiverFailsafe();

  // 3. Arming Logic & Boot Safety Guard
  updateArmingState();

  // 4. Read IMU burst data
  readIMU();

  // 5. Apply Calibration Offsets
  applyCalibration();

  // 6. Attitude Estimation (Complementary Filter)
  updateAttitudeFilter();

  // 7-12. Cascaded Angle & Rate PID Calculations
  processPID();

  // 13-16. Motor Mixing, Clamping, Disarm Guard & PWM Writes
  updateMotors();

  // 17. LED Status Update
  updateLED();

  // 18. Throttled Telemetry Logging (~5 Hz)
  logTelemetry();

  // 19. Process Serial CLI Commands
  processSerialCommand();

  // 20. Loop Timing — fixed 250 Hz with Overrun Guard
  uint32_t elapsed = micros() - LoopTimer;
  if (elapsed < LOOP_PERIOD_US) {
    while (micros() - LoopTimer < LOOP_PERIOD_US);
    LoopTimer += LOOP_PERIOD_US;
  } else {
    // Resync timer on loop overrun to prevent rapid catchup loops
    LoopTimer = micros();
  }
}
