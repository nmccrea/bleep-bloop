/**
 * @brief Application entry point.
 */

#include "MKL25Z4.h"
#include "accelerometer.h"
#include "board.h"
#include "busy_wait.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "systick.h"

// 50 ms period, or 20 Hz frequency
#define SYSTEM_PERIOD_MS (50U)

/**
 * @brief Application entry point.
 */
int main(void)
{
  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_BootClockRUN();
  BOARD_InitBootPeripherals();
  BOARD_InitDebugConsole();

  SysTick_Initialize();
  Accelerometer_Initialize();

  PRINTF("Screamo started\r\n");

  ticktime_t next_tick_time = SysTick_Now();
  AccelerometerData_t accelerometer_data;
  while (1) {
    next_tick_time += SYSTEM_PERIOD_MS;
    Accelerometer_Read(&accelerometer_data);
    Accelerometer_PrintData(&accelerometer_data);
    BusyWaitUntil(next_tick_time);
  }

  return 0;
}
