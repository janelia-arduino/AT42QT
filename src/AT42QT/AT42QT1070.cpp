// ----------------------------------------------------------------------------
// AT42QT1070.cpp
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#include "AT42QT1070.h"

AT42QT1070::Status AT42QT1070::getStatus() {
  return Status(
      read<uint16_t>(RegisterAddresses::AT42QT1070::STATUS, STATUS_SIZE));
}

bool AT42QT1070::calibrating() { return getStatus().calibrating; }

bool AT42QT1070::anyTouched(Status status) { return status.keys != 0; }

bool AT42QT1070::touched(Status status, uint8_t key) {
  return key < KEY_COUNT ? bitRead(status.keys, key) : false;
}

uint16_t AT42QT1070::getKeySignal(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT1070::KEY_SIGNAL, key);
}

uint16_t AT42QT1070::getKeyReference(uint8_t key) {
  return readKey<uint16_t>(RegisterAddresses::AT42QT1070::KEY_REFERENCE, key);
}

uint8_t AT42QT1070::getKeyDetectThreshold(uint8_t key) {
  return readKey<uint8_t>(RegisterAddresses::AT42QT1070::KEY_DETECT_THRESHOLD,
                          key);
}

void AT42QT1070::setKeyDetectThreshold(uint8_t key, uint8_t threshold) {
  writeKey(RegisterAddresses::AT42QT1070::KEY_DETECT_THRESHOLD, key, threshold);
}

uint8_t AT42QT1070::getKeyAKSLevel(uint8_t key) {
  return readKey<uint8_t>(RegisterAddresses::AT42QT1070::KEY_AKS_LEVEL, key);
}

void AT42QT1070::setKeyAKSLevel(uint8_t key, uint8_t level) {
  writeKey(RegisterAddresses::AT42QT1070::KEY_AKS_LEVEL, key, level);
}

uint8_t AT42QT1070::getKeyDetectIntegrator(uint8_t key) {
  return readKey<uint8_t>(RegisterAddresses::AT42QT1070::KEY_DETECT_INTEGRATOR,
                          key);
}

void AT42QT1070::setKeyDetectIntegrator(uint8_t key, uint8_t integrator) {
  writeKey(RegisterAddresses::AT42QT1070::KEY_DETECT_INTEGRATOR, key,
           integrator);
}

uint8_t AT42QT1070::getFastOutMaxCalGuardChannel() {
  return read<uint8_t>(RegisterAddresses::AT42QT1070::FO_MAX_CAL_GUARD_CHANNEL);
}

void AT42QT1070::setFastOutMaxCalGuardChannel(uint8_t value) {
  write(RegisterAddresses::AT42QT1070::FO_MAX_CAL_GUARD_CHANNEL, value);
}

uint8_t AT42QT1070::getMeasurementIntervalCount() {
  return read<uint8_t>(RegisterAddresses::AT42QT1070::LOW_POWER_MODE);
}

void AT42QT1070::setMeasurementIntervalCount(uint8_t interval_count) {
  write(RegisterAddresses::AT42QT1070::LOW_POWER_MODE, interval_count);
}

uint8_t AT42QT1070::getMaxOnDuration() {
  return read<uint8_t>(RegisterAddresses::AT42QT1070::MAX_ON_DURATION);
}

void AT42QT1070::setMaxOnDuration(uint8_t value) {
  write(RegisterAddresses::AT42QT1070::MAX_ON_DURATION, value);
}
