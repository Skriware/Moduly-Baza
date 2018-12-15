#include "Nokia5110LCD.h"

Nokia5110LCD::Nokia5110LCD(SPI_HandleTypeDef *hspi,
                           GPIO_TypeDef *rstPort,
                           uint16_t rstPin,
                           GPIO_TypeDef *cePort,
                           uint16_t cePin,
                           GPIO_TypeDef *dcPort,
                           uint16_t dcPin) : hspi(hspi),
                                             rstPort(rstPort), rstPin(rstPin),
                                             cePort(cePort), cePin(cePin),
                                             dcPort(dcPort), dcPin(dcPin)
{
}

Nokia5110LCD::Nokia5110LCD(SPI_HandleTypeDef *hspi,
                           GPIO_TypeDef *port,
                           uint16_t rstPin,
                           uint16_t cePin,
                           uint16_t dcPin) : hspi(hspi),
                                             rstPort(port), rstPin(rstPin),
                                             cePort(port), cePin(cePin),
                                             dcPort(port), dcPin(dcPin)
{
}

void Nokia5110LCD::bitmap(uint8_t *bitmap,
                          uint8_t x, uint8_t y,
                          uint16_t width, uint16_t height,
                          bool transparent)
{
  uint8_t dx = 0;
  uint8_t dy = 0;

  for (int line = 0; line < width * (height / 8); line++)
  {
    for (int i = 0; i < 8; i++)
    {
      uint8_t pixel_state = (uint8_t) !!(bitmap[line] & (1 << i));

      if ((transparent && pixel_state == 1) || ! transparent)
        this->setPixel(x + dx, y + dy, pixel_state);

      dy++;
    }
    dy -= 8;
    dx++;
    if (dx >= width)
    {
      dx = 0;
      dy += 8;
    }
  }
}

uint16_t Nokia5110LCD::bufferPosition(uint8_t x, uint8_t y)
{
  return (y / 8) * 84 + x;
}

void Nokia5110LCD::clear()
{
  for (int i = 0; i < sizeof(this->buffer); i++)
    (this->buffer)[i] = 0;
}

void Nokia5110LCD::displayMode(uint8_t mode)
{
  this->sendCommand(0b00001000 | mode);
}

void Nokia5110LCD::flip()
{
  this->setAddressX(0);
  this->setAddressY(0);
  this->sendData(this->buffer, sizeof(this->buffer));
}

void Nokia5110LCD::functionSet(uint8_t powerDown,
                               uint8_t verticalAddressing,
                               uint8_t extendedInstructionSet)
{
  this->sendCommand(0b00100000
                    | (powerDown << 2)
                    | (verticalAddressing << 1)
                    | extendedInstructionSet);
}

void Nokia5110LCD::init(uint8_t contrast, uint8_t verticalAddressing)
{
  this->reset();
  this->functionSet(0, verticalAddressing, 1);
  this->sendCommand(0b10000000 | contrast);
  this->sendCommand(0b00000110); // temperature coeff = 2
  this->sendCommand(0b00010011); // bias = 3
  this->functionSet(0, verticalAddressing, 0);
  this->setAddressX(0);
  this->setAddressY(0);
  this->displayMode(Nokia5110LCD::NORMAL);
}

void Nokia5110LCD::invert()
{
  if (this->inverted)
  {
    this->displayMode(Nokia5110LCD::NORMAL);
    this->inverted = false;
  }
  else
  {
    this->displayMode(Nokia5110LCD::INVERSE);
    this->inverted = true;
  }
}

void Nokia5110LCD::printf(uint8_t x, uint8_t y, const char *fmt, ...)
{
  uint8_t dx = 0;
  char buf[32];

  va_list args;
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);

  for (int i = 0; i < strlen(buf); i++)
  {
    this->putChar(buf[i], x + dx, y);
    dx += 6;
  }
}

void Nokia5110LCD::putChar(char c, uint8_t x, uint8_t y)
{
  this->bitmap((uint8_t *) Nokia5110LCDFont[(uint8_t) c - 32], x, y, 6, 8);
}

void Nokia5110LCD::reset()
{
  HAL_GPIO_WritePin(this->rstPort, this->rstPin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(this->rstPort, this->rstPin, GPIO_PIN_SET);
}

void Nokia5110LCD::sendCommand(uint8_t command)
{
  HAL_GPIO_WritePin(this->cePort, this->cePin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(this->dcPort, this->dcPin, GPIO_PIN_RESET);

  HAL_SPI_Transmit(this->hspi, &command, 1, 10);

  HAL_GPIO_WritePin(this->cePort, this->cePin, GPIO_PIN_SET);
}

void Nokia5110LCD::sendData(uint8_t *data, uint32_t size)
{
  HAL_GPIO_WritePin(this->cePort, this->cePin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(this->dcPort, this->dcPin, GPIO_PIN_SET);

  HAL_SPI_Transmit(this->hspi, data, size, 10);

  HAL_GPIO_WritePin(this->cePort, this->cePin, GPIO_PIN_SET);
}

void Nokia5110LCD::setAddressX(uint8_t address)
{
  if (address >= 0 && address < 84)
    this->sendCommand(0b10000000 | address);
}

void Nokia5110LCD::setAddressY(uint8_t address)
{
  if (address >= 0 && address < 5)
    this->sendCommand(0b01000000 | address);
}

void Nokia5110LCD::setPixel(uint8_t x, uint8_t y, uint8_t set)
{
  uint16_t bufPos = Nokia5110LCD::bufferPosition(x, y);

  if (set)
    this->buffer[bufPos] |= 1 << (y % 8);
  else
    this->buffer[bufPos] &= ~(1 << (y % 8));
}