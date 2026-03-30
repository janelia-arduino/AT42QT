// Minimal Wire compatibility shim for native unit tests.
#ifndef AT42QT_TEST_NATIVE_WIRE_H
#define AT42QT_TEST_NATIVE_WIRE_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

class TwoWire {
public:
  bool begin_called{false};
  bool timeout_configured{false};
  unsigned long timeout_us{0};
  bool timeout_reset_on_flag{false};
  bool timeout_flag_cleared{false};
  std::vector<uint8_t> tx_buffer;
  std::vector<uint8_t> rx_buffer;
  uint8_t transmission_address{0};
  uint8_t request_address{0};
  uint8_t request_quantity{0};
  uint8_t end_transmission_result{0};
  uint8_t request_from_result{1};
  size_t rx_index{0};

  void begin() { begin_called = true; }

  void setWireTimeout(unsigned long timeout, bool reset_on_timeout) {
    timeout_configured = true;
    timeout_us = timeout;
    timeout_reset_on_flag = reset_on_timeout;
  }

  void clearWireTimeoutFlag() { timeout_flag_cleared = true; }

  void beginTransmission(uint8_t address) {
    transmission_address = address;
    tx_buffer.clear();
  }

  size_t write(uint8_t data) {
    tx_buffer.push_back(data);
    return 1;
  }

  uint8_t endTransmission(bool = true) { return end_transmission_result; }

  uint8_t requestFrom(uint8_t address, uint8_t quantity) {
    request_address = address;
    request_quantity = quantity;
    rx_index = 0;
    return request_from_result;
  }

  int available() { return (int)(rx_buffer.size() - rx_index); }

  int read() {
    if (rx_index >= rx_buffer.size()) {
      return -1;
    }
    return rx_buffer[rx_index++];
  }
};

inline TwoWire Wire;

#endif
