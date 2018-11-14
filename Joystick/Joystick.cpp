#include "Joystick.h"

// Just like Arduino's map function but with float values
float floatMap(float x,
               float inMin, float inMax,
               float outMin, float outMax)
{
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

JoystickPosition::JoystickPosition(float x, float y)
{
  this->x = x;
  this->y = y;
}

Joystick::Joystick(uint8_t xSignalPin, uint8_t ySignalPin)
{
  this->xSignalPin = xSignalPin;
  this->ySignalPin = ySignalPin;
  this->xMin = 0.0;
  this->xMax = 1023.0;
  this->yMin = 0.0;
  this->yMax = 1023.0;
}

void Joystick::begin()
{
  pinMode(this->xSignalPin, INPUT);
  pinMode(this->ySignalPin, INPUT);
}

void Joystick::calibrate(float xMin, float xMax, float yMin, float yMax)
{
  this->xMin = xMin;
  this->xMax = xMax;
  this->yMin = yMin;
  this->yMax = yMax;
}

JoystickPosition Joystick::readPosition()
{
  float x = (float) analogRead(this->xSignalPin);
  float y = (float) analogRead(this->ySignalPin);

  x = floatMap(x, this->xMin, this->xMax, -1.0, 1.0);
  y = floatMap(y, this->yMin, this->yMax, -1.0, 1.0);

  JoystickPosition pos(x, y);
  return pos;
}
