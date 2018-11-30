#include "LedMatrix.h"

LedMatrix::LedMatrix(uint8_t width, uint8_t height, uint8_t signalPin)
{
  this->width = width;
  this->height = height;
  this->signalPin = signalPin;
}

void LedMatrix::begin()
{
  this->leds = new Adafruit_NeoPixel(this->width * this->height,
                                     this->signalPin);
  this->leds->begin();
}

void LedMatrix::clearScreen()
{
  this->fillScreen(0);
}

// Color based on hue (as in HSV) and brightness
uint32_t LedMatrix::color(uint8_t angle, float brightness)
{
  uint8_t r, g, b;

  if (angle < 60)
  {
    r = 255;
    g = round(angle * 4.25 - 0.01);
    b = 0;
  }
  else if (angle < 120)
  {
    r = round((120 - angle) * 4.25 - 0.01);
    g = 255;
    b = 0;
  }
  else if (angle < 180)
  {
    r = 0;
    g = 255;
    b = round((angle - 120) * 4.25 - 0.01);
  }
  else if (angle < 240)
  {
    r = 0;
    g = round((240 - angle) * 4.25 - 0.01);
    b = 255;
  }
  else if (angle < 300)
  {
    r = round((angle - 240) * 4.25 - 0.01);
    g = 0;
    b = 255;
  }
  else
  {
    r = 255;
    g = 0;
    b = round((360 - angle) * 4.25 - 0.01);
  }

  r = (uint8_t) (brightness / 255.0 * (float) r);
  g = (uint8_t) (brightness / 255.0 * (float) g);
  b = (uint8_t) (brightness / 255.0 * (float) b);

  return this->leds->Color(r, g, b);
}

uint32_t LedMatrix::color(uint8_t r, uint8_t g, uint8_t b)
{
  return this->leds->Color(r, g, b);
}

void LedMatrix::drawCrosshair(uint8_t x, uint8_t y, uint32_t color)
{
  this->setPixel(x, y, color);
  this->setPixel(x-1, y, color);
  this->setPixel(x+1, y, color);
  this->setPixel(x, y-1, color);
  this->setPixel(x, y+1, color);

  this->leds->show();
}

void LedMatrix::drawLine(uint8_t x1, uint8_t y1,
                         uint8_t x2, uint8_t y2, uint32_t color)
{
}

void LedMatrix::fillScreen(uint32_t color)
{
  for (uint8_t i = 0; i < this->width * this->height; i++)
    this->leds->setPixelColor(i, color);
}

uint8_t LedMatrix::getWidth()
{
  return this->width;
}

uint8_t LedMatrix::getHeight()
{
  return this->height;
}

// Converts cartesian coordinates to the position in the led strip
uint8_t LedMatrix::positionToLedstripNumber(uint8_t x, uint8_t y)
{
  if (y % 2 == 0)
    return y * this->height + x;
  else
    return y * this->height + this->width - x - 1;
}

// Sets pixel color based on position in the led strip
void LedMatrix::setPixel(uint8_t n, uint32_t color)
{
  this->leds->setPixelColor(n, color);
}

// Sets pixel color based on cartesian coordinates
void LedMatrix::setPixel(uint8_t x, uint8_t y, uint32_t color)
{
  this->setPixel(this->positionToLedstripNumber(x, y), color);
}

void LedMatrix::show()
{
  this->leds->show();
}
