/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*                              ��оƬ:STM32F411re/cc
*                           �����ϴ���ά���ӿƼ����޹�˾
*
*                               ��ֲ/�ü�������־
********************************************************************************
*�ļ���     : device_menu.h
*����       : UIҵ���߼�
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2017/12/14        ��ʼ�汾
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
