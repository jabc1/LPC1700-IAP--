/********************************************************************************************************    
*                                                                        
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : st7920.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*				   ��λ�˿ڿ�������
*
********************************************************************************************************/

#ifndef __ST7920_H
#define __ST7920_H

/********************************************************************************************************
*                              Pin Definition                                                 
********************************************************************************************************/
#define LCM_D0_FUN             (PIO1_19_GPIO)    				                 // P1.19     
#define LCM_D0_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D0_PIN       	   (19)								                 // �ܽ�

#define LCM_D1_FUN             (PIO1_20_GPIO)    				                 // P1.20     
#define LCM_D1_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D1_PIN       	   (20)								                 // �ܽ�

#define LCM_D2_FUN             (PIO1_21_GPIO)    				                 // P1.21     
#define LCM_D2_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D2_PIN       	   (21)								                 // �ܽ�

#define LCM_D3_FUN             (PIO1_22_GPIO)    				                 // P1.22     
#define LCM_D3_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D3_PIN       	   (22)								                 // �ܽ�

#define LCM_D4_FUN             (PIO1_23_GPIO)    				                 // P1.23     
#define LCM_D4_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D4_PIN       	   (23)								                 // �ܽ�

#define LCM_D5_FUN             (PIO1_24_GPIO)    				                 // P1.24     
#define LCM_D5_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D5_PIN       	   (24)								                 // �ܽ�

#define LCM_D6_FUN             (PIO1_25_GPIO)    				                 // P1.25     
#define LCM_D6_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D6_PIN       	   (25)								                 // �ܽ�

#define LCM_D7_FUN             (PIO1_26_GPIO)    				                 // P1.26     
#define LCM_D7_PORT       	   (PORT1)							                 // �˿� 
#define LCM_D7_PIN       	   (26)								                 // �ܽ�

#define LCM_DAT_PORT       	   (PORT1)							                 // �˿�  
#define LCM_DAT_PIN       	   (19)						                         // ���ݹܽ���ʼ
#define LCM_DAT       	       (0xFF << LCM_DAT_PIN)					         // �ܽ� P1.19~P1.26

//-------------------------------------------------------------------------------------------------------

#define LCM_EN_FUN             (PIO1_27_GPIO)    				                 // P1.27 
#define LCM_EN_PORT       	   (PORT1)							                 // �˿�  
#define LCM_EN_PIN       	   (27)								                 // �ܽ�

#define LCM_RW_FUN             (PIO1_28_GPIO)    				                 // P1.28 
#define LCM_RW_PORT       	   (PORT1)							                 // �˿�  
#define LCM_RW_PIN       	   (28)								                 // �ܽ�

#define LCM_RS_FUN             (PIO1_29_GPIO)    				                 // P1.29 
#define LCM_RS_PORT       	   (PORT1)							                 // �˿�  
#define LCM_RS_PIN       	   (29)								                 // �ܽ�

#define LCM_PS_FUN             (PIO1_31_GPIO)    				                 // P1.31 
#define LCM_PS_PORT       	   (PORT1)							                 // �˿�  
#define LCM_PS_PIN       	   (31)								                 // �ܽ�

#define LCM_RT_FUN             (PIO3_25_GPIO)    				                 // P3.25 
#define LCM_RT_PORT       	   (PORT3)							                 // �˿�  
#define LCM_RT_PIN       	   (25)								                 // �ܽ�

#define LCM_BK_FUN             (PIO3_26_GPIO)    				                 // P3.26 
#define LCM_BK_PORT       	   (PORT3)							                 // �˿�  
#define LCM_BK_PIN       	   (26)								                 // �ܽ�

/********************************************************************************************************
* ���ܽŷ��� Pin direction                                                
********************************************************************************************************/
#define LCM_DAT_OUT()          GPIOSetMultiDir(LCM_DAT_PORT, LCM_DAT, GPIO_OUT)	 // ���ö˿ڷ���

#define LCM_EN_OUT()           GPIOSetDir(LCM_EN_PORT, LCM_EN_PIN, GPIO_OUT)  	 // ʹ��
#define LCM_RW_OUT()           GPIOSetDir(LCM_RW_PORT, LCM_RW_PIN, GPIO_OUT)	 // ��д
#define LCM_RS_OUT()           GPIOSetDir(LCM_RS_PORT, LCM_RS_PIN, GPIO_OUT)	 // ����/ָ��ѡ��
#define LCM_BK_OUT()           GPIOSetDir(LCM_BK_PORT, LCM_BK_PIN, GPIO_OUT)	 // ����
#define LCM_PS_OUT()           GPIOSetDir(LCM_PS_PORT, LCM_PS_PIN, GPIO_OUT)	 // PSB
#define LCM_RT_OUT()           GPIOSetDir(LCM_RT_PORT, LCM_RT_PIN, GPIO_OUT)     // ��λ

/********************************************************************************************************
* ���ܽ�д�� Pin Write                                                
********************************************************************************************************/
#define LCM_DAT_WRITE(dat)	   GPIOSetMultiValue(LCM_DAT_PORT, LCM_DAT, dat<<LCM_DAT_PIN)	 

#define LCM_EN_WRITE(dat)      GPIOSetValue(LCM_EN_PORT, LCM_EN_PIN, dat)	     // �˿����
#define LCM_RW_WRITE(dat)      GPIOSetValue(LCM_RW_PORT, LCM_RW_PIN, dat)
#define LCM_RS_WRITE(dat)      GPIOSetValue(LCM_RS_PORT, LCM_RS_PIN, dat)	    
#define LCM_BK_WRITE(dat)      GPIOSetValue(LCM_BK_PORT, LCM_BK_PIN, dat)
#define LCM_PS_WRITE(dat)      GPIOSetValue(LCM_PS_PORT, LCM_PS_PIN, dat)	     
#define LCM_RT_WRITE(dat)      GPIOSetValue(LCM_RT_PORT, LCM_RT_PIN, dat)         

/********************************************************************************************************
*                               Macro Definition                                                 
********************************************************************************************************/
#define LCM_BASIC_INS          (0x30)									         // ����ָ��
#define LCM_EXTEN_INS          (0x36)									         // ��չָ��

#define LCM_CLR_SCREEN		   (0x01)									         // ����
#define LCM_DISPLAY_ON         (0x0C)									         // ��ʾ��

/****************************** LCM128*64��ַ��� ******************************************************/
#define LCM_LINE1              (0x00)                                            // ��һ�� 
#define LCM_LINE2              (0x01)                                            // �ڶ���
#define LCM_LINE3              (0x02)                                            // ������
#define LCM_LINE4              (0x03)                                            // ������
#define LCM_LINE_ALL           (0x04)                                            //	ȫ��

#define LCM_LINE_MAX           (4)                                               // �������
#define LCM_LINE_LENGTH	   	   (16)										         // ÿ�������ʾ16���ַ�

#define LCM_LINE1_HEAD         (0x80)                                            // ��һ��DDRAM��ʼ��ַ 
#define LCM_LINE2_HEAD         (0x90)                                            // �ڶ���DDRAM��ʼ��ַ
#define LCM_LINE3_HEAD         (0x88)                                            // ������DDRAM��ʼ��ַ
#define LCM_LINE4_HEAD         (0x98)                                            // ������DDRAM��ʼ��ַ

/********************************* ���� ****************************************************************/
#define LCM_BLK_ON			   (0x01)									         // ���⿪
#define LCM_BLK_OFF			   (0x00)									         // �����

#define LCM_PSB_PRL			   (0x01)									         // ����ͨѶ
#define LCM_PSB_SRL			   (0x00)									         // ����ͨѶ

//-------------------------------------------------------------------------------------------------------
typedef enum
{
    LCM_DIS_NML = 0x0000,					                                     // ��ͨ��ʾ
	LCM_DIS_RVS = 0xFFFF,					                                     // ������ʾ

} LCM_DIS_MODE;														             // ��ʾģʽ

/********************************************************************************************************
*                               Internal Function                                                  
********************************************************************************************************/
static void LCM_Delay(u16 us);							                         // ��ʱ
static void LCM_Reset(void);												     // ��λ
static void LCM_WriteCmd(u8 cmd);					                             // д��ָ��
static void LCM_WriteData(u8 dat);						                         // д������

/********************************************************************************************************
*                               Global Function                                                 
********************************************************************************************************/
extern void LCMInit(void);									                     // ��ʼ��
extern void LCMClear(u8 lin, u16 rvsBit);										 // ����
extern u8   LCMDisplayStr(u8 x, u8 y, u8 *pStr);	                             // ��ʾ�ַ���
extern void LCMDisplayPicture(u8 *img);					                         // ��ʾͼƬ
extern void LCMBacklightCtrl(u8 blk);											 // �رձ���

extern void LCMDispTempSym(u8 x, u8 y);											 // ��ʾ�¶ȷ���
extern void LCMDisplayStr1(u8 x, u8 y, u16 rvsBit, u8 *pStr);			         // ��ʾ�ַ���

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                               End Of File                                                  
********************************************************************************************************/
