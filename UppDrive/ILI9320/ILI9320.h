/********************************************************************************************************    
*                                                                        
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ILI9320.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

#ifndef __ILI9320_H 
#define __ILI9320_H

/********************************************************************************************************
* ���ܽŶ��塿 pin definition                                                
********************************************************************************************************/
#define TFT_D0_FUN            (PIO1_19_GPIO)    				         // P1.19     
#define TFT_D0_PORT       	  (PORT1)							         // �˿� 
#define TFT_D0_PIN       	  (19)								         // �ܽ�

#define TFT_D1_FUN            (PIO1_20_GPIO)    				         // P1.20     
#define TFT_D1_PORT       	  (PORT1)							         // �˿� 
#define TFT_D1_PIN       	  (20)								         // �ܽ�

#define TFT_D2_FUN            (PIO1_21_GPIO)    				         // P1.21     
#define TFT_D2_PORT       	  (PORT1)							         // �˿� 
#define TFT_D2_PIN       	  (21)								         // �ܽ�

#define TFT_D3_FUN            (PIO1_22_GPIO)    				         // P1.22     
#define TFT_D3_PORT       	  (PORT1)							         // �˿� 
#define TFT_D3_PIN       	  (22)								         // �ܽ�

#define TFT_D4_FUN            (PIO1_23_GPIO)    				         // P1.23     
#define TFT_D4_PORT       	  (PORT1)							         // �˿� 
#define TFT_D4_PIN       	  (23)								         // �ܽ�

#define TFT_D5_FUN            (PIO1_24_GPIO)    				         // P1.24     
#define TFT_D5_PORT       	  (PORT1)							         // �˿� 
#define TFT_D5_PIN       	  (24)								         // �ܽ�

#define TFT_D6_FUN            (PIO1_25_GPIO)    				         // P1.25     
#define TFT_D6_PORT       	  (PORT1)							         // �˿� 
#define TFT_D6_PIN       	  (25)								         // �ܽ�

#define TFT_D7_FUN            (PIO1_26_GPIO)    				         // P1.26     
#define TFT_D7_PORT       	  (PORT1)							         // �˿� 
#define TFT_D7_PIN       	  (26)								         // �ܽ�

#define TFT_DAT_L_PORT        (PORT1)							         // �˿�  
#define TFT_DAT_L_PIN         (19)						                 // ���ݹܽ���ʼ
#define TFT_DAT_L       	  (0xFF << TFT_DAT_L_PIN)				     // �ܽ� P1.19~P1.26

//-------------------------------------------------------------------------------------------------------
#define TFT_D8_FUN            (PIO0_15_GPIO)    				         // P0.15     
#define TFT_D8_PORT       	  (PORT0)							         // �˿� 
#define TFT_D8_PIN       	  (15)								         // �ܽ�

#define TFT_D9_FUN            (PIO0_16_GPIO)    				         // P0.16     
#define TFT_D9_PORT       	  (PORT0)							         // �˿� 
#define TFT_D9_PIN       	  (16)								         // �ܽ�

#define TFT_D10_FUN           (PIO0_17_GPIO)    				         // P0.17     
#define TFT_D10_PORT       	  (PORT0)							         // �˿� 
#define TFT_D10_PIN       	  (17)								         // �ܽ�

#define TFT_D11_FUN           (PIO0_18_GPIO)    				         // P0.18     
#define TFT_D11_PORT       	  (PORT0)							         // �˿� 
#define TFT_D11_PIN       	  (18)								         // �ܽ�

#define TFT_D12_FUN           (PIO0_19_GPIO)    				         // P0.19     
#define TFT_D12_PORT       	  (PORT0)							         // �˿� 
#define TFT_D12_PIN       	  (23)								         // �ܽ�

#define TFT_D13_FUN           (PIO0_20_GPIO)    				         // P0.20     
#define TFT_D13_PORT       	  (PORT0)							         // �˿� 
#define TFT_D13_PIN       	  (20)								         // �ܽ�

#define TFT_D14_FUN           (PIO0_21_GPIO)    				         // P0.21     
#define TFT_D14_PORT       	  (PORT0)							         // �˿� 
#define TFT_D14_PIN       	  (21)								         // �ܽ�

#define TFT_D15_FUN           (PIO0_22_GPIO)    				         // P0.22     
#define TFT_D15_PORT       	  (PORT0)							         // �˿� 
#define TFT_D15_PIN       	  (22)								         // �ܽ�

#define TFT_DAT_H_PORT        (PORT0)							         // �˿�  
#define TFT_DAT_H_PIN         (15)						                 // ���ݹܽ���ʼ
#define TFT_DAT_H       	  (0xFF << TFT_DAT_H_PIN)				     // �ܽ� P0.15~P0.22

//-------------------------------------------------------------------------------------------------------
#define TFT_CS_FUN            (PIO2_0_GPIO)    				             // P2.0 
#define TFT_CS_PORT       	  (PORT2)							         // �˿�  
#define TFT_CS_PIN       	  (0)								         // �ܽ�

#define TFT_RS_FUN            (PIO2_1_GPIO)    				             // P2.1 
#define TFT_RS_PORT       	  (PORT2)							         // �˿�  
#define TFT_RS_PIN       	  (1)								         // �ܽ�

#define TFT_WR_FUN            (PIO2_2_GPIO)    				             // P2.2 
#define TFT_WR_PORT       	  (PORT2)							         // �˿�  
#define TFT_WR_PIN       	  (2)								         // �ܽ�

#define TFT_RD_FUN            (PIO2_3_GPIO)    				             // P2.3 
#define TFT_RD_PORT       	  (PORT2)							         // �˿�  
#define TFT_RD_PIN       	  (3)								         // �ܽ�

#define TFT_RT_FUN            (PIO2_4_GPIO)    				             // P2.4 
#define TFT_RT_PORT       	  (PORT2)							         // �˿�  
#define TFT_RT_PIN       	  (4)								         // �ܽ�

#define TFT_EN_FUN            (PIO2_5_GPIO)    				             // P2.5 
#define TFT_EN_PORT       	  (PORT2)							         // �˿�  
#define TFT_EN_PIN       	  (5)								         // �ܽ�

//-------------------------------------------------------------------------------------------------------
#define TFT_SC_FUN            (PIO0_6_GPIO)    				             // P0.6 
#define TFT_SC_PORT       	  (PORT0)							         // �˿�  
#define TFT_SC_PIN       	  (6)								         // �ܽ�

#define TFT_INT_FUN           (PIO2_6_GPIO)    				             // P2.6 
#define TFT_INT_PORT       	  (PORT2)							         // �˿�  
#define TFT_INT_PIN       	  (6)								         // �ܽ�

#define TFT_LE_FUN            (PIO2_7_GPIO)    				             // P2.7 
#define TFT_LE_PORT       	  (PORT2)							         // �˿�  
#define TFT_LE_PIN       	  (7)								         // �ܽ�

#define TFT_FC_FUN            (PIO2_8_GPIO)    				             // P2.8 
#define TFT_FC_PORT       	  (PORT2)							         // �˿�  
#define TFT_FC_PIN       	  (8)								         // �ܽ�

#define TFT_TC_FUN            (PIO1_15_GPIO)    				         // P1.15 
#define TFT_TC_PORT       	  (PORT1)							         // �˿�  
#define TFT_TC_PIN       	  (15)								         // �ܽ�

//-------------------------------------------------------------------------------------------------------
#define   TFT_CS_OUT(dat)	  GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, dat) 
#define   TFT_RS_OUT(dat)	  GPIOSetValue(TFT_RS_PORT, TFT_RS_PIN, dat)
#define   TFT_WR_OUT(dat)	  GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, dat)
#define   TFT_RD_OUT(dat)	  GPIOSetValue(TFT_RD_PORT, TFT_RD_PIN, dat)
#define   TFT_RT_OUT(dat)	  GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, dat)
#define   TFT_EN_OUT(dat)	  GPIOSetValue(TFT_EN_PORT, TFT_EN_PIN, dat)

//-------------------------------------------------------------------------------------------------------
#define   TFT_DAT_OUT(dat)	  GPIOSetMultiValue(TFT_DAT_H_PORT, TFT_DAT_H, ((u8)(dat>>8)) << TFT_DAT_H_PIN);\
	                          GPIOSetMultiValue(TFT_DAT_L_PORT, TFT_DAT_L, (u8)(dat) << TFT_DAT_L_PIN);

/********************************************************************************************************
* ���궨�塿 macro definition                                                 
********************************************************************************************************/
typedef enum
{
    TFT_RED     = (0xF800),						                         // ��ɫ
    TFT_GREEN   = (0x03E0),						                         // ��ɫ
    TFT_BLUE    = (0x001F), 					                         // ��ɫ
    TFT_WRITE   = (0xFFFF),						                         // ��ɫ
    TFT_BLACK   = (0x0000),						                         // ��ɫ
    TFT_ORANGE  = (0xF860),					                             // ��ɫ
    TFT_YELLOW	= (0xFFE0),						                         // ��ɫ
    TFT_CYAN	= (0x07FF),						                         // ��ɫ
    TFT_PROPLE  = (0xF81F),						                         // ��ɫ
    TFT_NOCOLOR = (0x0001),            			                         // ��ɫ

} TFT_COLOR;

//-------------------------------------------------------------------------------------------------------
typedef enum
{
    TFT_WIDTH   = (240),						                         // ˮƽ����
    TFT_HEIGTH  = (320),						                         // ��ֱ����

} TFT_PIXELS;

//-------------------------------------------------------------------------------------------------------
typedef enum
{
     TFT_BLK_ON	 = (0x01),									             // ���⿪
     TFT_BLK_OFF = (0x00),									             // �����

} TFT_BACKLIGHT;

/********************************************************************************************************
* ���ⲿ������ External function                                              
********************************************************************************************************/
#define TFT_CN_SATRA_ADDR       W25Q_CN_START_ADDR						 // �����ֿ��׵�ַ
#define TFT_READ_CN_FONT(addr)	W25QReadByte(addr)						 // �������ֿ�
#define TFT_READ_EN_FONT(dat)	St8x16[dat]								 // �������ֿ�
			  
/********************************************************************************************************
* ���ڲ������� internal function                                              
********************************************************************************************************/
static void TFT_SetGpio(void);				                             // ���ö˿ڷ���
static void TFT_Delay(u32 nCount); 	                                     // Һ����ʱ                     
static void TFT_Reset(void);					                         // ��λҺ��
static void TFT_WriteRegister(u16 index, u16 dat);                       // д�Ĵ���
static void TFT_SetCursor(u16 x, u16 y);			                     // ��������
static void TFT_Clear(u16 col, u16 sx, u16 sy, u16 ex, u16 ey);		     // ����
static void TFT_RWReady(void);                                           // ��д׼��

/********************************************************************************************************
* ��ȫ�ֺ����� global function                                                 
********************************************************************************************************/
extern void TFTInit(u16 color);                                          // ��ʼ��TFT
extern void TFTBacklightCtrl(u8 blk);                	                 // �������
extern void TFTWriteData(u16 dat);				                         // д����
extern void TFTWriteIndex(u16 idx);				                         // д��ַ
extern void TFTSetWindows(u16 stX, u16 stY, u16 endX, u16 endY);         // ���ô���
extern void TFTSetCooOrigin(u8 gs, u8 ss);              	             // ����ԭ������
extern void TFTSetPoint(u16 x, u16 y, u16 color);	                     // ָ�����껭��

extern void TFTPutChar(u16 x, u16 y, u8 *pCh, u16 chClr, u16 bkClr);     // ��ʾ�����ַ�
extern void TFTPutChinese(u16 x, u16 y, u8 *ptr, u16 chClr, u16 bkClr);  // ��ʾ����
extern void TFTPutStr(u16 x, u16 y, u8 *ptr, u16 chClr, u16 bkClr);      // ��ʾ�ַ���
extern void TFTDrawPicture(u16 sx, u16 sy, u16 ex, u16 ey, uc8 *pic);    // ��ʾͼƬ

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                               End Of File                                                  
********************************************************************************************************/
