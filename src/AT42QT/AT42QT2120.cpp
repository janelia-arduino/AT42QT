// ----------------------------------------------------------------------------
// AT42QT2120.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT2120.h"


AT42QT2120::Status AT42QT2120::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT2120::DETECTION_STATUS,status.bytes,STATUS_SIZE);
  return status;
}

bool AT42QT2120::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}

uint8_t AT42QT2120::getMeasurementIntervalCount()
{
  uint8_t interval_count;
  read(RegisterAddresses::AT42QT2120::LOW_POWER_MODE,interval_count);
  return interval_count;
}

void AT42QT2120::setMeasurementIntervalCount(uint8_t interval_count)
{
  write(RegisterAddresses::AT42QT2120::LOW_POWER_MODE,interval_count);
}

uint8_t AT42QT2120::getTowardsDriftDuration()
{
  uint8_t tdd;
  read(RegisterAddresses::AT42QT2120::TOWARDS_DRIFT_DURATION,tdd);
  return tdd;
}

void AT42QT2120::setTowardsDriftDuration(uint8_t tdd)
{
  if (tdd > DRIFT_DURATION_MAX)
  {
    tdd = DRIFT_DURATION_MAX;
  }
  write(RegisterAddresses::AT42QT2120::TOWARDS_DRIFT_DURATION,tdd);
}

uint8_t AT42QT2120::getAwayDriftDuration()
{
  uint8_t add;
  read(RegisterAddresses::AT42QT2120::AWAY_DRIFT_DURATION,add);
  return add;
}

void AT42QT2120::setAwayDriftDuration(uint8_t add)
{
  if (add > DRIFT_DURATION_MAX)
  {
    add = DRIFT_DURATION_MAX;
  }
  write(RegisterAddresses::AT42QT2120::AWAY_DRIFT_DURATION,add);
}

uint8_t AT42QT2120::getDetectionIntegrator()
{
  uint8_t di;
  read(RegisterAddresses::AT42QT2120::DETECTION_INTEGRATOR,di);
  return di;
}

void AT42QT2120::setDetectionIntegrator(uint8_t di)
{
  if (di > DETECTION_INTEGRATOR_MAX)
  {
    di = DETECTION_INTEGRATOR_MAX;
  }
  write(RegisterAddresses::AT42QT2120::DETECTION_INTEGRATOR,di);
}

uint8_t AT42QT2120::getRecalibrationDelay()
{
  uint8_t rd;
  read(RegisterAddresses::AT42QT2120::RECALIBRATION_DELAY,rd);
  return rd;
}

void AT42QT2120::setRecalibrationDelay(uint8_t rd)
{
  write(RegisterAddresses::AT42QT2120::RECALIBRATION_DELAY,rd);
}

uint8_t AT42QT2120::getDriftHoldDuration()
{
  uint8_t dhd;
  read(RegisterAddresses::AT42QT2120::DRIFT_HOLD_DURATION,dhd);
  return dhd;
}

void AT42QT2120::setDriftHoldDuration(uint8_t dhd)
{
  write(RegisterAddresses::AT42QT2120::DRIFT_HOLD_DURATION,dhd);
}

