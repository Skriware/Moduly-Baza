# BH1750

A library for BH1750 light sensor.

# Usage example

An I2C interface must be enabled.

    #include "BH1750.h"

    BH1750 lightSensor(&hi2c1, BH1750::LOW_ADDRESS);

    // some code

    int main()
    {
      // some code

      lightSensor.setMode(BH1750::CONTINUOUS_HI_RES_MODE2);
      float lx;

      while (1)
      {
        HAL_Delay(BH1750::HI_RES_DELAY);
        lx = lightSensor.readMeasurement();
      }
    }

## Planned features

* DMA support
* Measurement Time Register (MTREG) support