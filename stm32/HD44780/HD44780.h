#ifndef __HD44780_H
#define __HD44780_H

extern "C"
{
  #include "stm32f0xx.h"
  #include <stdio.h>
  #include <stdarg.h>
  #include <string.h>
}


class HD44780
{
  // Currently just single port, 4-bit data bus support.
  GPIO_TypeDef *port;
  uint16_t rsPin, enPin, db4Pin, db5Pin, db6Pin, db7Pin;

  // Switch some settings.
  //   display: 0 = display off, 1 = display on
  //   cursor: 0 = no cursor, 1 = cursor
  //   blinking: 0 = no blinking, 1 = cursor blinking
  void displayOnOffControl(uint8_t display, uint8_t cursor, uint8_t blinking);

  // Set the interface.
  //   dataLength: 0 = 4-bit data bus, 1 = 8-bit data bus
  //   displayLines: 0 = 1 line, 1 = 2 lines
  void functionSet(uint8_t dataLength, uint8_t displayLines, uint8_t characterFont);

  void delay_us(uint16_t ms);

  // Send a half of a byte to the display CGRAM or DDRAM.
  void send4Bits(uint8_t rs, uint8_t value);
  void sendInstruction(uint8_t rs, uint8_t instruction);

  // Set the Character Generator RAM address.
  //   address: 3-bit addres of modifiable character (there's 8 of them)
  //   line: number of the character's line to be modified

  // After calling this method, one should write data to the RAM.
  // For each character this method has to be called 8 times as a character
  // consists of 8 lines.
  void setCGRAMAddress(uint8_t address, uint8_t line);

  // Set the Display Data RAM address.
  void setDDRAMAddress(uint8_t address);

  public:
    // Because for some reason DDRAM is mapped rather weirdly to the actual
    // positions in the display, line offsets must be defined.
    static const uint8_t LINE1_OFFSET = 0x0;
    static const uint8_t LINE2_OFFSET = 0x40;
    static const uint8_t LINE3_OFFSET = 0x14;
    static const uint8_t LINE4_OFFSET = 0x54;

    HD44780(GPIO_TypeDef* port,
            uint16_t rsPin, uint16_t enPin,
            uint16_t db4Pin, uint16_t db5Pin,
            uint16_t db6Pin, uint16_t db7Pin);

    // Clear the DDRAM (i.e. write spaces into all DDRAM addresses) and
    // set the DDRAM address to 0. This instruction also sets the increment mode
    // leaving shift mode unchanged. 
    void clear(void);

    // Sets the entry mode.
    //   increments: 0 = decrement DDRAM address upon writing
    //                   a charater to the RAM,
    //               1 = increment
    //   shift: 0 = no shifting,
    //          1 = shifts the display while writing to the RAM
    //              (the direction is based on the `increments` value)
    void entryModeSet(uint8_t increments, uint8_t shift);

    // Sets the 3-bit address of custom character in CGRAM
    // and populates it with the data specified in 8-byte long
    // `character` array. Then it goes back to the DDRAM mode.
    void generateCharacter(uint8_t address, uint8_t *character);

    // Basically sets DDRAM address to 0.
    // If in shift mode, it shifts the display to the original location.
    void home(void);

    // Initializes the display in 20x4 chars, 4-bit data bus mode.
    void init(void);

    void printf(const char *format, ...);

    // Sets DDRAM address considering the offsets.
    //   y: line
    //   x: row
    void setCursor(uint8_t y, uint8_t x);

    // Shifts the display or the cursor.
    //   mode: 0 = cursor, 1 = display
    //   direction: 0 = left, 1 = right
    void shift(uint8_t mode, uint8_t direction);

    // Writes 8-bits to the RAM in two 4-bit chunks.
    void write(uint8_t data);
};

#endif
