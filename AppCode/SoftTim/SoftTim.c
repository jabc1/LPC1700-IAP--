/******************************************************************************************************** 
*                                 LPC1700 ����ģ��
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : SoftTim.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23              JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                           // ͷ�ļ�

/********************************************************************************************************
* Variable Definition                            
********************************************************************************************************/
static SFT_TABLE SFT_Tlb[] =							                       // ��ʱ��
{
    {SFT_OPEN  , 500  , 500  , SFT_SecFlash    },					           // ����˸
	{SFT_CLOSE , 30000, 30000, SFT_BlkCls      },					           // �رձ���
	{SFT_CLOSE , 30000, 30000, SFT_ExitMenu    },						       // �˳��˵�
};

/********************************************************************************************************
* FunctionName   : SFT_SecFlash()							   
* Description    : ����˸
* EntryParameter : num - �ڶ�ʱ���е����
* ReturnValue    : None
********************************************************************************************************/
void SFT_SecFlash(u8 num)
{
    SFT_Tlb[num].State = SFT_OPEN;										       // ���´򿪶�ʱ��
    AppPrmt.SecFls = !AppPrmt.SecFls;										       // ��־ȡ��
}

/********************************************************************************************************
* FunctionName   : SFT_BlkCls()							   
* Description    : ����ر�
* EntryParameter : num - �ڶ�ʱ���е����
* ReturnValue    : None
********************************************************************************************************/
void SFT_BlkCls(u8 num)
{
    LCMBacklightCtrl(LCM_BLK_OFF);									           // �رձ���
}

/********************************************************************************************************
* FunctionName   : SFT_ExitMenu()							   
* Description    : �˳��˵�����
* EntryParameter : num - �ڶ�ʱ���е����
* ReturnValue    : None
********************************************************************************************************/
void SFT_ExitMenu(u8 num)
{
    AppPrmt.ExitMenu = TRUE;											           // �˳��˵�
	AppPrmt.DispItem = APP_DI_CINT;
}

/********************************************************************************************************
* FunctionName   : SFTInit()							   
* Description    : ��ʼ�����ʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTInit(u8 timNum, u32 timVlu)
{
   TIM_SET timSet;

   timSet.Mode       = TIM_TIMER;										        // ��ʱ��ģʽ
   timSet.IntClear   = TIM_MR0;											        // MR0
   timSet.InitValue  = 0;												        // ��ʱ����ʼֵΪ0
   timSet.Prescaler  = 0;												        // ��ʱ��ʱ�ӷ�ƵΪ0

   timSet.MatchSet   = TIM_MR0I|TIM_MR0R;								        // MR��TCƥ��ʱ�����ж�
   timSet.MatchOut	 = 0;												        // ������ƥ�����
   timSet.TimValue   = timVlu;											        // ��ʱ����ʱֵ����λ��us

   timSet.CapCtrl	 = 0;												        // ������ƣ�û��ʹ�ã�����
   timSet.CaptureSet = 0;												        // ��������

   TIM32Init(timNum, &timSet);											        // ��ʼ��ʱ��
}

/********************************************************************************************************
* FunctionName   : SFTIrq()							   
* Description    : ��ʱ��0�жϷ���
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTIrq(void)
{
    u8 i;

	TIM32ClearInt(TIM0, TIM_MR0);

	for (i=0; i<(sizeof(SFT_Tlb)/sizeof(SFT_Tlb[0])); i++)					   // ѭ���ж�
	{
	    if (SFT_Tlb[i].State == SFT_OPEN)
		{
			if (SFT_Tlb[i].CountValue)					                       // ��Ϊ��
			{
			    SFT_Tlb[i].CountValue--;
				if (SFT_Tlb[i].CountValue == 0)			                       // ��ʱʱ�䵽
				{
					SFT_Tlb[i].CountValue = SFT_Tlb[i].InitValue;
					SFT_Tlb[i].State = SFT_CLOSE;			                   // ��־���
					SFT_Tlb[i].Fun(i);				                           // ��������
				}
			}
		}
	}
}

/********************************************************************************************************
* FunctionName   : SFTSart()							   
* Description    : ������ʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTSart(u8 num)
{
	SFT_Tlb[num].CountValue = SFT_Tlb[num].InitValue;						// ��ֵ
    SFT_Tlb[num].State = SFT_OPEN;											// ����ʱ��
}

/********************************************************************************************************
* FunctionName   : SFTStop()							   
* Description    : ֹͣ��ʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTStop(u8 num)
{
    SFT_Tlb[num].State = SFT_CLOSE;
}

/********************************************************************************************************
* FunctionName   : SFTSartBlk()							   
* Description    : ����������ƶ�ʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SFTSartBlk(void)
{
    u8 blk = (AppSet.BlkTim>>4) * 10 + (AppSet.BlkTim & 0x0F);

	LCMBacklightCtrl(LCM_BLK_ON);										         // �򿪱���

	if (blk > 0)														         // Ϊ0�����������Զ��ر�
	{
		SFT_Tlb[SFT_BLK].InitValue = blk * 1000;					             // ����ر�ʱ��
		SFTSart(SFT_BLK);
	}													    
}												 

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
