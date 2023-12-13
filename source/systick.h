/**
 * @brief This header file declares an interface for SysTick timing functionality.
 */

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>

// An elapsed time counter in milliseconds
typedef uint32_t ticktime_t;

/**
 * @brief Initialize SysTick to generate an interrupt every 256th of a second.
 */
void SysTick_Initialize();

/**
 * @brief Get the current ticktime, in milliseconds, since system start.
 */
ticktime_t SysTick_Now();

#endif