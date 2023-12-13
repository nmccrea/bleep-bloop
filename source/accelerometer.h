/**
 * @brief This header declares interfaces for controlling the off-chip MPU-6050 accelerometer over
 * I2C.
 */

#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <stdint.h>

/** A 3-axis accelerometer reading in raw units.  */
typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
  uint32_t square_magnitude;
} AccelerometerData_t;

/**
 * @brief Initializes the I2C channel over which the MPU-6050 will be controlled.
 */
void Accelerometer_Initialize();

/**
 * @brief Takes a reading from the accelerometer and writes the result to the given data object.
 *
 * @param data The data object to which to write the accelerometer reading.
 */
void Accelerometer_Read(AccelerometerData_t* data);

#endif