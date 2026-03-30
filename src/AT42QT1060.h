// ----------------------------------------------------------------------------
// AT42QT1060.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#ifndef AT42QT1060_H
#define AT42QT1060_H

#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"

class AT42QT1060 : public AT42QT<RegisterAddresses::AT42QT1060> {
public:
  static constexpr uint8_t DEVICE_ADDRESS = 0x12;
  static constexpr uint8_t CHIP_ID = 0x31;
  static constexpr uint8_t KEY_COUNT = 6;

  AT42QT1060(TwoWire &wire = Wire, int8_t change_pin = -1,
             int8_t reset_pin = -1)
      : AT42QT<RegisterAddresses::AT42QT1060>(
            DEVICE_ADDRESS, CHIP_ID, KEY_COUNT, wire, change_pin, reset_pin) {}

  struct Status {
    uint16_t bytes{0};
    at42qt::bits::FieldRef<uint16_t, 0, 6> keys;
    at42qt::bits::BitRef<uint16_t, 7> calibrating;
    at42qt::bits::FieldRef<uint16_t, 8, 7> inputs;

    Status() : keys(bytes), calibrating(bytes), inputs(bytes) {}
    explicit Status(uint16_t raw)
        : bytes(raw), keys(bytes), calibrating(bytes), inputs(bytes) {}
    Status(const Status &other)
        : bytes(other.bytes), keys(bytes), calibrating(bytes), inputs(bytes) {}
    Status &operator=(const Status &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t STATUS_SIZE = 2;

  struct IOMask {
    uint8_t bytes{0};
    at42qt::bits::BitRef<uint8_t, 0> io_0;
    at42qt::bits::BitRef<uint8_t, 1> io_1;
    at42qt::bits::BitRef<uint8_t, 2> io_2;
    at42qt::bits::BitRef<uint8_t, 3> io_3;
    at42qt::bits::BitRef<uint8_t, 4> io_4;
    at42qt::bits::BitRef<uint8_t, 5> io_5;
    at42qt::bits::BitRef<uint8_t, 6> io_6;

    IOMask()
        : io_0(bytes), io_1(bytes), io_2(bytes), io_3(bytes), io_4(bytes),
          io_5(bytes), io_6(bytes) {}
    explicit IOMask(uint8_t raw)
        : bytes(raw), io_0(bytes), io_1(bytes), io_2(bytes), io_3(bytes),
          io_4(bytes), io_5(bytes), io_6(bytes) {}
    IOMask(const IOMask &other)
        : bytes(other.bytes), io_0(bytes), io_1(bytes), io_2(bytes),
          io_3(bytes), io_4(bytes), io_5(bytes), io_6(bytes) {}
    IOMask &operator=(const IOMask &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t IO_OUTPUT = 1;
  static constexpr uint8_t IO_INPUT = 0;

  struct KeyMask {
    uint8_t bytes{0};
    at42qt::bits::BitRef<uint8_t, 0> key_0;
    at42qt::bits::BitRef<uint8_t, 1> key_1;
    at42qt::bits::BitRef<uint8_t, 2> key_2;
    at42qt::bits::BitRef<uint8_t, 3> key_3;
    at42qt::bits::BitRef<uint8_t, 4> key_4;
    at42qt::bits::BitRef<uint8_t, 5> key_5;
    at42qt::bits::BitRef<uint8_t, 7> calibrating;

    KeyMask()
        : key_0(bytes), key_1(bytes), key_2(bytes), key_3(bytes), key_4(bytes),
          key_5(bytes), calibrating(bytes) {}
    explicit KeyMask(uint8_t raw)
        : bytes(raw), key_0(bytes), key_1(bytes), key_2(bytes), key_3(bytes),
          key_4(bytes), key_5(bytes), calibrating(bytes) {}
    KeyMask(const KeyMask &other)
        : bytes(other.bytes), key_0(bytes), key_1(bytes), key_2(bytes),
          key_3(bytes), key_4(bytes), key_5(bytes), calibrating(bytes) {}
    KeyMask &operator=(const KeyMask &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t ENABLES_CHANGE_TRANSITION = 1;
  static constexpr uint8_t DISABLES_CHANGE_TRANSITION = 0;

  struct AdjacentKeySuppressionMask {
    uint8_t bytes{0};
    at42qt::bits::BitRef<uint8_t, 0> key_0;
    at42qt::bits::BitRef<uint8_t, 1> key_1;
    at42qt::bits::BitRef<uint8_t, 2> key_2;
    at42qt::bits::BitRef<uint8_t, 3> key_3;
    at42qt::bits::BitRef<uint8_t, 4> key_4;
    at42qt::bits::BitRef<uint8_t, 5> key_5;

    AdjacentKeySuppressionMask()
        : key_0(bytes), key_1(bytes), key_2(bytes), key_3(bytes), key_4(bytes),
          key_5(bytes) {}
    explicit AdjacentKeySuppressionMask(uint8_t raw)
        : bytes(raw), key_0(bytes), key_1(bytes), key_2(bytes), key_3(bytes),
          key_4(bytes), key_5(bytes) {}
    AdjacentKeySuppressionMask(const AdjacentKeySuppressionMask &other)
        : bytes(other.bytes), key_0(bytes), key_1(bytes), key_2(bytes),
          key_3(bytes), key_4(bytes), key_5(bytes) {}
    AdjacentKeySuppressionMask &
    operator=(const AdjacentKeySuppressionMask &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t INCLUDE_IN_ADJACENT_KEY_SUPPRESSION = 1;
  static constexpr uint8_t EXCLUDE_FROM_ADJACENT_KEY_SUPPRESSION = 0;

  struct PWMMask {
    uint8_t bytes{0};
    at42qt::bits::BitRef<uint8_t, 0> io_0;
    at42qt::bits::BitRef<uint8_t, 1> io_1;
    at42qt::bits::BitRef<uint8_t, 2> io_2;
    at42qt::bits::BitRef<uint8_t, 3> io_3;
    at42qt::bits::BitRef<uint8_t, 4> io_4;
    at42qt::bits::BitRef<uint8_t, 5> io_5;
    at42qt::bits::BitRef<uint8_t, 6> io_6;

    PWMMask()
        : io_0(bytes), io_1(bytes), io_2(bytes), io_3(bytes), io_4(bytes),
          io_5(bytes), io_6(bytes) {}
    explicit PWMMask(uint8_t raw)
        : bytes(raw), io_0(bytes), io_1(bytes), io_2(bytes), io_3(bytes),
          io_4(bytes), io_5(bytes), io_6(bytes) {}
    PWMMask(const PWMMask &other)
        : bytes(other.bytes), io_0(bytes), io_1(bytes), io_2(bytes),
          io_3(bytes), io_4(bytes), io_5(bytes), io_6(bytes) {}
    PWMMask &operator=(const PWMMask &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t ENABLE_PWM = 1;
  static constexpr uint8_t DISABLE_PWM = 0;

  struct DetectionMask {
    uint8_t bytes{0};
    at42qt::bits::BitRef<uint8_t, 0> io_0;
    at42qt::bits::BitRef<uint8_t, 1> io_1;
    at42qt::bits::BitRef<uint8_t, 2> io_2;
    at42qt::bits::BitRef<uint8_t, 3> io_3;
    at42qt::bits::BitRef<uint8_t, 4> io_4;
    at42qt::bits::BitRef<uint8_t, 5> io_5;
    at42qt::bits::BitRef<uint8_t, 6> io_6;

    DetectionMask()
        : io_0(bytes), io_1(bytes), io_2(bytes), io_3(bytes), io_4(bytes),
          io_5(bytes), io_6(bytes) {}
    explicit DetectionMask(uint8_t raw)
        : bytes(raw), io_0(bytes), io_1(bytes), io_2(bytes), io_3(bytes),
          io_4(bytes), io_5(bytes), io_6(bytes) {}
    DetectionMask(const DetectionMask &other)
        : bytes(other.bytes), io_0(bytes), io_1(bytes), io_2(bytes),
          io_3(bytes), io_4(bytes), io_5(bytes), io_6(bytes) {}
    DetectionMask &operator=(const DetectionMask &other) {
      bytes = other.bytes;
      return *this;
    }
  };
  static constexpr uint8_t ENABLE_DETECTION = 1;
  static constexpr uint8_t DISABLE_DETECTION = 0;

  Status getStatus();
  bool calibrating();
  bool anyTouched(Status status);
  bool touched(Status status, uint8_t key);

  static constexpr uint8_t ENABLE_RELATIVE_DRIFT_COMPENSATION = 0;
  static constexpr uint8_t DISABLE_RELATIVE_DRIFT_COMPENSATION = 1;
  void enableRelativeDriftCompensation();
  void disableRelativeDriftCompensation();

  uint8_t getRecalibrationDelay();
  void setRecalibrationDelay(uint8_t rd);
  uint8_t getKeyDetectThreshold(uint8_t key);
  void setKeyDetectThreshold(uint8_t key, uint8_t threshold);
  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);

  IOMask getIOMask();
  void setIOMask(IOMask mask);
  KeyMask getKeyMask();
  void setKeyMask(KeyMask mask);
  AdjacentKeySuppressionMask getAdjacentKeySuppressionMask();
  void setAdjacentKeySuppressionMask(AdjacentKeySuppressionMask mask);
  PWMMask getPWMMask();
  void setPWMMask(PWMMask mask);
  DetectionMask getDetectionMask();
  void setDetectionMask(DetectionMask mask);

  uint8_t getActiveLevelMask();
  void setActiveLevelMask(uint8_t mask);
  uint8_t getUserOutputBuffer();
  void setUserOutputBuffer(uint8_t value);
  uint8_t getDetectionIntegrator();
  void setDetectionIntegrator(uint8_t value);
  uint8_t getPWMLevel();
  void setPWMLevel(uint8_t value);
  uint16_t getKeySignal(uint8_t key);
  uint16_t getKeyReference(uint8_t key);
};

#endif
