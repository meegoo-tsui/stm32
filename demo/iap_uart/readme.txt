/**
  @page IAP AN2557 IAP Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    IAP/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   Description of the AN2557 "STM32F10xxx in-application programming
  *          using the USART (IAP)".
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files and pre-configured projects that 
describes how to build an application to be loaded into Flash memory using
In-Application Programming (IAP, through USART).


@par Directory contents 

 - "IAP\inc": contains the IAP firmware header files 
    - IAP/inc/common.h            This file provides all the headers of the common functions.
    - IAP/inc/ymodem.h            This file provides all the software function headers of the ymodem.c file.    
    - IAP/inc/stm32f10x_conf.h    Library Configuration file
    - IAP/inc/stm32f10x_it.h      Header for stm32f10x_it.c    

 - "IAP\MDK-ARM": contains pre-configured project for MDK-ARM toolchain

 - "IAP\RIDE": contains pre-configured project for RIDE toolchain

 - "IAP\HiTOP": contains pre-configured project for HiTOP toolchain

 - "IAP\EWARM": contains pre-configured project for EWARM toolchain
 
 - "IAP\TrueSTUDIO": contains pre-configured project for TrueSTUDIO toolchain

 - "IAP\src": contains the IAP firmware source files
    - IAP/src/main.c              Main program
    - IAP/src/stm32f10x_it.c      Interrupt handlers
    - IAP/src/download.c          This file provides the firmware which allows to download an image to internal Flash.
    - IAP/src/upload.c            This file provides the firmware which allows to upload an image from internal Flash. 
    - IAP/src/common.c            This file provides all the common functions. 
    - IAP/src/ymodem.c            This file provides all the firmware functions related to the ymodem protocol.              
    - IAP/src/system_stm32f10x.c  STM32F10x system source file
  
 - "IAP\binary_template": contains the binary template firmware source files

@par Hardware and Software environment  

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density 
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density 
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity line), 
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density)   
    evaluation boards and can be easilytailored to any other supported device 
    and development board.    
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN10 (USART1) and PC serial port.
    - Use the Key push-button connected to pin PG.08 (EXTI Line8). 
    
  - STM32100B-EVAL Set-up  
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN10 (USART1) and PC serial port.
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    
  - STM3210C-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 (USART2) and PC serial port.
    @note Make sure that jumpers JP19 and JP18 are open.
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
        
  - STM3210E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN12 (USART1) and PC serial port.
    - Use the Key push-button connected to pin PG.08 (EXTI Line8). 
  
  - STM3210B-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 (USART1) and PC serial port.  
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
  
            
  - Hyperterminal configuration: 
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following:

1. Generate a binary image for the pogram provided in the "Project\IAP\binary_template"
   directory. 
2. Program the internal Flash with the IAP (see below) 
3. Open HyperTerminal window using the settings already defined in section
   "Hardware environment" 
4. To run the IAP driver, keep the Key push-button pressed at Reset. 
   The IAP main menu is then displayed on the HyperTerminal window.
5. To download an application, press 1 and use the Ymodem protocol
      
      
In order to load the IAP code, you have do the following:
 - EWARM:
    - Open the IAP.eww workspace
    - In the workspace toolbar select the project config:
        - STM32100E-EVAL: to configure the project for STM32 High-density Value line devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32100B-EVAL: to configure the project for STM32 Medium-Density Value line devices    
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - RIDE:
    - Open the IAP.rprj project
    - In the configuration toolbar(Project->properties) select the project config:
        - STM32100E-EVAL: to configure the project for STM32 High-density Value line devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32100B-EVAL: to configure the project for STM32 Medium-Density Value line devices    
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 - MDK-ARM:
    - Open the IAP.uvproj project
    - In the build toolbar select the project config:
        - STM32100E-EVAL: to configure the project for STM32 High-density Value line devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32100B-EVAL: to configure the project for STM32 Medium-Density Value line devices    
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)

 - HiTOP:
    - Open the HiTOP toolchain.
    - Change the ApplicationAddress definition to "0x8003000" in "common.h" file.
    - Browse to open the Project.htp
        - STM32100E-EVAL: to configure the project for STM32 High-density Value line devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32100B-EVAL: to configure the project for STM32 Medium-Density Value line devices    
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - A "Download application" window is displayed, click "cancel".
    - Rebuild all files: Project->Rebuild all
    - Load project image : Click "ok" in the "Download application" window.
    - Run program: Debug->Run 

 - TrueSTUDO:
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project:
        - STM32100E-EVAL: to configure the project for STM32 High-density Value line devices
        - STM3210E-EVAL_XL: to load the project for STM32 XL-density devices
        - STM32100B-EVAL: to load the project for STM32 Medium-Density Value line devices    
        - STM3210C-EVAL: to load the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to load the project for STM32 Medium-density devices
        - STM3210E-EVAL: to load the project for STM32 High-density devices
    - Under Windows->Preferences->General->Workspace->Linked Resources, add 
      a variable path named "CurPath" which points to the folder containing
      "Libraries", "Project" and "Utilities" folders.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
    
@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
 
 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 */
