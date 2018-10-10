/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : uart.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*				   只能实现普通串口功能
*
********************************************************************************************************/

#ifndef __UART_H 
#define __UART_H

/********************************************************************************************************
* 【管脚定义】 pin definition                                                
********************************************************************************************************/
#define UART1_IO_SELECT		   (0x01)						                      // 0x00 - 选择P0.15和P0.16; 0x01 - 选择P2.0和P2.1;
#define UART2_IO_SELECT		   (0x00)						                      // 0x00 - 选择P0.10和P0.11; 0x01 - 选择P2.8和P2.9;
#define UART3_IO_SELECT		   (0x00)						                      // 0x00 - 选择P0.0和P0.1; 0x01 - 选择P0.25和P0.26; 0x02 - 选择P4.28和P4.29

// ---------------------------- UART0 ------------------------------------------------------------------
#define UART_TXD0_FUN          (PIO0_2_TXD0)    			                      // P0.2 串口发送
#define UART_TXD0_PORT         (PORT0)						                      // 端口  
#define UART_TXD0_PIN          (2)							                      // 管脚

#define UART_RXD0_FUN          (PIO0_3_RXD0)    			                      // P0.3 串口接收
#define UART_RXD0_PORT         (PORT0)						                      // 端口  
#define UART_RXD0_PIN          (3)							                      // 管脚

// ---------------------------- UART1 ------------------------------------------------------------------
#if (UART1_IO_SELECT == 0x00)
#define UART_TXD1_FUN          (PIO0_15_TXD1)    			                      // P0.15 串口发送
#define UART_TXD1_PORT         (PORT0)						                      // 端口  
#define UART_TXD1_PIN          (15)							                      // 管脚

#define UART_RXD1_FUN          (PIO0_16_RXD1)    			                      // P0.26 串口接收
#define UART_RXD1_PORT         (PORT0)						                      // 端口  
#define UART_RXD1_PIN          (16)							                      // 管脚

#else
#define UART_TXD1_FUN          (PIO2_0_TXD1)    			                      // P2.0 串口发送
#define UART_TXD1_PORT         (PORT2)						                      // 端口  
#define UART_TXD1_PIN          (0)							                      // 管脚

#define UART_RXD1_FUN          (PIO2_1_RXD1)    			                      // P2.1 串口接收
#define UART_RXD1_PORT         (PORT2)						                      // 端口  
#define UART_RXD1_PIN          (1)							                      // 管脚
#endif

// ---------------------------- UART2 ------------------------------------------------------------------
#if (UART2_IO_SELECT == 0x00)
#define UART_TXD2_FUN          (PIO0_10_TXD2)    			                      // P0.10 串口发送
#define UART_TXD2_PORT         (PORT0)						                      // 端口  
#define UART_TXD2_PIN          (10)							                      // 管脚

#define UART_RXD2_FUN          (PIO0_11_RXD2)    			                      // P0.11 串口接收
#define UART_RXD2_PORT         (PORT0)						                      // 端口  
#define UART_RXD2_PIN          (11)							                      // 管脚

#else
#define UART_TXD2_FUN          (PIO2_8_TXD2)    			                      // P2.8 串口发送
#define UART_TXD2_PORT         (PORT2)						                      // 端口  
#define UART_TXD2_PIN          (8)							                      // 管脚

#define UART_RXD2_FUN          (PIO2_9_RXD2)    			                      // P2.9 串口接收
#define UART_RXD2_PORT         (PORT2)						                      // 端口  
#define UART_RXD2_PIN          (9)							                      // 管脚
#endif

// ---------------------------- UART3 ------------------------------------------------------------------
#if (UART3_IO_SELECT == 0x00)
#define UART_TXD3_FUN          (PIO0_0_TXD3)    			                      // P0.0 串口发送
#define UART_TXD3_PORT         (PORT0)						                      // 端口  
#define UART_TXD3_PIN          (0)							                      // 管脚

#define UART_RXD3_FUN          (PIO0_1_RXD3)    			                      // P0.1 串口接收
#define UART_RXD3_PORT         (PORT0)						                      // 端口  
#define UART_RXD3_PIN          (1)							                      // 管脚

#elif (UART3_IO_SELECT == 0x01)
#define UART_TXD3_FUN          (PIO0_25_TXD3)    			                      // P0.25 串口发送
#define UART_TXD3_PORT         (PORT0)						                      // 端口  
#define UART_TXD3_PIN          (25)							                      // 管脚

#define UART_RXD3_FUN          (PIO0_26_RXD3)    			                      // P0.26 串口接收
#define UART_RXD3_PORT         (PORT0)						                      // 端口  
#define UART_RXD3_PIN          (26)							                      // 管脚

#else
#define UART_TXD3_FUN          (PIO4_28_TXD3)    			                      // P4.28 串口发送
#define UART_TXD3_PORT         (PORT4)						                      // 端口  
#define UART_TXD3_PIN          (28)							                      // 管脚

#define UART_RXD3_FUN          (PIO4_29_RXD3)    			                      // P4.29 串口接收
#define UART_RXD3_PORT         (PORT4)						                      // 端口  
#define UART_RXD3_PIN          (29)							                      // 管脚
#endif

/********************************************************************************************************
* UART1                                                 
********************************************************************************************************/
/* 注意事项：

   这里没有使用LPC_UART1，而是使用的是LPC_UART1M，主要原因是LPC_UART1与其他3个串口有区别，多了个Modem接口。
所以，在头文件里边单独定义了一个LPC_UART1_TypeDef，导致我们无法把LPC_UART1与其他3个串口整合在一起，所以特
别把LPC_UART1也定义成为LPC_UART_TypeDef，使得4个串口代码可以通用，但是这样将无法实现LPC_UART1的特殊功能。
如果要使用LPC_UART1的特殊功能，需要自己重新编写驱动代码。*/

#define LPC_UART1M             ((LPC_UART_TypeDef*) LPC_UART1_BASE)

/******************************************************************************************************** 
* 【枚举变量】 Enumerations                                                
********************************************************************************************************/
typedef enum
{
    UART0 = 0,
	UART1 = 1,
	UART2 = 2,
	UART3 = 3,

} UART_NUM;																		  // 串口

//------------------------------------------------------------------------------------------------------
typedef enum
{																				   
    UART_INQUIRY = 0x00,													      // 查询收发
	UART_IT_RXDE = 0x01,													      // 接收中断，查询发送
    UART_IT_TXDE = 0x02,													      // 查询接收，中断发生
	RXDI_IT_RTEN = 0x03,													      // 接收中断，中断发送

} UART_WORK_MODE;																  // 工作模式

//------------------------------------------------------------------------------------------------------
typedef enum
{
    DAT_BIT5 = 0,																  // 5位字符长度
	DAT_BIT6 = 1,																  // 6位字符长度
	DAT_BIT7 = 2,																  // 7位字符长度
	DAT_BIT8 = 3,																  // 8位字符长度

} UART_DAT_BIT;																	  // 数据位

//------------------------------------------------------------------------------------------------------
typedef enum
{																				   
    STOP_BIT1 = 0,																  // 1个停止位
	STOP_BIT2 = 4,																  // 2个停止位

} UART_STOP_BIT;																  // 停止

//------------------------------------------------------------------------------------------------------
typedef enum
{																				  
    PARITY_DI = 0,																  // 禁止奇偶产生和校验 
	PARITY_EN = 8,																  // 使能奇偶产生和校验

} UART_PARITY_BIT;																  // 停止

//------------------------------------------------------------------------------------------------------
typedef enum
{																				   
    TRI_1BYTE  = (0x00 << 6),													  // 触发点0（默认1字节或0x01）
	TRI_4BYTE  = (0x01 << 6),													  // 触发点1（默认4字节或0x04）
	TRI_8BYTE  = (0x02 << 6),													  // 触发点2（默认8字节或0x08）
	TRI_14BYTE = (0x03 << 6),													  // 触发点3（默认14字节或0x0E）

} UART_TRI_BYTE;																  // 中断触发字节缓冲

/******************************************************************************************************** 
* 【寄存器】 Register                                              
********************************************************************************************************/

// 中断使能寄存器（IER-0x4000 C004）
#define IER_QUERY				 (0x00)											  // 查询方式收发
#define IER_RBR	                 (0x01 << 0)				                      // 接收中断使能
#define IER_THRE	             (0x01 << 1)				                      // 发送中断使能
#define IER_RLS		             (0x01 << 2)				                      // 接收线状态中断使能
#define IER_MSI		             (0x01 << 3)				                      // 使能调制解调器中断
#define IER_CTS		             (0x01 << 7)				                      // 使能CTS中断
#define IER_ABE		             (0x01 << 8)				                      // 使能auto-baud结束中断
#define IER_ABT		             (0x01 << 9)				                      // 使能auto-baud超时中断

// 中断标识寄存器（IER-0x4000 C008）
#define IIR_PEND 	             (0x01)				                              // 中断状态
#define IIR_RLS		             (0x03)				                              // 接收线状态
#define IIR_RDA		             (0x02)				                              // 接收数据可用
#define IIR_CTI		             (0x06)				                              // 字符超时指示
#define IIR_THRE	             (0x01)				                              // 3 – THRE中断

// FIFO控制寄存器（LCR-0x4000 C008）
#define	LCR_FIFO_EN				 (0x01 << 1)									  // 使能对UARTn RX FIFO和TX FIFO
#define	LCR_RX_RST				 (0x01 << 2)									  // 清零Rx FIFO
#define	LCR_TX_RST				 (0x01 << 3)									  // 清零Tx FIFO

// 线控制寄存器（LCR-0x4000 C00C）
#define LCR_BKCL		         (0x01 << 6)				                      // 使能间隔发送
#define LCR_DLAB		         (0x01 << 7)				                      // 使能访问除数锁存器

// 线状态寄存器（LSR-0x4000 C014）
#define LSR_RDR		             (0x01)				                              // 接收数据就绪
#define LSR_OE		             (0x02)				                              // 溢出错误
#define LSR_PE		             (0x04)				                              // 奇偶错误
#define LSR_FE		             (0x08)				                              // 帧错误
#define LSR_BI		             (0x10)				                              // 间隔中断
#define LSR_THRE	             (0x20)				                              // 发送FIFO空
#define LSR_TEMT	             (0x40)				                              // 发送器空
#define LSR_RXFE	             (0x80)				                              // Rx FIFO错误

// Auto-baud控制寄存器（ACR – 0x4000 C02C）
#define ACR_START		         (0x00)				                              // 在auto-baud功能结束后，该位会自动清零
#define ACR_MODE		         (0x01)				                              // Auto-baud模式选择位
#define ACR_AUTO_RESTART		 (0x02)				                              // 启动模式
#define ACR_ABEO_INT_CLR		 (0x08)				                              // Auto-baud结束中断清零位
#define ACR_ABTO_INT_CLR		 (0x09)				                              // Auto-baud超时中断清零位

// RS485控制寄存器（RS485CTRL – 0x4001 004C）
#define RS485_NMMEN		         (0x1 << 0)			                              // RS-485/EIA-485普通多点模式（NMM）禁能
#define RS485_RXDIS		         (0x1 << 1)			                              // 禁能接收器
#define RS485_AADEN		         (0x1 << 2)			                              // 使能自动地址检测（AAD）
#define RS485_SEL	 	         (0x1 << 3)			                              // 如果使能了方向控制（位DCTRL =1）,管脚DTR会被用于方向控制
#define RS485_DCTRL		         (0x1 << 4)			                              // 使能自动方向控制
#define RS485_OINV		         (0x1 << 5)			                              // 在数最后一个数据位被发送出去后，该位就会被驱动为逻辑“0”

/********************************************************************************************************
* Variable definition                                               
********************************************************************************************************/
typedef struct _UART_DCM_TBL
{
    fp32 FR;																	  // 小数值
	u8   Div;																	  // 分频
	u8   Mul;																	  // 乘数

} UART_DCM_TBL;

/********************************************************************************************************
* 【内部函数】 internal function                                              
********************************************************************************************************/
static void UART_SetGpio(u8 num);	                                              // 管脚设置
static u32  UART_OffsetAddr(u8 num);											  // 偏移地址
static void UART_Dlab(u8 num, u8 mode);											  // 访问除数锁存器
static u8   UART_BaudSet(u8 num, u32 bps);										  // 设置波特率
static u32  UART_AutoBaud(u8 num, u8 mode);										  // 自动波特率

/********************************************************************************************************
* 【全局函数】 global function                                                 
********************************************************************************************************/
extern u8   UARTInit(u8 num, u32 bps, u8 mod, u8 tri);	                          // 初始化

extern void UARTSendByte(u8 num, u8 ch);                                          // 发送一字节
extern void UARTSendStr(u8 num, u8 *pStr);										  // 发送字符串
extern void UARTSendMultiByte(u8 num, u8 *pStr, u32 length);                      // 发送数据

extern u8   UARTReceiveByte(u8 num);				                              // 接收一字节
extern void UARTReceiveMultiByte(u8 num, u8 *pStr, u32 length);                   // 接收数据

/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
