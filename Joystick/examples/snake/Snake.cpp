#include "Snake.h"

Snake::Snake(LedMatrix *matrix)
{
  this->matrix = matrix;

  SnakePosition headPos(3, 3);
  SnakePosition tailPos(4, 3);
  this->head = new SnakeList(headPos); 
  this->head->push(tailPos);

  this->apple = new SnakePosition(1, 1);
  this->digestedApple = new SnakePosition;
  
  this->direction = left;
}

// Draw the beginning position of the snake
void Snake::begin()
{
  SnakeList *ptr = this->head;
  while (ptr != NULL)
  {
    this->matrix->setPixel(ptr->pos.x, ptr->pos.y, 0x0000ff);
    ptr = ptr->next;
  }
  this->matrix->setPixel(this->apple->x, this->apple->y, 0xff0000);
  this->matrix->show();
}

void Snake::changeDirection(SnakeDirection dir)
{
  switch (dir)
  {
    case left:
      if (this->direction != right)
        this->direction = left;
      break;
    case right:
      if (this->direction != left)
        this->direction = right;
      break;
    case up:
      if (this->direction != down)
        this->direction = up;
      break;
    case down:
      if (this->direction != up)
        this->direction = down;
      break;
  }
}

// Stop the game, show the score (green pixels) and blink infinitely
void Snake::gameOver()
{
  for (uint8_t i = 0; i < this->head->length(); i++)
  {
    this->matrix->setPixel(i, 0x00ff00);
    this->matrix->show();
    delay(100);
  }

  while (true)
  {
    this->matrix->setPixel(0, 0, 0xff0000);
    this->matrix->show();
    delay(100);
    this->matrix->setPixel(0, 0, 0);
    this->matrix->show();
    delay(100);
  }
}

bool Snake::eatsItself()
{
  return this->head->next->contains(&(this->head->pos));
}

bool Snake::eatsApple()
{
  return (this->head->pos.x == this->apple->x
          && this->head->pos.y == this->apple->y);
}

void Snake::move()
{
  if (this->eatsApple())
  {
    this->digestedApple->x = this->apple->x;
    this->digestedApple->y = this->apple->y;

    this->head->push(*(this->apple));

    // Spawn an apple randomly but avoid spawning it inside the snake
    while (this->head->contains(this->apple))
    {
      this->apple->x = random(0, this->matrix->getWidth());
      this->apple->y = random(0, this->matrix->getHeight());
    }
  }

  SnakeList *tail = this->head->pop();
  this->matrix->setPixel(tail->pos.x, tail->pos.y, 0);

  // Walking through the walls!
  switch (this->direction)
  {
    case left:
    {
      tail->pos.x = this->head->pos.x - 1;
      tail->pos.y = this->head->pos.y;

      if (this->head->pos.x - 1 < 0)
        tail->pos.x = this->matrix->getWidth() - 1;

      break;
    }
    case right:
    {
      tail->pos.x = this->head->pos.x + 1;
      tail->pos.y = this->head->pos.y;

      if (tail->pos.x >= this->matrix->getWidth())
        tail->pos.x = 0;

      break;
    }
    case up:
    {
      tail->pos.x = this->head->pos.x;
      tail->pos.y = this->head->pos.y - 1;

      if (this->head->pos.y - 1 < 0)
        tail->pos.y = this->matrix->getHeight() - 1;

      break;
    }
    case down:
    {
      tail->pos.x = this->head->pos.x;
      tail->pos.y = this->head->pos.y + 1;

      if (tail->pos.y >= this->matrix->getHeight())
        tail->pos.y = 0;

      break;
    }
  }

  // Cutting the tail of the snake
  // and making it its head.
  // `pop` method of the list ensurses that the new tail
  // doesn't point to the new head
  tail->next = this->head;
  this->head = tail;

  // This prevents gaps in snake's body after eating an apple
  if (this->head->contains(this->digestedApple))
    this->matrix->setPixel(this->digestedApple->x,
                           this->digestedApple->y, 0x0000ff);

  if (this->eatsItself())
    this->gameOver();

  this->matrix->setPixel(this->apple->x, this->apple->y, 0xff0000);
  this->matrix->setPixel(this->head->pos.x, this->head->pos.y, 0x0000ff);
  this->matrix->show();
}