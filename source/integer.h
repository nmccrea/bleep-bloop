/**
 * @brief This header declares general-purpose integer math functions.
 */

#ifndef __INTEGER_H__
#define __INTEGER_H__

#include <stdint.h>

/**
 * @brief Returns the square root of an integer.
 *
 * @param x The integer for which to take the square root.
 *
 * @returns The square root of x, or -1 if `x` is negative.
 */
int32_t Integer_SquareRoot(int32_t x);

#endif