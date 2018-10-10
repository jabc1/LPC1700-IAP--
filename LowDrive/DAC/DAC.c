/******************************************************************************************************** 
*                                 
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : DAC.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23              JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                        // ͷ�ļ�

/********************************************************************************************************
* Variable Definition                            
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : DACInit()							   
* Description    : ��ʼ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void DACInit(void)
{
    GPIOSetMode(DAC_IO_PORT, DAC_IO_PIN, DAC_IO_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);			
}

/********************************************************************************************************
* FunctionName   : DACConvert()							   
* Description    : DACת��
* EntryParameter : speed - ת���ٶȣ�0-1MHz��1-400KHz����mv - ��Ҫת����ģ���źŵ����֣���λΪmv
* ReturnValue    : None
********************************************************************************************************/
void DACConvert(u8 speed, u16 mv)
{
	LPC_DAC->DACR = (speed << DAC_BIAS) | (((mv*1024) / DAC_VREF) << DAC_VALUE);
}												 

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/