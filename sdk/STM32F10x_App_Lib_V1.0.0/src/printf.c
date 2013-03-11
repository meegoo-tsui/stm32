/**
  ******************************************************************************
  * @file    printf.c 
  * @author  Xu Mingfeng
  * @version V1.0.0
  * @date    2013-03-07
  * @brief   Printf function.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef __GNUC__
	/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
	 set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
	#define GETCHAR_PROTOTYPE int __io_getchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (PRINTF_VIA_INT != 0)
	uint8_t  getchar_buf[GETCHAR_BUF_SIZE];
	volatile uint16_t in_getchar, out_getchar;
	volatile uint32_t error_getchar;
	uint8_t  putchar_buf[PUTCHAR_BUF_SIZE];
	volatile uint16_t in_putchar, out_putchar;
	volatile uint32_t error_putchar;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Init hardware for printf.
  * @param  None
  * @retval None
  */
void printf_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
#if (PRINTF_VIA_INT != 0)
	NVIC_InitTypeDef NVIC_InitStructure;
#endif

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

#if (PRINTF_VIA_INT != 0)
	/* init buffer cursor */
	in_getchar    = 0;
	out_getchar   = 0;
	error_getchar = 0;
	in_putchar    = 0;
	out_putchar   = 0;
	error_putchar = 0;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#endif	
}

/******************************************************************************/
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
#if (PRINTF_VIA_INT == 0)
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
	}
#else
	putchar_buf[in_putchar] = (uint8_t) ch;
	in_putchar = (in_putchar + 1)%PUTCHAR_BUF_SIZE;
	if(in_putchar == out_putchar){
		error_putchar++;
	}
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
#endif

	return ch;
}

/******************************************************************************/
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
#if (PRINTF_VIA_INT == 0)
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {
	}

	return ((int)USART_ReceiveData(USART1));
#else
	int rtn;

	if(is_getchar_status()){
		rtn = getchar_buf[out_getchar];
		out_getchar = (out_getchar + 1)%GETCHAR_BUF_SIZE;
		return rtn;
	} else{
		return -1;
	}
#endif
}

/******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number */
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);

	/* Infinite loop */
	while (1) {
	}
}
#endif

#if (PRINTF_VIA_INT != 0)
/******************************************************************************/
/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	/* interrupt - RxD */
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		getchar_buf[in_getchar] = USART_ReceiveData(USART1);
		in_getchar = (in_getchar + 1)%GETCHAR_BUF_SIZE;
		if(in_getchar == out_getchar){
			error_getchar++;
		}
	}

  	/* interrupt - TxD */
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){
		if(is_putchar_status()){
			USART_SendData(USART1, putchar_buf[out_putchar]);
			out_putchar = (out_putchar + 1)%PUTCHAR_BUF_SIZE;
		} else{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
}

/******************************************************************************/
/**
  * @brief  Printf the module status.
  * @param  None
  * @retval None
  */
void printf_stu(void)
{
	if(error_getchar != 0){
		printf(COLOR_RED"error_getchar = %d\r\n"COLOR_RESET, error_getchar);
	}

	if(error_putchar != 0){
		printf(COLOR_RED"error_putchar = %d\r\n"COLOR_RESET, error_putchar);
	}
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
