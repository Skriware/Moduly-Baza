#include "HD44780.h"

HD44780::HD44780(GPIO_TypeDef* port,
                 uint16_t rsPin, uint16_t enPin,
                 uint16_t db4Pin, uint16_t db5Pin,
                 uint16_t db6Pin, uint16_t db7Pin)
               : port(port),
                 rsPin(rsPin), enPin(enPin),
                 db4Pin(db4Pin), db5Pin(db5Pin),
                 db6Pin(db6Pin), db7Pin(db7Pin)
{
}

void HD44780::clear()
{
  this->sendInstruction(0, 1);
  HAL_Delay(1);
}

void HD44780::displayOnOffControl(uint8_t display, uint8_t cursor, uint8_t blinking)
{
  this->sendInstruction(0, 0b00001000
                           | (display << 2)
                           | (cursor << 1)
                           | blinking);
}

void HD44780::entryModeSet(uint8_t increments, uint8_t shift)
{
  this->sendInstruction(0, 0b00000100
                          | (increments << 1)
                          | shift);
}

void HD44780::functionSet(uint8_t dataLength, uint8_t displayLines, uint8_t characterFont)
{
  this->sendInstruction(0, 0b00100000
                           | (dataLength << 4)
                           | (displayLines << 3)
                           | (characterFont << 2));
  HAL_Delay(5);
}

void HD44780::generateCharacter(uint8_t address, uint8_t *character)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    this->setCGRAMAddress(address, i);
    this->write(character[i]);
  }
  
  this->setDDRAMAddress(0);
}

void HD44780::home()
{
  this->sendInstruction(0, 2);
}

// Initialization for 20x4 display
void HD44780::init()
{
  this->functionSet(0, 1, 0);
  this->displayOnOffControl(1, 0, 0);
  this->entryModeSet(1, 0);
  this->clear();
  HAL_Delay(10);
}

void HD44780::delay_us(uint16_t us)
{
  uint8_t mhz = HAL_RCC_GetSysClockFreq() / 1000000;
  uint8_t coeff = mhz < 24 ? mhz / 5 : mhz / 6;

  asm volatile ("MOV R0, %[loops]\n\t"
                "1: \n\t"
                "SUB R0, #1\n\t"
                "CMP R0, #0\n\t"
                "BNE 1b \n\t" : : [loops] "r" (coeff * us) : "memory");
}

void HD44780::printf(const char *format, ...)
{
  char buffer[20 * 4];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  for (uint8_t i = 0; i < strlen(buffer); i++)
    this->write(buffer[i]);
}

void HD44780::send4Bits(uint8_t rs, uint8_t value)
{
    HAL_GPIO_WritePin(this->port, this->rsPin, (GPIO_PinState) rs);
    HAL_GPIO_WritePin(this->port, this->enPin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(this->port, this->db7Pin, (GPIO_PinState) (value & 8));
    HAL_GPIO_WritePin(this->port, this->db6Pin, (GPIO_PinState) (value & 4));
    HAL_GPIO_WritePin(this->port, this->db5Pin, (GPIO_PinState) (value & 2));
    HAL_GPIO_WritePin(this->port, this->db4Pin, (GPIO_PinState) (value & 1));

    HAL_GPIO_WritePin(this->port, this->enPin, GPIO_PIN_SET);
    this->delay_us(37);
    HAL_GPIO_WritePin(this->port, this->enPin, GPIO_PIN_RESET);
}

void HD44780::sendInstruction(uint8_t rs, uint8_t instruction)
{
  this->send4Bits(rs, instruction >> 4);
  this->send4Bits(rs, instruction);
  HAL_Delay(1); // let the command settle
}

void HD44780::setCursor(uint8_t y, uint8_t x)
{
  switch (y)
  {
    case 0:
      this->setDDRAMAddress(HD44780::LINE1_OFFSET + x);
      break;
    case 1:
      this->setDDRAMAddress(HD44780::LINE2_OFFSET + x);
      break;
    case 2:
      this->setDDRAMAddress(HD44780::LINE3_OFFSET + x);
      break;
    case 3:
      this->setDDRAMAddress(HD44780::LINE4_OFFSET + x);
      break;
    default:
      this->setDDRAMAddress(HD44780::LINE1_OFFSET);
      break;
  }
}

void HD44780::setCGRAMAddress(uint8_t address, uint8_t line)
{
  this->sendInstruction(0, 0b01000000 | (address << 3) | line);
}

void HD44780::setDDRAMAddress(uint8_t address)
{
  this->sendInstruction(0, 0b10000000 | address);
}

void HD44780::shift(uint8_t mode, uint8_t direction)
{
  this->sendInstruction(0, 0b00010000 | (mode << 3) | (direction << 2));
}

void HD44780::write(uint8_t data)
{
  this->sendInstruction(1, data);
}
