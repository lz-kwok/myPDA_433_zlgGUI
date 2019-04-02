/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : logic.c
*����       : ҵ���߼������ļ�
*ԭ��       : ���������Ϣ���簴�����������ȣ���׼��һЩ�����Ĵ���ģʽ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/7       ��ʼ�汾
********************************************************************************
*/
#include "system.h"

extern uint8_t CheckCode;
extern uint8_t MasterBuf[8];

static uint8_t AppTick = 0;
static uint8_t BatTick = 0;

static void  RSSIDet(void);

//uint8_t ReadCMD[8] = {0x0C,0x03,0x00,0x13,0x00,0x01,0x74,0xD2};

/*******************************************************************************
* ����	    : ����2��Ϣ������
* �������  : data��������Ϣֵ
*******************************************************************************/
void Usart1Process(byte data)
{
    static uint8_t m = 0;
    switch(data)
    {
        case 0x01:
            if(Uart1enth>8)
              Uart1enth = 8;
              
            ShowString(rxBuff,CmdShow,Uart1enth);
            if(Uart1enth == 5)
            {
                for(m=10;m<16;m++)
                  CmdShow[m] = ' ';
            }
            else if(Uart1enth == 7)
            {
                for(m=14;m<16;m++)
                  CmdShow[m] = ' ';
            }
            Uart1enth = 0;
            break;
            
        case 0x02:             

            break;      
            
        case 0x03:       

          break;
          
        case 0xC0:       

          break;   
    }
    Ack_Flag = 0;
    Uart1enth = 0;
}

   



 
static void AppSystick10000(void)
{
    uint16_t regAddr = 0;
    static uint8_t FailTick = 0;
    AppTick ++;

    if(GetBit(Menuhierarchy,1) == 0)       //�����󣬻�1s�ٽ���ɨ��
      KeyStatusDetect();
    
    if(AppTick == 10)
    {
        AppTick = 0;
        BatTick ++;
        
        if((GetBit(PowerOnFlag,Homepage) == 1)&&(GetBit(PowerOnFlag,Menupage) == 0))
        {
            if(BatTick == 1)
            {
//                PostMessage(Message433,0x01); 
              regAddr = 40020;
            }
            else
            {
                if(BatTick != 100)
                {
                    regAddr = 40020;
                    BatFlag = 0;
                }
                else
                {
                    regAddr = 40033;
                    BatTick = 0;
                    BatFlag = 1;
                }
            }
         
            if((EnableRead == 1)&&(Read433Para == 0)&&(writeCtr == 0))
            {
              if(writeConfirm == 1)
                System.Device.Usart1.WriteNData(MasterBuf,8);
              else
                ModbusMasterRead(SlaverAddr,regAddr,0x03,0x00,0x01);
            }
            
            if(TransmitFail == 0)
            {
                FailTick++;
                
                if(FailTick == 20)
                {
                    FailTick = 0;
                    NonAck = 1;
                }
            }
            else if(TransmitFail == 1)
            {
                FailTick = 0;
                TransmitFail = 0;
                NonAck = 0;
            }
        }
    }
}

static void InitLogic(void)
{
    System.Device.Systick.Register(Systick100, AppSystick10000);
}


void LogicTask(void)
{      
    uint message;
    uint data; 
    InitLogic();
       
    //�߼�ҵ�������ȡ��Ϣ�����䴦��
    while(true)
    {    						
        message = System.OS.PendMessageQueue();        
        data = message & 0x00FFFFFF;
        
        switch(message>>24)
        {
            case MessageKey:                                   //������Ϣ
                KeyProcess(data);
                break;
//            case Message433:
//                RSSIDet();
//                break;
            case MessageTimer:
                Function(data + RomBase);	   //
                break;
            default:
                break;
        }					
    }
}




static void  RSSIDet(void)
{
    RssiFlag = 1;
    SETB_SET();
    DelayMs(5);
    SETB_CLR();
}









