// ----------------------------------------------------------------------------
// AT42QT2160.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef AT42QT2160_H
#define AT42QT2160_H
#include "AT42QT.h"


class AT42QT2160 : public AT42QT
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x0D;
  static const uint8_t CHIP_ID = 0x11;

  AT42QT2160(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1);

  union Status
  {
    struct
    {
      uint32_t slider : 1;
      uint32_t common_change : 1;
      uint32_t space0 : 4;
      uint32_t cycle_overrun : 1;
      uint32_t was_reset : 1;
      uint32_t keys : 16;
      uint32_t slider_position : 8;
    };
    uint32_t bytes;
  };
  Status getStatus();

  void triggerCalibration();

  void reset();
private:
  enum struct RegisterAddresses
  {
    CHIP_ID = 0,
    CODE_VERSION,
    GENERAL_STATUS,
    KEY_STATUS,
    SLIDER_POSITION=5,
    GPIO_READ,
    SUB_REVISION,
    CALIBRATE = 10,
    RESET,
    LP_MODE,
    BREP,
    NEG_DRIFT_COMP = 15,
    POS_DRIFT_COMP,
    DI_LIMIT,
    NEG_RECAL_DELAY,
    DRIFT_HOLD_TIME,
    SLIDER_CONTROL,
    SLIDER_OPTIONS,
    KEY0_AKS_GRP,
    KEY1_AKS_GRP,
    KEY2_AKS_GRP,
    KEY3_AKS_GRP,
    KEY4_AKS_GRP,
    KEY5_AKS_GRP,
    KEY6_AKS_GRP,
    KEY7_AKS_GRP,
    KEY8_AKS_GRP,
    KEY9_AKS_GRP,
    KEY10_AKS_GRP,
    KEY11_AKS_GRP,
    KEY12_AKS_GRP,
    KEY13_AKS_GRP,
    KEY14_AKS_GRP,
    KEY15_AKS_GRP,
    KEY0_NTHR,
    KEY1_NTHR,
    KEY2_NTHR,
    KEY3_NTHR,
    KEY4_NTHR,
    KEY5_NTHR,
    KEY6_NTHR,
    KEY7_NTHR,
    KEY8_NTHR,
    KEY9_NTHR,
    KEY10_NTHR,
    KEY11_NTHR,
    KEY12_NTHR,
    KEY13_NTHR,
    KEY14_NTHR,
    KEY15_NTHR,
    KEY0_BL,
    KEY1_BL,
    KEY2_BL,
    KEY3_BL,
    KEY4_BL,
    KEY5_BL,
    KEY6_BL,
    KEY7_BL,
    KEY8_BL,
    KEY9_BL,
    KEY10_BL,
    KEY11_BL,
    KEY12_BL,
    KEY13_BL,
    KEY14_BL,
    KEY15_BL,
    GPO_DRIVE,
    GPIO_DRIVE,
    GPIO_DIR = 73,
    GPO_PWM,
    GPIO_PWM,
    PWM_LEVEL,
    GPIO_WAKE,
    COMM_CHG_KEYS_1,
    COMM_CHG_KEYS_2,
    KEY0_SIGNAL = 100,
    KEY1_SIGNAL = 102,
    KEY2_SIGNAL = 104,
    KEY3_SIGNAL = 106,
    KEY4_SIGNAL = 108,
    KEY5_SIGNAL = 110,
    KEY6_SIGNAL = 112,
    KEY7_SIGNAL = 114,
    KEY8_SIGNAL = 116,
    KEY9_SIGNAL = 118,
    KEY10_SIGNAL = 120,
    KEY11_SIGNAL = 122,
    KEY12_SIGNAL = 124,
    KEY13_SIGNAL = 126,
    KEY14_SIGNAL = 128,
    KEY15_SIGNAL = 130,		
    KEY0_REFERENCE = 132,
    KEY1_REFERENCE = 134,
    KEY2_REFERENCE = 136,
    KEY3_REFERENCE = 138,
    KEY4_REFERENCE = 140,	
    KEY5_REFERENCE = 142,
    KEY6_REFERENCE = 144,	
    KEY7_REFERENCE = 146,
    KEY8_REFERENCE = 148,	
    KEY9_REFERENCE = 150,
    KEY10_REFERENCE = 152,	
    KEY11_REFERENCE = 154,
    KEY12_REFERENCE = 156,	
    KEY13_REFERENCE = 158,
    KEY14_REFERENCE = 160,	
    KEY15_REFERENCE = 162,	
  };

};

#endif
