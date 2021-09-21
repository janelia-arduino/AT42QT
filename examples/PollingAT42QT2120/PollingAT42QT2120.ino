#include <AT42QT2120.h>
#include <Streaming.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_LOOP_DELAY = 50;

const int LOOP_DELAY = 500;

AT42QT2120 touch_sensor;

unsigned long loop_counter;

void printKeyStatus(AT42QT2120::Status status, uint8_t key_count)
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
    Serial << touch_sensor.touched(status,key);
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

  AT42QT2120::Status status = touch_sensor.getStatus();

  Serial << "status.anyTouched(): " << touch_sensor.anyTouched(status) << endl;

  Serial << "status.overflow: " << status.overflow << endl;

  Serial << "status.keys: " << _BIN(status.keys) << endl;

  printKeyStatus(status,touch_sensor.KEY_COUNT);

  uint8_t interval_count = 2;
  Serial << "setMeasurementIntervalCount(" << interval_count << ")" << endl;
  touch_sensor.setMeasurementIntervalCount(interval_count);
  interval_count = touch_sensor.getMeasurementIntervalCount();
  Serial << "getMeasurementIntervalCount(): " << interval_count << endl;

  uint8_t tdcd = 21;
  Serial << "setTowardsDriftCompensationDuration(" << tdcd << ")" << endl;
  touch_sensor.setTowardsDriftCompensationDuration(tdcd);
  tdcd = touch_sensor.getTowardsDriftCompensationDuration();
  Serial << "getTowardsDriftCompensationDuration(): " << tdcd << endl;

  uint8_t adcd = 8;
  Serial << "setAwayDriftCompensationDuration(" << adcd << ")" << endl;
  touch_sensor.setAwayDriftCompensationDuration(adcd);
  adcd = touch_sensor.getAwayDriftCompensationDuration();
  Serial << "getAwayDriftCompensationDuration(): " << adcd << endl;

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

  uint8_t dchd = 26;
  Serial << "setDriftCompensationHoldDuration(" << dchd << ")" << endl;
  touch_sensor.setDriftCompensationHoldDuration(dchd);
  dchd = touch_sensor.getDriftCompensationHoldDuration();
  Serial << "getDriftCompensationHoldDuration(): " << dchd << endl;

  Serial << "disableSliderAndWheel()" << endl;
  touch_sensor.disableSliderAndWheel();
  bool swe = touch_sensor.sliderOrWheelEnabled();
  Serial << "sliderOrWheelEnabled(): " << swe << endl;

  uint8_t cd = 255;
  Serial << "setChargeDuration(" << cd << ")" << endl;
  touch_sensor.setChargeDuration(cd);
  rd = touch_sensor.getChargeDuration();
  Serial << "getChargeDuration(): " << cd << endl;

  uint8_t key = 3;
  uint8_t threshold = 66;
  Serial << "setKeyDetectThreshold(" << key << "," << threshold << ")" << endl;
  touch_sensor.setKeyDetectThreshold(key,threshold);
  threshold = touch_sensor.getKeyDetectThreshold(key);
  Serial << "getKeyDetectThreshold(" << key << "): " << threshold << endl;

  key = 5;
  AT42QT2120::KeyControl key_control;
  uint8_t aksg = 2;
  key_control.adjacent_key_suppression_group = aksg;
  Serial << "setKeyControl: key = " << key << ", adjacent_key_suppression_group = " << aksg << endl;
  touch_sensor.setKeyControl(key,key_control);
  key_control = touch_sensor.getKeyControl(key);
  aksg = key_control.adjacent_key_suppression_group;
  Serial << "getKeyControl: key = " << key << ", adjacent_key_suppression_group = " << aksg << endl;

  key = 0;
  AT42QT2120::KeyPulseScale key_pulse_scale;
  uint8_t pulse = 0x8;
  uint8_t scale = 0x4;
  key_pulse_scale.pulse = pulse;
  key_pulse_scale.scale = scale;
  Serial << "setKeyPulseScale: key = " << key << ", pulse = " << pulse << ", scale = " << scale << endl;
  touch_sensor.setKeyPulseScale(key,key_pulse_scale);
  key_pulse_scale = touch_sensor.getKeyPulseScale(key);
  pulse = key_pulse_scale.pulse;
  scale = key_pulse_scale.scale;
  Serial << "getKeyPulseScale: key = " << key << ", pulse = " << pulse << ", scale = " << scale << endl;
  pulse = 0;
  scale = 0;
  key_pulse_scale.pulse = pulse;
  key_pulse_scale.scale = scale;
  Serial << "setKeyPulseScale: key = " << key << ", pulse = " << pulse << ", scale = " << scale << endl;
  touch_sensor.setKeyPulseScale(key,key_pulse_scale);

  key = 8;
  uint16_t signal = touch_sensor.getKeySignal(key);
  Serial << "getKeySignal(" << key << "): " << signal << endl;

  uint16_t reference = touch_sensor.getKeyReference(key);
  Serial << "getKeyReference(" << key << "): " << reference << endl;

  Serial << endl;
  ++loop_counter;
}
