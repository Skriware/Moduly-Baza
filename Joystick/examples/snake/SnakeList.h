#include <Arduino.h>

class SnakeList
{
  public:
    SnakePosition pos;
    SnakeList *next;

    SnakeList(SnakePosition pos);
    SnakeList(SnakePosition pos, SnakeList *next);
    void push(SnakePosition pos);
    SnakeList *pop(void);
    uint8_t length(void);
    bool contains(SnakePosition *pos);
};