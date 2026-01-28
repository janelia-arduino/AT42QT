#include <AT42QT2120.h>
#include <Arduino.h>
#include <Wire.h>

// If you don’t use CHANGE or RESET pins, you can just pass the wire:
AT42QT2120 touch0 (Wire);  // I2C bus 0
AT42QT2120 touch1 (Wire1); // I2C bus 1

void
setup ()
{
  Serial.begin (115200);

  touch0.begin (); // calls Wire.begin()
  touch1.begin (); // calls Wire1.begin()

  // Optional: speed
  Wire.setClock (400000);
  Wire1.setClock (400000);

  // Optional sanity check
  Serial.print ("touch0 communicating: ");
  Serial.println (touch0.communicating ());
  Serial.print ("touch1 communicating: ");
  Serial.println (touch1.communicating ());
}

void
loop ()
{
  auto s0 = touch0.getStatus ();
  auto s1 = touch1.getStatus ();

  if (touch0.anyTouched (s0))
    {
      Serial.print ("touch0 keys: ");
      Serial.println (s0.keys, BIN);
    }
  if (touch1.anyTouched (s1))
    {
      Serial.print ("touch1 keys: ");
      Serial.println (s1.keys, BIN);
    }

  delay (20);
}
