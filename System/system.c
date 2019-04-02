/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���   : system.c
*����     : ϵͳӦ���ļ�
*����ʱ�� : 2016.9.12
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
* ������       : ucharFucCheckSum(uchar *i,ucharln)
* ����         :���У�飨ȡ���͡�����Э���1\2\3\4\5\6\7�ĺ�ȡ��+1��
* ����˵��     :�������Ԫ��1-�����ڶ���Ԫ����Ӻ�ȡ��+1��Ԫ�ظ����������2��
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
* ������    : PostMessage
* ����	    : ��LogicTask������Ϣ
* �������  : messageType: ��Ϣ����
*             data: ��Ϣֵ
* ���ز���  : ��
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


