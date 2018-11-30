/*
  This example is a simple game of Snake.

  Apart from this library, you'll need to download LedMatrix library
  in order to play the game.
*/

#include <Joystick.h>
#include "Snake.h"

const uint8_t joySignalX    = A0;
const uint8_t joySignalY    = A3;
const uint8_t joyVcc        = A2;
const uint8_t joyGnd        = A1;
const uint8_t ledMatrixSize = 7; 
const uint8_t ledMatrixPin  = 23;
const float   joyThreshold  = 0.75;

LedMatrix *matrix = new LedMatrix(ledMatrixSize,
                                  ledMatrixSize,
                                  ledMatrixPin);
Joystick joy(joySignalX, joySignalY);
Snake snake(matrix);

uint32_t lastJoyDebounce;
uint32_t lastSnakeMovement;

void setup()
{
  pinMode(joyVcc, OUTPUT);
  digitalWrite(joyVcc, HIGH);
  pinMode(joyGnd, OUTPUT);
  digitalWrite(joyGnd, LOW);

  matrix->begin();
  joy.begin();
  joy.calibrate(0.0, 1023.0, 45.0, 1020.0);
  snake.begin();
  Serial.begin(9600);
}

void loop()
{
  JoystickPosition pos = joy.readPosition();

  if (millis() - lastJoyDebounce > 25)
  {
    if (pos.x < -joyThreshold)
      snake.changeDirection(left);
    else if (pos.x > joyThreshold)
      snake.changeDirection(right);
    else if (pos.y < -joyThreshold)
      snake.changeDirection(up);
    else if (pos.y > joyThreshold)
      snake.changeDirection(down);

    lastJoyDebounce = millis();
  }

  if (millis() - lastSnakeMovement > 100)
  {
    snake.move();
    lastSnakeMovement = millis();
  }
}
