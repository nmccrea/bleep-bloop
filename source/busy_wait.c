#include "busy_wait.h"
#include "systick.h"
#include <stdint.h>

void BusyWaitFor(uint32_t milliseconds)
{
  uint32_t end_time = SysTick_Now() + milliseconds;
  while (SysTick_Now() < end_time)
    ;
}

void BusyWaitUntil(ticktime_t time_milliseconds)
{
  while (SysTick_Now() < time_milliseconds)
    ;
}