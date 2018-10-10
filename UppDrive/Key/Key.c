/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : KEY.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* header file                                                 
********************************************************************************************************/
#include "../UserCode/source/config.h"

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
			
/********************************************************************************************************
* FunctionName   : KeyInit()
* Description    : ��ʼ������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void KeyInit(void)
{
	// ���ö˿�Ϊ���룬Ĭ�������Ϊ���뷽ʽ�����Կ��Բ�����
	KEY_SET_DIR(KEY_UP_PORT, KEY_UP_PIN);
	KEY_SET_DIR(KEY_DN_PORT, KEY_DN_PIN);
	KEY_SET_DIR(KEY_MD_PORT, KEY_MD_PIN);
	KEY_SET_DIR(KEY_LF_PORT, KEY_LF_PIN);
	KEY_SET_DIR(KEY_RT_PORT, KEY_RT_PIN);		      
}

/********************************************************************************************************
* FunctionName   : Key_DelayMs()
* Description    : ��ʱ
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void Key_DelayMs(u8 t)
{
    u16 i;

	while (t--)
	{
	    for (i=16663; i>0; i--)	 
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : Key_GetStatus()
* Description    : ��ȡ����״̬
* EntryParameter : None
* ReturnValue    : �м����·���1�����򷵻�0
********************************************************************************************************/
u8 Key_GetStatus(u8 portNum, u8 bitPosi)
{
    u8 val = 0;

	if (KEY_READ_STATUS(portNum, bitPosi) == 0)
	{
	    Key_DelayMs(10);								   // ǰ������
		if (KEY_READ_STATUS(portNum, bitPosi) == 0)			   
		{
		    val = 1;						               // ��ȡ����ֵ
			Key_DelayMs(10);							   // ��������			            
		}
	}  
	
	return (val);  
}

/********************************************************************************************************
* FunctionName   : KeyGetValue()
* Description    : ��ȡ����ֵ
* EntryParameter : None
* ReturnValue    : ���ذ���ֵ
********************************************************************************************************/
u8 KeyGetValue(void)
{
	if (Key_GetStatus(KEY_UP_PORT, KEY_UP_PIN))		   // ���ϼ�
	{
	    return (KEY_VAL_UP);
	}

	if (Key_GetStatus(KEY_DN_PORT, KEY_DN_PIN))		   // ���¼�
	{
	    return (KEY_VAL_DN);
	}

	if (Key_GetStatus(KEY_MD_PORT, KEY_MD_PIN))		   // ���м�
	{
		return (KEY_VAL_MD);
	}

	if (Key_GetStatus(KEY_LF_PORT, KEY_LF_PIN))		   // �����
	{
		return (KEY_VAL_LF);
	}

	if ( Key_GetStatus(KEY_RT_PORT, KEY_RT_PIN))	   // ���Ҽ�
	{
		return (KEY_VAL_RT);
	}

	return (KEY_VAL_NO);
}

/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
