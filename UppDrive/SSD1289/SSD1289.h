/**************************************************************************************    
*                       Crazy ARM LPC2148 V2.0 ������ʵ�����                                                 
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : SSD1289.h
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V3.70
* Description    :
*
*
**************************************************************************************/

#ifndef __SSD1289_H 
#define __SSD1289_H

/**************************************************************************************
*                               PinDefinition                                                 
**************************************************************************************/
/*   0 - 8λģʽ��1 - 16λģʽ   */
#define	  TFT_DATA_MODE	  (1)							   // 16λģʽ����8λģʽ

/**************************************************************************************
*                               PinDefinition                                                 
**************************************************************************************/
#define   TFT_CS          (16)                             // P1.16 Ƭѡ
#define   TFT_RS          (17)                             // P1.17 ָ��/����ѡ�� 0 - ָ�1 - ����
#define   TFT_WR          (18)                             // P1.18
#define   TFT_RD          (19)                             // P1.19
#define   TFT_LE          (23)                             // P1.23 ����
#define   TFT_RST         (20)                             // P1.20 ��λ
#define   TFT_BLK         (21)                             // P1.21 �������	
#define   TFT_DAT         (0xFFFF)                         // P0.0~P0.15

#define   TFT_CS_DDR()    (IO1DIR)						   // �������
#define   TFT_RS_DDR()    (IO1DIR)
#define   TFT_WR_DDR()    (IO1DIR)
#define   TFT_RD_DDR()    (IO1DIR)
#define   TFT_LE_DDR()    (IO1DIR)
#define   TFT_RST_DDR()   (IO1DIR)
#define   TFT_BLK_DDR()   (IO1DIR)
#define   TFT_DAT_DDR()   (IO0DIR)
										  
#define   TFT_CS_SET()    (IO1SET |= (1<<TFT_CS)) 
#define   TFT_CS_CLR()    (IO1CLR |= (1<<TFT_CS))

#define   TFT_RS_SET()    (IO1SET |= (1<<TFT_RS)) 
#define   TFT_RS_CLR()    (IO1CLR |= (1<<TFT_RS))

#define   TFT_WR_SET()    (IO1SET |= (1<<TFT_WR)) 
#define   TFT_WR_CLR()    (IO1CLR |= (1<<TFT_WR))

#define   TFT_RD_SET()    (IO1SET |= (1<<TFT_RD)) 
#define   TFT_RD_CLR()    (IO1CLR |= (1<<TFT_RD))

#define   TFT_LE_SET()    (IO1SET |= (1<<TFT_LE)) 
#define   TFT_LE_CLR()    (IO1CLR |= (1<<TFT_LE))

#define   TFT_RST_SET()   (IO1SET |= (1<<TFT_RST)) 
#define   TFT_RST_CLR()   (IO1CLR |= (1<<TFT_RST))

#define   TFT_BLK_ON()    (IO1SET |= (1<<TFT_BLK)) 		    // ���⿪
#define   TFT_BLK_OFF()   (IO1CLR |= (1<<TFT_BLK))			// �����

//        ******    д����16λ
#define   TFT_WRITE_WORD_DATA(dat)     	\
          {						  	    \
		      IO0CLR = TFT_DAT;		    \
			  IO0SET = dat;			    \
		  }

//        ******    д����8λ
#define   TFT_WRITE_BYTE_DATA(dat)     	\
          {						  	    \
		      IO0CLR = 0xFF00;		    \
			  IO0SET = dat;			    \
		  }			  	

#define   TFT_READ_WORD_DATA()  (IO0PIN & 0xFFFF)    			// ��16λ����
#define   TFT_READ_BYTE_DATA()  (IO0PIN & 0xFF00)    			// ��8λ����	 

/**************************************************************************************
*                               InternalFunction                                                  
**************************************************************************************/
#define   RED             (0xF800)						    // ��ɫ
#define   GREEN           (0x03E0)						    // ��ɫ
#define   BLUE            (0x001F) 					        // ��ɫ
#define   WRITE           (0xFFFF)						    // ��ɫ
#define   BLACK           (0x0000)						    // ��ɫ
#define	  ORANGE          (0xF860)					        // ��ɫ
#define	  YELLOW		  (0xFFE0)						    // ��ɫ
#define	  CYAN			  (0x07FF)						    // ��ɫ
#define	  PROPLE      	  (0xF81F)						    // ��ɫ
#define   NOCOLOR         (0x0001)            			    // ��ɫ

#define   WIDTH			  (320)						        // ����
#define   HEIGTH		  (240)						        // ����

/**************************************************************************************
*                               InternalFunction                                                  
**************************************************************************************/
static void   TFT_SetGPIO(void);				            // ���ö˿ڷ���
static void   TFT_Delay(uint32 nCount); 	                // Һ����ʱ
static uint16 TFT_CheckContronller(void);	                // 
static void   TFT_Reset(void);					            // ��λҺ��
static void   TFT_WriteRegister(uint16 index,uint16 dat);   // д�Ĵ���
static uint16 TFT_ReadRegister(uint16 index);			    // ���Ĵ���
static uint16 TFT_ReadData(void);						    // д����
static void   TFT_WriteData(uint16 dat);				    // ������
static void   TFT_SetCursor(uint16 x,uint16 y);			    // ��������
static void   TFT_Clear(uint16 dat);					    // ����
static uint16 TFT_BGR2RGB(uint16 c);					    // ��ɫת��
static void   TFT_WriteIndex(uint16 idx);				    // д��ַ
static void   TFT_SetWindows(uint16 startX,uint16 startY,uint16 endX,uint16 endY);	         // ���ô���
static void   TFT_PutChar(uint16 x,uint16 y,uint8 *asciiCode,uint16 charColor,uint16 bkColor);// ��ʾ�����ַ�
static void   TFT_PutChinese(uint16 x,uint16 y,uint8* ptr,uint16 charColor,uint16 bkColor);	 // ��ʾ����

/**************************************************************************************
*                               GlobalFunction                                                 
**************************************************************************************/
extern void   TFTInit(uint16 color);                        // ��ʼ��TFT
extern void   TFTTest(void);                                // ����TFT
extern void   TFTBackLight(uint8 status);                	// �������
extern uint16 TFTGetPoint(uint16 x,uint16 y);               // ��ȡָ��������ɫֵ
extern void   TFTSetPoint(uint16 x,uint16 y,uint16 color);	// ָ�����껭��
extern void   TFTPutStr(uint16 x,uint16 y,uint8 *ptr,uint16 charColor,uint16 bkColor);  // ��ʾ�ַ���
extern void   TFTDrawPicture(uint16 startX,uint16 startY,uint16 endX,uint16 endY,uint8 const *pic);	// ��ʾͼƬ
extern void   TFTDrawLine(uint16 startX,uint16 startY,uint8 endX,uint16 endY,uint16 color);         // ����

/*************************************************************************************/
#endif

/**************************************************************************************
*                               End Of File                                                  
**************************************************************************************/
