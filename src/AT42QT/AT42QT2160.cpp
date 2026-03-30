// ----------------------------------------------------------------------------
// AT42QT2160.cpp
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#include "AT42QT2160.h"

AT42QT2160::Status AT42QT2160::getStatus() {
  return Status(
      read<uint64_t>(RegisterAddresses::AT42QT2160::STATUS, STATUS_SIZE));
}

bool AT42QT2160::anyTouched(Status status) { return status.keys != 0; }

bool AT42QT2160::touched(Status status, uint8_t key) {
  return key < KEY_COUNT ? bitRead(status.keys, key) : false;
}

uint8_t AT42QT2160::getMeasurementIntervalCount() {
  return read<uint8_t>(RegisterAddresses::AT42QT2160::LOW_POWER_MODE);
}

void AT42QT2160::setMeasurementIntervalCount(uint8_t interval_count) {
  write(RegisterAddresses::AT42QT2160::LOW_POWER_MODE, interval_count);
}

uint8_t AT42QT2160::getBurstRepetition() {
  return read<uint8_t>(RegisterAddresses::AT42QT2160::BURST_REPETITION);
}

void AT42QT2160::setBurstRepetition(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::BURST_REPETITION, value);
}

uint8_t AT42QT2160::getTowardsDriftCompensationDuration() {
  return read<uint8_t>(
      RegisterAddresses::AT42QT2160::TOWARDS_DRIFT_COMPENSATION_DURATION);
}

void AT42QT2160::setTowardsDriftCompensationDuration(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::TOWARDS_DRIFT_COMPENSATION_DURATION,
        value);
}

uint8_t AT42QT2160::getAwayDriftCompensationDuration() {
  return read<uint8_t>(
      RegisterAddresses::AT42QT2160::AWAY_DRIFT_COMPENSATION_DURATION);
}

void AT42QT2160::setAwayDriftCompensationDuration(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::AWAY_DRIFT_COMPENSATION_DURATION, value);
}

uint8_t AT42QT2160::getDetectionIntegrator() {
  return read<uint8_t>(RegisterAddresses::AT42QT2160::DETECTION_INTEGRATOR);
}

void AT42QT2160::setDetectionIntegrator(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::DETECTION_INTEGRATOR, value);
}

uint8_t AT42QT2160::getRecalibrationDelay() {
  return read<uint8_t>(RegisterAddresses::AT42QT2160::RECALIBRATION_DELAY);
}

void AT42QT2160::setRecalibrationDelay(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::RECALIBRATION_DELAY, value);
}

uint8_t AT42QT2160::getDriftCompensationHoldDuration() {
  return read<uint8_t>(
      RegisterAddresses::AT42QT2160::DRIFT_COMPENSATION_HOLD_DURATION);
}

void AT42QT2160::setDriftCompensationHoldDuration(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::DRIFT_COMPENSATION_HOLD_DURATION, value);
}

uint8_t AT42QT2160::getKeyDetectThreshold(uint8_t key) {
  return readKey<uint8_t>(RegisterAddresses::AT42QT2160::KEY_DETECT_THRESHOLD,
                          key);
}

void AT42QT2160::setKeyDetectThreshold(uint8_t key, uint8_t value) {
  writeKey(RegisterAddresses::AT42QT2160::KEY_DETECT_THRESHOLD, key, value);
}

uint8_t AT42QT2160::getKeyBurstLength(uint8_t key) {
  return readKey<uint8_t>(RegisterAddresses::AT42QT2160::KEY_BURST_LENGTH, key);
}

void AT42QT2160::setKeyBurstLength(uint8_t key, uint8_t value) {
  writeKey(RegisterAddresses::AT42QT2160::KEY_BURST_LENGTH, key, value);
}

uint8_t AT42QT2160::getGPIOWake() {
  return read<uint8_t>(RegisterAddresses::AT42QT2160::GPIO_WAKE);
}

void AT42QT2160::setGPIOWake(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::GPIO_WAKE, value);
}

uint8_t AT42QT2160::getCommonChangeKeys() {
  return read<uint8_t>(RegisterAddresses::AT42QT2160::COMMON_CHANGE_KEYS);
}

void AT42QT2160::setCommonChangeKeys(uint8_t value) {
  write(RegisterAddresses::AT42QT2160::COMMON_CHANGE_KEYS, value);
}

uint16_t AT42QT2160::getKeySignal(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT2160::KEY_SIGNAL, key);
}

uint16_t AT42QT2160::getKeyReference(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT2160::KEY_REFERENCE, key);
}
