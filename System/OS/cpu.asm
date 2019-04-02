/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名   : cpu.asm
*作用     : 系统应用文件,针对IAR对汇编与keil的支持不同，在任务切换部分，直接使用
*           asm进行任务内核切换
*创建时间 : 2016.9.12
********************************************************************************
*/    
//    PUBLIC  InitPsp
    PUBLIC  PendSV_Handler


;PAGE
;********************************************************************************************************
;                                               EQUATES
;********************************************************************************************************

NVIC_INT_CTRL   EQU     0xE000ED04                              ; Interrupt control state register.
NVIC_SYSPRI14   EQU     0xE000ED22                              ; System priority register (priority 14).
NVIC_PENDSV_PRI EQU     0xFF                                    ; PendSV priority value (lowest).
NVIC_PENDSVSET  EQU     0x10000000                              ; Value to trigger PendSV exception.


;********************************************************************************************************
;                                     CODE GENERATION DIRECTIVES
;********************************************************************************************************

    RSEG CODE:CODE:NOROOT(2)
    THUMB
    

;********************************************************************************************************
;                                   CRITICAL SECTION METHOD 3 FUNCTIONS
;
; Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
;              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
;              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
;              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
;              into the CPU's status register.
;
; Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
;                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
;
;
; Note(s)    : 1) These functions are used in general like this:
;
;                 void Task (void *p_arg)
;                 {
;                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
;                     OS_CPU_SR  cpu_sr;
;                 #endif
;
;                          :
;                          :
;                     OS_ENTER_CRITICAL();             /* cpu_sr = OS_CPU_SaveSR();                */
;                          :
;                          :
;                     OS_EXIT_CRITICAL();              /* OS_CPU_RestoreSR(cpu_sr);                */
;                          :
;                          :
;                 }
;********************************************************************************************************
OS_CPU_SR_Save
    MRS     R0, PRIMASK                                         ; Set prio int mask to mask all (except faults)
    CPSID   I
    BX      LR

OS_CPU_SR_Restore
    MSR     PRIMASK, R0
    BX      LR




/*******************************************************************************
* 描述	    : 任务切换，在PendSV中断中实现
*******************************************************************************/
PendSV_Handler
  IMPORT  CurrentTaskPointer
  IMPORT  NextTaskPointer

    CPSID    I                          // 关闭中断
    MRS      R0, PSP                    // 读取PSP到R0   
    CBZ      R0, NextTask               // 复位后，任务启动时，从中断模式跳转到任务中	
    TST      LR,#0x10
    IT       EQ
    VSTMDBEQ R0!, {S0-S31}
    MOV R3,  LR
    VMRS     R2, FPSCR
    STMDB    R0!,{R2-R11}	 
    LDR      R2, =CurrentTaskPointer     // 获取指向当前任务指针的地址
    LDR      R1, [R2]                    // 获取指向当前任务的指针，也指向第一成员StackPointer
    STR      R0, [R1]                    // 把当前栈顶存入表中第一个成员, 任务的第一个值就是栈顶的值	
		
NextTask
    LDR      R3, =NextTaskPointer        // 获取指向新任务指针的地址
    LDR      R0, [R3]                    // 获取新任务指针变量值
    STR      R0, [R2]                    // 赋值给当前任务指针变量
    LDR      R0, [R0]  
    LDMIA    R0!,{R2-R11}
    MOV      LR,R3;
    VMSR     FPSCR, R2	
    TST      LR,#0X10
    IT       EQ
    VLDMIAEQ R0!,{S0-S31}
    MSR      PSP, R0                     // 新栈顶写入PSP
    ORR      LR, LR, #0x04               // LR的bit2写入1，中断退出后进入任务模式，使用PSP栈    //进入线程模式，也就是 CONTROL[1] 因为此时是特权模式下，所以才可以写入
    CPSIE    I                           // 开中断
    BX       LR                          // 返回
    NOP                                      
	

    END
