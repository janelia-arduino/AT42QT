#include <AT42QT2160.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_DELAY = 2000;

const int LOOP_DELAY = 500;

AT42QT2160 touch_sensor;

unsigned long loop_counter;

void setup()
{
  Serial.begin(BAUD);

  touch_sensor.begin();

  if (!touch_sensor.communicating())
  {
    return;
  }

  Serial << "reseting..." << endl;
  touch_sensor.reset();
  delay(RESET_DELAY);

  Serial << "triggerCalibration" << endl;
  touch_sensor.triggerCalibration();
  delay(CALIBRATION_DELAY);
  Serial << "finished calibrating" << endl;
  
  loop_counter = 0;
}

void loop()
{
  delay(LOOP_DELAY);

  if (!touch_sensor.communicating())
  {
    Serial << "Not communicating! Is chip connected and powered properly?" << endl;
    return;
  }

  uint8_t chip_id = touch_sensor.getChipId();
  Serial << "chip_id: " << _HEX(chip_id) << endl;

  bool any_key_touched = touch_sensor.anyKeyTouched();
  Serial << "any_key_touched: " << any_key_touched << endl;

  AT42QT2160::KeyStatus key_status = touch_sensor.getKeyStatus();
  Serial << "key_status: " << endl;
  Serial << "0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15" << endl;
  Serial << key_status.fields.key_0 << "  " \
         << key_status.fields.key_1 << "  " \
         << key_status.fields.key_2 << "  " \
         << key_status.fields.key_3 << "  " \
         << key_status.fields.key_4 << "  " \
         << key_status.fields.key_5 << "  " \
         << key_status.fields.key_6 << "  " \
         << key_status.fields.key_7 << "  " \
         << key_status.fields.key_8 << "  " \
         << key_status.fields.key_9 << "  " \
         << key_status.fields.key_10 << "  " \
         << key_status.fields.key_11 << "  " \
         << key_status.fields.key_12 << "  " \
         << key_status.fields.key_13 << "  " \
         << key_status.fields.key_14 << "  " \
         << key_status.fields.key_15 << endl;

  Serial << endl;
  ++loop_counter;
}
