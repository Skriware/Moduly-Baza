#include "BH1750.h"

BH1750::BH1750(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t mode, uint8_t mtreg)
              : hi2c(hi2c), address(address), mode(mode), mtreg(mtreg)
{
}

float BH1750::readMeasurement()
{
  uint8_t recvData[2];
  HAL_I2C_Master_Receive(this->hi2c,
                         (this->address << 1) | 1,
                         recvData, 2, 10);

  // "Glue" two received bytes together into one number.
  uint16_t rawResult = recvData[1] | (recvData[0] << 8);

  return this->convertResult(rawResult);
}

float BH1750::convertResult(uint16_t rawResult)
{
  float result = 0.0;

  switch (this->mode)
  {
    case BH1750::ONE_TIME_LO_RES_MODE:
    case BH1750::CONTINUOUS_LO_RES_MODE:
    case BH1750::ONE_TIME_HI_RES_MODE:
    case BH1750::CONTINUOUS_HI_RES_MODE:
      result = (float) rawResult;
      break;
    case BH1750::ONE_TIME_HI_RES_MODE2:
    case BH1750::CONTINUOUS_HI_RES_MODE2:
      result = 0.5 * (float) (rawResult & 1);
      result += (float) (rawResult >> 1);
      break;
    default:
      result = -1.0;
      break;
  }

  return result / 1.2;
}

void BH1750::reset()
{
  this->sendCommand(BH1750::RESET);
}

void BH1750::sendCommand(const uint8_t command)
{
  HAL_I2C_Master_Transmit(this->hi2c,
                          (this->address << 1) | 1,
                          (uint8_t *) &command, 1, 10);
}

void BH1750::setMeasurementTime(uint8_t mtreg)
{
  this->mtreg = mtreg;

  // Set high bits of MTREG
  this->sendCommand(0b01000000 | this->mtreg >> 5);
  // Set low bits of MTREG
  this->sendCommand(0b01100000 | (this->mtreg & 0b00011111));
}

void BH1750::setMode(uint8_t mode)
{
  this->turnOn();
  this->mode = mode;
  this->sendCommand(this->mode);
}

void BH1750::turnOff()
{
  this->sendCommand(BH1750::POWER_DOWN);
}

void BH1750::turnOn()
{
  this->sendCommand(BH1750::POWER_ON);
}