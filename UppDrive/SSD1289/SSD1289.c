/**************************************************************************************    
*                       Crazy ARM LPC2148 V2.0 ������ʵ�����                                                 
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : SSD1289.c
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V3.70
* Description    :
*
*
**************************************************************************************/

/**************************************************************************************
*                                  HeaderFiles                                                  
**************************************************************************************/
#include "../source/config.h"    

/**************************************************************************************
* FunctionName   : TFT_Delay()
* Description    : TFT��ʱ
* EntryParameter : t
* ReturnValue    : NO
**************************************************************************************/
void TFT_Delay(uint32 t) 
{
	for (; t!=0; t--)
	{
        ;
	}
}

/**************************************************************************************
* FunctionName   : TFT_SetGPIO()
* Description    : TFT�˿ڷ�������
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void TFT_SetGPIO(void)
{
    TFT_CS_DDR() |= (1<<TFT_CS);
	TFT_RS_DDR() |= (1<<TFT_RS);
	TFT_WR_DDR() |= (1<<TFT_WR);
	TFT_RD_DDR() |= (1<<TFT_RD);
	TFT_LE_DDR() |= (1<<TFT_LE);

	TFT_RST_DDR() |= (1<<TFT_RST);
	TFT_BLK_DDR() |= (1<<TFT_BLK); 
	TFT_DAT_DDR() |= (TFT_DAT);

	TFT_RST_SET();									     // ���п��ƶ˿�����
	TFT_WR_SET();
	TFT_CS_SET();
	TFT_RS_SET();
	TFT_RD_SET();
	TFT_LE_SET();
	TFT_RST_SET();
	TFT_BLK_ON();	           						     // ������
}

/**************************************************************************************
* FunctionName   : TFT_Reset()
* Description    : TFT��λ,	ע�⸴λʱ��
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void TFT_Reset(void)
{
/*----------------------------------- ʱ��ͼ -----------------------------------------
--                                                                                  --
--                  ------------\_____________________/-----------                  --
--                              | <----  Tres  ---->  |                             --
--                                                                                  --
--                             Tres: Min.1ms                                        --
------------------------------------------------------------------------------------*/
    TFT_RST_SET();
	TFT_Delay(5000);
	
	TFT_RST_CLR();
	TFT_Delay(5000);
	
    TFT_RST_SET();
	TFT_Delay(5000);	  
}

/**************************************************************************************
* FunctionName   : TFT_WriteRegister()
* Description    : дָ����ַ�Ĵ�����ֵ
* EntryParameter : index - �Ĵ�����ַ��dat - �Ĵ���ֵ
* ReturnValue    : NO
**************************************************************************************/
void TFT_WriteRegister(uint16 index,uint16 dat)
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
 
    TFT_CS_CLR();
	TFT_RD_SET();

	TFT_RS_CLR();	                                 // дָ��
    TFT_WriteData(index);

	TFT_RS_SET();
	TFT_WriteData(dat);		                         // д����

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFT_ReadRegister()
* Description    : ��ȡָ����ַ�Ĵ�����ֵ
* EntryParameter : index - �Ĵ�����ַ
* ReturnValue    : NO
**************************************************************************************/
uint16 TFT_ReadRegister(uint16 index)
{
    uint16 tmp;

	TFT_CS_CLR();
    TFT_RS_CLR();
	TFT_RD_SET();

	TFT_WriteData(index);

	TFT_RD_CLR();

	tmp = TFT_ReadData();

	TFT_RD_SET();
	TFT_CS_SET();

    return tmp;
}

/**************************************************************************************
* FunctionName   : TFT_ReadData()
* Description    : ��ȡ���ƼĴ�����
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
uint16 TFT_ReadData(void)
{
    uint16 tmp;

#if TFT_DATA_MODE
    tmp = (TFT_READ_WORD_DATA() & TFT_DAT);		 // 16λ��
#else
												 // 8λ��
#endif
	return tmp;
}

/**************************************************************************************
* FunctionName   : TFT_WriteData()
* Description    : дTFT�Ĵ�������
* EntryParameter : dat - �Ĵ�������
* ReturnValue    : NO
**************************************************************************************/
void TFT_WriteData(uint16 dat)
{
#if TFT_DATA_MODE
	TFT_WRITE_WORD_DATA(dat);						 // д16λ����
#else
	TFT_LE_SET();
	TFT_WRITE_BYTE_DATA((uint8)dat);				 // ��д��8λ
	TFT_LE_CLR();									 // ��������
	TFT_WRITE_BYTE_DATA((uint8)(dat>>8));			 // д��8λ
#endif 
	TFT_WR_CLR();
	TFT_WR_SET();
}

/**************************************************************************************
* FunctionName   : TFT_SetCursor()
* Description    : ������Ļ����
* EntryParameter : x - �����ꣻy - ������
* ReturnValue    : NO
**************************************************************************************/
void TFT_SetCursor(uint16 x,uint16 y)
{
    TFT_WriteRegister(0x004F,y);                     // ��
	TFT_WriteRegister(0x004E,x);					 // ��
}

/**************************************************************************************
* FunctionName   : TFT_SetWindows()
* Description    : ���ô�������
* EntryParameter : startX,startY,endX,endY
* ReturnValue    : NO
**************************************************************************************/
void TFT_SetWindows(uint16 startX,uint16 startY,uint16 endX,uint16 endY)
{
    TFT_SetCursor(startX,startY);
	TFT_WriteRegister(0x0050,startX);	             // ˮƽGRAM��ʼλ��
	TFT_WriteRegister(0x0051,endX);                  // ˮƽGRAM��ֹλ��
	TFT_WriteRegister(0x0052,startY);				 // ��ֱGRAM��ʼλ��
	TFT_WriteRegister(0x0053,endY);					 // ��ֱGRAM��ֹλ��
}

/**************************************************************************************
* FunctionName   : TFT_BGR2RGB()
* Description    : ��ɫ���ݸ�ʽת��
* EntryParameter : c RRRRRGGGGGBBBBB �ĳ� BBBBBGGGGGRRRRR
* ReturnValue    : NO
**************************************************************************************/
uint16 TFT_BGR2RGB(uint16 c)
{
    uint16 r,g,b;

	b = (c>>0) & 0x1F;
	g = (c>>5) & 0x3F;
	r = (c>>11) & 0x1F;

	return ((b<<11) | (g<<5) || (r<<0));
}

/**************************************************************************************
* FunctionName   : TFT_WriteIndex()
* Description    : дTFT�Ĵ�����ַ,����ǰ����ѡ�п�����
* EntryParameter : idx - �Ĵ�����ַ
* ReturnValue    : NO
**************************************************************************************/
void TFT_WriteIndex(uint16 idx)
{
    TFT_RS_CLR();
	TFT_RD_SET();
	TFT_WriteData(idx);
	TFT_RS_SET();    
}

/**************************************************************************************
* FunctionName   : TFT_Clear()
* Description    : ����Ļ����ָ������ɫ��������������� 0xFFFF
* EntryParameter : dat - ���ֵ
* ReturnValue    : NO
**************************************************************************************/
void TFT_Clear(uint16 dat)
{
    uint32 i;

    TFT_SetCursor(0x00,0x0000);
	TFT_WriteRegister(0x0050,0x00);	                 // ˮƽGRAM��ʼλ��
	TFT_WriteRegister(0x0052,0x00);                  // ��ֱGRAM��ʼλ��
	TFT_WriteRegister(0x0051,239);	                 // ˮƽGRAM��ֹλ��
	TFT_WriteRegister(0x0053,319);	                 // ��ֱGRAM��ֹλ��

    TFT_CS_CLR();
	TFT_WriteIndex(0x0022);
	TFT_RS_SET();

    for (i=0; i<76800; i++)
	{
	    TFT_WriteData(dat);
	}

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTDrawPicture()
* Description    : ��ָ�����귶Χ��ʾһ��ͼƬ
* EntryParameter : startX,startY,endX,endY,*pic
* ReturnValue    : NO
**************************************************************************************/
void TFTDrawPicture(uint16 startX,uint16 startY,uint16 endX,uint16 endY,uint8 const *pic)
{
    uint32 i;
	uint16 tmp;

	TFT_SetWindows(startX,startY,endX,endY);
	TFT_SetCursor(startX,startY);

	TFT_CS_CLR();
	TFT_WriteIndex(0x0022);
	TFT_RS_SET();

	for (i=0; i<(endX*endY); i++)
	{
	    tmp = *pic++;                       		// ��8λ�����ݴ�
		tmp += (*pic++ << 8);					    // ȡ��8λ���ݣ����ϲ���16λ����

	    TFT_WriteData(tmp);							// д��16λ����
	}

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTPutChar()
* Description    : ��ָ��������ʾһ��8x16�����ascii�ַ�
* EntryParameter : x - �����ꣻy - �����ꣻcharColor - �ַ�����ɫ��bkColor - �ַ�������ɫ
* ReturnValue    : NO
**************************************************************************************/
void TFT_PutChar(uint16 x,uint16 y,uint8 *asciiCode,uint16 charColor,uint16 bkColor)
{
    uint16 i = 0;
	uint16 j = 0;
    uint8 tmpChar = 0;

	for (i=0; i<16; i++)
	{
	    tmpChar = AsciiLib[((*asciiCode-0x20)*16) + i];

		for (j=0; j<8; j++)
		{
		    if (((tmpChar >> (7-j)) & 0x01) == 0x01)
			{
			    TFTSetPoint(x+j,y+i,charColor);	     // �ַ���ɫ
			}
			else
			{
			    if (bkColor != NOCOLOR)
				{
			        TFTSetPoint(x+j,y+i,bkColor);	 // ������ɫ
				}
			}
		}
	}
}

/**************************************************************************************
* FunctionName   : TFT_PutChinese()
* Description    : ��ָ��������ʾһ��16x16��������ĺ���
* EntryParameter : x - �����ꣻy - �����ꣻcharColor - �ַ�����ɫ��bkColor - �ַ�������ɫ
* ReturnValue    : NO
**************************************************************************************/
void TFT_PutChinese(uint16 x,uint16 y,uint8* ptr,uint16 charColor,uint16 bkColor)
{
    uint8 i,k;
	uint32 offSet,str;

	// ���ֵĵ�ַ�Ǵ�0xB0A1��ʼ
	offSet = ((*ptr - 0xB0)*94 + (*(ptr+1) - 0xA1))*32;

	for (i=0; i<32; i++)						  	   // һ������32�ֽ�
	{
	    str = *(HzLib + offSet + i);
		for (k=0; k<8; k++)							   // һ���ֽ�8λ
		{
		    if ((str >> (7-k)) & 0x01)
			{
			    TFTSetPoint(x+k,y+(i/2),charColor);	   // �ַ���ɫ
			}
			else
			{
				if (bkColor != NOCOLOR)
				{
			        TFTSetPoint(x+k,y+(i/2),bkColor);  // ������ɫ
				}
			}
		}

		if (((i!=0)&&((i%2)) != 0))                    // ����
		{
		    x -= 8;									   // д���
		}
		else
		{
		    x += 8;									   // д�Ұ�
		}
	}
}

/**************************************************************************************
* FunctionName   : TFTPutStr()
* Description    : ����ַ����������Ǻ��֣�Ҳ������Ӣ���ַ�
* EntryParameter : x - �����ꣻy - �����ꣻcharColor - �ַ�����ɫ��bkColor - �ַ�������ɫ
* ReturnValue    : NO
**************************************************************************************/
void TFTPutStr(uint16 x,uint16 y,uint8 *ptr,uint16 charColor,uint16 bkColor)
{
	while (*ptr != 0)
	{                                                  // ASCII�ַ��뺺���������������128
		if (*ptr <= 0x80)	                           // ASCII < 128
		{
		     TFT_PutChar(x,y,ptr,charColor,bkColor);   // ��ʾ�ַ�

			 x += 8;							       // �����ַ��1���ַ�λ��
			 ptr++;								       // ȡ��һ�ַ�
		}											
		else										   // ���� > 128
		{
			TFT_PutChinese(x,y,ptr,charColor,bkColor); // ��ʾ����

			x += 16;							       // �����ַ��1���ַ�λ��
			ptr += 2;
		}
	}
}

/**************************************************************************************
* FunctionName   : TFTTest()
* Description    : ����Һ����,��ʾ����������Һ���Ƿ���������
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void TFTTest(void)
{
    uint8 R_Dat,G_Dat,B_Dat,i,j;

	TFT_SetCursor(0x00,0x0000);
	TFT_WriteRegister(0x0050,0x00);
	TFT_WriteRegister(0x0051,239);
	TFT_WriteRegister(0x0052,0x00);
	TFT_WriteRegister(0x0053,319);

	TFT_CS_CLR();
	TFT_WriteIndex(0x0022);
	TFT_RS_SET();
// ------------------  ��ɫ������  --------------------
	R_Dat = 0;
	G_Dat = 0;
	B_Dat = 0;
	for (j=0; j<53; j++)  
	{
	    for (i=0; i<240; i++)
		{
		    R_Dat = i/8;
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	R_Dat = 0x1F;
	G_Dat = 0x3F;
	B_Dat = 0x1F;
	for (j=0; j<54; j++) 
	{
	    for (i=0; i<240; i++)
		{
		    G_Dat = 0x3F - (i/4);
			B_Dat = 0x1F - (i/8);
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}
// ------------------  ��ɫ������  --------------------
	R_Dat = 0;
	G_Dat = 0;
	B_Dat = 0;
	for (j=0; j<53; j++)  
	{
	    for (i=0; i<240; i++)
		{
		    G_Dat = i/8;
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	R_Dat = 0x1F;
	G_Dat = 0x3F;
	B_Dat = 0x1F;
	for (j=0; j<54; j++)
	{
	    for (i=0; i<240; i++)
		{
		    R_Dat = 0x3F - (i/4);
			B_Dat = 0x1F - (i/8);
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}
// ------------------  ��ɫ������  --------------------
	R_Dat = 0;
	G_Dat = 0;
	B_Dat = 0;
	for (j=0; j<53; j++)  
	{
	    for (i=0; i<240; i++)
		{
		    B_Dat = i/8;
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	R_Dat = 0x1F;
	G_Dat = 0x3F;
	B_Dat = 0x1F;
	for (j=0; j<53; j++)  // ��ɫ������
	{
	    for (i=0; i<240; i++)
		{
		    G_Dat = 0x3F - (i/4);
			R_Dat = 0x1F - (i/8);
			TFT_WriteData((R_Dat<<11) | (G_Dat<<5) | B_Dat);
		}
	}

	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTBackLight()
* Description    : TFT���⿪��
* EntryParameter : status  1 - ���⿪��0 - �����
* ReturnValue    : NO
**************************************************************************************/
void TFTBackLight(uint8 status)
{
    if (status >= 1)
	{
	    TFT_BLK_ON();			                     // ���⿪
	}
	else
	{
	    TFT_BLK_OFF();							    // �����
	}
}

/**************************************************************************************
* FunctionName   : TFTInit()
* Description    : ��ʼ��TFT
* EntryParameter : color - ��ʼ����ɫ
* ReturnValue    : NO
**************************************************************************************/
void TFTInit(uint16 color)
{
    TFT_SetGPIO();					                     // ��ʼ�����ƶ˿�
	TFT_WriteData(0xFFFF);
											    
	TFT_Reset();									     // ��λҺ��ģ��
	TFT_Delay(15000);

    TFT_WriteRegister(0x0000,0x0001); TFT_Delay(15000);  //�򿪾���
    TFT_WriteRegister(0x0003,0xA8A4); TFT_Delay(15000);  //0xA8A4
    TFT_WriteRegister(0x000C,0x0000); TFT_Delay(15000);   
    TFT_WriteRegister(0x000D,0x080C); TFT_Delay(15000);   
    TFT_WriteRegister(0x000E,0x2B00); TFT_Delay(15000);   
    TFT_WriteRegister(0x001E,0x00B0); TFT_Delay(15000);   
    TFT_WriteRegister(0x0001,0x2B3F); TFT_Delay(15000);  //�����������320*240  0x6B3F
    TFT_WriteRegister(0x0002,0x0600); TFT_Delay(15000);
    TFT_WriteRegister(0x0010,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0011,0x6070); TFT_Delay(15000);  //0x4030 �������ݸ�ʽ  16λɫ 
    TFT_WriteRegister(0x0005,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0006,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0016,0xEF1C); TFT_Delay(15000);
    TFT_WriteRegister(0x0017,0x0003); TFT_Delay(15000);
    TFT_WriteRegister(0x0007,0x0233); TFT_Delay(15000);  //0x0233       
    TFT_WriteRegister(0x000B,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x000F,0x0000); TFT_Delay(15000);  //ɨ�迪ʼ��ַ
    TFT_WriteRegister(0x0041,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0042,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0048,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0049,0x013F); TFT_Delay(15000);
    TFT_WriteRegister(0x004A,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x004B,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0044,0xEF00); TFT_Delay(15000);
    TFT_WriteRegister(0x0045,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0046,0x013F); TFT_Delay(15000);
    TFT_WriteRegister(0x0030,0x0707); TFT_Delay(15000);
    TFT_WriteRegister(0x0031,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0032,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0033,0x0502); TFT_Delay(15000);
    TFT_WriteRegister(0x0034,0x0507); TFT_Delay(15000);
    TFT_WriteRegister(0x0035,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0036,0x0204); TFT_Delay(15000);
    TFT_WriteRegister(0x0037,0x0502); TFT_Delay(15000);
    TFT_WriteRegister(0x003A,0x0302); TFT_Delay(15000);
    TFT_WriteRegister(0x003B,0x0302); TFT_Delay(15000);
    TFT_WriteRegister(0x0023,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0024,0x0000); TFT_Delay(15000);
    TFT_WriteRegister(0x0025,0x8000); TFT_Delay(15000);
    TFT_WriteRegister(0x004F,0);        //����ַ0
    TFT_WriteRegister(0x004E,0);        //����ַ0

	TFT_Clear(color);
}

/**************************************************************************************
* FunctionName   : TFTGetPoint()
* Description    : ��ȡָ���������ɫֵ
* EntryParameter : x - �����ꣻy - ������
* ReturnValue    : ��ǰ������ɫֵ
**************************************************************************************/
uint16 TFTGetPoint(uint16 x,uint16 y)
{
    TFT_SetCursor(x,y);
	return (TFT_BGR2RGB(TFT_ReadRegister(0x0022)));
}

/**************************************************************************************
* FunctionName   : TFTSetPoint()
* Description    : ��ָ�����껭��
* EntryParameter : x - �����ꣻy - �����ꣻcolor - �����ɫ
* ReturnValue    : NO
**************************************************************************************/
void TFTSetPoint(uint16 x,uint16 y,uint16 color)
{
    if ((x>240) || (y>320))
	{
	    return;
	}

	TFT_SetCursor(x,y);
	TFT_CS_CLR();

	TFT_WriteIndex(0x0022);
	TFT_RS_SET();

	TFT_WriteData(color);
	TFT_CS_SET();
}

/**************************************************************************************
* FunctionName   : TFTDrawLine()
* Description    : ��ָ�����껭��
* EntryParameter : 
* ReturnValue    : NO
**************************************************************************************/
void TFTDrawLine(uint16 startX,uint16 startY,uint8 endX,uint16 endY,uint16 color)
{
    int32   dx;                                        // ֱ��x���ֵ����
    int32   dy;                                        // ֱ��y���ֵ����
    int8    xDrc;                                      // x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
    int8    yDrc;                                      // y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
    int32   dx2;                                       // dx*2ֵ���������ڼӿ������ٶ�
    int32   dy2;                                       // dy*2ֵ���������ڼӿ������ٶ�
    int32   di;                                        // ���߱���
	uint16  x,y;

    dx = endX-startX;                                  // ��ȡ����֮��Ĳ�ֵ
    dy = endY-startY;
   
   /* �ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� */
    if (dx > 0)		                                   // ��������
	{
	    xDrc = 1;                       
	}
	else
	{
	    if (dx < 0)		                               // ��������
		{
		    xDrc = -1;                
		}
		else		                                   // ��ֱ����
		{
		    for (y=startY; y<endY; y++)
			{
			     TFTSetPoint(startX,y,color);
			} 
			return ;   
		}
	}

	if (dy > 0)		                                    // ��������
	{
	    yDrc = 1;                       
	}
	else
	{
	    if (dy < 0)		                                // ��������
		{
		    yDrc = -1;                
		}
		else		                                    // ˮƽ����
		{
		    for (x=startX; x<endX; x++)
			{
			     TFTSetPoint(x,startY,color);
			}  
			return ;   
		}
	}

	// ��dx��dyȡ����ֵ
	dx = xDrc * dx;
	dy = yDrc * dy;
	// ����2����dx��dyֵ
	dx2 = dx * 2;
	dy2 = dy * 2;

	// ʹ��Bresenham�����л���
	if (dx >= dy)		                                 // ʹ��x��Ϊ��׼
	{
	    di = dy2 - dx;
		while (startX != endX)
		{
		     TFTSetPoint(startX,startY,color);
			 startX += xDrc;

			 if (di < 0)
			 {
			     di += dy2;                              // �������һ���ľ���ֵ   
			 }
			 else
			 {
			     di += (dy2 - dx2);
				 startY += yDrc; 
			 }

			 TFTSetPoint(startX,startY,color);
		}
	}
	else		                                         // ʹ��y��Ϊ��׼
	{
	    di = dx2 - dy;
		while (startY != endY)
		{
		     TFTSetPoint(startX,startY,color);
			 startY += yDrc;

			 if (di < 0)
			 {
			     di += dx2;                              // �������һ���ľ���ֵ   
			 }
			 else
			 {
			     di += (dx2 - dy2);
				 startX += xDrc; 
			 }

			 TFTSetPoint(startX,startY,color);
		}	
	}

}

/**************************************************************************************
*                                     End Of File                                                  
**************************************************************************************/
