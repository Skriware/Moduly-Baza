#ifndef __BH1750_H
#define __BH1750_H

extern "C"
{
  #include "stm32f0xx_hal.h"
}

class BH1750
{
  I2C_HandleTypeDef *hi2c;
  uint8_t address;
  uint8_t mode;

  static const uint8_t POWER_DOWN =                0b00000000;
  static const uint8_t POWER_ON =                  0b00000001;
  static const uint8_t RESET =                     0b00000111;

  void sendCommand(const uint8_t command);

  public:
    static const uint8_t LOW_ADDRESS =             0b0100011;
    static const uint8_t HIGH_ADDRESS =            0b0100011;

    // Mode               Resolution  Typical measurement time
    // ------------------ ----------- ------------------------
    // Low resolution           4 lx                     16 ms
    // High resolution          1 lx                    120 ms
    // High resolution 2      0.5 lx                    120 ms

    static const uint8_t CONTINUOUS_HI_RES_MODE =  0b00010000;
    static const uint8_t CONTINUOUS_HI_RES_MODE2 = 0b00010001;
    static const uint8_t CONTINUOUS_LO_RES_MODE =  0b00010011;
    static const uint8_t ONE_TIME_HI_RES_MODE =    0b00100000;
    static const uint8_t ONE_TIME_HI_RES_MODE2 =   0b00100001;
    static const uint8_t ONE_TIME_LO_RES_MODE =    0b00100011;

    static const uint8_t LO_RES_DELAY =                    16;
    static const uint8_t HI_RES_DELAY =                   120;

    BH1750(I2C_HandleTypeDef *hi2c,
           uint8_t address,
           uint8_t mode=BH1750::CONTINUOUS_HI_RES_MODE2);
    void turnOn(void);
    void turnOff(void);
    void setMode(uint8_t mode);
    float readMeasurement(void);
};

#endif
