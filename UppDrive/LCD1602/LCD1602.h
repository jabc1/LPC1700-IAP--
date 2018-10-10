/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : lcd1602.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

#ifndef __LCD1602_H
#define __LCD1602_H

/********************************************************************************************************
* Pin Definition                                                 
********************************************************************************************************/
#define LCD_D0_FUN                (PIO1_19_GPIO)    				        // P1.19     
#define LCD_D0_PORT       	      (PORT1)							        // �˿� 
#define LCD_D0_PIN       	      (19)								        // �ܽ�

#define LCD_D1_FUN                (PIO1_20_GPIO)    				        // P1.20     
#define LCD_D1_PORT       	      (PORT1)							        // �˿� 
#define LCD_D1_PIN       	      (20)								        // �ܽ�

#define LCD_D2_FUN                (PIO1_21_GPIO)    				        // P1.21     
#define LCD_D2_PORT       	      (PORT1)							        // �˿� 
#define LCD_D2_PIN       	      (21)								        // �ܽ�

#define LCD_D3_FUN                (PIO1_22_GPIO)    				        // P1.22     
#define LCD_D3_PORT       	      (PORT1)							        // �˿� 
#define LCD_D3_PIN       	      (22)								        // �ܽ�

#define LCD_D4_FUN                (PIO1_23_GPIO)    				        // P1.23     
#define LCD_D4_PORT       	      (PORT1)							        // �˿� 
#define LCD_D4_PIN       	      (23)								        // �ܽ�

#define LCD_D5_FUN                (PIO1_24_GPIO)    				        // P1.24     
#define LCD_D5_PORT       	      (PORT1)							        // �˿� 
#define LCD_D5_PIN       	      (24)								        // �ܽ�

#define LCD_D6_FUN                (PIO1_25_GPIO)    				        // P1.25     
#define LCD_D6_PORT       	      (PORT1)							        // �˿� 
#define LCD_D6_PIN       	      (25)								        // �ܽ�

#define LCD_D7_FUN                (PIO1_26_GPIO)    				        // P1.26     
#define LCD_D7_PORT       	      (PORT1)							        // �˿� 
#define LCD_D7_PIN       	      (26)								        // �ܽ�

#define LCD_DAT_PORT       	      (PORT1)							        // �˿�  
#define LCD_DAT_PIN       	      (19)						                // ���ݹܽ���ʼ
#define LCD_DAT       	          (0xFF << LCD_DAT_PIN)					    // �ܽ� P1.19~P1.26

//-------------------------------------------------------------------------------------------------------

#define LCD_EN_FUN                (PIO1_27_GPIO)    				        // P1.27 
#define LCD_EN_PORT       	      (PORT1)							        // �˿�  
#define LCD_EN_PIN       	      (27)								        // �ܽ�

#define LCD_RW_FUN                (PIO1_28_GPIO)    				        // P1.28 
#define LCD_RW_PORT       	      (PORT1)							        // �˿�  
#define LCD_RW_PIN       	      (28)								        // �ܽ�

#define LCD_RS_FUN                (PIO1_29_GPIO)    				        // P1.29 
#define LCD_RS_PORT       	      (PORT1)							        // �˿�  
#define LCD_RS_PIN       	      (29)								        // �ܽ�

#define LCD_BK_FUN                (PIO3_26_GPIO)    				        // P3.26 
#define LCD_BK_PORT       	      (PORT3)							        // �˿�  
#define LCD_BK_PIN       	      (26)								        // �ܽ�

/********************************************************************************************************
* ���ܽŷ��� Pin direction                                                
********************************************************************************************************/
#define LCD_DAT_OUT()             GPIOSetMultiDir(LCD_DAT_PORT, LCD_DAT, GPIO_OUT)	 // ���ö˿ڷ���

#define LCD_EN_OUT()              GPIOSetDir(LCD_EN_PORT, LCD_EN_PIN, GPIO_OUT)  	 // ʹ��
#define LCD_RW_OUT()              GPIOSetDir(LCD_RW_PORT, LCD_RW_PIN, GPIO_OUT)	     // ��д
#define LCD_RS_OUT()              GPIOSetDir(LCD_RS_PORT, LCD_RS_PIN, GPIO_OUT)	     // ����/ָ��ѡ��
#define LCD_BK_OUT()              GPIOSetDir(LCD_BK_PORT, LCD_BK_PIN, GPIO_OUT)	     // ����

/********************************************************************************************************
* ���ܽ�д�� Pin Write                                                
********************************************************************************************************/
#define LCD_DAT_WRITE(dat)	      GPIOSetMultiValue(LCD_DAT_PORT, LCD_DAT, dat<<LCD_DAT_PIN)	 

#define LCD_EN_WRITE(dat)         GPIOSetValue(LCD_EN_PORT, LCD_EN_PIN, dat)	     // �˿����
#define LCD_RW_WRITE(dat)         GPIOSetValue(LCD_RW_PORT, LCD_RW_PIN, dat)
#define LCD_RS_WRITE(dat)         GPIOSetValue(LCD_RS_PORT, LCD_RS_PIN, dat)	    
#define LCD_BK_WRITE(dat)         GPIOSetValue(LCD_BK_PORT, LCD_BK_PIN, dat)         

/********************************************************************************************************
*                               MacroDefinition                                                 
********************************************************************************************************/
// ���뷽ʽ����
#define LCD_AC_AUTO_INCREMENT     (0x06)                                    // ���ݶ���д������,AC�Զ���һ
#define LCD_AC_AUTO_DECREASE      (0x04)                                    // ���ݶ���д������,AC�Զ���һ
#define LCD_MOVE_ENABLE           (0x05)                                    // ���ݶ���д����,����ƽ��
#define LCD_MOVE_DISENABLE        (0x04)                                    // ���ݶ���д����,���治��
#define LCD_GO_HOME               (0x02)                                    // AC=0,��ꡢ�����HOMEλ

// ������ʾ����꼰��˸������
#define LCD_DISPLAY_ON            (0x0C)                                    // ��ʾ��
#define LCD_DISPLAY_OFF           (0x08)                                    // ��ʾ��
#define LCD_CURSOR_ON             (0x0A)                                    // �����ʾ
#define LCD_CURSOR_OFF            (0x08)                                    // ��겻��ʾ
#define LCD_CURSOR_BLINK_ON       (0x09)                                    // �����˸
#define LCD_CURSOR_BLINK_OFF      (0x08)                                    // ��겻��˸

// ��ꡢ�����ƶ�,��Ӱ��DDRAM
#define LCD_LEFT_MOVE             (0x18)                                    // LCD��ʾ����һλ
#define LCD_RIGHT_MOVE            (0x1C)                                    // LCD��ʾ����һλ
#define LCD_CURSOR_LEFT_MOVE      (0x10)                                    // �������һλ
#define LCD_CURSOR_RIGHT_MOVE     (0x14)                                    // �������һλ

// ������ʽ����
#define LCD_DISPLAY_DOUBLE_LINE   (0x38)                                    // ������ʾ
#define LCD_DISPLAY_SINGLE_LINE   (0x30)                                    // ������ʾ
#define LCD_CLEAR_SCREEN          (0x01)                                    // ����
											   
/************************* LCD1602��ַ��� *************************************************************/
#define LCD_LINE1_HEAD            (0x80)                                    // ��һ��DDRAM��ʼ��ַ 
#define LCD_LINE2_HEAD            (0xC0)                                    // �ڶ���DDRAM��ʼ��ַ
#define LCD_LINE1                 (0x00)                                    // ��һ��
#define LCD_LINE2                 (0x01)                                    // �ڶ���

#define LCD_LINE_MAX              (2)                                       // �������
#define LCD_LINE_LENGTH			  (16)										// ÿ�������ʾ16���ַ�

#define LCD_BLK_ON				  (0x01)									// ���⿪
#define LCD_BLK_OFF				  (0x00)									// �����

/********************************************************************************************************
*                               InternalFunction                                                  
********************************************************************************************************/
static void LCD_Delay(u16 us);								                // Һ����ʱ�������״̬��
static void LCD_WriteCommand(u8 cmd);					                    // дָ��
static void LCD_WriteData(u8 dat);                                          // д����

/********************************************************************************************************
*                               GlobalFunction                                                 
********************************************************************************************************/
extern void LCDInit(void);	                                                // ��ʼ��LCD
extern u8   LCDDisplayChar(u8 x, u8 y, u8 ch);	                            // ��ʾ�����ַ�
extern u8   LCDDisplayStr(u8 x, u8 y, u8 *pStr);	                        // ��ʾ�ַ���

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                               End Of File                                                  
********************************************************************************************************/
