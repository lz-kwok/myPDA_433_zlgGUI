/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*                              主芯片:STM32F411re/cc
*                           江苏南大五维电子科技有限公司
*
*                               移植/裁剪：郭良志
********************************************************************************
*文件名     : device_menu.h
*作用       : UI业务逻辑
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2017/12/14        初始版本
********************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_MENU__H
#define __DEVICE_MENU__H

#include "config.h"
/* Includes ------------------------------------------------------------------*/

#define CodeValue      131

#define Homepage       0
#define Menupage       1
#define Recvpage       2
#define Parapage       3
#define Langpage       4
#define Systpage       5
#define Exitpage       6
#define Poffpage       7

extern WINDOWS winxp;


extern void HomePageShow();
extern void SetupPageShow(int8_t ctr);
extern void SystemSetPageShow(int8_t ctr);
extern void ParameterSetShow(int8_t ctr);
extern void ParameterSetupMenu(uint8_t ctr);
extern void SelectReceiverMenuShow();
extern void SelectFrebandMenuShow();
extern void CipherCheckFunction(uint8_t KeyIndex);
extern void ParaSetFunction(uint8_t ctr,uint8_t KeyIndex);
extern void PoweroffMenu(void);
extern void SystemSetupMenu(int8_t ctr);
extern void SysSetFunction(uint8_t ctr,uint8_t KeyIndex);
extern void LanguageSetMenu();
extern void ParameterInqury(void);
extern void DelayTurnoff();


#endif /* __DEVICE_MENU__H */
