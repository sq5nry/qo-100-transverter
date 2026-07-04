/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l0xx_hal.h"

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

/* USER CODE BEGIN Private defines */

#define BANDSWITCH_1_Pin GPIO_PIN_4
#define BANDSWITCH_1_GPIO_Port GPIOA
#define BANDSWITCH_2_Pin GPIO_PIN_1
#define BANDSWITCH_2_GPIO_Port GPIOA
#define BANDSWITCH_3_Pin GPIO_PIN_0
#define BANDSWITCH_3_GPIO_Port GPIOA

#define RFFC_Port GPIOA
#define RFFC_ENX_Pin GPIO_PIN_10
#define RFFC_SCLK_Pin GPIO_PIN_9
#define RFFC_SDATA_Pin GPIO_PIN_7

#define LED_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_15

#define PTT_GPIO_Port GPIOC
#define PTT_Pin GPIO_PIN_14

#define PTT_RX GPIO_PIN_SET
#define PTT_TX GPIO_PIN_RESET

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
