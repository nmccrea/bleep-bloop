#ifndef __BUSY_WAIT_H__
#define __BUSY_WAIT_H__

#include "systick.h"
#include <stdint.h>

/**
 * @brief Consumes the given number of milliseconds on the CPU.
 *
 * @param milliseconds The number of milliseconds to wait.
 */
void BusyWaitFor(uint32_t milliseconds);

/**
 * @brief Consumes the CPU until the given ticktime has been reached.
 *
 * @param time_milliseconds The ticktime until which to wait.
 */
void BusyWaitUntil(ticktime_t time_milliseconds);

#endif