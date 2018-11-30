Library for controlling `ny * nx` LED matrices made of WS2812B LED strips in the following manner:

           |----- nx -----|

           0 1 2 ... x-1  x   ---+  -
                                 |  |
      +--- 2x+1  ... x+2 x+1  <--+  |
      |                             |
      +--> ...  ...  ... ...  ---+  ny
                                 |  |
      +--- ...  ...  ... ...  <--+  |
      |                             |
      +--> ...  ...  ...  n         -

The library makes use of `Adafruit_NeoPixel` library.
