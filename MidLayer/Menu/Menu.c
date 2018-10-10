/********************************************************************************************************
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : Mnu.c
* Version        : V1.0
* Programmer(s)  : 
* Parameters     : ARM LPC1700  12MHz
* DebugTools     : JLINK V8.0 And Realview MDK V4.22
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                       // 头文件

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : MnuPrmtInit()
* Description    : 初始化菜单参数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void MnuPrmtInit(Mnu_PRMT *prmt)
{
    prmt->Cursor = 0;										                  // 光标清零
	prmt->PageNo = 0;										                  // 页清零
	prmt->Index  = 0;										                  // 索引清零
}

/********************************************************************************************************
* FunctionName   : MnuSetPrmt()
* Description    : 设置菜单显示参数
* EntryParameter : num - 页菜单显示项，page - 最多页数
* ReturnValue    : None
********************************************************************************************************/
void MnuSetPrmt(Mnu_PRMT *prmt, u8 num, u8 page)
{
	prmt->DispNum = num;									                   // 页最多显示项目数
	prmt->MaxPage = page;									                   // 最多页数
}

/********************************************************************************************************
* FunctionName   : MnuDisplay()
* Description    : 显示菜单
* EntryParameter : page - 显示页，dispNum - 每一页的显示项，cursor - 光标位置
* ReturnValue    : None
********************************************************************************************************/
void MnuDisplay(Mnu_TABLE *mnu, u8 page, u8 dispNum, u8 cursor)
{
    u8 i;

	for (i=0; i<dispNum; i++)
	{
	    if (cursor == i)
		{		   	      
			MNU_DISP_RVS(0, i+1, mnu[page+i].MnuItem);         				 // 反白显示菜单项
		}
		else
		{	             
			MNU_DISP_NML(0, i+1, mnu[page+i].MnuItem);						 // 显示菜单项
		}
	}
}

/********************************************************************************************************
* FunctionName   : MnuMove()
* Description    : 菜单移动
* EntryParameter : prmt - 菜单参数, key - 按键值
* ReturnValue    : 执行菜单返回TRUE，移动菜单FALSE
********************************************************************************************************/
u8 MnuMove(Mnu_PRMT *prmt, u8 key)
{
    u8 rValue = FALSE;

    switch (key) 
	{
	    case MNU_KV_UP:													 // 向上
	    { 
	        if (prmt->Cursor != 0)						                 // 光标不在顶端
			{
			    prmt->Cursor--;							                 // 光标上移
			}														 
			else											             // 光标在顶端
			{
			    if (prmt->PageNo != 0)					                 // 页面没有到最小
				{
				    prmt->PageNo--;						                 // 向上翻
				}
				else
				{
				    prmt->Cursor = prmt->DispNum-1;	                     // 光标到底
					prmt->PageNo = prmt->MaxPage-1;	                     // 最后页
				}
			}
			break;
		}

		case MNU_KV_DN: 												 // 向下
	    {
			if (prmt->Cursor < prmt->DispNum-1)                          // 光标没有到底，移动光标
			{
			    prmt->Cursor++;							                 // 光标向下移动
			}
			else                                                         // 光标到底
			{
			    if (prmt->PageNo < prmt->MaxPage-1)                      // 页面没有到底，页面移动
			    {
			        prmt->PageNo++;						                 // 下翻一页
			    }
			    else                                                     // 页面和光标都到底，返回开始页
			    {
			        prmt->Cursor = 0;
			        prmt->PageNo = 0;
			    }
			}
			break;
		}

		case MNU_KV_MD:													 // 确认
	    { 
	        prmt->Index = prmt->Cursor + prmt->PageNo;                   // 计算执行项的索引
			rValue = TRUE;
	        break;
		}

		case MNU_KV_LF:												     // 左键跳到顶部
	    {
		    prmt->Cursor = 0;
			prmt->PageNo = 0;
			break;
		}

		case MNU_KV_RT:													 // 右键跳到底部
	    {
			prmt->Cursor = prmt->DispNum-1;	                             // 光标到底
			prmt->PageNo = prmt->MaxPage-1;	                             // 最后页
			break;
		}

		default:break;
	} 

	return rValue; 												         // 返回执行索引
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
