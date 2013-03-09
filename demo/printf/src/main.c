/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "stm32f10x.h"
#include "printf.h"
#include "hw_config.h"
#include "stm32f10x_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARRAYSIZE                                800

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t source[ARRAYSIZE];
uint32_t destination[ARRAYSIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  dma men to men test
  * @param  None
  * @retval None
  */
void dma_mem_to_mem_test(void)
{
	uint32_t i;

	/* initialize array */
	for(i=0; i<ARRAYSIZE; i++){
		source[i]      = i;
		destination[i] = 0;
	}

	dma_config(source, destination, ARRAYSIZE);                   /* Init DMA */
	DMA_Cmd(DMA1_Channel1, ENABLE);     
	while(dma1_channel1_status == 0){                        /* wait dma done */
	}
	for(i=0; i<ARRAYSIZE; i++){
		if(i%8 == 0){
			printf("\r\n");
		}
		printf("%08x ", destination[i]);
	}
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  printf test
  * @param  None
  * @retval None
  */
void printf_test(void)
{
	/* Print project info */
	printf(COLOR_RED);
	printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
	printf(COLOR_RESET);
	STM32_TRACE(0, "STM32 TRACE 0\r\n");	
	STM32_TRACE(1, "STM32 TRACE 1\r\n");
	SysTick_delay_nMS(3000);
	CURSOR_MV_UP(0, 1);
	CURSOR_CLR_LINE(0);
	CURSOR_MV_UP(0, 1);
	CURSOR_CLR_LINE(0);
}

/******************************************************************************/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	
	hardware_config();                                       /* Init hardware */

	dma_mem_to_mem_test();
	printf_test();

	printf("Please input:\r\n");
	while (1){
		fputc(fgetc(stdin), stdout);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
