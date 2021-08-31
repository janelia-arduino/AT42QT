#include <AT42QT2120.h>
#include <Wire.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_LOOP_DELAY = 50;

const int CHANGE_PIN = 12;

AT42QT2120 touch_sensor(Wire,CHANGE_PIN);

volatile bool change_line_activated = false;

void changeLineActivated()
{
  change_line_activated = true;
}

void setup()
{
  Serial.begin(BAUD);

  touch_sensor.begin();

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

  // change line is pulled low 85ms after reset
  // must read any device memory location to clear change line
  touch_sensor.getDetectionStatus();

  touch_sensor.attachChangeCallback(changeLineActivated);

  Serial << "waiting for touch..." << endl;
}

void loop()
{
  if (change_line_activated)
  {
    change_line_activated = false;
    uint32_t status = touch_sensor.getStatus();
    Serial << "status: " << _BIN(status) << endl;
    // if (touch_sensor.anyKeyTouched())
    // {
    //   AT42QT2120::KeyStatus key_status = touch_sensor.getKeyStatus();
    //   Serial << "key_status.uint16: " << _BIN(key_status.uint16) << endl;

    //   uint8_t slider_position = touch_sensor.getSliderPosition();
    //   Serial << "slider_position: " << slider_position << endl;


    // }
  }
}
