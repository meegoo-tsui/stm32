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
	uint8_t  putchar_buf[PUTCHAR_BUF_SIZE];
	volatile uint16_t in_putchar, out_putchar;
	volatile uint16_t error_putchar;
	volatile FunctionalState putchar_enable;
	volatile uint32_t putchar_cnt;
	uint8_t  getchar_buf[GETCHAR_BUF_SIZE];
	volatile uint16_t in_getchar, out_getchar;
	volatile uint16_t error_getchar;
	volatile FunctionalState getchar_enable;
	volatile uint32_t getchar_cnt;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if (PRINTF_VIA_INT != 0)
/**
  * @brief  Init printf vars.
  * @param  None
  * @retval None
  */
static void init_printf_vars(uint32_t clr)
{
	in_putchar     = 0;
	out_putchar    = 0;
	in_getchar     = 0;
	out_getchar    = 0;
	if(clr == 0){
		putchar_cnt   = 0;
		error_putchar = 0;
		error_getchar = 0;
		getchar_cnt   = 0;
	}
}

/******************************************************************************/
/**
  * @brief  enable putchar.
  * @param  None
  * @retval None
  */
static void enable_putchar(uint32_t en)
{
	if(en == 0){
		putchar_enable = DISABLE;
		USART_ITConfig(USART1, USART_IT_TXE, putchar_enable);	
	}else{
		if(putchar_enable == DISABLE){
			putchar_enable = ENABLE;
			USART_ITConfig(USART1, USART_IT_TXE, putchar_enable);
			if(is_putchar_status()){
				USART_SendData(USART1, putchar_buf[out_putchar]);
				out_putchar = (out_putchar + 1)%PUTCHAR_BUF_SIZE;
			}
		}	
	}

}

/******************************************************************************/
/**
  * @brief  enable getchar.
  * @param  None
  * @retval None
  */
static void enable_getchar(uint32_t en)
{
	if(en == 0){
		getchar_enable = DISABLE;
		USART_ITConfig(USART1, USART_IT_RXNE, getchar_enable);		
	}else{
		if(getchar_enable == DISABLE){
			getchar_enable = ENABLE;
			USART_ITConfig(USART1, USART_IT_RXNE, getchar_enable);
		}
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
}
#endif

/******************************************************************************/
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
	/* init vars */
	init_printf_vars(0);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	enable_putchar(0);
	enable_getchar(0);
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
	if(is_putchar_busy()){
		error_putchar++;
		printf_stu();
		return ch;
	}
	putchar_cnt++;
	putchar_buf[in_putchar] = (uint8_t) ch;
	in_putchar = (in_putchar + 1)%PUTCHAR_BUF_SIZE;
	enable_putchar(1);
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

	enable_getchar(1);
	if(is_getchar_status()){
		getchar_cnt++;
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
		if(is_getchar_busy()){
			USART_ReceiveData(USART1);
			error_getchar++;
		}else{
			getchar_buf[in_getchar] = USART_ReceiveData(USART1);
			in_getchar = (in_getchar + 1)%GETCHAR_BUF_SIZE;		
		}
	}

  	/* interrupt - TxD */
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){
		if(is_putchar_status()){
			USART_SendData(USART1, putchar_buf[out_putchar]);
			out_putchar = (out_putchar + 1)%PUTCHAR_BUF_SIZE;
		} else{
			enable_putchar(0);
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
	uint16_t in_putchar_bak, out_putchar_bak,
	         in_getchar_bak, out_getchar_bak;
	         

	if(error_getchar != 0 || error_putchar != 0){
		in_putchar_bak  = in_putchar;
		out_putchar_bak = out_putchar;
		in_getchar_bak  = in_getchar;
		out_getchar_bak = out_getchar;


		init_printf_vars(1);                                     /* init vars */

		printf(COLOR_RED"\r\n");
		printf("putchar - %08x: [I.%04x, O.%04x]\r\n", putchar_cnt, in_putchar_bak, out_putchar_bak);
		printf("error_putchar = %d\r\n", error_putchar);
		printf("getchar - %08x: [I.%04x, O.%04x]\r\n", getchar_cnt, in_getchar_bak, out_getchar_bak);
		printf("error_getchar = %d\r\n", error_getchar);
		printf(COLOR_RESET"\r\n");
		while(1); /* loop here */
	}
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
