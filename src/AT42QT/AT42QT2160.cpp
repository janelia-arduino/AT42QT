// ----------------------------------------------------------------------------
// AT42QT2160.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT2160.h"


AT42QT2160::Status AT42QT2160::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT2160::GENERAL_STATUS,status.bytes);
  return status;
}
