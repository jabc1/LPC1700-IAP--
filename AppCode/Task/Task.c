/******************************************************************************************************** 
*                           
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : Task.c
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
#include "../UserCode/source/config.h"   			                                // ͷ�ļ�

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/
TPC_TASK TaskComps[] = 
{
	{0, 1   , 2   , Task_KeySan       },								                // ����ɨ��
	{0, 2   , 1   , Task_HostCom      },												// ��������
	{0, 2   , 2   , Task_Display      },							                    // ��ʾ����
	{0, 2   , 2   , Task_OnlineUpgrade},							                    // ��������
};

/********************************************************************************************************
* FunctionName   : Task_GetNum()
* Description    : ��ȡ�������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void Task_GetNum(void)
{
    TPCTaskNum = (sizeof(TaskComps)/sizeof(TaskComps[0]));
}

/********************************************************************************************************
* FunctionName   : TaskInit()
* Description    : ��ʼ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TaskInit(void)
{
    AppInit();																   // �������ʼ��
    AppInitSysPrmt();														   // ��ʼ������
	Task_GetNum();                                                             // ��ȡ������          
}

/*------------------------------------------ ������ -------------------------------------------------*/

/********************************************************************************************************
* FunctionName   : Task_KeySan()
* Description    : ɨ������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void Task_KeySan(void)
{
    static u8 keyVal = 0xFF;

	if (keyVal == 0xFF)
	{
	    keyVal = KeyGetValue();								       // ��ȡ����
	    switch (keyVal)
		{
		    case APP_KV_NO: AppPrmt.DispItem = APP_DI_JAPP; break; // �ް�������ʾ��ת
			case APP_KV_MD:	AppPrmt.DispItem = APP_DI_SDUG;	break; // �м������ʾSD������
			case APP_KV_UP:
			case APP_KV_DN:
			case APP_KV_LF: 
			case APP_KV_RT:	AppPrmt.DispItem = APP_DI_SDWN; break; // ����������ʾ��������
			default: break;
		} 
	}
}

/********************************************************************************************************
* FunctionName   : Task_HostCom()
* Description    : ͨѶ
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void Task_HostCom(void) 
{
    static u16 timCount = 0;

	if ((AppPrmt.DispItem == APP_DI_SDWN) || (AppPrmt.DispItem == APP_DI_DOWN))
	{
	    if (HCTackCtrl())
		{
		    timCount = 0;
		    AppPrmt.DispItem = APP_DI_DOWN;							          // ��������ģʽ
		}
		else
		{
			if ((++timCount >= 30*10) && (AppPrmt.DispItem == APP_DI_DOWN))	  // 30s�޺��������˳�������ʾģʽ
			{
			    timCount = 0;
				HCClearAll(&HCPrmt);								          // ���
			    AppPrmt.DispItem = APP_DI_JAPP; 
				LCMClear(LCM_LINE_ALL, LCM_DIS_NML);   
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : Task_Display()
* Description    : ��ʾ����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void Task_Display(void)
{
    switch (AppPrmt.DispItem)
	{
	    case APP_DI_JAPP: AppDisJumToUser();   break;	
		case APP_DI_SDUG: AppDisSDUpgrade();   break;
		case APP_DI_SDWN: AppDisSerialDown();  break;	 
		case APP_DI_DOWN: AppDisDown(&HCPrmt); break;  
		default: break;
	}
}

/*******************************************************************************************************
* FunctionName   : Task_OnlineUpgrade()
* Description    : ��������
* EntryParameter : None
* ReturnValue    : None
*******************************************************************************************************/
void Task_OnlineUpgrade(void)
{
    u8 tmpBuf[10] = {0};
	u32 startAddr, tmpLen;	 
	
    if (AppPrmt.DispItem == APP_DI_SDUG)							   // OK��SD������
	{          
		AppSDCardProgram("AppCode.bdl");					           // (ֻ֧�ֶ��ļ���8+3)SD������
		AppPrmt.DispItem = APP_DI_JAPP; 
	}
	else
	{
		if (AppPrmt.DispItem == APP_DI_JAPP)							
		{
	        ATReadNByte(APP_UPG_ADDR, tmpBuf, 9);			           // ������������־
	
			if (tmpBuf[0] == 0x01)									   // Ƭ��Flash����
			{
				startAddr = (tmpBuf[1]<<24) + (tmpBuf[2]<<16) + (tmpBuf[3]<<8) + (tmpBuf[4]<<0);
				tmpLen    = (tmpBuf[5]<<24) + (tmpBuf[6]<<16) + (tmpBuf[7]<<8) + (tmpBuf[8]<<0);

				AppFlashProgram(startAddr, tmpLen);				       // ��������
				ATWriteNByte(APP_UPG_ADDR, 0x00, 1);			       // ȥ��������־
	        }
			else
			{
				AppExecuteUserCode();                                  // ��������
			}
		}
	}
}		  

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
