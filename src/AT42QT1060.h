// ----------------------------------------------------------------------------
// AT42QT1060.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT1060_H
#define AT42QT1060_H
#include "AT42QT.h"


class AT42QT1060 : public AT42QT
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x12;
  static const uint8_t CHIP_ID = 0x31;

  AT42QT1060(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1);

  union DetectionStatus
  {
    struct Fields
    {
      uint8_t any_keys : 6;
      uint8_t space : 1;
      uint8_t calibrate : 1;
    } fields;
    uint8_t uint8;
  };
  DetectionStatus getDetectionStatus();
  bool anyKeyTouched();
  bool calibrating();

  union KeyStatus
  {
    struct Fields
    {
      uint8_t key_0 : 1;
      uint8_t key_1 : 1;
      uint8_t key_2 : 1;
      uint8_t key_3 : 1;
      uint8_t key_4 : 1;
      uint8_t key_5 : 1;
      uint8_t space : 2;
    } fields;
    uint8_t uint8;
  };
  KeyStatus getKeyStatus();

  void triggerCalibration();

  void reset();
private:
  enum struct RegisterAddresses
  {
    CHIP_ID = 0,
    VERSION,
    MINOR_VERSION,
    DETECTION_STATUS = 4,
    INPUT_PORT_STATUS,
    CALIBRATE = 12,
    RESET,
    DRIFT_OPTION,
    POSITIVE_RECAL_DELAY,
    KEY0_NTHR,
    KEY1_NTHR,
    KEY2_NTHR,
    KEY3_NTHR,
    KEY4_NTHR,
    KEY5_NTHR,
    LP,
    IO_MASK,
    KEY_MASK,
    AKS_MASK,
    PWM_MASK,
    DETECTION_MASK,
    ACTIVE_LEVEL_MASK,
    USER_OUTPUT_BUFFER,
    DI,
    PWM_LEVEL,	
    KEY0_SIGNAL = 40,
    KEY1_SIGNAL = 42,
    KEY2_SIGNAL = 44,
    KEY3_SIGNAL = 46,
    KEY4_SIGNAL = 48,
    KEY5_SIGNAL = 50,
    KEY0_REFERENCE = 52,
    KEY1_REFERENCE = 54,
    KEY2_REFERENCE = 56,
    KEY3_REFERENCE = 58,
    KEY4_REFERENCE = 60,
    KEY5_REFERENCE = 62
  };

  template<typename T>
  void write(RegisterAddresses register_address,
    const T & data)
  {
    AT42QT::write(static_cast<uint8_t>(register_address),data);
  }

  template<typename T>
  void read(RegisterAddresses register_address,
    T & data)
  {
    AT42QT::read(static_cast<uint8_t>(register_address),data);
  }

};

#endif
