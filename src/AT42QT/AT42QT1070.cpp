// ----------------------------------------------------------------------------
// AT42QT1070.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT1070.h"


AT42QT1070::AT42QT1070(TwoWire & wire,
  int8_t change_pin,
  int8_t reset_pin) :
AT42QT(DEVICE_ADDRESS,CHIP_ID,wire,change_pin,reset_pin)
{
}

AT42QT1070::Status AT42QT1070::getStatus()
{
  Status status;
  read(RegisterAddresses::DETECTION_STATUS,status.bytes);
  return status;
}

bool AT42QT1070::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}

void AT42QT1070::triggerCalibration()
{
  write(RegisterAddresses::CALIBRATE,NONZERO_VALUE);
}

void AT42QT1070::reset()
{
  write(RegisterAddresses::RESET,NONZERO_VALUE);
}

