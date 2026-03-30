// ----------------------------------------------------------------------------
// AT42QT1060.cpp
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#include "AT42QT1060.h"

AT42QT1060::Status AT42QT1060::getStatus() {
  return Status(
      read<uint16_t>(RegisterAddresses::AT42QT1060::STATUS, STATUS_SIZE));
}

bool AT42QT1060::calibrating() { return getStatus().calibrating; }

bool AT42QT1060::anyTouched(Status status) { return status.keys != 0; }

bool AT42QT1060::touched(Status status, uint8_t key) {
  return key < KEY_COUNT ? bitRead(status.keys, key) : false;
}

void AT42QT1060::enableRelativeDriftCompensation() {
  write(RegisterAddresses::AT42QT1060::DRIFT_OPTION,
        ENABLE_RELATIVE_DRIFT_COMPENSATION);
}

void AT42QT1060::disableRelativeDriftCompensation() {
  write(RegisterAddresses::AT42QT1060::DRIFT_OPTION,
        DISABLE_RELATIVE_DRIFT_COMPENSATION);
}

uint8_t AT42QT1060::getRecalibrationDelay() {
  return read<uint8_t>(RegisterAddresses::AT42QT1060::RECALIBRATION_DELAY);
}

void AT42QT1060::setRecalibrationDelay(uint8_t rd) {
  write(RegisterAddresses::AT42QT1060::RECALIBRATION_DELAY, rd);
}

uint8_t AT42QT1060::getKeyDetectThreshold(uint8_t key) {
  return readKey<uint8_t>(RegisterAddresses::AT42QT1060::KEY_DETECT_THRESHOLD,
                          key);
}

void AT42QT1060::setKeyDetectThreshold(uint8_t key, uint8_t threshold) {
  writeKey(RegisterAddresses::AT42QT1060::KEY_DETECT_THRESHOLD, key, threshold);
}

uint8_t AT42QT1060::getMeasurementIntervalCount() {
  return read<uint8_t>(RegisterAddresses::AT42QT1060::LOW_POWER_MODE);
}

void AT42QT1060::setMeasurementIntervalCount(uint8_t interval_count) {
  write(RegisterAddresses::AT42QT1060::LOW_POWER_MODE, interval_count);
}

AT42QT1060::IOMask AT42QT1060::getIOMask() {
  return IOMask(read<uint8_t>(RegisterAddresses::AT42QT1060::IO_MASK));
}

void AT42QT1060::setIOMask(IOMask mask) {
  write(RegisterAddresses::AT42QT1060::IO_MASK, mask.bytes);
}

AT42QT1060::KeyMask AT42QT1060::getKeyMask() {
  return KeyMask(read<uint8_t>(RegisterAddresses::AT42QT1060::KEY_MASK));
}

void AT42QT1060::setKeyMask(KeyMask mask) {
  write(RegisterAddresses::AT42QT1060::KEY_MASK, mask.bytes);
}

AT42QT1060::AdjacentKeySuppressionMask
AT42QT1060::getAdjacentKeySuppressionMask() {
  return AdjacentKeySuppressionMask(read<uint8_t>(
      RegisterAddresses::AT42QT1060::ADJACENT_KEY_SUPPRESSION_MASK));
}

void AT42QT1060::setAdjacentKeySuppressionMask(
    AdjacentKeySuppressionMask mask) {
  write(RegisterAddresses::AT42QT1060::ADJACENT_KEY_SUPPRESSION_MASK,
        mask.bytes);
}

AT42QT1060::PWMMask AT42QT1060::getPWMMask() {
  return PWMMask(read<uint8_t>(RegisterAddresses::AT42QT1060::PWM_MASK));
}

void AT42QT1060::setPWMMask(PWMMask mask) {
  write(RegisterAddresses::AT42QT1060::PWM_MASK, mask.bytes);
}

AT42QT1060::DetectionMask AT42QT1060::getDetectionMask() {
  return DetectionMask(
      read<uint8_t>(RegisterAddresses::AT42QT1060::DETECTION_MASK));
}

void AT42QT1060::setDetectionMask(DetectionMask mask) {
  write(RegisterAddresses::AT42QT1060::DETECTION_MASK, mask.bytes);
}

uint8_t AT42QT1060::getActiveLevelMask() {
  return read<uint8_t>(RegisterAddresses::AT42QT1060::ACTIVE_LEVEL_MASK);
}

void AT42QT1060::setActiveLevelMask(uint8_t mask) {
  write(RegisterAddresses::AT42QT1060::ACTIVE_LEVEL_MASK, mask);
}

uint8_t AT42QT1060::getUserOutputBuffer() {
  return read<uint8_t>(RegisterAddresses::AT42QT1060::USER_OUTPUT_BUFFER);
}

void AT42QT1060::setUserOutputBuffer(uint8_t value) {
  write(RegisterAddresses::AT42QT1060::USER_OUTPUT_BUFFER, value);
}

uint8_t AT42QT1060::getDetectionIntegrator() {
  return read<uint8_t>(RegisterAddresses::AT42QT1060::DETECTION_INTEGRATOR);
}

void AT42QT1060::setDetectionIntegrator(uint8_t value) {
  write(RegisterAddresses::AT42QT1060::DETECTION_INTEGRATOR, value);
}

uint8_t AT42QT1060::getPWMLevel() {
  return read<uint8_t>(RegisterAddresses::AT42QT1060::PWM_LEVEL);
}

void AT42QT1060::setPWMLevel(uint8_t value) {
  write(RegisterAddresses::AT42QT1060::PWM_LEVEL, value);
}

uint16_t AT42QT1060::getKeySignal(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT1060::KEY_SIGNAL, key);
}

uint16_t AT42QT1060::getKeyReference(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT1060::KEY_REFERENCE, key);
}
