/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : device_systick.c
*����       : ϵͳ����
*ԭ��       : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/7        ��ʼ�汾
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
* ����	    : ϵͳ����ע�ắ��������ע��ص���������ͬ�ٶȵ�ϵͳ���������С�
* �������  : systemTick ϵͳ�����ٶ����� registerFunction ע��ĺ���ָ��
* ���ز���  : bool���������������Ƿ�ɹ�
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
    System.Device.Systick.Register = RegisterSystick;   // �����ӿں���ָ��  
    
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
* ����	    : ϵͳ�����ж���ڣ��ǳ���Ҫ��ÿ��10000�Σ���0.1mSһ��
*           : ϵͳ����ͨ��switchϸ��Ϊÿ��1000�Σ�ÿ��100�ε����н���
*           : ͬ��ֱ�ӵ��ã��߲����ע�����
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





