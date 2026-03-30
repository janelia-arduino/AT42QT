// Minimal Arduino compatibility shim for native unit tests.
#ifndef AT42QT_TEST_NATIVE_ARDUINO_H
#define AT42QT_TEST_NATIVE_ARDUINO_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

#ifndef HIGH
#define HIGH 0x1
#endif
#ifndef LOW
#define LOW 0x0
#endif
#ifndef OUTPUT
#define OUTPUT 0x1
#endif
#ifndef INPUT
#define INPUT 0x0
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 0x2
#endif
#ifndef FALLING
#define FALLING 0x2
#endif
#ifndef bitRead
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#endif

inline std::vector<size_t> g_pin_mode_pins;
inline std::vector<uint8_t> g_pin_mode_modes;
inline std::vector<size_t> g_digital_write_pins;
inline std::vector<uint8_t> g_digital_write_values;
inline unsigned int g_last_delay_microseconds = 0;
inline unsigned long g_last_delay_milliseconds = 0;
inline size_t g_last_interrupt_pin = 0;
inline uint8_t g_last_interrupt_mode = 0;
inline void (*g_last_interrupt_callback)(void) = nullptr;

inline void pinMode(size_t pin, uint8_t mode) {
  g_pin_mode_pins.push_back(pin);
  g_pin_mode_modes.push_back(mode);
}

inline void digitalWrite(size_t pin, uint8_t value) {
  g_digital_write_pins.push_back(pin);
  g_digital_write_values.push_back(value);
}

inline void delayMicroseconds(unsigned int delay_us) {
  g_last_delay_microseconds = delay_us;
}

inline void delay(unsigned long delay_ms) {
  g_last_delay_milliseconds = delay_ms;
}

inline int digitalPinToInterrupt(int pin) { return pin; }

inline void attachInterrupt(uint8_t interrupt_number, void (*callback)(void),
                            int mode) {
  g_last_interrupt_pin = interrupt_number;
  g_last_interrupt_callback = callback;
  g_last_interrupt_mode = (uint8_t)mode;
}

#endif
