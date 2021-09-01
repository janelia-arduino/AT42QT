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

  Serial << "status.keys: " << _BIN(status.keys) << endl;
  for (uint8_t key=0; key < touch_sensor.KEY_COUNT; ++key)
  {
    if ((key != 0) && (key < 10))
      Serial << "  ";
    else if (key >= 10)
      Serial << " ";
    Serial << key;
  }
  Serial << endl;
  for (uint8_t key=0; key < touch_sensor.KEY_COUNT; ++key)
  {
    if (key != 0)
      Serial << "  ";
    Serial << bitRead(status.keys,key);
  }
  Serial << endl;

  uint8_t interval_count = 2;
  Serial << "setMeasurementIntervalCount(" << interval_count << ")" << endl;
  touch_sensor.setMeasurementIntervalCount(interval_count);
  interval_count = touch_sensor.getMeasurementIntervalCount();
  Serial << "getMeasurementIntervalCount(): " << interval_count << endl;

  uint8_t tdd = 21;
  Serial << "setTowardsDriftDuration(" << tdd << ")" << endl;
  touch_sensor.setTowardsDriftDuration(tdd);
  tdd = touch_sensor.getTowardsDriftDuration();
  Serial << "getTowardsDriftDuration(): " << tdd << endl;

  uint8_t add = 8;
  Serial << "setAwayDriftDuration(" << add << ")" << endl;
  touch_sensor.setAwayDriftDuration(add);
  add = touch_sensor.getAwayDriftDuration();
  Serial << "getAwayDriftDuration(): " << add << endl;

  uint8_t di = 5;
  Serial << "setDetectionIntegrator(" << di << ")" << endl;
  touch_sensor.setDetectionIntegrator(di);
  di = touch_sensor.getDetectionIntegrator();
  Serial << "getDetectionIntegrator(): " << di << endl;

  uint8_t rd = 250;
  Serial << "setRecalibrationDelay(" << rd << ")" << endl;
  touch_sensor.setRecalibrationDelay(rd);
  rd = touch_sensor.getRecalibrationDelay();
  Serial << "getRecalibrationDelay(): " << rd << endl;

  uint8_t dhd = 26;
  Serial << "setDriftHoldDuration(" << dhd << ")" << endl;
  touch_sensor.setDriftHoldDuration(dhd);
  dhd = touch_sensor.getDriftHoldDuration();
  Serial << "getDriftHoldDuration(): " << dhd << endl;

  Serial << endl;
  ++loop_counter;
}
