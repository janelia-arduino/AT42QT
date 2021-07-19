// ----------------------------------------------------------------------------
// AT42QT.cpp
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "AT42QT.h"


AT42QT::AT42QT(TwoWire & wire,
  uint8_t device_address) :
wire_ptr_(wire),
device_address_(device_address)
{
}
