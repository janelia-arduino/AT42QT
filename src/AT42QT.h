// ----------------------------------------------------------------------------
// AT42QT.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#ifndef AT42QT_H
#define AT42QT_H

#include <Arduino.h>
#include <Wire.h>

#include <type_traits>

#include "AT42QT/BitHelpers.h"

namespace at42qt {
enum class I2cError : uint8_t {
  None = 0,
  NotInitialized,
  InvalidKey,
  DataTooLong,
  AddressNack,
  DataNack,
  OtherBusError,
  RequestFailed,
  ShortRead,
};

template <typename T> struct Result {
  T value{};
  I2cError error{I2cError::None};

  constexpr bool ok() const { return error == I2cError::None; }
};

template <> struct Result<void> {
  I2cError error{I2cError::None};

  constexpr bool ok() const { return error == I2cError::None; }
};
} // namespace at42qt

template <typename RegisterAddress> class AT42QT {
public:
  using I2cError = at42qt::I2cError;
  using Callback = void (*)();

  AT42QT(uint8_t device_address, uint8_t chip_id, uint8_t key_count,
         TwoWire &wire = Wire, int8_t change_pin = -1, int8_t reset_pin = -1);

  // Preferred setup path: sketches own bus configuration and call wire.begin().
  void setup(TwoWire &wire);
  // Legacy setup path for older sketches.
  void begin();

  bool initialized() const { return initialized_; }
  uint8_t keyCount() const { return key_count_; }
  uint8_t deviceAddress() const { return device_address_; }
  I2cError getLastI2cError() const { return last_i2c_error_; }

  at42qt::Result<uint8_t> getChipIdResult();
  uint8_t getChipId();
  bool communicating();

  void hardReset();
  void attachChangeCallback(Callback callback);

  at42qt::Result<void> triggerCalibrationResult();
  void triggerCalibration();
  at42qt::Result<void> resetResult();
  void reset();

protected:
  static constexpr uint8_t NONZERO_VALUE = 1;

  template <typename Data>
  at42qt::Result<void> writeResult(RegisterAddress register_address, Data data);

  template <typename Data>
  at42qt::Result<void> writeResult(RegisterAddress register_address, Data data,
                                   uint8_t data_size);

  template <typename Data>
  at42qt::Result<void> writeKeyResult(RegisterAddress base_register_address,
                                      uint8_t key, Data data);

  template <typename Data>
  at42qt::Result<Data> readResult(RegisterAddress register_address);

  template <typename Data>
  at42qt::Result<Data> readResult(RegisterAddress register_address,
                                  uint8_t data_size);

  template <typename Data>
  at42qt::Result<Data> readKeyResult(RegisterAddress base_register_address,
                                     uint8_t key);

  template <typename Data>
  void write(RegisterAddress register_address, Data data);

  template <typename Data>
  void write(RegisterAddress register_address, Data data, uint8_t data_size);

  template <typename Data>
  void writeKey(RegisterAddress base_register_address, uint8_t key, Data data);

  template <typename Data> Data read(RegisterAddress register_address);

  template <typename Data>
  Data read(RegisterAddress register_address, uint8_t data_size);

  template <typename Data>
  Data readKey(RegisterAddress base_register_address, uint8_t key);

private:
  uint8_t device_address_;
  uint8_t chip_id_;
  uint8_t key_count_;
  TwoWire *wire_ptr_;
  int8_t change_pin_;
  int8_t reset_pin_;
  bool initialized_;
  I2cError last_i2c_error_;

  void configurePins_();
  bool keyValid_(uint8_t key) const { return key < key_count_; }
  I2cError endTransmissionToError_(uint8_t status) const;

  template <typename Data>
  at42qt::Result<void> writeRegisterBlock_(uint8_t register_address_number,
                                           Data data, uint8_t data_size);

  template <typename Data>
  at42qt::Result<Data> readRegisterBlock_(uint8_t register_address_number,
                                          uint8_t data_size);
};

#include "AT42QT/AT42QTDefinitions.h"

#endif
