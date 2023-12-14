/**
 * @brief Application entry point.
 */

#include "MKL25Z4.h"
#include "accelerometer.h"
#include "board.h"
#include "busy_wait.h"
#include "clock_config.h"
#include "dac.h"
#include "fsl_debug_console.h"
#include "integer_test.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "systick.h"
#include "waveform.h"

// 50 ms period, or 20 Hz frequency
#define SYSTEM_PERIOD_MS (50U)

#define SCREAMO_MIN_OUTPUT_FREQUENCY (DAC_MIN_OUTPUT_FREQUENCY)
#define SCREAMO_MAX_OUTPUT_FREQUENCY (2000)

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

  PRINTF("Running startup tests...\r\n");
  if (IntegerTest() > 0) {
    PRINTF("Startup tests failed. Exiting.\r\n");
    return 1;
  };

  SysTick_Initialize();
  Dac_Initialize();
  Accelerometer_Initialize();

  PRINTF("\r\n");
  PRINTF("Screamo started\r\n");

  DacSamples_t dac_samples_a;
  DacSamples_t dac_samples_b;
  DacSamples_t* dac_samples_next = &dac_samples_a;

  const uint32_t magnitude_units_per_frequency_unit =
      ACCELEROMETER_MAX_MAGNITUDE / (SCREAMO_MAX_OUTPUT_FREQUENCY - SCREAMO_MIN_OUTPUT_FREQUENCY);

  uint16_t frequency;
  ticktime_t next_tick_time = SysTick_Now();
  AccelerometerData_t accelerometer_data;
  while (1) {
    next_tick_time += SYSTEM_PERIOD_MS;

    // Read the accelerometer data
    Accelerometer_Read(&accelerometer_data);

    // Calculate the frequency to play
    frequency = DAC_MIN_OUTPUT_FREQUENCY +
                (accelerometer_data.magnitude / magnitude_units_per_frequency_unit);

    // Generate the next waveform
    dac_samples_next = (dac_samples_next == &dac_samples_a) ? &dac_samples_b : &dac_samples_a;
    Dac_WaveformToSamples(
        Waveform_Square, SQUARE_DOMAIN_PERIOD, frequency, SQUARE_SCALE_FACTOR, dac_samples_next);

    // Play the next waveform
    Dac_SetSource(dac_samples_next);
    Dac_Play();

    BusyWaitUntil(next_tick_time);
  }

  return 0;
}
