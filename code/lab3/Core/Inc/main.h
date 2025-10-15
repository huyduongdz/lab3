/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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


//new var

void setTimer0(int duration);
void timer_run();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SA1_Pin GPIO_PIN_1
#define SA1_GPIO_Port GPIOA
#define SA2_Pin GPIO_PIN_2
#define SA2_GPIO_Port GPIOA
#define SA3_Pin GPIO_PIN_3
#define SA3_GPIO_Port GPIOA
#define SA4_Pin GPIO_PIN_4
#define SA4_GPIO_Port GPIOA
#define SA5_Pin GPIO_PIN_5
#define SA5_GPIO_Port GPIOA
#define SA6_Pin GPIO_PIN_6
#define SA6_GPIO_Port GPIOA
#define SA7_Pin GPIO_PIN_7
#define SA7_GPIO_Port GPIOA
#define BUTTON_1_Pin GPIO_PIN_0
#define BUTTON_1_GPIO_Port GPIOB
#define BUTTON_2_Pin GPIO_PIN_1
#define BUTTON_2_GPIO_Port GPIOB
#define BUTTON_3_Pin GPIO_PIN_2
#define BUTTON_3_GPIO_Port GPIOB
#define R1_Pin GPIO_PIN_10
#define R1_GPIO_Port GPIOB
#define G1_Pin GPIO_PIN_11
#define G1_GPIO_Port GPIOB
#define Y1_Pin GPIO_PIN_12
#define Y1_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_13
#define R2_GPIO_Port GPIOB
#define G2_Pin GPIO_PIN_14
#define G2_GPIO_Port GPIOB
#define Y2_Pin GPIO_PIN_15
#define Y2_GPIO_Port GPIOB
#define SB1_Pin GPIO_PIN_8
#define SB1_GPIO_Port GPIOA
#define SB2_Pin GPIO_PIN_9
#define SB2_GPIO_Port GPIOA
#define SB3_Pin GPIO_PIN_10
#define SB3_GPIO_Port GPIOA
#define SB4_Pin GPIO_PIN_11
#define SB4_GPIO_Port GPIOA
#define SB5_Pin GPIO_PIN_12
#define SB5_GPIO_Port GPIOA
#define SB6_Pin GPIO_PIN_13
#define SB6_GPIO_Port GPIOA
#define SB7_Pin GPIO_PIN_14
#define SB7_GPIO_Port GPIOA
#define BLINKY_Pin GPIO_PIN_15
#define BLINKY_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
