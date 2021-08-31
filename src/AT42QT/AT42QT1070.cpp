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

AT42QT1070::DetectionStatus AT42QT1070::getDetectionStatus()
{
  DetectionStatus detection_status;
  read(RegisterAddresses::DETECTION_STATUS,detection_status.uint8);
  return detection_status;
}

bool AT42QT1070::anyKeyTouched()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.any_keys;
}

bool AT42QT1070::overflow()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.overflow;
}

bool AT42QT1070::calibrating()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.calibrate;
}

AT42QT1070::KeyStatus AT42QT1070::getKeyStatus()
{
  KeyStatus key_status;
  read(RegisterAddresses::DETECTION_STATUS,key_status.uint8);
  return key_status;
}

void AT42QT1070::triggerCalibration()
{
  write(RegisterAddresses::CALIBRATE,NONZERO_VALUE);
}

void AT42QT1070::reset()
{
  write(RegisterAddresses::RESET,NONZERO_VALUE);
}

