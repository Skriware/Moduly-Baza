#include <Arduino.h>
#include <LedMatrix.h>

#include "SnakeDirection.h"
#include "SnakePosition.h"
#include "SnakeList.h"


class Snake
{
  LedMatrix *matrix;
  SnakeList *head;
  SnakePosition *apple;
  SnakePosition *digestedApple;
  SnakeDirection direction;

  public:
    Snake(LedMatrix *matrix);
    void begin(void);
    void changeDirection(SnakeDirection dir);
    void gameOver(void);
    bool eatsItself(void);
    bool eatsApple(void);
    void move(void);
};
