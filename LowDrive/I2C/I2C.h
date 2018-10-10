/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : i2c.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*				   使用寄存器地址实现多总线控制，从而减小程序代码空间
*
********************************************************************************************************/

#ifndef __I2C_H 
#define __I2C_H

/********************************************************************************************************
* 【管脚定义】 pin definition                                                
********************************************************************************************************/
#define I2C1_IO_SELECT		         (0x01)						                      // 0x00 - 选择P0.0和P0.1; 0x01 - 选择P0.19和P0.20;

// ---------------------------- I2C0 -------------------------------------------------
#define I2C_SDA0_FUN                 (PIO0_27_SDA0)    			                      // P0.27 数据输入
#define I2C_SDA0_PORT                (PORT0)						                  // 端口  
#define I2C_SDA0_PIN       	         (27)							                  // 管脚

#define I2C_SCL0_FUN                 (PIO0_28_SCL0)    			                      // P0.28 时钟输出
#define I2C_SCL0_PORT                (PORT0)						                  // 端口  
#define I2C_SCL0_PIN       	         (28)							                  // 管脚

// ---------------------------- I2C1 -------------------------------------------------
#if (I2C1_IO_SELECT == 0x00)
#define I2C_SDA1_FUN                 (PIO0_0_SDA1)    			                      // P0.0 数据输入 
#define I2C_SDA1_PORT                (PORT0)						                  // 端口 
#define I2C_SDA1_PIN       	         (0)							                  // 管脚

#define I2C_SCL1_FUN                 (PIO0_1_SCL1)    			                      // P0.1 时钟输出
#define I2C_SCL1_PORT                (PORT0)						                  // 端口  
#define I2C_SCL1_PIN       	         (1)							                  // 管脚

#else
#define I2C_SDA1_FUN                 (PIO0_19_SDA1)    			                      // P0.19 数据输入
#define I2C_SDA1_PORT                (PORT0)						                  // 端口  
#define I2C_SDA1_PIN       	         (19)							                  // 管脚

#define I2C_SCL1_FUN                 (PIO0_20_SCL1)    			                      // P0.20 时钟输出
#define I2C_SCL1_PORT                (PORT0)						                  // 端口 
#define I2C_SCL1_PIN       	         (20)							                  // 管脚
#endif

// ---------------------------- I2C2 -------------------------------------------------
#define I2C_SDA2_FUN                 (PIO0_10_SDA2)    			                      // P0.10 数据输入
#define I2C_SDA2_PORT                (PORT0)						                  // 端口  
#define I2C_SDA2_PIN       	         (10)							                  // 管脚

#define I2C_SCL2_FUN                 (PIO0_11_SCL2)    			                      // P0.11 时钟输出 
#define I2C_SCL2_PORT                (PORT0)						                  // 端口 
#define I2C_SCL2_PIN       	         (11)							                  // 管脚

/******************************************************************************************************** 
* 【枚举变量】 Enumerations                                                
********************************************************************************************************/
typedef enum
{
    I2C0 = 0,
	I2C1 = 1,
	I2C2 = 2,

} I2C_NUM;																			  // I2C接口

/******************************************************************************************************** 
* 【寄存器】 Register                                              
********************************************************************************************************/

// 控制置位寄存器（I2CONSET-0x4001 C000）
#define I2CONSET_AA			         (0x01 << 2)									  // 应答标志
#define I2CONSET_SI		         	 (0x01 << 3)									  // I2C中断标志
#define I2CONSET_STO		         (0x01 << 4)									  // 停止标志
#define I2CONSET_STA		         (0x01 << 5)									  // 起始标志
#define I2CONSET_I2EN		         (0x01 << 6)                                      // I2C接口使能

// 清零寄存器（I2CONCLR-0x4001 C018）
#define I2CONCLR_AAC		         (0x01 << 2)                                      // 应答标志清零位
#define I2CONCLR_SIC		         (0x01 << 3)                                      // I2C中断标志清零位
#define I2CONCLR_STAC		         (0x01 << 5)                                      // 起始标志清零位
#define I2CONCLR_I2ENC		         (0x01 << 6)                                      // I2C接口禁止位

// 数据寄存器（I2DAT-0x4001 C008）
#define I2DAT_I2C			         (0x00)                                           // 该寄存器保存已接收的数据或要发送的数据 

// 从机地址寄存器（I2ADR-0x4001 C00C）
#define I2ADR_I2C			         (0x00)                                           // I2C 从机地址

/********************************************************************************************************
* 【内部函数】 internal function                                              
********************************************************************************************************/
static void I2C_DelayMs(u16 t);										                  // 延时
static void I2C_SetGpio(u8 num);									                  // 端口设置
static void I2C_Start(u8 num);								                          // 发启动信号
static void I2C_Stop(u8 num);    							                          // 发停止信号
static u8   I2C_WriteByte(u8 num, u8 dat, u8 mode); 		                          // 发送一字节数据
static u8   I2C_ReadByte(u8 num, u8 last); 				                              // 读取数据

/********************************************************************************************************
* 【全局函数】 global function                                                 
********************************************************************************************************/
extern u8   I2CInit(u8 num, u32 fi2c);							                      // 初始化IIC
extern void I2CWriteByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 dat);	              // 写一字节字节
extern void I2CWriteNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 len);       // 多字节写入
extern u8   I2CReadByte(u8 num, u8 icAddr, u8 type, u8 addr);                         // 读取一字节
extern void I2CReadNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 len);        // 多字节读取

/********************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
