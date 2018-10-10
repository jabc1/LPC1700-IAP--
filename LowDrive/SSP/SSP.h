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
#define SSP_SCK0_SELECT       (0x00)						                  // ʱ�ӹܽ�ѡ��0x00 - P0_15, 0x01 - P1_20
#define SSP_MISO0_SELECT      (0x00)						                  // ʱ�ӹܽ�ѡ��0x00 - P0_17, 0x01 - P1_23
#define SSP_MOSI0_SELECT      (0x00)						                  // ʱ�ӹܽ�ѡ��0x00 - P0_18, 0x01 - P1_24

#define SSP_SCK1_SELECT       (0x00)						                  // ʱ�ӹܽ�ѡ��0x00 - P0_7, 0x01 - P1_31

// ---------------------------- SSP0 --------------------------------------------------
#if (SSP_SCK0_SELECT == 0x00)
#define SSP_SCK0_FUN          (PIO0_15_SCK0)    			                  // P0.15 ʱ��
#define SSP_SCK0_PORT         (PORT0)						                  // �˿�  
#define SSP_SCK0_PIN          (15)							                  // �ܽ�
#else
#define SSP_SCK0_FUN          (PIO1_20_SCK0)    			                  // P1.20 ʱ��
#define SSP_SCK0_PORT         (PORT1)						                  // �˿�  
#define SSP_SCK0_PIN          (20)
#endif

#if (SSP_MISO0_SELECT == 0x00)
#define SSP_MISO0_FUN         (PIO0_17_MISO0)    			                  // P0.17 ��������
#define SSP_MISO0_PORT        (PORT0)						                  // �˿�  
#define SSP_MISO0_PIN         (17)							                  // �ܽ�
#else
#define SSP_MISO0_FUN         (PIO1_23_MISO0)    			                  // P1.23 ��������
#define SSP_MISO0_PORT        (PORT1)						                  // �˿�  
#define SSP_MISO0_PIN         (23)							                  // �ܽ�
#endif

#if (SSP_MOSI0_SELECT == 0x00)
#define SSP_MOSI0_FUN         (PIO0_18_MOSI0)    			                  // P0.18 �������
#define SSP_MOSI0_PORT        (PORT0)						                  // �˿�  
#define SSP_MOSI0_PIN         (18)							                  // �ܽ�
#else
#define SSP_MOSI0_FUN         (PIO1_24_MOSI0)    			                  // P1.24 �������
#define SSP_MOSI0_PORT        (PORT1)						                  // �˿�  
#define SSP_MOSI0_PIN         (24)							                  // �ܽ�
#endif

// ---------------------------- SSP1 --------------------------------------------------
#if (SSP_SCK1_SELECT == 0x00)
#define SSP_SCK1_FUN          (PIO0_7_SCK1)    			                      // P0.7 ʱ��
#define SSP_SCK1_PORT         (PORT0)						                  // �˿�  
#define SSP_SCK1_PIN          (7)							                  // �ܽ�
#else
#define SSP_SCK1_FUN          (PIO1_31_SCK1)    			                  // P1.31 ʱ��
#define SSP_SCK1_PORT         (PORT1)						                  // �˿�  
#define SSP_SCK1_PIN          (31)							                  // �ܽ�
#endif

#define SSP_MISO1_FUN         (PIO0_8_MISO1)    			                  // P0.8 ��������
#define SSP_MISO1_PORT        (PORT0)						                  // �˿�  
#define SSP_MISO1_PIN         (8)							                  // �ܽ�

#define SSP_MOSI1_FUN         (PIO0_9_MOSI1)    			                  // P0.9 �������
#define SSP_MOSI1_PORT        (PORT0)						                  // �˿�  
#define SSP_MOSI1_PIN         (9)							                  // �ܽ�

/******************************************************************************************************** 
* ��ö�ٱ����� Enumerations                                                
********************************************************************************************************/
typedef enum
{
    SSP0 = 0,
    SSP1 = 1,

} SSP_NUM;																	  // SSP

/******************************************************************************************************** 
* ���Ĵ����� Register                                              
********************************************************************************************************/

// ���ƼĴ�����CR0-0x4008 8000��
#define CR0_DSS			(0x00)												  // ���ݳ���ѡ��
#define CR0_FRF			(0x04)												  // ֡��ʽ
#define CR0_SPO			(0x06)												  // ʱ���������
#define CR0_SPH			(0x07)												  // ʱ�������λ
#define CR0_SCR			(0x08)												  // ����SSPͨ�ŵ�λ����

// ���ƼĴ�����CR1-0x4008 8004��
#define CR1_LBM			(0x00)												  // ��дģʽ
#define CR1_SSE			(0x01)												  // SSPʹ��
#define CR1_MS			(0x02)												  // ����/�ӻ�ģʽ
#define CR1_SOD			(0x03)												  // �ӻ��������

// �ж�����Ĵ�����ICR-0x4008 8020��
#define ICR_RORIC		(0x01 << 0)											  // ���λд"1"�������������ж�
#define ICR_RTIC		(0x01 << 1)											  // ���λд"1"�������ʱ�ж�

// ״̬�Ĵ�����SR �C 0x4008 800C��
#define SR_TFE		    (0x01 << 0)							                  // ����FIFO��
#define SR_TNF		    (0x01 << 1) 							              // ����FIFOδ��
#define SR_RNE		    (0x01 << 2)							                  // ����FIFO��Ϊ��
#define SR_RFF	  	    (0x01 << 3) 							              // ����FIFO��
#define SR_BSY		    (0x01 << 4)							                  // æ

// �ж�ʹ����λ/����Ĵ�����IMSC �C 0x4008 8014��
#define IMSC_RORIM	    (0x01 << 0)							                  // �����λ��λ��ʹ�ܽ�������ж�
#define IMSC_RTIM	    (0x01 << 1)							                  // �����λ��λ��ʹ�ܽ��ճ�ʱ�ж�
#define IMSC_RXIM	    (0x01 << 2)							                  // ʹ�õ�Rx FIFO������һ��Ϊ��ʱ�����ж�
#define IMSC_TXIM  	    (0x01 << 3)							                  // ʹ�õ�Tx FIFO������һ��Ϊ��ʱ�����ж�

// ԭʼ�жϼĴ�����RIS �C 0x4008 8018��
#define RIS_RORRIS	    (0x01 << 0)							                  // ��Rx FIFO��ʱ�ֽ��յ���һ֡����ʱ����λ��λ
#define RIS_RTRIS	    (0x01 << 1)							                  // ���Rx FIFO��Ϊ�գ����ڡ���ʱ���ڡ���û�б�����ʱ����λ��λ
#define RIS_RXRIS	    (0x01 << 2)							                  // ��Rx FIFO������һ��Ϊ��ʱ����λ��λ
#define RIS_TXRIS	    (0x01 << 3)							                  // ��Tx FIFO������һ��Ϊ��ʱ����λ��λ

// ʹ���жϼĴ���λ������MIS �C 0x4008 801C��
#define MIS_RORMIS	    (0x01 << 0)							                  // ��Rx FIFO��ʱ�ֽ��յ�����һ֡���ݣ����жϱ�ʹ��ʱ����λ��λ
#define MIS_RTMIS	    (0x01 << 1)							                  // ���Rx FIFO��Ϊ�ղ��ڡ���ʱ���ڡ���û�б����������жϱ�ʹ��ʱ����λ��λ
#define MIS_RXMIS	    (0x01 << 2)							                  // ��Rx FIFO������һ��Ϊ�������жϱ�ʹ��ʱ����λ��λ
#define MIS_TXMIS	    (0x01 << 3)							                  // ��Tx FIFO������һ��Ϊ�գ����жϱ�ʹ��ʱ����λ��λ

/********************************************************************************************************
* Internal Function                                                  
********************************************************************************************************/
static void SSP_SetGpio(u8 spiNum);		                                      // �˿ڳ�ʼ��

/********************************************************************************************************
* Global Function                                                 
********************************************************************************************************/
extern u8   SSPInit(u8 sspNum, u8 clk);			                              // ��ʼ��SSP
extern u8   SSPSendByte(u8 sspNum, u8 dat);			                          // ����һ�ֽ�����
extern u8   SSPRcvByte(u8 sspNum);					 	                      // ����һ�ֽ�����
extern u8   SSPSendRcvByte(u8 sspNum, u8 dat);  	                          // SSP�����շ�
extern void SSPSetSpeed(u8 sspNum, u8 clk);		                              // SSP�ٶ�����

/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
