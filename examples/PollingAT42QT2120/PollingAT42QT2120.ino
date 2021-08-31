#include <AT42QT2120.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_LOOP_DELAY = 50;

const int LOOP_DELAY = 500;

AT42QT2120 touch_sensor;

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
  delay(CALIBRATION_LOOP_DELAY);
  while (touch_sensor.calibrating())
  {
    Serial << "calibrating..." << endl;
    delay(CALIBRATION_LOOP_DELAY);
  }
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

  AT42QT2120::Status status = touch_sensor.getStatus();

  Serial << "status.any_key: " << status.any_key << endl;

  Serial << "status.key_: " << endl;
  Serial << "0  1  2  3  4  5  6  7  8  9 10 11" << endl;
  Serial << status.key_0 << "  " \
         << status.key_1 << "  " \
         << status.key_2 << "  " \
         << status.key_3 << "  " \
         << status.key_4 << "  " \
         << status.key_5 << "  " \
         << status.key_6 << "  " \
         << status.key_7 << "  " \
         << status.key_8 << "  " \
         << status.key_9 << "  " \
         << status.key_10 << "  " \
         << status.key_11 << endl;

  Serial << "setMeasurementIntervalCount(2): " << endl;
  touch_sensor.setMeasurementIntervalCount(2);

  uint8_t interval_count = touch_sensor.getMeasurementIntervalCount();
  Serial << "getMeasurementIntervalCount(): " << interval_count << endl;

  Serial << endl;
  ++loop_counter;
}
