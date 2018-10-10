/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : Buzzer.c
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
#include "../UserCode/source/config.h"   			                           // ͷ�ļ�

/********************************************************************************************************
* FunctionName   : BUZInit()
* Description    : ��ʼ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void BUZInit(void)
{
	BZR_CTRL_OUT();					                                           // ���ö˿�Ϊ����˿�	
	BZR_CTRL_WRITE(BZR_OFF);			                                       // �رշ�����
}

/********************************************************************************************************
* FunctionName   : BUZ_DelayMs()
* Description    : ��ʱ
* EntryParameter : ms - ��ʱ����
* ReturnValue    : None
********************************************************************************************************/
void BUZ_DelayMs(u16 ms)
{
    u16 i;

	while (ms--)
	{
	    for (i=16663; i>0; i--)	
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : BUZCtrl()
* Description    : ���������ؿ���
* EntryParameter : sw - ���أ�0-����1-�أ�
* ReturnValue    : None
********************************************************************************************************/
void BUZCtrl(u8 sw)
{
    (sw == 0) ? BZR_CTRL_WRITE(BZR_ON) : BZR_CTRL_WRITE(BZR_OFF);
}

/********************************************************************************************************
* FunctionName   : BUZSound()
* Description    : ����������
* EntryParameter : times - ����������onTime - ����������ʱ�䣬offTime - �ط�������ʱ��
* ReturnValue    : None
********************************************************************************************************/
void BUZSound(u8 times, u16 onTime, u16 offTime)
{
    while (times--)
	{
	    BZR_CTRL_WRITE(BZR_ON);			                            // ��
		BUZ_DelayMs(onTime);

		BZR_CTRL_WRITE(BZR_OFF);			                        // ��
		BUZ_DelayMs(offTime);
	} 
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/