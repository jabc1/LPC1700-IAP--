/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : rit.c
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
#include "../UserCode/source/config.h"  			                          // ͷ�ļ�

/********************************************************************************************************
* FunctionName   : RITInit()
* Description    : ��ʼ������
* EntryParameter : nMs - n ms
* ReturnValue    : None
********************************************************************************************************/
void RITInit(u32 nMs)
{
    u8 ratio = 4;									                          // ����ʱ�����ں�ʱ�ӵı���

	LPC_SC->PCONP |= (1 << 16);						                          // ��RIT��Դ

	LPC_RIT->RICOMPVAL = ((SystemCoreClock/ratio)/1000) * nMs;                // ���ñȽϼĴ���
	LPC_RIT->RIMASK	   = 0x00000000;				                          // ���μĴ���
	LPC_RIT->RICTRL	   = RIT_INT | RIT_ENCLR | RIT_EN;	                      // ���ƼĴ���
	LPC_RIT->RICOUNTER = 0x00000000;			                              // 32λ������

	NVIC_EnableIRQ(RIT_IRQn);		                                          // Enable the TIMER0 Interrupt
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/