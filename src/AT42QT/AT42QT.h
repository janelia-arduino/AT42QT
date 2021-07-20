// ----------------------------------------------------------------------------
// AT42QT.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT_H
#define AT42QT_H
#include <Wire.h>


class AT42QT
{
public:
  AT42QT(TwoWire & wire,
    uint8_t device_address) :
  wire_ptr_(&wire),
  device_address_(device_address)
  {}
;

private:
  TwoWire * wire_ptr_;
  uint8_t device_address_;

  const static uint8_t BITS_PER_BYTE = 8;
  const static uint8_t BYTE_MAX = 0xFF;

  template<typename T>
  void write(uint8_t register_address,
    T data)
  {
    int byte_count = sizeof(data);
    wire_ptr_->beginTransmission(device_address_);
    wire_ptr_->write(register_address);
    for (int byte_n=0; byte_n<byte_count; ++byte_n)
    {
      wire_ptr_->write((data >> (BITS_PER_BYTE * byte_n)) & BYTE_MAX);
    }
    wire_ptr_->endTransmission();
  }

  template<typename T>
  void read(uint8_t register_address,
    T & data)
  {
    int byte_count = sizeof(data);
    wire_ptr_->beginTransmission(device_address_);
    wire_ptr_->write(register_address);
    wire_ptr_->endTransmission(false);

    wire_ptr_->requestFrom(device_address_,byte_count);
    data = 0;
    for (int byte_n=0; byte_n<byte_count; ++byte_n)
    {
      data |= (wire_ptr_->read()) << (BITS_PER_BYTE * byte_n);
    }
  }
};
#endif
