// ----------------------------------------------------------------------------
// AT42QT.cpp
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "AT42QT.h"

AT42QT::AT42QT()
{
  device_count_ = 0;
  touch_status_mask_ = TOUCH_STATUS_MASK_BASE;
}

bool AT42QT::setupSingleDevice(TwoWire & wire,
  DeviceAddress device_address,
  bool fast_mode)
{
  setWire(wire,fast_mode);
  addDevice(device_address);
  bool successfully_setup = setupDevice(device_address);
  return successfully_setup;
}

bool AT42QT::communicating(DeviceAddress device_address)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return false;
  }

  uint8_t register_data = 0;
  read(device_address,AFE2_REGISTER_ADDRESS,register_data);

  if (register_data == afe2_register_values_[device_index])
  {
    return true;
  }
  return false;
}

// proximity_mode:
// set number of channels to use to generate virtual "13th"
// proximity channel
// see http://cache.freescale.com/files/sensors/doc/app_note/AN3893.pdf
//
// N.B. - this is not related to general proximity detection or
// reading back continuous proximity data
// "13th channel" proximity modes
// N.B. this does not relate to normal proximity detection
// see http://cache.freescale.com/files/sensors/doc/app_note/AN3893.pdf
void AT42QT::startChannels(uint8_t physical_channel_count,
  ProximityMode proximity_mode)
{
  startChannelsAllDevices(physical_channel_count,proximity_mode);
}

void AT42QT::startAllChannels(ProximityMode proximity_mode)
{
  for (uint8_t device_index=0; device_index<device_count_; ++device_index)
  {
    startAllChannels(device_addresses_[device_index],proximity_mode);
  }
}

void AT42QT::stopAllChannels()
{
  for (uint8_t device_index=0; device_index<device_count_; ++device_index)
  {
    stopAllChannels(device_addresses_[device_index]);
  }
}

uint8_t AT42QT::getChannelCount()
{
  return CHANNELS_PER_DEVICE * device_count_;
}

uint8_t AT42QT::getRunningChannelCount()
{
  uint8_t running_channel_count = 0;
  for (uint8_t device_index=0; device_index<device_count_; ++device_index)
  {
    running_channel_count += getRunningChannelCount(device_addresses_[device_index]);
  }
  return running_channel_count;
}

void AT42QT::setChannelThresholds(uint8_t channel,
  uint8_t touch_threshold,
  uint8_t release_threshold)
{
  if (channel >= getChannelCount())
  {
    return;
  }
  DeviceAddress device_address = channelToDeviceAddress(channel);
  uint8_t device_channel = channelToDeviceChannel(channel);

  setDeviceChannelThresholds(device_address,
    device_channel,
    touch_threshold,
    release_threshold);
}

void AT42QT::setAllChannelsThresholds(uint8_t touch_threshold,
  uint8_t release_threshold)
{
  for (uint8_t device_index=0; device_index<device_count_; ++device_index)
  {
    setAllDeviceChannelsThresholds(device_addresses_[device_index],
      touch_threshold,
      release_threshold);
  }
}

bool AT42QT::channelTouched(uint8_t channel)
{
  if (channel >= getChannelCount())
  {
    return false;
  }
  DeviceAddress device_address = channelToDeviceAddress(channel);
  uint8_t device_channel = channelToDeviceChannel(channel);

  uint16_t touch_status = getTouchStatus(device_address);
  return deviceChannelTouched(touch_status,device_channel);
}

uint16_t AT42QT::getChannelFilteredData(uint8_t channel)
{
  if (channel >= getChannelCount())
  {
    return 0;
  }
  DeviceAddress device_address = channelToDeviceAddress(channel);
  uint8_t device_channel = channelToDeviceChannel(channel);

  return getDeviceChannelFilteredData(device_address,device_channel);
}

uint16_t AT42QT::getChannelBaselineData(uint8_t channel)
{
  if (channel >= getChannelCount())
  {
    return 0;
  }
  DeviceAddress device_address = channelToDeviceAddress(channel);
  uint8_t device_channel = channelToDeviceChannel(channel);

  return getDeviceChannelBaselineData(device_address,device_channel);
}

void AT42QT::setWire(TwoWire & wire,
  bool fast_mode)
{
  wire_ptr_ = &wire;
  wire_ptr_->begin();
  if (fast_mode)
  {
    wire_ptr_->setClock(FAST_MODE_CLOCK_FREQUENCY);
  }

}

void AT42QT::addDevice(DeviceAddress device_address)
{
  if (device_count_ >= DEVICE_COUNT_MAX)
  {
    return;
  }
  uint8_t device_index = device_count_++;
  device_addresses_[device_index] = device_address;
  proximity_modes_[device_index] = PROXIMITY_MODE_DISABLED;
  afe2_register_values_[device_index] = AFE2_REGISTER_DEFAULT;
}

bool AT42QT::setupDevice(DeviceAddress device_address)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return false;
  }

  // soft reset
  write(device_address,SRST_REGISTER_ADDRESS,SRST_REGISTER_VALUE);
  afe2_register_values_[device_index] = AFE2_REGISTER_DEFAULT;
  delay(1);

  if (communicating(device_address))
  {
    applySettings(device_address,default_settings_);
    return true;
  }
  return false;
}

bool AT42QT::setupAllDevices()
{
  bool successfully_setup;
  bool all_successfully_setup = true;
  for (uint8_t device_index=0; device_index<device_count_; ++device_index)
  {
    successfully_setup = setupDevice(device_addresses_[device_index]);
    all_successfully_setup = (all_successfully_setup && successfully_setup);
  }
  return all_successfully_setup;
}

void AT42QT::startChannels(DeviceAddress device_address,
  uint8_t physical_channel_count,
  ProximityMode proximity_mode)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  proximity_modes_[device_index] = proximity_mode;

  clearOverCurrentFlag(device_address);

  ElectrodeConfiguration electrode_configuration;
  read(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
  if (physical_channel_count > PHYSICAL_CHANNELS_PER_DEVICE)
  {
    physical_channel_count = PHYSICAL_CHANNELS_PER_DEVICE;
  }
  electrode_configuration.fields.electrode_enable = physical_channel_count;
  electrode_configuration.fields.proximity_enable = proximity_mode;
  write(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
}

void AT42QT::startChannelsAllDevices(uint8_t physical_channel_count,
  ProximityMode proximity_mode)
{
  for (uint8_t device_index=0; device_index<device_count_; ++device_index)
  {
    startChannels(device_addresses_[device_index],
      physical_channel_count,
      proximity_mode);
  }
}

void AT42QT::startAllChannels(DeviceAddress device_address,
  ProximityMode proximity_mode)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  proximity_modes_[device_index] = proximity_mode;

  clearOverCurrentFlag(device_address);

  ElectrodeConfiguration electrode_configuration;
  read(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
  electrode_configuration.fields.electrode_enable = PHYSICAL_CHANNELS_PER_DEVICE;
  electrode_configuration.fields.proximity_enable = proximity_mode;
  write(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
}

void AT42QT::stopAllChannels(DeviceAddress device_address)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  ElectrodeConfiguration electrode_configuration;
  read(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
  electrode_configuration.fields.electrode_enable = 0;
  electrode_configuration.fields.proximity_enable = 0;
  write(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
}

uint8_t AT42QT::getDeviceCount()
{
  return device_count_;
}

uint8_t AT42QT::getDeviceChannelCount()
{
  return CHANNELS_PER_DEVICE;
}

uint8_t AT42QT::getRunningChannelCount(DeviceAddress device_address)
{
  ElectrodeConfiguration electrode_configuration;
  read(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
  uint8_t running_channel_count = electrode_configuration.fields.electrode_enable;
  if (running_channel_count >= PHYSICAL_CHANNELS_PER_DEVICE)
  {
    running_channel_count = PHYSICAL_CHANNELS_PER_DEVICE;
  }
  if (electrode_configuration.fields.proximity_enable)
  {
    running_channel_count += 1;
  }
  return running_channel_count;
}

void AT42QT::setDeviceChannelThresholds(DeviceAddress device_address,
  uint8_t device_channel,
  uint8_t touch_threshold,
  uint8_t release_threshold)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  if (device_channel >= CHANNELS_PER_DEVICE)
  {
    return;
  }
  if (release_threshold > touch_threshold)
  {
    uint8_t threshold = touch_threshold;
    touch_threshold = release_threshold;
    release_threshold = threshold;
  }
  uint8_t register_address;
  pauseChannels(device_address);
  register_address = TOUCH_THRESHOLD0_REGISTER_ADDRESS + device_channel * 2;
  write(device_address,register_address,touch_threshold);
  register_address = RELEASE_THRESHOLD0_REGISTER_ADDRESS + device_channel * 2;
  write(device_address,register_address,release_threshold);
  resumeChannels(device_address);
}

void AT42QT::setAllDeviceChannelsThresholds(DeviceAddress device_address,
  uint8_t touch_threshold,
  uint8_t release_threshold)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  if (release_threshold > touch_threshold)
  {
    uint8_t threshold = touch_threshold;
    touch_threshold = release_threshold;
    release_threshold = threshold;
  }
  uint8_t register_address;
  pauseChannels(device_address);
  for (uint8_t device_channel=0; device_channel<CHANNELS_PER_DEVICE; ++device_channel)
  {
    register_address = TOUCH_THRESHOLD0_REGISTER_ADDRESS + device_channel * 2;
    write(device_address,register_address,touch_threshold);
    register_address = RELEASE_THRESHOLD0_REGISTER_ADDRESS + device_channel * 2;
    write(device_address,register_address,release_threshold);
  }
  resumeChannels(device_address);
}

uint16_t AT42QT::getTouchStatus(DeviceAddress device_address)
{
  uint16_t touch_status;
  read(device_address,TOUCH_STATUS_REGISTER_ADDRESS,touch_status);
  return touch_status;
}

bool AT42QT::overCurrentDetected(uint16_t touch_status)
{
  return touch_status & OVER_CURRENT_REXT;
}

bool AT42QT::anyTouched(uint16_t touch_status)
{
  return touch_status & touch_status_mask_;
}

uint8_t AT42QT::getTouchCount(uint16_t touch_status)
{
  return touch_status & touch_status_mask_;
}

bool AT42QT::deviceChannelTouched(uint16_t touch_status,
  uint8_t device_channel)
{
  return touch_status & (1 << device_channel);
}

uint16_t AT42QT::getOutOfRangeStatus(DeviceAddress device_address)
{
  uint16_t out_of_range_status;
  read(device_address,OUT_OF_RANGE_STATUS_REGISTER_ADDRESS,out_of_range_status);
  return out_of_range_status;
}

bool AT42QT::deviceChannelOutOfRange(uint16_t out_of_range_status,
  uint8_t device_channel)
{
  return out_of_range_status & (1 << device_channel);
}

bool AT42QT::autoConfigFail(uint16_t out_of_range_status)
{
  return out_of_range_status & OUT_OF_RANGE_STATUS_ACFF;
}

bool AT42QT::autoReconfigFail(uint16_t out_of_range_status)
{
  return out_of_range_status & OUT_OF_RANGE_STATUS_ARFF;
}

uint16_t AT42QT::getDeviceChannelFilteredData(DeviceAddress device_address,
  uint8_t device_channel)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  uint16_t device_channel_filtered_data = 0;
  if (device_index < 0)
  {
    return device_channel_filtered_data;
  }
  if (device_channel >= CHANNELS_PER_DEVICE)
  {
    return device_channel_filtered_data;
  }
  uint8_t register_address;
  register_address = FILTERED_DATA0_REGISTER_ADDRESS + device_channel * 2;
  read(device_address,register_address,device_channel_filtered_data);
  return device_channel_filtered_data;
}

uint16_t AT42QT::getDeviceChannelBaselineData(DeviceAddress device_address,
  uint8_t device_channel)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  uint8_t device_channel_baseline_data = 0;
  if (device_index < 0)
  {
    return device_channel_baseline_data;
  }
  if (device_channel >= CHANNELS_PER_DEVICE)
  {
    return device_channel_baseline_data;
  }
  uint8_t register_address;
  register_address = BASELINE_DATA0_REGISTER_ADDRESS + device_channel;
  read(device_address,register_address,device_channel_baseline_data);
  return device_channel_baseline_data << BASELINE_DATA_BIT_SHIFT;
}

void AT42QT::setBaselineTracking(DeviceAddress device_address,
  BaselineTracking baseline_tracking)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  ElectrodeConfiguration electrode_configuration_backup;
  read(device_address,ECR_REGISTER_ADDRESS,electrode_configuration_backup.uint8);
  ElectrodeConfiguration electrode_configuration;
  electrode_configuration.uint8 = 0;
  write(device_address,ECR_REGISTER_ADDRESS,electrode_configuration.uint8);
  electrode_configuration.fields.calibration_lock = baseline_tracking;
  write(device_address,ECR_REGISTER_ADDRESS,electrode_configuration_backup.uint8);
  electrode_configuration_backup.fields.calibration_lock = baseline_tracking;
  write(device_address,ECR_REGISTER_ADDRESS,electrode_configuration_backup.uint8);
}

void AT42QT::setDebounce(DeviceAddress device_address,
  uint8_t touch_debounce,
  uint8_t release_debounce)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  DebounceConfiguration debounce_configuration;
  debounce_configuration.fields.touch_debounce = touch_debounce;
  debounce_configuration.fields.release_debounce = release_debounce;
  pauseChannels(device_address);
  write(device_address,DEBOUNCE_REGISTER_ADDRESS,debounce_configuration.uint8);
  resumeChannels(device_address);
}

void AT42QT::setChargeDischargeCurrent(DeviceAddress device_address,
  uint8_t charge_discharge_current)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  charge_discharge_current = constrain(charge_discharge_current,
    CHARGE_DISCHARGE_CURRENT_MIN,
    CHARGE_DISCHARGE_CURRENT_MAX);
  AFE1Configuration afe1;
  read(device_address,AFE1_REGISTER_ADDRESS,afe1.uint8);
  afe1.fields.charge_discharge_current = charge_discharge_current;
  pauseChannels(device_address);
  write(device_address,AFE1_REGISTER_ADDRESS,afe1.uint8);
  resumeChannels(device_address);
}

void AT42QT::setDeviceChannelChargeDischargeCurrent(DeviceAddress device_address,
  uint8_t device_channel,
  uint8_t charge_discharge_current)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if ((device_index < 0) || (device_channel >= CHANNELS_PER_DEVICE))
  {
    return;
  }
  charge_discharge_current = constrain(charge_discharge_current,
    CHARGE_DISCHARGE_CURRENT_MIN,
    CHARGE_DISCHARGE_CURRENT_MAX);
  uint8_t register_address = CDC0_REGISTER_ADDRESS + device_channel;
  pauseChannels(device_address);
  write(device_address,register_address,charge_discharge_current);
  resumeChannels(device_address);
}

void AT42QT::setChargeDischargeTime(DeviceAddress device_address,
  ChargeDischargeTime charge_discharge_time)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  AFE2Configuration afe2;
  read(device_address,AFE2_REGISTER_ADDRESS,afe2.uint8);
  afe2.fields.charge_discharge_time = charge_discharge_time;
  pauseChannels(device_address);
  write(device_address,AFE2_REGISTER_ADDRESS,afe2.uint8);
  afe2_register_values_[device_index] = afe2.uint8;
  resumeChannels(device_address);
}

void AT42QT::setDeviceChannelChargeDischargeTime(DeviceAddress device_address,
  uint8_t device_channel,
  ChargeDischargeTime charge_discharge_time)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if ((device_index < 0) || (device_channel >= CHANNELS_PER_DEVICE))
  {
    return;
  }
  uint8_t register_address = CDT0_REGISTER_ADDRESS + (device_channel / CDT_DIVISOR);
  uint8_t bit_shift = CDT_OFFSET * (device_channel % CDT_DIVISOR);
  uint8_t charge_discharge_time_shifted = (uint8_t)charge_discharge_time << bit_shift;
  pauseChannels(device_address);
  write(device_address,register_address,charge_discharge_time_shifted);
  resumeChannels(device_address);
}

void AT42QT::setFirstFilterIterations(DeviceAddress device_address,
  FirstFilterIterations first_filter_iterations)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  AFE1Configuration afe1;
  read(device_address,AFE1_REGISTER_ADDRESS,afe1.uint8);
  afe1.fields.first_filter_iterations = first_filter_iterations;
  pauseChannels(device_address);
  write(device_address,AFE1_REGISTER_ADDRESS,afe1.uint8);
  resumeChannels(device_address);
}

void AT42QT::setSecondFilterIterations(DeviceAddress device_address,
  SecondFilterIterations second_filter_iterations)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  AFE2Configuration afe2;
  read(device_address,AFE2_REGISTER_ADDRESS,afe2.uint8);
  afe2.fields.second_filter_iterations = second_filter_iterations;
  pauseChannels(device_address);
  write(device_address,AFE2_REGISTER_ADDRESS,afe2.uint8);
  afe2_register_values_[device_index] = afe2.uint8;
  resumeChannels(device_address);
}

void AT42QT::setSamplePeriod(DeviceAddress device_address,
  SamplePeriod sample_period)
{
  int device_index = deviceAddressToDeviceIndex(device_address);
  if (device_index < 0)
  {
    return;
  }
  AFE2Configuration afe2;
  read(device_address,AFE2_REGISTER_ADDRESS,afe2.uint8);
  afe2.fields.sample_period = sample_period;
  pauseChannels(device_address);
  write(device_address,AFE2_REGISTER_ADDRESS,afe2.uint8);
  afe2_register_values_[device_index] = afe2.uint8;
  resumeChannels(device_address);
}

// private

int AT42QT::deviceAddressToDeviceIndex(DeviceAddress device_address)
{
  int device_index = DEVICE_INDEX_NONE;
  for (uint8_t index=0; index<device_count_; ++index)
  {
    if (device_address == device_addresses_[index])
    {
      device_index = index;
      break;
    }
  }
  return device_index;
}

void AT42QT::pauseChannels(DeviceAddress device_address)
{
  read(device_address,ECR_REGISTER_ADDRESS,electrode_configuration_backup_.uint8);
  stopAllChannels(device_address);
}

void AT42QT::resumeChannels(DeviceAddress device_address)
{
  write(device_address,ECR_REGISTER_ADDRESS,electrode_configuration_backup_.uint8);
}

uint8_t AT42QT::channelToDeviceIndex(uint8_t channel)
{
  return channel / CHANNELS_PER_DEVICE;
}

AT42QT::DeviceAddress AT42QT::channelToDeviceAddress(uint8_t channel)
{
  uint8_t device_index = channelToDeviceIndex(channel);
  return device_addresses_[device_index];
}

uint8_t AT42QT::channelToDeviceChannel(uint8_t channel)
{
  return channel % CHANNELS_PER_DEVICE;
}

void AT42QT::applySettings(DeviceAddress device_address,
  const Settings & settings)
{
  stopAllChannels(device_address);

  write(device_address,MHDR_REGISTER_ADDRESS,settings.MHDR);
  write(device_address,NHDR_REGISTER_ADDRESS,settings.NHDR);
  write(device_address,NCLR_REGISTER_ADDRESS,settings.NCLR);
  write(device_address,FDLR_REGISTER_ADDRESS,settings.FDLR);
  write(device_address,MHDF_REGISTER_ADDRESS,settings.MHDF);
  write(device_address,NHDF_REGISTER_ADDRESS,settings.NHDF);
  write(device_address,NCLF_REGISTER_ADDRESS,settings.NCLF);
  write(device_address,FDLF_REGISTER_ADDRESS,settings.FDLF);
  write(device_address,NHDT_REGISTER_ADDRESS,settings.NHDT);
  write(device_address,NCLT_REGISTER_ADDRESS,settings.NCLT);
  write(device_address,FDLT_REGISTER_ADDRESS,settings.FDLT);
  write(device_address,MHDPROXR_REGISTER_ADDRESS,settings.MHDPROXR);
  write(device_address,NHDPROXR_REGISTER_ADDRESS,settings.NHDPROXR);
  write(device_address,NCLPROXR_REGISTER_ADDRESS,settings.NCLPROXR);
  write(device_address,FDLPROXR_REGISTER_ADDRESS,settings.FDLPROXR);
  write(device_address,MHDPROXF_REGISTER_ADDRESS,settings.MHDPROXF);
  write(device_address,NHDPROXF_REGISTER_ADDRESS,settings.NHDPROXF);
  write(device_address,NCLPROXF_REGISTER_ADDRESS,settings.NCLPROXF);
  write(device_address,FDLPROXF_REGISTER_ADDRESS,settings.FDLPROXF);
  write(device_address,NHDPROXT_REGISTER_ADDRESS,settings.NHDPROXT);
  write(device_address,NCLPROXT_REGISTER_ADDRESS,settings.NCLPROXT);
  write(device_address,FDLPROXT_REGISTER_ADDRESS,settings.FDLPROXT);
  write(device_address,DEBOUNCE_REGISTER_ADDRESS,settings.DEBOUNCE);
  write(device_address,ECR_REGISTER_ADDRESS,settings.ECR);
  write(device_address,ACCR0_REGISTER_ADDRESS,settings.ACCR0);
  write(device_address,ACCR1_REGISTER_ADDRESS,settings.ACCR1);
  write(device_address,USL_REGISTER_ADDRESS,settings.USL);
  write(device_address,LSL_REGISTER_ADDRESS,settings.LSL);
  write(device_address,TL_REGISTER_ADDRESS,settings.TL);

  setAllDeviceChannelsThresholds(device_address,
    settings.touch_threshold,
    settings.release_threshold);
}

void AT42QT::clearOverCurrentFlag(DeviceAddress device_address)
{
  write(device_address,TOUCH_STATUS_REGISTER_ADDRESS,OVER_CURRENT_REXT);
}
