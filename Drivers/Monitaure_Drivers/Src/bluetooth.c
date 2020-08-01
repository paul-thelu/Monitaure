/*
 * bluetooth.c
 *
 *  Created on: Mar 1, 2020
 *      Author: PaulT
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../inc/global.h"
#include "../../Monitaure_Drivers/Inc/bluetooth.h"

extern UART_HandleTypeDef huart2;

/**
 * \brief			Send out a serial formatted string in blocking mode over the UART COM port
 * \param[in]		*msg: data string to transmit
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t bluetooth_SerialPrint(const char *msg, ...)
{
	char buffer[100];

	va_list args;
	va_start(args, msg);
	vsprintf(buffer, msg, args);
	va_end(args);

	if (HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), UART_MAX_DELAY) != HAL_OK) return(STATUS_KO);

	return STATUS_OK;
}
