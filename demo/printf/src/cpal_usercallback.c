/**
  ******************************************************************************
  * @file    cpal_usercallback.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-June-2011
  * @brief   This file provides all the CPAL UserCallback functions .
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cpal_i2c.h"
#include "stm32_i2c_ee_cpal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  User callback that manages the Timeout error.
  * @param  pDevInitStruct .
  * @retval None.
  */
uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
	/* Deinitialize peripheral */
	sEE_DeInit(sEE_DevStructures[pDevInitStruct->CPAL_Dev]) ;
	
	/* Initialize peripherals for communication with sEE */
	sEE_StructInit(sEE_DevStructures[pDevInitStruct->CPAL_Dev]);
	sEE_Init(sEE_DevStructures[pDevInitStruct->CPAL_Dev]); 
	  
	sEE_DevStructures[pDevInitStruct->CPAL_Dev]->sEEState = sEE_STATE_ERROR;

	return CPAL_PASS;
}

/******************************************************************************/
/**
  * @brief  Manages the End of Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
	sEE_WriteHandler(pDevInitStruct->CPAL_Dev);
}


/******************************************************************************/
/**
  * @brief  Manages the End of Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */ 
void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
	sEE_ReadHandler(pDevInitStruct->CPAL_Dev);
}

/******************************************************************************/
/**
  * @brief  Manages Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_TX_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/

/******************************************************************************/
/**
  * @brief  Manages Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_RX_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/

/******************************************************************************/
/**
  * @brief  Manages the End of DMA Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMATXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/

/******************************************************************************/
/**
  * @brief  Manages the Half of DMA Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMATXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/

/******************************************************************************/
/**
  * @brief  Manages Error of DMA Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMATXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/

/******************************************************************************/
/**
  * @brief  Manages the End of DMA Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMARXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/

/******************************************************************************/
/**
  * @brief  Manages the Half of DMA Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_DMARXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/

/******************************************************************************/
/**
  * @brief  Manages Error of DMA Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_DMARXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/

/******************************************************************************/
/**
  * @brief  User callback that manages the I2C device errors.
  * @note   Make sure that the define USE_SINGLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInitStruct. 
  * @param  DeviceError.
  * @retval None
  */ 
void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError)
{
  /* Stop timeout countdown */ 
  sEE_DevStructures[pDevInstance]->sEE_CPALStructure->wCPAL_Timeout  = CPAL_I2C_TIMEOUT_DEFAULT;  
  
  /* Deinitialize peripheral */
  sEE_DeInit(sEE_DevStructures[pDevInstance]) ;
  
  /* Initialize peripherals for communication with sEE */
  sEE_StructInit(sEE_DevStructures[pDevInstance]);
  sEE_Init(sEE_DevStructures[pDevInstance]); 
    
  sEE_DevStructures[pDevInstance]->sEEState = sEE_STATE_ERROR; 
}

/******************************************************************************/
/**
  * @brief  User callback that manages BERR I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */  
/*void CPAL_I2C_BERR_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/

/******************************************************************************/
/**
  * @brief  User callback that manages ARLO I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */
/*void CPAL_I2C_ARLO_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/

/******************************************************************************/
/**
  * @brief  User callback that manages OVR I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */
/*void CPAL_I2C_OVR_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/

/******************************************************************************/
/**
  * @brief  User callback that manages AF I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */ 
/*void CPAL_I2C_AF_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/

/******************************************************************************/
/**
  * @brief  User callback that manage General Call Addressing mode.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_GENCALL_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/


/******************************************************************************/
/**
  * @brief  User callback that manage Dual Address Addressing mode.
  * @param  pDevInitStruct 
  * @retval None
  */  
/*void CPAL_I2C_DUALF_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/
 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
