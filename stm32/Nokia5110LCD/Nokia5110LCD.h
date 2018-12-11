#ifndef __NOKIA_5110_LCD_H
#define __NOKIA_5110_LCD_H

extern "C"
{
  #include "stm32f0xx.h"
  #include <stdarg.h>
  #include <string.h>
  #include <stdio.h>
}

#include "Nokia5110LCDFont.h"

/*
  SPI configuration:
    * Data size: 8 bytes
    * Baud rate: 4 Mbits/s
*/
class Nokia5110LCD
{
  static const uint8_t BLANK = 0b000;
  static const uint8_t NORMAL = 0b100;
  static const uint8_t BLACK = 0b001;
  static const uint8_t INVERSE = 0b101;

  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *rstPort;
  uint16_t rstPin;
  GPIO_TypeDef *cePort;
  uint16_t cePin;
  GPIO_TypeDef *dcPort;
  uint16_t dcPin;

  uint8_t buffer[84 * 6];
  bool inverted = false;

  // Convert cartesian coordinates to position in the screen buffer
  static uint16_t bufferPosition(uint8_t x, uint8_t y);

  void functionSet(uint8_t powerDown,
                    uint8_t verticalAddressing,
                    uint8_t extendedInstructionSet);
  void displayMode(uint8_t mode);
  void putChar(char c, uint8_t x, uint8_t y);
  void sendCommand(uint8_t command);
  void sendData(uint8_t *data, uint32_t size);
  void setAddressX(uint8_t address);
  void setAddressY(uint8_t address);
  void setPixel(uint8_t x, uint8_t y, uint8_t set);

  public:
    Nokia5110LCD(SPI_HandleTypeDef *hspi,
                 GPIO_TypeDef *rstPort,
                 uint16_t rstPin,
                 GPIO_TypeDef *cePort,
                 uint16_t cePin,
                 GPIO_TypeDef *dcPort,
                 uint16_t dcPin);
    Nokia5110LCD(SPI_HandleTypeDef *hspi, GPIO_TypeDef *port,
                 uint16_t rstPin, uint16_t cePin, uint16_t dcPin);

    // Fill the buffer with 0's
    void clear(void);

    // Initialize the display, set temp., contrast, bias, etc.
    void init(uint8_t contrast, uint8_t verticalAddressing=0);

    // Toggle between the normal and inverted display modes
    void invert(void);

    void printf(uint8_t x, uint8_t y, const char *fmt, ...);

    void bitmap(uint8_t *bitmap,
                uint8_t x, uint8_t y,
                uint16_t width, uint16_t height,
                bool transparent=false);

    void reset(void);

    // Send the buffer to the display's RAM
    void flip(void);
};

#endif
