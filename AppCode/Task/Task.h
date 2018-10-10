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
* �ڲ�����                            
********************************************************************************************************/
static void Task_GetNum(void);												  // ��ȡ������

static void Task_KeySan(void);							                      // ����ɨ��
static void Task_Display(void);							                      // ��ʾ
static void Task_HostCom(void);												  // ͨѶ
static void Task_OnlineUpgrade(void);					                      // ��������

/********************************************************************************************************
* ȫ�ֺ���                            
********************************************************************************************************/
extern void TaskInit(void);													  // ��ʼ��

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/