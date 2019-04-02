/*'
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : scada.c
*����       : ���ݲɼ�
*ԭ��       : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/7       ��ʼ�汾
********************************************************************************
*/

#include "system.h"
#include "App.h"

extern void IWDG_Refresh(void);

/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/


/*******************************************************************************
* ������    : ScadaTask
* ����	    : ScadaTask���񣬱���Ϊ������ȼ�����
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2017/12/13       ��ʼ�汾
*******************************************************************************/
void ScadaTask(void)
{
    volatile uint16_t i,j;
    if(Read433Para == 1)
    {
      Init433Module();
      IWDG_Start();      //�����������Ź�
    }

    while(true)             
    {	  
        if(GetBit(PowerOnFlag,Homepage) == 1)
        {
          HomePageShow();                      //��ҳ
        }
        else if(GetBit(PowerOnFlag,Menupage) == 1)
        {
          SetupPageShow(MenuChooseIndex);      //���ò˵�
        }
        else if(GetBit(PowerOnFlag,Recvpage) == 1)
        {
          SelectReceiverMenuShow();            //ѡ����Ž���
        }
        else if(GetBit(PowerOnFlag,Parapage) == 1)
        {
          if(ParaMenuFlag == 0)
            ParameterSetShow(ParaChooseIndex); //�������ý���
          else
          {
            if(ParaAskFlag == 1)
            {
              ParaAskFlag = 0;
              ParameterInqury();
            }
            ParameterSetupMenu(ParaChooseIndex);  //������������Ӳ˵�ҳ��
          }
        }
        else if(GetBit(PowerOnFlag,Langpage) == 1)
        {
          LanguageConfirm = 1;
          LanguageSetMenu();
        }
        else if(GetBit(PowerOnFlag,Systpage) == 1)
        {
          if(SystemSetFlag == 0)               //ϵͳ���ý���
            SystemSetPageShow(SysChooseIndex);
          else
            SystemSetupMenu(SysChooseIndex);   //����ϵͳ�����Ӳ˵�ҳ��
        }
        else if(GetBit(PowerOnFlag,Exitpage) == 1)
        {
          PowerOnFlag = 0;
          GUI_WindowsHide(&winxp);
          SetBit(PowerOnFlag,Homepage);
        }
        else if(GetBit(PowerOnFlag,Poffpage) == 1)
        {
          PoweroffMenu();
        }

        GUI_Exec();
        DelayMs(100);
        IWDG_Refresh();
    }
}




