/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    board.h
 * @brief   Board initialization header file.
 */

/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#ifndef _BOARD_H_
#define _BOARD_H_

/* The board name */
#define BOARD_NAME "FRDM-KL25Z"

/* The LPSCI to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPSCI
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART0
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_PllFllSelClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetPllFllSelClkFreq()
#define BOARD_UART_IRQ UART0_IRQn
#define BOARD_UART_IRQ_HANDLER UART0_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
  #define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/**
 * @brief	The flash size
 */
#define BOARD_FLASH_SIZE (0x0U)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief 	Initialize board specific settings.
 */
void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
