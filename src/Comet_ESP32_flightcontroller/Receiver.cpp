#include "Config.h"
#include "Receiver.h"

void initReceiver() {
  Serial2.begin(115200, SERIAL_8N1, IBUS_RX_PIN, -1);
  for (int i = 0; i < IBUS_NUM_CHANNELS; i++) rcChannels[i] = 1000;
  rcChannels[0] = 1500;  // Roll   center
  rcChannels[1] = 1500;  // Pitch  center
  rcChannels[3] = 1500;  // Yaw    center
  // rcChannels[2] stays at 1000 (throttle low)
}

void readIBUS() {
  uint32_t now = micros();

  // Timeout: reset parser if no byte received for >3 ms (half frame period)
  if (ibusIndex > 0 && (now - lastIbusByteTime) > 3000) {
    ibusIndex = 0;
  }

  while (Serial2.available()) {
    uint8_t b = Serial2.read();
    lastIbusByteTime = now;

    // Synchronise to frame header 0x20 0x40
    if (ibusIndex == 0 && b != IBUS_HEADER1) continue;
    if (ibusIndex == 1 && b != IBUS_HEADER2) { ibusIndex = 0; continue; }

    ibusBuffer[ibusIndex++] = b;

    if (ibusIndex == IBUS_FRAME_SIZE) {
      ibusIndex = 0;

      // Validate checksum: 0xFFFF − sum(bytes[0..29])
      uint16_t checksum = 0xFFFF;
      for (uint8_t i = 0; i < 30; i++) checksum -= ibusBuffer[i];
      uint16_t received = ibusBuffer[30] | ((uint16_t)ibusBuffer[31] << 8);

      if (checksum == received) {
        // Parse 14 little-endian 16-bit channel values
        for (uint8_t ch = 0; ch < IBUS_NUM_CHANNELS; ch++) {
          rcChannels[ch] = ibusBuffer[2 + ch * 2] | ((uint16_t)ibusBuffer[3 + ch * 2] << 8);
        }
        lastIbusFrameTime = now;
      }
    }
  }
}

void checkReceiverFailsafe() {
  if (micros() - lastIbusFrameTime > RX_TIMEOUT_US) {
    if (!failsafe) Serial.println("!!! FAILSAFE — no IBUS frames");
    failsafe = true;
    armed    = false;
  } else {
    failsafe = false;
  }
}

void updateArmingState() {
  // Boot arming guard: require AUX1 switch to be seen DISARMED (<1300) at least once after power-up
  if (rcChannels[4] < 1300) {
    armPreventedOnBoot = false;
  }

  if (!failsafe && !armed && !armPreventedOnBoot && rcChannels[4] > 1700 && rcChannels[2] < 1050) {
    armed = true;
    Serial.println(">> ARMED");
  } else if (rcChannels[4] < 1300 || failsafe) {
    if (armed) Serial.println(failsafe ? ">> FAILSAFE DISARM" : ">> DISARMED");
    armed = false;
  }
}
