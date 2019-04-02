/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���   : os.c
*����ʱ�� : 2016.12.7
*******************************************************************************/
#include "system.h"

uint16_t CriticalNesting = 0;


typedef struct                             // �����������
{
    uint    *   Start;                      // ָ����п�ʼ
    uint    *   End;                        // ָ����н���
    uint    *   In;                         // ����һ����Ϣ
    uint    *   Out;                        // ȡ��һ����Ϣ
    ushort      Entries;                    // ��Ϣ����
} QueueStruct;

typedef struct 
{
    uint *          StackPointer;           // ջ��ָ��
    uint            Message;                // ��Ϣ
    uint            Delay;                  // ������ʱ������
} TaskStruct;

#define LogicTaskStackSum   800             // ҵ���߼�����ջ���
#define ScadaTaskStackSum   800             // �˵���������ջ���
#define QueueBufferSum      400             // ��Ϣ�������

#define PendSVRegister *((volatile  uint *)0xE000ED04)
#define PendSVSet 0x10000000

#define SwitchContext() PendSVRegister = PendSVSet

static QueueStruct MessageQueue;
static uint QueueBuffer[QueueBufferSum];         // ҵ���߼���Ϣ����

#pragma data_alignment = 8 
static uint LogicTaskStack[LogicTaskStackSum];   //ҵ���߼�����ջ����
#pragma data_alignment = 8 
static uint ScadaTaskStack[ScadaTaskStackSum];     //�˵���������ջ����

static TaskStruct LogicTask;
static TaskStruct ScadaTask;

TaskStruct * CurrentTaskPointer;
TaskStruct * NextTaskPointer;

/*******************************************************************************
* ����	    : ��ʼ������ջ��xPSR��PC��R14��R12��R3��R2��R1��R0��ջ˳������Cortex
*           : оƬ�����ġ��ж���ջ��оƬӲ���Զ���ɣ�R11~R4˳������OS�Լ������ġ�
* �������  : taskPointer: ������ڵ�ַ, stackRamTopPointer: ջ�ڴ���ߵ�ַ
* ���ز���  : ջ��
*******************************************************************************/
//������� Ϊ��Ӧ����FPU 
static uint * InitStack(void (*taskPointer)(void), uint * stackPointer)
{
    *(stackPointer)   = (uint)0x01000000L;       // xPSR
    *(--stackPointer) = (uint)taskPointer;       // ������ڵ�ַ
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
    *(--stackPointer) = (uint)0xFFFFFFFDu; 											/*  ��˵LRҲ����Ҫ  EXC_RETURN */
    *(--stackPointer) = (uint)0x20000000u; 											/*  FPSCR */		
    return(stackPointer);
}


/*******************************************************************************
* ����	    : ��ʼ�������
* �������  : task: �����, sp:ջ��
*******************************************************************************/
static void InitTask(TaskStruct * task, uint * sp)
{
    task->StackPointer = sp;
    task->Message = 0;
    task->Delay = 0;
}


/*******************************************************************************
* ����	    : ����ҵ���߼�����
* �������  : ҵ���߼�������ڵ�ַ
*******************************************************************************/
static void CreateLogicTask(function taskPointer)
{
    uint * sp;	
    sp = InitStack(taskPointer, &LogicTaskStack[LogicTaskStackSum - 1]);
    InitTask(&LogicTask, sp);
}

/*******************************************************************************
* ����	    : �����˵���������
* �������  : �˵�����������ڵ�ַ
*******************************************************************************/
static void CreateScadaTask(function taskPointer)
{
    uint * sp;	
    sp = InitStack(taskPointer, &ScadaTaskStack[ScadaTaskStackSum - 1]);
    InitTask(&ScadaTask, sp);
}

/*******************************************************************************
* ����	    : �����л�
* �������  : �л���ָ������,����PendSV�ж�ʵ���л�
*******************************************************************************/
static void Schedule(TaskStruct *taskPointer)
{
    if (taskPointer != CurrentTaskPointer)
    {
        NextTaskPointer = taskPointer;
        SwitchContext();    //����PendSV�жϣ����ж���ʵ������������л�
    }
}

/*******************************************************************************
* ����	    : �������У���Ҫ�������������ʱ����
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
* ����	    : OS����
*******************************************************************************/
static void Start(void)
{
    System.Device.Systick.Register(Systick1000, SystickRoutine); //ע��OS���ĺ���

    CurrentTaskPointer = &LogicTask;
    NextTaskPointer = CurrentTaskPointer;
    
    EnableIrq();
    SwitchContext();
    
    while(1);
}

/*******************************************************************************
* ����	    : ����һ����Ϣ����Ϣ�����У����ڵȴ���������Զ�����
*******************************************************************************/
static bool PostMessageQueue(uint message)
{	
    EnterCritical();
    // �˵����������£�������Ϣ���������ԱMessage���ݣ�������Ϣ����
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
* ����	    : �ȴ���Ϣ���У�����Ϣ����Ϊ��ʱ�������������
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
* ����	    : �������ȴ�ʱ�䣬��λΪ1��OS���ģ�Ĭ����1mS.
* �������  : times�������������Ĭ��һ������Ϊ1mS������1000,����1����
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
* ����	    : ��ʼ���û�ջΪ0������OS��һ�������л��ж��ã����忴�л����ֻ�ຯ��
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
    
    InitPsp(0);     // PSP���㣬PSP�����жϴ���ģʽ�ж�
    
    System.OS.CreateLogicTask = CreateLogicTask;
    System.OS.CreateScadaTask = CreateScadaTask;
    System.OS.Start = Start;
    System.OS.PendMessageQueue = PendMessageQueue;
    System.OS.PostMessageQueue = PostMessageQueue;
    System.OS.DelayMs = DelayMs;
}

