/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : ssp.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef __SSP_H 
#define __SSP_H

/********************************************************************************************************
* Pin Definition                                                 
********************************************************************************************************/
#define SSP_SCK0_SELECT       (0x00)						                  // 时钟管脚选择：0x00 - P0_15, 0x01 - P1_20
#define SSP_MISO0_SELECT      (0x00)						                  // 时钟管脚选择：0x00 - P0_17, 0x01 - P1_23
#define SSP_MOSI0_SELECT      (0x00)						                  // 时钟管脚选择：0x00 - P0_18, 0x01 - P1_24

#define SSP_SCK1_SELECT       (0x00)						                  // 时钟管脚选择：0x00 - P0_7, 0x01 - P1_31

// ---------------------------- SSP0 --------------------------------------------------
#if (SSP_SCK0_SELECT == 0x00)
#define SSP_SCK0_FUN          (PIO0_15_SCK0)    			                  // P0.15 时钟
#define SSP_SCK0_PORT         (PORT0)						                  // 端口  
#define SSP_SCK0_PIN          (15)							                  // 管脚
#else
#define SSP_SCK0_FUN          (PIO1_20_SCK0)    			                  // P1.20 时钟
#define SSP_SCK0_PORT         (PORT1)						                  // 端口  
#define SSP_SCK0_PIN          (20)
#endif

#if (SSP_MISO0_SELECT == 0x00)
#define SSP_MISO0_FUN         (PIO0_17_MISO0)    			                  // P0.17 数据输入
#define SSP_MISO0_PORT        (PORT0)						                  // 端口  
#define SSP_MISO0_PIN         (17)							                  // 管脚
#else
#define SSP_MISO0_FUN         (PIO1_23_MISO0)    			                  // P1.23 数据输入
#define SSP_MISO0_PORT        (PORT1)						                  // 端口  
#define SSP_MISO0_PIN         (23)							                  // 管脚
#endif

#if (SSP_MOSI0_SELECT == 0x00)
#define SSP_MOSI0_FUN         (PIO0_18_MOSI0)    			                  // P0.18 数据输出
#define SSP_MOSI0_PORT        (PORT0)						                  // 端口  
#define SSP_MOSI0_PIN         (18)							                  // 管脚
#else
#define SSP_MOSI0_FUN         (PIO1_24_MOSI0)    			                  // P1.24 数据输出
#define SSP_MOSI0_PORT        (PORT1)						                  // 端口  
#define SSP_MOSI0_PIN         (24)							                  // 管脚
#endif

// ---------------------------- SSP1 --------------------------------------------------
#if (SSP_SCK1_SELECT == 0x00)
#define SSP_SCK1_FUN          (PIO0_7_SCK1)    			                      // P0.7 时钟
#define SSP_SCK1_PORT         (PORT0)						                  // 端口  
#define SSP_SCK1_PIN          (7)							                  // 管脚
#else
#define SSP_SCK1_FUN          (PIO1_31_SCK1)    			                  // P1.31 时钟
#define SSP_SCK1_PORT         (PORT1)						                  // 端口  
#define SSP_SCK1_PIN          (31)							                  // 管脚
#endif

#define SSP_MISO1_FUN         (PIO0_8_MISO1)    			                  // P0.8 数据输入
#define SSP_MISO1_PORT        (PORT0)						                  // 端口  
#define SSP_MISO1_PIN         (8)							                  // 管脚

#define SSP_MOSI1_FUN         (PIO0_9_MOSI1)    			                  // P0.9 数据输出
#define SSP_MOSI1_PORT        (PORT0)						                  // 端口  
#define SSP_MOSI1_PIN         (9)							                  // 管脚

/******************************************************************************************************** 
* 【枚举变量】 Enumerations                                                
********************************************************************************************************/
typedef enum
{
    SSP0 = 0,
    SSP1 = 1,

} SSP_NUM;																	  // SSP

/******************************************************************************************************** 
* 【寄存器】 Register                                              
********************************************************************************************************/

// 控制寄存器（CR0-0x4008 8000）
#define CR0_DSS			(0x00)												  // 数据长度选择
#define CR0_FRF			(0x04)												  // 帧格式
#define CR0_SPO			(0x06)												  // 时钟输出极性
#define CR0_SPH			(0x07)												  // 时钟输出相位
#define CR0_SCR			(0x08)												  // 控制SSP通信的位速率

// 控制寄存器（CR1-0x4008 8004）
#define CR1_LBM			(0x00)												  // 回写模式
#define CR1_SSE			(0x01)												  // SSP使能
#define CR1_MS			(0x02)												  // 主机/从机模式
#define CR1_SOD			(0x03)												  // 从机输出禁能

// 中断清除寄存器（ICR-0x4008 8020）
#define ICR_RORIC		(0x01 << 0)											  // 向该位写"1"来清除接收溢出中断
#define ICR_RTIC		(0x01 << 1)											  // 向该位写"1"来清除超时中断

// 状态寄存器（SR – 0x4008 800C）
#define SR_TFE		    (0x01 << 0)							                  // 发送FIFO空
#define SR_TNF		    (0x01 << 1) 							              // 发送FIFO未满
#define SR_RNE		    (0x01 << 2)							                  // 接收FIFO不为空
#define SR_RFF	  	    (0x01 << 3) 							              // 接收FIFO满
#define SR_BSY		    (0x01 << 4)							                  // 忙

// 中断使能置位/清零寄存器（IMSC – 0x4008 8014）
#define IMSC_RORIM	    (0x01 << 0)							                  // 软件置位该位来使能接收溢出中断
#define IMSC_RTIM	    (0x01 << 1)							                  // 软件置位该位来使能接收超时中断
#define IMSC_RXIM	    (0x01 << 2)							                  // 使得当Rx FIFO至少有一半为满时触发中断
#define IMSC_TXIM  	    (0x01 << 3)							                  // 使得当Tx FIFO至少有一半为空时触发中断

// 原始中断寄存器（RIS – 0x4008 8018）
#define RIS_RORRIS	    (0x01 << 0)							                  // 当Rx FIFO满时又接收到另一帧数据时，该位置位
#define RIS_RTRIS	    (0x01 << 1)							                  // 如果Rx FIFO不为空，且在“超时周期”中没有被读出时，该位置位
#define RIS_RXRIS	    (0x01 << 2)							                  // 当Rx FIFO至少有一半为满时，该位置位
#define RIS_TXRIS	    (0x01 << 3)							                  // 当Tx FIFO至少有一半为空时，该位置位

// 使能中断寄存器位描述（MIS – 0x4008 801C）
#define MIS_RORMIS	    (0x01 << 0)							                  // 当Rx FIFO满时又接收到另外一帧数据，且中断被使能时，该位置位
#define MIS_RTMIS	    (0x01 << 1)							                  // 如果Rx FIFO不为空并在“超时周期”中没有被读出，且中断被使能时，该位置位
#define MIS_RXMIS	    (0x01 << 2)							                  // 当Rx FIFO至少有一半为满，且中断被使能时，该位置位
#define MIS_TXMIS	    (0x01 << 3)							                  // 当Tx FIFO至少有一半为空，且中断被使能时，该位置位

/********************************************************************************************************
* Internal Function                                                  
********************************************************************************************************/
static void SSP_SetGpio(u8 spiNum);		                                      // 端口初始化

/********************************************************************************************************
* Global Function                                                 
********************************************************************************************************/
extern u8   SSPInit(u8 sspNum, u8 clk);			                              // 初始化SSP
extern u8   SSPSendByte(u8 sspNum, u8 dat);			                          // 发送一字节数据
extern u8   SSPRcvByte(u8 sspNum);					 	                      // 发送一字节数据
extern u8   SSPSendRcvByte(u8 sspNum, u8 dat);  	                          // SSP数据收发
extern void SSPSetSpeed(u8 sspNum, u8 clk);		                              // SSP速度设置

/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
