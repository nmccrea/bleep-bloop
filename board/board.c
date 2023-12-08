/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    board.c
 * @brief   Board initialization file.
 */

/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include "board.h"
#include "fsl_common.h"
#include <stdint.h>

#if BOARD_FLASH_SIZE == 0
  #warning "BOARD_FLASH_SIZE not set; using 0 as default value"
#endif

/**
 * @brief Set up and initialize all required blocks and functions related to the board hardware.
 */
void BOARD_InitDebugConsole(void)
{
  uint32_t uartClkSrcFreq;
  /* SIM_SOPT2[27:26]:
   *  00: Clock Disabled
   *  01: IRC48M
   *  10: OSCERCLK
   *  11: MCGIRCCLK
   */
  CLOCK_SetLpsci0Clock(1);

  uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
  DbgConsole_Init(
      BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
