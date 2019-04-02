/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名   : os.h
*作用     : RTOS头文件
*创建时间 : 2013.7.21
********************************************************************************
*/

#ifndef __OS_H
#define __OS_H

extern uint16_t CriticalNesting;

#define  OS_ENTER_CRITICAL()  {CriticalNesting = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(CriticalNesting);}


  
extern void InitOs(void);

    




#endif






















