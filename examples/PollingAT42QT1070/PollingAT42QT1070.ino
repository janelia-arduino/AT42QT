#include <AT42QT1070.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_LOOP_DELAY = 50;

const int LOOP_DELAY = 500;

AT42QT1070 touch_sensor;

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

  AT42QT1070::Status status = touch_sensor.getStatus();

  Serial << "status.any_key: " << status.any_key << endl;

  Serial << "status.keys: " << _BIN(status.keys) << endl;
  Serial << "0  1  2  3  4  5  6" << endl;
  Serial << bitRead(status.keys,0) << "  "      \
         << bitRead(status.keys,1) << "  "      \
         << bitRead(status.keys,2) << "  "      \
         << bitRead(status.keys,3) << "  "      \
         << bitRead(status.keys,4) << "  "      \
         << bitRead(status.keys,5) << "  "      \
         << bitRead(status.keys,6) << endl;

  Serial << endl;
  ++loop_counter;
}
