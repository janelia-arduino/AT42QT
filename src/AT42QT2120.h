// ----------------------------------------------------------------------------
// AT42QT2120.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#ifndef AT42QT2120_H
#define AT42QT2120_H

#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"

class AT42QT2120 : public AT42QT<RegisterAddresses::AT42QT2120> {
public:
  static constexpr uint8_t DEVICE_ADDRESS = 0x1C;
  static constexpr uint8_t CHIP_ID = 0x3E;
  static constexpr uint8_t KEY_COUNT = 12;

  AT42QT2120(TwoWire &wire = Wire, int8_t change_pin = -1,
             int8_t reset_pin = -1)
      : AT42QT<RegisterAddresses::AT42QT2120>(
            DEVICE_ADDRESS, CHIP_ID, KEY_COUNT, wire, change_pin, reset_pin) {}

  struct Status {
    uint32_t bytes{0};
    bool any_key_touched{false};
    bool slider_or_wheel{false};
    bool overflow{false};
    bool calibrating{false};
    uint16_t keys{0};
    uint8_t slider_or_wheel_position{0};
  };
  static constexpr uint8_t STATUS_SIZE = 4;

  struct SliderOrWheelEnable {
    uint8_t bytes{0};
    at42qt::bits::BitRef<uint8_t, 6> wheel;
    at42qt::bits::BitRef<uint8_t, 7> enable;

    SliderOrWheelEnable() : wheel(bytes), enable(bytes) {}
    explicit SliderOrWheelEnable(uint8_t raw)
        : bytes(raw), wheel(bytes), enable(bytes) {}
    SliderOrWheelEnable(const SliderOrWheelEnable &other)
        : bytes(other.bytes), wheel(bytes), enable(bytes) {}
    SliderOrWheelEnable &operator=(const SliderOrWheelEnable &other) {
      bytes = other.bytes;
      return *this;
    }
  };

  struct KeyControl {
    uint8_t bytes{0};

    bool touchEnabled() const { return (bytes & 0x01u) == 0; }
    void setTouchEnabled(bool enabled) {
      if (enabled) {
        bytes &= static_cast<uint8_t>(~0x01u);
      } else {
        bytes |= 0x01u;
      }
    }

    bool outputHigh() const { return (bytes & 0x02u) != 0; }
    void setOutputHigh(bool high) {
      if (high) {
        bytes |= 0x02u;
      } else {
        bytes &= static_cast<uint8_t>(~0x02u);
      }
    }

    uint8_t adjacentKeySuppressionGroup() const {
      return static_cast<uint8_t>((bytes >> 2) & 0x03u);
    }
    void setAdjacentKeySuppressionGroup(uint8_t group) {
      bytes = static_cast<uint8_t>((bytes & ~0x0Cu) | ((group & 0x03u) << 2));
    }

    bool guardEnabled() const { return (bytes & 0x10u) != 0; }
    void setGuardEnabled(bool enabled) {
      if (enabled) {
        bytes |= 0x10u;
      } else {
        bytes &= static_cast<uint8_t>(~0x10u);
      }
    }
  };

  struct KeyPulseScale {
    uint8_t bytes{0};
    at42qt::bits::FieldRef<uint8_t, 0, 4> scale;
    at42qt::bits::FieldRef<uint8_t, 4, 4> pulse;

    KeyPulseScale() : scale(bytes), pulse(bytes) {}
    explicit KeyPulseScale(uint8_t raw)
        : bytes(raw), scale(bytes), pulse(bytes) {}
    KeyPulseScale(const KeyPulseScale &other)
        : bytes(other.bytes), scale(bytes), pulse(bytes) {}
    KeyPulseScale &operator=(const KeyPulseScale &other) {
      bytes = other.bytes;
      return *this;
    }
  };

  static constexpr uint8_t DRIFT_COMPENSATION_DURATION_MAX = 127;
  static constexpr uint8_t DETECTION_INTEGRATOR_MAX = 32;

  Status getStatus();
  bool calibrating();
  bool anyTouched(Status status);
  bool touched(Status status, uint8_t key);

  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);
  uint8_t getTowardsDriftCompensationDuration();
  void setTowardsDriftCompensationDuration(uint8_t tdcd);
  uint8_t getAwayDriftCompensationDuration();
  void setAwayDriftCompensationDuration(uint8_t adcd);
  uint8_t getDetectionIntegrator();
  void setDetectionIntegrator(uint8_t di);
  uint8_t getRecalibrationDelay();
  void setRecalibrationDelay(uint8_t rd);
  uint8_t getDriftCompensationHoldDuration();
  void setDriftCompensationHoldDuration(uint8_t dchd);
  bool sliderOrWheelEnabled();
  void disableSliderAndWheel();
  void enableSlider();
  void enableWheel();
  uint8_t getChargeDuration();
  void setChargeDuration(uint8_t cd);
  uint8_t getKeyDetectThreshold(uint8_t key);
  void setKeyDetectThreshold(uint8_t key, uint8_t threshold);
  KeyControl getKeyControl(uint8_t key);
  void setKeyControl(uint8_t key, KeyControl key_control);
  KeyPulseScale getKeyPulseScale(uint8_t key);
  void setKeyPulseScale(uint8_t key, KeyPulseScale key_pulse_scale);
  uint16_t getKeySignal(uint8_t key);
  uint16_t getKeyReference(uint8_t key);
};

#endif
