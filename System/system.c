/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名   : system.c
*作用     : 系统应用文件
*创建时间 : 2016.9.12
********************************************************************************
*/

#include "system.h"


uint Empty;
void Dummy(void) {};



uint RomBase;
uint RamBase;
DataStruct * AppDataPointer;

static const byte HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


void Delay(int times)
{
  while(times--);
}

void DelayMs(int times)
{
    while(times--)
      HAL_Delay(1);
}


byte HexToAscii(byte hex)
{
    Assert(hex < 16);

    if (hex < 16) 
        return HextoAscii[hex];
    else 
         return 0x30;
}

byte SumCheck(byte *data,byte num)
{
    static byte ii = 0;
    uint32_t Sum = 0;
    byte result = 0;
    
    for(ii=0;ii<num;ii++)
    {
        Sum += data[ii];
    }
    
    result = Sum&0xff;
    Sum = 0;
    
    return result;
}


/**********************************************************************
* 函数名       : ucharFucCheckSum(uchar *i,ucharln)
* 描述         :求和校验（取发送、接收协议的1\2\3\4\5\6\7的和取反+1）
* 函数说明     :将数组的元素1-倒数第二个元素相加后取反+1（元素个数必须大于2）
**********************************************************************/
byte FucCheckSum(byte *data,byte num)
{
    unsigned char j,tempq=0;
    data+=1;
    
    for(j=0;j<(num-2);j++)
    {
      tempq += *data;
      data ++;
    }
    
    tempq = (~tempq)+1;
    
    return(tempq);
}


    
/*******************************************************************************
* 函数名    : PostMessage
* 描述	    : 向LogicTask发送消息
* 输入参数  : messageType: 消息类型
*             data: 消息值
* 返回参数  : 无
*******************************************************************************/
void PostMessage(MessageEnum message, uint data)
{
    Byte3(data) = message;
    System.OS.PostMessageQueue(data);	
}




static void Init(void)
{
    DisableIrq();
    
    RomBase = (uint)Init & 0xFF000000;
    RamBase = (uint)(&RamBase) & 0xFF000000;
    
    InitOs();
    InitDevice();
}

SystemStruct System = 
{
    Init,
};


