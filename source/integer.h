/**
 * @brief This header declares general-purpose integer math functions.
 */

#ifndef __INTEGER_H__
#define __INTEGER_H__

#include <stdint.h>

#define INTEGER_GREATEST_16_BIT_SIGNED_MAGNITUDE ((2U << 14U) - 1U)

/**
 * @brief Returns the square root of an unsigned integer.
 *
 * @param x The integer for which to take the square root.
 *
 * @returns The square root of x.
 */
int32_t Integer_SquareRoot(uint32_t x);

#endif