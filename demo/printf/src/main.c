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
#define EEPROM_PAGE_SIZE                         8
#define sEE_DevStructure                         sEE1_DevStructure 

#define BUFFER1_SIZE                             (countof(Tx1Buffer)-1)
#define BUFFER2_SIZE                             (countof(Tx2Buffer)-1)

#define EEPROM_READ_ADDR1                        ((uint16_t)0x0010)
#define EEPROM_WRITE_ADDR1                       ((uint16_t)0x0010)
#define EEPROM_READ_ADDR2                        ((uint16_t)0x00FF)
#define EEPROM_WRITE_ADDR2                       ((uint16_t)0x00FF)

/* Private macro -------------------------------------------------------------*/
#define countof(a)                               (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint32_t source[ARRAYSIZE];
uint32_t destination[ARRAYSIZE];

const uint8_t Tx1Buffer[] = "ABCDEFG";
const uint8_t Tx2Buffer[] = "0123456";

uint8_t Rx1Buffer[BUFFER1_SIZE] = {0};
uint8_t Rx2Buffer[BUFFER2_SIZE] = {0};

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
	/* Print test info */
	printf(COLOR_RED "\r\nIIC CPAL Example: read i2c eeprom via cpal lib" COLOR_RESET);

	/*------------- Initialize sEE_DevStructure -------------*/
	sEE_DevStructure.sEEAddress = EEPROM_ADDRESS;
	sEE_DevStructure.sEEPageSize = EEPROM_PAGE_SIZE;
	sEE_DevStructure.sEEMemoryAddrMode = sEE_OPT_16BIT_REG;

	/*-------------- Initialize sEE peripheral -------------*/
	sEE_StructInit(&sEE_DevStructure);
	sEE_Init(&sEE_DevStructure);

	/* Write Data in EEPROM */
	SysTick_delay_nMS(100);
	sEE_WriteBuffer(&sEE_DevStructure, (uint8_t*)Tx1Buffer, EEPROM_WRITE_ADDR1, BUFFER1_SIZE);

	/* Wail until communication is complete */
	printf(COLOR_GREEN "\r\nWait write done ..." COLOR_RESET);
	while((sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE) && 
	  	(sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_ERROR)){ 
	}

	/* Read Data from EEPROM */
	SysTick_delay_nMS(100);
	sEE_ReadBuffer(&sEE_DevStructure, Rx1Buffer, EEPROM_READ_ADDR1, BUFFER1_SIZE);

	/* Wail until communication is complete */
	printf(COLOR_GREEN "\r\nWait read done ..." COLOR_RESET);
	while((sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE) && 
	  (sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_ERROR)){
	}

	printf(COLOR_GREEN "\r\nWr:<%s>\r\n" COLOR_RESET, Tx1Buffer);
	printf(COLOR_GREEN "Rd:<%s>\r\n" COLOR_RESET, Rx1Buffer);
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
	printf_init();                                             /* Init printf */

	//dma_mem_to_mem_test();
	i2c_ee_cpal_test();
	printf_test();
	
	printf(COLOR_GREEN "\r\nall done.\r\n" COLOR_RESET);
	while(1){
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
