/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*                              主芯片:STM32F411re/cc
*                           江苏南大五维电子科技有限公司
*
*                               移植/裁剪：郭良志
********************************************************************************
*文件名     : device_gpio.h
*作用       : IO设备头文件
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/6        初始版本
********************************************************************************
*/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_GPIO_H
#define __DEVICE_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "system.h"
   
   
#define  EnableCtrl()       __GPIOC_CLK_ENABLE()
#define  CtrlPORT           GPIOC  
#define  CtrlPin            GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_13    

#define  OledPowerOn()      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET)
#define  OledPowerOff()     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET)
#define  DTUPowerOn()       HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET)
#define  DTUPowerOff()      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET)
#define  CtrPowerOn()       HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)
#define  CtrPowerOff()      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)

   
#define  LCD_RST_CLR()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET)
#define  LCD_RST_SET()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET)

#define  LCD_DC_CLR()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET)
#define  LCD_DC_SET()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET)
   
#define  SETA_CLR()        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)
#define  SETA_SET()        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)
#define  SETB_CLR()        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET)
#define  SETB_SET()        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET)

   
   
void InitGpio(void);


  
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

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_GPIO_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
