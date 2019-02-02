#ifndef DHT11_H
#define DHT11_H

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

  // Data buffer.
  uint8_t data[5];

  void delay_us(uint16_t us);

  // Counts the number of CPU cycles passed until a certain
  // type of signal is encountered.
  uint32_t expectPulse(GPIO_PinState pinState);

  public:
    // The pin should be configured as GPIO_Output Open Drain
    DHT11(GPIO_TypeDef *port, uint16_t pin);

    DHT11Result read(void);
};

#endif
