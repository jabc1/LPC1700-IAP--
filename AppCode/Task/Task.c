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
#include "../UserCode/source/config.h"   			                                // 头文件

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/
TPC_TASK TaskComps[] = 
{
	{0, 1   , 2   , Task_KeySan       },								                // 按键扫描
	{0, 2   , 1   , Task_HostCom      },												// 串口下载
	{0, 2   , 2   , Task_Display      },							                    // 显示任务
	{0, 2   , 2   , Task_OnlineUpgrade},							                    // 在线升级
};

/********************************************************************************************************
* FunctionName   : Task_GetNum()
* Description    : 获取任务个数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void Task_GetNum(void)
{
    TPCTaskNum = (sizeof(TaskComps)/sizeof(TaskComps[0]));
}

/********************************************************************************************************
* FunctionName   : TaskInit()
* Description    : 初始化
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TaskInit(void)
{
    AppInit();																   // 内外设初始化
    AppInitSysPrmt();														   // 初始化参数
	Task_GetNum();                                                             // 获取任务数          
}

/*------------------------------------------ 任务处理 -------------------------------------------------*/

/********************************************************************************************************
* FunctionName   : Task_KeySan()
* Description    : 扫描任务
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void Task_KeySan(void)
{
    static u8 keyVal = 0xFF;

	if (keyVal == 0xFF)
	{
	    keyVal = KeyGetValue();								       // 获取按键
	    switch (keyVal)
		{
		    case APP_KV_NO: AppPrmt.DispItem = APP_DI_JAPP; break; // 无按键，显示跳转
			case APP_KV_MD:	AppPrmt.DispItem = APP_DI_SDUG;	break; // 中间键，显示SD卡升级
			case APP_KV_UP:
			case APP_KV_DN:
			case APP_KV_LF: 
			case APP_KV_RT:	AppPrmt.DispItem = APP_DI_SDWN; break; // 其他键，显示串口下载
			default: break;
		} 
	}
}

/********************************************************************************************************
* FunctionName   : Task_HostCom()
* Description    : 通讯
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
		    AppPrmt.DispItem = APP_DI_DOWN;							          // 进入下载模式
		}
		else
		{
			if ((++timCount >= 30*10) && (AppPrmt.DispItem == APP_DI_DOWN))	  // 30s无后续数据退出下载显示模式
			{
			    timCount = 0;
				HCClearAll(&HCPrmt);								          // 清除
			    AppPrmt.DispItem = APP_DI_JAPP; 
				LCMClear(LCM_LINE_ALL, LCM_DIS_NML);   
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : Task_Display()
* Description    : 显示任务
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
* Description    : 在线升级
* EntryParameter : None
* ReturnValue    : None
*******************************************************************************************************/
void Task_OnlineUpgrade(void)
{
    u8 tmpBuf[10] = {0};
	u32 startAddr, tmpLen;	 
	
    if (AppPrmt.DispItem == APP_DI_SDUG)							   // OK键SD卡升级
	{          
		AppSDCardProgram("AppCode.bdl");					           // (只支持短文件名8+3)SD卡升级
		AppPrmt.DispItem = APP_DI_JAPP; 
	}
	else
	{
		if (AppPrmt.DispItem == APP_DI_JAPP)							
		{
	        ATReadNByte(APP_UPG_ADDR, tmpBuf, 9);			           // 读在线升级标志
	
			if (tmpBuf[0] == 0x01)									   // 片外Flash升级
			{
				startAddr = (tmpBuf[1]<<24) + (tmpBuf[2]<<16) + (tmpBuf[3]<<8) + (tmpBuf[4]<<0);
				tmpLen    = (tmpBuf[5]<<24) + (tmpBuf[6]<<16) + (tmpBuf[7]<<8) + (tmpBuf[8]<<0);

				AppFlashProgram(startAddr, tmpLen);				       // 在线升级
				ATWriteNByte(APP_UPG_ADDR, 0x00, 1);			       // 去除升级标志
	        }
			else
			{
				AppExecuteUserCode();                                  // 启动程序
			}
		}
	}
}		  

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
