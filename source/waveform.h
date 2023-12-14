/**
 * @brief This header file declares various waveform functions.
 */

#ifndef __WAVEFORM_H__
#define __WAVEFORM_H__

#include "fp_trig.h"
#include <stdint.h>

#define SQUARE_SCALE_FACTOR (2037)
#define SQUARE_DOMAIN_PERIOD (TWO_PI)
#define SQUARE_RISING_EDGE (PI)

/**
 * @brief Computes the square wave function of `x`, given in `fp_trig` radians.
 *
 * @param x The input value, expressed as radians * `SQUARE_SCALE_FACTOR`.
 *
 * @returns A square wave output with amplitude `SQUARE_SCALE_FACTOR` and period
 * `SQUARE_DOMAIN_PERIOD`.
 */
int32_t Waveform_Square(int32_t x);

#define SINE_SCALE_FACTOR (TRIG_SCALE_FACTOR)
#define SINE_DOMAIN_PERIOD (TWO_PI)

/**
 * @brief Computes the sine wave function of `x`, given in `fp_trig` radians.
 *
 * @param x The input value, expressed as radians * `SINE_SCALE_FACTOR`.
 *
 * @returns A square wave output with amplitude `SINE_SCALE_FACTOR` and period `SINE_DOMAIN_PERIOD`.
 */
int32_t Waveform_Sine(int32_t x);

#endif