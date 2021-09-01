// ----------------------------------------------------------------------------
// AT42QT1070.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../AT42QT1070.h"


AT42QT1070::Status AT42QT1070::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT1070::DETECTION_STATUS,status.bytes,STATUS_SIZE);
  return status;
}

bool AT42QT1070::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}
