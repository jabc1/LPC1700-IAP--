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
* Description    : 初始化按键
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void KeyInit(void)
{
	// 设置端口为输入，默认情况就为输入方式，所以可以不设置
	KEY_SET_DIR(KEY_UP_PORT, KEY_UP_PIN);
	KEY_SET_DIR(KEY_DN_PORT, KEY_DN_PIN);
	KEY_SET_DIR(KEY_MD_PORT, KEY_MD_PIN);
	KEY_SET_DIR(KEY_LF_PORT, KEY_LF_PIN);
	KEY_SET_DIR(KEY_RT_PORT, KEY_RT_PIN);		      
}

/********************************************************************************************************
* FunctionName   : Key_DelayMs()
* Description    : 延时
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
* Description    : 获取按键状态
* EntryParameter : None
* ReturnValue    : 有键按下返回1，否则返回0
********************************************************************************************************/
u8 Key_GetStatus(u8 portNum, u8 bitPosi)
{
    u8 val = 0;

	if (KEY_READ_STATUS(portNum, bitPosi) == 0)
	{
	    Key_DelayMs(10);								   // 前沿消抖
		if (KEY_READ_STATUS(portNum, bitPosi) == 0)			   
		{
		    val = 1;						               // 获取按键值
			Key_DelayMs(10);							   // 后沿消抖			            
		}
	}  
	
	return (val);  
}

/********************************************************************************************************
* FunctionName   : KeyGetValue()
* Description    : 读取按键值
* EntryParameter : None
* ReturnValue    : 返回按键值
********************************************************************************************************/
u8 KeyGetValue(void)
{
	if (Key_GetStatus(KEY_UP_PORT, KEY_UP_PIN))		   // 读上键
	{
	    return (KEY_VAL_UP);
	}

	if (Key_GetStatus(KEY_DN_PORT, KEY_DN_PIN))		   // 读下键
	{
	    return (KEY_VAL_DN);
	}

	if (Key_GetStatus(KEY_MD_PORT, KEY_MD_PIN))		   // 读中间
	{
		return (KEY_VAL_MD);
	}

	if (Key_GetStatus(KEY_LF_PORT, KEY_LF_PIN))		   // 读左键
	{
		return (KEY_VAL_LF);
	}

	if ( Key_GetStatus(KEY_RT_PORT, KEY_RT_PIN))	   // 读右键
	{
		return (KEY_VAL_RT);
	}

	return (KEY_VAL_NO);
}

/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
