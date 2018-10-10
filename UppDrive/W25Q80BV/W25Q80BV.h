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
// Ƭѡ
#define W25Q_CS_FUN            (PIO1_0_GPIO)                               // P1.0 Ƭѡ   
#define W25Q_CS_PORT       	   (PORT1)							           // �˿�
#define W25Q_CS_PIN       	   (0)								           // �ܽ�

/* ���������SSP���ߣ��Ѿ���SSP�ļ��ж��壬�˴�����Ҫ�ٶ���
#define W25Q_SCK_FUN           (PIO0_7_GPIO)                               // P0.7 ʱ��   
#define W25Q_SCK_PORT          (PORT0)							           // �˿�
#define W25Q_SCK_PIN       	   (7)								           // �ܽ�

#define W25Q_MISO_FUN          (PIO0_8_GPIO)                               // P0.8 ��������   
#define W25Q_MISO_PORT         (PORT0)							           // �˿�
#define W25Q_MISO_PIN          (8)							 	           // �ܽ�

#define W25Q_MOSI_FUN          (PIO0_9_GPIO)                               // P0.9 �������  
#define W25Q_MOSI_PORT         (PORT0)							           // �˿�
#define W25Q_MOSI_PIN          (9)								           // �ܽ�	*/

/********************************************************************************************************
* ���ܽŷ��� Pin direction                                                
********************************************************************************************************/
#define W25Q_CS_OUT()          GPIOSetDir(W25Q_CS_PORT, W25Q_CS_PIN, GPIO_OUT)	 

/********************************************************************************************************
* ���ܽ�д�� Pin Write                                                
********************************************************************************************************/
#define W25Q_CS_WRITE(sw)      GPIOSetValue(W25Q_CS_PORT, W25Q_CS_PIN, sw)	    

/********************************************************************************************************
* ���궨�塿 macro definition                                               
********************************************************************************************************/
#define  W25Q_READ_ONLY	       (0x00)  	                                   // 0:Read/Write or 1:Read only 
#define  W25Q_BUF_SIZE		   (4096)								       // ��������

#define  W25Q_CN_START_ADDR	   (0x0C0000)						           //	������ʼ��ַ

/********************************************************************************************************
* �������Command set                                                 
********************************************************************************************************/
#define  W25Q_EWSR             (0x50)                                      // ʹ��д״̬�Ĵ�������
#define  W25Q_RDSR             (0x05)                                      // ��״̬�Ĵ�������
#define  W25Q_WRSR             (0x01)                                      // д״̬�Ĵ�������
#define  W25Q_WREN             (0x06)                                      // дʹ������
#define  W25Q_WRDI             (0x04)                                      // д��ֹ����
#define  W25Q_READ             (0x03)                                      // ����������
#define  W25Q_HIGH_SPEED_READ  (0x0B)                                      // �������ݶ�(��ַ�Զ�����)����
#define  W25Q_CHIP_ERASE       (0x60)                                      // 0xC7 оƬ��������
#define  W25Q_BLOCK_ERASE      (0x52)                                      // ���������
#define  W25Q_SECTOR_ERASE     (0x20)                                      // ������������
#define  W25Q_AAI_PROGRAM      (0xAF)                                      // �ֽڱ��(��ַ�Զ�����)����
#define  W25Q_BYTE_PROGRAM     (0x02)                                      // �ֽڱ������  
#define  W25Q_READ_ID          (0x90)                                      // ������ID
#define  W25Q_READ_ID2         (0xAB)                                      // ������ID(������ȫͬREAD_ID)
#define  W25Q_POW_DOWN		   (0xB9) 								       // ����ģʽ
#define  W25Q_RLS_POW_DOWN     (0xAB) 								       // �ָ�

/********************************************************************************************************
* ���ⲿ������ SPI function                                                 
********************************************************************************************************/
#define W25Q_WriteByte(dat)	   (SSPSendRcvByte(SSP1, dat) )		           // дһ�ֽ�����
#define W25Q_ReadByte()	       (SSPSendRcvByte(SSP1, 0xFF))	               // ��һ�ֽ�����

/********************************************************************************************************
* ���ڲ������� internal function                                              
********************************************************************************************************/
#if !W25Q_READ_ONLY
static u8   W25Q_ReadStatusReg(void);	                                   // ��״̬�Ĵ���
static void W25Q_EnableWriteReg(void);	                                   // ʹ��д״̬�Ĵ���
static void W25Q_WriteStatusReg(u8 dat);                                   // д״̬�Ĵ��� 
static void W25Q_EnableWrite(void);			                               // дʹ�ܼĴ���
static void W25Q_DisableWrite(void);		                               // д��ֹ�Ĵ���
static void W25Q_WaitBusy(void);										   // �ж�æ״̬
#endif

static void W25Q_WriteAddr(u32 addr);								       // д��ַ

/********************************************************************************************************
* ��ȫ�ֺ����� global function                                                 
********************************************************************************************************/
extern void W25QInit(void);				                                   // ��ʼ��IC
extern u8   W25QReadDeviceID(u8 idAddr);                                   // ������/����ID
extern u8   W25QReadByte(u32 addr);		                                   // ��ָ����ַ��ȡһ���ֽ�����
extern u8   W25QHighSpeedReadByte(u32 addr);                               // ���ٶ�һ�ֽ�
extern void W25QReadNByte(u32 addr, u8 *buf, u32 len);                     // ���ֽڶ�ȡ
extern void W25QHighSpeedReadNByte(u32 addr, u8 *buf, u32 len);            // ��������������

extern void W25QPowerDown(void); 										   // ����ģʽ
extern void W25QWakeUp(void);											   // �˳�����

#if !W25Q_READ_ONLY													       // д��غ���
extern void W25QWriteByte(u32 addr, u8 dat);						       // д��һ�ֽ�����
extern void W25QWriteNByte(u32 addr, u8 *buf, u32 len);		               // д�����ֽ�
extern void W25QWriteNByteSecEra(u32 addr, u8 *buf, u32 len);	           // ��������������

extern void W25QWriteNByteEra(u32 addr, u8 *buf, u32 len);			       // ���ֽ�д������������

extern void W25QChipErase(void);										   // оƬ����
extern void W25QSectorErase(u32 sectorAddr);						       // ��������(1������4Kbyte)
extern void W25QBlockErase(u32 blockAddr); 							       // �����(1����32Kbyte)
#endif

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
