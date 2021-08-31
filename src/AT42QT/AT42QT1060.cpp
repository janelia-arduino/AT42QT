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

AT42QT1060::DetectionStatus AT42QT1060::getDetectionStatus()
{
  DetectionStatus detection_status;
  read(RegisterAddresses::DETECTION_STATUS,detection_status.uint8);
  return detection_status;
}

bool AT42QT1060::anyKeyTouched()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.any_keys;
}

bool AT42QT1060::calibrating()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.calibrate;
}

AT42QT1060::KeyStatus AT42QT1060::getKeyStatus()
{
  KeyStatus key_status;
  read(RegisterAddresses::DETECTION_STATUS,key_status.uint8);
  return key_status;
}

void AT42QT1060::triggerCalibration()
{
  write(RegisterAddresses::CALIBRATE,NONZERO_VALUE);
}

void AT42QT1060::reset()
{
  write(RegisterAddresses::RESET,NONZERO_VALUE);
}

