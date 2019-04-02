/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名   : system.h
*作用     : 系统库全局头文件
*创建时间 : 2016.9.12
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif


/*****************以下是系统包含头文件,默认打开了FPU***************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "stm32f4xx.h"          //必须在第一个，否则会引起编译错误
#include "math.h"
#include "arm_math.h"  
#include "system_stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_tim.h" 
#include "stm32f4xx_hal_pwr.h"

#include "device_sysclock.h"
#include "systemglobal.h"
 
  
  
#include "device_systick.h"
#include "device_gpio.h"
#include "device_uart.h"
#include "device_key.h"
#include "device_printf.h"
#include "device_timer.h"
#include "device_pwm.h"
#include "device_iic.h"
#include "device_adc.h"
#include "device_spi.h"
#include "device_flash.h"
#include "device_crc.h"  
#include "device_oled.h"
#include "device_433.h"
#include "device_menu.h"


#include "config.h"

/*
1、类型定义兼容C#，尽可能用C#标准，常用的为int
2、char在C#中为16bit的unicode，keil C下为8bit
*/
//8bit
//typedef char                sbyte;	    
typedef unsigned char       byte;       
//16bit
//typedef short             short
typedef unsigned short      ushort;
//32bit
//typedef int               int;
typedef unsigned int        uint;

//字符串
typedef char *              string;

//typedef unsigned char       bool;

typedef void (*function)(void);
#define Function(address)   ((function)(address))()

typedef enum
{
    MessageKey      = 0x01,     // 消息类型不能为0x00
    MessageTimer    = 0x02,
    MessageUsart1   = 0x03,
    MessageUsart2   = 0x04,
    Message433      = 0x05,
    /*请填充类型*/
}MessageEnum;


typedef enum
{
    RedOn           = 0x01,     // 消息类型不能为0x00
    GreenOn         = 0x02,
    BlueOn          = 0x03,
    AllOn           = 0x04,
    AllOff          = 0x05
}LedEnum;

#define SlaverAddr              0x0C

//以下头文件必须在这个位置
#include "os.h"
#include "device.h"  
#include "data.h"


#define true                        1
#define false                       0

#define on                          1
#define off                         0

#define null                        ((void *) 0)                    // 空指针
#define invalid                     0xFF                            // 无效值
#define MainClock                   72000000                        // 系统时钟频率
#define IdentifyNumberAddress       0x1FFFF7E8                      // 芯片ID号

#define SetBit(data, offset)        ((data) |= 1U << (offset))      // 置位
#define ResetBit(data, offset)      ((data) &= ~(1U << (offset)))   // 复位
#define GetBit(data, offset)        (((data) >> (offset)) & 0x01)   // 获取位

#define Byte0(data)                 ((byte *)(&(data)))[0]
#define Byte1(data)                 ((byte *)(&(data)))[1]
#define Byte2(data)                 ((byte *)(&(data)))[2]
#define Byte3(data)                 ((byte *)(&(data)))[3]

#define Ushort0(data)               ((ushort *)(&(data)))[0]
#define Ushort1(data)               ((ushort *)(&(data)))[1]

#define Byte(data)                  *((byte *)(&data)) 
#define Ushort(data)                *((ushort *)(&data)) 
#define Uint(data)                  *((uint *)(&data)) 
#define Float(data)                 *(float *)(&data)

#define pByte(address)              *((byte *)(address)) 
#define pUshort(address)            *((ushort *)(address)) 
#define pUint(address)              *((uint *)(address)) 
#define pFloat(address)             *(float *)(address)

//#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0
#define Assert(express)  (void)0

/************ Uncomment to enable the adaquate RTC Clock Source **************/
#define RTC_CLOCK_SOURCE_LSE
//#define RTC_CLOCK_SOURCE_LSI
 				  
extern uint Empty;
extern void Dummy(void);
extern ushort CriticalNesting;
#define	EnterCritical()		__disable_irq(); CriticalNesting++;
#define ExitCritical()		if(--CriticalNesting == 0){__enable_irq();}

#define EnableIrq()         __enable_irq();
#define DisableIrq()        __disable_irq();

extern uint RomBase;
extern uint RamBase;
extern DataStruct * AppDataPointer;

extern void Delay(int times);

extern void DelayMs(int times);

extern byte HexToAscii(byte hex);

extern byte SumCheck(byte *data,byte num);
extern byte FucCheckSum(byte *data,byte num);

extern void PostMessage(MessageEnum message, uint data);


/*******************************************************************************
* 结构体    : SystemStruct
* 描述	    : msOS中最重要的一个结构体，封装了System层的所有接口
********************************************************************************
*版本     作者            日期            说明
*V0.1    guolz         2016/09/12       初始版本
*******************************************************************************/
typedef struct
{
    void (*Init)(void);
    
    struct Device
    {     
        struct Adc
        {
            void (*Register)(AdcChannelEnum channel, ushort * dataPointer);
        }Adc;
        
        struct Usart1
        {
            void (*WriteData)(byte data);
            void (*WriteNData)(byte *data,ushort len);
            void (*WriteString)(byte  *s);
        }Usart1;	

        struct Usart2
        {
            void (*WriteData)(byte data);
            void (*WriteString)(byte  *s);
        }Usart2;
        
        struct Usart6
        {
            void (*WriteData)(byte data);
            void (*WriteString)(byte  *s);
        }Usart6;

        struct Iic1
        {
            byte (*WriteData)(byte SlaveAdd,byte reg,byte *data,uint16_t num);
            byte (*ReadData)(byte SlaveAdd,byte ReadAdd,byte *data,uint16_t NumToRead);
        }Iic1;	
     
        struct Spi1
        {
            byte (*WriteReadData)(byte TxData);
        }Spi1;	
		
        struct Timer
        {
            void (*Start)(int id, TimerModeEnum mode, int times, function functionPointer);
            void (*Stop)(int id); 
        }Timer;

        struct Systick
        {
            bool (*Register)(SystickEnum type, function functionPointer);
        }Systick;
        
        struct Pwm
        {
            void (*SetDutyCycle)(uint32_t DutyDat);
        }Pwm;
        
    }Device;

    struct OS
    {
        void (* CreateLogicTask)(function taskPointer);
        void (* CreateScadaTask)(function taskPointer);
        void (* Start)(void);
        bool (* PostMessageQueue)(uint message);
        uint (* PendMessageQueue)(void);
        void (* DelayMs)(int times); 
    }OS;
}SystemStruct;

extern SystemStruct System;




/* Exported functions ------------------------------------------------------- */
void Error_Handler( void );



#ifdef __cplusplus
}
#endif



#endif 
