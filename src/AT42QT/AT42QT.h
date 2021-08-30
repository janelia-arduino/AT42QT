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
  AT42QT(uint8_t device_address,
    uint8_t chip_id,
    TwoWire & wire,
    int8_t change_pin,
    int8_t reset_pin);

  void begin();
  void hardReset();
  uint8_t getChipId();
  bool communicating();

private:
  static const uint8_t REGISTER_ADDRESS_CHIP_ID = 0;
  const uint8_t device_address_;
  const uint8_t chip_id_;
  TwoWire * const wire_ptr_;
  const int8_t change_pin_;
  const int8_t reset_pin_;

  const static uint8_t BITS_PER_BYTE = 8;
  const static uint8_t BYTE_MAX = 0xFF;

protected:
  static const uint8_t NONZERO_VALUE = 1;

  template<typename T>
  void write(uint8_t register_address,
    const T & data)
  {
    uint8_t byte_count = sizeof(data);
    wire_ptr_->beginTransmission(device_address_);
    wire_ptr_->write(register_address);
    for (uint8_t byte_n=0; byte_n<byte_count; ++byte_n)
    {
      wire_ptr_->write((data >> (BITS_PER_BYTE * byte_n)) & BYTE_MAX);
    }
    wire_ptr_->endTransmission();
  }

  template<typename T>
  void read(uint8_t register_address,
    T & data)
  {
    uint8_t byte_count = sizeof(data);
    wire_ptr_->beginTransmission(device_address_);
    wire_ptr_->write(register_address);
    wire_ptr_->endTransmission(false);

    wire_ptr_->requestFrom(device_address_,byte_count);
    data = 0;
    for (uint8_t byte_n=0; byte_n<byte_count; ++byte_n)
    {
      data |= (wire_ptr_->read()) << (BITS_PER_BYTE * byte_n);
    }
  }
  
};
#endif
