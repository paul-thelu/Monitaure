/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INT_TMP_Pin GPIO_PIN_13
#define INT_TMP_GPIO_Port GPIOC
#define INT_OXI_Pin GPIO_PIN_0
#define INT_OXI_GPIO_Port GPIOA
#define INT_OXI_EXTI_IRQn EXTI0_IRQn
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
#define INT_IMU2_EXTI_IRQn EXTI9_5_IRQn
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
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
