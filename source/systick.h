/**
 * @brief This header file declares an interface for SysTick timing functionality.
 */

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>

// SysTick frequency shall be 64 Hz
#define SYSTICK_FREQUENCY_SHIFT (6U)
// SysTick frequency, in Hz
#define SYSTICK_FREQUENCY (1U << SYSTICK_FREQUENCY_SHIFT)

// An elapsed time counter in sixteenths of a second.
typedef uint32_t ticktime_t;

/**
 * @brief Initialize SysTick to generate an interrupt every 64th of a second.
 */
void SysTick_Initialize();

/**
 * @brief Get the current ticktime, in 64ths of a second since ticktime reset.
 */
ticktime_t SysTick_Now();

/**
 * @brief Get the current ticktime, in milliseconds since ticktime reset.
 */
uint32_t SysTick_NowMilliseconds();

#endif