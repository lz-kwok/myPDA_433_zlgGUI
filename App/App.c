/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : App.c
*作用       : 主函数
*原理       : 任务创建，系统启动
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/7       初始版本
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
* 描述	    : 初始化数据库
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
    System.OS.CreateLogicTask(LogicTask);   // 创建业务逻辑任务
    System.OS.CreateScadaTask(ScadaTask);   // 创建数据采集任务
    System.OS.Start();

    EnableIrq();                            // 开中断
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
