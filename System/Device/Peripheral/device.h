/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              硬件平台:RockingDemo
*                              主芯片:STM32F411re/cc
*                           江苏南大五维电子可以有限公司
*
*                                OS原作者:王绍伟
*                               移植/裁剪：郭良志
********************************************************************************
*文件名     : device.h
*作用       : 定义设备层相关宏定义
*原理       : 
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz        2014/06/21       初始版本
********************************************************************************
*/


#ifndef __DEVICE_H
#define __DEVICE_H

typedef enum
{
    AdcChanne0             = 0,
    AdcChanne1             = 1,
    AdcChanne2             = 2,
    AdcChanne3             = 3        
}AdcChannelEnum;

typedef struct 
{
    uint16_t A0;
    uint16_t A1;
    uint16_t A2;
    uint16_t A3;
    uint16_t * pA0;
    uint16_t * pA1;
    uint16_t * pA2;
    uint16_t * pA3;
}AdcStruct;




void InitDevice(void);



#endif


