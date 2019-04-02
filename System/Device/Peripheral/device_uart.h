/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*                              主芯片:STM32F411re/cc
*                           江苏南大五维电子科技有限公司
*
*                               移植/裁剪：郭良志
********************************************************************************
*文件名     : device_uart.h
*作用       : 串口设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/6        初始版本
********************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_UART__H
#define __DEVICE_UART__H

/* Includes ------------------------------------------------------------------*/
#include "system.h"


/* Exported defines ----------------------------------------------------------*/
#define UART_BufferSize 256

/* User can use this section to tailor USARTx/UARTx instance used and associated resources */
/* Definition for USARTx clock resources */

/* Definition for USART1 Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA

#define USART1_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define USART1_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART1_FORCE_RESET()             __USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()           __USART1_RELEASE_RESET()

#define USART1_TX_AF                     GPIO_AF7_USART1
#define USART1_RX_AF                     GPIO_AF7_USART1


/* Definition for USART2 Pins */
#define USART2_TX_PIN                    GPIO_PIN_2
#define USART2_TX_GPIO_PORT              GPIOA
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA   

#define USART2_CLK_ENABLE()              __USART2_CLK_ENABLE();
#define USART2_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART2_FORCE_RESET()             __USART2_FORCE_RESET()
#define USART2_RELEASE_RESET()           __USART2_RELEASE_RESET()

#define USART2_TX_AF                     GPIO_AF7_USART2
#define USART2_RX_AF                     GPIO_AF7_USART2

/* Definition for USART6 Pins */
#define USART6_TX_PIN                    GPIO_PIN_6
#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_RX_PIN                    GPIO_PIN_7
#define USART6_RX_GPIO_PORT              GPIOC  

#define USART6_CLK_ENABLE()              __USART6_CLK_ENABLE();
#define USART6_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USART6_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USART6_FORCE_RESET()             __USART6_FORCE_RESET()
#define USART6_RELEASE_RESET()           __USART6_RELEASE_RESET()

#define USART6_TX_AF                     GPIO_AF8_USART6
#define USART6_RX_AF                     GPIO_AF8_USART6

 
/* Exported variables --------------------------------------------------------*/
extern volatile uint8_t UART_RxBuffer[UART_BufferSize];
extern volatile uint8_t UART_TxBuffer[UART_BufferSize];
extern const uint32_t Usart_BaudRate;
extern UART_HandleTypeDef UartHandle;



/* Exported functions ------------------------------------------------------- */
void USART1_SendString(uint8_t *s);
void USART2_SendString(uint8_t *s);
void InitUsart1(uint32_t Usart_BaudRate);
void InitUsart2(uint32_t Usart_BaudRate);
void Uart1000Routine(void);
void ParaAnalyze(void);



#endif /* __DEVICEUART__H */
