#include <Arduino.h>
#include <AT42QT.h>
#include <Streaming.h>


AT42QT at42qt;

void setup()
{
  Serial.begin(constants::baud);

  at42qt.setupSingleDevice(*constants::wire_ptr,
    constants::device_address,
    constants::fast_mode);

  at42qt.setAllChannelsThresholds(constants::touch_threshold,
    constants::release_threshold);
  at42qt.setDebounce(constants::device_address,
    constants::touch_debounce,
    constants::release_debounce);
  at42qt.setBaselineTracking(constants::device_address,
    constants::baseline_tracking);
  at42qt.setChargeDischargeCurrent(constants::device_address,
    constants::charge_discharge_current);
  at42qt.setChargeDischargeTime(constants::device_address,
    constants::charge_discharge_time);
  at42qt.setFirstFilterIterations(constants::device_address,
    constants::first_filter_iterations);
  at42qt.setSecondFilterIterations(constants::device_address,
    constants::second_filter_iterations);
  at42qt.setSamplePeriod(constants::device_address,
    constants::sample_period);

  at42qt.startChannels(constants::physical_channel_count,
    constants::proximity_mode);
}

void loop()
{
  delay(constants::loop_delay);
  if (!at42qt.communicating(constants::device_address))
  {
    Serial << "at42qt device not commmunicating!\n\n";
    return;
  }

  uint8_t channel_count = at42qt.getChannelCount();
  Serial << "channel_count: " << channel_count << "\n";
  uint8_t running_channel_count = at42qt.getRunningChannelCount();
  Serial << "running_channel_count: " << running_channel_count << "\n";

  uint16_t touch_status = at42qt.getTouchStatus(constants::device_address);
  Serial << "touch_status: " << _BIN(touch_status) << "\n";
  if (at42qt.overCurrentDetected(touch_status))
  {
    Serial << "Over current detected!\n\n";
    at42qt.startChannels(constants::physical_channel_count,
      constants::proximity_mode);
    return;
  }
  bool any_touched = at42qt.anyTouched(touch_status);
  Serial << "any_touched: " << any_touched << "\n";
  bool device_channel_touched = at42qt.deviceChannelTouched(touch_status,
    constants::channel);
  Serial << "device_channel_touched: " << device_channel_touched << "\n";

  uint16_t out_of_range_status = at42qt.getOutOfRangeStatus(constants::device_address);
  Serial << "out_of_range_status: " << _BIN(out_of_range_status) << "\n";

  bool channel_touched = at42qt.channelTouched(constants::channel);
  Serial << "channel_touched: " << channel_touched << "\n";

  uint16_t channel_filtered_data = at42qt.getChannelFilteredData(constants::channel);
  Serial << "channel_filtered_data: " << channel_filtered_data << "\n";

  uint16_t channel_baseline_data = at42qt.getChannelBaselineData(constants::channel);
  Serial << "channel_baseline_data: " << channel_baseline_data << "\n";

  Serial << "\n";
}
