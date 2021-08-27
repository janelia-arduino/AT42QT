// ----------------------------------------------------------------------------
// AT42QT2120.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT2120_H
#define AT42QT2120_H
#include "AT42QT/AT42QT.h"


class AT42QT2120 : public AT42QT
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x1C;
  static const uint8_t CHIP_ID = 0x3E;

  AT42QT2120(TwoWire & wire=Wire,
    int8_t reset_pin=-1) :
  AT42QT(DEVICE_ADDRESS,CHIP_ID,wire,reset_pin)
  {};
private:
};

#endif
