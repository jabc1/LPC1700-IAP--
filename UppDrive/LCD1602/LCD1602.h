/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : lcd1602.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

#ifndef __LCD1602_H
#define __LCD1602_H

/********************************************************************************************************
* Pin Definition                                                 
********************************************************************************************************/
#define LCD_D0_FUN                (PIO1_19_GPIO)    				        // P1.19     
#define LCD_D0_PORT       	      (PORT1)							        // 端口 
#define LCD_D0_PIN       	      (19)								        // 管脚

#define LCD_D1_FUN                (PIO1_20_GPIO)    				        // P1.20     
#define LCD_D1_PORT       	      (PORT1)							        // 端口 
#define LCD_D1_PIN       	      (20)								        // 管脚

#define LCD_D2_FUN                (PIO1_21_GPIO)    				        // P1.21     
#define LCD_D2_PORT       	      (PORT1)							        // 端口 
#define LCD_D2_PIN       	      (21)								        // 管脚

#define LCD_D3_FUN                (PIO1_22_GPIO)    				        // P1.22     
#define LCD_D3_PORT       	      (PORT1)							        // 端口 
#define LCD_D3_PIN       	      (22)								        // 管脚

#define LCD_D4_FUN                (PIO1_23_GPIO)    				        // P1.23     
#define LCD_D4_PORT       	      (PORT1)							        // 端口 
#define LCD_D4_PIN       	      (23)								        // 管脚

#define LCD_D5_FUN                (PIO1_24_GPIO)    				        // P1.24     
#define LCD_D5_PORT       	      (PORT1)							        // 端口 
#define LCD_D5_PIN       	      (24)								        // 管脚

#define LCD_D6_FUN                (PIO1_25_GPIO)    				        // P1.25     
#define LCD_D6_PORT       	      (PORT1)							        // 端口 
#define LCD_D6_PIN       	      (25)								        // 管脚

#define LCD_D7_FUN                (PIO1_26_GPIO)    				        // P1.26     
#define LCD_D7_PORT       	      (PORT1)							        // 端口 
#define LCD_D7_PIN       	      (26)								        // 管脚

#define LCD_DAT_PORT       	      (PORT1)							        // 端口  
#define LCD_DAT_PIN       	      (19)						                // 数据管脚起始
#define LCD_DAT       	          (0xFF << LCD_DAT_PIN)					    // 管脚 P1.19~P1.26

//-------------------------------------------------------------------------------------------------------

#define LCD_EN_FUN                (PIO1_27_GPIO)    				        // P1.27 
#define LCD_EN_PORT       	      (PORT1)							        // 端口  
#define LCD_EN_PIN       	      (27)								        // 管脚

#define LCD_RW_FUN                (PIO1_28_GPIO)    				        // P1.28 
#define LCD_RW_PORT       	      (PORT1)							        // 端口  
#define LCD_RW_PIN       	      (28)								        // 管脚

#define LCD_RS_FUN                (PIO1_29_GPIO)    				        // P1.29 
#define LCD_RS_PORT       	      (PORT1)							        // 端口  
#define LCD_RS_PIN       	      (29)								        // 管脚

#define LCD_BK_FUN                (PIO3_26_GPIO)    				        // P3.26 
#define LCD_BK_PORT       	      (PORT3)							        // 端口  
#define LCD_BK_PIN       	      (26)								        // 管脚

/********************************************************************************************************
* 【管脚方向】 Pin direction                                                
********************************************************************************************************/
#define LCD_DAT_OUT()             GPIOSetMultiDir(LCD_DAT_PORT, LCD_DAT, GPIO_OUT)	 // 设置端口方向

#define LCD_EN_OUT()              GPIOSetDir(LCD_EN_PORT, LCD_EN_PIN, GPIO_OUT)  	 // 使能
#define LCD_RW_OUT()              GPIOSetDir(LCD_RW_PORT, LCD_RW_PIN, GPIO_OUT)	     // 读写
#define LCD_RS_OUT()              GPIOSetDir(LCD_RS_PORT, LCD_RS_PIN, GPIO_OUT)	     // 数据/指令选择
#define LCD_BK_OUT()              GPIOSetDir(LCD_BK_PORT, LCD_BK_PIN, GPIO_OUT)	     // 背光

/********************************************************************************************************
* 【管脚写】 Pin Write                                                
********************************************************************************************************/
#define LCD_DAT_WRITE(dat)	      GPIOSetMultiValue(LCD_DAT_PORT, LCD_DAT, dat<<LCD_DAT_PIN)	 

#define LCD_EN_WRITE(dat)         GPIOSetValue(LCD_EN_PORT, LCD_EN_PIN, dat)	     // 端口输出
#define LCD_RW_WRITE(dat)         GPIOSetValue(LCD_RW_PORT, LCD_RW_PIN, dat)
#define LCD_RS_WRITE(dat)         GPIOSetValue(LCD_RS_PORT, LCD_RS_PIN, dat)	    
#define LCD_BK_WRITE(dat)         GPIOSetValue(LCD_BK_PORT, LCD_BK_PIN, dat)         

/********************************************************************************************************
*                               MacroDefinition                                                 
********************************************************************************************************/
// 输入方式设置
#define LCD_AC_AUTO_INCREMENT     (0x06)                                    // 数据读、写操作后,AC自动增一
#define LCD_AC_AUTO_DECREASE      (0x04)                                    // 数据读、写操作后,AC自动减一
#define LCD_MOVE_ENABLE           (0x05)                                    // 数据读、写操作,画面平移
#define LCD_MOVE_DISENABLE        (0x04)                                    // 数据读、写操作,画面不动
#define LCD_GO_HOME               (0x02)                                    // AC=0,光标、画面回HOME位

// 设置显示、光标及闪烁开、关
#define LCD_DISPLAY_ON            (0x0C)                                    // 显示开
#define LCD_DISPLAY_OFF           (0x08)                                    // 显示关
#define LCD_CURSOR_ON             (0x0A)                                    // 光标显示
#define LCD_CURSOR_OFF            (0x08)                                    // 光标不显示
#define LCD_CURSOR_BLINK_ON       (0x09)                                    // 光标闪烁
#define LCD_CURSOR_BLINK_OFF      (0x08)                                    // 光标不闪烁

// 光标、画面移动,不影响DDRAM
#define LCD_LEFT_MOVE             (0x18)                                    // LCD显示左移一位
#define LCD_RIGHT_MOVE            (0x1C)                                    // LCD显示右移一位
#define LCD_CURSOR_LEFT_MOVE      (0x10)                                    // 光标左移一位
#define LCD_CURSOR_RIGHT_MOVE     (0x14)                                    // 光标右移一位

// 工作方式设置
#define LCD_DISPLAY_DOUBLE_LINE   (0x38)                                    // 两行显示
#define LCD_DISPLAY_SINGLE_LINE   (0x30)                                    // 单行显示
#define LCD_CLEAR_SCREEN          (0x01)                                    // 清屏
											   
/************************* LCD1602地址相关 *************************************************************/
#define LCD_LINE1_HEAD            (0x80)                                    // 第一行DDRAM起始地址 
#define LCD_LINE2_HEAD            (0xC0)                                    // 第二行DDRAM起始地址
#define LCD_LINE1                 (0x00)                                    // 第一行
#define LCD_LINE2                 (0x01)                                    // 第二行

#define LCD_LINE_MAX              (2)                                       // 最大行数
#define LCD_LINE_LENGTH			  (16)										// 每行最多显示16个字符

#define LCD_BLK_ON				  (0x01)									// 背光开
#define LCD_BLK_OFF				  (0x00)									// 背光关

/********************************************************************************************************
*                               InternalFunction                                                  
********************************************************************************************************/
static void LCD_Delay(u16 us);								                // 液晶延时，代替读状态字
static void LCD_WriteCommand(u8 cmd);					                    // 写指令
static void LCD_WriteData(u8 dat);                                          // 写数据

/********************************************************************************************************
*                               GlobalFunction                                                 
********************************************************************************************************/
extern void LCDInit(void);	                                                // 初始化LCD
extern u8   LCDDisplayChar(u8 x, u8 y, u8 ch);	                            // 显示单个字符
extern u8   LCDDisplayStr(u8 x, u8 y, u8 *pStr);	                        // 显示字符串

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                               End Of File                                                  
********************************************************************************************************/
