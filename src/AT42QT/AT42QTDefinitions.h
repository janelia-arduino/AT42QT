// ----------------------------------------------------------------------------
// AT42QTDefinitions.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------

#ifndef AT42QT_DEFINITIONS_H
#define AT42QT_DEFINITIONS_H

template <typename RegisterAddress>
AT42QT<RegisterAddress>::AT42QT(uint8_t device_address, uint8_t chip_id,
                                uint8_t key_count, TwoWire &wire,
                                int8_t change_pin, int8_t reset_pin)
    : device_address_(device_address), chip_id_(chip_id), key_count_(key_count),
      wire_ptr_(&wire), change_pin_(change_pin), reset_pin_(reset_pin),
      initialized_(false), last_i2c_error_(I2cError::NotInitialized) {}

template <typename RegisterAddress>
void AT42QT<RegisterAddress>::configurePins_() {
  if (change_pin_ >= 0) {
    pinMode(change_pin_, INPUT_PULLUP);
  }

  if (reset_pin_ >= 0) {
    pinMode(reset_pin_, OUTPUT);
    digitalWrite(reset_pin_, HIGH);
  }
}

template <typename RegisterAddress>
void AT42QT<RegisterAddress>::setup(TwoWire &wire) {
  wire_ptr_ = &wire;
  configurePins_();
  initialized_ = true;
  last_i2c_error_ = I2cError::None;
}

template <typename RegisterAddress> void AT42QT<RegisterAddress>::begin() {
  wire_ptr_->begin();
  setup(*wire_ptr_);
}

template <typename RegisterAddress>
typename AT42QT<RegisterAddress>::I2cError
AT42QT<RegisterAddress>::endTransmissionToError_(uint8_t status) const {
  switch (status) {
  case 0:
    return I2cError::None;
  case 1:
    return I2cError::DataTooLong;
  case 2:
    return I2cError::AddressNack;
  case 3:
    return I2cError::DataNack;
  default:
    return I2cError::OtherBusError;
  }
}

template <typename RegisterAddress>
at42qt::Result<uint8_t> AT42QT<RegisterAddress>::getChipIdResult() {
  return readResult<uint8_t>(RegisterAddress::CHIP_ID);
}

template <typename RegisterAddress>
uint8_t AT42QT<RegisterAddress>::getChipId() {
  return getChipIdResult().value;
}

template <typename RegisterAddress>
bool AT42QT<RegisterAddress>::communicating() {
  at42qt::Result<uint8_t> chip_id_result = getChipIdResult();
  return chip_id_result.ok() && (chip_id_result.value == chip_id_);
}

template <typename RegisterAddress> void AT42QT<RegisterAddress>::hardReset() {
  if (reset_pin_ < 0) {
    return;
  }

  pinMode(reset_pin_, OUTPUT);
  digitalWrite(reset_pin_, HIGH);
  digitalWrite(reset_pin_, LOW);
  delay(1);
  digitalWrite(reset_pin_, HIGH);
}

template <typename RegisterAddress>
void AT42QT<RegisterAddress>::attachChangeCallback(Callback callback) {
  if (change_pin_ >= 0) {
    attachInterrupt(digitalPinToInterrupt(change_pin_), callback, FALLING);
  }
}

template <typename RegisterAddress>
at42qt::Result<void> AT42QT<RegisterAddress>::triggerCalibrationResult() {
  return writeResult(RegisterAddress::CALIBRATE, NONZERO_VALUE);
}

template <typename RegisterAddress>
void AT42QT<RegisterAddress>::triggerCalibration() {
  triggerCalibrationResult();
}

template <typename RegisterAddress>
at42qt::Result<void> AT42QT<RegisterAddress>::resetResult() {
  return writeResult(RegisterAddress::RESET, NONZERO_VALUE);
}

template <typename RegisterAddress> void AT42QT<RegisterAddress>::reset() {
  resetResult();
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<void>
AT42QT<RegisterAddress>::writeResult(RegisterAddress register_address,
                                     Data data) {
  static_assert(std::is_integral<Data>::value || std::is_enum<Data>::value,
                "writeResult requires an integral or enum type.");
  return writeRegisterBlock_(static_cast<uint8_t>(register_address), data,
                             sizeof(Data));
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<void>
AT42QT<RegisterAddress>::writeResult(RegisterAddress register_address,
                                     Data data, uint8_t data_size) {
  static_assert(std::is_integral<Data>::value || std::is_enum<Data>::value,
                "writeResult requires an integral or enum type.");
  return writeRegisterBlock_(static_cast<uint8_t>(register_address), data,
                             data_size);
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<void>
AT42QT<RegisterAddress>::writeKeyResult(RegisterAddress base_register_address,
                                        uint8_t key, Data data) {
  if (!keyValid_(key)) {
    at42qt::Result<void> result;
    result.error = I2cError::InvalidKey;
    last_i2c_error_ = result.error;
    return result;
  }

  constexpr uint8_t data_size = sizeof(Data);
  uint8_t register_address_number = static_cast<uint8_t>(base_register_address);
  register_address_number =
      static_cast<uint8_t>(register_address_number + (key * data_size));
  return writeRegisterBlock_(register_address_number, data, data_size);
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<Data>
AT42QT<RegisterAddress>::readResult(RegisterAddress register_address) {
  static_assert(std::is_integral<Data>::value || std::is_enum<Data>::value,
                "readResult requires an integral or enum type.");
  return readRegisterBlock_<Data>(static_cast<uint8_t>(register_address),
                                  sizeof(Data));
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<Data>
AT42QT<RegisterAddress>::readResult(RegisterAddress register_address,
                                    uint8_t data_size) {
  static_assert(std::is_integral<Data>::value || std::is_enum<Data>::value,
                "readResult requires an integral or enum type.");
  return readRegisterBlock_<Data>(static_cast<uint8_t>(register_address),
                                  data_size);
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<Data>
AT42QT<RegisterAddress>::readKeyResult(RegisterAddress base_register_address,
                                       uint8_t key) {
  if (!keyValid_(key)) {
    at42qt::Result<Data> result;
    result.error = I2cError::InvalidKey;
    last_i2c_error_ = result.error;
    return result;
  }

  constexpr uint8_t data_size = sizeof(Data);
  uint8_t register_address_number = static_cast<uint8_t>(base_register_address);
  register_address_number =
      static_cast<uint8_t>(register_address_number + (key * data_size));
  return readRegisterBlock_<Data>(register_address_number, data_size);
}

template <typename RegisterAddress>
template <typename Data>
void AT42QT<RegisterAddress>::write(RegisterAddress register_address,
                                    Data data) {
  writeResult(register_address, data);
}

template <typename RegisterAddress>
template <typename Data>
void AT42QT<RegisterAddress>::write(RegisterAddress register_address, Data data,
                                    uint8_t data_size) {
  writeResult(register_address, data, data_size);
}

template <typename RegisterAddress>
template <typename Data>
void AT42QT<RegisterAddress>::writeKey(RegisterAddress base_register_address,
                                       uint8_t key, Data data) {
  writeKeyResult(base_register_address, key, data);
}

template <typename RegisterAddress>
template <typename Data>
Data AT42QT<RegisterAddress>::read(RegisterAddress register_address) {
  return readResult<Data>(register_address).value;
}

template <typename RegisterAddress>
template <typename Data>
Data AT42QT<RegisterAddress>::read(RegisterAddress register_address,
                                   uint8_t data_size) {
  return readResult<Data>(register_address, data_size).value;
}

template <typename RegisterAddress>
template <typename Data>
Data AT42QT<RegisterAddress>::readKey(RegisterAddress base_register_address,
                                      uint8_t key) {
  return readKeyResult<Data>(base_register_address, key).value;
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<void>
AT42QT<RegisterAddress>::writeRegisterBlock_(uint8_t register_address_number,
                                             Data data, uint8_t data_size) {
  at42qt::Result<void> result;

  if (!initialized_ || (wire_ptr_ == nullptr)) {
    result.error = I2cError::NotInitialized;
    last_i2c_error_ = result.error;
    return result;
  }

  using UnsignedData = typename std::make_unsigned<Data>::type;
  UnsignedData value = static_cast<UnsignedData>(data);

  wire_ptr_->beginTransmission(device_address_);
  wire_ptr_->write(register_address_number);
  for (uint8_t byte_n = 0; byte_n < data_size; ++byte_n) {
    wire_ptr_->write(static_cast<uint8_t>((value >> (8u * byte_n)) &
                                          static_cast<UnsignedData>(0xFF)));
  }
  result.error = endTransmissionToError_(wire_ptr_->endTransmission());
  last_i2c_error_ = result.error;
  return result;
}

template <typename RegisterAddress>
template <typename Data>
at42qt::Result<Data>
AT42QT<RegisterAddress>::readRegisterBlock_(uint8_t register_address_number,
                                            uint8_t data_size) {
  at42qt::Result<Data> result;

  if (!initialized_ || (wire_ptr_ == nullptr)) {
    result.error = I2cError::NotInitialized;
    last_i2c_error_ = result.error;
    return result;
  }

  wire_ptr_->beginTransmission(device_address_);
  wire_ptr_->write(register_address_number);
  result.error = endTransmissionToError_(wire_ptr_->endTransmission(false));
  if (!result.ok()) {
    last_i2c_error_ = result.error;
    return result;
  }

#if defined(WIRE_HAS_TIMEOUT)
  wire_ptr_->clearWireTimeoutFlag();
#endif
  uint8_t bytes_received = wire_ptr_->requestFrom(device_address_, data_size);
  if (bytes_received != data_size) {
    result.error = I2cError::RequestFailed;
    last_i2c_error_ = result.error;
    return result;
  }

  if (wire_ptr_->available() < data_size) {
    result.error = I2cError::ShortRead;
    last_i2c_error_ = result.error;
    return result;
  }

  using UnsignedData = typename std::make_unsigned<Data>::type;
  UnsignedData value = 0;
  for (uint8_t byte_n = 0; byte_n < data_size; ++byte_n) {
    value |= static_cast<UnsignedData>(static_cast<uint8_t>(wire_ptr_->read()))
             << (8u * byte_n);
  }

  result.value = static_cast<Data>(value);
  last_i2c_error_ = result.error;
  return result;
}

#endif
