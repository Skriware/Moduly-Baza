/*
  This example shows the basic usage of the library.
  The joystick position at any given moment is printed to 
  the serial port.

  If your axes values don't reach [-1.0; 1.0] boundaries,
  you might want to consider using `calibrate` method
  presented in a different sample sketch.
*/

#include <Joystick.h>

const int joySignalX = A0;
const int joySignalY = A1;

Joystick joy(joySignalX, joySignalY);

void setup()
{
  Serial.begin(9600);
  joy.begin();
}

void loop()
{
  JoystickPosition pos = joy.readPosition();

  Serial.print("x: ");
  Serial.print(pos.x);
  Serial.print(" y: ");
  Serial.println(pos.y);
}
