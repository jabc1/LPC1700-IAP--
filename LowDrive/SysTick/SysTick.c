/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : SysTick.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                  // ͷ�ļ�				

/********************************************************************************************************
* FunctionName   : SysTickInit()
* Description    : ��ʼ��SysTick
* EntryParameter : tMs - n ms
* ReturnValue    : None
********************************************************************************************************/
void SysTickInit(u32 tMs)
{ 
    SysTick_Config((SystemCoreClock/1000) * tMs);	                      // ��ʼ���������δ�ʱ���������ж�����
}

/********************************************************************************************************
* FunctionName   : SysTickClear()
* Description    : SysTick����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SysTickClear(void)
{
    SysTick->VAL = 0;
}

/********************************************************************************************************
* FunctionName   : SysTickDisable()
* Description    : ��ֹ��ʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SysTickDisable(void)
{
    SysTick->CTRL &= ~STC_ENABLE;									      // ���� ENABLE
}

/********************************************************************************************************
* FunctionName   : SysTickEnable()
* Description    : ʹ�ܶ�ʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SysTickEnable(void)
{
    SysTick->CTRL |= STC_ENABLE;									      // ��һ ENABLE
}

/********************************************************************************************************
* FunctionName   : SysTickLoad()
* Description    : ��ʱ����װֵ
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SysTickLoad(u32 tMs)
{
    SysTick->LOAD = (0xFFFFFF & ((SystemCoreClock/1000) * tMs)) - 1;     // ��ʱʱ������
}

/********************************************************************************************************
* FunctionName   : SysTickGetValue()
* Description    : ��ȡ��ʱ����ǰֵ
* EntryParameter : None
* ReturnValue    : ���ض�ʱ����ǰֵ
********************************************************************************************************/
u32 SysTickGetValue(void)
{
    return ((SysTick->VAL) & (0xFFFFFF)); 
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/