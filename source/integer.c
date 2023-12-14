/**
 * @brief This file implements general-purpose integer math functions.
 */

#include "integer.h"
#include <stdint.h>

int32_t Integer_SquareRoot(uint32_t value)
{
  // Handle zero input
  if (value == 0) return 0;

  // Perform successive-average search for the square root (Heron's method)
  uint32_t lower_bound = 1;
  uint32_t upper_bound = value;
  uint32_t guess = (lower_bound + upper_bound) / 2;
  while (guess > lower_bound) {
    uint64_t square = (uint64_t)guess * (uint64_t)guess;
    if (square == value)
      return guess;
    else if (square > value)
      upper_bound = guess;
    else
      lower_bound = guess;
    guess = (lower_bound + upper_bound) / 2;
  }

  return guess;
}
