/********************************************************************************************************    
*                                                                        
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ili9281.c
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V3.70
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
*                                  HeaderFiles                                                  
********************************************************************************************************/
#include "../UserCode/source/config.h"    

/********************************************************************************************************
* FunctionName   : TFT_DelayUs()
* Description    : TFT延时
* EntryParameter : us - us
* ReturnValue    : None
********************************************************************************************************/
void TFT_DelayUs(u32 us) 
{
    u16 i;

	while (us--)
	{
	    for (i=14; i>0; i--)	 
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFT_SetGpio()
* Description    : TFT端口方向设置
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TFT_SetGpio(void)
{
    GPIOSetMultiDir(TFT_DAT_L_PORT, TFT_DAT_L, GPIO_OUT);					 // 数据端口方向设置
	GPIOSetMultiDir(TFT_DAT_H_PORT, TFT_DAT_H, GPIO_OUT);

	GPIOSetDir(TFT_CS_PORT, TFT_CS_PIN, GPIO_OUT);			                 //
	GPIOSetDir(TFT_RS_PORT, TFT_RS_PIN, GPIO_OUT);			                 //  
	GPIOSetDir(TFT_WR_PORT, TFT_WR_PIN, GPIO_OUT);			                 // 
	GPIOSetDir(TFT_RD_PORT, TFT_RD_PIN, GPIO_OUT);			                 // 
	GPIOSetDir(TFT_RT_PORT, TFT_RT_PIN, GPIO_OUT);			                 // 复位
	GPIOSetDir(TFT_EN_PORT, TFT_EN_PIN, GPIO_OUT);			                 // 背光使能

	// 所以管脚置1
	TFTWriteData(0xFFFF);

	GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_SET);
	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_SET);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
	GPIOSetValue(TFT_RS_PORT, TFT_RS_PIN, GPIO_SET);
	GPIOSetValue(TFT_RD_PORT, TFT_RD_PIN, GPIO_SET);
	GPIOSetValue(TFT_EN_PORT, TFT_EN_PIN, GPIO_SET);

	TFTBacklightCtrl(TFT_BLK_OFF);										     // 背光关 
}

/********************************************************************************************************
* FunctionName   : TFT_Reset()
* Description    : TFT复位,	注意复位时间
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TFT_Reset(void)
{
/*----------------------------------- 时序图 -----------------------------------------
--                                                                                  --
--                  ------------\_____________________/-----------                  --
--                              | <----  Tres  ---->  |                             --
--                                                                                  --
--                             Tres: Min.1ms                                        --
------------------------------------------------------------------------------------*/
    GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_SET);					   // 可以不要
	GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_CLR);					   // 保持低电平1ms复位TFT
	TFT_DelayUs(1000);
	GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_SET);	  				   // 拉高撤销复位
}

/********************************************************************************************************
* FunctionName   : TFTBacklightCtrl()
* Description    : 背光开关
* EntryParameter : blk - 1开，0关
* ReturnValue    : None
********************************************************************************************************/
void TFTBacklightCtrl(u8 blk)
{
    GPIOSetValue(TFT_EN_PORT, TFT_EN_PIN, blk);
}

/********************************************************************************************************
* FunctionName   : TFTInit()
* Description    : 初始化TFT
* EntryParameter : color - 初始化颜色
* ReturnValue    : None
********************************************************************************************************/
void TFTInit(u16 color)
{
    TFT_SetGpio();					                     // 初始化控制端口											    
	TFT_Reset();									     // 复位液晶模块

	TFT_WriteIndex(EXT_SLP_MODE);						 // Exit_sleep_mode (11h)
	TFT_WriteIndex(ENT_NML_MODE);						 // Enter_normal_mode (13h)
	
	TFT_WriteIndex(POW_SET);						     // Power_Setting (D0h)
	TFTWriteData(0x0007);
	TFTWriteData(0x0040);   
	TFTWriteData(0x001C); 
	
	TFT_WriteIndex(VCOM_CTRL);							 // VCOM Control (D1h)
	TFTWriteData(0x0000);
	TFTWriteData(0x0018);
	TFTWriteData(0x001D);
	
	TFT_WriteIndex(POW_SET_NML_MODE);					 // Power_Setting for Normal Mode (D2h)
	TFTWriteData(0x0001);
	TFTWriteData(0x0011);
	
	TFT_WriteIndex(PNL_DRV_SET);						 // Panel Driving Setting (C0h)
	TFTWriteData(0x0000);
	TFTWriteData(0x003B);
	TFTWriteData(0x0000);
	TFTWriteData(0x0002);
	TFTWriteData(0x0011);
	
	TFT_WriteIndex(DISP_TIM_SET_NML_MODE);				 // Display_Timing_Setting for Normal Mode (C1h)
	TFTWriteData(0x0010);
	TFTWriteData(0x000B);
	TFTWriteData(0x0088);
	
	TFT_WriteIndex(FRM_RATE_INV_CTRL);					 // Frame Rate and Inversion Control (C5h)
	TFTWriteData(0x0001);
	
	TFT_WriteIndex(GMA_SET);							 // Gamma Setting (C8h)
	TFTWriteData(0x0000);
	TFTWriteData(0x0030);  
	TFTWriteData(0x0036);
	TFTWriteData(0x0045);
	TFTWriteData(0x0004);
	TFTWriteData(0x0016);
	TFTWriteData(0x0037);
	TFTWriteData(0x0075);
	TFTWriteData(0x0077);
	TFTWriteData(0x0054);
	TFTWriteData(0x000F);
	TFTWriteData(0x0000);
	
	TFTSetDisplayMode(0, 0, 1, 1, 0);					 // Set_address_mode (36h)
	
	TFT_WriteIndex(SET_PXL_FRMT);						 // Set_pixel_format (3Ah)
	TFTWriteData(0x0055); 
	
	TFT_DelayUs(48000);									 // 延时不能减少了
	TFT_WriteIndex(SET_DISP_ON);						 // Set_display_on (29h)
	TFT_WriteIndex(WRITE_MMR_START);					 // Write_memory_start (2Ch)

    TFTSetDisplayMode(0, 0, 1, 0, 0);
	TFTClear(color);									 // 清屏
	TFTBacklightCtrl(TFT_BLK_ON);						 // 背光开
}

/********************************************************************************************************
* FunctionName   : TFTWriteData()
* Description    : 写TFT寄存器数据
* EntryParameter : dat - 寄存器数据
* ReturnValue    : None
********************************************************************************************************/
void TFTWriteData(u16 dat)
{
    GPIOSetValue(TFT_RS_PORT, TFT_RS_PIN, GPIO_SET);				// 写入数据
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);

    GPIOSetMultiValue(TFT_DAT_H_PORT, TFT_DAT_H, ((u8)(dat>>8)) << TFT_DAT_H_PIN);
	GPIOSetMultiValue(TFT_DAT_L_PORT, TFT_DAT_L, (u8)(dat) << TFT_DAT_L_PIN); 

	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_CLR);
	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_SET);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFT_WriteIndex()
* Description    : 写TFT寄存器地址,调用前需先选中控制器
* EntryParameter : idx - 寄存器地址
* ReturnValue    : None
********************************************************************************************************/
void TFT_WriteIndex(u16 idx)
{
    GPIOSetValue(TFT_RS_PORT, TFT_RS_PIN, GPIO_CLR);				  // 写入指令
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);

    GPIOSetMultiValue(TFT_DAT_H_PORT, TFT_DAT_H, ((u8)(idx>>8)) << TFT_DAT_H_PIN);
	GPIOSetMultiValue(TFT_DAT_L_PORT, TFT_DAT_L, (u8)(idx) << TFT_DAT_L_PIN); 

	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_CLR);
	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_SET);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);    
}

/********************************************************************************************************
* FunctionName   : TFT_SetCursor()
* Description    : 设置屏幕坐标
* EntryParameter : x - 行坐标；y - 列坐标
* ReturnValue    : None
********************************************************************************************************/
void TFT_SetCursor(u16 x, u16 y)
{
	TFT_WriteIndex(0x002A);		 // Set_column_address (2Ah) < 320
	TFTWriteData(x >> 8);		 // 
	TFTWriteData(0x00FF & x);
	TFTWriteData(0x0001);
	TFTWriteData(0x00DF);

	TFT_WriteIndex(0x002B);		 // Set_page_address (2Bh) < 480
	TFTWriteData(y >> 8);
	TFTWriteData(0x00FF & y);
	TFTWriteData(0x0001);
	TFTWriteData(0x003F);
	
	TFT_WriteIndex(0x002C);				
}

/********************************************************************************************************
* FunctionName   : TFTSetPoint()
* Description    : 在指定座标画点
* EntryParameter : x - 行坐标；y - 列坐标；color - 点的颜色
* ReturnValue    : None
********************************************************************************************************/
void TFTSetPoint(u16 x, u16 y, u16 color)
{
	TFT_SetCursor(x,y);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);

	TFT_WriteIndex(0x002C);
	TFTWriteData(color);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTClear()
* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xFFFF
* EntryParameter : dat - 填充值
* ReturnValue    : None
********************************************************************************************************/
void TFTClear(u16 dat)
{
    u32 i;

	TFT_SetCursor(0, 0);														// 设置光标在起始点
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);
	TFT_WriteIndex(WRITE_MMR_START);

    for (i=0; i<TFT_WIDTH*TFT_HEIGTH; i++)										// 循环写入数据
	{
	    TFTWriteData(dat);
	}

	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTDrawPicture()
* Description    : 在指定座标范围显示一副图片，只能显示取模的图片
* EntryParameter : startX,startY,heigth,width,*pic
* ReturnValue    : None
********************************************************************************************************/
void TFTDrawPicture(u16 startX, u16 startY, u16 heigth, u16 width, uc8 *pic)
{
    u32 i,k;
	u16 tmp;

	for (i=startY; i<startY+width; i++)
	{
	    TFT_SetCursor(startX,startY+i);
	    for (k=startX; k<startX+heigth; k++)
		{
		    tmp  = *pic++;                       		// 低8位数据暂存
			tmp += (*pic++ << 8);					    // 取高8位数据，并合并成16位数据
			TFTWriteData(tmp);
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFTPutChar()
* Description    : 在指定座标显示一个8x16点阵的ascii字符(水平扫描，高位在左)
* EntryParameter : x - 行坐标；y - 列坐标；pCh - 字符；chColor - 字符的颜色；bkColor - 字符背景颜色
* ReturnValue    : None
********************************************************************************************************/
void TFT_PutChar(u16 x,u16 y, u8 *pCh, u16 chColor, u16 bkColor)
{
    u16 i = 0;
	u16 j = 0;
    u8 tmpChar = 0;

	for (i=0; i<16; i++)
	{
	    tmpChar = St8x16[((*pCh-0x20)*16) + i];

		for (j=0; j<8; j++)
		{
		    if (((tmpChar >> (7-j)) & 0x01) == 0x01)
			{
			    TFTSetPoint(x+j ,y+i, chColor);	     // 字符颜色
			}
			else
			{
			    if (bkColor != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+j, y+i, bkColor);	 // 背景颜色
				}
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFT_PutChinese()
* Description    : 在指定座标显示一个16x16点阵的中文汉字(水平扫描，高位在左)
* EntryParameter : x - 行坐标；y - 列坐标；chColor - 字符的颜色；bkColor - 字符背景颜色
* ReturnValue    : None
********************************************************************************************************/
void TFT_PutChinese(u16 x, u16 y, u8* pStr, u16 chColor, u16 bkColor)
{
    u8 i,k;
	u32 offSet, str;

	// 汉字的地址是从0xA1A1开始
	offSet  = ((*pStr - 0xA1)*94 + (*(pStr+1) - 0xA1))*32;
	offSet += W25Q_CN_START_ADDR;

	for (i=0; i<32; i++)						  	   // 一个汉字32字节
	{
	    str = W25QReadByte(offSet + i);
		for (k=0; k<8; k++)							     // 一个字节8位
		{
		    if ((str >> (7-k)) & 0x01)
			{
			    TFTSetPoint(x+k, y+(i/2), chColor);	     // 字符颜色
			}
			else
			{
				if (bkColor != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+k, y+(i/2), bkColor);  // 背景颜色
				}
			}
		}

		if (((i!=0)&&((i%2)) != 0))                      // 奇数
		{
		    x -= 8;									     // 写左半
		}
		else
		{
		    x += 8;									     // 写右半
		}
	}
} 

/********************************************************************************************************
* FunctionName   : TFTPutStr()
* Description    : 输出字符串，可以是汉字，也可以是英文字符(带自动换行显示)
* EntryParameter : x - 行坐标；y - 列坐标；chColor - 字符的颜色；bkColor - 字符背景颜色
* ReturnValue    : 显示字符数
********************************************************************************************************/
u16 TFTPutStr(u16 x, u16 y, u8 *pStr, u16 chColor, u16 bkColor)
{
    u16	disCount  = 0;									           // 显示字符数
	u16 lineCount = 0;									           // 行计数
	u16 yAdd = y;

	lineCount = y;
	while ((*pStr != 0) && ((yAdd+16) <= TFT_HEIGTH))
	{   
	                                                               // ASCII字符与汉字内码的区别在于128
		if (*pStr <= 0x80)	                                       // ASCII < 128
		{
		     if ((lineCount + 8) > TFT_WIDTH)			           // 不够显示一个字符，换行显示
			 {
			     lineCount = 0;
				 x = 0;
				 yAdd += 16;    
			 }
			 else
			 {
			     TFT_PutChar(x, yAdd, pStr, chColor, bkColor);     // 显示字符
	
				 x += 8;							               // 横向地址加1个字符位置
				 pStr++;								           // 取下一字符
				 disCount++;
				 lineCount += 8;
			 }
		}											
		else										               // 汉字 > 128
		{
		    if ((lineCount + 16) > TFT_WIDTH)			           // 不够显示一个字符，换行显示
			{
			    lineCount = 0;
				x = 0;
				yAdd += 16;    
			}
			else
			{
				TFT_PutChinese(x, yAdd, pStr, chColor, bkColor);   // 显示汉字
															   
				x += 16;							               // 横向地址加1个字符位置
			    pStr += 2;
			    disCount += 2;
				lineCount += 16;
			}
		}	
	}

	return disCount;
}

/********************************************************************************************************
* FunctionName   : TFTSetDisplayMode()
* Description    : 设定图片显示方向和显示扫描方向
* EntryParameter : rlScan-左右扫描, tbScan-上下扫描, dirScan-水平/垂直扫描, hFilp-水平翻转, vFilp-垂直翻转
* ReturnValue    : None
********************************************************************************************************/
/* TB: Selects the output shift direction of the gate driver. When TB = 1, G0 shifts to G319. When TB = 0, 
G319 shifts to G0. */
void TFTSetDisplayMode(u8 rlScan, u8 tbScan, u8 dirScan, u8 hFilp, u8 vFilp)
{
    u16 vol = 0x0008;
	u16 column = 0, page = 0;

    if (rlScan == 1)						 // 左右扫描
	{
	    vol |= 0x0040;
	}

	if (tbScan == 1)						 // 上下扫描
	{
	    vol |= 0x0080;
	}

	if (dirScan == 1)						 // 扫描方向
	{
	    vol |= 0x0020;
	}

	if (hFilp == 1)							 // 水平翻转
	{
	    vol |= 0x0002;
	}

	if (vFilp == 1)						     // 垂直翻转
	{
	    vol |= 0x0001;
	}

	TFT_WriteIndex(SET_ADDR_MODE);		     // Set_address_mode (36h)
	TFTWriteData(vol);
   
	if (dirScan == 1)					     // 更改扫描方向后，需要更改长宽设置
	{
		column = TFT_WIDTH-1;
		page = TFT_HEIGTH-1;					 
	}
	else
	{
		column = TFT_HEIGTH-1;
		page = TFT_WIDTH-1;		
	}

	TFT_WriteIndex(SET_CLM_ADDR);		     // Set_column_address (2Ah)
	TFTWriteData(0x0000);
	TFTWriteData(0x0000);
	TFTWriteData(column>>8);
	TFTWriteData(column&0xFF);								 
	
	TFT_WriteIndex(SET_PAGE_ADDR);		     // Set_page_address (2Bh)
	TFTWriteData(0x0000);
	TFTWriteData(0x0000);
	TFTWriteData(page>>8);
	TFTWriteData(page&0xFF); 
									 
	TFT_WriteIndex(WRITE_MMR_START);	    // Write_memory_start (2Ch)
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/
