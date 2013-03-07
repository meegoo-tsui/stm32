/**
  ******************************************************************************
  * @file    hw_config.h 
  * @author  Xu Mingfeng
  * @version V1.0.0
  * @date    2013-03-07
  * @brief   config hard ware.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_HW_CONFIG_H
#define __APP_HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern USART_InitTypeDef USART_InitStructure;
extern GPIO_InitTypeDef  GPIO_InitStructure;

/* Exported functions ------------------------------------------------------- */
extern void hardware_config(void);

#endif /* __APP_HW_CONFIG_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
