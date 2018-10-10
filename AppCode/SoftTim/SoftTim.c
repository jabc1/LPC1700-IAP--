/******************************************************************************************************** 
*                                 LPC1700 工程模块
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : SoftTim.c
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
#include "../UserCode/source/config.h"    			                           // 头文件

/********************************************************************************************************
* Variable Definition                            
********************************************************************************************************/
static SFT_TABLE SFT_Tlb[] =							                       // 定时器
{
    {SFT_OPEN  , 500  , 500  , SFT_SecFlash    },					           // 秒闪烁
	{SFT_CLOSE , 30000, 30000, SFT_BlkCls      },					           // 关闭背光
	{SFT_CLOSE , 30000, 30000, SFT_ExitMenu    },						       // 退出菜单
};

/********************************************************************************************************
* FunctionName   : SFT_SecFlash()							   
* Description    : 秒闪烁
* EntryParameter : num - 在定时器中的序号
* ReturnValue    : None
********************************************************************************************************/
void SFT_SecFlash(u8 num)
{
    SFT_Tlb[num].State = SFT_OPEN;										       // 重新打开定时器
    AppPrmt.SecFls = !AppPrmt.SecFls;										       // 标志取反
}

/********************************************************************************************************
* FunctionName   : SFT_BlkCls()							   
* Description    : 背光关闭
* EntryParameter : num - 在定时器中的序号
* ReturnValue    : None
********************************************************************************************************/
void SFT_BlkCls(u8 num)
{
    LCMBacklightCtrl(LCM_BLK_OFF);									           // 关闭背光
}

/********************************************************************************************************
* FunctionName   : SFT_ExitMenu()							   
* Description    : 退出菜单操作
* EntryParameter : num - 在定时器中的序号
* ReturnValue    : None
********************************************************************************************************/
void SFT_ExitMenu(u8 num)
{
    AppPrmt.ExitMenu = TRUE;											           // 退出菜单
	AppPrmt.DispItem = APP_DI_CINT;
}

/********************************************************************************************************
* FunctionName   : SFTInit()							   
* Description    : 初始软件定时器
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTInit(u8 timNum, u32 timVlu)
{
   TIM_SET timSet;

   timSet.Mode       = TIM_TIMER;										        // 定时器模式
   timSet.IntClear   = TIM_MR0;											        // MR0
   timSet.InitValue  = 0;												        // 定时器初始值为0
   timSet.Prescaler  = 0;												        // 定时器时钟分频为0

   timSet.MatchSet   = TIM_MR0I|TIM_MR0R;								        // MR与TC匹配时产生中断
   timSet.MatchOut	 = 0;												        // 不产生匹配输出
   timSet.TimValue   = timVlu;											        // 定时器定时值，单位：us

   timSet.CapCtrl	 = 0;												        // 捕获控制，没有使用，清零
   timSet.CaptureSet = 0;												        // 捕获设置

   TIM32Init(timNum, &timSet);											        // 初始定时器
}

/********************************************************************************************************
* FunctionName   : SFTIrq()							   
* Description    : 定时器0中断服务
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTIrq(void)
{
    u8 i;

	TIM32ClearInt(TIM0, TIM_MR0);

	for (i=0; i<(sizeof(SFT_Tlb)/sizeof(SFT_Tlb[0])); i++)					   // 循环判断
	{
	    if (SFT_Tlb[i].State == SFT_OPEN)
		{
			if (SFT_Tlb[i].CountValue)					                       // 不为零
			{
			    SFT_Tlb[i].CountValue--;
				if (SFT_Tlb[i].CountValue == 0)			                       // 定时时间到
				{
					SFT_Tlb[i].CountValue = SFT_Tlb[i].InitValue;
					SFT_Tlb[i].State = SFT_CLOSE;			                   // 标志清除
					SFT_Tlb[i].Fun(i);				                           // 运行任务
				}
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : SFTSart()							   
* Description    : 启动定时器
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTSart(u8 num)
{
	SFT_Tlb[num].CountValue = SFT_Tlb[num].InitValue;						// 赋值
    SFT_Tlb[num].State = SFT_OPEN;											// 开定时器
}

/********************************************************************************************************
* FunctionName   : SFTStop()							   
* Description    : 停止定时器
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTStop(u8 num)
{
    SFT_Tlb[num].State = SFT_CLOSE;
}

/********************************************************************************************************
* FunctionName   : SFTSartBlk()							   
* Description    : 启动背光控制定时器
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTSartBlk(void)
{
    u8 blk = (AppSet.BlkTim>>4) * 10 + (AppSet.BlkTim & 0x0F);

	LCMBacklightCtrl(LCM_BLK_ON);										         // 打开背光

	if (blk > 0)														         // 为0常亮，不用自动关闭
	{
		SFT_Tlb[SFT_BLK].InitValue = blk * 1000;					             // 背光关闭时间
		SFTSart(SFT_BLK);
	}													    
}												 

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
