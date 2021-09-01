// ----------------------------------------------------------------------------
// AT42QT2120.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT2120_H
#define AT42QT2120_H
#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"


class AT42QT2120 : public AT42QT<RegisterAddresses::AT42QT2120>
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x1C;
  static const uint8_t CHIP_ID = 0x3E;

  AT42QT2120(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1) :
  AT42QT<RegisterAddresses::AT42QT2120>(DEVICE_ADDRESS,CHIP_ID,wire,change_pin,reset_pin)
  {
  }

  union Status
  {
    struct
    {
      uint32_t any_key : 1;
      uint32_t slider : 1;
      uint32_t space0 : 4;
      uint32_t overflow : 1;
      uint32_t calibrating : 1;
      uint32_t keys: 12;
      uint32_t space1 : 12;
    };
    uint32_t bytes;
  };
  Status getStatus();
  bool calibrating();

  uint8_t getSliderPosition();

  // each interval is 16ms
  // an interval_count of 4 equals 16ms*4=64ms between measurements
  // power down device by writing a zero interval_count
  // wake device by resetting or writing a nonzero interval_count
  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);
};

#endif
