/**
 * @brief This file implements for SysTick timing functionality.
 */

#include "systick.h"
#include "MKL25Z4.h"
#include "cmsis_gcc.h"
#include "core_cm0plus.h"
#include "fsl_clock.h"
#include <stdint.h>

// SysTick frequency shall be 256 Hz
#define SYSTICK_FREQUENCY_SHIFT (8U)
// SysTick frequency, in Hz
#define SYSTICK_FREQUENCY (1U << SYSTICK_FREQUENCY_SHIFT)

// The elapsed time since system boot, in 256ths of a second.
static volatile uint32_t ticktime_raw;

void SysTick_Initialize()
{
  // Set the interrupt to the highest priority
  NVIC_EnableIRQ(SysTick_IRQn);
  NVIC_SetPriority(SysTick_IRQn, 0);

  // Configure the tick frequency
  SysTick->LOAD = (CLOCK_GetCoreSysClkFreq() >> SYSTICK_FREQUENCY_SHIFT);

  // Reset the SysTick counter
  SysTick->VAL = 0;

  // Begin SysTick operation
  SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | // Use the processor's internal 48 MHz clock
                    SysTick_CTRL_TICKINT_Msk |   // Enable SysTick interrupt
                    SysTick_CTRL_ENABLE_Msk      // Start the SysTick counter
  );
}

ticktime_t SysTick_Now()
{
  return (1000U * ticktime_raw) >> SYSTICK_FREQUENCY_SHIFT;
}

/** The SysTick interrupt handler. */
void SysTick_Handler(void)
{
  uint32_t primask = __get_PRIMASK();
  __disable_irq();

  ++ticktime_raw;

  __set_PRIMASK(primask);
}
