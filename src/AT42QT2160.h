// ----------------------------------------------------------------------------
// AT42QT2160.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT2160_H
#define AT42QT2160_H
#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"


class AT42QT2160 : public AT42QT<RegisterAddresses::AT42QT2160>
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x0D;
  static const uint8_t CHIP_ID = 0x11;

  AT42QT2160(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1) :
  AT42QT<RegisterAddresses::AT42QT2160>(DEVICE_ADDRESS,CHIP_ID,wire,change_pin,reset_pin)
  {
  }

  union Status
  {
    struct
    {
      uint32_t slider : 1;
      uint32_t common_change : 1;
      uint32_t space0 : 4;
      uint32_t cycle_overrun : 1;
      uint32_t was_reset : 1;
      uint32_t keys : 16;
      uint32_t slider_position : 8;
    };
    uint32_t bytes;
  };
  Status getStatus();
};

#endif
