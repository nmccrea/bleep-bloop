/**
 * @brief This header file declares various waveform functions.
 */

#ifndef __WAVEFORM_H__
#define __WAVEFORM_H__

#include <stdint.h>

#define SQUARE_SCALE_FACTOR (2037)
#define SQUARE_DOMAIN_PERIOD (12799)
#define SQUARE_RISING_EDGE (6399)

/**
 * @brief Computes the square wave function of `x`, given in radians.
 *
 * @param x The input value, expressed as radians * `SQUARE_SCALE_FACTOR`.
 *
 * @returns A square wave output with amplitude `SQUARE_SCALE_FACTOR` and period
 * `SQUARE_DOMAIN_PERIOD`.
 */
int32_t Waveform_Square(int32_t x);

#endif