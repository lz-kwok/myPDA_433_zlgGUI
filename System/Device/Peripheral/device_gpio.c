/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : device_gpio.c
*����       : IO�豸
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/6        ��ʼ�汾
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
