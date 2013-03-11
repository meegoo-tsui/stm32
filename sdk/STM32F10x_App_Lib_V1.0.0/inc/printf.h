/**
  ******************************************************************************
  * @file    printf.h 
  * @author  Xu Mingfeng
  * @version V1.0.0
  * @date    2013-03-07
  * @brief   This file contains the headers of the printf.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_SDK_PRINTF_H
#define __STM32F10x_SDK_PRINTF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef PRINTF_VIA_INT
	#define PRINTF_VIA_INT                       1
#endif
#if (PRINTF_VIA_INT != 0)
	#define GETCHAR_BUF_SIZE                     256
	#define PUTCHAR_BUF_SIZE                     256
#endif

/*
 * 设置字体色
 * 30: 黑
 * 31: 红
 * 32: 绿
 * 33: 黄
 * 34: 蓝
 * 35: 紫
 * 36: 深绿
 * 37: 白色
*/
#define COLOR_RESET                              "\033[1;m"
#define COLOR_RED                                "\033[1;31m"
#define COLOR_GREEN                              "\033[1;32m"
#define COLOR_YELLOW                             "\033[1;33m"
#define COLOR_BLUE                               "\033[1;34m"
#define COLOR_PURPLE                             "\033[1;35m"
#define COLOR_CYAN                               "\033[1;36m"
#define COLOR_WHITE                              "\033[1;37m"

/* Exported macro ------------------------------------------------------------*/
#define STM32_TRACE(level, fmt, args...) \
	printf##level(COLOR_GREEN "%d[%s:%d] %s():" COLOR_RESET fmt, level, __FILE__, __LINE__, __func__, ##args)

/*!
 * @def printf_0
 * 打印级别0控制，在app中配置宏PRINTF_LEVEL。
 * @def printf_1
 * 打印级别1控制，在app中配置宏PRINTF_LEVEL。
 * @def printf_2
 * 打印级别2控制，在app中配置宏PRINTF_LEVEL。
 * @def printf_3
 * 打印级别3控制，在app.h中配置宏PRINTF_LEVEL。
 */
#ifdef PRINTF_LEVEL
	#if   (PRINTF_LEVEL == 0)
		#define printf0(fmt, args...)
		#define printf1(fmt, args...)
	#elif (PRINTF_LEVEL == 1)
		#define printf0 printf
		#define printf1(fmt, args...)
	#elif (PRINTF_LEVEL >= 2)
		#define printf0 printf
		#define printf1 printf
	#else
		#define printf0(fmt, args...)
		#define printf1(fmt, args...)
	#endif
#else
	#define printf0(fmt, args...)
	#define printf1(fmt, args...)
#endif

/*
 * 光标移动控制
 */
#define CURSOR_MV_UP(level, n)                   printf##level("\033[%dA", n)
#define CURSOR_MV_DOWN(level, n)                 printf##level("\033[%dB", n)
#define CURSOR_MV_R(level, n)                    printf##level("\033[%dC", n)
#define CURSOR_MV_F(level, n)                    printf##level("\033[%dD", n)
#define CURSOR_CLR_LINE(level)                   printf##level("\r\033[K")
#define CURSOR_CLR_ALL(level)                    printf##level("\033[2J")

#if (PRINTF_VIA_INT != 0)
	#define is_putchar_status()                  (in_putchar != out_putchar)
	#define is_getchar_status()                  (in_getchar != out_getchar)
#else
	#define is_putchar_status() 				 (1 == 1)
	#define is_getchar_status() 				 (1 == 1)
#endif

/* Exported variables --------------------------------------------------------*/
#if (PRINTF_VIA_INT != 0)
	extern uint8_t  getchar_buf[GETCHAR_BUF_SIZE];
	extern volatile uint16_t in_getchar, out_getchar;
	extern volatile uint32_t error_getchar;
	extern uint8_t  putchar_buf[PUTCHAR_BUF_SIZE];
	extern volatile uint16_t in_putchar, out_putchar;
	extern volatile uint32_t error_putchar;
#endif

/* Exported functions ------------------------------------------------------- */
extern void printf_init(void);
#if (PRINTF_VIA_INT != 0)
	extern void USART1_IRQHandler(void);
	extern void printf_stu(void);
#else
	#define printf_stu()
#endif

#endif /* __STM32F10x_SDK_PRINTF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
