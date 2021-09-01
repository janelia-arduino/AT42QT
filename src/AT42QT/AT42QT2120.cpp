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

