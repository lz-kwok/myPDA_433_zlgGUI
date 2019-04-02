/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名       : device.c
*作用         : 设备接口
*原理         : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/09/13       初始版本
********************************************************************************
*/

#include "system.h"

static uint32_t CpuID[3]; 

extern void InitUSBVcp(void);

IWDG_HandleTypeDef  hiwdg;


static void GetLockCode(void) 
{ 
  //获取CPU唯一ID 
  CpuID[0]=*(uint32_t*)(0x1ffff7e8); 
  CpuID[1]=*(uint32_t*)(0x1ffff7ec); 
  CpuID[2]=*(uint32_t*)(0x1ffff7f0); 
  //加密算法,很简单的加密算法 
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
* 函数名    : 独立看门狗初始化函数
* 描述	    : 
* 输入参数  : 无
* 返回参数  : 无
* 输入参数  : Tout = prv/40 * rlv (ms) prv可以是[4,8,16,32,64,128,256]
  *            prv:预分频器值，取值如下：
  *            参数 IWDG_PRESCALER_4: IWDG prescaler set to 4
  *            参数 IWDG_PRESCALER_8: IWDG prescaler set to 8
  *            参数 IWDG_PRESCALER_16: IWDG prescaler set to 16
  *            参数 IWDG_PRESCALER_32: IWDG prescaler set to 32
  *            参数 IWDG_PRESCALER_64: IWDG prescaler set to 64
  *            参数 IWDG_PRESCALER_128: IWDG prescaler set to 128
  *            参数 IWDG_PRESCALER_256: IWDG prescaler set to 256
  *
  *            rlv:预分频器值，取值范围为：0-0XFFF
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

void IWDG_Start(void)           //开启独立看门狗
{
   HAL_IWDG_Start(&hiwdg);
}

void IWDG_Refresh(void)           //独立看门狗喂狗函数
{
   HAL_IWDG_Refresh(&hiwdg);
}




/*******************************************************************************
* 描述	    : 初始化设备
*******************************************************************************/
void InitDevice(void)
{  
    InitHSIClock(72);        //初始化主时钟    
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


