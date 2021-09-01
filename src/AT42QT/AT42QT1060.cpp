// ----------------------------------------------------------------------------
// AT42QT1060.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT1060.h"


AT42QT1060::AT42QT1060(TwoWire & wire,
  int8_t change_pin,
  int8_t reset_pin) :
AT42QT(DEVICE_ADDRESS,CHIP_ID,wire,change_pin,reset_pin)
{
}

AT42QT1060::Status AT42QT1060::getStatus()
{
  Status status;
  read(RegisterAddresses::DETECTION_STATUS,status.bytes);
  return status;
}

bool AT42QT1060::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}

void AT42QT1060::triggerCalibration()
{
  write(RegisterAddresses::CALIBRATE,NONZERO_VALUE);
}

void AT42QT1060::reset()
{
  write(RegisterAddresses::RESET,NONZERO_VALUE);
}

