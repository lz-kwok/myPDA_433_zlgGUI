/**
  ******************************************************************************
  * @file    stm324xg_eval_ioe.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    05-March-2012
  * @brief   This file contains all the functions prototypes for the
  *          stm324xg_eval_ioe.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

  /* File Info : ---------------------------------------------------------------
    SUPPORTED FEATURES:
      - IO Read/write : Set/Reset and Read (Polling/Interrupt)
      - Joystick: config and Read (Polling/Interrupt)
      - Touch Screen Features: Single point mode (Polling/Interrupt)
      - TempSensor Feature: accuracy not determined (Polling).

    UNSUPPORTED FEATURES:
      - Row ADC Feature is not supported (not implemented on STM324xG_EVAL board)
  ----------------------------------------------------------------------------*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM324xG_EVAL_IOE_H
#define __STM324xG_EVAL_IOE_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include "stm324xg_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324xG_EVAL
  * @{
  */
    
/** @defgroup STM324xG_EVAL_IOE 
  * @{
  */    


/** @defgroup STM324xG_EVAL_IOE_Exported_Types
  * @{
  */ 

/** 
  * @brief  Touch Screen Information structure  
  */ 
typedef struct
{
  uint16_t TouchDetected;
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
}TS_STATE; 
  
/** 
  * @brief  Joystick State definitions  
  */ 
#ifndef __STM324xG_EVAL_H
typedef enum 
{ 
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef
;
#endif /* __STM324xG_EVAL_H */
 
/** 
  * @brief  IO_Expander Error codes  
  */ 
typedef enum
{
  IOE_OK = 0,
  IOE_FAILURE, 
  IOE_TIMEOUT,
  PARAM_ERROR,
  IOE1_NOT_OPERATIONAL, 
  IOE2_NOT_OPERATIONAL
}IOE_Status_TypDef;

/** 
  * @brief  IO bit values  
  */ 
typedef enum
{
  BitReset = 0,
  BitSet = 1
}IOE_BitValue_TypeDef;

/** 
  * @brief  IOE DMA Direction  
  */ 
typedef enum
{
  IOE_DMA_TX = 0,
  IOE_DMA_RX = 1
}IOE_DMADirection_TypeDef;


/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_IOE_Exported_Constants
  * @{
  */ 

/**
 * @brief Uncomment the line below to enable verfying each written byte in write
 *        operation. The I2C_WriteDeviceRegister() function will then compare the
 *        written and read data and return error status if a mismatch occurs.
 */
/* #define VERIFY_WRITTENDATA */

/**
 * @brief Uncomment the line below if you want to use user timeout callback.
 *        Function prototypes is declared in this file but function body may be
 *        implemented into user application.  
 */
/* #define USE_TIMEOUT_USER_CALLBACK */

/**
 * @brief Uncomment the line below if you want to use user defined Delay function
 *        (for precise timing), otherwise default _delay_ function defined within
 *         this driver is used (less precise timing).  
 */
/* #define USE_Delay */

#ifdef USE_Delay
#include "main.h"
 
  #define _delay_     Delay  /* !< User can provide more timing precise _delay_ function
                                   (with 10ms time base), using SysTick for example */
#else
  #define _delay_     delay      /* !< Default _delay_ function with less precise timing */
#endif    


/** 
  * @brief  Edge detection value  
  */ 
#define EDGE_FALLING              0x01
#define EDGE_RISING               0x02

/** 
  * @brief  Global interrupt Enable bit  
  */ 
#define IOE_GIT_EN                0x01

/**
  * @}
  */ 



/** @defgroup STM324xG_EVAL_IOE_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 



/** @defgroup STM324xG_EVAL_IOE_Exported_Functions
  * @{
  */ 

/** 
  * @brief  Configuration and initialization functions  
  */
uint8_t IOE_Config(void);
uint8_t IOE_ITConfig(uint32_t IOE_ITSRC_Source);

/** 
  * @brief  Timeout user callback function. This function is called when a timeout
  *         condition occurs during communication with IO Expander. Only protoype
  *         of this function is decalred in IO Expander driver. Its implementation
  *         may be done into user application. This function may typically stop
  *         current operations and reset the I2C peripheral and IO Expander.
  *         To enable this function use uncomment the define USE_TIMEOUT_USER_CALLBACK
  *         at the top of this file.          
  */
#ifdef USE_TIMEOUT_USER_CALLBACK 
 uint8_t IOE_TimeoutUserCallback(void);
#else
 #define IOE_TimeoutUserCallback()  IOE_TIMEOUT
#endif /* USE_TIMEOUT_USER_CALLBACK */

/** 
  * @brief IO pins control functions
  */
uint8_t IOE_WriteIOPin(uint8_t IO_Pin, IOE_BitValue_TypeDef BitVal);
uint8_t IOE_ReadIOPin(uint32_t IO_Pin);


/** 
  * @brief Touch Screen controller functions
  */
TS_STATE* IOE_TS_GetState(void);

/** 
  * @brief Interrupts Mangement functions
  */
FlagStatus IOE_GetGITStatus(uint8_t DeviceAddr, uint8_t Global_IT);
uint8_t IOE_ClearGITPending(uint8_t DeviceAddr, uint8_t IO_IT);
FlagStatus IOE_GetIOITStatus(uint8_t DeviceAddr, uint8_t IO_IT);
uint8_t IOE_ClearIOITPending(uint8_t DeviceAddr, uint8_t IO_IT);

/** 
  * @brief Temperature Sensor functions
  */
uint32_t IOE_TempSens_GetData(void);

/** 
  * @brief IO-Expander Control functions
  */
uint8_t IOE_IsOperational(uint8_t DeviceAddr);
uint8_t IOE_Reset(uint8_t DeviceAddr);
uint16_t IOE_ReadID(uint8_t DeviceAddr);

uint8_t IOE_FnctCmd(uint8_t DeviceAddr, uint8_t Fct, FunctionalState NewState);
uint8_t IOE_IOPinConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Direction);
uint8_t IOE_GITCmd(uint8_t DeviceAddr, FunctionalState NewState);
uint8_t IOE_GITConfig(uint8_t DeviceAddr, uint8_t Global_IT, FunctionalState NewState);
uint8_t IOE_IOITConfig(uint8_t DeviceAddr, uint8_t IO_IT, FunctionalState NewState);

/** 
  * @brief Low Layer functions
  */
uint8_t IOE_TS_Config(void);
uint8_t IOE_TempSens_Config(void);
uint8_t IOE_IOAFConfig(uint8_t DeviceAddr, uint8_t IO_Pin, FunctionalState NewState);
uint8_t IOE_IOEdgeConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Edge);
uint8_t IOE_ITOutConfig(uint8_t Polarity, uint8_t Type);

uint8_t I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue);
uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr);
uint16_t I2C_ReadDataBuffer(uint8_t DeviceAddr, uint32_t RegisterAddr);

#ifdef __cplusplus
}
#endif
#endif /* __STM324xG_EVAL_IOE_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */       
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
