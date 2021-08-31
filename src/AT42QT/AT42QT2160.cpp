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

AT42QT2160::GeneralStatus AT42QT2160::getGeneralStatus()
{
  GeneralStatus general_status;
  read(RegisterAddresses::GENERAL_STATUS,general_status.uint8);
  return general_status;
}

bool AT42QT2160::sliderTouched()
{
  GeneralStatus general_status = getGeneralStatus();
  return general_status.fields.slider;
}

bool AT42QT2160::commonChange()
{
  GeneralStatus general_status = getGeneralStatus();
  return general_status.fields.common_change;
}

bool AT42QT2160::cycleOverrun()
{
  GeneralStatus general_status = getGeneralStatus();
  return general_status.fields.cycle_overrun;
}

bool AT42QT2160::wasReset()
{
  GeneralStatus general_status = getGeneralStatus();
  return general_status.fields.reset;
}

AT42QT2160::KeyStatus AT42QT2160::getKeyStatus()
{
  KeyStatus key_status;
  read(RegisterAddresses::KEY_STATUS,key_status.uint16);
  return key_status;
}

bool AT42QT2160::anyKeyTouched()
{
  KeyStatus key_status = getKeyStatus();
  return key_status.uint16;
}

uint8_t AT42QT2160::getSliderPosition()
{
  uint8_t slider_position;
  read(RegisterAddresses::SLIDER_POSITION,slider_position);
  return slider_position;
}

void AT42QT2160::triggerCalibration()
{
  write(RegisterAddresses::CALIBRATE,NONZERO_VALUE);
}

void AT42QT2160::reset()
{
  write(RegisterAddresses::RESET,NONZERO_VALUE);
}

