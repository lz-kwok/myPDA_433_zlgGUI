/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名   : device_sysclock.c
*创建时间 : 2016.12.20
*******************************************************************************/
/*
   OSC_VALUE为HSE_VALUE或HSI_VALUE，根据实际选择使用
   注意：在8M外部晶振的条件下，F411RE最多超频到144M，不可越级使用(内部HSI也一样)
   PLLM = (OSC_VALUE/1000000); PLLN = 96;  PLLP = 8; PLLQ = 2; break; //12MHz
   PLLM = (OSC_VALUE/1000000); PLLN = 96;  PLLP = 6; PLLQ = 2; break; //16MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 72;  PLLP = 8; PLLQ = 3; break; //18MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 96;  PLLP = 8; PLLQ = 4; break; //24MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 120; PLLP = 8; PLLQ = 5; break; //30MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 96;  PLLP = 6; PLLQ = 4; break; //32MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 144; PLLP = 8; PLLQ = 6; break; //36MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 120; PLLP = 6; PLLQ = 5; break; //40MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 168; PLLP = 8; PLLQ = 7; break; //42MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 192; PLLP = 8; PLLQ = 8; break; //48MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 216; PLLP = 8; PLLQ = 9; break; //54MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 168; PLLP = 6; PLLQ = 7; break; //56MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 120; PLLP = 4; PLLQ = 5; break; //60MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 192; PLLP = 6; PLLQ = 8; break; //64MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 144; PLLP = 4; PLLQ = 6; break; //72MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 240; PLLP = 6; PLLQ = 10;break; //80MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 168; PLLP = 4; PLLQ = 7; break; //84MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 192; PLLP = 4; PLLQ = 8; break; //96MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 216; PLLP = 4; PLLQ = 9; break; //108MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 120; PLLP = 2; PLLQ = 5; break; //120MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 144; PLLP = 2; PLLQ = 6; break; //144MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 168; PLLP = 2; PLLQ = 7; break; //168MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 192; PLLP = 2; PLLQ = 8; break; //192MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 216; PLLP = 2; PLLQ = 9; break; //216MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 240; PLLP = 2; PLLQ = 10;break; //240MHz
   PLLM = (OSC_VALUE/2000000); PLLN = 260; PLLP = 2; PLLQ = 11;break; //260MHz

   PLLQ是设置，主要为了使USB时钟始终在48MHz
*/

#include "system.h"


/*******************************************************************************
* 函数名    : InitHSEClock
* 描述	    : 初始化外部晶振作为主时钟源
* 输入参数  : MainFreq
* 返回参数  : 无
*******************************************************************************/
void InitHSEClock(uint8_t MainFreq)
{
    static uint32_t PLLM = 0,PLLN = 0,PLLP = 0,PLLQ = 0;

    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* -1- Select HSI as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -2- Enable HSE Oscillator, select it as PLL source and finally activate the PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    
#ifdef HSE_CRYSTAL  
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
#elif defined (HSE_BYPASS)
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
#endif /* HSE_CRYSTAL */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    
    switch(MainFreq)
    {
        case 48 :  
          PLLM = (HSE_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV8; PLLQ = 8;   //48MHz
          break;        
        case 72:  
          PLLM = (HSE_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV4; PLLQ = 6;   //72MHz
          break;        
        case 80:  
          PLLM = (HSE_VALUE/2000000); PLLN = 240; PLLP = RCC_PLLP_DIV6; PLLQ = 10;  //80MHz
          break;        
        case 96:  
          PLLM = (HSE_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV4; PLLQ = 8;   //96MHz
          break; 
        case 108:
          PLLM = (HSE_VALUE/2000000); PLLN = 216; PLLP = RCC_PLLP_DIV4; PLLQ = 9;   //108MHz
          break;
        case 144:  
          PLLM = (HSE_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV2; PLLQ = 6;   //144MHz
          break;
        default:
          PLLM = (HSE_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV4; PLLQ = 8;   //96MHz
          break;        
    }
        
    RCC_OscInitStruct.PLL.PLLM = PLLM;
    RCC_OscInitStruct.PLL.PLLN = PLLN;
    RCC_OscInitStruct.PLL.PLLP = PLLP;
    RCC_OscInitStruct.PLL.PLLQ = PLLQ;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
    
    /* -3- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -4- Optional: Disable HSI Oscillator (if the HSI is no more needed by the application) */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
}


/*******************************************************************************
* 函数名    : InitHSEClock
* 描述	    : 初始化外部晶振作为主时钟源
* 输入参数  : MainFreq
* 返回参数  : 无
*******************************************************************************/
void InitHSIClock(uint8_t MainFreq)
{
    static uint32_t PLLM = 0,PLLN = 0,PLLP = 0,PLLQ = 0;
  
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    
    /* -1- Select HSE as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -2- Enable HSI Oscillator, select it as PLL source and finally activate the PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    
    switch(MainFreq)
    {
        case 16 : 
          PLLM = (HSI_VALUE/1000000); PLLN = 96;  PLLP = RCC_PLLP_DIV6; PLLQ = 2;   //16MHz
          break; 
        case 24:
          PLLM = (HSI_VALUE/2000000); PLLN = 96;  PLLP = RCC_PLLP_DIV8; PLLQ = 4;   //24MHz
          break; 
        case 48 :  
          PLLM = (HSI_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV8; PLLQ = 8;   //48MHz
          break;        
        case 72:  
          PLLM = (HSI_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV4; PLLQ = 6;   //72MHz
          break;        
        case 80:  
          PLLM = (HSI_VALUE/2000000); PLLN = 240; PLLP = RCC_PLLP_DIV6; PLLQ = 10;  //80MHz
          break;        
        case 96:  
          PLLM = (HSI_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV4; PLLQ = 8;   //96MHz
          break; 
        case 108:
          PLLM = (HSI_VALUE/2000000); PLLN = 216; PLLP = RCC_PLLP_DIV4; PLLQ = 9;   //108MHz
          break;
        case 144:  
          PLLM = (HSI_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV2; PLLQ = 6;   //144MHz
          break;
        default:
          PLLM = (HSI_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV4; PLLQ = 6;   //72MHz   
          break;        
    }
    
    RCC_OscInitStruct.PLL.PLLM = PLLM;
    RCC_OscInitStruct.PLL.PLLN = PLLN;
    RCC_OscInitStruct.PLL.PLLP = PLLP;
    RCC_OscInitStruct.PLL.PLLQ = PLLQ;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* -3- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType         = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource      = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider     = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider    = RCC_HCLK_DIV2;  
    RCC_ClkInitStruct.APB2CLKDivider    = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -4- Optional: Disable HSE Oscillator (if the HSE is no more needed by the application) */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
}