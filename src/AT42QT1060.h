// ----------------------------------------------------------------------------
// AT42QT1060.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT1060_H
#define AT42QT1060_H
#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"


class AT42QT1060 : public AT42QT<RegisterAddresses::AT42QT1060>
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x12;
  static const uint8_t CHIP_ID = 0x31;
  static const uint8_t KEY_COUNT = 6;

  AT42QT1060(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1) :
  AT42QT<RegisterAddresses::AT42QT1060>(DEVICE_ADDRESS,
    CHIP_ID,
    KEY_COUNT,
    wire,
    change_pin,
    reset_pin)
  {
  }

  union Status
  {
    struct
    {
      uint8_t keys : 6;
      uint8_t space : 1;
      uint8_t calibrating : 1;
    };
    uint8_t bytes;
  };
  static const uint8_t STATUS_SIZE = 1;
  Status getStatus();
  bool calibrating();
};

#endif
