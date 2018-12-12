#include "DHT11.h"

DHT11::DHT11(GPIO_TypeDef *port, uint16_t pin) : port(port), pin(pin)
{
}

// Based heavily on
// https://www.carminenoviello.com/2015/09/04/precisely-measure-microseconds-stm32/
void DHT11::delay_us(uint16_t us)
{
  uint8_t mhz = HAL_RCC_GetSysClockFreq() / 1000000;
  uint8_t coeff = mhz < 24 ? mhz / 5 : mhz / 6;

  asm volatile ("MOV R0, %[loops]\n\t"
                "1: \n\t"
                "SUB R0, #1\n\t"
                "CMP R0, #0\n\t"
                "BNE 1b \n\t" : : [loops] "r" (coeff * us) : "memory");
}

DHT11Result DHT11::read()
{
  // Pull down for 20 ms and then up for 40 us as a request for DHT11 reading.
  HAL_GPIO_WritePin(this->port, this->pin, GPIO_PIN_RESET);
  HAL_Delay(20);
  HAL_GPIO_WritePin(this->port, this->pin, GPIO_PIN_SET);
  this->delay_us(40);

  // Wait a little bit longer to let the DHT pull the wire down.
  this->delay_us(10);

  // If DHT doesn't respond, just return zeroes.
  if (HAL_GPIO_ReadPin(this->port, this->pin) == GPIO_PIN_SET)
    return DHT11Result(0, 0);

  // Wait for DHT to pull up.
  while (HAL_GPIO_ReadPin(this->port, this->pin) == GPIO_PIN_RESET);
  // Wait for DHT to pull down.
  while (HAL_GPIO_ReadPin(this->port, this->pin) == GPIO_PIN_SET);

  // Read 40 bits of data
  // For every bit of data DHT pulls the line low for 50 us and then pulls it high
  // for some time depending on whether it sends 1 or 0.

  for (uint8_t i = 0; i < 5; i++)
    this->data[i] = 0;

  uint32_t cycles[80];
  for (uint8_t i = 0; i < 80; i+=2)
  {
    cycles[i] = this->expectPulse(GPIO_PIN_RESET);
    cycles[i + 1] = this->expectPulse(GPIO_PIN_SET);
  }

  for (uint8_t i = 0; i < 40; i++)
  {
    uint32_t lowCycles = cycles[2 * i];
    uint32_t highCycles = cycles[2 * i + 1];

    this->data[i / 8] <<= 1;
    if (highCycles > lowCycles)
      this->data[i / 8] |= 1;
  }

  uint8_t checksum = 0;
  for (uint8_t i = 0; i < 4; i++)
    checksum += this->data[i];
  
  if (checksum == this->data[4])
    return DHT11Result(data[0], data[2]);
  else
    return DHT11Result(0, 0);
}

uint32_t DHT11::expectPulse(GPIO_PinState pinState)
{
  uint32_t cycles = 0;
  while (HAL_GPIO_ReadPin(this->port, this->pin) == pinState)
    cycles++;
  return cycles;
}

DHT11Result::DHT11Result(uint8_t humidity, uint8_t temperature) :
                         humidity(humidity), temperature(temperature)
{
}
