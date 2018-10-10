/**************************************************************************************    
*                       Crazy ARM LPC2148 V2.0 开发板实验程序                                                 
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : SSD1289.h
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V3.70
* Description    :
*
*
**************************************************************************************/

#ifndef __SSD1289_H 
#define __SSD1289_H

/**************************************************************************************
*                               PinDefinition                                                 
**************************************************************************************/
/*   0 - 8位模式，1 - 16位模式   */
#define	  TFT_DATA_MODE	  (1)							   // 16位模式还是8位模式

/**************************************************************************************
*                               PinDefinition                                                 
**************************************************************************************/
#define   TFT_CS          (16)                             // P1.16 片选
#define   TFT_RS          (17)                             // P1.17 指令/数据选择 0 - 指令；1 - 数据
#define   TFT_WR          (18)                             // P1.18
#define   TFT_RD          (19)                             // P1.19
#define   TFT_LE          (23)                             // P1.23 锁存
#define   TFT_RST         (20)                             // P1.20 复位
#define   TFT_BLK         (21)                             // P1.21 背光控制	
#define   TFT_DAT         (0xFFFF)                         // P0.0~P0.15

#define   TFT_CS_DDR()    (IO1DIR)						   // 方向控制
#define   TFT_RS_DDR()    (IO1DIR)
#define   TFT_WR_DDR()    (IO1DIR)
#define   TFT_RD_DDR()    (IO1DIR)
#define   TFT_LE_DDR()    (IO1DIR)
#define   TFT_RST_DDR()   (IO1DIR)
#define   TFT_BLK_DDR()   (IO1DIR)
#define   TFT_DAT_DDR()   (IO0DIR)
										  
#define   TFT_CS_SET()    (IO1SET |= (1<<TFT_CS)) 
#define   TFT_CS_CLR()    (IO1CLR |= (1<<TFT_CS))

#define   TFT_RS_SET()    (IO1SET |= (1<<TFT_RS)) 
#define   TFT_RS_CLR()    (IO1CLR |= (1<<TFT_RS))

#define   TFT_WR_SET()    (IO1SET |= (1<<TFT_WR)) 
#define   TFT_WR_CLR()    (IO1CLR |= (1<<TFT_WR))

#define   TFT_RD_SET()    (IO1SET |= (1<<TFT_RD)) 
#define   TFT_RD_CLR()    (IO1CLR |= (1<<TFT_RD))

#define   TFT_LE_SET()    (IO1SET |= (1<<TFT_LE)) 
#define   TFT_LE_CLR()    (IO1CLR |= (1<<TFT_LE))

#define   TFT_RST_SET()   (IO1SET |= (1<<TFT_RST)) 
#define   TFT_RST_CLR()   (IO1CLR |= (1<<TFT_RST))

#define   TFT_BLK_ON()    (IO1SET |= (1<<TFT_BLK)) 		    // 背光开
#define   TFT_BLK_OFF()   (IO1CLR |= (1<<TFT_BLK))			// 背光关

//        ******    写数据16位
#define   TFT_WRITE_WORD_DATA(dat)     	\
          {						  	    \
		      IO0CLR = TFT_DAT;		    \
			  IO0SET = dat;			    \
		  }

//        ******    写数据8位
#define   TFT_WRITE_BYTE_DATA(dat)     	\
          {						  	    \
		      IO0CLR = 0xFF00;		    \
			  IO0SET = dat;			    \
		  }			  	

#define   TFT_READ_WORD_DATA()  (IO0PIN & 0xFFFF)    			// 读16位数据
#define   TFT_READ_BYTE_DATA()  (IO0PIN & 0xFF00)    			// 读8位数据	 

/**************************************************************************************
*                               InternalFunction                                                  
**************************************************************************************/
#define   RED             (0xF800)						    // 红色
#define   GREEN           (0x03E0)						    // 绿色
#define   BLUE            (0x001F) 					        // 蓝色
#define   WRITE           (0xFFFF)						    // 白色
#define   BLACK           (0x0000)						    // 黑色
#define	  ORANGE          (0xF860)					        // 橙色
#define	  YELLOW		  (0xFFE0)						    // 黄色
#define	  CYAN			  (0x07FF)						    // 青色
#define	  PROPLE      	  (0xF81F)						    // 紫色
#define   NOCOLOR         (0x0001)            			    // 无色

#define   WIDTH			  (320)						        // 屏宽
#define   HEIGTH		  (240)						        // 屏高

/**************************************************************************************
*                               InternalFunction                                                  
**************************************************************************************/
static void   TFT_SetGPIO(void);				            // 设置端口方向
static void   TFT_Delay(uint32 nCount); 	                // 液晶延时
static uint16 TFT_CheckContronller(void);	                // 
static void   TFT_Reset(void);					            // 复位液晶
static void   TFT_WriteRegister(uint16 index,uint16 dat);   // 写寄存器
static uint16 TFT_ReadRegister(uint16 index);			    // 读寄存器
static uint16 TFT_ReadData(void);						    // 写数据
static void   TFT_WriteData(uint16 dat);				    // 读数据
static void   TFT_SetCursor(uint16 x,uint16 y);			    // 设置坐标
static void   TFT_Clear(uint16 dat);					    // 清屏
static uint16 TFT_BGR2RGB(uint16 c);					    // 颜色转换
static void   TFT_WriteIndex(uint16 idx);				    // 写地址
static void   TFT_SetWindows(uint16 startX,uint16 startY,uint16 endX,uint16 endY);	         // 设置窗口
static void   TFT_PutChar(uint16 x,uint16 y,uint8 *asciiCode,uint16 charColor,uint16 bkColor);// 显示单个字符
static void   TFT_PutChinese(uint16 x,uint16 y,uint8* ptr,uint16 charColor,uint16 bkColor);	 // 显示中文

/**************************************************************************************
*                               GlobalFunction                                                 
**************************************************************************************/
extern void   TFTInit(uint16 color);                        // 初始化TFT
extern void   TFTTest(void);                                // 测试TFT
extern void   TFTBackLight(uint8 status);                	// 背光控制
extern uint16 TFTGetPoint(uint16 x,uint16 y);               // 获取指定坐标颜色值
extern void   TFTSetPoint(uint16 x,uint16 y,uint16 color);	// 指定坐标画点
extern void   TFTPutStr(uint16 x,uint16 y,uint8 *ptr,uint16 charColor,uint16 bkColor);  // 显示字符串
extern void   TFTDrawPicture(uint16 startX,uint16 startY,uint16 endX,uint16 endY,uint8 const *pic);	// 显示图片
extern void   TFTDrawLine(uint16 startX,uint16 startY,uint8 endX,uint16 endY,uint16 color);         // 画线

/*************************************************************************************/
#endif

/**************************************************************************************
*                               End Of File                                                  
**************************************************************************************/
