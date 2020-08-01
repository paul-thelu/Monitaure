/*
 * global.h
 *
 *  Created on: Jan 18, 2020
 *      Author: PaulT
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "stm32l4xx_hal.h"

typedef enum
{
	STATUS_OK = 0,
	STATUS_KO = 1
} status_t;

typedef enum
{
	ACK_CMD,
	ACK_AOK,
	ACK_ERR,
	n_ACK,
} IS1678_ack_status_t;

/* Public defines ------------------------------------------------------------*/
#define INT_TMP_Pin GPIO_PIN_13
#define INT_TMP_GPIO_Port GPIOC
#define INT_OXI_Pin GPIO_PIN_0
#define INT_OXI_GPIO_Port GPIOA
#define BT_SW_BTN_Pin GPIO_PIN_1
#define BT_SW_BTN_GPIO_Port GPIOA
#define BT_WK_UP_Pin GPIO_PIN_4
#define BT_WK_UP_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_0
#define OLED_RST_GPIO_Port GPIOB
#define OLED_D_C_Pin GPIO_PIN_1
#define OLED_D_C_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_2
#define OLED_CS_GPIO_Port GPIOB
#define _13V_EN_Pin GPIO_PIN_10
#define _13V_EN_GPIO_Port GPIOB
#define INT_IMU2_Pin GPIO_PIN_8
#define INT_IMU2_GPIO_Port GPIOA
#define INT_IMU1_Pin GPIO_PIN_11
#define INT_IMU1_GPIO_Port GPIOA
#define GPOUT_Pin GPIO_PIN_12
#define GPOUT_GPIO_Port GPIOA
#define SST_CS_Pin GPIO_PIN_15
#define SST_CS_GPIO_Port GPIOA
#define CHG_DET_Pin GPIO_PIN_8
#define CHG_DET_GPIO_Port GPIOB
#define VIBRATOR_Pin GPIO_PIN_9
#define VIBRATOR_GPIO_Port GPIOB

#define SPI_MAX_DELAY 	100
#define I2C_MAX_DELAY 	100
#define UART_MAX_DELAY	100

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#endif /* INC_GLOBAL_H_ */
