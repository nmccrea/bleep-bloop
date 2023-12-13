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
#include <stdint.h>

#include "fsl_debug_console.h"

#define ACCELEROMETER_I2C_BAUD_RATE_BPS (300000U)
#define ACCELEROMETER_PORTE_PIN_SDA (0)
#define ACCELEROMETER_PORTE_PIN_SCL (1)
#define ACCELEROMETER_PORTE_PIN_MUX (6)

// AD0 pin low. If AD0 pin is held high, the address is 0x69.
#define ACCELEROMETER_I2C_ADDRESS (0x68)
#define ACCELEROMETER_FIRST_DATA_REGISTER (0x3B)
#define ACCELEROMETER_UNITS_PER_G (16384)

void Accelerometer_Initialize()
{
  // Initialize I2C1 peripheral
  const i2c_master_config_t i2c_config = {
      .enableMaster = true,
      .baudRate_Bps = ACCELEROMETER_I2C_BAUD_RATE_BPS,
  };
  I2C_MasterInit(I2C1, &i2c_config, CLOCK_GetBusClkFreq());

  // Configure the I2C1 SDA and SDC pins
  CLOCK_EnableClock(kCLOCK_PortE);
  const port_pin_config_t accelerometer_i2c_pin_config = {.mux = ACCELEROMETER_PORTE_PIN_MUX};
  PORT_SetPinConfig(PORTE, ACCELEROMETER_PORTE_PIN_SDA, &accelerometer_i2c_pin_config);
  PORT_SetPinConfig(PORTE, ACCELEROMETER_PORTE_PIN_SCL, &accelerometer_i2c_pin_config);
}

void Accelerometer_Read(AccelerometerData_t* data)
{
  uint8_t data_raw[6];
  i2c_master_transfer_t transfer_array = {
      .slaveAddress = ACCELEROMETER_I2C_ADDRESS,
      .direction = kI2C_Read,
      .subaddress = ACCELEROMETER_FIRST_DATA_REGISTER,
      .subaddressSize = 1,
      .data = data_raw,
      .dataSize = 6,
  };

  if (I2C_MasterTransferBlocking(I2C1, &transfer_array) != kStatus_Success) {
    PRINTF("Accelerometer read failed\r\n");
    return;
  }

  data->x = (data_raw[0] << 8) | data_raw[1];
  data->y = (data_raw[2] << 8) | data_raw[3];
  data->z = (data_raw[4] << 8) | data_raw[5];
  data->square_magnitude = (data->x * data->x) + (data->y * data->y) + (data->z * data->z);
}

void Accelerometer_PrintData(AccelerometerData_t* data)
{
  PRINTF("ACCELEROMETER: x: %d, y: %d, z: %d, magnitude: %d\r\n",
         data->x,
         data->y,
         data->z,
         data->square_magnitude);
}