/**************************************************************************************    
*                       Crazy ARM LPC2148 V2.0 开发板实验程序                                                 
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : SSD1289.c
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V3.70
* Description    :
*
*
**************************************************************************************/

/**************************************************************************************
*                                  HeaderFiles                                                  
**************************************************************************************/
#include "../source/config.h"    

/**************************************************************************************
* FunctionName   : TFT_Delay()
* Description    : TFT延时
* EntryParameter : t
* ReturnValue    : NO
**************************************************************************************/
void TFT_Delay(uint32 t) 
{
	for (; t!=0; t--)
	{
        ;
	}
}

/**************************************************************************************
* FunctionName   : TFT_SetGPIO()
* Description    : TFT端口方向设置
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void TFT_SetGPIO(void)
{
    TFT_CS_DDR() |= (1<<TFT_CS);
	TFT_RS_DDR() |= (1<<TFT_RS);
	TFT_WR_DDR() |= (1<<TFT_WR);
	TFT_RD_DDR() |= (1<<TFT_RD);
	TFT_LE_DDR() |= (1<<TFT_LE);

	TFT_RST_DDR() |= (1<<TFT_RST);
	TFT_BLK_DDR() |= (1<<TFT_BLK); 
	TFT_DAT_DDR() |= (TFT_DAT);

	TFT_RST_SET();									     // 所有控制端口拉高
	TFT_WR_SET();
	TFT_CS_SET();
	TFT_RS_SET();
	TFT_RD_SET();
	TFT_LE_SET();
	TFT_RST_SET();
	TFT_BLK_ON();	           						     // 开背光
}

/**************************************************************************************
* FunctionName   : TFT_Reset()
* Description    : TFT复位,	注意复位时间
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void TFT_Reset(void)
{
/*----------------------------------- 时序图 -----------------------------------------
--                                                                                  --
--                  ------------\_____________________/-----------                  --
--                              | <----  Tres  ---->  |                             --
--                                                                                  --
--                             Tres: Min.1ms                                        --
------------------------------------------------------------------------------------*/
    TFT_RST_SET();
	TFT_Delay(5000);
	
	TFT_RST_CLR();
	TFT_Delay(5000);
	
    TFT_RST_SET();
	TFT_Delay(5000);	  
}

/**************************************************************************************
* FunctionName   : TFT_WriteRegister()
* Description    : 写指定地址寄存器的值
* EntryParameter : index - 寄存器地址；dat - 寄存器值
* ReturnValue    : NO
**************************************************************************************/
void TFT_WriteRegister(uint16 index,uint16 dat)
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
 
    TFT_CS_CLR();
	TFT_RD_SET();

	TFT_RS_CLR();	                                 // 写指令
    TFT_WriteData(index);

	TFT_RS_SET();
	TFT_WriteData(dat);		                         // 写数据

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFT_ReadRegister()
* Description    : 读取指定地址寄存器的值
* EntryParameter : index - 寄存器地址
* ReturnValue    : NO
**************************************************************************************/
uint16 TFT_ReadRegister(uint16 index)
{
    uint16 tmp;

	TFT_CS_CLR();
    TFT_RS_CLR();
	TFT_RD_SET();

	TFT_WriteData(index);

	TFT_RD_CLR();

	tmp = TFT_ReadData();

	TFT_RD_SET();
	TFT_CS_SET();

    return tmp;
}

/**************************************************************************************
* FunctionName   : TFT_ReadData()
* Description    : 读取控制寄存器数
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
uint16 TFT_ReadData(void)
{
    uint16 tmp;

#if TFT_DATA_MODE
    tmp = (TFT_READ_WORD_DATA() & TFT_DAT);		 // 16位读
#else
												 // 8位读
#endif
	return tmp;
}

/**************************************************************************************
* FunctionName   : TFT_WriteData()
* Description    : 写TFT寄存器数据
* EntryParameter : dat - 寄存器数据
* ReturnValue    : NO
**************************************************************************************/
void TFT_WriteData(uint16 dat)
{
#if TFT_DATA_MODE
	TFT_WRITE_WORD_DATA(dat);						 // 写16位数据
#else
	TFT_LE_SET();
	TFT_WRITE_BYTE_DATA((uint8)dat);				 // 先写低8位
	TFT_LE_CLR();									 // 数据锁存
	TFT_WRITE_BYTE_DATA((uint8)(dat>>8));			 // 写高8位
#endif 
	TFT_WR_CLR();
	TFT_WR_SET();
}

/**************************************************************************************
* FunctionName   : TFT_SetCursor()
* Description    : 设置屏幕坐标
* EntryParameter : x - 行坐标；y - 列坐标
* ReturnValue    : NO
**************************************************************************************/
void TFT_SetCursor(uint16 x,uint16 y)
{
    TFT_WriteRegister(0x004F,y);                     // 行
	TFT_WriteRegister(0x004E,x);					 // 列
}

/**************************************************************************************
* FunctionName   : TFT_SetWindows()
* Description    : 设置窗口区域
* EntryParameter : startX,startY,endX,endY
* ReturnValue    : NO
**************************************************************************************/
void TFT_SetWindows(uint16 startX,uint16 startY,uint16 endX,uint16 endY)
{
    TFT_SetCursor(startX,startY);
	TFT_WriteRegister(0x0050,startX);	             // 水平GRAM起始位置
	TFT_WriteRegister(0x0051,endX);                  // 水平GRAM终止位置
	TFT_WriteRegister(0x0052,startY);				 // 垂直GRAM起始位置
	TFT_WriteRegister(0x0053,endY);					 // 垂直GRAM终止位置
}

/**************************************************************************************
* FunctionName   : TFT_BGR2RGB()
* Description    : 颜色数据格式转换
* EntryParameter : c RRRRRGGGGGBBBBB 改成 BBBBBGGGGGRRRRR
* ReturnValue    : NO
**************************************************************************************/
uint16 TFT_BGR2RGB(uint16 c)
{
    uint16 r,g,b;

	b = (c>>0) & 0x1F;
	g = (c>>5) & 0x3F;
	r = (c>>11) & 0x1F;

	return ((b<<11) | (g<<5) || (r<<0));
}

/**************************************************************************************
* FunctionName   : TFT_WriteIndex()
* Description    : 写TFT寄存器地址,调用前需先选中控制器
* EntryParameter : idx - 寄存器地址
* ReturnValue    : NO
**************************************************************************************/
void TFT_WriteIndex(uint16 idx)
{
    TFT_RS_CLR();
	TFT_RD_SET();
	TFT_WriteData(idx);
	TFT_RS_SET();    
}

/**************************************************************************************
* FunctionName   : TFT_Clear()
* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xFFFF
* EntryParameter : dat - 填充值
* ReturnValue    : NO
**************************************************************************************/
void TFT_Clear(uint16 dat)
{
    uint32 i;

    TFT_SetCursor(0x00,0x0000);
	TFT_WriteRegister(0x0050,0x00);	                 // 水平GRAM起始位置
	TFT_WriteRegister(0x0052,0x00);                  // 垂直GRAM起始位置
	TFT_WriteRegister(0x0051,239);	                 // 水平GRAM终止位置
	TFT_WriteRegister(0x0053,319);	                 // 垂直GRAM终止位置

    TFT_CS_CLR();
	TFT_WriteIndex(0x0022);
	TFT_RS_SET();

    for (i=0; i<76800; i++)
	{
	    TFT_WriteData(dat);
	}

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTDrawPicture()
* Description    : 在指定座标范围显示一副图片
* EntryParameter : startX,startY,endX,endY,*pic
* ReturnValue    : NO
**************************************************************************************/
void TFTDrawPicture(uint16 startX,uint16 startY,uint16 endX,uint16 endY,uint8 const *pic)
{
    uint32 i;
	uint16 tmp;

	TFT_SetWindows(startX,startY,endX,endY);
	TFT_SetCursor(startX,startY);

	TFT_CS_CLR();
	TFT_WriteIndex(0x0022);
	TFT_RS_SET();

	for (i=0; i<(endX*endY); i++)
	{
	    tmp = *pic++;                       		// 低8位数据暂存
		tmp += (*pic++ << 8);					    // 取高8位数据，并合并成16位数据

	    TFT_WriteData(tmp);							// 写入16位数据
	}

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTPutChar()
* Description    : 在指定座标显示一个8x16点阵的ascii字符
* EntryParameter : x - 行坐标；y - 列坐标；charColor - 字符的颜色；bkColor - 字符背景颜色
* ReturnValue    : NO
**************************************************************************************/
void TFT_PutChar(uint16 x,uint16 y,uint8 *asciiCode,uint16 charColor,uint16 bkColor)
{
    uint16 i = 0;
	uint16 j = 0;
    uint8 tmpChar = 0;

	for (i=0; i<16; i++)
	{
	    tmpChar = AsciiLib[((*asciiCode-0x20)*16) + i];

		for (j=0; j<8; j++)
		{
		    if (((tmpChar >> (7-j)) & 0x01) == 0x01)
			{
			    TFTSetPoint(x+j,y+i,charColor);	     // 字符颜色
			}
			else
			{
			    if (bkColor != NOCOLOR)
				{
			        TFTSetPoint(x+j,y+i,bkColor);	 // 背景颜色
				}
			}
		}
	}
}

/**************************************************************************************
* FunctionName   : TFT_PutChinese()
* Description    : 在指定座标显示一个16x16点阵的中文汉字
* EntryParameter : x - 行坐标；y - 列坐标；charColor - 字符的颜色；bkColor - 字符背景颜色
* ReturnValue    : NO
**************************************************************************************/
void TFT_PutChinese(uint16 x,uint16 y,uint8* ptr,uint16 charColor,uint16 bkColor)
{
    uint8 i,k;
	uint32 offSet,str;

	// 汉字的地址是从0xB0A1开始
	offSet = ((*ptr - 0xB0)*94 + (*(ptr+1) - 0xA1))*32;

	for (i=0; i<32; i++)						  	   // 一个汉字32字节
	{
	    str = *(HzLib + offSet + i);
		for (k=0; k<8; k++)							   // 一个字节8位
		{
		    if ((str >> (7-k)) & 0x01)
			{
			    TFTSetPoint(x+k,y+(i/2),charColor);	   // 字符颜色
			}
			else
			{
				if (bkColor != NOCOLOR)
				{
			        TFTSetPoint(x+k,y+(i/2),bkColor);  // 背景颜色
				}
			}
		}

		if (((i!=0)&&((i%2)) != 0))                    // 奇数
		{
		    x -= 8;									   // 写左半
		}
		else
		{
		    x += 8;									   // 写右半
		}
	}
}

/**************************************************************************************
* FunctionName   : TFTPutStr()
* Description    : 输出字符串，可以是汉字，也可以是英文字符
* EntryParameter : x - 行坐标；y - 列坐标；charColor - 字符的颜色；bkColor - 字符背景颜色
* ReturnValue    : NO
**************************************************************************************/
void TFTPutStr(uint16 x,uint16 y,uint8 *ptr,uint16 charColor,uint16 bkColor)
{
	while (*ptr != 0)
	{                                                  // ASCII字符与汉字内码的区别在于128
		if (*ptr <= 0x80)	                           // ASCII < 128
		{
		     TFT_PutChar(x,y,ptr,charColor,bkColor);   // 显示字符

			 x += 8;							       // 横向地址加1个字符位置
			 ptr++;								       // 取下一字符
		}											
		else										   // 汉字 > 128
		{
			TFT_PutChinese(x,y,ptr,charColor,bkColor); // 显示汉字

			x += 16;							       // 横向地址加1个字符位置
			ptr += 2;
		}
	}
}

/**************************************************************************************
* FunctionName   : TFTTest()
* Description    : 测试液晶屏,显示彩条，测试液晶是否正常工作
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void TFTTest(void)
{
    uint8 R_Dat,G_Dat,B_Dat,i,j;

	TFT_SetCursor(0x00,0x0000);
	TFT_WriteRegister(0x0050,0x00);
	TFT_WriteRegister(0x0051,239);
	TFT_WriteRegister(0x0052,0x00);
	TFT_WriteRegister(0x0053,319);

	TFT_CS_CLR();
	TFT_WriteIndex(0x0022);
	TFT_RS_SET();
// ------------------  红色渐亮条  --------------------
	R_Dat = 0;
	G_Dat = 0;
	B_Dat = 0;
	for (j=0; j<53; j++)  
	{
	    for (i=0; i<240; i++)
		{
		    R_Dat = i/8;
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	R_Dat = 0x1F;
	G_Dat = 0x3F;
	B_Dat = 0x1F;
	for (j=0; j<54; j++) 
	{
	    for (i=0; i<240; i++)
		{
		    G_Dat = 0x3F - (i/4);
			B_Dat = 0x1F - (i/8);
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}
// ------------------  绿色渐亮条  --------------------
	R_Dat = 0;
	G_Dat = 0;
	B_Dat = 0;
	for (j=0; j<53; j++)  
	{
	    for (i=0; i<240; i++)
		{
		    G_Dat = i/8;
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	R_Dat = 0x1F;
	G_Dat = 0x3F;
	B_Dat = 0x1F;
	for (j=0; j<54; j++)
	{
	    for (i=0; i<240; i++)
		{
		    R_Dat = 0x3F - (i/4);
			B_Dat = 0x1F - (i/8);
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}
// ------------------  蓝色渐亮条  --------------------
	R_Dat = 0;
	G_Dat = 0;
	B_Dat = 0;
	for (j=0; j<53; j++)  
	{
	    for (i=0; i<240; i++)
		{
		    B_Dat = i/8;
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	R_Dat = 0x1F;
	G_Dat = 0x3F;
	B_Dat = 0x1F;
	for (j=0; j<53; j++)  // 红色渐亮条
	{
	    for (i=0; i<240; i++)
		{
		    G_Dat = 0x3F - (i/4);
			R_Dat = 0x1F - (i/8);
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTBackLight()
* Description    : TFT背光开关
* EntryParameter : status  1 - 背光开；0 - 背光关
* ReturnValue    : NO
**************************************************************************************/
void TFTBackLight(uint8 status)
{
    if (status >= 1)
	{
	    TFT_BLK_ON();			                     // 背光开
	}
	else
	{
	    TFT_BLK_OFF();							    // 背光关
	}
}

/**************************************************************************************
* FunctionName   : TFTInit()
* Description    : 初始化TFT
* EntryParameter : color - 初始化颜色
* ReturnValue    : NO
**************************************************************************************/
void TFTInit(uint16 color)
{
    TFT_SetGPIO();					                     // 初始化控制端口
	TFT_WriteData(0xFFFF);
											    
	TFT_Reset();									     // 复位液晶模块
	TFT_Delay(15000);

    TFT_WriteRegister(0x0000,0x0001); TFT_Delay(15000);  //打开晶振
    TFT_WriteRegister(0x0003,0xA8A4); TFT_Delay(15000);  //0xA8A4
    TFT_WriteRegister(0x000C,0x0000); TFT_Delay(15000);   
    TFT_WriteRegister(0x000D,0x080C); TFT_Delay(15000);   
    TFT_WriteRegister(0x000E,0x2B00); TFT_Delay(15000);   
    TFT_WriteRegister(0x001E,0x00B0); TFT_Delay(15000);   
    TFT_WriteRegister(0x0001,0x2B3F); TFT_Delay(15000);  //驱动输出控制320*240  0x6B3F
    TFT_WriteRegister(0x0002,0x0600); TFT_Delay(15000);
    TFT_WriteRegister(0x0010,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0011,0x6070); TFT_Delay(15000);  //0x4030 定义数据格式  16位色 
    TFT_WriteRegister(0x0005,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0006,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0016,0xEF1C); TFT_Delay(15000);
    TFT_WriteRegister(0x0017,0x0003); TFT_Delay(15000);
    TFT_WriteRegister(0x0007,0x0233); TFT_Delay(15000);  //0x0233       
    TFT_WriteRegister(0x000B,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x000F,0x0000); TFT_Delay(15000);  //扫描开始地址
    TFT_WriteRegister(0x0041,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0042,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0048,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0049,0x013F); TFT_Delay(15000);
    TFT_WriteRegister(0x004A,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x004B,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0044,0xEF00); TFT_Delay(15000);
    TFT_WriteRegister(0x0045,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0046,0x013F); TFT_Delay(15000);
    TFT_WriteRegister(0x0030,0x0707); TFT_Delay(15000);
    TFT_WriteRegister(0x0031,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0032,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0033,0x0502); TFT_Delay(15000);
    TFT_WriteRegister(0x0034,0x0507); TFT_Delay(15000);
    TFT_WriteRegister(0x0035,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0036,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0037,0x0502); TFT_Delay(15000);
    TFT_WriteRegister(0x003A,0x0302); TFT_Delay(15000);
    TFT_WriteRegister(0x003B,0x0302); TFT_Delay(15000);
    TFT_WriteRegister(0x0023,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0024,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0025,0x8000); TFT_Delay(15000);
    TFT_WriteRegister(0x004F,0);        //行首址0
    TFT_WriteRegister(0x004E,0);        //列首址0

	TFT_Clear(color);
}

/**************************************************************************************
* FunctionName   : TFTGetPoint()
* Description    : 获取指定座标的颜色值
* EntryParameter : x - 行坐标；y - 列坐标
* ReturnValue    : 当前坐标颜色值
**************************************************************************************/
uint16 TFTGetPoint(uint16 x,uint16 y)
{
    TFT_SetCursor(x,y);
	return (TFT_BGR2RGB(TFT_ReadRegister(0x0022)));
}

/**************************************************************************************
* FunctionName   : TFTSetPoint()
* Description    : 在指定座标画点
* EntryParameter : x - 行坐标；y - 列坐标；color - 点的颜色
* ReturnValue    : NO
**************************************************************************************/
void TFTSetPoint(uint16 x,uint16 y,uint16 color)
{
    if ((x>240) || (y>320))
	{
	    return;
	}

	TFT_SetCursor(x,y);
	TFT_CS_CLR();

	TFT_WriteIndex(0x0022);
	TFT_RS_SET();

	TFT_WriteData(color);
	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTDrawLine()
* Description    : 在指定座标画线
* EntryParameter : 
* ReturnValue    : NO
**************************************************************************************/
void TFTDrawLine(uint16 startX,uint16 startY,uint8 endX,uint16 endY,uint16 color)
{
    int32   dx;                                        // 直线x轴差值变量
    int32   dy;                                        // 直线y轴差值变量
    int8    xDrc;                                      // x轴增长方向，为-1时减值方向，为1时增值方向
    int8    yDrc;                                      // y轴增长方向，为-1时减值方向，为1时增值方向
    int32   dx2;                                       // dx*2值变量，用于加快运算速度
    int32   dy2;                                       // dy*2值变量，用于加快运算速度
    int32   di;                                        // 决策变量
	uint16  x,y;

    dx = endX-startX;                                  // 求取两点之间的差值
    dy = endY-startY;
   
   /* 判断增长方向，或是否为水平线、垂直线、点 */
    if (dx > 0)		                                   // 增量方向
	{
	    xDrc = 1;                       
	}
	else
	{
	    if (dx < 0)		                               // 减量方向
		{
		    xDrc = -1;                
		}
		else		                                   // 垂直方向
		{
		    for (y=startY; y<endY; y++)
			{
			     TFTSetPoint(startX,y,color);
			} 
			return ;   
		}
	}

	if (dy > 0)		                                    // 增量方向
	{
	    yDrc = 1;                       
	}
	else
	{
	    if (dy < 0)		                                // 减量方向
		{
		    yDrc = -1;                
		}
		else		                                    // 水平方向
		{
		    for (x=startX; x<endX; x++)
			{
			     TFTSetPoint(x,startY,color);
			}  
			return ;   
		}
	}

	// 将dx、dy取绝对值
	dx = xDrc * dx;
	dy = yDrc * dy;
	// 计算2倍的dx及dy值
	dx2 = dx * 2;
	dy2 = dy * 2;

	// 使用Bresenham法进行划线
	if (dx >= dy)		                                 // 使用x轴为基准
	{
	    di = dy2 - dx;
		while (startX != endX)
		{
		     TFTSetPoint(startX,startY,color);
			 startX += xDrc;

			 if (di < 0)
			 {
			     di += dy2;                              // 计算出下一步的决策值   
			 }
			 else
			 {
			     di += (dy2 - dx2);
				 startY += yDrc; 
			 }

			 TFTSetPoint(startX,startY,color);
		}
	}
	else		                                         // 使用y轴为基准
	{
	    di = dx2 - dy;
		while (startY != endY)
		{
		     TFTSetPoint(startX,startY,color);
			 startY += yDrc;

			 if (di < 0)
			 {
			     di += dx2;                              // 计算出下一步的决策值   
			 }
			 else
			 {
			     di += (dx2 - dy2);
				 startX += xDrc; 
			 }

			 TFTSetPoint(startX,startY,color);
		}	
	}

}

/**************************************************************************************
*                                     End Of File                                                  
**************************************************************************************/
