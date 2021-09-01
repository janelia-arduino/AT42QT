// ----------------------------------------------------------------------------
// AT42QTDefinitions.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT_DEFINITIONS_H
#define AT42QT_DEFINITIONS_H


template<typename RegisterAddress>
AT42QT<RegisterAddress>::AT42QT(uint8_t device_address,
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

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::begin()
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

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::hardReset()
{
  if(reset_pin_ < 0) return;
  digitalWrite(reset_pin_,LOW);
  delay(1);
  digitalWrite(reset_pin_,HIGH);
}

template<typename RegisterAddress>
uint8_t AT42QT<RegisterAddress>::getChipId()
{
  uint8_t chip_id;
  read(RegisterAddress::CHIP_ID,chip_id);
  return chip_id;
}

template<typename RegisterAddress>
bool AT42QT<RegisterAddress>::communicating()
{
  uint8_t chip_id = getChipId();
  return chip_id == chip_id_;
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::attachChangeCallback(const Callback callback)
{
  if(change_pin_ >= 0)
  {
    attachInterrupt(digitalPinToInterrupt(change_pin_),callback,FALLING);
  }
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::triggerCalibration()
{
  write(RegisterAddress::CALIBRATE,NONZERO_VALUE);
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::reset()
{
  write(RegisterAddress::RESET,NONZERO_VALUE);
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::write(RegisterAddress register_address,
  const Data & data)
{
  uint8_t byte_count = sizeof(data);
  wire_ptr_->beginTransmission(device_address_);
  wire_ptr_->write(static_cast<uint8_t>(register_address));
  for (uint8_t byte_n=0; byte_n<byte_count; ++byte_n)
  {
    wire_ptr_->write((data >> (BITS_PER_BYTE * byte_n)) & BYTE_MAX);
  }
  wire_ptr_->endTransmission();
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::read(RegisterAddress register_address,
  Data & data)
{
  uint8_t byte_count = sizeof(data);
  wire_ptr_->beginTransmission(device_address_);
  wire_ptr_->write(static_cast<uint8_t>(register_address));
  wire_ptr_->endTransmission(false);

  wire_ptr_->requestFrom(device_address_,byte_count);
  data = 0;
  for (uint8_t byte_n=0; byte_n<byte_count; ++byte_n)
  {
    data |= (wire_ptr_->read()) << (BITS_PER_BYTE * byte_n);
  }
}

#endif
