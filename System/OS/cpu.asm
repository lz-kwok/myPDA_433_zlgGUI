/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���   : cpu.asm
*����     : ϵͳӦ���ļ�,���IAR�Ի����keil��֧�ֲ�ͬ���������л����֣�ֱ��ʹ��
*           asm���������ں��л�
*����ʱ�� : 2016.9.12
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
* ����	    : �����л�����PendSV�ж���ʵ��
*******************************************************************************/
PendSV_Handler
  IMPORT  CurrentTaskPointer
  IMPORT  NextTaskPointer

    CPSID    I                          // �ر��ж�
    MRS      R0, PSP                    // ��ȡPSP��R0   
    CBZ      R0, NextTask               // ��λ����������ʱ�����ж�ģʽ��ת��������	
    TST      LR,#0x10
    IT       EQ
    VSTMDBEQ R0!, {S0-S31}
    MOV R3,  LR
    VMRS     R2, FPSCR
    STMDB    R0!,{R2-R11}	 
    LDR      R2, =CurrentTaskPointer     // ��ȡָ��ǰ����ָ��ĵ�ַ
    LDR      R1, [R2]                    // ��ȡָ��ǰ�����ָ�룬Ҳָ���һ��ԱStackPointer
    STR      R0, [R1]                    // �ѵ�ǰջ��������е�һ����Ա, ����ĵ�һ��ֵ����ջ����ֵ	
		
NextTask
    LDR      R3, =NextTaskPointer        // ��ȡָ��������ָ��ĵ�ַ
    LDR      R0, [R3]                    // ��ȡ������ָ�����ֵ
    STR      R0, [R2]                    // ��ֵ����ǰ����ָ�����
    LDR      R0, [R0]  
    LDMIA    R0!,{R2-R11}
    MOV      LR,R3;
    VMSR     FPSCR, R2	
    TST      LR,#0X10
    IT       EQ
    VLDMIAEQ R0!,{S0-S31}
    MSR      PSP, R0                     // ��ջ��д��PSP
    ORR      LR, LR, #0x04               // LR��bit2д��1���ж��˳����������ģʽ��ʹ��PSPջ    //�����߳�ģʽ��Ҳ���� CONTROL[1] ��Ϊ��ʱ����Ȩģʽ�£����Բſ���д��
    CPSIE    I                           // ���ж�
    BX       LR                          // ����
    NOP                                      
	

    END
