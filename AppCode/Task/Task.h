/********************************************************************************************************    
*                                                                      
*                                     Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : Task.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef  __TASK__H__
#define  __TASK__H__	 

/********************************************************************************************************
* Variable definition                                               
********************************************************************************************************/			   	  
extern TPC_TASK TaskComps[];

/********************************************************************************************************
* 内部函数                            
********************************************************************************************************/
static void Task_GetNum(void);												  // 获取任务数

static void Task_KeySan(void);							                      // 按键扫描
static void Task_Display(void);							                      // 显示
static void Task_HostCom(void);												  // 通讯
static void Task_OnlineUpgrade(void);					                      // 在线升级

/********************************************************************************************************
* 全局函数                            
********************************************************************************************************/
extern void TaskInit(void);													  // 初始化

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
