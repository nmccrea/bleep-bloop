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

  AccelerometerData_t accelerometer_data;

  while (1) {
    Accelerometer_Read(&accelerometer_data);
    PRINTF("x: %d, y: %d, z: %d\r\n",
           accelerometer_data.x,
           accelerometer_data.y,
           accelerometer_data.z);
    BusyWait(100);
  }

  return 0;
}
