/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : device_pwm.c
*����       : PWM�����
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/6        ��ʼ�汾
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "device_pwm.h"


/********Private Variables*****************************************************/
uint8_t dir = 1;
uint16_t led0pwmval=0;


TIM_HandleTypeDef   TIM3_Handler;     //��ʱ��3PWM���
TIM_OC_InitTypeDef  TIM3_CH4Handler;  //��ʱ��3ͨ��4���


void TIM_SetTIM3Compare4(uint32_t compare)
{
    TIM3->CCR4=compare;
}


/*******************************************************************************
* ������    : InitTim3PWM
* ����	    : ��ʼ��PWM����
* �������  : FreqkHz �������ΪkHz
* ���ز���  : ��
*******************************************************************************/
void InitTim3PWM(uint16_t FreqkHz)
{
    uint16_t psc = HAL_RCC_GetHCLKFreq()/1000000 - 1;
    uint16_t arr = 1000/FreqkHz - 1;  
      
    TIM3_Handler.Instance            =  TIM3; //��ʱ��3
    TIM3_Handler.Init.Prescaler      =  psc; //��ʱ����Ƶ
    TIM3_Handler.Init.CounterMode    =  TIM_COUNTERMODE_UP;   //���ϼ���ģʽ
    TIM3_Handler.Init.Period         =  arr; //�Զ���װ��ֵ
    TIM3_Handler.Init.ClockDivision  =  TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);        //��ʼ��PWM
    
    TIM3_CH4Handler.OCMode      = TIM_OCMODE_PWM1;    //ģʽѡ��PWM1
    TIM3_CH4Handler.Pulse       = arr/2;              //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ�
    TIM3_CH4Handler.OCPolarity  = TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ��
    
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4); //����TIM3ͨ��4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//����PWMͨ��4
    
    System.Device.Pwm.SetDutyCycle = TIM_SetTIM3Compare4;
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM3_CLK_ENABLE();                //ʹ�ܶ�ʱ��3
    __HAL_RCC_GPIOB_CLK_ENABLE();               //����GPIOBʱ��
    GPIO_Initure.Pin       = GPIO_PIN_1;        //PB1
    GPIO_Initure.Mode      = GPIO_MODE_AF_PP;   //�����������
    GPIO_Initure.Pull      = GPIO_PULLUP;       //����
    GPIO_Initure.Speed     = GPIO_SPEED_HIGH;   //����
    GPIO_Initure.Alternate = GPIO_AF2_TIM3;     //PB1����ΪTIM3_CH4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}


/*******************************************************************************
* ����	 : ϵͳʱ��100/S����10mS�������һ�����У���̬����PWM��ռ�ձ�,ʵ�ֺ�����
*******************************************************************************/
void PwmSystick100Routine(void)
{
    if(dir)
      led0pwmval++;     //dir==1 led0pwmval����
    else 
      led0pwmval--;     //dir==0 led0pwmval�ݼ�
    if(led0pwmval>120)
      dir=0;            //led0pwmval����120�󣬷���Ϊ�ݼ�
    if(led0pwmval==0)
      dir=1;            //led0pwmval�ݼ���0�󣬷����Ϊ����
    
    System.Device.Pwm.SetDutyCycle(led0pwmval); //�޸ıȽ�ֵ���޸�ռ�ձ�
}



    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
