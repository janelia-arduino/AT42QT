// ----------------------------------------------------------------------------
// AT42QT2160.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AT42QT2160.h"


AT42QT2160::Status AT42QT2160::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT2160::STATUS,status.bytes);
  return status;
}
