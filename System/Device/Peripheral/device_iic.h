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
#ifndef __DEVICE_IIC__H
#define __DEVICE_IIC__H

/* Includes ------------------------------------------------------------------*/
#define IIC_SPEED                            50000                 
#define IIC_TIMEOUT_MAX                      0x1000        /*<! The value of the maximal timeout for BUS waiting loops */

/* I2C peripheral configuration defines */
#define IIC_EXPBD                            I2C1
#define IIC_EXPBD_CLK_ENABLE()               __I2C1_CLK_ENABLE()
#define IIC_EXPBD_SCL_SDA_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define IIC_EXPBD_SCL_SDA_AF                 GPIO_AF4_I2C1
#define IIC_EXPBD_SCL_SDA_GPIO_PORT          GPIOB
#define IIC_EXPBD_SCL_PIN                    GPIO_PIN_8
#define IIC_EXPBD_SDA_PIN                    GPIO_PIN_9

#define IIC_EXPBD_FORCE_RESET()              __I2C1_FORCE_RESET()
#define IIC_EXPBD_RELEASE_RESET()            __I2C1_RELEASE_RESET()

/* I2C interrupt requests */
#define IIC_EXPBD_EV_IRQn                    I2C1_EV_IRQn
#define IIC_EXPBD_ER_IRQn                    I2C1_ER_IRQn




static uint8_t IIC_ReadData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size );
static uint8_t IIC_WriteData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size );
uint8_t InitIIC1(void);



#endif /* __DEVICEUART__H */
