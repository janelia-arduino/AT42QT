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

  touch_sensor.attachChangeCallback(changeLineActivated);

  Serial << "waiting for touch..." << endl;
}

void loop()
{
  if (change_line_activated)
  {
    change_line_activated = false;
    AT42QT2120::Status status = touch_sensor.getStatus();
    if (touch_sensor.anyTouched(status))
    {
      Serial << "status.keys: " << _BIN(status.keys) << endl;
    }
  }
}

