// ----------------------------------------------------------------------------
// AT42QT1070.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT1070_H
#define AT42QT1070_H
#include "AT42QT.h"


class AT42QT1070 : public AT42QT
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x1B;
  static const uint8_t CHIP_ID = 0x2E;

  AT42QT1070(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1);

  union DetectionStatus
  {
    struct Fields
    {
      uint8_t any_keys : 1;
      uint8_t space : 5;
      uint8_t overflow : 1;
      uint8_t calibrate : 1;
    } fields;
    uint8_t uint8;
  };
  DetectionStatus getDetectionStatus();
  bool anyKeyTouched();
  bool overflow();
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
      uint8_t key_6 : 1;
      uint8_t space : 1;
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
    FIRMWARE_VERSION,
    DETECTION_STATUS,
    KEY_STATUS,
    KEY0_SIGNAL,
    KEY1_SIGNAL = 6,
    KEY2_SIGNAL = 8,
    KEY3_SIGNAL = 10,
    KEY4_SIGNAL = 12,
    KEY5_SIGNAL = 14,
    KEY6_SIGNAL = 16,
    KEY0_REFERENCE = 18,
    KEY1_REFERENCE = 20,
    KEY2_REFERENCE = 22,
    KEY3_REFERENCE = 24,
    KEY4_REFERENCE = 26,
    KEY5_REFERENCE = 28,
    KEY6_REFERENCE = 30,
    KEY0_NTHR = 32,
    KEY1_NTHR,
    KEY2_NTHR,
    KEY3_NTHR,
    KEY4_NTHR,
    KEY5_NTHR,
    KEY6_NTHR,
    KEY0__AVE_AKS = 39,
    KEY1_AVE_AKS,
    KEY2_AVE_AKS,
    KEY3_AVE_AKS,
    KEY4_AVE_AKS,
    KEY5_AVE_AKS,
    KEY6_AVE_AKS,
    KEY0_DI = 46,
    KEY1_DI,
    KEY2_DI,
    KEY3_DI,
    KEY4_DI,
    KEY5_DI,
    KEY6_DI,
    FO_MO_GDNO,
    LP,
    MAX_ON_DURATION,
    CALIBRATE,
    RESET,
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
