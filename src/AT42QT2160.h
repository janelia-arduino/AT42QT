// ----------------------------------------------------------------------------
// AT42QT2160.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#ifndef AT42QT2160_H
#define AT42QT2160_H

#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"

class AT42QT2160 : public AT42QT<RegisterAddresses::AT42QT2160> {
public:
  static constexpr uint8_t DEVICE_ADDRESS = 0x0D;
  static constexpr uint8_t CHIP_ID = 0x11;
  static constexpr uint8_t KEY_COUNT = 16;

  AT42QT2160(TwoWire &wire = Wire, int8_t change_pin = -1,
             int8_t reset_pin = -1)
      : AT42QT<RegisterAddresses::AT42QT2160>(
            DEVICE_ADDRESS, CHIP_ID, KEY_COUNT, wire, change_pin, reset_pin) {}

  struct Status {
    uint64_t bytes{0};
    at42qt::bits::BitRef<uint64_t, 0> slider;
    at42qt::bits::BitRef<uint64_t, 1> common_change;
    at42qt::bits::BitRef<uint64_t, 6> cycle_overrun;
    at42qt::bits::BitRef<uint64_t, 7> was_reset;
    at42qt::bits::FieldRef<uint64_t, 8, 16> keys;
    at42qt::bits::FieldRef<uint64_t, 24, 8> slider_position;
    at42qt::bits::FieldRef<uint64_t, 32, 8> gpio;

    Status()
        : slider(bytes), common_change(bytes), cycle_overrun(bytes),
          was_reset(bytes), keys(bytes), slider_position(bytes), gpio(bytes) {}
    explicit Status(uint64_t raw)
        : bytes(raw), slider(bytes), common_change(bytes), cycle_overrun(bytes),
          was_reset(bytes), keys(bytes), slider_position(bytes), gpio(bytes) {}
    Status(const Status &other)
        : bytes(other.bytes), slider(bytes), common_change(bytes),
          cycle_overrun(bytes), was_reset(bytes), keys(bytes),
          slider_position(bytes), gpio(bytes) {}
    Status &operator=(const Status &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t STATUS_SIZE = 5;

  Status getStatus();
  bool anyTouched(Status status);
  bool touched(Status status, uint8_t key);

  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);
  uint8_t getBurstRepetition();
  void setBurstRepetition(uint8_t value);
  uint8_t getTowardsDriftCompensationDuration();
  void setTowardsDriftCompensationDuration(uint8_t value);
  uint8_t getAwayDriftCompensationDuration();
  void setAwayDriftCompensationDuration(uint8_t value);
  uint8_t getDetectionIntegrator();
  void setDetectionIntegrator(uint8_t value);
  uint8_t getRecalibrationDelay();
  void setRecalibrationDelay(uint8_t value);
  uint8_t getDriftCompensationHoldDuration();
  void setDriftCompensationHoldDuration(uint8_t value);
  uint8_t getKeyDetectThreshold(uint8_t key);
  void setKeyDetectThreshold(uint8_t key, uint8_t value);
  uint8_t getKeyBurstLength(uint8_t key);
  void setKeyBurstLength(uint8_t key, uint8_t value);
  uint8_t getGPIOWake();
  void setGPIOWake(uint8_t value);
  uint8_t getCommonChangeKeys();
  void setCommonChangeKeys(uint8_t value);
  uint16_t getKeySignal(uint8_t key);
  uint16_t getKeyReference(uint8_t key);
};

#endif
