#include <Arduino.h>

float floatMap(float x,
               float inMin, float inMax,
               float outMin, float outMax);

struct JoystickPosition
{
  float x;
  float y;

  JoystickPosition(float x, float y);
};

class Joystick
{
  uint8_t xSignalPin;
  uint8_t ySignalPin;
  float xMin;
  float xMax;
  float yMin;
  float yMax;

  public:
    Joystick(uint8_t xSignalPin, uint8_t ySignalPin);
    void begin(void);
    void calibrate(void);
    void calibrate(float xMin, float xMax, float yMin, float yMax);
    JoystickPosition readPosition(void);
};
