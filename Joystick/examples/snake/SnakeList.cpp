#include <Arduino.h>
#include "SnakePosition.h"
#include "SnakeList.h"

SnakeList::SnakeList(SnakePosition pos)
{
  this->pos = pos;
  this->next = NULL;
}

SnakeList::SnakeList(SnakePosition pos, SnakeList *next)
{
  this->pos = pos;
  this->next = next;
}

void SnakeList::push(SnakePosition pos)
{
  SnakeList *ptr = this;
  while (ptr->next != NULL)
    ptr = ptr->next;
  SnakeList *newNode = new SnakeList(pos);
  ptr->next = newNode;
}

SnakeList *SnakeList::pop()
{
  SnakeList *ptr = this;

  while (ptr->next->next != NULL)
    ptr = ptr->next;

  SnakeList *output = ptr->next;
  ptr->next = NULL;

  return output;
}

uint8_t SnakeList::length()
{
  SnakeList *ptr = this;
  uint8_t counter = 0;
  while (ptr != NULL)
  {
    counter++;
    ptr = ptr->next;
  }
  return counter;
}

bool SnakeList::contains(SnakePosition *pos)
{
  SnakeList *ptr = this;
  while (ptr != NULL)
  {
    if (ptr->pos.x == pos->x && ptr->pos.y == pos->y)
      return true;
    ptr = ptr->next;
  }
  return false;
}