#include <AT42QT2120.h>
#include <Streaming.h>


const long BAUD = 115200;
const int LOOP_DELAY = 500;

AT42QT2120 sensor;

void setup()
{
  Serial.begin(BAUD);
  sensor.begin();
}

void loop()
{
  uint8_t chip_id = sensor.getChipId();
  Serial << "chip_id: " << _HEX(chip_id) << endl;

  bool communicating = sensor.communicating();
  Serial << "communicating: " << communicating << endl;

  bool any_key_touched = sensor.anyKeyTouched();
  Serial << "any_key_touched: " << any_key_touched << endl;

  Serial << endl;
  delay(LOOP_DELAY);
}
