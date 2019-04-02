/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_pwm.c
*作用       : PWM波输出
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/6        初始版本
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "device_pwm.h"


/********Private Variables*****************************************************/
uint8_t dir = 1;
uint16_t led0pwmval=0;


TIM_HandleTypeDef   TIM3_Handler;     //定时器3PWM句柄
TIM_OC_InitTypeDef  TIM3_CH4Handler;  //定时器3通道4句柄


void TIM_SetTIM3Compare4(uint32_t compare)
{
    TIM3->CCR4=compare;
}


/*******************************************************************************
* 函数名    : InitTim3PWM
* 描述	    : 初始化PWM操作
* 输入参数  : FreqkHz 输入参数为kHz
* 返回参数  : 无
*******************************************************************************/
void InitTim3PWM(uint16_t FreqkHz)
{
    uint16_t psc = HAL_RCC_GetHCLKFreq()/1000000 - 1;
    uint16_t arr = 1000/FreqkHz - 1;  
      
    TIM3_Handler.Instance            =  TIM3; //定时器3
    TIM3_Handler.Init.Prescaler      =  psc; //定时器分频
    TIM3_Handler.Init.CounterMode    =  TIM_COUNTERMODE_UP;   //向上计数模式
    TIM3_Handler.Init.Period         =  arr; //自动重装载值
    TIM3_Handler.Init.ClockDivision  =  TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);        //初始化PWM
    
    TIM3_CH4Handler.OCMode      = TIM_OCMODE_PWM1;    //模式选择PWM1
    TIM3_CH4Handler.Pulse       = arr/2;              //设置比较值,此值用来确定占空比
    TIM3_CH4Handler.OCPolarity  = TIM_OCPOLARITY_LOW; //输出比较极性为低
    
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4); //配置TIM3通道4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启PWM通道4
    
    System.Device.Pwm.SetDutyCycle = TIM_SetTIM3Compare4;
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM3_CLK_ENABLE();                //使能定时器3
    __HAL_RCC_GPIOB_CLK_ENABLE();               //开启GPIOB时钟
    GPIO_Initure.Pin       = GPIO_PIN_1;        //PB1
    GPIO_Initure.Mode      = GPIO_MODE_AF_PP;   //复用推挽输出
    GPIO_Initure.Pull      = GPIO_PULLUP;       //上拉
    GPIO_Initure.Speed     = GPIO_SPEED_HIGH;   //高速
    GPIO_Initure.Alternate = GPIO_AF2_TIM3;     //PB1复用为TIM3_CH4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}


/*******************************************************************************
* 描述	 : 系统时钟100/S，即10mS间隔调用一次运行，动态调整PWM波占空比,实现呼吸灯
*******************************************************************************/
void PwmSystick100Routine(void)
{
    if(dir)
      led0pwmval++;     //dir==1 led0pwmval递增
    else 
      led0pwmval--;     //dir==0 led0pwmval递减
    if(led0pwmval>120)
      dir=0;            //led0pwmval到达120后，方向为递减
    if(led0pwmval==0)
      dir=1;            //led0pwmval递减到0后，方向改为递增
    
    System.Device.Pwm.SetDutyCycle(led0pwmval); //修改比较值，修改占空比
}



    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
