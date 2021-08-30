#include <AT42QT2120.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int LOOP_DELAY = 500;
const int CALIBRATION_LOOP_DELAY = 50;
const int NUMBER_OF_LOOPS_TO_TRIGGER_CALIBRATION = 20;

AT42QT2120 sensor;

unsigned long loop_counter;

void setup()
{
  Serial.begin(BAUD);
  sensor.begin();

  loop_counter = 0;
}

void loop()
{
  if (loop_counter == 0)
  {
    Serial << "reseting..." << endl;
    delay(RESET_DELAY);
  }

  uint8_t chip_id = sensor.getChipId();
  Serial << "chip_id: " << _HEX(chip_id) << endl;

  bool communicating = sensor.communicating();
  Serial << "communicating: " << communicating << endl;

  bool any_key_touched = sensor.anyKeyTouched();
  Serial << "any_key_touched: " << any_key_touched << endl;

  AT42QT2120::KeyStatus key_status = sensor.getKeyStatus();
  Serial << "key_status: " << endl;
  Serial << "0  1  2  3  4  5  6  7  8  9 10 11" << endl;
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
    << key_status.fields.key_11 << endl;

  if ((loop_counter != 0) &&
    ((loop_counter % NUMBER_OF_LOOPS_TO_TRIGGER_CALIBRATION) == 0))
  {
    Serial << "triggerCalibration" << endl;
    sensor.triggerCalibration();
    delay(CALIBRATION_LOOP_DELAY);
    while (sensor.calibrating())
    {
      Serial << "calibrating..." << endl;
      delay(CALIBRATION_LOOP_DELAY);
    }
    Serial << "finished calibrating" << endl;
  }
  
  Serial << "setMeasurementIntervalCount(2) 2*16ms=32ms: " << endl;
  sensor.setMeasurementIntervalCount(2);

  uint8_t interval_count = sensor.getMeasurementIntervalCount();
  Serial << "getMeasurementIntervalCount(): " << interval_count << endl;

  Serial << endl;
  ++loop_counter;
  delay(LOOP_DELAY);
}
