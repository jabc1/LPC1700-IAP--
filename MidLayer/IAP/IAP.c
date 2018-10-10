/********************************************************************************************************
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : iap.c
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
#include "../UserCode/source/config.h"    			                         // ͷ�ļ�

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/ 
void (*IAP_Entry)(u32 *prmTab, u32 *rslTab) = (void(*)())IAP_ENTER_ADR;      // ���庯��ָ��                  

u32  IAP_PrmIn[5];                                                           // IAP��ڲ���������            
u32  IAP_PrmOut[5];                                                          // IAP���ڲ���������            

/********************************************************************************************************
* FunctionName   : IAPSectorPrepare()							   
* Description    : IAP��������ѡ���������50
* EntryParameter : sec1 - ��ʼ����,	sec2 - ��ֹ����
* ReturnValue    : None
********************************************************************************************************/
u32 IAPSectorPrepare(u8 sec1, u8 sec2)
{  
    IAP_PrmIn[0] = IAP_PREPARE;                                              // ����������                   
    IAP_PrmIn[1] = sec1;                                                     // ���ò���                     
    IAP_PrmIn[2] = sec2;                            
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������             
   
    return (IAP_PrmOut[0]);                                                  // ����״̬��                   
}

/********************************************************************************************************
* FunctionName   : IAPRamToFlash()							   
* Description    : ����RAM�����ݵ�FLASH���������51
* EntryParameter : dst - Ŀ���ַ, src - Դ��ַ, no	- �����ֽڸ���
* ReturnValue    : None
********************************************************************************************************/
u32 IAPRamToFlash(u32 dst, u32 src, u32 no)
{  
    IAP_PrmIn[0] = IAP_RAMTOFLASH;                                           // ����������                  
    IAP_PrmIn[1] = dst;                                                      // Ŀ���ַ����FLASH��ʼ��ַ����256�ֽ�Ϊ�ֽ�                    
    IAP_PrmIn[2] = src;														 // Դ��ַ����RAM��ַ����ַ�����ֶ���
    IAP_PrmIn[3] = no;														 // �����ֽڸ�����Ϊ256/512/1024/4096
    IAP_PrmIn[4] = IAP_FCCLK;
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������              
    
    return (IAP_PrmOut[0]);                                                  // ����״̬��                   
}

/********************************************************************************************************
* FunctionName   : IAPSectorErase()							   
* Description    : �����������������52
* EntryParameter : sec1 - ��ʼ����,	sec2 - ��ֹ����92
* ReturnValue    : None
********************************************************************************************************/
u32 IAPSectorErase(u8 sec1, u8 sec2)
{  
    IAP_PrmIn[0] = IAP_ERASESECTOR;                                          // ����������                   
    IAP_PrmIn[1] = sec1;                                                     // ���ò���                     
    IAP_PrmIn[2] = sec2;
    IAP_PrmIn[3] = IAP_FCCLK;
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������              
   
    return (IAP_PrmOut[0]);                                                  // ����״̬��                   
}

/********************************************************************************************************
* FunctionName   : IAPBlankChk()							   
* Description    : ������գ��������53
* EntryParameter : sec1 - ��ʼ����,	sec2 - ��ֹ����92
* ReturnValue    : None
********************************************************************************************************/
u32 IAPBlankChk(u8 sec1, u8 sec2)
{  
    IAP_PrmIn[0] = IAP_BLANKCHK;                                             // ����������                   
    IAP_PrmIn[1] = sec1;                                                     // ���ò���                     
    IAP_PrmIn[2] = sec2;
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������              

    return (IAP_PrmOut[0]);                                                  // ����״̬��                   
}

/********************************************************************************************************
* FunctionName   : IAPParIdRead()							   
* Description    : ��������ʶ��54
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPParIdRead(void)
{  
    IAP_PrmIn[0] = IAP_READPARTID;                                           // ����������                   
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������              

    return (IAP_PrmOut[0]);                                                  // ����״̬��                   
}

/********************************************************************************************************
* FunctionName   : IAPCodeIdBoot()							   
* Description    : ��Boot ����汾��55
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPCodeIdBoot(void)
{  
    IAP_PrmIn[0] = IAP_BOOTCODEID;                                          // ����������                 
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                    // ����IAP�������            

    return (IAP_PrmOut[0]);                                                 // ����״̬��                 
}

/********************************************************************************************************
* FunctionName   : IAPDataCompare()							   
* Description    : У�����ݣ��������56
* EntryParameter : dst - Ŀ���ַ, src - Դ��ַ, no - �����ֽڸ���
* ReturnValue    : None
********************************************************************************************************/
u32 IAPDataCompare(u32 dst, u32 src, u32 no)
{  
    IAP_PrmIn[0] = IAP_COMPARE;                                              // ����������                 
    IAP_PrmIn[1] = dst;                                                      // Ŀ���ַ����RAM/FLASH��ʼ��ַ����ַ�����ֶ���                   
    IAP_PrmIn[2] = src;														 // Դ��ַ����FLASH/RAM��ַ����ַ�����ֶ���
    IAP_PrmIn[3] = no;														 // �����ֽڸ����������ܱ�4����
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������            

    return (IAP_PrmOut[0]);                                                  // ����״̬��                 
}

/********************************************************************************************************
* FunctionName   : IAPReCallIsp()							   
* Description    : ���µ���ISP 57
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPReCallIsp(void)
{  
    IAP_PrmIn[0] = IAP_RECALLISP;                                            // ����������                 
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������            

    return (IAP_PrmOut[0]);                                                  // ����״̬��                 
}

/********************************************************************************************************
* FunctionName   : IAPDeviceSNRead()							   
* Description    : ���������к�58
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPDeviceSNRead(void)
{  
    IAP_PrmIn[0] = IAP_READDEVICESN;                                         // ����������                 
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // ����IAP�������            

    return (IAP_PrmOut[0]);                                                  // ����״̬��                 
}

/********************************************************************************************************
* FunctionName   : IAPReadFlash()							   
* Description    : ��Flash
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void IAPReadFlash(u32 addr, u8 *pBuf, u16 len)
{
    u16 i;

	for (i=0; i<len; i++)
	{
	    *(pBuf+i) = *(u8 *)(addr+i);							              // ��ֱ�ӵ�ַ
	}
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
