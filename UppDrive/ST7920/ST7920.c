/********************************************************************************************************    
*                                                                      
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : st7920.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
*                                  HeaderFiles                                                  
********************************************************************************************************/
#include "../UserCode/source/config.h" 

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : LCM_Delay()
* Description    : ��ʱ
* EntryParameter : us - us��ʱ
* ReturnValue    : None
********************************************************************************************************/
void LCM_Delay(u16 us)
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
* FunctionName   : LCM_SetGpio()
* Description    : �˿�����ΪGPIO��������Ϊ����˿�
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCM_SetGpio(void)
{
	LCM_DAT_OUT();						                                    // ���ݶ˿ڷ�������

	LCM_EN_OUT();			                                                // ʹ��
	LCM_RW_OUT();			                                                // ��д
	LCM_RS_OUT();			                                                // ����/ָ��ѡ��
	LCM_BK_OUT();			                                                // ����
	LCM_PS_OUT();			                                                // PSB
	LCM_RT_OUT();			                                                // ��λ
}

/********************************************************************************************************
* FunctionName   : LCMBacklightCtrl()
* Description    : ���⿪��
* EntryParameter : blk - 1����0��
* ReturnValue    : None
********************************************************************************************************/
void LCMBacklightCtrl(u8 blk)
{
    LCM_BK_WRITE(blk);
}

/********************************************************************************************************
* FunctionName   : LCM_CommMode()
* Description    : ͨѶģʽ
* EntryParameter : mode - ͨѶģʽ(0-���У�1-����)
* ReturnValue    : None
********************************************************************************************************/
void LCM_CommMode(u8 mode)
{
    LCM_PS_WRITE(mode);
}

/********************************************************************************************************
* FunctionName   : LCM_Reset()
* Description    : ��λLCM
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCM_Reset(void)
{
    LCM_RT_WRITE(GPIO_CLR);
	LCM_Delay(50);
	LCM_RT_WRITE(GPIO_SET); 
	LCM_Delay(200);
}

/********************************************************************************************************
* FunctionName   : LCM_WriteCmd()
* Description    : ��LCM��д��ָ��
* EntryParameter : cmd - ָ��
* ReturnValue    : None
********************************************************************************************************/
void LCM_WriteCmd(u8 cmd)
{
	LCM_Delay(10);                                                           // û��æ״̬�жϱ�־������ʱ
	LCM_DAT_WRITE(cmd);                                                      // д��ָ�LCM

    LCM_RS_WRITE(GPIO_CLR);			                                         // ѡ��ָ��
	LCM_RW_WRITE(GPIO_CLR);			                                         // д����
	LCM_EN_WRITE(GPIO_CLR);						                             // �������ٶ�̫�߿���������С����ʱ
	LCM_Delay(50);
	LCM_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : LCM_WriteData()
* Description    : ��LCM1602��д������
* EntryParameter : dat - ����
* ReturnValue    : None
********************************************************************************************************/
void LCM_WriteData(u8 dat)
{
	LCM_Delay(10);                                                           // û��æ״̬�жϱ�־������ʱ         
	LCM_DAT_WRITE(dat);                                                      // д�����ݵ�LCM

	LCM_RS_WRITE(GPIO_SET);			                                         // ѡ������
	LCM_RW_WRITE(GPIO_CLR);			                                         // д����
	LCM_EN_WRITE(GPIO_CLR);						                             // �������ٶ�̫�߿���������С����ʱ
	LCM_Delay(50);
	LCM_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : LCMInit()
* Description    : LCM�ڹ���ǰ��Ҫ����ʾ����ʼ��,����ģ���޷���������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCMInit(void)
{
    LCM_SetGpio();					                                         // �������
	LCM_Reset();														     // ��λ

    LCM_CommMode(LCM_PSB_PRL);  	                                         // ѡ���д��䷽ʽ
    LCM_Delay(500); 

    LCM_WriteCmd(LCM_BASIC_INS);  			                                 // ѡ�����ָ�
    LCM_Delay(250);        
    LCM_WriteCmd(LCM_DISPLAY_ON);  		  	                                 // ����ʾ
    LCM_Delay(250);
    LCM_WriteCmd(LCM_CLR_SCREEN);  			                                 // ����ʾ�����ַָ��Ϊ00H
    LCM_Delay(800);
	LCMBacklightCtrl(LCM_BLK_ON);											 // ������
    LCM_Delay(500);	 

	LCMClear(LCM_LINE_ALL, LCM_DIS_NML);									 // ����
}

/********************************************************************************************************
* FunctionName   : LCMDisplayStr()
* Description    : ��ָ��ĵ�ַд���ַ���(��ʾ�����ַ������ż����ַ��ʼ)�����ڰ�ȫ����������ָ�����ȱ�֤��ȫ
* EntryParameter : x-������(0~15)��y-������(0~3)��pStr-�ַ���
* ReturnValue    : ��ȷ����0�����򷵻�1
********************************************************************************************************/
u8 LCMDisplayStr(u8 x, u8 y, u8 *pStr)
{
	u8  i;                                                                   // ��ʾ��ַ
	uc8 lineAddr[] = {LCM_LINE1_HEAD, LCM_LINE2_HEAD, LCM_LINE3_HEAD, LCM_LINE4_HEAD}; 

	if ((y >= LCM_LINE_MAX) || (x >= LCM_LINE_LENGTH))						 // x, y�������
	{
	    return 1;
	}

	LCM_WriteCmd(LCM_BASIC_INS);                                             // ����ָ��
	LCM_WriteCmd(lineAddr[y] + x);                                           // д���ַ 

    for (i=x; ((i<LCM_LINE_LENGTH) && (*pStr != '\0')); i++)                  	                                     
    {
    	LCM_WriteData(*pStr++);												 // д���ַ���
    }

	if (*pStr != '\0')                                                       // �ж������Ƿ�д��
	{
 		x = 0;                                                               // ûд��д����1��
		if (++y < LCM_LINE_MAX)
		{
		    LCMDisplayStr(x, y, pStr); 										 // ������ʾ(�ݹ����)
		}
	}

	return 0;
}

/********************************************************************************************************
* FunctionName   : LCM_GetLattice()
* Description    : ��һ���ַ���������뻺��
* EntryParameter : pBuf - �����Ż���(pBuf[16][16])��pStr - �ַ�����rvsBit - ����λ
* ReturnValue    : �����ַ�������
********************************************************************************************************/
u8 LCM_GetLattice(u8 (*pBuf)[16], u8 *pStr, u16 rvsBit)
{
	u8 len = 0, k;
	u32 offSet;

	while ((len<LCM_LINE_LENGTH) && (*pStr))
	{
	    if (*pStr <= 0x80)													 // Ӣ��(ASCII < 128)
		{
		    if (rvsBit & (0x01 << len))
			{
			    for (k=0; k<16; k++)
				{
			        pBuf[k][len] = 0xFF - St8x16[((*pStr-0x20)*16) + k];	 // ��ȡ����
				}
			}
			else
			{
				for (k=0; k<16; k++)
				{
			        pBuf[k][len] = St8x16[((*pStr-0x20)*16) + k];	         // ��ȡ����
				}
			}

			pStr++;
			len++;
		}
		else																 // ����(ASCII > 128)
		{
		    if (len < (LCM_LINE_LENGTH-1))									 // �пռ���һ������
			{
			    offSet  = ((*pStr - 0xA1)*94 + (*(pStr+1) - 0xA1))*32;		 // ���ֵĵ�ַ�Ǵ�0xA1A1��ʼ
				offSet += W25Q_CN_START_ADDR;

				if (rvsBit & (0x01 << len))
				{
				    for (k=0; k<16; k++)
					{
					    pBuf[k][len  ] = 0xFF - W25QReadByte(offSet + k*2);	
						pBuf[k][len+1] = 0xFF - W25QReadByte(offSet + k*2+1); 
					}
				}
				else
				{
				    for (k=0; k<16; k++)
					{
					    pBuf[k][len  ] = W25QReadByte(offSet + k*2);	
						pBuf[k][len+1] = W25QReadByte(offSet + k*2+1); 
					}
				}

				pStr += 2;
				len  += 2;
			}
			else
			{
			    break;														 // �ռ䲻��������ѭ��
			}
		}
	}
	
	return len;	
}

/********************************************************************************************************
* FunctionName   : LCM_DisplayBuf()
* Description    : ��ʾһ�е���(ע��ͼ��ģʽ�������ʽ)
* EntryParameter : lin - ��ʾ��(0~3), pBuf - Ҫ��ʾ�Ļ���
* ReturnValue    : None
********************************************************************************************************/
void LCM_DisplayBuf(u8 x, u8 y, u8 len, u8 (*pBuf)[16])
{
    u8 i, k;
	u8 startX, startY;

	startX = LCM_LINE1_HEAD + x;											 //
	startY = LCM_LINE1_HEAD;												 //

	switch (y)
	{
	    case LCM_LINE1:	startX += 0; startY += 0;  break;
		case LCM_LINE2:	startX += 0; startY += 16; break;
		case LCM_LINE3: startX += 8; startY += 0;  break;
		case LCM_LINE4:	startX += 8; startY += 16; break;
		default: break;
	}

	if ((len % 2) == 0)													     // ��ʾ�ַ�����
	{
	    len /= 2;
	}
	else
	{
	    len = (len+1) / 2; 
	}

   	LCM_WriteCmd(LCM_EXTEN_INS);     	                                     // ��չָ��

	for (k=0; k<16; k++)
	{      
	    LCM_WriteCmd(k + startY);   	                                     // ��д�봹ֱ����
	    LCM_WriteCmd(0 + startX); 	                                         // ��д��ˮƽ����

	    for (i=0; i<len; i++)
	    {
	       	LCM_WriteData(pBuf[k][i*2]);  	                             	 // д�����ֽ�����
	       	LCM_WriteData(pBuf[k][i*2+1]);			
		}
	}
	
	LCM_WriteCmd(LCM_BASIC_INS);   	                                         // ����ָ��		  	                                
}

/********************************************************************************************************
* FunctionName   : LCMDisplayStr1()
* Description    : ��ʾһ��
* EntryParameter : lin - ��ʾ��(0~3), rvsBit - ��ʾģʽ(��Ӧ�ķ���λ), pBuf - Ҫ��ʾ�Ļ���
* ReturnValue    : None
********************************************************************************************************/
void LCMDisplayStr1(u8 x, u8 y, u16 rvsBit, u8 *pStr)
{
    u8 buf[16][16] = {0};
	u8 len;

	len = LCM_GetLattice(buf, pStr, rvsBit);										 // ���ַ�������뻺��
	LCM_DisplayBuf(x, y, len, buf); 
}

/********************************************************************************************************
* FunctionName   : LCMClear()
* Description    : ����
* EntryParameter : lin - ��(1~4)��dat - ����
* ReturnValue    : None
********************************************************************************************************/
void LCMClear(u8 lin, u16 rvsBit)
{
    u8 i;

    if (lin > (LCM_LINE_MAX-1))												 // ͼ�η�ʽ����
	{
	    for (i=0; i<LCM_LINE_MAX; i++)
		{
	        LCMDisplayStr1(0, i, rvsBit, "                ");
		}	
	}
	else
	{
		LCMDisplayStr1(0, lin, rvsBit, "                ");	
	}

	LCM_WriteCmd(LCM_DISPLAY_ON);  		  	                                 // ����ʾ
    LCM_Delay(250);
    LCM_WriteCmd(LCM_CLR_SCREEN);  			                                 // ����ʾ�����ַָ��Ϊ00H
    LCM_Delay(800);
	LCMBacklightCtrl(LCM_BLK_ON);											 // ������
    LCM_Delay(500);
}

/********************************************************************************************************
* FunctionName   : LCMDispTempSym()
* Description    : �¶ȷ�����ʾ
* EntryParameter : x��y - ����
* ReturnValue    : None
********************************************************************************************************/
void LCMDispTempSym(u8 x, u8 y)
{
    u8 k, sym[] = {"��C"};
	u8 buf[16][16] = {0};
    u32 offSet;

    offSet = ((*sym - 0xA1)*94 + (*(sym+1) - 0xA1))*32;		
    offSet += W25Q_CN_START_ADDR;

	for (k=0; k<16; k++)
	{
	    buf[k][0] = W25QReadByte(offSet + k*2);	
		//pBuf[k][1] = W25QReadByte(offSet + k*2+1); 
	}

	for (k=0; k<16; k++)
	{
	    buf[k][1] = St8x16[((*(sym+2)-0x20)*16) + k];
	}

	LCM_DisplayBuf(x, y, 2, buf);
}

/********************************************************************************************************
* FunctionName   : LCMDisplayPicture()
* Description    : д��һ��128x64��ͼƬ
* EntryParameter : pImg - ͼ������
* ReturnValue    : None
********************************************************************************************************/
void LCMDisplayPicture(u8 *pImg)
{
    u16 k;
    u8  x, y, n = 2;
	u8  startX = 0;
   
    do
	{
	    if (2 == n)
		{
		    k = 0;										                     // �ϰ����׵�ַ 
		}
		else
		{
			k = 512;									                     // �°����׵�ַ
			startX = 8;
		}

		LCM_WriteCmd(LCM_EXTEN_INS);     	                                 // ��չָ��
	    for (y=0; y<32; y++)                	                             // ��ֱ����32λ
		{	
		    LCM_WriteCmd(y + LCM_LINE1_HEAD);   	                         // ��д�봹ֱ����
		    LCM_WriteCmd(0 + LCM_LINE1_HEAD + startX); 	                     // ��д��ˮƽ����

		    for (x=0; x<8; x++)            	                                 // ˮƽ����16�ֽ�
		    {
		       	LCM_WriteData(pImg[k++]);  	                                 // ����д��16λ����
		       	LCM_WriteData(pImg[k++]);	      
		    }
		}

	} while (--n); 
	
	LCM_WriteCmd(LCM_BASIC_INS);   	                                         // ����ָ��
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/
