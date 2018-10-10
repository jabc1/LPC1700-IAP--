/******************************************************************************************************** 
*                              
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : lcd1602.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
*                                  HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                          // 头文件

/********************************************************************************************************
* FunctionName   : LCD_Delay()
* Description    : LCD显示延时
* EntryParameter : us - 微秒
* ReturnValue    : None
********************************************************************************************************/
void LCD_Delay(u16 us)
{
    u8 i;

	while (us--)
	{
	    for (i=14; i>0; i--)	
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : LCD_SetGpio()
* Description    : 端口设置为GPIO，并设置为输出端口
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCD_SetGpio(void)
{
    LCD_DAT_OUT();						                                     // 数据端口方向设置

	LCD_EN_OUT();			                                                 // 使能
	LCD_RW_OUT();			                                                 // 读写
	LCD_RS_OUT();			                                                 // 数据/指令选择
	LCD_BK_OUT();			                                                 // 背光
}

/********************************************************************************************************
* FunctionName   : LCDBacklightCtrl()
* Description    : 背光开关
* EntryParameter : blk - 1开，0关
* ReturnValue    : None
********************************************************************************************************/
void LCDBacklightCtrl(u8 blk)
{
    LCD_BK_WRITE(blk);
}

/********************************************************************************************************
* FunctionName   : LCD_WriteData()
* Description    : 向LCD1602中写入数据
* EntryParameter : dat - 数据
* ReturnValue    : None
********************************************************************************************************/
void LCD_WriteData(u8 dat)
{
	LCD_Delay(500);                                                          // 没有忙状态判断标志，需延时         
	LCD_DAT_WRITE(dat);                                                      // 写入数据到LCD

	LCD_RS_WRITE(GPIO_SET);			                                         // 选择数据
	LCD_RW_WRITE(GPIO_CLR);			                                         // 写数据
	LCD_EN_WRITE(GPIO_CLR);						                             // 若晶振速度太高可以在这后加小的延时
	LCD_Delay(500);
	LCD_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : WriteCommand()
* Description    : 向LCD1602中写入指令
* EntryParameter : cmd - 指令
* ReturnValue    : None
********************************************************************************************************/
void LCD_WriteCommand(u8 cmd) 
{
	LCD_Delay(500);                                                          // 没有忙状态判断标志，需延时
	LCD_DAT_WRITE(cmd);                                                      // 写入指令到LCD

	LCD_RS_WRITE(GPIO_CLR);			                                         // 选择指令
	LCD_RW_WRITE(GPIO_CLR);			                                         // 写数据
	LCD_EN_WRITE(GPIO_CLR);						                             // 若晶振速度太高可以在这后加小的延时
	LCD_Delay(500);
	LCD_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : LCDInit()
* Description    : LCD在工作前先要对显示屏初始化,否则模块无法正常工作
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCDInit(void) 
{
    LCD_SetGpio();			                                                 // 设置端口方向

	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);                               // 三次显示模式设置，不检测忙信号
	LCD_Delay(500); 
	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);                               // 0x38指令表示:8位数据显示模式,两行都显示
	LCD_Delay(500); 
	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);
	LCD_Delay(500); 

	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);                               // 显示模式设置,开始要求每次检测忙信号
	LCD_WriteCommand(LCD_DISPLAY_OFF);                                       // 关闭显示
	LCD_WriteCommand(LCD_CLEAR_SCREEN);                                      // 显示清屏
	LCD_WriteCommand(LCD_AC_AUTO_INCREMENT);                                 // 显示光标移动设置
	LCD_WriteCommand(LCD_DISPLAY_ON);                                        // 显示开及光标设置

	LCDBacklightCtrl(LCD_BLK_ON);									         // 开背光
}

/********************************************************************************************************
* FunctionName   : LCD_DisplayChar()
* Description    : 按指定坐标中写入数据
* EntryParameter : x - 横坐标, y - 纵坐标, ch - 为ASCII值
* ReturnValue    : 正确返回0，否则返回1
********************************************************************************************************/
u8 LCDDisplayChar(u8 x, u8 y, u8 ch)
{
	if ((y > LCD_LINE_MAX) || (x > LCD_LINE_LENGTH))						 // x,y坐标溢出
	{
	    return 1;
	}

	x |= ((y == 1) ? LCD_LINE2_HEAD : LCD_LINE1_HEAD);						 // 显示坐标
	LCD_WriteCommand(x);                                                     // 这里不检测忙信号，发送地址码
	LCD_WriteData(ch);

	return 0;
}

/********************************************************************************************************
* FunctionName   : LCD_DisplayStr()
* Description    : 向指定坐标中写入字符串（有自动换行显示功能）
* EntryParameter : x - x坐标(0~15), y - y坐标(0~1), *pStr - 显示字符串
* ReturnValue    : 正确返回0，错误返回1
********************************************************************************************************/
u8 LCDDisplayStr(u8 x, u8 y, u8 *pStr)
{
    u8 i;
	uc8 lineAddr[] = {LCD_LINE1_HEAD, LCD_LINE2_HEAD};

	if ((y >= LCD_LINE_MAX) || (x >= LCD_LINE_LENGTH))						 // x, y坐标溢出
	{
	    return 1;
	}

	LCD_WriteCommand(lineAddr[y] + x);                                       // 写入地址

	for (i=x; ((i<LCD_LINE_LENGTH) && (*pStr!='\0')); i++)
	{
 		LCD_WriteData(*(pStr++));                                            // 写入数据
	}

	if (*pStr != '\0')                                                       // 判断数据是否写完
	{
 		x = 0;                                                               // 没写完写入第2行
		if (++y < LCD_LINE_MAX)
		{
		    LCDDisplayStr(x, y, pStr);   									 // 下一行继续显示(递归调用)
		} 
	}

	return 0;
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/


