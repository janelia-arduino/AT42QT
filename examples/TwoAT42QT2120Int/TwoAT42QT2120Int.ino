#include <AT42QT2120.h>
#include <Arduino.h>
#include <Wire.h>

constexpr int CHANGE0_PIN = 2; // change to your wiring
constexpr int CHANGE1_PIN = 3; // change to your wiring

AT42QT2120 touch0 (Wire, CHANGE0_PIN);  // bus Wire
AT42QT2120 touch1 (Wire1, CHANGE1_PIN); // bus Wire1

volatile bool changed0 = false;
volatile bool changed1 = false;

void
isr0 ()
{
  changed0 = true;
}
void
isr1 ()
{
  changed1 = true;
}

void
setup ()
{
  Serial.begin (115200);

  touch0.begin (); // Wire.begin() + config CHANGE0_PIN as INPUT_PULLUP
  touch1.begin (); // Wire1.begin() + config CHANGE1_PIN as INPUT_PULLUP

  Wire.setClock (400000);
  Wire1.setClock (400000);

  // Optional: reset + calibrate both (same pattern as your example)
  touch0.reset ();
  touch1.reset ();
  delay (2000);

  touch0.triggerCalibration ();
  touch1.triggerCalibration ();
  delay (50);
  while (touch0.calibrating () || touch1.calibrating ())
    {
      delay (50);
    }

  touch0.attachChangeCallback (isr0);
  touch1.attachChangeCallback (isr1);

  Serial.println ("Ready.");
}

void
loop ()
{
  if (changed0)
    {
      changed0 = false;
      auto st = touch0.getStatus ();
      if (touch0.anyTouched (st))
        {
          Serial.print ("touch0 keys: ");
          Serial.println (st.keys, BIN);
        }
    }

  if (changed1)
    {
      changed1 = false;
      auto st = touch1.getStatus ();
      if (touch1.anyTouched (st))
        {
          Serial.print ("touch1 keys: ");
          Serial.println (st.keys, BIN);
        }
    }
}
