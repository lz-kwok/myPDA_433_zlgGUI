/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : device_adc.c
*����       : ģ��ת��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/14        ��ʼ�汾
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"

#define ChanSum         5       // 4ͨ��


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handler;     //ADC���;

static uint16_t NullValue;
static uint16_t * RegisterPointerBlock[ChanSum];

static uint16_t Adc[ChanSum-1];
uint32_t ADSum = 0;
uint8_t ADTick = 0;


static void InitRegisterBlock(void)
{
    static uint8_t m;
    for (m = 0; m < ChanSum; m++)
        RegisterPointerBlock[m] = &NullValue;
}

static void PortRegister(void)
{
    AppDataPointer->Adc.A0 = Adc[0];
}

/*******************************************************************************
* ����	    : ��Ӧ�ò����ָ��ע�ᵽ��Ӧ��ͨ�������У�ʵ�ֵײ������򶥲㴫��
* �������  : adcChannel: ADCͨ����
*           : dataPoint: Ӧ�ò����ָ��
*******************************************************************************/
static void Register(AdcChannelEnum channel, ushort * dataPointer)
{
    RegisterPointerBlock[channel] = dataPointer;
}


/*******************************************************************************
* ������    : InitAdc
* ����	    : ADC��ʼ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitAdc(void)
{
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=72/4=18MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ���������
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE; //ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;        //�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                            //��ʼ��                               

    InitRegisterBlock();
    PortRegister();
    System.Device.Adc.Register = Register;
}


uint16_t GetAdcValue(uint8_t ch)
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;            //ͨ��
    ADC1_ChanConf.Rank=1;                //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES; //����ʱ��
    ADC1_ChanConf.Offset=0;
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf); //ͨ������
    HAL_ADC_Start(&ADC1_Handler);                        //����AD
    HAL_ADC_PollForConversion(&ADC1_Handler,10);         //��ѯת��
    
    return (uint16_t)HAL_ADC_GetValue(&ADC1_Handler);  //�������ת�����
}




/*******************************************************************************
* ����	    : ϵͳʱ��100/S����10mS�������һ�����У���ȡADCֵ��App�����
*******************************************************************************/
void AdcSystick100Routine(void)
{
    Adc[0] = GetAdcValue(0);
    PortRegister();
    *RegisterPointerBlock[0] = Adc[0];
    
    ADSum += Adc[0];
    ADTick ++;
    
    if(ADTick == 100)
    {
        ADTick = 0;
        ADSum = 0;
        if(ADSum > 180000)
        {
          AppDataPointer->Adc.A0 = ADSum/100;
        }
    }
}




