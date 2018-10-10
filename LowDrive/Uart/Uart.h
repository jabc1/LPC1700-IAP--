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
*				   ֻ��ʵ����ͨ���ڹ���
*
********************************************************************************************************/

#ifndef __UART_H 
#define __UART_H

/********************************************************************************************************
* ���ܽŶ��塿 pin definition                                                
********************************************************************************************************/
#define UART1_IO_SELECT		   (0x01)						                      // 0x00 - ѡ��P0.15��P0.16; 0x01 - ѡ��P2.0��P2.1;
#define UART2_IO_SELECT		   (0x00)						                      // 0x00 - ѡ��P0.10��P0.11; 0x01 - ѡ��P2.8��P2.9;
#define UART3_IO_SELECT		   (0x00)						                      // 0x00 - ѡ��P0.0��P0.1; 0x01 - ѡ��P0.25��P0.26; 0x02 - ѡ��P4.28��P4.29

// ---------------------------- UART0 ------------------------------------------------------------------
#define UART_TXD0_FUN          (PIO0_2_TXD0)    			                      // P0.2 ���ڷ���
#define UART_TXD0_PORT         (PORT0)						                      // �˿�  
#define UART_TXD0_PIN          (2)							                      // �ܽ�

#define UART_RXD0_FUN          (PIO0_3_RXD0)    			                      // P0.3 ���ڽ���
#define UART_RXD0_PORT         (PORT0)						                      // �˿�  
#define UART_RXD0_PIN          (3)							                      // �ܽ�

// ---------------------------- UART1 ------------------------------------------------------------------
#if (UART1_IO_SELECT == 0x00)
#define UART_TXD1_FUN          (PIO0_15_TXD1)    			                      // P0.15 ���ڷ���
#define UART_TXD1_PORT         (PORT0)						                      // �˿�  
#define UART_TXD1_PIN          (15)							                      // �ܽ�

#define UART_RXD1_FUN          (PIO0_16_RXD1)    			                      // P0.26 ���ڽ���
#define UART_RXD1_PORT         (PORT0)						                      // �˿�  
#define UART_RXD1_PIN          (16)							                      // �ܽ�

#else
#define UART_TXD1_FUN          (PIO2_0_TXD1)    			                      // P2.0 ���ڷ���
#define UART_TXD1_PORT         (PORT2)						                      // �˿�  
#define UART_TXD1_PIN          (0)							                      // �ܽ�

#define UART_RXD1_FUN          (PIO2_1_RXD1)    			                      // P2.1 ���ڽ���
#define UART_RXD1_PORT         (PORT2)						                      // �˿�  
#define UART_RXD1_PIN          (1)							                      // �ܽ�
#endif

// ---------------------------- UART2 ------------------------------------------------------------------
#if (UART2_IO_SELECT == 0x00)
#define UART_TXD2_FUN          (PIO0_10_TXD2)    			                      // P0.10 ���ڷ���
#define UART_TXD2_PORT         (PORT0)						                      // �˿�  
#define UART_TXD2_PIN          (10)							                      // �ܽ�

#define UART_RXD2_FUN          (PIO0_11_RXD2)    			                      // P0.11 ���ڽ���
#define UART_RXD2_PORT         (PORT0)						                      // �˿�  
#define UART_RXD2_PIN          (11)							                      // �ܽ�

#else
#define UART_TXD2_FUN          (PIO2_8_TXD2)    			                      // P2.8 ���ڷ���
#define UART_TXD2_PORT         (PORT2)						                      // �˿�  
#define UART_TXD2_PIN          (8)							                      // �ܽ�

#define UART_RXD2_FUN          (PIO2_9_RXD2)    			                      // P2.9 ���ڽ���
#define UART_RXD2_PORT         (PORT2)						                      // �˿�  
#define UART_RXD2_PIN          (9)							                      // �ܽ�
#endif

// ---------------------------- UART3 ------------------------------------------------------------------
#if (UART3_IO_SELECT == 0x00)
#define UART_TXD3_FUN          (PIO0_0_TXD3)    			                      // P0.0 ���ڷ���
#define UART_TXD3_PORT         (PORT0)						                      // �˿�  
#define UART_TXD3_PIN          (0)							                      // �ܽ�

#define UART_RXD3_FUN          (PIO0_1_RXD3)    			                      // P0.1 ���ڽ���
#define UART_RXD3_PORT         (PORT0)						                      // �˿�  
#define UART_RXD3_PIN          (1)							                      // �ܽ�

#elif (UART3_IO_SELECT == 0x01)
#define UART_TXD3_FUN          (PIO0_25_TXD3)    			                      // P0.25 ���ڷ���
#define UART_TXD3_PORT         (PORT0)						                      // �˿�  
#define UART_TXD3_PIN          (25)							                      // �ܽ�

#define UART_RXD3_FUN          (PIO0_26_RXD3)    			                      // P0.26 ���ڽ���
#define UART_RXD3_PORT         (PORT0)						                      // �˿�  
#define UART_RXD3_PIN          (26)							                      // �ܽ�

#else
#define UART_TXD3_FUN          (PIO4_28_TXD3)    			                      // P4.28 ���ڷ���
#define UART_TXD3_PORT         (PORT4)						                      // �˿�  
#define UART_TXD3_PIN          (28)							                      // �ܽ�

#define UART_RXD3_FUN          (PIO4_29_RXD3)    			                      // P4.29 ���ڽ���
#define UART_RXD3_PORT         (PORT4)						                      // �˿�  
#define UART_RXD3_PIN          (29)							                      // �ܽ�
#endif

/********************************************************************************************************
* UART1                                                 
********************************************************************************************************/
/* ע�����

   ����û��ʹ��LPC_UART1������ʹ�õ���LPC_UART1M����Ҫԭ����LPC_UART1������3�����������𣬶��˸�Modem�ӿڡ�
���ԣ���ͷ�ļ���ߵ���������һ��LPC_UART1_TypeDef�����������޷���LPC_UART1������3������������һ��������
���LPC_UART1Ҳ�����ΪLPC_UART_TypeDef��ʹ��4�����ڴ������ͨ�ã������������޷�ʵ��LPC_UART1�����⹦�ܡ�
���Ҫʹ��LPC_UART1�����⹦�ܣ���Ҫ�Լ����±�д�������롣*/

#define LPC_UART1M             ((LPC_UART_TypeDef*) LPC_UART1_BASE)

/******************************************************************************************************** 
* ��ö�ٱ����� Enumerations                                                
********************************************************************************************************/
typedef enum
{
    UART0 = 0,
	UART1 = 1,
	UART2 = 2,
	UART3 = 3,

} UART_NUM;																		  // ����

//------------------------------------------------------------------------------------------------------
typedef enum
{																				   
    UART_INQUIRY = 0x00,													      // ��ѯ�շ�
	UART_IT_RXDE = 0x01,													      // �����жϣ���ѯ����
    UART_IT_TXDE = 0x02,													      // ��ѯ���գ��жϷ���
	RXDI_IT_RTEN = 0x03,													      // �����жϣ��жϷ���

} UART_WORK_MODE;																  // ����ģʽ

//------------------------------------------------------------------------------------------------------
typedef enum
{
    DAT_BIT5 = 0,																  // 5λ�ַ�����
	DAT_BIT6 = 1,																  // 6λ�ַ�����
	DAT_BIT7 = 2,																  // 7λ�ַ�����
	DAT_BIT8 = 3,																  // 8λ�ַ�����

} UART_DAT_BIT;																	  // ����λ

//------------------------------------------------------------------------------------------------------
typedef enum
{																				   
    STOP_BIT1 = 0,																  // 1��ֹͣλ
	STOP_BIT2 = 4,																  // 2��ֹͣλ

} UART_STOP_BIT;																  // ֹͣ

//------------------------------------------------------------------------------------------------------
typedef enum
{																				  
    PARITY_DI = 0,																  // ��ֹ��ż������У�� 
	PARITY_EN = 8,																  // ʹ����ż������У��

} UART_PARITY_BIT;																  // ֹͣ

//------------------------------------------------------------------------------------------------------
typedef enum
{																				   
    TRI_1BYTE  = (0x00 << 6),													  // ������0��Ĭ��1�ֽڻ�0x01��
	TRI_4BYTE  = (0x01 << 6),													  // ������1��Ĭ��4�ֽڻ�0x04��
	TRI_8BYTE  = (0x02 << 6),													  // ������2��Ĭ��8�ֽڻ�0x08��
	TRI_14BYTE = (0x03 << 6),													  // ������3��Ĭ��14�ֽڻ�0x0E��

} UART_TRI_BYTE;																  // �жϴ����ֽڻ���

/******************************************************************************************************** 
* ���Ĵ����� Register                                              
********************************************************************************************************/

// �ж�ʹ�ܼĴ�����IER-0x4000 C004��
#define IER_QUERY				 (0x00)											  // ��ѯ��ʽ�շ�
#define IER_RBR	                 (0x01 << 0)				                      // �����ж�ʹ��
#define IER_THRE	             (0x01 << 1)				                      // �����ж�ʹ��
#define IER_RLS		             (0x01 << 2)				                      // ������״̬�ж�ʹ��
#define IER_MSI		             (0x01 << 3)				                      // ʹ�ܵ��ƽ�����ж�
#define IER_CTS		             (0x01 << 7)				                      // ʹ��CTS�ж�
#define IER_ABE		             (0x01 << 8)				                      // ʹ��auto-baud�����ж�
#define IER_ABT		             (0x01 << 9)				                      // ʹ��auto-baud��ʱ�ж�

// �жϱ�ʶ�Ĵ�����IER-0x4000 C008��
#define IIR_PEND 	             (0x01)				                              // �ж�״̬
#define IIR_RLS		             (0x03)				                              // ������״̬
#define IIR_RDA		             (0x02)				                              // �������ݿ���
#define IIR_CTI		             (0x06)				                              // �ַ���ʱָʾ
#define IIR_THRE	             (0x01)				                              // 3 �C THRE�ж�

// FIFO���ƼĴ�����LCR-0x4000 C008��
#define	LCR_FIFO_EN				 (0x01 << 1)									  // ʹ�ܶ�UARTn RX FIFO��TX FIFO
#define	LCR_RX_RST				 (0x01 << 2)									  // ����Rx FIFO
#define	LCR_TX_RST				 (0x01 << 3)									  // ����Tx FIFO

// �߿��ƼĴ�����LCR-0x4000 C00C��
#define LCR_BKCL		         (0x01 << 6)				                      // ʹ�ܼ������
#define LCR_DLAB		         (0x01 << 7)				                      // ʹ�ܷ��ʳ���������

// ��״̬�Ĵ�����LSR-0x4000 C014��
#define LSR_RDR		             (0x01)				                              // �������ݾ���
#define LSR_OE		             (0x02)				                              // �������
#define LSR_PE		             (0x04)				                              // ��ż����
#define LSR_FE		             (0x08)				                              // ֡����
#define LSR_BI		             (0x10)				                              // ����ж�
#define LSR_THRE	             (0x20)				                              // ����FIFO��
#define LSR_TEMT	             (0x40)				                              // ��������
#define LSR_RXFE	             (0x80)				                              // Rx FIFO����

// Auto-baud���ƼĴ�����ACR �C 0x4000 C02C��
#define ACR_START		         (0x00)				                              // ��auto-baud���ܽ����󣬸�λ���Զ�����
#define ACR_MODE		         (0x01)				                              // Auto-baudģʽѡ��λ
#define ACR_AUTO_RESTART		 (0x02)				                              // ����ģʽ
#define ACR_ABEO_INT_CLR		 (0x08)				                              // Auto-baud�����ж�����λ
#define ACR_ABTO_INT_CLR		 (0x09)				                              // Auto-baud��ʱ�ж�����λ

// RS485���ƼĴ�����RS485CTRL �C 0x4001 004C��
#define RS485_NMMEN		         (0x1 << 0)			                              // RS-485/EIA-485��ͨ���ģʽ��NMM������
#define RS485_RXDIS		         (0x1 << 1)			                              // ���ܽ�����
#define RS485_AADEN		         (0x1 << 2)			                              // ʹ���Զ���ַ��⣨AAD��
#define RS485_SEL	 	         (0x1 << 3)			                              // ���ʹ���˷�����ƣ�λDCTRL =1��,�ܽ�DTR�ᱻ���ڷ������
#define RS485_DCTRL		         (0x1 << 4)			                              // ʹ���Զ��������
#define RS485_OINV		         (0x1 << 5)			                              // �������һ������λ�����ͳ�ȥ�󣬸�λ�ͻᱻ����Ϊ�߼���0��

/********************************************************************************************************
* Variable definition                                               
********************************************************************************************************/
typedef struct _UART_DCM_TBL
{
    fp32 FR;																	  // С��ֵ
	u8   Div;																	  // ��Ƶ
	u8   Mul;																	  // ����

} UART_DCM_TBL;

/********************************************************************************************************
* ���ڲ������� internal function                                              
********************************************************************************************************/
static void UART_SetGpio(u8 num);	                                              // �ܽ�����
static u32  UART_OffsetAddr(u8 num);											  // ƫ�Ƶ�ַ
static void UART_Dlab(u8 num, u8 mode);											  // ���ʳ���������
static u8   UART_BaudSet(u8 num, u32 bps);										  // ���ò�����
static u32  UART_AutoBaud(u8 num, u8 mode);										  // �Զ�������

/********************************************************************************************************
* ��ȫ�ֺ����� global function                                                 
********************************************************************************************************/
extern u8   UARTInit(u8 num, u32 bps, u8 mod, u8 tri);	                          // ��ʼ��

extern void UARTSendByte(u8 num, u8 ch);                                          // ����һ�ֽ�
extern void UARTSendStr(u8 num, u8 *pStr);										  // �����ַ���
extern void UARTSendMultiByte(u8 num, u8 *pStr, u32 length);                      // ��������

extern u8   UARTReceiveByte(u8 num);				                              // ����һ�ֽ�
extern void UARTReceiveMultiByte(u8 num, u8 *pStr, u32 length);                   // ��������

/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
