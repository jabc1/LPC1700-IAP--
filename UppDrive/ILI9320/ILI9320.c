/********************************************************************************************************    
*                                                                        
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ILI9320.c
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

	TFTWriteData(0xFFFF);

	TFT_RT_OUT(GPIO_SET);
	TFT_WR_OUT(GPIO_SET);
	TFT_CS_OUT(GPIO_SET);
	TFT_RS_OUT(GPIO_SET);
	TFT_RD_OUT(GPIO_SET);
	TFT_EN_OUT(GPIO_SET);

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

    TFT_RT_OUT(GPIO_SET);					   // 可以不要
	TFT_DelayUs(10);
	TFT_RT_OUT(GPIO_CLR);					   // 保持低电平1ms复位TFT
	TFT_DelayUs(1000);
	TFT_RT_OUT(GPIO_SET);	  				   // 拉高撤销复位
}

/********************************************************************************************************
* FunctionName   : TFTBacklightCtrl()
* Description    : 背光开关
* EntryParameter : blk - 1开，0关
* ReturnValue    : None
********************************************************************************************************/
void TFTBacklightCtrl(u8 blk)
{
	TFT_EN_OUT(blk);
}

/********************************************************************************************************
* FunctionName   : TFT_WriteRegister()
* Description    : 写指定地址寄存器的值
* EntryParameter : index - 寄存器地址；dat - 寄存器值
* ReturnValue    : None
********************************************************************************************************/
void TFT_WriteRegister(u16 index, u16 dat)
{
	/*------------------------------- 时序图 ---------------------------------------------
	--                                                  --
	--               nCS      ----\______________________________/---                   --
	--               RS       ------\____________/-------------------                   --
	--               nRD      ---------------------------------------                   --
	--               nWR      --------\_______/-----\_____/----------                   --
	--               DB[0:15] ---------[index]------[data]-----------                   --
	--                                                                                  --
	------------------------------------------------------------------------------------*/   
 
    TFT_CS_OUT(GPIO_CLR);                              // 片选
	TFTWriteIndex(index); 							   // 写寄存器地址
	TFTWriteData(dat);		                           // 写数据
	TFT_CS_OUT(GPIO_SET);                              // 
}

/********************************************************************************************************
* FunctionName   : TFT_SetCursor()
* Description    : 设置屏幕坐标
* EntryParameter : x - 行坐标；y - 列坐标
* ReturnValue    : None
********************************************************************************************************/
void TFT_SetCursor(u16 x, u16 y)
{
    TFT_WriteRegister(0x0020, x);                    // 行
	TFT_WriteRegister(0x0021, y);					 // 列
	TFT_RWReady();								 // 写准备
}

/********************************************************************************************************
* FunctionName   : TFT_Clear()
* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xFFFF
* EntryParameter : dat - 填充值
* ReturnValue    : None
********************************************************************************************************/
void TFT_Clear(u16 col, u16 sx, u16 sy, u16 ex, u16 ey)
{
    u32 i, k;

    TFTSetWindows(sx, sy, ex, ey);							  // 设置窗口
	for (i=sx; i<ex-sx; i++)
	{
	    TFT_SetCursor(sx+i, sy);							  // 坐标设置
	    for (k=sy; k<ey-sy; k++)
		{				      
		    TFTWriteData(col);		                          // 写数据
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFTWriteData()
* Description    : 写TFT寄存器数据
* EntryParameter : dat - 寄存器数据
* ReturnValue    : None
********************************************************************************************************/
void TFTWriteData(u16 dat)
{
    TFT_DAT_OUT(dat);									 // 写16位数据
	TFT_WR_OUT(GPIO_CLR);
	TFT_WR_OUT(GPIO_SET);  
}

/********************************************************************************************************
* FunctionName   : TFTSetWindows()
* Description    : 设置窗口区域
* EntryParameter : stX,stY,endX,endY
* ReturnValue    : None
********************************************************************************************************/
void TFTSetWindows(u16 stX, u16 stY, u16 endX, u16 endY)
{
	TFT_WriteRegister(0x0050, stX);	                     // 水平GRAM起始位置
	TFT_WriteRegister(0x0051, endX);                     // 水平GRAM终止位置
	TFT_WriteRegister(0x0052, stY);				         // 垂直GRAM起始位置
	TFT_WriteRegister(0x0053, endY);					 // 垂直GRAM终止位置
}

/********************************************************************************************************
* FunctionName   : TFTWriteIndex()
* Description    : 写TFT寄存器地址,调用前需先选中控制器
* EntryParameter : idx - 寄存器地址
* ReturnValue    : None
********************************************************************************************************/
void TFTWriteIndex(u16 idx)
{
    TFT_RS_OUT(GPIO_CLR);				                  // 写入指令
	TFTWriteData(idx);								      // 写地址
	TFT_RS_OUT(GPIO_SET);	  							  // 
}

/********************************************************************************************************
* FunctionName   : TFT_RWReady()
* Description    : 读写准备
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TFT_RWReady(void)
{
	TFT_CS_OUT(GPIO_CLR);
	TFTWriteIndex(0x0022);
	TFT_RS_OUT(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTPutChar()
* Description    : 在指定座标显示一个8x16点阵的ascii字符
* EntryParameter : x - 行坐标；y - 列坐标；chClr - 字符的颜色；bkClr - 字符背景颜色
* ReturnValue    : None
********************************************************************************************************/
/* 注意：要对字体进行垂直或水平翻转，或左右上下翻转，只需要交换x，y坐标或更改x，y坐标的增减即可。 */
void TFTPutChar(u16 x, u16 y, u8 *pCh, u16 chClr, u16 bkClr)
{
    u8 i, k;
    u8 tmpChar = 0;

	for (i=0; i<16; i++)
	{
	    tmpChar = TFT_READ_EN_FONT(((*pCh-0x20)*16) + i);	   // 读西文字模
		for (k=0; k<8; k++)
		{
		    if (((tmpChar >> (7-k)) & 0x01) == 0x01)
			{
			    TFTSetPoint(x+i, y+k, chClr);	               // 字符颜色
			}
			else
			{
			    if (bkClr != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+i, y+k, bkClr);	           // 背景颜色
				}
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFTPutChinese()
* Description    : 在指定座标显示一个16x16点阵的中文汉字
* EntryParameter : x - 行坐标；y - 列坐标；chClr - 字符的颜色；bkClr - 字符背景颜色
* ReturnValue    : None
********************************************************************************************************/
void TFTPutChinese(u16 x, u16 y, u8 *pStr, u16 chClr, u16 bkClr)
{
    u8 i,k;
	u32 offSet, str;

	// 汉字的地址是从0xA1A1开始
	offSet  = ((*pStr - 0xA1)*94 + (*(pStr+1) - 0xA1))*32;
	offSet += TFT_CN_SATRA_ADDR;								// 中文字库首地址

	for (i=0; i<32; i++)						  	            // 一个汉字32字节
	{
	    str = TFT_READ_CN_FONT(offSet + i);					    // 读字库
		for (k=0; k<8; k++)							            // 一个字节8位
		{
		    if ((str >> (7-k)) & 0x01)
			{
			    TFTSetPoint(x+(i/2), y+k, chClr);	            // 字符颜色
			}
			else
			{
				if (bkClr != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+(i/2), y+k, bkClr);           // 背景颜色
				}
			}
		}

		y = (((i!=0)&&((i%2)) != 0)) ? (y-8) : (y+8);           // 奇数
	}
} 

/********************************************************************************************************
* FunctionName   : TFTPutStr()
* Description    : 输出字符串，可以是汉字，也可以是英文字符
* EntryParameter : x - 行坐标；y - 列坐标；chClr - 字符的颜色；bkClr - 字符背景颜色
* ReturnValue    : None
********************************************************************************************************/
void TFTPutStr(u16 x, u16 y, u8 *ptr, u16 chClr, u16 bkClr)
{
	while (*ptr != 0)
	{                                                  // ASCII字符与汉字内码的区别在于128
		if (*ptr <= 0x80)	                           // ASCII < 128
		{
		     TFTPutChar(x, y, ptr, chClr, bkClr);      // 显示字符
			 y += 8;							       // 横向地址加1个字符位置
			 ptr++;								       // 取下一字符
		}											
		else										   // 汉字 > 128
		{
			TFTPutChinese(x, y, ptr, chClr, bkClr);    // 显示汉字
			y += 16;							       // 横向地址加1个字符位置
			ptr += 2;
		}
	}
} 

/********************************************************************************************************
* FunctionName   : TFTDrawPicture()
* Description    : 在指定座标范围显示一副图片
* EntryParameter : sx,sy,ex,ey,*pic
* ReturnValue    : None
********************************************************************************************************/
void TFTDrawPicture(u16 sx, u16 sy, u16 ex, u16 ey, uc8 *pic)
{
    u32 i, k;

	TFTSetWindows(sx, sy, ex, ey);	                          // 设置图片显示窗口
	for (i=sx; i<ex-sx; i++)
	{
	    TFT_SetCursor(sx+i, sy);							  // 坐标设置 
	    for (k=sy; k<ey-sy; k++)
		{				      
		    TFTWriteData((*pic++) + (*pic++ << 8));		      // 取高、低8位数据，并合并成16位数据
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFTSetPoint()
* Description    : 在指定座标画点
* EntryParameter : x - 行坐标；y - 列坐标；color - 点的颜色
* ReturnValue    : None
********************************************************************************************************/
void TFTSetPoint(u16 x, u16 y, u16 color)
{
    if ((x<TFT_WIDTH) && (y<TFT_HEIGTH))	                        // 判断显示坐标范围
	{
	    TFT_SetCursor(x, y);					                    // 写入坐标值
	    TFTWriteData(color);				 	                    // 写入数据
	}
}

/********************************************************************************************************
* FunctionName   : TFTInit()
* Description    : 初始化TFT
* EntryParameter : color - 初始化颜色
* ReturnValue    : None
********************************************************************************************************/
void TFTInit(u16 color)
{
    TFT_SetGpio();					                                // 初始化控制端口
	TFTWriteData(0xFFFF);
											    
    TFT_Reset();									                // TFT复位
	TFT_WriteRegister(0x00, 0x0001);                                // 启动晶振
	TFT_WriteRegister(0x01, 0x0000);	                            // 驱动器输出控制, SS = 0
	TFT_WriteRegister(0x02, 0x0700);	                            // LCD驱动波形对照
	TFT_WriteRegister(0x03, 0x1038);	                            // 进入模式设置, 竖直扫描

	TFT_WriteRegister(0x04, 0x0000);	                            // 调整大小设置
	TFT_WriteRegister(0x08, 0x0202);	                            // 显示控制2
	TFT_WriteRegister(0x09, 0x0000);     	                        // 显示控制3
	TFT_WriteRegister(0x0A, 0x0000);	                            // 
	TFT_WriteRegister(0x0C, (1<<0));	                            // 外接显示接口
	TFT_WriteRegister(0x0D, 0x0000);	                            // 
	TFT_WriteRegister(0x0F, 0x0000);	                            // 

	TFT_WriteRegister(0x07, 0x0101);	                            // 显示控制

	TFT_WriteRegister(0x10, (1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	// 电源控制1
	TFT_WriteRegister(0x11, 0x0007);			                    // 电源控制2
	TFT_WriteRegister(0x12, (1<<8)|(1<<4)|(0<<0));                  // 电源控制3 
	TFT_WriteRegister(0x13, 0x0B00);				                // 电源控制4
	TFT_WriteRegister(0x29, 0x0000);					            // 电源控制7

	TFT_WriteRegister(0x2B, (1<<14)|(1<<4));
	
	TFT_WriteRegister(0x50, 0);		                                // 设置X起始坐标
	TFT_WriteRegister(0x51, 239);	                                // 设置X结束坐标
	TFT_WriteRegister(0x52, 0);		                                // 设置Y起始坐标
	TFT_WriteRegister(0x53, 319);	                                // 设置Y结束坐标

	TFT_WriteRegister(0x60, 0x2700);	                            // 驱动器输出, GS = 0
	TFT_WriteRegister(0x61, 0x0001);	                            // 驱动器输出
	TFT_WriteRegister(0x6A, 0x0000);	                            // 垂直Srcoll控制

	TFT_WriteRegister(0x80, 0x0000);	                            // 部分图片1显示位置
	TFT_WriteRegister(0x81, 0x0000);	                            // 部分图片1RAM起始/结束地址
	TFT_WriteRegister(0x82, 0x0000);	                            // 
	TFT_WriteRegister(0x83, 0x0000);	                            // 部分图片2显示位置
	TFT_WriteRegister(0x84, 0x0000);	                            // 部分图片2RAM起始/结束地址
	TFT_WriteRegister(0x85, 0x0000);	                            // 

	TFT_WriteRegister(0x90, (0<<7)|(16<<0));	                    // 面板接口控制1
	TFT_WriteRegister(0x92, 0x0000);	                            // 面板接口控制2
	TFT_WriteRegister(0x93, 0x0001);	                            // 面板接口控制3
	TFT_WriteRegister(0x95, 0x0110);	                            // 面板接口控制4
	TFT_WriteRegister(0x97, (0<<8));	                            // 面板接口控制5
	TFT_WriteRegister(0x98, 0x0000);	                            // 面板接口控制6

	TFT_WriteRegister(0x07, 0x0173);	                            // 显示控制	

	TFT_DelayUs(50000);
	TFT_Clear(color, 0, 0, TFT_WIDTH, TFT_HEIGTH);					// 写入一种颜色
	TFTBacklightCtrl(TFT_BLK_ON);									// 打开背光
}

/********************************************************************************************************
* FunctionName   : TFTSetCooOrigin()
* Description    : 坐标原点设置
* EntryParameter : gs - 左右翻转，ss - 上下翻转
* ReturnValue    : NO
********************************************************************************************************/
void TFTSetCooOrigin(u8 gs, u8 ss)
{
	(gs == 0) ? TFT_WriteRegister(0x60, 0x2700) : TFT_WriteRegister(0x60, 0xA700);	// 左右翻
    (ss == 0) ?	TFT_WriteRegister(0x01, 0x0000) : TFT_WriteRegister(0x01, 0x0100);	// 上下翻
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/
