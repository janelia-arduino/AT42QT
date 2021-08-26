// ----------------------------------------------------------------------------
// AT42QT2120.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT2120_H
#define AT42QT2120_H
#include "AT42QT/AT42QT.h"


const uint8_t DEVICE_ADDRESS = 0x1C;
const uint8_t DEVICE_ID = 0x3E;

class AT42QT2120 : public AT42QT
{
public:
  AT42QT2120(TwoWire & wire=Wire,
    int8_t reset_pin=-1) :
  AT42QT(wire,DEVICE_ADDRESS,DEVICE_ID,reset_pin)
  {};
private:
};

#endif
