/*
 * File:   stm32l4xx_is1678.c
 * Author: Paul Th�lu
 */

#include <string.h>
#include "../inc/global.h"
#include "../../Monitaure_Drivers/Inc/stm32l4xx_is1678.h"

status_t IS1678_Init(UART_HandleTypeDef *huart)
{
	/* Bluetooth module ON */
	HAL_GPIO_WritePin(BT_SW_BTN_GPIO_Port, BT_SW_BTN_Pin, GPIO_PIN_SET);

	/* Bluetooth module AWAKE */
	IS1678_WakeUp();

	/* Enter COMMAND mode */
	IS1678_SetCMD(huart, (uint8_t *)"$$$");

	/* Check whether we are in CMD mode or not */
	if (IS1678_CmdAck(huart) == ACK_CMD) {							/* CMD mode ON */
		do {
			IS1678_SetCMD(huart, (uint8_t *)"SQ,1008\r");			/* SW BTN activated */
		} while (IS1678_CmdAck(huart) != ACK_AOK);
		do {
			IS1678_SetCMD(huart, (uint8_t *)"SN,TickTrack\r");		/* Rename BT module */
		} while (IS1678_CmdAck(huart) != ACK_AOK);

		/* Reboot Bluetooth module */
		IS1678_SetCMD(huart, (uint8_t *)"R,1\r");					/* Reboot BT module */

		/* Exit COMMAND mode */
		IS1678_SetCMD(huart, (uint8_t *)"---\r");					/* Leaving CMD mode */
	} else {														/* CMD mode not ON */
		return STATUS_KO;
	}
	//HAL_UART_Transmit(huart, (uint8_t *)cmdBuffer, 4, 100);
	//IS1678_TxCMD(huart, (uint8_t *)"D\r");						// ECHO Mode ON
	//IS1678_SetCMD(huart, (uint8_t *)"SF,1\r");				// factory reset
	//IS1678_SetCMD(huart, (uint8_t *)"SM,6\r");
	//IS1678_SetCMD(huart, (uint8_t *)"SY,4\r");				// Max Power [0-4]
	//IS1678_SetCMD(huart, (uint8_t *)"S-,TickTrack\r");		// Rename BT module

	return STATUS_OK;
}

/* Todo
 * Check the end of the message mark instead of counting the exact number of characters to be received
 */


IS1678_ack_status_t IS1678_CmdAck(UART_HandleTypeDef *huart)
{
	uint8_t cmdPrompt[3] = {'C', 'M', 'D'};
	uint8_t aokPrompt[3] = {'A', 'O', 'K'};
	uint8_t errPrompt[3] = {'E', 'R', 'R'};
	uint8_t ackBuffer[3];	/* Should be either CMD, AOK or ERR */
	uint8_t character;
	uint8_t index = 0;

	/* Reading ACK status */
	HAL_UART_Receive(huart, (uint8_t *)ackBuffer, 3, 2000);

	if (memcmp(ackBuffer, cmdPrompt, sizeof(ackBuffer)) == 0) {				/* Checking CMD prompt ACK */
		return ACK_CMD;
	} else if (memcmp(ackBuffer, aokPrompt, sizeof(ackBuffer)) == 0) {		/* Checking AOK prompt ACK */
		return ACK_AOK;
	} else if (memcmp(ackBuffer, errPrompt, sizeof(ackBuffer)) == 0) {		/* Checking ERR prompt ACK */
		return ACK_ERR;
	} else {
		return n_ACK;														/* return not ack */
	}

	/* Checking the new line feed character "CMD>" */
	while (character != '>') {
		if (HAL_UART_Receive(huart, &character, 1, UART_MAX_DELAY) != HAL_OK) return(STATUS_KO);
		++index;
	}
}

status_t IS1678_Sleep(UART_HandleTypeDef *huart)
{
	/* Enter COMMAND mode */
	IS1678_SetCMD(huart, (uint8_t *)"$$$");
	/* Check whether we are in CMD mode or not */
	if (IS1678_CmdAck(huart) == ACK_CMD) {							/* CMD mode ON */
		while (IS1678_CmdAck(huart) != ACK_AOK) {
			IS1678_SetCMD(huart, (uint8_t *)"O,0\r");				/* Enter low power mode */
		}
		/* Exit COMMAND mode */
		IS1678_SetCMD(huart, (uint8_t *)"---\r");					/* Leaving CMD mode */
	} else {														/* CMD mode not ON */
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t IS1678_WakeUp(void)
{
	HAL_GPIO_WritePin(BT_WK_UP_GPIO_Port, BT_WK_UP_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BT_WK_UP_GPIO_Port, BT_WK_UP_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);

	return STATUS_OK;
}

status_t IS1678_SetCMD(UART_HandleTypeDef *huart, uint8_t *pData)
{
	uint8_t size = strlen((const char *)pData);

	for (uint8_t i = 0; i < size; ++i) {
		HAL_Delay(10);
		if (HAL_UART_Transmit(huart, &pData[i], 1, UART_MAX_DELAY) != HAL_OK) return(STATUS_KO);
	}

	return STATUS_OK;
}

status_t IS1678_setRFPower(UART_HandleTypeDef *huart, uint8_t rfPower)
{
	/* Enter COMMAND mode */
	IS1678_SetCMD(huart, (uint8_t *)"$$$");

	if (IS1678_CmdAck(huart) == ACK_CMD) {							/* CMD mode ON */
		switch (rfPower) {
			case 0:
				IS1678_SetCMD(huart, (uint8_t *)"SY,0\r");			/* RF set to Very Low Power */

				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			default:
				break;
		}
	}

	IS1678_SetCMD(huart, (uint8_t *)"---\r");					/* Leaving CMD mode */

	return STATUS_OK;
}
