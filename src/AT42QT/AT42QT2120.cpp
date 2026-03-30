// ----------------------------------------------------------------------------
// AT42QT2120.cpp
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#include "AT42QT2120.h"

AT42QT2120::Status AT42QT2120::getStatus() {
  Status status{};
  status.bytes =
      read<uint32_t>(RegisterAddresses::AT42QT2120::STATUS, STATUS_SIZE);
  status.any_key_touched = (status.bytes & 0x01u) != 0;
  status.slider_or_wheel = (status.bytes & 0x02u) != 0;
  status.overflow = (status.bytes & 0x40u) != 0;
  status.calibrating = (status.bytes & 0x80u) != 0;
  status.keys = static_cast<uint16_t>((status.bytes >> 8) & 0x0FFFu);
  status.slider_or_wheel_position =
      static_cast<uint8_t>((status.bytes >> 24) & 0xFFu);
  return status;
}

bool AT42QT2120::calibrating() { return getStatus().calibrating; }

bool AT42QT2120::anyTouched(Status status) { return status.keys != 0; }

bool AT42QT2120::touched(Status status, uint8_t key) {
  return key < KEY_COUNT ? bitRead(status.keys, key) : false;
}

uint8_t AT42QT2120::getMeasurementIntervalCount() {
  return read<uint8_t>(RegisterAddresses::AT42QT2120::LOW_POWER_MODE);
}

void AT42QT2120::setMeasurementIntervalCount(uint8_t interval_count) {
  write(RegisterAddresses::AT42QT2120::LOW_POWER_MODE, interval_count);
}

uint8_t AT42QT2120::getTowardsDriftCompensationDuration() {
  return read<uint8_t>(
      RegisterAddresses::AT42QT2120::TOWARDS_DRIFT_COMPENSATION_DURATION);
}

void AT42QT2120::setTowardsDriftCompensationDuration(uint8_t tdcd) {
  if (tdcd > DRIFT_COMPENSATION_DURATION_MAX) {
    tdcd = DRIFT_COMPENSATION_DURATION_MAX;
  }
  write(RegisterAddresses::AT42QT2120::TOWARDS_DRIFT_COMPENSATION_DURATION,
        tdcd);
}

uint8_t AT42QT2120::getAwayDriftCompensationDuration() {
  return read<uint8_t>(
      RegisterAddresses::AT42QT2120::AWAY_DRIFT_COMPENSATION_DURATION);
}

void AT42QT2120::setAwayDriftCompensationDuration(uint8_t adcd) {
  if (adcd > DRIFT_COMPENSATION_DURATION_MAX) {
    adcd = DRIFT_COMPENSATION_DURATION_MAX;
  }
  write(RegisterAddresses::AT42QT2120::AWAY_DRIFT_COMPENSATION_DURATION, adcd);
}

uint8_t AT42QT2120::getDetectionIntegrator() {
  return read<uint8_t>(RegisterAddresses::AT42QT2120::DETECTION_INTEGRATOR);
}

void AT42QT2120::setDetectionIntegrator(uint8_t di) {
  if (di > DETECTION_INTEGRATOR_MAX) {
    di = DETECTION_INTEGRATOR_MAX;
  }
  write(RegisterAddresses::AT42QT2120::DETECTION_INTEGRATOR, di);
}

uint8_t AT42QT2120::getRecalibrationDelay() {
  return read<uint8_t>(RegisterAddresses::AT42QT2120::RECALIBRATION_DELAY);
}

void AT42QT2120::setRecalibrationDelay(uint8_t rd) {
  write(RegisterAddresses::AT42QT2120::RECALIBRATION_DELAY, rd);
}

uint8_t AT42QT2120::getDriftCompensationHoldDuration() {
  return read<uint8_t>(
      RegisterAddresses::AT42QT2120::DRIFT_COMPENSATION_HOLD_DURATION);
}

void AT42QT2120::setDriftCompensationHoldDuration(uint8_t dchd) {
  write(RegisterAddresses::AT42QT2120::DRIFT_COMPENSATION_HOLD_DURATION, dchd);
}

bool AT42QT2120::sliderOrWheelEnabled() {
  SliderOrWheelEnable swe(
      read<uint8_t>(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE));
  return swe.enable;
}

void AT42QT2120::disableSliderAndWheel() {
  SliderOrWheelEnable swe{};
  swe.wheel = 0;
  swe.enable = 0;
  write(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE, swe.bytes);
}

void AT42QT2120::enableSlider() {
  SliderOrWheelEnable swe{};
  swe.wheel = 0;
  swe.enable = 1;
  write(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE, swe.bytes);
}

void AT42QT2120::enableWheel() {
  SliderOrWheelEnable swe{};
  swe.wheel = 1;
  swe.enable = 1;
  write(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE, swe.bytes);
}

uint8_t AT42QT2120::getChargeDuration() {
  return read<uint8_t>(RegisterAddresses::AT42QT2120::CHARGE_DURATION);
}

void AT42QT2120::setChargeDuration(uint8_t cd) {
  write(RegisterAddresses::AT42QT2120::CHARGE_DURATION, cd);
}

uint8_t AT42QT2120::getKeyDetectThreshold(uint8_t key) {
  return readKey<uint8_t>(RegisterAddresses::AT42QT2120::KEY_DETECT_THRESHOLD,
                          key);
}

void AT42QT2120::setKeyDetectThreshold(uint8_t key, uint8_t threshold) {
  writeKey(RegisterAddresses::AT42QT2120::KEY_DETECT_THRESHOLD, key, threshold);
}

AT42QT2120::KeyControl AT42QT2120::getKeyControl(uint8_t key) {
  KeyControl key_control{};
  key_control.bytes =
      readKey<uint8_t>(RegisterAddresses::AT42QT2120::KEY_CONTROL, key);
  return key_control;
}

void AT42QT2120::setKeyControl(uint8_t key, KeyControl key_control) {
  writeKey(RegisterAddresses::AT42QT2120::KEY_CONTROL, key, key_control.bytes);
}

AT42QT2120::KeyPulseScale AT42QT2120::getKeyPulseScale(uint8_t key) {
  return KeyPulseScale(
      readKey<uint8_t>(RegisterAddresses::AT42QT2120::KEY_PULSE_SCALE, key));
}

void AT42QT2120::setKeyPulseScale(uint8_t key, KeyPulseScale key_pulse_scale) {
  writeKey(RegisterAddresses::AT42QT2120::KEY_PULSE_SCALE, key,
           key_pulse_scale.bytes);
}

uint16_t AT42QT2120::getKeySignal(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT2120::KEY_SIGNAL, key);
}

uint16_t AT42QT2120::getKeyReference(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT2120::KEY_REFERENCE, key);
}
