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
*				   ʹ�üĴ�����ַʵ�ֶ����߿��ƣ��Ӷ���С�������ռ�
*
********************************************************************************************************/

#ifndef __I2C_H 
#define __I2C_H

/********************************************************************************************************
* ���ܽŶ��塿 pin definition                                                
********************************************************************************************************/
#define I2C1_IO_SELECT		         (0x01)						                      // 0x00 - ѡ��P0.0��P0.1; 0x01 - ѡ��P0.19��P0.20;

// ---------------------------- I2C0 -------------------------------------------------
#define I2C_SDA0_FUN                 (PIO0_27_SDA0)    			                      // P0.27 ��������
#define I2C_SDA0_PORT                (PORT0)						                  // �˿�  
#define I2C_SDA0_PIN       	         (27)							                  // �ܽ�

#define I2C_SCL0_FUN                 (PIO0_28_SCL0)    			                      // P0.28 ʱ�����
#define I2C_SCL0_PORT                (PORT0)						                  // �˿�  
#define I2C_SCL0_PIN       	         (28)							                  // �ܽ�

// ---------------------------- I2C1 -------------------------------------------------
#if (I2C1_IO_SELECT == 0x00)
#define I2C_SDA1_FUN                 (PIO0_0_SDA1)    			                      // P0.0 �������� 
#define I2C_SDA1_PORT                (PORT0)						                  // �˿� 
#define I2C_SDA1_PIN       	         (0)							                  // �ܽ�

#define I2C_SCL1_FUN                 (PIO0_1_SCL1)    			                      // P0.1 ʱ�����
#define I2C_SCL1_PORT                (PORT0)						                  // �˿�  
#define I2C_SCL1_PIN       	         (1)							                  // �ܽ�

#else
#define I2C_SDA1_FUN                 (PIO0_19_SDA1)    			                      // P0.19 ��������
#define I2C_SDA1_PORT                (PORT0)						                  // �˿�  
#define I2C_SDA1_PIN       	         (19)							                  // �ܽ�

#define I2C_SCL1_FUN                 (PIO0_20_SCL1)    			                      // P0.20 ʱ�����
#define I2C_SCL1_PORT                (PORT0)						                  // �˿� 
#define I2C_SCL1_PIN       	         (20)							                  // �ܽ�
#endif

// ---------------------------- I2C2 -------------------------------------------------
#define I2C_SDA2_FUN                 (PIO0_10_SDA2)    			                      // P0.10 ��������
#define I2C_SDA2_PORT                (PORT0)						                  // �˿�  
#define I2C_SDA2_PIN       	         (10)							                  // �ܽ�

#define I2C_SCL2_FUN                 (PIO0_11_SCL2)    			                      // P0.11 ʱ����� 
#define I2C_SCL2_PORT                (PORT0)						                  // �˿� 
#define I2C_SCL2_PIN       	         (11)							                  // �ܽ�

/******************************************************************************************************** 
* ��ö�ٱ����� Enumerations                                                
********************************************************************************************************/
typedef enum
{
    I2C0 = 0,
	I2C1 = 1,
	I2C2 = 2,

} I2C_NUM;																			  // I2C�ӿ�

/******************************************************************************************************** 
* ���Ĵ����� Register                                              
********************************************************************************************************/

// ������λ�Ĵ�����I2CONSET-0x4001 C000��
#define I2CONSET_AA			         (0x01 << 2)									  // Ӧ���־
#define I2CONSET_SI		         	 (0x01 << 3)									  // I2C�жϱ�־
#define I2CONSET_STO		         (0x01 << 4)									  // ֹͣ��־
#define I2CONSET_STA		         (0x01 << 5)									  // ��ʼ��־
#define I2CONSET_I2EN		         (0x01 << 6)                                      // I2C�ӿ�ʹ��

// ����Ĵ�����I2CONCLR-0x4001 C018��
#define I2CONCLR_AAC		         (0x01 << 2)                                      // Ӧ���־����λ
#define I2CONCLR_SIC		         (0x01 << 3)                                      // I2C�жϱ�־����λ
#define I2CONCLR_STAC		         (0x01 << 5)                                      // ��ʼ��־����λ
#define I2CONCLR_I2ENC		         (0x01 << 6)                                      // I2C�ӿڽ�ֹλ

// ���ݼĴ�����I2DAT-0x4001 C008��
#define I2DAT_I2C			         (0x00)                                           // �üĴ��������ѽ��յ����ݻ�Ҫ���͵����� 

// �ӻ���ַ�Ĵ�����I2ADR-0x4001 C00C��
#define I2ADR_I2C			         (0x00)                                           // I2C �ӻ���ַ

/********************************************************************************************************
* ���ڲ������� internal function                                              
********************************************************************************************************/
static void I2C_DelayMs(u16 t);										                  // ��ʱ
static void I2C_SetGpio(u8 num);									                  // �˿�����
static void I2C_Start(u8 num);								                          // �������ź�
static void I2C_Stop(u8 num);    							                          // ��ֹͣ�ź�
static u8   I2C_WriteByte(u8 num, u8 dat, u8 mode); 		                          // ����һ�ֽ�����
static u8   I2C_ReadByte(u8 num, u8 last); 				                              // ��ȡ����

/********************************************************************************************************
* ��ȫ�ֺ����� global function                                                 
********************************************************************************************************/
extern u8   I2CInit(u8 num, u32 fi2c);							                      // ��ʼ��IIC
extern void I2CWriteByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 dat);	              // дһ�ֽ��ֽ�
extern void I2CWriteNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 len);       // ���ֽ�д��
extern u8   I2CReadByte(u8 num, u8 icAddr, u8 type, u8 addr);                         // ��ȡһ�ֽ�
extern void I2CReadNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 len);        // ���ֽڶ�ȡ

/********************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
