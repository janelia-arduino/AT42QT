// ----------------------------------------------------------------------------
// AT42QT1070.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT1070_H
#define AT42QT1070_H
#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"


class AT42QT1070 : public AT42QT<RegisterAddresses::AT42QT1070>
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x1B;
  static const uint8_t CHIP_ID = 0x2E;

  AT42QT1070(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1) :
  AT42QT<RegisterAddresses::AT42QT1070>(DEVICE_ADDRESS,CHIP_ID,wire,change_pin,reset_pin)
  {
  }


  union Status
  {
    struct
    {
      uint16_t any_key : 1;
      uint16_t space0 : 5;
      uint16_t overflow : 1;
      uint16_t calibrating : 1;
      uint16_t keys : 7;
      uint16_t space1 : 1;
    };
    uint16_t bytes;
  };
  Status getStatus();
  bool calibrating();
private:
};

#endif
