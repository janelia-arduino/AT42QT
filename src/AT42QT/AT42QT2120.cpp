// ----------------------------------------------------------------------------
// AT42QT2120.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT2120.h"


AT42QT2120::AT42QT2120(TwoWire & wire,
  int8_t change_pin,
  int8_t reset_pin) :
AT42QT(DEVICE_ADDRESS,CHIP_ID,wire,change_pin,reset_pin)
{
}

AT42QT2120::Status AT42QT2120::getStatus()
{
  Status status;
  read(RegisterAddresses::DETECTION_STATUS,status.uint32);
  return status;
}

bool AT42QT2120::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}

uint8_t AT42QT2120::getSliderPosition()
{
  uint8_t slider_position;
  read(RegisterAddresses::SLIDER_POSITION,slider_position);
  return slider_position;
}

void AT42QT2120::triggerCalibration()
{
  write(RegisterAddresses::CALIBRATE,NONZERO_VALUE);
}

void AT42QT2120::reset()
{
  write(RegisterAddresses::RESET,NONZERO_VALUE);
}

uint8_t AT42QT2120::getMeasurementIntervalCount()
{
  uint8_t interval_count;
  read(RegisterAddresses::LOW_POWER_MODE,interval_count);
  return interval_count;
}

void AT42QT2120::setMeasurementIntervalCount(uint8_t interval_count)
{
  write(RegisterAddresses::LOW_POWER_MODE,interval_count);
}

