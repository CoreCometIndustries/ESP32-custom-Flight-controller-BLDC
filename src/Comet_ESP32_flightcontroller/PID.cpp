#include "Config.h"
#include "PID.h"

void processPID() {
  // -------- 1. RC Input Mapping --------
  DesiredAngleRoll  = 0.1f  * ((float)rcChannels[0] - 1500.0f);
  DesiredAnglePitch = 0.1f  * ((float)rcChannels[1] - 1500.0f);
  InputThrottle     = (float)rcChannels[2];
  DesiredRateYaw    = 0.15f * ((float)rcChannels[3] - 1500.0f);

  // -------- 2. Angle PID — Roll --------
  ErrorAngleRoll = DesiredAngleRoll - complementaryAngleRoll;
  PtermRoll = PAngleRoll * ErrorAngleRoll;
  ItermRoll = PrevItermAngleRoll + (IAngleRoll * (ErrorAngleRoll + PrevErrorAngleRoll) * (dt / 2.0f));
  if (ItermRoll >  400.0f) ItermRoll =  400.0f;
  if (ItermRoll < -400.0f) ItermRoll = -400.0f;
  DtermRoll = DAngleRoll * ((ErrorAngleRoll - PrevErrorAngleRoll) / dt);
  PIDOutputRoll = PtermRoll + ItermRoll + DtermRoll;
  if (PIDOutputRoll >  400.0f) PIDOutputRoll =  400.0f;
  if (PIDOutputRoll < -400.0f) PIDOutputRoll = -400.0f;
  DesiredRateRoll    = PIDOutputRoll;
  PrevErrorAngleRoll = ErrorAngleRoll;
  PrevItermAngleRoll = ItermRoll;

  // -------- 3. Angle PID — Pitch --------
  ErrorAnglePitch = DesiredAnglePitch - complementaryAnglePitch;
  PtermPitch = PAnglePitch * ErrorAnglePitch;
  ItermPitch = PrevItermAnglePitch + (IAnglePitch * (ErrorAnglePitch + PrevErrorAnglePitch) * (dt / 2.0f));
  if (ItermPitch >  400.0f) ItermPitch =  400.0f;
  if (ItermPitch < -400.0f) ItermPitch = -400.0f;
  DtermPitch = DAnglePitch * ((ErrorAnglePitch - PrevErrorAnglePitch) / dt);
  PIDOutputPitch = PtermPitch + ItermPitch + DtermPitch;
  if (PIDOutputPitch >  400.0f) PIDOutputPitch =  400.0f;
  if (PIDOutputPitch < -400.0f) PIDOutputPitch = -400.0f;
  DesiredRatePitch    = PIDOutputPitch;
  PrevErrorAnglePitch = ErrorAnglePitch;
  PrevItermAnglePitch = ItermPitch;

  // -------- 4. Rate PID — Roll --------
  ErrorRateRoll = DesiredRateRoll - RateRoll;
  PtermRoll = PRateRoll * ErrorRateRoll;
  ItermRoll = PrevItermRateRoll + (IRateRoll * (ErrorRateRoll + PrevErrorRateRoll) * (dt / 2.0f));
  if (ItermRoll >  400.0f) ItermRoll =  400.0f;
  if (ItermRoll < -400.0f) ItermRoll = -400.0f;
  DtermRoll = -DRateRoll * ((RateRoll - PrevRateRoll) / dt);
  PIDOutputRoll = PtermRoll + ItermRoll + DtermRoll;
  if (PIDOutputRoll >  400.0f) PIDOutputRoll =  400.0f;
  if (PIDOutputRoll < -400.0f) PIDOutputRoll = -400.0f;
  InputRoll          = PIDOutputRoll;
  PrevErrorRateRoll  = ErrorRateRoll;
  PrevItermRateRoll  = ItermRoll;
  PrevRateRoll       = RateRoll;

  // -------- 5. Rate PID — Pitch --------
  ErrorRatePitch = DesiredRatePitch - RatePitch;
  PtermPitch = PRatePitch * ErrorRatePitch;
  ItermPitch = PrevItermRatePitch + (IRatePitch * (ErrorRatePitch + PrevErrorRatePitch) * (dt / 2.0f));
  if (ItermPitch >  400.0f) ItermPitch =  400.0f;
  if (ItermPitch < -400.0f) ItermPitch = -400.0f;
  DtermPitch = -DRatePitch * ((RatePitch - PrevRatePitch) / dt);
  PIDOutputPitch = PtermPitch + ItermPitch + DtermPitch;
  if (PIDOutputPitch >  400.0f) PIDOutputPitch =  400.0f;
  if (PIDOutputPitch < -400.0f) PIDOutputPitch = -400.0f;
  InputPitch          = PIDOutputPitch;
  PrevErrorRatePitch  = ErrorRatePitch;
  PrevItermRatePitch  = ItermPitch;
  PrevRatePitch       = RatePitch;

  // -------- 6. Rate PID — Yaw --------
  ErrorRateYaw = DesiredRateYaw - RateYaw;
  PtermYaw = PRateYaw * ErrorRateYaw;
  ItermYaw = PrevItermRateYaw + (IRateYaw * (ErrorRateYaw + PrevErrorRateYaw) * (dt / 2.0f));
  if (ItermYaw >  400.0f) ItermYaw =  400.0f;
  if (ItermYaw < -400.0f) ItermYaw = -400.0f;
  DtermYaw = -DRateYaw * ((RateYaw - PrevRateYaw) / dt);
  PIDOutputYaw = PtermYaw + ItermYaw + DtermYaw;
  if (PIDOutputYaw >  400.0f) PIDOutputYaw =  400.0f;
  if (PIDOutputYaw < -400.0f) PIDOutputYaw = -400.0f;
  InputYaw          = PIDOutputYaw;
  PrevErrorRateYaw  = ErrorRateYaw;
  PrevItermRateYaw  = ItermYaw;
  PrevRateYaw       = RateYaw;
}

void resetPIDState() {
  PrevErrorRateRoll  = 0.0f;  PrevItermRateRoll  = 0.0f;  PrevRateRoll  = 0.0f;
  PrevErrorRatePitch = 0.0f;  PrevItermRatePitch = 0.0f;  PrevRatePitch = 0.0f;
  PrevErrorRateYaw   = 0.0f;  PrevItermRateYaw   = 0.0f;  PrevRateYaw   = 0.0f;
  PrevErrorAngleRoll = 0.0f;  PrevItermAngleRoll = 0.0f;
  PrevErrorAnglePitch= 0.0f;  PrevItermAnglePitch= 0.0f;
}
