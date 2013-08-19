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
#include "stm32_i2c_ee_cpal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARRAYSIZE                                256

/* Define EEPROM address, size and page size */
#define EEPROM_ADDRESS                           0xA0
#define EEPROM_SIZE                              256
#define EEPROM_PAGE_SIZE                         8
#define sEE_DevStructure                         sEE1_DevStructure

/* Private macro -------------------------------------------------------------*/
#define countof(a)                               (sizeof(a) / sizeof(*(a)))

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

	/* Print test info */
	printf(COLOR_RED "\r\nDMA Example: men to men" COLOR_RESET);

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

/******************************************************************************/
/**
  * @brief  printf test
  * @param  None
  * @retval None
  */
void printf_test(void)
{
	int get_char;

	/* Print test info */
	printf(COLOR_RED);
	printf("\r\nUSART Printf Example: retarget the C library printf function to the USART\r\n");
	printf(COLOR_RESET);
	STM32_TRACE(0, "STM32 TRACE 0\r\n");	
	STM32_TRACE(1, "STM32 TRACE 1\r\n");
	SysTick_delay_nMS(3000);
	CURSOR_MV_UP(0, 1);
	CURSOR_CLR_LINE(0);
	CURSOR_MV_UP(0, 1);
	CURSOR_CLR_LINE(0);

	/* loop */
	printf("Please input:\r\n");
	while (1){
		printf_stu();
		get_char = fgetc(stdin);
		if(get_char == -1){
			continue;
		}
		printf("%02x - %c\r\n", get_char, get_char);
		if(get_char == 0x03 ){ /* Ctrl + C */
			break;
		}
	}
}

/******************************************************************************/
/**
  * @brief  i2c eeprom cpal test
  * @param  None
  * @retval None
  */
void i2c_ee_cpal_test(void)
{
	int i, j;
	uint8_t txd_buf[EEPROM_PAGE_SIZE] = {0};
	uint8_t rxd_buf[EEPROM_SIZE] = {0};

	/* Print test info */
	printf(COLOR_RED "\r\nIIC CPAL Example: read i2c eeprom via cpal lib\r\n" COLOR_RESET);
	SysTick_delay_nMS(100);

	/*------------- Initialize sEE_DevStructure -------------*/
	sEE_DevStructure.sEEAddress = EEPROM_ADDRESS;
	sEE_DevStructure.sEEPageSize = EEPROM_PAGE_SIZE;
	sEE_DevStructure.sEEMemoryAddrMode = sEE_OPT_16BIT_REG;

	/*-------------- Initialize sEE peripheral -------------*/
	sEE_StructInit(&sEE_DevStructure);
	sEE_Init(&sEE_DevStructure);
	SysTick_delay_nMS(100);

	/* Write chip data in EEPROM */
	for(i=0; i<2; i++){
		printf(COLOR_GREEN "\r\nPage: %02d\r\n" COLOR_RESET, (i + 1));
		for(j=0; j<EEPROM_PAGE_SIZE; j++){
			txd_buf[j] = j + i + 1;
			printf("%02x ", txd_buf[j]);
		}
		printf("\r\n");

		/* Write page Data in EEPROM */
		sEE_WriteBuffer(&sEE_DevStructure, txd_buf, (i<<3), EEPROM_PAGE_SIZE);
		/* Wail until communication is complete */
		while((sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE) && 
			(sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_ERROR)){ 
		}
		SysTick_delay_nMS(100);
	}

	/* Read chip data in EEPROM */
	sEE_ReadBuffer(&sEE_DevStructure, rxd_buf, 0, EEPROM_PAGE_SIZE);
	SysTick_delay_nMS(100);
	for(i=0; i<EEPROM_SIZE; i++){
		if(i%8 == 0){
			printf("\r\n");
		}
		printf("%02x ", rxd_buf[i]);
	}
	printf("\r\n");
}

/******************************************************************************/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);    /* re-locate vect tab */
	hardware_config();                                       /* Init hardware */
	printf_init();                                             /* Init printf */

	//dma_mem_to_mem_test();
	i2c_ee_cpal_test();
	printf_test();
	
	printf(COLOR_GREEN "\r\nall done.\r\n" COLOR_RESET);
	while(1){
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
