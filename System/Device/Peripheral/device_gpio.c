/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_gpio.c
*作用       : IO设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/6        初始版本
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "system.h"



void InitGpio(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    EnableCtrl();
    
    /* Configure the GPIO_CTRL pin */
    GPIO_InitStruct.Pin   = CtrlPin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    
    HAL_GPIO_Init(CtrlPORT, &GPIO_InitStruct);
   
    SETA_CLR();
    SETB_CLR();
    
    CtrPowerOff(); 
    OledPowerOn();
}








    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
