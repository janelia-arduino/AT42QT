#include <cassert>

#include <AT42QT1060.h>
#include <AT42QT1070.h>
#include <AT42QT2120.h>
#include <AT42QT2160.h>

namespace {
void dummyCallback() {}
} // namespace

int main() {
  {
    AT42QT2120 touch_sensor;
    auto chip_id_result = touch_sensor.getChipIdResult();
    assert(!chip_id_result.ok());
    assert(chip_id_result.error == at42qt::I2cError::NotInitialized);
    assert(touch_sensor.getLastI2cError() == at42qt::I2cError::NotInitialized);
  }

  {
    TwoWire wire;
    wire.rx_buffer = {AT42QT2120::CHIP_ID};

    AT42QT2120 touch_sensor;
    touch_sensor.setup(wire);

    assert(!wire.begin_called);
    assert(touch_sensor.initialized());
    assert(touch_sensor.communicating());
    assert(wire.request_quantity == 1);
    assert(wire.tx_buffer[0] == 0);
  }

  {
    TwoWire wire;
    wire.rx_buffer = {0x01, 0x80, 0x0F, 0x55};
    wire.request_from_result = 4;

    AT42QT2120 touch_sensor;
    touch_sensor.setup(wire);
    auto status = touch_sensor.getStatus();
    assert(status.any_key_touched);
    assert(!status.slider_or_wheel);
    assert(status.keys == 0x0F80);
    assert(status.slider_or_wheel_position == 0x55);
  }

  {
    TwoWire wire;
    wire.rx_buffer = {0x3f};

    AT42QT1060 touch_sensor;
    touch_sensor.setup(wire);
    touch_sensor.getKeySignal(AT42QT1060::KEY_COUNT);
    assert(touch_sensor.getLastI2cError() == at42qt::I2cError::InvalidKey);
  }

  {
    TwoWire wire;
    wire.rx_buffer = {0x22};

    AT42QT1060 touch_sensor;
    touch_sensor.setup(wire);
    auto mask = touch_sensor.getAdjacentKeySuppressionMask();
    assert(mask.bytes == 0x22);
    assert(wire.tx_buffer.size() == 1);
    assert(wire.tx_buffer[0] ==
           static_cast<uint8_t>(
               RegisterAddresses::AT42QT1060::ADJACENT_KEY_SUPPRESSION_MASK));
  }

  {
    TwoWire wire;

    AT42QT1060 touch_sensor;
    touch_sensor.setup(wire);

    AT42QT1060::IOMask mask;
    mask.io_0 = true;
    mask.io_6 = true;
    touch_sensor.setIOMask(mask);

    assert(wire.tx_buffer.size() == 2);
    assert(wire.tx_buffer[0] ==
           static_cast<uint8_t>(RegisterAddresses::AT42QT1060::IO_MASK));
    assert(wire.tx_buffer[1] == 0x41);
  }

  {
    AT42QT1070::Status status(0x83C1);
    assert(status.any_key);
    assert(status.overflow);
    assert(status.calibrating);
    assert(status.keys == 0x03);
  }

  {
    TwoWire wire;
    wire.rx_buffer = {0x01, 0x02, 0x03, 0x04, 0x05};
    wire.request_from_result = 5;

    AT42QT2160 touch_sensor;
    touch_sensor.setup(wire);
    auto status = touch_sensor.getStatus();
    assert(wire.request_quantity == AT42QT2160::STATUS_SIZE);
    assert(status.bytes == 0x0504030201ULL);
    assert(status.slider);
    assert(!status.common_change);
    assert(status.keys == 0x0302);
    assert(status.slider_position == 0x04);
    assert(status.gpio == 0x05);
  }

  {
    TwoWire wire;
    wire.end_transmission_result = 2;

    AT42QT1070 touch_sensor;
    touch_sensor.setup(wire);
    auto chip_id_result = touch_sensor.getChipIdResult();
    assert(!chip_id_result.ok());
    assert(chip_id_result.error == at42qt::I2cError::AddressNack);
  }

  {
    g_pin_mode_pins.clear();
    g_pin_mode_modes.clear();
    g_digital_write_pins.clear();
    g_digital_write_values.clear();
    g_last_delay_milliseconds = 0;
    g_last_interrupt_pin = 0;
    g_last_interrupt_mode = 0;
    g_last_interrupt_callback = nullptr;

    TwoWire wire;
    AT42QT2120 touch_sensor(wire, 7, 9);
    touch_sensor.begin();
    touch_sensor.hardReset();
    touch_sensor.attachChangeCallback(dummyCallback);

    assert(wire.begin_called);
    assert(g_pin_mode_pins.size() >= 2);
    assert(g_last_delay_milliseconds == 1);
    assert(g_last_interrupt_pin == 7);
    assert(g_last_interrupt_mode == FALLING);
    assert(g_last_interrupt_callback == dummyCallback);
  }

  {
    TwoWire wire;

    AT42QT2120 touch_sensor;
    touch_sensor.setup(wire);

    AT42QT2120::KeyPulseScale key_pulse_scale;
    key_pulse_scale.pulse = 0x0A;
    key_pulse_scale.scale = 0x03;
    touch_sensor.setKeyPulseScale(0, key_pulse_scale);

    assert(wire.tx_buffer.size() == 2);
    assert(
        wire.tx_buffer[0] ==
        static_cast<uint8_t>(RegisterAddresses::AT42QT2120::KEY_PULSE_SCALE));
    assert(wire.tx_buffer[1] == 0xA3);
  }

  return 0;
}
