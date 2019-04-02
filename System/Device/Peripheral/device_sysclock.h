/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F411re/cc
*                           江苏南大五维电子科技有限公司
*
*                               移植/裁剪：郭良志
********************************************************************************
*文件名     : device_sysclock.c
*作用       : 系统节拍
*原理       : 
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/20        初始版本
********************************************************************************
*/
#ifndef __DEVICE_SYSCLOCK_H
#define __DEVICE_SYSCLOCK_H

#define HSE               (0x01)
#define HSI               (0x02)
#define HSE_OK            (0x03)
#define HSE_error         (0x04)
#define HSI_OK            (0x05)
#define HSI_error         (0x06)
#define Parameter_error   (0x07)

#define HSE_CRYSTAL

void InitHSEClock(uint8_t MainFreq);
void InitHSIClock(uint8_t MainFreq);


#endif
