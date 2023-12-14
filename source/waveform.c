/**
 * @brief This file implements various waveform functions.
 */

#include "waveform.h"
#include "fp_trig.h"
#include <stdint.h>

int32_t Waveform_Square(int32_t x)
{
  // Bring `x` into the fundamental domain [0, <period>)
  while (x >= SQUARE_DOMAIN_PERIOD)
    x -= SQUARE_DOMAIN_PERIOD;
  while (x < 0)
    x += SQUARE_DOMAIN_PERIOD;

  // Calculate result
  if (x < SQUARE_RISING_EDGE)
    return -SQUARE_SCALE_FACTOR;
  else
    return SQUARE_SCALE_FACTOR;
}

int32_t Waveform_Sine(int32_t x)
{
  return fp_sin(x);
}