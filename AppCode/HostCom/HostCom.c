/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : HostCom.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeadeHCiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			 // 头文件

		/*------------------------------------------------------------------------------------------
		|      帧头     |  流水号  |   指令	 |	数据长度  |	    数据      |  校验码  |    帧尾     |
		--------------------------------------------------------------------------------------------
		|      0x7E  	|  SERIAL  |   CMD	 |	 LEN 2B	  |	 LEN字节数据  |	  CRC16  |     7F      |
		------------------------------------------------------------------------------------------*/

/*    说明：
		   1. 数据包以7E开头，7F结尾，中间不会有这两个数字，如果有均会被转义；
		   2. 流水号由主机产生，一般为顺序加值，从机原样返回；
		   3. 数据长度为2字节，只包含数据的长度，不包其他值；
		   4. 校验码为其之前的所有数据的CRC16。
*/

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/
HC_PRMT HCPrmt;


/********************************************************************************************************
* FunctionName   : HC_SendPackage()
* Description    : 发送数据包
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_SendPackage(HC_PRMT *prmt, u8 *pDat, u16 len)
{
    u16 tmpLen, crc;
    u8 tmpBuf[20] = {0};
	u8 sndBuf[20] = {0};

	tmpBuf[0] = HC_FRM_HEAD;
	tmpBuf[1] = prmt->Buf[1];									 // 流水号
	tmpBuf[2] = prmt->Buf[2];									 // 应答状态
	tmpBuf[3] = (u8)(len >> 8);
	tmpBuf[4] = (u8)(len & 0xFF);

	memcpy(&tmpBuf[5], pDat, len);								 // 数据

	crc = CMCrc16(tmpBuf, len+5);
	tmpBuf[len+5] = (u8)(crc >> 8);
	tmpBuf[len+6] = (u8)(crc & 0xFF);
	tmpBuf[len+7] = HC_FRM_TAIL;

	tmpLen = CMEscape(sndBuf, tmpBuf, len+8);			         // 转义

	HC_SendDat(sndBuf, tmpLen);								     // 发送数据
}

/********************************************************************************************************
* FunctionName   : HCRcvData()
* Description    : HC数据接收
* EntryParameter : dat - 数据接收
* ReturnValue    : None
********************************************************************************************************/
void HCRcvData(u8 dat)
{
    HCPrmt.Buf[HCPrmt.WriteCount] = dat;			                    // 数据存入
    if (++(HCPrmt.WriteCount) >= HC_BUF_SIZE)			            // 缓冲判断
    {
        HCPrmt.WriteCount = 0;
    }
}

/********************************************************************************************************
* FunctionName   : HC_ReadByte()
* Description    : 从接受缓冲中读取一字节数据
* EntryParameter : prmt - 参数
* ReturnValue    : 返回读取数据
********************************************************************************************************/
u8 HC_ReadByte(HC_PRMT *prmt)
{
    u8 rValue = 0x00;
    
    if (prmt->WriteCount != prmt->ReadCount)
    {
        rValue = prmt->Buf[prmt->ReadCount++];                        // 读取一字节
        if (prmt->ReadCount >= HC_BUF_SIZE)
        {
            prmt->ReadCount = 0;					                  // 清零
        }
    } 
    
    return rValue;   							                      // 返回数据
}

/********************************************************************************************************
* FunctionName   : HC_GetBufUnReadLen()
* Description    : 获取缓冲区中未读数据长度
* EntryParameter : prmt - 参数
* ReturnValue    : 返回数据长度
********************************************************************************************************/
u16 HC_GetUnReadLen(HC_PRMT *prmt)
{
    if (prmt->WriteCount >= prmt->ReadCount)
    {
        return (prmt->WriteCount - prmt->ReadCount);
    }
    else
    {
        return ((prmt->WriteCount + HC_BUF_SIZE) - prmt->ReadCount);
    }	 
}

/********************************************************************************************************
* FunctionName   : HC_GetFram()
* Description    : 获取数据帧
* EntryParameter : prmt - 参数
* ReturnValue    : 获取一针返回1，否则返回0
********************************************************************************************************/
u8 HC_GetFram(HC_PRMT *prmt)
{
    u8  tmpDat;
    u16 i, tmpLen;
	static u8 headMark = 0;
	static u16 tmpCount = 0;
	static u16 timeOut = 0;                                       // 超时计数器

	tmpLen = HC_GetUnReadLen(prmt);                               // 获取未读数据长度
	if (tmpLen > 0)
	{
	    for (i=0; i<tmpLen; i++)
		{
		    tmpDat = HC_ReadByte(prmt);       					  // 读取一字节
			if (tmpDat == HC_FRM_HEAD)							  // 帧头
			{
			    timeOut  = 0;
			    headMark = 1;                                     // 接收到头置标志
				tmpCount = 1;
			}
			else
			{
				if (headMark == 1)
				{
					 if (++tmpCount >= HC_BUF_SIZE)				 // 越界判断
					 {
					     tmpDat   = HC_EMPTY;					 // 数据溢出
					     timeOut  = 0;
					     headMark = 0;
						 tmpCount = 0;
						 HC_Clear(prmt);
						 HC_SendPackage(prmt, &tmpDat, 1);		 // 溢出应答
						 return 0;
					 }

				     if (tmpDat == HC_FRM_TAIL)					  // 帧尾判断
					 {
					     timeOut  = 0;
					     headMark = 0;
						 tmpCount = 0;
						 return 1;
					 }
				}
			}
		}
	}

    if (headMark > 0)                                             // 已经接收到帧头
    {
        if (++timeOut >= 1000)                                    // 10s超时计数
        {
		    tmpDat   = HC_CODE;									  // 没有完整帧
            timeOut  = 0;
			headMark = 0;                                         // 清标志
            tmpCount = 0;	
			HC_Clear(prmt);	
			HC_SendPackage(prmt, &tmpDat, 1);			          // 超时未收到完整帧应答      
        }
    } 

	return 0;
}

/********************************************************************************************************
* FunctionName   : HC_Clear()
* Description    : 清零参
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_Clear(HC_PRMT *prmt)
{
    memset(prmt->Buf, 0, HC_BUF_SIZE);
	prmt->WriteCount = 0;
	prmt->ReadCount  = 0;
}

/********************************************************************************************************
* FunctionName   : HCClear()
* Description    : 清零参
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HCClearAll(HC_PRMT *prmt)
{
    memset(prmt->Buf, 0, HC_BUF_SIZE);
	prmt->WriteCount = 0;
	prmt->ReadCount  = 0;

	prmt->DownMode   = 0;
	prmt->TtlPack 	 = 0;
	prmt->CurPack	 = 0;
	prmt->CodeSize	 = 0;
	prmt->StartAddr	 = 0;
}

/********************************************************************************************************
* FunctionName   : HC_GetStartAddr()
* Description    : 获取起始地址
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_GetStartAddr(HC_PRMT *prmt)
{
    u8 ackDat;

    prmt->TtlPack   = 0;
	prmt->CurPack   = 0;
	prmt->CodeSize  = 0;
	prmt->DownMode  = prmt->Buf[9];									 // 获取下载模式
	prmt->PackSize	= CM_ARR2WORD(&prmt->Buf[10]);					 // 获取包大小
    prmt->StartAddr	= CM_ARR2DWORD(&prmt->Buf[5]);					 // 获取32位地址

    if (((prmt->DownMode == HC_DWN_FNT) && (prmt->StartAddr != HC_FONT_ADDR)) ||
	    ((prmt->DownMode == HC_DWN_COD) && (prmt->StartAddr != HC_CODE_ADDR)))
	{
	    ackDat = HC_NPRMT;											 // 地址错误
	}
	else
	{
	    ackDat = (prmt->PackSize > HC_DPG_SIZE)	? HC_PACK : HC_OK;	 // 判断数据包大小，并返回应答值
	}

	HC_SendPackage(prmt, &ackDat, 1);
	HC_Clear(prmt);
	LCMClear(LCM_LINE_ALL, LCM_DIS_NML);
}

/********************************************************************************************************
* FunctionName   : HC_DataToFlash()
* Description    : 数据写入Flash
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_DataToFlash(HC_PRMT *prmt)
{
    u8  i, tmpDat[10] = {0};
	u16 tmpLen;
	u32 tmpAddr;

	tmpLen = CM_ARR2WORD(&prmt->Buf[3]);						            // 数据包大小
	prmt->TtlPack = CM_ARR2WORD(&prmt->Buf[5]);						        // 总包数
	prmt->CurPack = CM_ARR2WORD(&prmt->Buf[7]);					 	        // 当前包数

	if (prmt->TtlPack >= prmt->CurPack)
	{
		tmpAddr = prmt->StartAddr + ((prmt->CurPack-1) * prmt->PackSize);	 // 存储地址  	 
		W25QWriteNByteSecEra(tmpAddr, &prmt->Buf[9], tmpLen-4);				 // 写入Flash
		//W25QWriteNByte(tmpAddr, &prmt->Buf[9], tmpLen-4);
		if (prmt->DownMode == HC_DWN_COD)
		{
		    prmt->CodeSize += (tmpLen - 4); 						         // 接收代码计数
		}
	}

	for (i=0; i<4; i++)												         // 应答数据
	{
	    tmpDat[i] = prmt->Buf[5+i];									         // 总包数和当前包数
	}

	tmpDat[4] = HC_OK;
    HC_SendPackage(prmt, tmpDat, 5);								         // 发送应答数据
	HC_Clear(prmt);

	if ((prmt->TtlPack <= prmt->CurPack) && (prmt->DownMode == HC_DWN_COD))  // 代码完成下载
	{
	    tmpDat[0] = 0x01;													 // 升级标志

		tmpDat[1] = (prmt->StartAddr >> 24);					             // 起始地址
		tmpDat[2] = (prmt->StartAddr >> 16);
		tmpDat[3] = (prmt->StartAddr >> 8);
		tmpDat[4] = (prmt->StartAddr >> 0);

		tmpDat[5] = (prmt->CodeSize >> 24);					                 // 总的字节数
		tmpDat[6] = (prmt->CodeSize >> 16);
		tmpDat[7] = (prmt->CodeSize >> 8);
		tmpDat[8] = (prmt->CodeSize >> 0);

		ATWriteNByte(APP_UPG_ADDR, tmpDat, 9);								 // 写入EEPROM
	}
}

/********************************************************************************************************
* FunctionName   : HC_FrameProcess()
* Description    : 数据帧处理
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_FrameProcess(HC_PRMT *prmt)
{
    u8 ascDat;
    u8 tmpBuf[HC_BUF_SIZE] = {0};
	u16 tmpLen, tmpCrc;

	memcpy(tmpBuf, prmt->Buf, prmt->ReadCount);						  // 
	memset(prmt->Buf, 0, HC_BUF_SIZE);
	tmpLen = CMUnEscape(prmt->Buf, tmpBuf, prmt->ReadCount);		  // 反转义

    if ((((prmt->Buf[3] << 8) + prmt->Buf[4]) + 8) == tmpLen)
	{	
		tmpCrc = CMCrc16(prmt->Buf, tmpLen-3);					      // CRC计算
	    if (tmpCrc == (prmt->Buf[tmpLen-3]<<8) + prmt->Buf[tmpLen-2]) 
		{
		    switch (prmt->Buf[2])
			{
			    case HC_SND_ADDR: HC_GetStartAddr(prmt); break;		  // 获取起始地址
				case HC_SND_DATA: HC_DataToFlash(prmt);  break;		  // 数据写入Flash
				default: 											  // 不支持的指令
				{
				    ascDat = HC_NPC; 
					HC_SendPackage(prmt, &ascDat, 1); 
					break;
				}
			}
		} 
		else														  // 校验错误，校验码不正确
		{															  
		    ascDat = HC_CRC;
			HC_SendPackage(prmt, &ascDat, 1);						  
		}   
	}
	else															  // 指令长度异常
	{
	    ascDat = HC_LEN;
		HC_SendPackage(prmt, &ascDat, 1);    
	}
}

/********************************************************************************************************
* FunctionName   : HCInit()
* Description    : 初始化
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HCInit(void)
{
    HCClearAll(&HCPrmt);
}

/********************************************************************************************************
* FunctionName   : HCTackCtrl()
* Description    : 任务控制
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u8 HCTackCtrl(void)
{
    u8 reVal = 0;

    if (HC_GetFram(&HCPrmt))										   // 获取一帧数据
	{
	    HC_FrameProcess(&HCPrmt);									   // 处理帧
		reVal = 1;
	}

	return reVal;
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
