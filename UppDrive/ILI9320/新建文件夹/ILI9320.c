/********************************************************************************************************    
*                                                                        
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ILI9320.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* header file                                                 
********************************************************************************************************/
#include "../UserCode/source/config.h"

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/ 
  
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

	TFTWriteData(0xFFFF);

	TFT_RT_OUT(GPIO_SET);
	TFT_WR_OUT(GPIO_SET);
	TFT_CS_OUT(GPIO_SET);
	TFT_RS_OUT(GPIO_SET);
	TFT_RD_OUT(GPIO_SET);
	TFT_EN_OUT(GPIO_SET);

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

    TFT_RT_OUT(GPIO_SET);					   // ���Բ�Ҫ
	TFT_DelayUs(1000);
	TFT_RT_OUT(GPIO_CLR);					   // ���ֵ͵�ƽ1ms��λTFT
	TFT_DelayUs(1000);
	TFT_RT_OUT(GPIO_SET);	  				   // ���߳�����λ
	TFT_DelayUs(1000);
}

/********************************************************************************************************
* FunctionName   : TFTBacklightCtrl()
* Description    : ���⿪��
* EntryParameter : blk - 1����0��
* ReturnValue    : None
********************************************************************************************************/
void TFTBacklightCtrl(u8 blk)
{
	TFT_EN_OUT(blk);
}

/********************************************************************************************************
* FunctionName   : TFT_WriteRegister()
* Description    : дָ����ַ�Ĵ�����ֵ
* EntryParameter : index - �Ĵ�����ַ��dat - �Ĵ���ֵ
* ReturnValue    : None
********************************************************************************************************/
void TFT_WriteRegister(u16 index, u16 dat)
{
	/*------------------------------- ʱ��ͼ ---------------------------------------------
	--                                                  --
	--               nCS      ----\______________________________/---                   --
	--               RS       ------\____________/-------------------                   --
	--               nRD      ---------------------------------------                   --
	--               nWR      --------\_______/-----\_____/----------                   --
	--               DB[0:15] ---------[index]------[data]-----------                   --
	--                                                                                  --
	------------------------------------------------------------------------------------*/   
 
    TFT_CS_OUT(GPIO_CLR);   // Ƭѡ
     
	TFTWriteIndex(index); 							   // д�Ĵ�����ַ
	TFTWriteData(dat);		                           // д����

	TFT_CS_OUT(GPIO_SET);   // 
}

/********************************************************************************************************
* FunctionName   : TFT_SetCursor()
* Description    : ������Ļ����
* EntryParameter : x - �����ꣻy - ������
* ReturnValue    : None
********************************************************************************************************/
void TFT_SetCursor(u16 x, u16 y)
{
    TFT_WriteRegister(0x0020, y);                    // ��
	TFT_WriteRegister(0x0021, x);					 // ��
}

/********************************************************************************************************
* FunctionName   : TFT_Clear()
* Description    : ����Ļ����ָ������ɫ��������������� 0xFFFF
* EntryParameter : dat - ���ֵ
* ReturnValue    : None
********************************************************************************************************/
void TFT_Clear(u16 dat)
{
    u32 i;

    TFTSetWindows(0x00, 0x00, TFT_WIDTH-1, TFT_HEIGTH-1);

    TFT_CS_OUT(GPIO_CLR);
	TFTWriteIndex(0x0022);
	TFT_RS_OUT(GPIO_SET);

    for (i=0; i<TFT_WIDTH*TFT_HEIGTH; i++)
	{
	    TFTWriteData(dat);
	}

    TFT_CS_OUT(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTWriteData()
* Description    : дTFT�Ĵ�������
* EntryParameter : dat - �Ĵ�������
* ReturnValue    : None
********************************************************************************************************/
void TFTWriteData(u16 dat)
{
    TFT_DAT_OUT(dat);											  // д16λ����
	TFT_WR_OUT(GPIO_CLR);
	TFT_WR_OUT(GPIO_SET);  
}

/********************************************************************************************************
* FunctionName   : TFTSetWindows()
* Description    : ���ô�������
* EntryParameter : stX,stY,endX,endY
* ReturnValue    : None
********************************************************************************************************/
void TFTSetWindows(u16 stX, u16 stY, u16 endX, u16 endY)
{
    TFT_SetCursor(stX, stY);
	TFT_WriteRegister(0x0050, stX);	                     // ˮƽGRAM��ʼλ��
	TFT_WriteRegister(0x0051, endX);                     // ˮƽGRAM��ֹλ��
	TFT_WriteRegister(0x0052, stY);				         // ��ֱGRAM��ʼλ��
	TFT_WriteRegister(0x0053, endY);					 // ��ֱGRAM��ֹλ��
}

/********************************************************************************************************
* FunctionName   : TFTWriteIndex()
* Description    : дTFT�Ĵ�����ַ,����ǰ����ѡ�п�����
* EntryParameter : idx - �Ĵ�����ַ
* ReturnValue    : None
********************************************************************************************************/
void TFTWriteIndex(u16 idx)
{
    TFT_RS_OUT(GPIO_CLR);				                  // д��ָ��
	TFTWriteData(idx);								      // д��ַ
	TFT_RS_OUT(GPIO_SET);	  
}

/********************************************************************************************************
* FunctionName   : TFTPutChar()
* Description    : ��ָ��������ʾһ��8x16�����ascii�ַ�
* EntryParameter : x - �����ꣻy - �����ꣻchClr - �ַ�����ɫ��bkClr - �ַ�������ɫ
* ReturnValue    : None
********************************************************************************************************/
/* ע�⣺
  Ҫ��������д�ֱ��ˮƽ��ת�����������·�ת��ֻ��Ҫ����x��y��������x��y������������ɡ�
*/
void TFTPutChar(u16 x, u16 y, u8 *pCh, u16 chClr, u16 bkClr)
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
			    TFTSetPoint(x+j ,y+i, chClr);	     // �ַ���ɫ
			}
			else
			{
			    if (bkClr != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+j, y+i, bkClr);	 // ������ɫ
				}
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : TFTPutChinese()
* Description    : ��ָ��������ʾһ��16x16��������ĺ���
* EntryParameter : x - �����ꣻy - �����ꣻchClr - �ַ�����ɫ��bkClr - �ַ�������ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFTPutChinese(u16 x, u16 y, u8 *pStr, u16 chClr, u16 bkClr)
{
    u8 i,k;
	u32 offSet, str;

	// ���ֵĵ�ַ�Ǵ�0xA1A1��ʼ
	offSet  = ((*pStr - 0xA1)*94 + (*(pStr+1) - 0xA1))*32;
	offSet += W25Q_CN_START_ADDR;

	for (i=0; i<32; i++)						  	            // һ������32�ֽ�
	{
	    str = W25QReadByte(offSet + i);
		for (k=0; k<8; k++)							            // һ���ֽ�8λ
		{
		    if ((str >> (7-k)) & 0x01)
			{
			    TFTSetPoint(x+k, y+(i/2), chClr);	            // �ַ���ɫ
			}
			else
			{
				if (bkClr != TFT_NOCOLOR)
				{
			        TFTSetPoint(x+k, y+(i/2), bkClr);           // ������ɫ
				}
			}
		}

		if (((i!=0)&&((i%2)) != 0))                             // ����
		{
		    x -= 8;									            // д���
		}
		else
		{
		    x += 8;									            // д�Ұ�
		}
	}
} 

/********************************************************************************************************
* FunctionName   : TFTPutStr()
* Description    : ����ַ����������Ǻ��֣�Ҳ������Ӣ���ַ�
* EntryParameter : x - �����ꣻy - �����ꣻchClr - �ַ�����ɫ��bkClr - �ַ�������ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFTPutStr(u16 x, u16 y, u8 *ptr, u16 chClr, u16 bkClr)
{
	while (*ptr != 0)
	{                                                  // ASCII�ַ��뺺���������������128
		if (*ptr <= 0x80)	                           // ASCII < 128
		{
		     TFTPutChar(x, y, ptr, chClr, bkClr);      // ��ʾ�ַ�

			 x += 8;							       // �����ַ��1���ַ�λ��
			 ptr++;								       // ȡ��һ�ַ�
		}											
		else										   // ���� > 128
		{
			TFTPutChinese(x, y, ptr, chClr, bkClr);    // ��ʾ����

			x += 16;							       // �����ַ��1���ַ�λ��
			ptr += 2;
		}
	}
} 

/********************************************************************************************************
* FunctionName   : TFTDrawPicture()
* Description    : ��ָ�����귶Χ��ʾһ��ͼƬ
* EntryParameter : stX,stY,hgth,wdth,*pic
* ReturnValue    : None
********************************************************************************************************/
void TFTDrawPicture(u16 stX, u16 stY, u16 hgth, u16 wdth, uc8 *pic)
{
    u32 i;
	u16 tmp;

	TFTSetWindows(stX, stY, stX+wdth, stY+hgth);	          // ����ͼƬ��ʾ����
	TFT_SetCursor(stX, stY);								  // ������ʼ���

	TFT_CS_OUT(GPIO_CLR);
	TFTWriteIndex(0x0022);
	TFT_RS_OUT(GPIO_SET);

	for (i=stY; i<hgth*wdth; i++)
	{
	    tmp = *pic++;                       		          // ��8λ�����ݴ�
		tmp += (*pic++ << 8);					              // ȡ��8λ���ݣ����ϲ���16λ����
		TFTWriteData(tmp);
	}

	TFT_CS_OUT(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTSetPoint()
* Description    : ��ָ�����껭��
* EntryParameter : x - �����ꣻy - �����ꣻcolor - �����ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFTSetPoint(u16 x, u16 y, u16 color)
{
    if ((x>TFT_HEIGTH) || (y>TFT_WIDTH))	                        // �ж���ʾ���귶Χ
	{
	    return;
	}

	TFT_SetCursor(x, y);					                        // д������ֵ
	TFT_CS_OUT(GPIO_CLR);

	TFTWriteIndex(0x0022);					                        // AC����
    TFT_RS_OUT(GPIO_SET);

	TFTWriteData(color);				 	                        // д������
	TFT_CS_OUT(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : TFTInit()
* Description    : ��ʼ��TFT
* EntryParameter : color - ��ʼ����ɫ
* ReturnValue    : None
********************************************************************************************************/
void TFTInit(u16 color)
{
    TFT_SetGpio();					                                // ��ʼ�����ƶ˿�
	TFTWriteData(0xFFFF);
											    
    TFT_Reset();									                // TFT��λ
	TFT_DelayUs(15000);
	TFT_WriteRegister(0x00,0x0001);                                 // ��������
	TFT_DelayUs(15000);

	TFT_WriteRegister(0x00,0x0000);
	TFT_WriteRegister(0x01,0x0000);	                                // �������������
	TFT_WriteRegister(0x02,0x0700);	                                // LCD�������ζ���
	TFT_WriteRegister(0x03,0x1038);	                                // ����ģʽ����

	TFT_WriteRegister(0x04,0x0000);	                                // ������С����
	TFT_WriteRegister(0x08,0x0202);	                                // ��ʾ����2
	TFT_WriteRegister(0x09,0x0000);     	                        // ��ʾ����3
	TFT_WriteRegister(0x0A,0x0000);	                                // 
	TFT_WriteRegister(0x0C,(1<<0));	                                // �����ʾ�ӿ�
	TFT_WriteRegister(0x0D,0x0000);	                                // 
	TFT_WriteRegister(0x0F,0x0000);	                                // 
    TFT_DelayUs(100000);

	TFT_WriteRegister(0x07,0x0101);	                                // ��ʾ����
	TFT_DelayUs(100000);

	TFT_WriteRegister(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	// ��Դ����1
	TFT_WriteRegister(0x11,0x0007);			                        // ��Դ����2
	TFT_WriteRegister(0x12,(1<<8)|(1<<4)|(0<<0));                   // ��Դ����3 
	TFT_WriteRegister(0x13,0x0B00);				                    // ��Դ����4
	TFT_WriteRegister(0x29,0x0000);					                // ��Դ����7

	TFT_WriteRegister(0x2B,(1<<14)|(1<<4));
	
	TFT_WriteRegister(0x50,0);		                                // ����X��ʼ����
	TFT_WriteRegister(0x51,239);	                                // ����X��������
	TFT_WriteRegister(0x52,0);		                                // ����Y��ʼ����
	TFT_WriteRegister(0x53,319);	                                // ����Y��������

	TFT_WriteRegister(0x60,0x2700);	                                // ���������
	TFT_WriteRegister(0x61,0x0001);	                                // ���������
	TFT_WriteRegister(0x6A,0x0000);	                                // ��ֱSrcoll����

	TFT_WriteRegister(0x80,0x0000);	                                // ����ͼƬ1��ʾλ��
	TFT_WriteRegister(0x81,0x0000);	                                // ����ͼƬ1 RAM��ʼ/������ַ
	TFT_WriteRegister(0x82,0x0000);	                                // 
	TFT_WriteRegister(0x83,0x0000);	                                // ����ͼƬ2��ʾλ��
	TFT_WriteRegister(0x84,0x0000);	                                // ����ͼƬ2 RAM��ʼ/������ַ
	TFT_WriteRegister(0x85,0x0000);	                                // 

	TFT_WriteRegister(0x90,(0<<7)|(16<<0));	                        // ���ӿڿ���1
	TFT_WriteRegister(0x92,0x0000);	                                // ���ӿڿ���2
	TFT_WriteRegister(0x93,0x0001);	                                // ���ӿڿ���3
	TFT_WriteRegister(0x95,0x0110);	                                // ���ӿڿ���4
	TFT_WriteRegister(0x97,(0<<8));	                                // ���ӿڿ���5
	TFT_WriteRegister(0x98,0x0000);	                                // ���ӿڿ���6


	TFT_WriteRegister(0x07,0x0173);	                                // ��ʾ����	

	TFT_DelayUs(50000);
	//TFTSetCooOrigin(0, 0);											// ����ԭ������
	TFT_Clear(color);								                // д��һ����ɫ
	TFTBacklightCtrl(TFT_BLK_ON);
}

/********************************************************************************************************
* FunctionName   : TFTSetCooOrigin()
* Description    : ����ԭ������
* EntryParameter : gs - ���ҷ�ת��ss - ���·�ת
* ReturnValue    : NO
********************************************************************************************************/
void TFTSetCooOrigin(u8 gs, u8 ss)
{
	(gs == 0) ? TFT_WriteRegister(0x60,0x2700) : TFT_WriteRegister(0x60,0xA700);	// ���ҷ�
    (ss == 0) ?	TFT_WriteRegister(0x01,0x0000) : TFT_WriteRegister(0x01,0x0100);	// ���·�
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/
