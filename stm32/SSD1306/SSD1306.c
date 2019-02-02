#include "SSD1306.h"

SSD1306::SSD1306(SPI_HandleTypeDef *hspi,
  GPIO_TypeDef *rstPort, uint16_t rstPin,
  GPIO_TypeDef *dcPort, uint16_t dcPin,
  uint8_t width, uint8_t height)
  : hspi(hspi),
    rstPort(rstPort), rstPin(rstPin),
    dcPort(dcPort), dcPin(dcPin),
    width(width), height(height)
{
  this->buffer.resize(this->width * (this->height / 8));
}

void SSD1306::init()
{
  HAL_GPIO_WritePin(this->dcPort, this->dcPin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(this->rstPort, this->rstPin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(this->rstPort, this->rstPin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(this->rstPort, this->rstPin, GPIO_PIN_SET);

  uint8_t mplex = (uint8_t) this->height - 1;

  uint8_t initCmds[] = {
    SSD1306::DISPLAYOFF,
    SSD1306::SETDISPLAYCLOCKDIV,
    0x80,
    SSD1306::SETMULTIPLEX,
    mplex,
    SSD1306::SETDISPLAYOFFSET,
    0x0,
    SSD1306::SETSTARTLINE,
    SSD1306::CHARGEPUMP,
    0x14,
    SSD1306::MEMORYMODE,
    0x0,
    SSD1306::SEGREMAP | 0x1,
    SSD1306::COMSCANDEC,
    SSD1306::SETCOMPINS,
    0x12,
    SSD1306::SETCONTRAST,
    0xcf,
    SSD1306::SETPRECHARGE,
    0xf1,
    SSD1306::SETVCOMDETECT,
    0x40,
    SSD1306::DISPLAYALLON_RESUME,
    SSD1306::NORMALDISPLAY,
    SSD1306::DEACTIVATE_SCROLL,
    SSD1306::DISPLAYON
  };

  HAL_SPI_Transmit_IT(this->hspi, initCmds, sizeof(initCmds));
}

void SSD1306::home()
{
  uint8_t cmdHome[] = { 0x0, 0x10 };
  this->sendCmd(cmdHome, 2);
}

void SSD1306::clear()
{
  std::fill(buffer.begin(), buffer.end(), 0);
}

void SSD1306::sendCmd(uint8_t *cmd, int cmdLen)
{
  HAL_GPIO_WritePin(this->dcPort, this->dcPin, GPIO_PIN_RESET);
  HAL_SPI_Transmit_IT(this->hspi, cmd, cmdLen);
}

void SSD1306::sendData(uint8_t *data, int dataLen)
{
  HAL_GPIO_WritePin(this->dcPort, this->dcPin, GPIO_PIN_SET);
  HAL_SPI_Transmit_IT(this->hspi, data, dataLen);
}

void SSD1306::update()
{
  this->home();

  // It is guaranteed that std::vector stores the internal array contiguously
  this->sendData((uint8_t *) &this->buffer[0], this->buffer.size());
}

int SSD1306::bufferOffset(int x, int y)
{
  return y/8 * this->width + x;
}

int SSD1306::blockOffset(int y)
{
  return y - (int)(y/8) * 8;
}

void SSD1306::putPixel(int x, int y, const uint8_t state)
{
  int i = this->bufferOffset(x, y);

  switch (state)
  {
    case SSD1306::PIXEL_RESET:
    case SSD1306::PIXEL_SET:
      this->buffer[i] |= state << this->blockOffset(y);
      break;
    default:
      break;
  }
}

// Right now only images placed at y: y % 8 == 0 are supported
void SSD1306::bitmap(int x, int y, uint8_t *bmp, int w, int h)
{
  int bmpPos = 0;
  int dy = y;

  while (dy < h + y)
  {
    int offset = this->bufferOffset(x, y+dy);
    for (int i = offset; i < offset + w; i++)
    {
      this->buffer[i] = bmp[bmpPos];
      bmpPos++;
    }

    dy += 8;
  }
}
