/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F411re/cc
*                           �����ϴ���ά���ӿƼ����޹�˾
*
*                               ��ֲ/�ü�������־
********************************************************************************
*�ļ���     : device_sysclock.c
*����       : ϵͳ����
*ԭ��       : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/20        ��ʼ�汾
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
