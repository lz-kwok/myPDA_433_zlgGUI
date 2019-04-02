/*'
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : scada.c
*作用       : 数据采集
*原理       : 
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/7       初始版本
********************************************************************************
*/

#include "system.h"
#include "App.h"

extern void IWDG_Refresh(void);

/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/


/*******************************************************************************
* 函数名    : ScadaTask
* 描述	    : ScadaTask任务，必须为最低优先级任务，
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2017/12/13       初始版本
*******************************************************************************/
void ScadaTask(void)
{
    volatile uint16_t i,j;
    if(Read433Para == 1)
    {
      Init433Module();
      IWDG_Start();      //开启独立看门狗
    }

    while(true)             
    {	  
        if(GetBit(PowerOnFlag,Homepage) == 1)
        {
          HomePageShow();                      //主页
        }
        else if(GetBit(PowerOnFlag,Menupage) == 1)
        {
          SetupPageShow(MenuChooseIndex);      //设置菜单
        }
        else if(GetBit(PowerOnFlag,Recvpage) == 1)
        {
          SelectReceiverMenuShow();            //选择机号界面
        }
        else if(GetBit(PowerOnFlag,Parapage) == 1)
        {
          if(ParaMenuFlag == 0)
            ParameterSetShow(ParaChooseIndex); //参数设置界面
          else
          {
            if(ParaAskFlag == 1)
            {
              ParaAskFlag = 0;
              ParameterInqury();
            }
            ParameterSetupMenu(ParaChooseIndex);  //进入参数设置子菜单页面
          }
        }
        else if(GetBit(PowerOnFlag,Langpage) == 1)
        {
          LanguageConfirm = 1;
          LanguageSetMenu();
        }
        else if(GetBit(PowerOnFlag,Systpage) == 1)
        {
          if(SystemSetFlag == 0)               //系统设置界面
            SystemSetPageShow(SysChooseIndex);
          else
            SystemSetupMenu(SysChooseIndex);   //进入系统设置子菜单页面
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




