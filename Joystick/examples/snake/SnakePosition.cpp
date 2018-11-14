#include <Arduino.h>
#include "SnakePosition.h"

SnakePosition::SnakePosition() {}

SnakePosition::SnakePosition(uint8_t x, uint8_t y)
{
  this->x = x;
  this->y = y;
}