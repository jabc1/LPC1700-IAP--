/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : HostCom.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeadeHCiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			 // ͷ�ļ�

		/*------------------------------------------------------------------------------------------
		|      ֡ͷ     |  ��ˮ��  |   ָ��	 |	���ݳ���  |	    ����      |  У����  |    ֡β     |
		--------------------------------------------------------------------------------------------
		|      0x7E  	|  SERIAL  |   CMD	 |	 LEN 2B	  |	 LEN�ֽ�����  |	  CRC16  |     7F      |
		------------------------------------------------------------------------------------------*/

/*    ˵����
		   1. ���ݰ���7E��ͷ��7F��β���м䲻�������������֣�����о��ᱻת�壻
		   2. ��ˮ��������������һ��Ϊ˳���ֵ���ӻ�ԭ�����أ�
		   3. ���ݳ���Ϊ2�ֽڣ�ֻ�������ݵĳ��ȣ���������ֵ��
		   4. У����Ϊ��֮ǰ���������ݵ�CRC16��
*/

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/
HC_PRMT HCPrmt;


/********************************************************************************************************
* FunctionName   : HC_SendPackage()
* Description    : �������ݰ�
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_SendPackage(HC_PRMT *prmt, u8 *pDat, u16 len)
{
    u16 tmpLen, crc;
    u8 tmpBuf[20] = {0};
	u8 sndBuf[20] = {0};

	tmpBuf[0] = HC_FRM_HEAD;
	tmpBuf[1] = prmt->Buf[1];									 // ��ˮ��
	tmpBuf[2] = prmt->Buf[2];									 // Ӧ��״̬
	tmpBuf[3] = (u8)(len >> 8);
	tmpBuf[4] = (u8)(len & 0xFF);

	memcpy(&tmpBuf[5], pDat, len);								 // ����

	crc = CMCrc16(tmpBuf, len+5);
	tmpBuf[len+5] = (u8)(crc >> 8);
	tmpBuf[len+6] = (u8)(crc & 0xFF);
	tmpBuf[len+7] = HC_FRM_TAIL;

	tmpLen = CMEscape(sndBuf, tmpBuf, len+8);			         // ת��

	HC_SendDat(sndBuf, tmpLen);								     // ��������
}

/********************************************************************************************************
* FunctionName   : HCRcvData()
* Description    : HC���ݽ���
* EntryParameter : dat - ���ݽ���
* ReturnValue    : None
********************************************************************************************************/
void HCRcvData(u8 dat)
{
    HCPrmt.Buf[HCPrmt.WriteCount] = dat;			                    // ���ݴ���
    if (++(HCPrmt.WriteCount) >= HC_BUF_SIZE)			            // �����ж�
    {
        HCPrmt.WriteCount = 0;
    }
}

/********************************************************************************************************
* FunctionName   : HC_ReadByte()
* Description    : �ӽ��ܻ����ж�ȡһ�ֽ�����
* EntryParameter : prmt - ����
* ReturnValue    : ���ض�ȡ����
********************************************************************************************************/
u8 HC_ReadByte(HC_PRMT *prmt)
{
    u8 rValue = 0x00;
    
    if (prmt->WriteCount != prmt->ReadCount)
    {
        rValue = prmt->Buf[prmt->ReadCount++];                        // ��ȡһ�ֽ�
        if (prmt->ReadCount >= HC_BUF_SIZE)
        {
            prmt->ReadCount = 0;					                  // ����
        }
    } 
    
    return rValue;   							                      // ��������
}

/********************************************************************************************************
* FunctionName   : HC_GetBufUnReadLen()
* Description    : ��ȡ��������δ�����ݳ���
* EntryParameter : prmt - ����
* ReturnValue    : �������ݳ���
********************************************************************************************************/
u16 HC_GetUnReadLen(HC_PRMT *prmt)
{
    if (prmt->WriteCount >= prmt->ReadCount)
    {
        return (prmt->WriteCount - prmt->ReadCount);
    }
    else
    {
        return ((prmt->WriteCount + HC_BUF_SIZE) - prmt->ReadCount);
    }	 
}

/********************************************************************************************************
* FunctionName   : HC_GetFram()
* Description    : ��ȡ����֡
* EntryParameter : prmt - ����
* ReturnValue    : ��ȡһ�뷵��1�����򷵻�0
********************************************************************************************************/
u8 HC_GetFram(HC_PRMT *prmt)
{
    u8  tmpDat;
    u16 i, tmpLen;
	static u8 headMark = 0;
	static u16 tmpCount = 0;
	static u16 timeOut = 0;                                       // ��ʱ������

	tmpLen = HC_GetUnReadLen(prmt);                               // ��ȡδ�����ݳ���
	if (tmpLen > 0)
	{
	    for (i=0; i<tmpLen; i++)
		{
		    tmpDat = HC_ReadByte(prmt);       					  // ��ȡһ�ֽ�
			if (tmpDat == HC_FRM_HEAD)							  // ֡ͷ
			{
			    timeOut  = 0;
			    headMark = 1;                                     // ���յ�ͷ�ñ�־
				tmpCount = 1;
			}
			else
			{
				if (headMark == 1)
				{
					 if (++tmpCount >= HC_BUF_SIZE)				 // Խ���ж�
					 {
					     tmpDat   = HC_EMPTY;					 // �������
					     timeOut  = 0;
					     headMark = 0;
						 tmpCount = 0;
						 HC_Clear(prmt);
						 HC_SendPackage(prmt, &tmpDat, 1);		 // ���Ӧ��
						 return 0;
					 }

				     if (tmpDat == HC_FRM_TAIL)					  // ֡β�ж�
					 {
					     timeOut  = 0;
					     headMark = 0;
						 tmpCount = 0;
						 return 1;
					 }
				}
			}
		}
	}

    if (headMark > 0)                                             // �Ѿ����յ�֡ͷ
    {
        if (++timeOut >= 1000)                                    // 10s��ʱ����
        {
		    tmpDat   = HC_CODE;									  // û������֡
            timeOut  = 0;
			headMark = 0;                                         // ���־
            tmpCount = 0;	
			HC_Clear(prmt);	
			HC_SendPackage(prmt, &tmpDat, 1);			          // ��ʱδ�յ�����֡Ӧ��      
        }
    } 

	return 0;
}

/********************************************************************************************************
* FunctionName   : HC_Clear()
* Description    : �����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_Clear(HC_PRMT *prmt)
{
    memset(prmt->Buf, 0, HC_BUF_SIZE);
	prmt->WriteCount = 0;
	prmt->ReadCount  = 0;
}

/********************************************************************************************************
* FunctionName   : HCClear()
* Description    : �����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HCClearAll(HC_PRMT *prmt)
{
    memset(prmt->Buf, 0, HC_BUF_SIZE);
	prmt->WriteCount = 0;
	prmt->ReadCount  = 0;

	prmt->DownMode   = 0;
	prmt->TtlPack 	 = 0;
	prmt->CurPack	 = 0;
	prmt->CodeSize	 = 0;
	prmt->StartAddr	 = 0;
}

/********************************************************************************************************
* FunctionName   : HC_GetStartAddr()
* Description    : ��ȡ��ʼ��ַ
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_GetStartAddr(HC_PRMT *prmt)
{
    u8 ackDat;

    prmt->TtlPack   = 0;
	prmt->CurPack   = 0;
	prmt->CodeSize  = 0;
	prmt->DownMode  = prmt->Buf[9];									 // ��ȡ����ģʽ
	prmt->PackSize	= CM_ARR2WORD(&prmt->Buf[10]);					 // ��ȡ����С
    prmt->StartAddr	= CM_ARR2DWORD(&prmt->Buf[5]);					 // ��ȡ32λ��ַ

    if (((prmt->DownMode == HC_DWN_FNT) && (prmt->StartAddr != HC_FONT_ADDR)) ||
	    ((prmt->DownMode == HC_DWN_COD) && (prmt->StartAddr != HC_CODE_ADDR)))
	{
	    ackDat = HC_NPRMT;											 // ��ַ����
	}
	else
	{
	    ackDat = (prmt->PackSize > HC_DPG_SIZE)	? HC_PACK : HC_OK;	 // �ж����ݰ���С��������Ӧ��ֵ
	}

	HC_SendPackage(prmt, &ackDat, 1);
	HC_Clear(prmt);
	LCMClear(LCM_LINE_ALL, LCM_DIS_NML);
}

/********************************************************************************************************
* FunctionName   : HC_DataToFlash()
* Description    : ����д��Flash
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_DataToFlash(HC_PRMT *prmt)
{
    u8  i, tmpDat[10] = {0};
	u16 tmpLen;
	u32 tmpAddr;

	tmpLen = CM_ARR2WORD(&prmt->Buf[3]);						            // ���ݰ���С
	prmt->TtlPack = CM_ARR2WORD(&prmt->Buf[5]);						        // �ܰ���
	prmt->CurPack = CM_ARR2WORD(&prmt->Buf[7]);					 	        // ��ǰ����

	if (prmt->TtlPack >= prmt->CurPack)
	{
		tmpAddr = prmt->StartAddr + ((prmt->CurPack-1) * prmt->PackSize);	 // �洢��ַ  	 
		W25QWriteNByteSecEra(tmpAddr, &prmt->Buf[9], tmpLen-4);				 // д��Flash
		//W25QWriteNByte(tmpAddr, &prmt->Buf[9], tmpLen-4);
		if (prmt->DownMode == HC_DWN_COD)
		{
		    prmt->CodeSize += (tmpLen - 4); 						         // ���մ������
		}
	}

	for (i=0; i<4; i++)												         // Ӧ������
	{
	    tmpDat[i] = prmt->Buf[5+i];									         // �ܰ����͵�ǰ����
	}

	tmpDat[4] = HC_OK;
    HC_SendPackage(prmt, tmpDat, 5);								         // ����Ӧ������
	HC_Clear(prmt);

	if ((prmt->TtlPack <= prmt->CurPack) && (prmt->DownMode == HC_DWN_COD))  // �����������
	{
	    tmpDat[0] = 0x01;													 // ������־

		tmpDat[1] = (prmt->StartAddr >> 24);					             // ��ʼ��ַ
		tmpDat[2] = (prmt->StartAddr >> 16);
		tmpDat[3] = (prmt->StartAddr >> 8);
		tmpDat[4] = (prmt->StartAddr >> 0);

		tmpDat[5] = (prmt->CodeSize >> 24);					                 // �ܵ��ֽ���
		tmpDat[6] = (prmt->CodeSize >> 16);
		tmpDat[7] = (prmt->CodeSize >> 8);
		tmpDat[8] = (prmt->CodeSize >> 0);

		ATWriteNByte(APP_UPG_ADDR, tmpDat, 9);								 // д��EEPROM
	}
}

/********************************************************************************************************
* FunctionName   : HC_FrameProcess()
* Description    : ����֡����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HC_FrameProcess(HC_PRMT *prmt)
{
    u8 ascDat;
    u8 tmpBuf[HC_BUF_SIZE] = {0};
	u16 tmpLen, tmpCrc;

	memcpy(tmpBuf, prmt->Buf, prmt->ReadCount);						  // 
	memset(prmt->Buf, 0, HC_BUF_SIZE);
	tmpLen = CMUnEscape(prmt->Buf, tmpBuf, prmt->ReadCount);		  // ��ת��

    if ((((prmt->Buf[3] << 8) + prmt->Buf[4]) + 8) == tmpLen)
	{	
		tmpCrc = CMCrc16(prmt->Buf, tmpLen-3);					      // CRC����
	    if (tmpCrc == (prmt->Buf[tmpLen-3]<<8) + prmt->Buf[tmpLen-2]) 
		{
		    switch (prmt->Buf[2])
			{
			    case HC_SND_ADDR: HC_GetStartAddr(prmt); break;		  // ��ȡ��ʼ��ַ
				case HC_SND_DATA: HC_DataToFlash(prmt);  break;		  // ����д��Flash
				default: 											  // ��֧�ֵ�ָ��
				{
				    ascDat = HC_NPC; 
					HC_SendPackage(prmt, &ascDat, 1); 
					break;
				}
			}
		} 
		else														  // У�����У���벻��ȷ
		{															  
		    ascDat = HC_CRC;
			HC_SendPackage(prmt, &ascDat, 1);						  
		}   
	}
	else															  // ָ����쳣
	{
	    ascDat = HC_LEN;
		HC_SendPackage(prmt, &ascDat, 1);    
	}
}

/********************************************************************************************************
* FunctionName   : HCInit()
* Description    : ��ʼ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void HCInit(void)
{
    HCClearAll(&HCPrmt);
}

/********************************************************************************************************
* FunctionName   : HCTackCtrl()
* Description    : �������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u8 HCTackCtrl(void)
{
    u8 reVal = 0;

    if (HC_GetFram(&HCPrmt))										   // ��ȡһ֡����
	{
	    HC_FrameProcess(&HCPrmt);									   // ����֡
		reVal = 1;
	}

	return reVal;
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
