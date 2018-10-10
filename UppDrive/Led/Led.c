/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : Led.c
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
#include "../UserCode/source/config.h"    			                                          // ͷ�ļ�

/********************************************************************************************************
* FunctionName   : LEDInit()
* Description    : ��ʼ��
* EntryParameter : None
* ReturnWRITE    : None
********************************************************************************************************/
void LEDInit(void)
{
    LED_DB0_OUT();	                                               // ���ö˿ڷ���
    LED_DB1_OUT();					             
    LED_DB2_OUT();
    LED_DB3_OUT();

    LED_DB4_OUT();
    LED_DB5_OUT();
    LED_DB6_OUT();
    LED_DB7_OUT();

	// �ر�����LED	
	LEDSwitchCtrl(LED_DB0|LED_DB1|LED_DB2|LED_DB3|LED_DB4|LED_DB5|LED_DB6|LED_DB7, LED_OFF); 
}

/********************************************************************************************************
* FunctionName   : LEDSwitchCtrl()
* Description    : LED����
* EntryParameter : ledNum - LED��sw - ����(0-�رգ�1-��)
* ReturnWRITE    : None
********************************************************************************************************/
void LEDSwitchCtrl(u8 ledNum, u8 sw)
{
    if (ledNum & LED_DB0)
	{
	    LED_DB0_WRITE(sw);
	} 

    if (ledNum & LED_DB1)
	{
	    LED_DB1_WRITE(sw);
	}

    if (ledNum & LED_DB2)
	{
	    LED_DB2_WRITE(sw);
	} 
	
    if (ledNum & LED_DB3)
	{
	    LED_DB3_WRITE(sw);
	} 
	
    if (ledNum & LED_DB4)
	{
	    LED_DB4_WRITE(sw);
	} 

    if (ledNum & LED_DB5)	
	{
	    LED_DB5_WRITE(sw);
	}

    if (ledNum & LED_DB6)
	{
	    LED_DB6_WRITE(sw);
	} 
	
    if (ledNum & LED_DB7)
	{
	    LED_DB7_WRITE(sw);
	}    
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
