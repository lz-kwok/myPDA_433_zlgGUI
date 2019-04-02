/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名   : os.c
*创建时间 : 2016.12.7
*******************************************************************************/
#include "system.h"

uint16_t CriticalNesting = 0;


typedef struct                             // 定义队列类型
{
    uint    *   Start;                      // 指向队列开始
    uint    *   End;                        // 指向队列结束
    uint    *   In;                         // 插入一个消息
    uint    *   Out;                        // 取出一个消息
    ushort      Entries;                    // 消息长度
} QueueStruct;

typedef struct 
{
    uint *          StackPointer;           // 栈顶指针
    uint            Message;                // 消息
    uint            Delay;                  // 任务延时节拍数
} TaskStruct;

#define LogicTaskStackSum   800             // 业务逻辑任务栈深度
#define ScadaTaskStackSum   800             // 菜单界面任务栈深度
#define QueueBufferSum      400             // 消息队列深度

#define PendSVRegister *((volatile  uint *)0xE000ED04)
#define PendSVSet 0x10000000

#define SwitchContext() PendSVRegister = PendSVSet

static QueueStruct MessageQueue;
static uint QueueBuffer[QueueBufferSum];         // 业务逻辑消息队列

#pragma data_alignment = 8 
static uint LogicTaskStack[LogicTaskStackSum];   //业务逻辑任务栈数组
#pragma data_alignment = 8 
static uint ScadaTaskStack[ScadaTaskStackSum];     //菜单界面任务栈数组

static TaskStruct LogicTask;
static TaskStruct ScadaTask;

TaskStruct * CurrentTaskPointer;
TaskStruct * NextTaskPointer;

/*******************************************************************************
* 描述	    : 初始化任务栈，xPSR、PC、R14、R12、R3、R2、R1、R0入栈顺序是由Cortex
*           : 芯片决定的。中断入栈是芯片硬件自动完成，R11~R4顺序是由OS自己决定的。
* 输入参数  : taskPointer: 任务入口地址, stackRamTopPointer: 栈内存最高地址
* 返回参数  : 栈顶
*******************************************************************************/
//多增两项， 为适应开启FPU 
static uint * InitStack(void (*taskPointer)(void), uint * stackPointer)
{
    *(stackPointer)   = (uint)0x01000000L;       // xPSR
    *(--stackPointer) = (uint)taskPointer;       // 任务入口地址
    *(--stackPointer) = (uint)0xFFFFFFFEL;       // R14 (LR)                                                                           
    *(--stackPointer) = (uint)0x12121212L;       // R12
    *(--stackPointer) = (uint)0x03030303L;       // R3
    *(--stackPointer) = (uint)0x02020202L;       // R2
    *(--stackPointer) = (uint)0x01010101L;       // R1
    *(--stackPointer) = (uint)0x00000000L;       // R0 
    *(--stackPointer) = (uint)0x11111111L;       // R11
    *(--stackPointer) = (uint)0x10101010L;       // R10
    *(--stackPointer) = (uint)0x09090909L;       // R9
    *(--stackPointer) = (uint)0x08080808L;       // R8
    *(--stackPointer) = (uint)0x07070707L;       // R7
    *(--stackPointer) = (uint)0x06060606L;       // R6
    *(--stackPointer) = (uint)0x05050505L;       // R5
    *(--stackPointer) = (uint)0x04040404L;       // R4
    *(--stackPointer) = (uint)0xFFFFFFFDu; 											/*  听说LR也很重要  EXC_RETURN */
    *(--stackPointer) = (uint)0x20000000u; 											/*  FPSCR */		
    return(stackPointer);
}


/*******************************************************************************
* 描述	    : 初始化任务表
* 输入参数  : task: 任务表, sp:栈顶
*******************************************************************************/
static void InitTask(TaskStruct * task, uint * sp)
{
    task->StackPointer = sp;
    task->Message = 0;
    task->Delay = 0;
}


/*******************************************************************************
* 描述	    : 创建业务逻辑任务
* 输入参数  : 业务逻辑任务入口地址
*******************************************************************************/
static void CreateLogicTask(function taskPointer)
{
    uint * sp;	
    sp = InitStack(taskPointer, &LogicTaskStack[LogicTaskStackSum - 1]);
    InitTask(&LogicTask, sp);
}

/*******************************************************************************
* 描述	    : 创建菜单界面任务
* 输入参数  : 菜单界面任务入口地址
*******************************************************************************/
static void CreateScadaTask(function taskPointer)
{
    uint * sp;	
    sp = InitStack(taskPointer, &ScadaTaskStack[ScadaTaskStackSum - 1]);
    InitTask(&ScadaTask, sp);
}

/*******************************************************************************
* 描述	    : 任务切换
* 输入参数  : 切换到指定任务,激活PendSV中断实现切换
*******************************************************************************/
static void Schedule(TaskStruct *taskPointer)
{
    if (taskPointer != CurrentTaskPointer)
    {
        NextTaskPointer = taskPointer;
        SwitchContext();    //激活PendSV中断，在中断中实现任务的真正切换
    }
}

/*******************************************************************************
* 描述	    : 节拍例行，主要用于任务挂起延时作用
*******************************************************************************/
static void SystickRoutine(void)
{       
    if (LogicTask.Delay > 0) 
    {
        LogicTask.Delay--;
        if (LogicTask.Delay == 0)            
            Schedule(&LogicTask);
    }
}

/*******************************************************************************
* 描述	    : OS启动
*******************************************************************************/
static void Start(void)
{
    System.Device.Systick.Register(Systick1000, SystickRoutine); //注册OS节拍函数

    CurrentTaskPointer = &LogicTask;
    NextTaskPointer = CurrentTaskPointer;
    
    EnableIrq();
    SwitchContext();
    
    while(1);
}

/*******************************************************************************
* 描述	    : 发送一个消息到消息队列中，处于等待的任务会自动运行
*******************************************************************************/
static bool PostMessageQueue(uint message)
{	
    EnterCritical();
    // 菜单界面任务下，且无消息，用任务成员Message传递，不用消息队列
    if (CurrentTaskPointer == &ScadaTask && LogicTask.Message == 0)
    {
        LogicTask.Delay = 0;
        LogicTask.Message = message;    	
        ExitCritical();
        Schedule(&LogicTask);
        return (true);
    }

    if (MessageQueue.Entries >= QueueBufferSum)
    {                                                              
        ExitCritical();
        return (false);
    }

    MessageQueue.Entries++;
    
    *MessageQueue.In++ = message;
    if (MessageQueue.In > MessageQueue.End)
       	MessageQueue.In = MessageQueue.Start;

    ExitCritical();

    return (true);
}

/*******************************************************************************
* 描述	    : 等待消息队列，当消息队列为空时，所在任务挂起
*******************************************************************************/
static uint PendMessageQueue(void)
{
    uint message;
    EnterCritical();

    if (LogicTask.Message != 0) goto ReadMessage;
		
    if (MessageQueue.Entries > 0)
    {                    
       	message = *MessageQueue.Out++;
       	MessageQueue.Entries--;
       	if (MessageQueue.Out > MessageQueue.End) 
            MessageQueue.Out = MessageQueue.Start;
        
       	ExitCritical();
       	return (message);
    }
    LogicTask.Delay = 0;
    ExitCritical();
    
    Schedule(&ScadaTask); 
    
    EnterCritical();
ReadMessage:
    message = LogicTask.Message; 	
    LogicTask.Message = 0; 	
    ExitCritical();
    return (message);
}

/*******************************************************************************
* 描述	    : 任务挂起等待时间，单位为1个OS节拍，默认是1mS.
* 输入参数  : times，挂起节拍数，默认一个节拍为1mS，比如1000,就是1秒钟
*******************************************************************************/
static void DelayMs(int times)
{
    if (times > 0) 
    {          
       	LogicTask.Delay = times;
        
       	Schedule(&ScadaTask); 
    }
}


/*******************************************************************************
* 描述	    : 初始化用户栈为0，用于OS第一次任务切换判断用，具体看切换部分汇编函数
*******************************************************************************/
void InitPsp(uint psp)
{
    __asm(
    "MSR     PSP, R0\n"
    "BX      LR\n");
}

void InitOs(void)
{    
    MessageQueue.Start        = QueueBuffer;
    MessageQueue.End          = QueueBuffer + QueueBufferSum - 1;
    MessageQueue.In           = MessageQueue.Start;
    MessageQueue.Out          = MessageQueue.Start;
    MessageQueue.Entries      = 0;	
    
    InitPsp(0);     // PSP清零，PSP用于中断处理模式判断
    
    System.OS.CreateLogicTask = CreateLogicTask;
    System.OS.CreateScadaTask = CreateScadaTask;
    System.OS.Start = Start;
    System.OS.PendMessageQueue = PendMessageQueue;
    System.OS.PostMessageQueue = PostMessageQueue;
    System.OS.DelayMs = DelayMs;
}

