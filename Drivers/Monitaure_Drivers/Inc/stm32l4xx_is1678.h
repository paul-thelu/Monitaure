/*
 * stm32l4xx_is1678.h
 *
 *  Created on: 11 juil. 2019
 *      Author: PaulT
 */

#ifndef MONITAURE_DRIVERS_INC_STM32L4XX_IS1678_H_
#define MONITAURE_DRIVERS_INC_STM32L4XX_IS1678_H_

typedef struct is1678_info {
	uint8_t rfPower;
} is1678_info_t;

status_t IS1678_Init(UART_HandleTypeDef *huart);
IS1678_ack_status_t IS1678_CmdAck(UART_HandleTypeDef *huart);
status_t IS1678_Sleep(UART_HandleTypeDef *huart);
status_t IS1678_WakeUp(void);
status_t IS1678_SetCMD(UART_HandleTypeDef *huart, uint8_t *pData);
status_t IS1678_setRFPower(UART_HandleTypeDef *huart, uint8_t rfPower);

#endif /* MONITAURE_DRIVERS_INC_STM32L4XX_IS1678_H_ */
