/**
 * @brief This header declares interfaces for controlling the off-chip MPU-6050 accelerometer over
 * I2C.
 */

#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

/**
 * Initializes the I2C channel over which the MPU-6050 will be controlled.
 */
void Accelerometer_Initialize();

#endif