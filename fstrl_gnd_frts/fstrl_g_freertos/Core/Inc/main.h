/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f1xx_ll_crc.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_rtc.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dwt_stm32_delay.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define LED1_ON()     LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
#define LED1_OFF()    LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
#define LED1_TOGGLE() LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
#define LED2_ON()     LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
#define LED2_OFF()    LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
#define LED2_TOGGLE() LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

#define USB_PIN_DISCONNECT() LL_GPIO_SetOutputPin(USB_DISCONNECT_GPIO_Port, USB_DISCONNECT_Pin);
#define USB_PIN_CONNECT()    LL_GPIO_ResetOutputPin(USB_DISCONNECT_GPIO_Port, USB_DISCONNECT_Pin);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define E32_AUX_Pin LL_GPIO_PIN_3
#define E32_AUX_GPIO_Port GPIOC
#define E32_M0_Pin LL_GPIO_PIN_0
#define E32_M0_GPIO_Port GPIOA
#define E32_M1_Pin LL_GPIO_PIN_1
#define E32_M1_GPIO_Port GPIOA
#define USB_DISCONNECT_Pin LL_GPIO_PIN_4
#define USB_DISCONNECT_GPIO_Port GPIOA
#define FLASH_SPI1_SCK_Pin LL_GPIO_PIN_5
#define FLASH_SPI1_SCK_GPIO_Port GPIOA
#define FLASH_SPI1_MISO_Pin LL_GPIO_PIN_6
#define FLASH_SPI1_MISO_GPIO_Port GPIOA
#define FLASH_SPI1_MOSI_Pin LL_GPIO_PIN_7
#define FLASH_SPI1_MOSI_GPIO_Port GPIOA
#define FLASH_CSN_Pin LL_GPIO_PIN_4
#define FLASH_CSN_GPIO_Port GPIOC
#define NRF24_PWR_EN_Pin LL_GPIO_PIN_5
#define NRF24_PWR_EN_GPIO_Port GPIOC
#define E32_PWR_EN_Pin LL_GPIO_PIN_0
#define E32_PWR_EN_GPIO_Port GPIOB
#define LED1_Pin LL_GPIO_PIN_1
#define LED1_GPIO_Port GPIOB
#define LED2_Pin LL_GPIO_PIN_2
#define LED2_GPIO_Port GPIOB
#define NRF24_IRQ_Pin LL_GPIO_PIN_12
#define NRF24_IRQ_GPIO_Port GPIOB
#define NRF24_SPI2_SCK_Pin LL_GPIO_PIN_13
#define NRF24_SPI2_SCK_GPIO_Port GPIOB
#define NRF24_SPI2_MISO_Pin LL_GPIO_PIN_14
#define NRF24_SPI2_MISO_GPIO_Port GPIOB
#define NRF24_SPI2_MOSI_Pin LL_GPIO_PIN_15
#define NRF24_SPI2_MOSI_GPIO_Port GPIOB
#define NRF24_CE_Pin LL_GPIO_PIN_6
#define NRF24_CE_GPIO_Port GPIOC
#define NRF24_CSN_Pin LL_GPIO_PIN_7
#define NRF24_CSN_GPIO_Port GPIOC
#define SDIO_SW_IN_Pin LL_GPIO_PIN_8
#define SDIO_SW_IN_GPIO_Port GPIOA
#define LCD_RW_Pin LL_GPIO_PIN_15
#define LCD_RW_GPIO_Port GPIOA
#define LCD_D4_Pin LL_GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin LL_GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin LL_GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin LL_GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOB
#define LCD_E_Pin LL_GPIO_PIN_8
#define LCD_E_GPIO_Port GPIOB
#define LCD_RS_Pin LL_GPIO_PIN_9
#define LCD_RS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


#define  RTOS_ENABLED   1

#define            ON   1
#define            OFF  0


#define  BIT7           (1 << 7)
#define  BIT6           (1 << 6)
#define  BIT5           (1 << 5)
#define  BIT4           (1 << 4)
#define  BIT3           (1 << 3)
#define  BIT2           (1 << 2)
#define  BIT1           (1 << 1)
#define  BIT0           (1 << 0)


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
