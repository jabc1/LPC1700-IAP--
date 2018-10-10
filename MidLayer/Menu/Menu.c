/********************************************************************************************************
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : Mnu.c
* Version        : V1.0
* Programmer(s)  : 
* Parameters     : ARM LPC1700  12MHz
* DebugTools     : JLINK V8.0 And Realview MDK V4.22
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                       // ͷ�ļ�

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : MnuPrmtInit()
* Description    : ��ʼ���˵�����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void MnuPrmtInit(Mnu_PRMT *prmt)
{
    prmt->Cursor = 0;										                  // �������
	prmt->PageNo = 0;										                  // ҳ����
	prmt->Index  = 0;										                  // ��������
}

/********************************************************************************************************
* FunctionName   : MnuSetPrmt()
* Description    : ���ò˵���ʾ����
* EntryParameter : num - ҳ�˵���ʾ�page - ���ҳ��
* ReturnValue    : None
********************************************************************************************************/
void MnuSetPrmt(Mnu_PRMT *prmt, u8 num, u8 page)
{
	prmt->DispNum = num;									                   // ҳ�����ʾ��Ŀ��
	prmt->MaxPage = page;									                   // ���ҳ��
}

/********************************************************************************************************
* FunctionName   : MnuDisplay()
* Description    : ��ʾ�˵�
* EntryParameter : page - ��ʾҳ��dispNum - ÿһҳ����ʾ�cursor - ���λ��
* ReturnValue    : None
********************************************************************************************************/
void MnuDisplay(Mnu_TABLE *mnu, u8 page, u8 dispNum, u8 cursor)
{
    u8 i;

	for (i=0; i<dispNum; i++)
	{
	    if (cursor == i)
		{		   	      
			MNU_DISP_RVS(0, i+1, mnu[page+i].MnuItem);         				 // ������ʾ�˵���
		}
		else
		{	             
			MNU_DISP_NML(0, i+1, mnu[page+i].MnuItem);						 // ��ʾ�˵���
		}
	}
}

/********************************************************************************************************
* FunctionName   : MnuMove()
* Description    : �˵��ƶ�
* EntryParameter : prmt - �˵�����, key - ����ֵ
* ReturnValue    : ִ�в˵�����TRUE���ƶ��˵�FALSE
********************************************************************************************************/
u8 MnuMove(Mnu_PRMT *prmt, u8 key)
{
    u8 rValue = FALSE;

    switch (key) 
	{
	    case MNU_KV_UP:													 // ����
	    { 
	        if (prmt->Cursor != 0)						                 // ��겻�ڶ���
			{
			    prmt->Cursor--;							                 // �������
			}														 
			else											             // ����ڶ���
			{
			    if (prmt->PageNo != 0)					                 // ҳ��û�е���С
				{
				    prmt->PageNo--;						                 // ���Ϸ�
				}
				else
				{
				    prmt->Cursor = prmt->DispNum-1;	                     // ��굽��
					prmt->PageNo = prmt->MaxPage-1;	                     // ���ҳ
				}
			}
			break;
		}

		case MNU_KV_DN: 												 // ����
	    {
			if (prmt->Cursor < prmt->DispNum-1)                          // ���û�е��ף��ƶ����
			{
			    prmt->Cursor++;							                 // ��������ƶ�
			}
			else                                                         // ��굽��
			{
			    if (prmt->PageNo < prmt->MaxPage-1)                      // ҳ��û�е��ף�ҳ���ƶ�
			    {
			        prmt->PageNo++;						                 // �·�һҳ
			    }
			    else                                                     // ҳ��͹�궼���ף����ؿ�ʼҳ
			    {
			        prmt->Cursor = 0;
			        prmt->PageNo = 0;
			    }
			}
			break;
		}

		case MNU_KV_MD:													 // ȷ��
	    { 
	        prmt->Index = prmt->Cursor + prmt->PageNo;                   // ����ִ���������
			rValue = TRUE;
	        break;
		}

		case MNU_KV_LF:												     // �����������
	    {
		    prmt->Cursor = 0;
			prmt->PageNo = 0;
			break;
		}

		case MNU_KV_RT:													 // �Ҽ������ײ�
	    {
			prmt->Cursor = prmt->DispNum-1;	                             // ��굽��
			prmt->PageNo = prmt->MaxPage-1;	                             // ���ҳ
			break;
		}

		default:break;
	} 

	return rValue; 												         // ����ִ������
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
