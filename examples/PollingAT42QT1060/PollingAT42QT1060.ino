#include <AT42QT1060.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_LOOP_DELAY = 50;

const int LOOP_DELAY = 500;

AT42QT1060 touch_sensor;

unsigned long loop_counter;

void printKeyStatus(uint16_t keys, uint8_t key_count)
{
  for (uint8_t key=0; key < key_count; ++key)
  {
    if ((key != 0) && (key < 10))
      Serial << "  ";
    else if (key >= 10)
      Serial << " ";
    Serial << key;
  }
  Serial << endl;
  for (uint8_t key=0; key < key_count; ++key)
  {
    if (key != 0)
      Serial << "  ";
    Serial << bitRead(keys,key);
  }
  Serial << endl;
}

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

  AT42QT1060::Status status = touch_sensor.getStatus();

  Serial << "status.keys: " << _BIN(status.keys) << endl;

  printKeyStatus(status.keys,touch_sensor.KEY_COUNT);

  Serial << endl;
  ++loop_counter;
}
