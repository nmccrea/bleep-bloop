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
#define BLEEPBLOOP_LOOP_PERIOD_MS (50U)

#define BLEEPBLOOP_MIN_OUTPUT_FREQUENCY (DAC_MIN_OUTPUT_FREQUENCY)
#define BLEEPBLOOP_MAX_OUTPUT_FREQUENCY (2000)

/**
 * @brief Runs the main application loop.
 */
void Run()
{
  // Precalculate constants for converting accelerometer magnitude to frequency
  const uint32_t magnitude_units_per_frequency_unit =
      ACCELEROMETER_MAX_MAGNITUDE /
      (BLEEPBLOOP_MAX_OUTPUT_FREQUENCY - BLEEPBLOOP_MIN_OUTPUT_FREQUENCY);
  const uint32_t magnitude_threshold = (ACCELEROMETER_UNITS_PER_G * 10) / 8; // 1.2 Gs

  // Initialize variables
  DacSamples_t audio_buffer_a, audio_buffer_b;
  DacSamples_t* audio_buffer_next = &audio_buffer_a;
  uint16_t frequency;
  ticktime_t next_tick_time = SysTick_Now();
  AccelerometerData_t accelerometer_data;

  // Run the main loop
  while (1) {
    // Read the accelerometer data
    Accelerometer_Read(&accelerometer_data);

    // If the magnitude is below the threshold, regail us with silence
    if (accelerometer_data.magnitude < magnitude_threshold) Dac_Stop();
    // Otherwise, bleep bloop
    else {
      // Calculate the frequency to play
      frequency = DAC_MIN_OUTPUT_FREQUENCY + ((accelerometer_data.magnitude - magnitude_threshold) /
                                              magnitude_units_per_frequency_unit);

      // Generate the next waveform
      audio_buffer_next =
          (audio_buffer_next == &audio_buffer_a) ? &audio_buffer_b : &audio_buffer_a;
      Dac_WaveformToSamples(
          Waveform_Square, SQUARE_DOMAIN_PERIOD, frequency, SQUARE_SCALE_FACTOR, audio_buffer_next);

      // Play the next waveform
      Dac_SetSource(audio_buffer_next);
      Dac_Play();
    }

    // Wait until the next tick
    next_tick_time += BLEEPBLOOP_LOOP_PERIOD_MS;
    BusyWaitUntil(next_tick_time);
  }
}

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
  PRINTF("Starting BleepBloop\r\n");

  Run();

  return 0;
}
