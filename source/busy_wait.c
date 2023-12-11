#include "busy_wait.h"
#include "systick.h"
#include <stdint.h>

void BusyWait(uint32_t milliseconds)
{
  uint32_t end_time = SysTick_NowMilliseconds() + milliseconds;
  while (SysTick_NowMilliseconds() < end_time)
    ;
}