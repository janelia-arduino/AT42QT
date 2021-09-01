#include <AT42QT1060.h>
#include <AT42QT2120.h>
#include <Streaming.h>


const long BAUD = 115200;
const int LOOP_DELAY = 1000;

AT42QT1060 touch_sensor_0;
AT42QT2120 touch_sensor_1;

void setup()
{
  Serial.begin(BAUD);

  touch_sensor_0.begin();
  touch_sensor_1.begin();
}

void loop()
{
  Serial << "touch_sensor_0.DEVICE_ADDRESS: " << _HEX(touch_sensor_0.DEVICE_ADDRESS) << endl;
  Serial << "touch_sensor_1.DEVICE_ADDRESS: " << _HEX(touch_sensor_1.DEVICE_ADDRESS) << endl;

  Serial << endl;
  delay(LOOP_DELAY);
}
