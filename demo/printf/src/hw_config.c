/**
  ******************************************************************************
  * @file    hw_config.c 
  * @author  Xu Mingfeng
  * @version V1.0.0
  * @date    2013-03-07
  * @brief   config hard ware.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "hw_config.h"
#include "stm32f10x_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
DMA_InitTypeDef   DMA_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure system hard ware.
  * @param  None
  * @retval None
  */
void hardware_config(void)
{
	/* Enable GPIOA and USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTx configured as follow:
			- BaudRate = 115200 baud  
			- Word Length = 8 Bits
			- One Stop Bit
			- No parity
			- Hardware flow control disabled (RTS and CTS signals)
			- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure the USART1 */
	USART_Init(USART1, &USART_InitStructure);
	/* Enable the USARTx */
	USART_Cmd(USART1, ENABLE);
	/* Clear flag for 1st byte */
	USART_GetFlagStatus(USART1, USART_FLAG_TC);

	/* Setup SysTick Timer for 1 sec interrupts */
	SysTick_1ms = 0;
	if(SysTick_Config(SystemCoreClock / 1000)){ 
		printf("\r\nCapture error.");
		while (1);
	}
}

/******************************************************************************/
/**
  * @brief  Configure dma Channel for memory to memory.
  * @param  None
  * @retval None
  */
void dma_config(uint32_t *source, uint32_t *destination, uint32_t size)
{
	/* enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* reset DMA1 channe1 to default values */
	DMA_DeInit(DMA1_Channel1);
	/* channel will be used for memory to memory transfer */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	/* setting normal mode (non circular) */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	/* medium priority */
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	/* source and destination data size word=32bit */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	/* automatic memory increment enable. Destination and source */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	/* Location assigned to peripheral register will be source */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/* chunk of data to be transfered */
	DMA_InitStructure.DMA_BufferSize = size;
	/* source and destination start addresses */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)source;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)destination;
	/* send values to DMA registers */
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA1 Channel Transfer Complete interrupt */
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	
	/* Enable DMA1 channel IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	dma1_channel1_status = 0;
}

/******************************************************************************/
/**
  * @brief  Delay n ms use system tick.
  * @param  None
  * @retval None
  */
void SysTick_delay_nMS(uint32_t n)
{
	uint32_t timeout;

	timeout = n + SysTick_1ms;
	while(SysTick_1ms != timeout){
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
