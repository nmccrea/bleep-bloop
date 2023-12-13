/**
 * @brief This file implements interfaces for operating the digital-analog converter (DAC).
 */

#include "dac.h"
#include "MKL25Z4.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "systick.h"
#include <stdint.h>

#define DAC_PORTE_OUTPUT_PIN (30U)
#define DAC_PORTE_OUTPUT_PIN_MUX (0)

#define DMA_DCR_SSIZE_16_BITS (2U)
#define DMA_DCR_DSIZE_16_BITS (2U)
#define DMAMUX_CHCFG_SOURCE_TPM0_OVERFLOW (54U)

// TPM prescale selection
#define DAC_TPM_CLOCK_PRESCALE (0)

// An idle signal sample buffer
DacSamples_t idle_samples = {.buffer = {0}, .length = DAC_SAMPLE_BUFFER_CAPACITY};

/** The current source of signal samples. */
DacSamples_t* source = &idle_samples;

void Dac_Initialize()
{
  // Clock Port E, DAC0, TPMO, DMAMUX, and DMA
  SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
  SIM->SCGC6 |= SIM_SCGC6_DAC0(1) | SIM_SCGC6_TPM0(1) | SIM_SCGC6_DMAMUX(1);
  SIM->SCGC7 |= SIM_SCGC7_DMA(1);

  // Connect the DAC output pin
  PORTE->PCR[DAC_PORTE_OUTPUT_PIN] = PORT_PCR_MUX(DAC_PORTE_OUTPUT_PIN_MUX);

  // Turn off DAC buffered mode
  DAC0->C1 = 0;
  DAC0->C2 = 0;

  // Enable the DAC and use VDDA as reference voltage
  DAC0->C0 = (DAC_C0_DACEN(1) | DAC_C0_DACRFS(1));

  // Disable DMA channel 0 for configuration
  DMAMUX0->CHCFG[0] = 0;

  // Configure the DMA channel 0 interrupt, peripheral requests, cycle stealing, source incrementing
  // and transfer size
  DMA0->DMA[0].DCR = DMA_DCR_EINT(1) | DMA_DCR_ERQ(1) | DMA_DCR_CS(1) | DMA_DCR_SINC(1) |
                     DMA_DCR_SSIZE(DMA_DCR_SSIZE_16_BITS) | DMA_DCR_DSIZE(DMA_DCR_DSIZE_16_BITS);

  // Enable DMA completion interrupts
  NVIC_SetPriority(DMA0_IRQn, 2);
  NVIC_ClearPendingIRQ(DMA0_IRQn);
  NVIC_EnableIRQ(DMA0_IRQn);

  // Set the TPM clock source to be the MCGFLLCLK (48MHz)
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL(1));

  // Configure the TPM period. With a 48 MHz counter and targeting a 96 kHz sample rate, this gives
  // a period of 48 MHz / 96 kHz = 500 ticks
  TPM0->MOD = (CLOCK_GetCoreSysClkFreq() / DAC_SAMPLE_FREQUENCY) - 1;

  // Start the TPM counter with the appropriate prescaler and DMA requests enabled
  TPM0->SC = TPM_SC_CMOD(1) | TPM_SC_PS(DAC_TPM_CLOCK_PRESCALE) | TPM_SC_DMA(1);
}

void Dac_WaveformToSamples(int32_t (*waveform_function)(int32_t),
                           uint16_t waveform_domain_period,
                           uint16_t output_frequency,
                           uint16_t sample_offset,
                           DacSamples_t* samples)
{
  // Coerce the output frequency to be within the allowable range
  if (output_frequency > DAC_MAX_OUTPUT_FREQUENCY) output_frequency = DAC_MAX_OUTPUT_FREQUENCY;
  if (output_frequency < DAC_MIN_OUTPUT_FREQUENCY) output_frequency = DAC_MIN_OUTPUT_FREQUENCY;

  samples->sample_period = (uint16_t)((uint32_t)DAC_SAMPLE_FREQUENCY / (uint32_t)output_frequency);

  // Here we utilize integer truncation. `samples->length` will be less than or equal to
  // `DAC_SAMPLE_BUFFER_CAPACITY`.
  uint16_t number_of_signal_periods = DAC_SAMPLE_BUFFER_CAPACITY / samples->sample_period;
  samples->length = number_of_signal_periods * samples->sample_period;

  // With integer division, we must divide the waveform input value each time to avoid accumulating
  // error.
  int32_t numerator = 0;
  for (uint16_t i = 0; i < samples->length; ++i) {
    samples->buffer[i] =
        sample_offset + waveform_function(numerator / (int32_t)samples->sample_period);
    numerator += waveform_domain_period;
  }
}

void Dac_SetSource(DacSamples_t* samples)
{
  source = samples;
}

void Dac_Play()
{
  // If the DMA is already running, do nothing
  if (DMA0->DMA[0].DSR_BCR & DMA_DSR_BCR_BSY_MASK) return;

  // Configure the source and destination addresses
  DMA0->DMA[0].SAR = (uint32_t)source->buffer;
  DMA0->DMA[0].DAR = (uint32_t)(&(DAC0->DAT[0].DATL));

  // Configure the number of bytes to transfer (2 bytes per sample)
  DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(source->length << 1);

  // Enable the DMA channel with TPM0 requests
  DMAMUX0->CHCFG[0] |=
      (DMAMUX_CHCFG_ENBL(1) | DMAMUX_CHCFG_SOURCE(DMAMUX_CHCFG_SOURCE_TPM0_OVERFLOW));
}

/**
 * @brief Resets the DMA to replay the buffered samples each time the previous DMA transfer
 * completes.
 */
void DMA0_IRQHandler()
{
  // Disable further DMA requests until the DMA has been reset
  DMAMUX0->CHCFG[0] = 0;

  // Stop playback if there was an error
  uint32_t dma_status = DMA0->DMA[0].DSR_BCR;
  if ((dma_status & DMA_DSR_BCR_CE(1)) != 0) {
    PRINTF("DMA configuration error\r\n");
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE(1);
    return;
  }
  if ((dma_status & DMA_DSR_BCR_BES(1)) != 0) {
    PRINTF("DMA bus source error\r\n");
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE(1);
    return;
  }
  if ((dma_status & DMA_DSR_BCR_BED(1)) != 0) {
    PRINTF("DMA bus destination error\r\n");
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE(1);
    return;
  }

  // Clear the status and error bits
  DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE(1);

  // Start the next DMA playback
  Dac_Play();
}