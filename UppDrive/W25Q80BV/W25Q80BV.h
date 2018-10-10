/******************************************************************************************************** 
*                           
*                                    Address
*  							    
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : W25Q80BV.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : W25Q80BV 1 Mbit(1M x 8) Serial Flash Memory
*
*
********************************************************************************************************/

#ifndef __W25Q80BV_H 
#define __W25Q80BV_H

/********************************************************************************************************
* Pin Definition                                                 
********************************************************************************************************/
// 片选
#define W25Q_CS_FUN            (PIO1_0_GPIO)                               // P1.0 片选   
#define W25Q_CS_PORT       	   (PORT1)							           // 端口
#define W25Q_CS_PIN       	   (0)								           // 管脚

/* 本代码采用SSP总线，已经在SSP文件中定义，此处不需要再定义
#define W25Q_SCK_FUN           (PIO0_7_GPIO)                               // P0.7 时钟   
#define W25Q_SCK_PORT          (PORT0)							           // 端口
#define W25Q_SCK_PIN       	   (7)								           // 管脚

#define W25Q_MISO_FUN          (PIO0_8_GPIO)                               // P0.8 数据输入   
#define W25Q_MISO_PORT         (PORT0)							           // 端口
#define W25Q_MISO_PIN          (8)							 	           // 管脚

#define W25Q_MOSI_FUN          (PIO0_9_GPIO)                               // P0.9 数据输出  
#define W25Q_MOSI_PORT         (PORT0)							           // 端口
#define W25Q_MOSI_PIN          (9)								           // 管脚	*/

/********************************************************************************************************
* 【管脚方向】 Pin direction                                                
********************************************************************************************************/
#define W25Q_CS_OUT()          GPIOSetDir(W25Q_CS_PORT, W25Q_CS_PIN, GPIO_OUT)	 

/********************************************************************************************************
* 【管脚写】 Pin Write                                                
********************************************************************************************************/
#define W25Q_CS_WRITE(sw)      GPIOSetValue(W25Q_CS_PORT, W25Q_CS_PIN, sw)	    

/********************************************************************************************************
* 【宏定义】 macro definition                                               
********************************************************************************************************/
#define  W25Q_READ_ONLY	       (0x00)  	                                   // 0:Read/Write or 1:Read only 
#define  W25Q_BUF_SIZE		   (4096)								       // 扇区缓冲

#define  W25Q_CN_START_ADDR	   (0x0C0000)						           //	汉字起始地址

/********************************************************************************************************
* 【命令集】Command set                                                 
********************************************************************************************************/
#define  W25Q_EWSR             (0x50)                                      // 使能写状态寄存器命令
#define  W25Q_RDSR             (0x05)                                      // 读状态寄存器命令
#define  W25Q_WRSR             (0x01)                                      // 写状态寄存器命令
#define  W25Q_WREN             (0x06)                                      // 写使能命令
#define  W25Q_WRDI             (0x04)                                      // 写禁止命令
#define  W25Q_READ             (0x03)                                      // 读数据命令
#define  W25Q_HIGH_SPEED_READ  (0x0B)                                      // 高速数据读(地址自动增加)命令
#define  W25Q_CHIP_ERASE       (0x60)                                      // 0xC7 芯片擦除命令
#define  W25Q_BLOCK_ERASE      (0x52)                                      // 块擦除命令
#define  W25Q_SECTOR_ERASE     (0x20)                                      // 扇区擦除命令
#define  W25Q_AAI_PROGRAM      (0xAF)                                      // 字节编程(地址自动增加)命令
#define  W25Q_BYTE_PROGRAM     (0x02)                                      // 字节编程命令  
#define  W25Q_READ_ID          (0x90)                                      // 读器件ID
#define  W25Q_READ_ID2         (0xAB)                                      // 读器件ID(功能完全同READ_ID)
#define  W25Q_POW_DOWN		   (0xB9) 								       // 掉电模式
#define  W25Q_RLS_POW_DOWN     (0xAB) 								       // 恢复

/********************************************************************************************************
* 【外部函数】 SPI function                                                 
********************************************************************************************************/
#define W25Q_WriteByte(dat)	   (SSPSendRcvByte(SSP1, dat) )		           // 写一字节数据
#define W25Q_ReadByte()	       (SSPSendRcvByte(SSP1, 0xFF))	               // 读一字节数据

/********************************************************************************************************
* 【内部函数】 internal function                                              
********************************************************************************************************/
#if !W25Q_READ_ONLY
static u8   W25Q_ReadStatusReg(void);	                                   // 读状态寄存器
static void W25Q_EnableWriteReg(void);	                                   // 使能写状态寄存器
static void W25Q_WriteStatusReg(u8 dat);                                   // 写状态寄存器 
static void W25Q_EnableWrite(void);			                               // 写使能寄存器
static void W25Q_DisableWrite(void);		                               // 写禁止寄存器
static void W25Q_WaitBusy(void);										   // 判断忙状态
#endif

static void W25Q_WriteAddr(u32 addr);								       // 写地址

/********************************************************************************************************
* 【全局函数】 global function                                                 
********************************************************************************************************/
extern void W25QInit(void);				                                   // 初始化IC
extern u8   W25QReadDeviceID(u8 idAddr);                                   // 读厂商/器件ID
extern u8   W25QReadByte(u32 addr);		                                   // 在指定地址读取一个字节数据
extern u8   W25QHighSpeedReadByte(u32 addr);                               // 高速读一字节
extern void W25QReadNByte(u32 addr, u8 *buf, u32 len);                     // 多字节读取
extern void W25QHighSpeedReadNByte(u32 addr, u8 *buf, u32 len);            // 高速连续读数据

extern void W25QPowerDown(void); 										   // 掉电模式
extern void W25QWakeUp(void);											   // 退出掉电

#if !W25Q_READ_ONLY													       // 写相关函数
extern void W25QWriteByte(u32 addr, u8 dat);						       // 写入一字节数据
extern void W25QWriteNByte(u32 addr, u8 *buf, u32 len);		               // 写入多个字节
extern void W25QWriteNByteSecEra(u32 addr, u8 *buf, u32 len);	           // 带扇区擦除功能

extern void W25QWriteNByteEra(u32 addr, u8 *buf, u32 len);			       // 多字节写，带擦出功能

extern void W25QChipErase(void);										   // 芯片擦除
extern void W25QSectorErase(u32 sectorAddr);						       // 扇区擦除(1个扇区4Kbyte)
extern void W25QBlockErase(u32 blockAddr); 							       // 块擦除(1个块32Kbyte)
#endif

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
