/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_systick.c
*作用       : 系统节拍
*原理       : 
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/7        初始版本
********************************************************************************
*/
#include "system.h"





static function Systick10000RegisterPointerBlock[Systick10000Sum] =
{
    Dummy, Dummy
};

static function Systick1000RegisterPointerBlock[Systick1000Sum] =
{
    Dummy, Dummy, Dummy, Dummy
};

static function Systick100RegisterPointerBlock[Systick100Sum] =
{
    Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy
};

/*******************************************************************************
* 描述	    : 系统节拍注册函数，用于注册回调函数到不同速度的系统节拍数组中。
* 输入参数  : systemTick 系统节拍速度类型 registerFunction 注册的函数指针
* 返回参数  : bool布尔变量，返回是否成功
*******************************************************************************/
static bool RegisterSystick(SystickEnum type, function functionPointer)
{
    static byte Systick100Counter = 0;
    static byte Systick1000Counter = 0;
    static byte Systick10000Counter = 0;

    switch(type)
    {
        case Systick100:
            if (Systick100Counter == Systick100Sum) return(false);
            Systick100RegisterPointerBlock[Systick100Counter++] = functionPointer;  
            return(true);
        case Systick1000:
            if (Systick1000Counter == Systick1000Sum) return(false);
            Systick1000RegisterPointerBlock[Systick1000Counter++] = functionPointer;   
            return(true);
        case Systick10000:
            if (Systick10000Counter == Systick10000Sum) return(false);
            Systick10000RegisterPointerBlock[Systick10000Counter++] = functionPointer;   
            return(true);
        default:
            return(false);
    }
}



void InitSystick(void)
{
    System.Device.Systick.Register = RegisterSystick;   // 关联接口函数指针  
    
    /* Set Interrupt Group Priority */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
    HAL_InitTick(TICK_INT_PRIORITY);     
}


static void sysReset(void)
{
    static uint32_t ConfirmTick = 0;
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_0) == GPIO_PIN_SET) 
      ConfirmTick ++;
    else
      ConfirmTick = 0;
    
    if(ConfirmTick > 500)
      HAL_NVIC_SystemReset();
}



/*******************************************************************************
* 描述	    : 系统节拍中断入口，非常重要，每秒10000次，即0.1mS一次
*           : 系统节拍通过switch细分为每秒1000次，每秒100次的例行节拍
*           : 同层直接调用，高层采用注册调用
*******************************************************************************/
void SysTick_Handler(void)
{
    byte div;
    static byte Counter = 0;
    if(++Counter == 100) 
    {
      Counter = 0;
    }
    Systick10000RegisterPointerBlock[0]();
    Systick10000RegisterPointerBlock[1]();
    div = Counter / 10;

    switch(Counter % 10)
    {
        case 0: Systick1000RegisterPointerBlock[0](); break;
        case 1: Systick1000RegisterPointerBlock[1](); break;
        case 2: Systick1000RegisterPointerBlock[2](); break;
        case 3: Systick1000RegisterPointerBlock[3](); break;
        case 4: Systick100RegisterPointerBlock[div]();break;
        case 5: AppDataPointer->Systick1000++;        break;
        case 6: HAL_IncTick();                        break;
        case 7: TimerSystick1000Routine();            break;
//        case 8: KeyScan1000Routine();                 break;
        case 9:
            switch(div)
            {
                case 0: AdcSystick100Routine();       break;
                case 1: KeyScan1000Routine();         break;
                case 2: Uart1000Routine();            break;
                case 3: sysReset();                   break;
                default:                              
                  break;
            }
            break;
    }
}





