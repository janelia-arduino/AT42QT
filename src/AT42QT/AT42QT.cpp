// ----------------------------------------------------------------------------
// AT42QT.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT.h"


AT42QT::AT42QT(uint8_t device_address,
  uint8_t chip_id,
  TwoWire & wire,
  int8_t change_pin,
  int8_t reset_pin) :
device_address_(device_address),
chip_id_(chip_id),
wire_ptr_(&wire),
change_pin_(change_pin),
reset_pin_(reset_pin)
{
}

void AT42QT::begin()
{
  if(change_pin_ >= 0)
  {
    pinMode(change_pin_,INPUT_PULLUP);
  }

  if(reset_pin_ >= 0)
  {
    pinMode(reset_pin_,OUTPUT);
    digitalWrite(reset_pin_,HIGH);
  }

  wire_ptr_->begin();
}

void AT42QT::hardReset()
{
  if(reset_pin_ < 0) return;
  digitalWrite(reset_pin_,LOW);
  delay(1);
  digitalWrite(reset_pin_,HIGH);
}

uint8_t AT42QT::getChipId()
{
  uint8_t chip_id;
  read(REGISTER_ADDRESS_CHIP_ID,chip_id);
  return chip_id;
}

bool AT42QT::communicating()
{
  uint8_t chip_id = getChipId();
  return chip_id == chip_id_;
}

void AT42QT::attachChangeCallback(const Callback callback)
{
  if(change_pin_ >= 0)
  {
    attachInterrupt(digitalPinToInterrupt(change_pin_),callback,FALLING);
  }
}

