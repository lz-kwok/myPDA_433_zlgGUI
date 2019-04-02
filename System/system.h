/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���   : system.h
*����     : ϵͳ��ȫ��ͷ�ļ�
*����ʱ�� : 2016.9.12
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif


/*****************������ϵͳ����ͷ�ļ�,Ĭ�ϴ���FPU***************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "stm32f4xx.h"          //�����ڵ�һ�������������������
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
1�����Ͷ������C#����������C#��׼�����õ�Ϊint
2��char��C#��Ϊ16bit��unicode��keil C��Ϊ8bit
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

//�ַ���
typedef char *              string;

//typedef unsigned char       bool;

typedef void (*function)(void);
#define Function(address)   ((function)(address))()

typedef enum
{
    MessageKey      = 0x01,     // ��Ϣ���Ͳ���Ϊ0x00
    MessageTimer    = 0x02,
    MessageUsart1   = 0x03,
    MessageUsart2   = 0x04,
    Message433      = 0x05,
    /*���������*/
}MessageEnum;


typedef enum
{
    RedOn           = 0x01,     // ��Ϣ���Ͳ���Ϊ0x00
    GreenOn         = 0x02,
    BlueOn          = 0x03,
    AllOn           = 0x04,
    AllOff          = 0x05
}LedEnum;

#define SlaverAddr              0x0C

//����ͷ�ļ����������λ��
#include "os.h"
#include "device.h"  
#include "data.h"


#define true                        1
#define false                       0

#define on                          1
#define off                         0

#define null                        ((void *) 0)                    // ��ָ��
#define invalid                     0xFF                            // ��Чֵ
#define MainClock                   72000000                        // ϵͳʱ��Ƶ��
#define IdentifyNumberAddress       0x1FFFF7E8                      // оƬID��

#define SetBit(data, offset)        ((data) |= 1U << (offset))      // ��λ
#define ResetBit(data, offset)      ((data) &= ~(1U << (offset)))   // ��λ
#define GetBit(data, offset)        (((data) >> (offset)) & 0x01)   // ��ȡλ

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
* �ṹ��    : SystemStruct
* ����	    : msOS������Ҫ��һ���ṹ�壬��װ��System������нӿ�
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    guolz         2016/09/12       ��ʼ�汾
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
