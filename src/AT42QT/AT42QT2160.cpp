// ----------------------------------------------------------------------------
// AT42QT2160.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT2160.h"


AT42QT2160::AT42QT2160(TwoWire & wire,
  int8_t change_pin,
  int8_t reset_pin) :
AT42QT(DEVICE_ADDRESS,CHIP_ID,wire,change_pin,reset_pin)
{
}

AT42QT2160::Status AT42QT2160::getStatus()
{
  Status status;
  read(RegisterAddresses::GENERAL_STATUS,status.bytes);
  return status;
}

void AT42QT2160::triggerCalibration()
{
  write(RegisterAddresses::CALIBRATE,NONZERO_VALUE);
}

void AT42QT2160::reset()
{
  write(RegisterAddresses::RESET,NONZERO_VALUE);
}

