/**************************************************************************************
*                       Crazy ARM LPC2148 V2.0 开发板实验程序                                                 
*                                      Address
*  
*                     (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ADS7843.h
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V4.02
* Description    :
*
*
**************************************************************************************/

#ifndef __xxx_H 
#define __xxx_H

/**************************************************************************************
*                               Pin Definition                                                
**************************************************************************************/
#define ADS_SCK          (17) 					        // P0.17	 时钟
#define ADS_DOUT         (18)					        // P0.18	 读端口
#define ADS_DIN          (19)					        // P0.19	 写端口
#define ADS_CS           (21)					        // P0.21	 片选
#define ADS_INT          (22)		                    // P1.22	 中断

#define ADS_SCK_DDR()    (IO0DIR) 				        // 
#define ADS_DOUT_DDR()   (IO0DIR)				        // 
#define ADS_DIN_DDR()    (IO0DIR)				        // 
#define ADS_CS_DDR()     (IO0DIR)				        // 
#define ADS_INT_DDR()    (IO1DIR)		                // 

#define ADS_SCK_SET()    (IO0SET |= (1<<ADS_SCK)) 
#define ADS_SCK_CLR()    (IO0CLR |= (1<<ADS_SCK))	
													
#define ADS_DIN_SET()    (IO0SET |= (1<<ADS_DIN)) 
#define ADS_DIN_CLR()    (IO0CLR |= (1<<ADS_DIN))	
													
#define ADS_CS_SET()     (IO0SET |= (1<<ADS_CS)) 
#define ADS_CS_CLR()     (IO0CLR |= (1<<ADS_CS))

#define ADS_DOUT_READ()  (IO0PIN & (1<<ADS_DOUT)) 
#define ADS_INT_READ()   (IO1PIN & (1<<ADS_INT)) 

/**************************************************************************************
*                               Macro Definition                                                 
**************************************************************************************/
// A/D 通道选择命令字和工作寄存器
#define	CHX 	         (0x90) 	                    // 通道Y+的选择控制字 0x94
#define	CHY 	         (0xD0)	                        // 通道X+的选择控制字 0xD4

/* 以下数据是通过多次读取触摸屏得出的 */
#define Tch_Xmax         (0x0F68)						// 触摸屏X轴最大输出
#define Tch_Xmin         (0x012C)						// 触摸屏X轴最小输出

#define Tch_Ymax         (0x0F03)						// 触摸屏Y轴最大输出
#define Tch_Ymin         (0x00C0) 						// 触摸屏Y轴最小输出

/* 以下数据为液晶的实际大小 */
#define Tch_LCDWidth     (320)							// LCD宽度
#define Tch_LCDHeight    (240)					   	    // LCD高度

/**************************************************************************************
*                               Internal Function                                                  
**************************************************************************************/
static void   ADS_Delay(uint8 t);                       // 延时消抖
static void   ADS_Start(void);							// 开始ISP总线
static uint16 ADS_Average(uint16 *p);                   // 求取平均值
static uint16 ADS_Read(void);					        // 读数据
static void   ADS_Wirte(uint8 dat);				        // 写数据
static uint32 ADS_Coordinate(void);                     // 返回坐标值

/**************************************************************************************
*                               Global Function                                                 
**************************************************************************************/
extern void   ADSInit(void);                            // 初始化ADC7843
extern void   ADSGetConversion(uint16 *px,uint16 *py);  // 获取转换坐标

/*************************************************************************************/
#endif

/**************************************************************************************
*                               End Of File                                                  
**************************************************************************************/
