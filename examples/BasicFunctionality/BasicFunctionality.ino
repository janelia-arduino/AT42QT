// AT42QT is the base class that works with all AT42QTxxxx sensors
// Normally AT42QT is not instantiated directly, e.g. use AT42QT2120 instead
// This example is just used to demonstrate and test the base class basic functionality
#include <AT42QT.h>
#include <Wire.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int LOOP_DELAY = 1000;

// AT42QT2120
// These are not needed when using AT42QT instead of AT42QT2120
const int DEVICE_ADDRESS = 0x1C;
const int CHIP_ID = 0x3E;

const int CHANGE_PIN = 12;
const int RESET_PIN = 15;

AT42QT touch_sensor(DEVICE_ADDRESS,CHIP_ID,Wire,CHANGE_PIN,RESET_PIN);

void setup()
{
  Serial.begin(BAUD);

  touch_sensor.begin();

  Serial << "resetting..." << endl;
  touch_sensor.hardReset();
  delay(RESET_DELAY);
}

void loop()
{
  uint8_t chip_id = touch_sensor.getChipId();
  Serial << "chip_id: " << _HEX(chip_id) << endl;

  bool communicating = touch_sensor.communicating();
  Serial << "communicating: " << communicating << endl;

  Serial << endl;
  delay(LOOP_DELAY);
}
