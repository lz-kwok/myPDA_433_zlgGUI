/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : App.c
*����       : ������
*ԭ��       : ���񴴽���ϵͳ����
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/7       ��ʼ�汾
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "system.h"
#include "App.h"
#include "Logic.h"
#include "Scada.h"
#include "data.h"



/* Private functions ---------------------------------------------------------*/
AppStruct App;

/*******************************************************************************
* ����	    : ��ʼ�����ݿ�
*******************************************************************************/
static void InitData(void)
{
    AppDataPointer = &(App.Data);
    
    AppDataPointer->Adc.A0 = 2400;
    App.Data.Voltage = 42;
    
    ABValue = FlashRead(0);
    if(ABValue > 3)
      ABValue = 1;
    
    TurnoffIndex = FlashRead(1);
    if(TurnoffIndex > 20)
      TurnoffIndex = 1;
    
    InOutValue = FlashRead(2);
    if(InOutValue > 20)
      InOutValue = 1;
}

  

int main( void )
{
    InitData();
    HAL_Init();  
    System.Init();    
    System.OS.CreateLogicTask(LogicTask);   // ����ҵ���߼�����
    System.OS.CreateScadaTask(ScadaTask);   // �������ݲɼ�����
    System.OS.Start();

    EnableIrq();                            // ���ж�
}




/**
 * @brief  This function is executed in case of error occurrence
 * @param  None
 * @retval None
 */
void Error_Handler( void )
{
;
//  while (1)
//  {}
}



#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed( uint8_t *file, uint32_t line )
{

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  while (1)
  {}
}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
