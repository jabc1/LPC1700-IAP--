/********************************************************************************************************    
*                                                                        
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ili9281.c
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V3.70
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
*                                  HeaderFiles                                                  
********************************************************************************************************/
#include "../UserCode/source/config.h"    

/********************************************************************************************************
* FunctionName   : TFT_DelayUs()
* Description    : TFT��ʱ
* EntryParameter : us - us
* ReturnValue    : None
********************************************************************************************************/
void TFT_DelayUs(u32 us) 
{
    u16 i;

	while (us--)
	{
	    for (i=14; i>0; i--)	 
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFT_SetGpio()
* Description    : TFT�˿ڷ�������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TFT_SetGpio(void)
{
    GPIOSetMultiDir(TFT_DAT_L_PORT, TFT_DAT_L, GPIO_OUT);					 // ���ݶ˿ڷ�������
	GPIOSetMultiDir(TFT_DAT_H_PORT, TFT_DAT_H, GPIO_OUT);

	GPIOSetDir(TFT_CS_PORT, TFT_CS_PIN, GPIO_OUT);			                 //
	GPIOSetDir(TFT_RS_PORT, TFT_RS_PIN, GPIO_OUT);			                 //  
	GPIOSetDir(TFT_WR_PORT, TFT_WR_PIN, GPIO_OUT);			                 // 
	GPIOSetDir(TFT_RD_PORT, TFT_RD_PIN, GPIO_OUT);			                 // 
	GPIOSetDir(TFT_RT_PORT, TFT_RT_PIN, GPIO_OUT);			                 // ��λ
	GPIOSetDir(TFT_EN_PORT, TFT_EN_PIN, GPIO_OUT);			                 // ����ʹ��

	// ���Թܽ���1
	TFTWriteData(0xFFFF);

	GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_SET);
	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_SET);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
	GPIOSetValue(TFT_RS_PORT, TFT_RS_PIN, GPIO_SET);
	GPIOSetValue(TFT_RD_PORT, TFT_RD_PIN, GPIO_SET);
	GPIOSetValue(TFT_EN_PORT, TFT_EN_PIN, GPIO_SET);

	TFTBacklightCtrl(TFT_BLK_OFF);										     // ����� 
}

/********************************************************************************************************
* FunctionName   : TFT_Reset()
* Description    : TFT��λ,	ע�⸴λʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TFT_Reset(void)
{
/*----------------------------------- ʱ��ͼ -----------------------------------------
--                                                                                  --
--                  ------------\_____________________/-----------                  --
--                              | <----  Tres  ---->  |                             --
--                                                                                  --
--                             Tres: Min.1ms                                        --
------------------------------------------------------------------------------------*/
    GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_SET);					   // ���Բ�Ҫ
	GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_CLR);					   // ���ֵ͵�ƽ1ms��λTFT
	TFT_DelayUs(1000);
	GPIOSetValue(TFT_RT_PORT, TFT_RT_PIN, GPIO_SET);	  				   // ���߳�����λ
}

/********************************************************************************************************
* FunctionName   : TFTBacklightCtrl()
* Description    : ���⿪��
* EntryParameter : blk - 1����0��
* ReturnValue    : None
********************************************************************************************************/
void TFTBacklightCtrl(u8 blk)
{
    GPIOSetValue(TFT_EN_PORT, TFT_EN_PIN, blk);
}

/********************************************************************************************************
* FunctionName   : TFTInit()
* Description    : ��ʼ��TFT
* EntryParameter : color - ��ʼ����ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFTInit(u16 color)
{
    TFT_SetGpio();					                     // ��ʼ�����ƶ˿�											    
	TFT_Reset();									     // ��λҺ��ģ��

	TFT_WriteIndex(EXT_SLP_MODE);						 // Exit_sleep_mode (11h)
	TFT_WriteIndex(ENT_NML_MODE);						 // Enter_normal_mode (13h)
	
	TFT_WriteIndex(POW_SET);						     // Power_Setting (D0h)
	TFTWriteData(0x0007);
	TFTWriteData(0x0040);   
	TFTWriteData(0x001C); 
	
	TFT_WriteIndex(VCOM_CTRL);							 // VCOM Control (D1h)
	TFTWriteData(0x0000);
	TFTWriteData(0x0018);
	TFTWriteData(0x001D);
	
	TFT_WriteIndex(POW_SET_NML_MODE);					 // Power_Setting for Normal Mode (D2h)
	TFTWriteData(0x0001);
	TFTWriteData(0x0011);
	
	TFT_WriteIndex(PNL_DRV_SET);						 // Panel Driving Setting (C0h)
	TFTWriteData(0x0000);
	TFTWriteData(0x003B);
	TFTWriteData(0x0000);
	TFTWriteData(0x0002);
	TFTWriteData(0x0011);
	
	TFT_WriteIndex(DISP_TIM_SET_NML_MODE);				 // Display_Timing_Setting for Normal Mode (C1h)
	TFTWriteData(0x0010);
	TFTWriteData(0x000B);
	TFTWriteData(0x0088);
	
	TFT_WriteIndex(FRM_RATE_INV_CTRL);					 // Frame Rate and Inversion Control (C5h)
	TFTWriteData(0x0001);
	
	TFT_WriteIndex(GMA_SET);							 // Gamma Setting (C8h)
	TFTWriteData(0x0000);
	TFTWriteData(0x0030);  
	TFTWriteData(0x0036);
	TFTWriteData(0x0045);
	TFTWriteData(0x0004);
	TFTWriteData(0x0016);
	TFTWriteData(0x0037);
	TFTWriteData(0x0075);
	TFTWriteData(0x0077);
	TFTWriteData(0x0054);
	TFTWriteData(0x000F);
	TFTWriteData(0x0000);
	
	TFTSetDisplayMode(0, 0, 1, 1, 0);					 // Set_address_mode (36h)
	
	TFT_WriteIndex(SET_PXL_FRMT);						 // Set_pixel_format (3Ah)
	TFTWriteData(0x0055); 
	
	TFT_DelayUs(48000);									 // ��ʱ���ܼ�����
	TFT_WriteIndex(SET_DISP_ON);						 // Set_display_on (29h)
	TFT_WriteIndex(WRITE_MMR_START);					 // Write_memory_start (2Ch)

    TFTSetDisplayMode(0, 0, 1, 0, 0);
	TFTClear(color);									 // ����
	TFTBacklightCtrl(TFT_BLK_ON);						 // ���⿪
}

/********************************************************************************************************
* FunctionName   : TFTWriteData()
* Description    : дTFT�Ĵ�������
* EntryParameter : dat - �Ĵ�������
* ReturnValue    : None
********************************************************************************************************/
void TFTWriteData(u16 dat)
{
    GPIOSetValue(TFT_RS_PORT, TFT_RS_PIN, GPIO_SET);				// д������
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);

    GPIOSetMultiValue(TFT_DAT_H_PORT, TFT_DAT_H, ((u8)(dat>>8)) << TFT_DAT_H_PIN);
	GPIOSetMultiValue(TFT_DAT_L_PORT, TFT_DAT_L, (u8)(dat) << TFT_DAT_L_PIN); 

	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_CLR);
	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_SET);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFT_WriteIndex()
* Description    : дTFT�Ĵ�����ַ,����ǰ����ѡ�п�����
* EntryParameter : idx - �Ĵ�����ַ
* ReturnValue    : None
********************************************************************************************************/
void TFT_WriteIndex(u16 idx)
{
    GPIOSetValue(TFT_RS_PORT, TFT_RS_PIN, GPIO_CLR);				  // д��ָ��
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);

    GPIOSetMultiValue(TFT_DAT_H_PORT, TFT_DAT_H, ((u8)(idx>>8)) << TFT_DAT_H_PIN);
	GPIOSetMultiValue(TFT_DAT_L_PORT, TFT_DAT_L, (u8)(idx) << TFT_DAT_L_PIN); 

	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_CLR);
	GPIOSetValue(TFT_WR_PORT, TFT_WR_PIN, GPIO_SET);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);    
}

/********************************************************************************************************
* FunctionName   : TFT_SetCursor()
* Description    : ������Ļ����
* EntryParameter : x - �����ꣻy - ������
* ReturnValue    : None
********************************************************************************************************/
void TFT_SetCursor(u16 x, u16 y)
{
	TFT_WriteIndex(0x002A);		 // Set_column_address (2Ah) < 320
	TFTWriteData(x >> 8);		 // 
	TFTWriteData(0x00FF & x);
	TFTWriteData(0x0001);
	TFTWriteData(0x00DF);

	TFT_WriteIndex(0x002B);		 // Set_page_address (2Bh) < 480
	TFTWriteData(y >> 8);
	TFTWriteData(0x00FF & y);
	TFTWriteData(0x0001);
	TFTWriteData(0x003F);
	
	TFT_WriteIndex(0x002C);				
}

/********************************************************************************************************
* FunctionName   : TFTSetPoint()
* Description    : ��ָ�����껭��
* EntryParameter : x - �����ꣻy - �����ꣻcolor - �����ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFTSetPoint(u16 x, u16 y, u16 color)
{
	TFT_SetCursor(x,y);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);

	TFT_WriteIndex(0x002C);
	TFTWriteData(color);
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTClear()
* Description    : ����Ļ����ָ������ɫ��������������� 0xFFFF
* EntryParameter : dat - ���ֵ
* ReturnValue    : None
********************************************************************************************************/
void TFTClear(u16 dat)
{
    u32 i;

	TFT_SetCursor(0, 0);														// ���ù������ʼ��
	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_CLR);
	TFT_WriteIndex(WRITE_MMR_START);

    for (i=0; i<TFT_WIDTH*TFT_HEIGTH; i++)										// ѭ��д������
	{
	    TFTWriteData(dat);
	}

	GPIOSetValue(TFT_CS_PORT, TFT_CS_PIN, GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTDrawPicture()
* Description    : ��ָ�����귶Χ��ʾһ��ͼƬ��ֻ����ʾȡģ��ͼƬ
* EntryParameter : startX,startY,heigth,width,*pic
* ReturnValue    : None
********************************************************************************************************/
void TFTDrawPicture(u16 startX, u16 startY, u16 heigth, u16 width, uc8 *pic)
{
    u32 i,k;
	u16 tmp;

	for (i=startY; i<startY+width; i++)
	{
	    TFT_SetCursor(startX,startY+i);
	    for (k=startX; k<startX+heigth; k++)
		{
		    tmp  = *pic++;                       		// ��8λ�����ݴ�
			tmp += (*pic++ << 8);					    // ȡ��8λ���ݣ����ϲ���16λ����
			TFTWriteData(tmp);
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFTPutChar()
* Description    : ��ָ��������ʾһ��8x16�����ascii�ַ�(ˮƽɨ�裬��λ����)
* EntryParameter : x - �����ꣻy - �����ꣻpCh - �ַ���chColor - �ַ�����ɫ��bkColor - �ַ�������ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFT_PutChar(u16 x,u16 y, u8 *pCh, u16 chColor, u16 bkColor)
{
    u16 i = 0;
	u16 j = 0;
    u8 tmpChar = 0;

	for (i=0; i<16; i++)
	{
	    tmpChar = St8x16[((*pCh-0x20)*16) + i];

		for (j=0; j<8; j++)
		{
		    if (((tmpChar >> (7-j)) & 0x01) == 0x01)
			{
			    TFTSetPoint(x+j ,y+i, chColor);	     // �ַ���ɫ
			}
			else
			{
			    if (bkColor != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+j, y+i, bkColor);	 // ������ɫ
				}
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFT_PutChinese()
* Description    : ��ָ��������ʾһ��16x16��������ĺ���(ˮƽɨ�裬��λ����)
* EntryParameter : x - �����ꣻy - �����ꣻchColor - �ַ�����ɫ��bkColor - �ַ�������ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFT_PutChinese(u16 x, u16 y, u8* pStr, u16 chColor, u16 bkColor)
{
    u8 i,k;
	u32 offSet, str;

	// ���ֵĵ�ַ�Ǵ�0xA1A1��ʼ
	offSet  = ((*pStr - 0xA1)*94 + (*(pStr+1) - 0xA1))*32;
	offSet += W25Q_CN_START_ADDR;

	for (i=0; i<32; i++)						  	   // һ������32�ֽ�
	{
	    str = W25QReadByte(offSet + i);
		for (k=0; k<8; k++)							     // һ���ֽ�8λ
		{
		    if ((str >> (7-k)) & 0x01)
			{
			    TFTSetPoint(x+k, y+(i/2), chColor);	     // �ַ���ɫ
			}
			else
			{
				if (bkColor != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+k, y+(i/2), bkColor);  // ������ɫ
				}
			}
		}

		if (((i!=0)&&((i%2)) != 0))                      // ����
		{
		    x -= 8;									     // д���
		}
		else
		{
		    x += 8;									     // д�Ұ�
		}
	}
} 

/********************************************************************************************************
* FunctionName   : TFTPutStr()
* Description    : ����ַ����������Ǻ��֣�Ҳ������Ӣ���ַ�(���Զ�������ʾ)
* EntryParameter : x - �����ꣻy - �����ꣻchColor - �ַ�����ɫ��bkColor - �ַ�������ɫ
* ReturnValue    : ��ʾ�ַ���
********************************************************************************************************/
u16 TFTPutStr(u16 x, u16 y, u8 *pStr, u16 chColor, u16 bkColor)
{
    u16	disCount  = 0;									           // ��ʾ�ַ���
	u16 lineCount = 0;									           // �м���
	u16 yAdd = y;

	lineCount = y;
	while ((*pStr != 0) && ((yAdd+16) <= TFT_HEIGTH))
	{   
	                                                               // ASCII�ַ��뺺���������������128
		if (*pStr <= 0x80)	                                       // ASCII < 128
		{
		     if ((lineCount + 8) > TFT_WIDTH)			           // ������ʾһ���ַ���������ʾ
			 {
			     lineCount = 0;
				 x = 0;
				 yAdd += 16;    
			 }
			 else
			 {
			     TFT_PutChar(x, yAdd, pStr, chColor, bkColor);     // ��ʾ�ַ�
	
				 x += 8;							               // �����ַ��1���ַ�λ��
				 pStr++;								           // ȡ��һ�ַ�
				 disCount++;
				 lineCount += 8;
			 }
		}											
		else										               // ���� > 128
		{
		    if ((lineCount + 16) > TFT_WIDTH)			           // ������ʾһ���ַ���������ʾ
			{
			    lineCount = 0;
				x = 0;
				yAdd += 16;    
			}
			else
			{
				TFT_PutChinese(x, yAdd, pStr, chColor, bkColor);   // ��ʾ����
															   
				x += 16;							               // �����ַ��1���ַ�λ��
			    pStr += 2;
			    disCount += 2;
				lineCount += 16;
			}
		}	
	}

	return disCount;
}

/********************************************************************************************************
* FunctionName   : TFTSetDisplayMode()
* Description    : �趨ͼƬ��ʾ�������ʾɨ�跽��
* EntryParameter : rlScan-����ɨ��, tbScan-����ɨ��, dirScan-ˮƽ/��ֱɨ��, hFilp-ˮƽ��ת, vFilp-��ֱ��ת
* ReturnValue    : None
********************************************************************************************************/
/* TB: Selects the output shift direction of the gate driver. When TB = 1, G0 shifts to G319. When TB = 0, 
G319 shifts to G0. */
void TFTSetDisplayMode(u8 rlScan, u8 tbScan, u8 dirScan, u8 hFilp, u8 vFilp)
{
    u16 vol = 0x0008;
	u16 column = 0, page = 0;

    if (rlScan == 1)						 // ����ɨ��
	{
	    vol |= 0x0040;
	}

	if (tbScan == 1)						 // ����ɨ��
	{
	    vol |= 0x0080;
	}

	if (dirScan == 1)						 // ɨ�跽��
	{
	    vol |= 0x0020;
	}

	if (hFilp == 1)							 // ˮƽ��ת
	{
	    vol |= 0x0002;
	}

	if (vFilp == 1)						     // ��ֱ��ת
	{
	    vol |= 0x0001;
	}

	TFT_WriteIndex(SET_ADDR_MODE);		     // Set_address_mode (36h)
	TFTWriteData(vol);
   
	if (dirScan == 1)					     // ����ɨ�跽�����Ҫ���ĳ�������
	{
		column = TFT_WIDTH-1;
		page = TFT_HEIGTH-1;					 
	}
	else
	{
		column = TFT_HEIGTH-1;
		page = TFT_WIDTH-1;		
	}

	TFT_WriteIndex(SET_CLM_ADDR);		     // Set_column_address (2Ah)
	TFTWriteData(0x0000);
	TFTWriteData(0x0000);
	TFTWriteData(column>>8);
	TFTWriteData(column&0xFF);								 
	
	TFT_WriteIndex(SET_PAGE_ADDR);		     // Set_page_address (2Bh)
	TFTWriteData(0x0000);
	TFTWriteData(0x0000);
	TFTWriteData(page>>8);
	TFTWriteData(page&0xFF); 
									 
	TFT_WriteIndex(WRITE_MMR_START);	    // Write_memory_start (2Ch)
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/
