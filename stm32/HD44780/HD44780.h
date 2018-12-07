#ifndef __HD44780_H
#define __HD44780_H

extern "C"
{
  #include "stm32f0xx.h"
}

#include <stdarg.h>
#include <string.h>

class HD44780
{
  // Currently just 4-bit data bus support w/o reading busy flag.
  GPIO_TypeDef *port;
  uint16_t rsPin, enPin, db4Pin, db5Pin, db6Pin, db7Pin;

  void instructionDelay(uint8_t times=1);
  void send4Bits(uint8_t rs, uint8_t value);
  void sendInstruction(uint8_t rs, uint8_t instruction);
  void functionSet(uint8_t dataLength, uint8_t displayLines, uint8_t characterFont);
  void displayOnOffControl(uint8_t display, uint8_t cursor, uint8_t blinking);
  void setDDRAMAddress(uint8_t address);

  public:
    static const uint8_t LINE1_OFFSET = 0x0;
    static const uint8_t LINE2_OFFSET = 0x40;
    static const uint8_t LINE3_OFFSET = 0x14;
    static const uint8_t LINE4_OFFSET = 0x54;

    HD44780(GPIO_TypeDef* port,
            uint16_t rsPin, uint16_t enPin,
            uint16_t db4Pin, uint16_t db5Pin,
            uint16_t db6Pin, uint16_t db7Pin);
    void init(void);
    void entryModeSet(uint8_t increments, uint8_t shift);
    void clear(void);
    void home(void);
    void write(uint8_t data);
    void printf(const char *format, ...);
    void setCursor(uint8_t y, uint8_t x);
};

#endif
