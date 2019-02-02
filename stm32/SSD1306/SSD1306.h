#ifndef SSD1306_H
#define SSD1306_H

#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <vector>

#include "spi.h"
#include "SSD1306Font.h"

class SSD1306
{
  public:
    static const uint8_t MEMORYMODE = 0x20;
    static const uint8_t COLUMNADDR = 0x21;
    static const uint8_t PAGEADDR = 0x22;
    static const uint8_t SETCONTRAST = 0x81;
    static const uint8_t CHARGEPUMP = 0x8D;
    static const uint8_t SEGREMAP = 0xA0;
    static const uint8_t DISPLAYALLON_RESUME = 0xA4;
    static const uint8_t DISPLAYALLON = 0xA5;
    static const uint8_t NORMALDISPLAY = 0xA6;
    static const uint8_t INVERTDISPLAY = 0xA7;
    static const uint8_t SETMULTIPLEX = 0xA8;
    static const uint8_t DISPLAYOFF = 0xAE;
    static const uint8_t DISPLAYON = 0xAF;
    static const uint8_t COMSCANINC = 0xC0;
    static const uint8_t COMSCANDEC = 0xC8;
    static const uint8_t SETDISPLAYOFFSET = 0xD3;
    static const uint8_t SETDISPLAYCLOCKDIV = 0xD5;
    static const uint8_t SETPRECHARGE = 0xD9;
    static const uint8_t SETCOMPINS = 0xDA;
    static const uint8_t SETVCOMDETECT = 0xDB;

    static const uint8_t SETLOWCOLUMN = 0x00;
    static const uint8_t SETHIGHCOLUMN = 0x10;
    static const uint8_t SETSTARTLINE = 0x40;

    static const uint8_t EXTERNALVCC = 0x01;
    static const uint8_t SWITCHCAPVCC = 0x02;

    static const uint8_t RIGHT_HORIZONTAL_SCROLL = 0x26;
    static const uint8_t LEFT_HORIZONTAL_SCROLL = 0x27;
    static const uint8_t VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;
    static const uint8_t VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;
    static const uint8_t DEACTIVATE_SCROLL = 0x2E;
    static const uint8_t ACTIVATE_SCROLL = 0x2F;
    static const uint8_t SET_VERTICAL_SCROLL_AREA = 0xA3;

    static const uint8_t PIXEL_RESET = 0;
    static const uint8_t PIXEL_SET = 1;
    static const uint8_t PIXEL_TOGGLE = 2;

  private:
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *rstPort;
    uint16_t rstPin;
    GPIO_TypeDef *dcPort;
    uint16_t dcPin;

  public:
    std::vector<uint8_t> buffer;
    uint8_t width;
    uint8_t height;


    SSD1306(SPI_HandleTypeDef *hspi,
      GPIO_TypeDef *rstPort, uint16_t rstPin,
      GPIO_TypeDef *dcPort, uint16_t dcPin,
      uint8_t width, uint8_t height);

    void home(void);
    void init(void);
    void clear(void);
    void sendCmd(uint8_t *cmd, int cmdLen);
    void sendData(uint8_t *data, int dataLen);
    void update(void);
    void putPixel(int x, int y, const uint8_t state);
    void bitmap(int x, int y, uint8_t *bmp, int w, int h, bool transparent=false);
    int bufferOffset(int x, int y);
    int blockOffset(int y);
    void printf(int x, int y, const char *fmt, ...);
    void putchar(int x, int y, char c);
};

#endif
