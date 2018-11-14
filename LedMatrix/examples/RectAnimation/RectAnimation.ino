#include <LedMatrix.h>

const int ledMatrixWidth  = 7;
const int ledMatrixHeight = 7;
const int ledMatrixPin    = 23; // Signal pin of the matrix 
const int ledMatrixDelay  = 20; // LED switching delay in ms

const uint32_t red   = 0xff0000;
const uint32_t green = 0x00ff00;
const uint32_t blue  = 0x0000ff;

LedMatrix matrix(ledMatrixWidth, ledMatrixHeight, ledMatrixPin);

void setup()
{
  matrix.begin();
}

void rectAnimation(uint32_t color, uint16_t delay_)
{
  for(int i = 0; i < matrix.getWidth(); i++)
  {
    matrix.setPixel(0, i, color);
    matrix.show();
    delay(delay_);
  }
  for(int i = 1; i < matrix.getHeight(); i++)
  {
    matrix.setPixel(i, matrix.getWidth() - 1, color);
    matrix.show();
    delay(delay_);
  }
  for(int i = matrix.getHeight() - 2; i >= 0; i--)
  {
    matrix.setPixel(matrix.getWidth() - 1, i, color);
    matrix.show();
    delay(delay_);
  }
  for(int i = matrix.getWidth() - 2; i >= 1; i--)
  {
    matrix.setPixel(i, 0, color);
    matrix.show();
    delay(delay_);
  }
}

void loop()
{
  rectAnimation(red, ledMatrixDelay);
  rectAnimation(green, ledMatrixDelay);
  rectAnimation(blue, ledMatrixDelay);
}
