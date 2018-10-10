/**************************************************************************************
*                       Crazy ARM LPC2148 V2.0 ������ʵ�����                                                 
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
#define ADS_SCK          (17) 					        // P0.17	 ʱ��
#define ADS_DOUT         (18)					        // P0.18	 ���˿�
#define ADS_DIN          (19)					        // P0.19	 д�˿�
#define ADS_CS           (21)					        // P0.21	 Ƭѡ
#define ADS_INT          (22)		                    // P1.22	 �ж�

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
// A/D ͨ��ѡ�������ֺ͹����Ĵ���
#define	CHX 	         (0x90) 	                    // ͨ��Y+��ѡ������� 0x94
#define	CHY 	         (0xD0)	                        // ͨ��X+��ѡ������� 0xD4

/* ����������ͨ����ζ�ȡ�������ó��� */
#define Tch_Xmax         (0x0F68)						// ������X��������
#define Tch_Xmin         (0x012C)						// ������X����С���

#define Tch_Ymax         (0x0F03)						// ������Y��������
#define Tch_Ymin         (0x00C0) 						// ������Y����С���

/* ��������ΪҺ����ʵ�ʴ�С */
#define Tch_LCDWidth     (320)							// LCD���
#define Tch_LCDHeight    (240)					   	    // LCD�߶�

/**************************************************************************************
*                               Internal Function                                                  
**************************************************************************************/
static void   ADS_Delay(uint8 t);                       // ��ʱ����
static void   ADS_Start(void);							// ��ʼISP����
static uint16 ADS_Average(uint16 *p);                   // ��ȡƽ��ֵ
static uint16 ADS_Read(void);					        // ������
static void   ADS_Wirte(uint8 dat);				        // д����
static uint32 ADS_Coordinate(void);                     // ��������ֵ

/**************************************************************************************
*                               Global Function                                                 
**************************************************************************************/
extern void   ADSInit(void);                            // ��ʼ��ADC7843
extern void   ADSGetConversion(uint16 *px,uint16 *py);  // ��ȡת������

/*************************************************************************************/
#endif

/**************************************************************************************
*                               End Of File                                                  
**************************************************************************************/
