/******************************************************************************************************** 
*                                 LPC1700 工程模块
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : .h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23              JLINK V8(SWD)
* Description    : 
*				   Time-chip processing cores
*
********************************************************************************************************/

#ifndef  __TPC__H__
#define  __TPC__H__

/********************************************************************************************************
* Variable definition                                               
********************************************************************************************************/			   	  
typedef struct _TPC_TASK
{
    u8   Run;									                  // 程序运行标记：0-不运行，1运行
	u16  Timer; 									              // 计时器
	u16  ItvTime;								                  // 任务运行间隔时间
	void (*Task)(void);							                  // 要运行的任务函数

} TPC_TASK;									                      // 任务定义

//-------------------------------------------------------------------------------------------------------
typedef enum
{
    TPC_RUN_CLM = 0x00,											  // 停止
	TPC_RUN_STM = 0x01,											  // 运行

} TPC_RUN_MARK;	

//-------------------------------------------------------------------------------------------------------
extern u8 TPCTaskNum;                                             // 任务数                   

/********************************************************************************************************
* Global function                           
********************************************************************************************************/
extern void TPCRemarks(TPC_TASK *pTask);						  // 标志处理(节拍中断函数中调用)
extern void TPCProcess(TPC_TASK *pTask);						  // 任务处理(主函数中调用)

/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
