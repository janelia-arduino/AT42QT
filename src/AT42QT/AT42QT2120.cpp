// ----------------------------------------------------------------------------
// AT42QT2120.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT2120.h"


AT42QT2120::AT42QT2120(TwoWire & wire,
  int8_t reset_pin) :
AT42QT(DEVICE_ADDRESS,CHIP_ID,wire,reset_pin)
{
}

AT42QT2120::DetectionStatus AT42QT2120::getDetectionStatus()
{
  DetectionStatus detection_status;
  read(RegisterAddresses::DETECTION_STATUS,detection_status.uint8);
  return detection_status;
}

bool AT42QT2120::calibrating()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.calibrate;
}

bool AT42QT2120::overflow()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.overflow;
}

bool AT42QT2120::sliderOrWheelTouched()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.any_slider_wheel_channels;
}

bool AT42QT2120::anyKeyTouched()
{
  DetectionStatus detection_status = getDetectionStatus();
  return detection_status.fields.any_keys;
}

AT42QT2120::KeyStatus AT42QT2120::getKeyStatus()
{
  KeyStatus key_status;
  read(RegisterAddresses::KEY_STATUS,key_status.uint16);
  return key_status;
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

