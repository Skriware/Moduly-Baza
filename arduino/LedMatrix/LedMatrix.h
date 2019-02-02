#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedMatrix
{
  Adafruit_NeoPixel *leds;
  uint8_t width;
  uint8_t height;
  uint8_t signalPin;

  public:
    LedMatrix(uint8_t width, uint8_t height, uint8_t signalPin);

    void begin(void);
    void clearScreen(void);
    uint32_t color(uint8_t angle, float brightness);
    uint32_t color(uint8_t r, uint8_t g, uint8_t b);
    void drawCrosshair(uint8_t x, uint8_t y, uint32_t color);
    void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint32_t color);
    void fillScreen(uint32_t color);
    uint8_t getWidth();
    uint8_t getHeight();
    uint8_t positionToLedstripNumber(uint8_t x, uint8_t y);
    void setPixel(uint8_t n, uint32_t color);
    void setPixel(uint8_t x, uint8_t y, uint32_t color);
    void show(void);
};

#endif
