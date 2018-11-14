/*
  This example shows how to calibrate the joystick.

  When running this sketch, move the joystick to its extreme
  points. It will calibrate automatically and print current
  values to the serial port.
*/

#define USE_LED_MATRIX false  // change to `true` if you want to
                              // use a LED matrix

#include <Joystick.h>

#if USE_LED_MATRIX
  #include <LedMatrix.h>
#endif

const uint8_t joySignalX = A0;
const uint8_t joySignalY = A1;

// Define current boundaries of the joystick
float xMin = 512.0;
float xMax = 512.0;
float yMin = 512.0;
float yMax = 512.0;

#if USE_LED_MATRIX
  LedMatrix matrix(7, 7, 23);
#endif

Joystick joy(joySignalX, joySignalY);

void setup()
{
  joy.begin();
  Serial.begin(9600);

  #if USE_LED_MATRIX
    matrix.begin();
  #endif
}

void loop()
{
  /*
    Read the absolute position of the joystick,
    i.e. the analog value of both of the joystick's pots
    and update the boundaries accordingly.
  */
  float x = (float) analogRead(joySignalX);
  float y = (float) analogRead(joySignalY);
  if (x < xMin) xMin = x;
  if (x > xMax) xMax = x;
  if (y < yMin) yMin = y;
  if (y > yMax) yMax = y;
  joy.calibrate(xMin, xMax, yMin, yMax);

  JoystickPosition pos = joy.readPosition();

  #if USE_LED_MATRIX
    matrix.clearScreen();
    matrix.drawCrosshair(3 + 3 * pos.x, 3 + 3 * pos.y, 0x0000ff);
  #endif

  Serial.print("X: [");
  Serial.print(xMin);
  Serial.print("; ");
  Serial.print(xMax);
  Serial.print("] Y: [");
  Serial.print(yMin);
  Serial.print("; ");
  Serial.print(yMax);
  Serial.print("] ");
  Serial.print("Pos: [");
  Serial.print(pos.x);
  Serial.print("; ");
  Serial.print(pos.y);
  Serial.println("]");
}
