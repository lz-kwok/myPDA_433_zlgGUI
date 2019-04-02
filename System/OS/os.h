/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���   : os.h
*����     : RTOSͷ�ļ�
*����ʱ�� : 2013.7.21
********************************************************************************
*/

#ifndef __OS_H
#define __OS_H

extern uint16_t CriticalNesting;

#define  OS_ENTER_CRITICAL()  {CriticalNesting = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(CriticalNesting);}


  
extern void InitOs(void);

    




#endif






















