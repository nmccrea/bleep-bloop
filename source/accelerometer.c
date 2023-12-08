/**
 * @brief This file implements interfaces for controlling the off-chip MPU-6050 accelerometer over
 * I2C.
 */

#include "accelerometer.h"
#include "MKL25Z4.h"
#include "fsl_clock.h"
#include "fsl_device_registers.h"
#include "fsl_i2c.h"
#include "fsl_port.h"

// AD0 pin low. If AD0 pin is held high, the address is 0x69.
#define ACCELEROMETER_I2C_ADDRESS (0x68)
#define ACCELEROMETER_BAUD_RATE_BPS (300000U)
#define ACCELEROMETER_PORTE_PIN_SDA (0)
#define ACCELEROMETER_PORTE_PIN_SCL (1)
#define ACCELEROMETER_PORTE_PIN_MUX (6)

void Accelerometer_Initialize()
{
  // Initialize I2C1 peripheral
  const i2c_master_config_t i2c_config = {
      .enableMaster = true,
      .baudRate_Bps = ACCELEROMETER_BAUD_RATE_BPS,
  };
  I2C_MasterInit(I2C1, &i2c_config, CLOCK_GetBusClkFreq());

  // Configure the I2C1 SDA and SDC pins
  CLOCK_EnableClock(kCLOCK_PortE);
  const port_pin_config_t accelerometer_i2c_pin_config = {.mux = ACCELEROMETER_PORTE_PIN_MUX};
  PORT_SetPinConfig(PORTE, ACCELEROMETER_PORTE_PIN_SDA, &accelerometer_i2c_pin_config);
  PORT_SetPinConfig(PORTE, ACCELEROMETER_PORTE_PIN_SCL, &accelerometer_i2c_pin_config);
}