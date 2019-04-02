/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_uart.c
*作用       : 串口设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/6        初始版本
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "device_uart.h"

static uint32_t FrequenceData = 0;
/* Private variables ---------------------------------------------------------*/
uint8_t UART1_RxBuffer[UART_BufferSize];
uint8_t UART1_TxBuffer[UART_BufferSize];
uint8_t UART2_RxBuffer[UART_BufferSize];
uint8_t UART2_TxBuffer[UART_BufferSize];
uint8_t UART6_RxBuffer[UART_BufferSize];
uint8_t UART6_TxBuffer[UART_BufferSize];




/**
 * @brief  USART1_SendByte
 * @param  *s
 * @retval None
 */
void USART1_SendByte(uint8_t data)
{
    HAL_UART_Transmit( &Uart1Handle, &data, 1, 1000);
}

void USART1_SendNByte(uint8_t *data,ushort len)
{
    HAL_UART_Transmit( &Uart1Handle, data, len, 1000);
}

/**
 * @brief  USART1_SendString
 * @param  *s
 * @retval None
 */
void USART1_SendString(uint8_t *s)
{
    while(*s != '\0')
    {
        HAL_UART_Transmit( &Uart1Handle, s, 1, 1000);
        s++;
    }
}



/**
 * @brief  Configure the USART
 * @param  None
 * @retval None
 */
void InitUsart1(uint32_t Usart_BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO TX/RX clock */
    USART1_TX_GPIO_CLK_ENABLE();
    USART1_RX_GPIO_CLK_ENABLE();
    
    /* Enable USART1 clock */
    USART1_CLK_ENABLE();
    
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART1_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USART1_TX_AF;
    
    HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART1_RX_PIN;
    GPIO_InitStruct.Alternate = USART1_RX_AF;
    
    HAL_GPIO_Init( USART1_RX_GPIO_PORT, &GPIO_InitStruct );
    
    /*##-1- Configure the UART peripheral ######################################*/
    
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART1 configured as follow:
            - Word Length = 8 Bits
            - Stop Bit = One Stop bit
            - Parity = None
            - BaudRate = 'Usart_BaudRate' baud
            - Hardware flow control disabled (RTS and CTS signals) */
    Uart1Handle.Instance        = USART1;
    Uart1Handle.Init.BaudRate   = Usart_BaudRate;
    Uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
    Uart1Handle.Init.StopBits   = UART_STOPBITS_1;
    Uart1Handle.Init.Parity     = UART_PARITY_NONE;
    Uart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    Uart1Handle.Init.Mode       = UART_MODE_TX_RX;
    
    if ( HAL_UART_Init( &Uart1Handle ) != HAL_OK )
    {
      while( 1 );
    }
    
    Uart1Handle.pRxBuffPtr = ( uint8_t * )UART1_RxBuffer;
    Uart1Handle.RxXferSize = UART_BufferSize;
    Uart1Handle.ErrorCode  = HAL_UART_ERROR_NONE;
    
    /* NVIC for USART1 */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    
    if(HAL_UART_Receive_IT(&Uart1Handle,UART1_RxBuffer,1)!=HAL_OK)
      Error_Handler();
    
#ifdef Printf2Uart1
    am_util_stdio_printf_init((am_util_stdio_print_char_t)
    USART1_SendString);  
#endif
    
    System.Device.Usart1.WriteData   = USART1_SendByte; 
    System.Device.Usart1.WriteString = USART1_SendString; 
    System.Device.Usart1.WriteNData  = USART1_SendNByte; 
}

/**
 * @brief  USART2_SendByte
 * @param  *s
 * @retval None
 */
void USART2_SendByte(uint8_t data)
{
    HAL_UART_Transmit( &Uart2Handle, &data, 1, 1000);
}

/**
 * @brief  USART2_SendString
 * @param  *s
 * @retval None
 */
void USART2_SendString(uint8_t *s)
{
    while(*s != '\0')
    {
        HAL_UART_Transmit(&Uart2Handle, s, 1,1000);
        s++;
    }
}

/**
 * @brief  Configure the USART
 * @param  None
 * @retval None
 */
void InitUsart2(uint32_t Usart_BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStruct;
   
    /* Enable GPIO TX/RX clock */
    USART2_TX_GPIO_CLK_ENABLE();
    USART2_RX_GPIO_CLK_ENABLE();
    
    /* Enable USART2 clock */
    USART2_CLK_ENABLE();
     
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART2_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USART2_TX_AF;
    
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART2_RX_PIN;
    GPIO_InitStruct.Alternate = USART2_RX_AF;
    
    HAL_GPIO_Init( USART2_RX_GPIO_PORT, &GPIO_InitStruct );
    
    /*##-1- Configure the UART peripheral ######################################*/
    
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART2 configured as follow:
            - Word Length = 8 Bits
            - Stop Bit = One Stop bit
            - Parity = None
            - BaudRate = 'Usart_BaudRate' baud
            - Hardware flow control disabled (RTS and CTS signals) */
    Uart2Handle.Instance        = USART2;
    Uart2Handle.Init.BaudRate   = Usart_BaudRate;
    Uart2Handle.Init.WordLength = UART_WORDLENGTH_8B;
    Uart2Handle.Init.StopBits   = UART_STOPBITS_1;
    Uart2Handle.Init.Parity     = UART_PARITY_NONE;
    Uart2Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    Uart2Handle.Init.Mode       = UART_MODE_TX_RX;
    
    if ( HAL_UART_Init( &Uart2Handle ) != HAL_OK )
    {
      while( 1 );
    }
    
    Uart2Handle.pRxBuffPtr = ( uint8_t * )UART2_RxBuffer;
    Uart2Handle.RxXferSize = UART_BufferSize;
    Uart2Handle.ErrorCode  = HAL_UART_ERROR_NONE;
    
    /* NVIC for USART1 */
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    
    if(HAL_UART_Receive_IT(&Uart2Handle,UART2_RxBuffer,1)!=HAL_OK)
      Error_Handler();
   
#ifdef Printf2Uart2
       am_util_stdio_printf_init((am_util_stdio_print_char_t)
    USART2_SendString);  
#endif
    
    System.Device.Usart2.WriteData = USART2_SendByte; 
    System.Device.Usart2.WriteString = USART2_SendString; 
}

static uint8_t CheckLength = 0;
static uint8_t CheckTick = 0;


void Uart1000Routine(void)
{  
  if(Read433Para == 0)
  {
    if((rxBuff[0] != SlaverAddr))
      Uart1enth = 0;
  }
  
  if((GetBit(PowerOnFlag,Homepage) == 1)&&(GetBit(PowerOnFlag,Menupage) == 0))
  {
      if(CheckLength == Uart1enth)
      {
        CheckTick++;
        if(CheckTick == 50)
        {
          CheckTick = 0;
          EnableRead = 0;
          InitUsart1(38400);
          EnableRead = 1;
        }
      }
      if(CheckLength != Uart1enth)
        CheckLength = Uart1enth;
  }
}

void ParaAnalyze(void)
{
  switch(ParaChooseIndex)
  {
    case 0x00:
      ForceSetIndex = rxBuff[4];
      break;
    case 0x01:
      InOutIndex = rxBuff[4];
      break;
    case 0x02:
      SpacingIndex = rxBuff[3]*256 + rxBuff[4];
      break;
    case 0x03:
      LengthSetIndex = rxBuff[3]*256 + rxBuff[4];
      break;
    case 0x04:
      MovementSetIndex = rxBuff[4];
      break;
    case 0x05:
      DelaySetIndex = rxBuff[4];
      break;
    case 0x06:
      OutNumIndex = rxBuff[4];
      break;
    case 0x07:
      ABSetIndex = rxBuff[4];
      break;
  }
}

static void DataAnalyzeCallBack(void)
{
    TransmitFail = 1;
    NonAck = 0;
    CalcuResult = Crc16(rxBuff,Uart1enth-2);
    CRC_Result[0] = (unsigned char)((CalcuResult & 0xFF00) >> 8);
    CRC_Result[1] = (unsigned char)(CalcuResult & 0xFF);

    if((rxBuff[Uart1enth-2] == CRC_Result[0]) && (rxBuff[Uart1enth-1] == CRC_Result[1]))   //判断数据接收是否存在异常
    {
        switch(rxBuff[1])
        {
          case 0x03:
            Enableupdate = 1;
            if(BatFlag == 1)
              RecPower = rxBuff[4];
            else if(BatFlag == 2)
            {
              BatFlag = 0;
              ParaAnalyze();
            }
            else
              AckValue = rxBuff[3]*256 + rxBuff[4];
            break;
          case 0x06:
            EnableRead = 1;
            writeConfirm = 0;
            break;

        }
    }
    
    Uart1enth = 0;
}

uint8_t FreqAnalyze(uint32_t dat)
{
    switch(dat)
    {
      case 433920:
        return 1;
        break;
      case 432920:
        return 2;
        break;
      case 431920:
        return 3;
        break;
      case 434920:
        return 4;
        break;
      case 435920:
        return 5;
        break;
      case 436920:
        return 6;
        break;
      case 437920:
        return 7;
        break;
      case 438920:
        return 8;
        break;
    }
    
    return 1;
}

/**
 * @brief  USART Interrupt Function Callback
 * @param  None
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{  
    if(UartHandle->Instance==USART1)
    {      
      if(EnableRead == 1)
      {
        rxBuff[Uart1enth++] = UART1_RxBuffer[0];
      }
      
      if((Read433Para == 1)&&(Uart1enth == 19))
      {
        Uart1enth = 0;
        
        if((rxBuff[0] == 0x24)&&(rxBuff[1] == 0x24)&&(rxBuff[2] == 0x24))
        {
          Read433Para = 0;
          DevIdChooseIndex = rxBuff[16];
          
          FrequenceData = rxBuff[5]*65536 + rxBuff[6]*256 + rxBuff[7];
          FreSetIndex = FreqAnalyze(FrequenceData);
        }
      }
      else
      {
        if((Uart1enth == 3)&&(rxBuff[0] == SlaverAddr))
          System.Device.Timer.Start(2,TimerSystick,10,DataAnalyzeCallBack);
      }
    } 
    
}


/**
 * @}
 */
void USART1_IRQHandler(void)  
{  
    EnterCritical();
    HAL_UART_IRQHandler(&Uart1Handle);
    ExitCritical();
    HAL_UART_Receive_IT(&Uart1Handle,UART1_RxBuffer,1);       //开启下一次接收中断  
} 

/**
 * @}
 */
void USART2_IRQHandler(void)  
{  
    HAL_UART_IRQHandler(&Uart2Handle);
} 






