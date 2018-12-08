#ifndef __DHT11_H_
#define __DHT11_H_

extern "C"
{
  #include "stm32f0xx.h"
}

struct DHT11Result
{
  uint8_t humidity;
  uint8_t temperature;

  DHT11Result(uint8_t humidity, uint8_t temperature);
};

class DHT11
{
  GPIO_TypeDef *port;
  uint16_t pin;
  uint8_t data[5];

  void delay_us(uint16_t us);
  uint32_t expectPulse(GPIO_PinState pinState);

  public:
    // The pin should be configured as GPIO_Output Open Drain
    DHT11(GPIO_TypeDef *port, uint16_t pin);

    DHT11Result read(void);
};

#endif