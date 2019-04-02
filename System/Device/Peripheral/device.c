/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���       : device.c
*����         : �豸�ӿ�
*ԭ��         : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/09/13       ��ʼ�汾
********************************************************************************
*/

#include "system.h"

static uint32_t CpuID[3]; 

extern void InitUSBVcp(void);

IWDG_HandleTypeDef  hiwdg;


static void GetLockCode(void) 
{ 
  //��ȡCPUΨһID 
  CpuID[0]=*(uint32_t*)(0x1ffff7e8); 
  CpuID[1]=*(uint32_t*)(0x1ffff7ec); 
  CpuID[2]=*(uint32_t*)(0x1ffff7f0); 
  //�����㷨,�ܼ򵥵ļ����㷨 
  Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3); 
}

void CheckTurnPower(void)
{
    if(TurnoffIndex != 1)
    {
       switch(TurnoffIndex)
       {
         case 2:
           TurnoffValue = 5;
           break;
         case 3:
           TurnoffValue = 10;
           break;
         case 4:
           TurnoffValue = 20;
           break;
         case 5:
           TurnoffValue = 30;
           break;
         case 6:
           TurnoffValue = 60;
           break;
         case 7:
           TurnoffValue = 300;
           break;
         case 8:
           TurnoffValue = 600;
           break;
         case 9:
           TurnoffValue = 1200;
           break;
       }
       
       System.Device.Timer.Start(3,TimerSystick,1000,DelayTurnoff);
    }
}


/*******************************************************************************
* ������    : �������Ź���ʼ������
* ����	    : 
* �������  : ��
* ���ز���  : ��
* �������  : Tout = prv/40 * rlv (ms) prv������[4,8,16,32,64,128,256]
  *            prv:Ԥ��Ƶ��ֵ��ȡֵ���£�
  *            ���� IWDG_PRESCALER_4: IWDG prescaler set to 4
  *            ���� IWDG_PRESCALER_8: IWDG prescaler set to 8
  *            ���� IWDG_PRESCALER_16: IWDG prescaler set to 16
  *            ���� IWDG_PRESCALER_32: IWDG prescaler set to 32
  *            ���� IWDG_PRESCALER_64: IWDG prescaler set to 64
  *            ���� IWDG_PRESCALER_128: IWDG prescaler set to 128
  *            ���� IWDG_PRESCALER_256: IWDG prescaler set to 256
  *
  *            rlv:Ԥ��Ƶ��ֵ��ȡֵ��ΧΪ��0-0XFFF
*******************************************************************************/
void InitIwdg(uint8_t prv ,uint16_t rlv)
{
   hiwdg.Instance = IWDG;
   hiwdg.Init.Prescaler = prv;
   hiwdg.Init.Reload = rlv;

  if(HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }   
      
}

void IWDG_Start(void)           //�����������Ź�
{
   HAL_IWDG_Start(&hiwdg);
}

void IWDG_Refresh(void)           //�������Ź�ι������
{
   HAL_IWDG_Refresh(&hiwdg);
}




/*******************************************************************************
* ����	    : ��ʼ���豸
*******************************************************************************/
void InitDevice(void)
{  
    InitHSIClock(72);        //��ʼ����ʱ��    
    InitGpio();
    InitIwdg(IWDG_PRESCALER_256,2500);
    InitKey(BUTTON_KEY, BUTTON_MODE_GPIO);
    InitUsart1(9600);	    
//    InitUsart2(112500);
    InitSPI1(256);
    InitSystick();
    InitTimer();
    InitAdc();
    GUI_Initialize();
    GetLockCode();
    CheckTurnPower();
}


