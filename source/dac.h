/**
 * @brief This header declares interfaces for operating the digital-analog converter (DAC).
 */

#ifndef __DAC_H__
#define __DAC_H__

#include <stdint.h>

// DAC sample frequency in Hz
#define DAC_SAMPLE_FREQUENCY (96000U)
#define DAC_SAMPLE_BUFFER_CAPACITY (1024)
#define DAC_MAX_OUTPUT_FREQUENCY (48000)
#define DAC_MIN_OUTPUT_FREQUENCY (94) // DAC_SAMPLE_FREQUENCY / DAC_SAMPLE_BUFFER_CAPACITY

/** A DAC sample buffer and associated properties. */
typedef struct {
  // The sample buffer
  uint16_t buffer[DAC_SAMPLE_BUFFER_CAPACITY];

  // The number of samples currently held in the buffer
  uint16_t length;

  // The number of samples per period of the signal
  uint16_t sample_period;
} DacSamples_t;

/**
 * @brief Initializes the DAC module and enables periodic sample transfers with DMA.
 */
void Dac_Initialize();

/**
 * @brief Writes a waveform into a DAC sample buffer.
 *
 * @param waveform_function The waveform function.
 * @param waveform_domain_period The fundamental period of the waveform in the function's input
 * units. That is, the non-negative input value at which the waveform function first repeats.
 * @param output_frequency The desired analog signal frequency in Hz. The output frequency will be
 * coerced to be within the allowable range.
 * @param sample_offset An offset value that will be added to the output value of the waveform
 * function before writing to the sample buffer.
 * @param samples The DAC samples object into which the samples and metadata will be written.
 */
void Dac_WaveformToSamples(int32_t (*waveform_function)(int32_t),
                           uint16_t waveform_domain_period,
                           uint16_t output_frequency,
                           uint16_t sample_offset,
                           DacSamples_t* samples);

/**
 * @brief Sets the DAC sample source buffer. If the DAC is already playing, the new source will
 * begin playing once any current DMA transfer is complete.
 *
 * @param samples The DAC samples object that will become the new signal source.
 */
void Dac_SetSource(DacSamples_t* samples);

/**
 * @brief Begins playing out the signal provided in the configured sample source buffer. The signal
 * in the buffer will be looped over indefinitely.
 */
void Dac_Play();

/**
 * @brief Stops playing the current signal.
 */
void Dac_Stop();

#endif