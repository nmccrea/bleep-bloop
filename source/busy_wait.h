#ifndef __BUSY_WAIT_H__
#define __BUSY_WAIT_H__

#include <stdint.h>

/**
 * @brief Consumes the given number of milliseconds on the CPU.
 *
 * @param milliseconds The number of milliseconds to wait.
 */
void BusyWait(uint32_t milliseconds);

#endif