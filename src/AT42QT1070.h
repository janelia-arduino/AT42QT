// ----------------------------------------------------------------------------
// AT42QT1070.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#ifndef AT42QT1070_H
#define AT42QT1070_H

#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"

class AT42QT1070 : public AT42QT<RegisterAddresses::AT42QT1070> {
public:
  static constexpr uint8_t DEVICE_ADDRESS = 0x1B;
  static constexpr uint8_t CHIP_ID = 0x2E;
  static constexpr uint8_t KEY_COUNT = 7;

  AT42QT1070(TwoWire &wire = Wire, int8_t change_pin = -1,
             int8_t reset_pin = -1)
      : AT42QT<RegisterAddresses::AT42QT1070>(
            DEVICE_ADDRESS, CHIP_ID, KEY_COUNT, wire, change_pin, reset_pin) {}

  struct Status {
    uint16_t bytes{0};
    at42qt::bits::BitRef<uint16_t, 0> any_key;
    at42qt::bits::BitRef<uint16_t, 6> overflow;
    at42qt::bits::BitRef<uint16_t, 7> calibrating;
    at42qt::bits::FieldRef<uint16_t, 8, 7> keys;

    Status()
        : any_key(bytes), overflow(bytes), calibrating(bytes), keys(bytes) {}
    explicit Status(uint16_t raw)
        : bytes(raw), any_key(bytes), overflow(bytes), calibrating(bytes),
          keys(bytes) {}
    Status(const Status &other)
        : bytes(other.bytes), any_key(bytes), overflow(bytes),
          calibrating(bytes), keys(bytes) {}
    Status &operator=(const Status &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t STATUS_SIZE = 2;

  Status getStatus();
  bool calibrating();
  bool anyTouched(Status status);
  bool touched(Status status, uint8_t key);

  uint16_t getKeySignal(uint8_t key);
  uint16_t getKeyReference(uint8_t key);
  uint8_t getKeyDetectThreshold(uint8_t key);
  void setKeyDetectThreshold(uint8_t key, uint8_t threshold);
  uint8_t getKeyAKSLevel(uint8_t key);
  void setKeyAKSLevel(uint8_t key, uint8_t level);
  uint8_t getKeyDetectIntegrator(uint8_t key);
  void setKeyDetectIntegrator(uint8_t key, uint8_t integrator);
  uint8_t getFastOutMaxCalGuardChannel();
  void setFastOutMaxCalGuardChannel(uint8_t value);
  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);
  uint8_t getMaxOnDuration();
  void setMaxOnDuration(uint8_t value);
};

#endif
