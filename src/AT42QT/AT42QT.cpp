// ----------------------------------------------------------------------------
// AT42QT.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "AT42QT.h"


AT42QT::AT42QT(TwoWire & wire,
  uint8_t device_address,
  uint8_t device_id,
  int8_t reset_pin) :
wire_ptr_(&wire),
device_address_(device_address),
device_id_(device_id),
reset_pin_(reset_pin)
{
  if(reset_pin_ >= 0)
  {
    pinMode(reset_pin_, OUTPUT);
    digitalWrite(reset_pin_, HIGH);
  }
}

void AT42QT::begin()
{
  wire_ptr_->begin();
}

void AT42QT::hardReset()
{
  if(reset_pin_ < 0) return;
  digitalWrite(reset_pin_, LOW);
  delay(1);
  digitalWrite(reset_pin_, HIGH);
}

uint8_t AT42QT::getChipId()
{
  uint8_t chip_id;
  read(0,chip_id);
  return chip_id;
}

bool AT42QT::communicating()
{
  uint8_t chip_id = getChipId();
  return chip_id == device_id_;
}

