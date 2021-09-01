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
  static const uint8_t KEY_COUNT = 12;

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
      uint32_t space1 : 4;
      uint32_t slider_position : 8;
    };
    uint32_t bytes;
  };
  static const uint8_t STATUS_SIZE = 4;
  Status getStatus();
  bool calibrating();

  // each interval is 16ms
  // an interval_count of 4 equals 16ms*4=64ms between measurements
  // power down device by writing a zero interval_count
  // wake device by resetting or writing a nonzero interval_count
  // default = 1
  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);

  // increments of 0.16s
  // should be more than four times the measurement interval count
  // default = 20
  static const uint8_t DRIFT_DURATION_MAX = 127;
  uint8_t getTowardsDriftDuration();
  void setTowardsDriftDuration(uint8_t tdd);

  // increments of 0.16s
  // should be more than four times the measurement interval count
  // default = 5
  uint8_t getAwayDriftDuration();
  void setAwayDriftDuration(uint8_t add);

  // number of consecutive measurements that must be confirmed as having passed
  // the key threshold value before that key is registered as being in detect
  // default = 4
  static const uint8_t DETECTION_INTEGRATOR_MAX = 32;
  uint8_t getDetectionIntegrator();
  void setDetectionIntegrator(uint8_t di);

  // increments of 0.16s
  // automatic recalibration delay after extended unintentional touch
  // default = 255
  uint8_t getRecalibrationDelay();
  void setRecalibrationDelay(uint8_t rd);

  // increments of 0.16s
  // default = 25
  uint8_t getDriftHoldDuration();
  void setDriftHoldDuration(uint8_t dhd);
};

#endif
