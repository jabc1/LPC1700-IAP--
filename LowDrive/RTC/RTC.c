/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : rtc.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*				   RTCʹ�õ���ʮ���ƣ������úͶ�ȡʱ������ΪBCD��
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"     			                           // ͷ�ļ�

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : RTCInit()
* Description    : ��ʼ��RTC����
* EntryParameter : addInt - ��ֵ�жϣ�almInt - �����ж�
* ReturnValue    : None
********************************************************************************************************/
void RTCInit(u8 addInt, u8 almInt)
{
    NVIC_DisableIRQ(RTC_IRQn);				                                   // �жϽ�ֹ
	
	LPC_RTC->CCR  = 0x00; 						                               // ʱ��ʹ�ܡ�CTC��λ��У׼������ʹ��					   			              
    LPC_RTC->ILR  = RTCCIF | RTCALF;                                           // ��ֵ�������ж�
	RTCSetAddInt(addInt);                            					  	   // ��ֵ�ж�
	RTCSetAlmInt(almInt);			                     					   // �����ж�
		
	NVIC_EnableIRQ(RTC_IRQn);				                                   // �ж�ʹ��
    RTCStartClock();														   // ������ʱ��   
}

/********************************************************************************************************
* FunctionName   : RTCSetClock()
* Description    : ����ʱ��
* EntryParameter : pClock - ʱ��ֵ��˳����,��,ʱ,��,��,��,�꣬����BCD��
* ReturnValue    : None
********************************************************************************************************/
void RTCSetClock(u8 *clock)
{
    u16 clockTmp[7];

	// ��BCD��ת����ʮ����
	*(clockTmp+0) = ((*(clock+6)>>4) * 10) + (*(clock+6) & 0x0F) + 2000;       // �� 
	*(clockTmp+1) = ((*(clock+5)>>4) * 10) + (*(clock+5) & 0x0F);              // ��
	*(clockTmp+2) = ((*(clock+4)>>4) * 10) + (*(clock+4) & 0x0F);              // ��
	*(clockTmp+3) = ((*(clock+3)>>4) * 10) + (*(clock+3) & 0x0F);              // ��
	*(clockTmp+4) = ((*(clock+2)>>4) * 10) + (*(clock+2) & 0x0F);              // ʱ
	*(clockTmp+5) = ((*(clock+1)>>4) * 10) + (*(clock+1) & 0x0F);              // ��
	*(clockTmp+6) = ((*(clock+0)>>4) * 10) + (*(clock+0) & 0x0F);              // ��

	// д������ʱ��
	LPC_RTC->YEAR  = *(clockTmp+0);					                           // ��
	LPC_RTC->MONTH = *(clockTmp+1);				                               // ��
	LPC_RTC->DOM   = *(clockTmp+2);					                           // ��

	LPC_RTC->DOW   = *(clockTmp+3);					                           // ��

	LPC_RTC->HOUR  = *(clockTmp+4);					                           // ʱ
	LPC_RTC->MIN   = *(clockTmp+5);					                           // ��
	LPC_RTC->SEC   = *(clockTmp+6);					                           // ��
}

/********************************************************************************************************
* FunctionName   : RTCGetClockr()
* Description    : ��ȡʱ��
* EntryParameter : pClock - ʱ��ֵ��˳����,��,ʱ,��,��,��,�꣬����BCD��
* ReturnValue    : None
********************************************************************************************************/
void RTCGetClock(u8 *clock)
{
    u32 datas; 
    u32 times;
	u16 clockTmp[7];

	// ��ȡ������ʱ�ӼĴ���
    times = LPC_RTC->CTIME0;                                                    // SEC(5:0), MIN(13:8), HOUR(20:16), WEEK(26:24)                             
    datas = LPC_RTC->CTIME1;					                                // DATE(4:0), MONTH(11:8), YEAR(31:28)

	// ��ʱ�ӷֱ�ȡ������������
	*(clockTmp+0) = (datas >> 16) & 0xFFF;	                                    // ��
	*(clockTmp+1) = (datas >> 8 ) & 0x0F;		                                // ��
	*(clockTmp+2) = (datas >> 0 ) & 0x1F;			                            // ��
	*(clockTmp+3) = (times >> 24) & 0x07;		                                // ��
	*(clockTmp+4) = (times >> 16) & 0x1F;		                                // ʱ
	*(clockTmp+5) = (times >> 8 ) & 0x3F;		                                // ��
	*(clockTmp+6) = (times >> 0 ) & 0x3F;			                            // ��

	// ת��BCD�룬��������
	*(clock+0) = ((*(clockTmp+6)/10) << 4) | (*(clockTmp+6) % 10);              // ��
	*(clock+1) = ((*(clockTmp+5)/10) << 4) | (*(clockTmp+5) % 10);              // ��
	*(clock+2) = ((*(clockTmp+4)/10) << 4) | (*(clockTmp+4) % 10);              // ʱ

	*(clock+3) = *(clockTmp+3);									                // ��
																	 
	*(clock+4) = ((*(clockTmp+2)/10) << 4) | (*(clockTmp+2) % 10);              // ��
	*(clock+5) = ((*(clockTmp+1)/10) << 4) | (*(clockTmp+1) % 10);              // ��
	*(clock+6) = (((*(clockTmp+0)%100)/10) << 4) | ((*(clockTmp+0)%100)% 10);   // ��
}

/********************************************************************************************************
* FunctionName   : RTCSetAlarmClock()
* Description    : ���ñ���ʱ��
* EntryParameter : pClock - ʱ��ֵ��˳����,��,ʱ,��,��,��,�꣬����BCD��
* ReturnValue    : None
********************************************************************************************************/
void RTCSetAlarmClock(u8 *clock)
{
    u16 clockTmp[7];

	// ��BCD��ת����ʮ����
	*(clockTmp+0) = ((*(clock+6)>>4) * 10) + (*(clock+6) & 0x0F) + 2000;        // �� 
	*(clockTmp+1) = ((*(clock+5)>>4) * 10) + (*(clock+5) & 0x0F);               // ��
	*(clockTmp+2) = ((*(clock+4)>>4) * 10) + (*(clock+4) & 0x0F);               // ��
	*(clockTmp+3) = ((*(clock+3)>>4) * 10) + (*(clock+3) & 0x0F);               // ��
	*(clockTmp+4) = ((*(clock+2)>>4) * 10) + (*(clock+2) & 0x0F);               // ʱ
	*(clockTmp+5) = ((*(clock+1)>>4) * 10) + (*(clock+1) & 0x0F);               // ��
	*(clockTmp+6) = ((*(clock+0)>>4) * 10) + (*(clock+0) & 0x0F);               // ��

	// д������ʱ��
	LPC_RTC->ALYEAR  = *(clockTmp+0);					                        // ��
	LPC_RTC->ALMON   = *(clockTmp+1);				                            // ��
	LPC_RTC->ALDOM   = *(clockTmp+2);					                        // ��

	LPC_RTC->ALDOW   = *(clockTmp+3);					                        // ��

	LPC_RTC->ALHOUR  = *(clockTmp+4);					                        // ʱ
	LPC_RTC->ALMIN   = *(clockTmp+5);					                        // ��
	LPC_RTC->ALSEC   = *(clockTmp+6);					                        // ��
}

/********************************************************************************************************
* FunctionName   : RTCStartClock()
* Description    : ����RTCʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void RTCStartClock(void)
{
    LPC_RTC->CCR |= (0x01 << CLKEN); 						     			    // ��һ
}

/********************************************************************************************************
* FunctionName   : RTCStopClock()
* Description    : ֹͣRTCʱ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void RTCStopClock(void)
{
    LPC_RTC->CCR &= ~(0x01 << CLKEN); 						      			    // ����
}

/********************************************************************************************************
* FunctionName   : RTCSetAddInt()
* Description    : ��ֵ�ж�����
* EntryParameter : value - �жϣ���ӦλΪ1�����жϣ�
* ReturnValue    : None
********************************************************************************************************/
void RTCSetAddInt(u8 value)
{				                     
    LPC_RTC->CIIR = value;													   // ��ֵ�жϣ�0x00-�������жϣ�0x01-�����ж�

	/* ��ֵ�жϣ�0x00-�������жϣ�0x01-�����ж� 				                     
    LPC_RTC->CIIR  = (0x00 << IMSEC ) | 						               // ��ֵ������
	                 (0x00 << IMMIN ) | 						               // ��ֵ������
					 (0x00 << IMHOUR) | 						               // ʱֵ������
					 (0x00 << IMDOM ) | 						               // ��ֵ������
	                 (0x00 << IMDOW ) | 						               // ��ֵ������
					 (0x00 << IMDOY ) | 					                   // ����ֵ������(һ���еĵڼ���)
					 (0x00 << IMMON ) | 						               // ��ֵ������
					 (0x00 << IMYEAR);                   		               // ��ֵ����	 */
}

/********************************************************************************************************
* FunctionName   : RTCSetAlmInt()
* Description    : �����ж�����
* EntryParameter : value - �жϣ���Ӧλ����Ϊ0�����жϣ�
* ReturnValue    : None
********************************************************************************************************/
void RTCSetAlmInt(u8 value)
{				                     
    LPC_RTC->AMR = ~value;													   // �����жϣ�0x00-��������0x01-����

	/* �������Σ�0x00-�����Σ�0x01-���� 				                
    LPC_RTC->AMR   = (0x01 << AMRSEC ) | 						               // ��ֵ��
	                 (0x01 << AMRMIN ) | 						               // ��ֵ��
					 (0x01 << AMRHOUR) | 						               // ʱֵ��
					 (0x01 << AMRDOM ) | 						               // ��ֵ��
					 (0x01 << AMRDOW ) | 						               // ��ֵ��
					 (0x01 << AMRDOY ) | 						               // ����ֵ(1~366)
					 (0x01 << AMRMON ) | 						               // ��ֵ��
					 (0x01 << AMRYEAR);                                        // ��ֵ�� 	  */
}

/********************************************************************************************************
* FunctionName   : RTCClearAddInt()
* Description    : �����ֵ�ж�
* EntryParameter : intType - �ж����ͣ�1-��ֵ�жϣ�2-�����жϣ�3-�����ж϶��壩
* ReturnValue    : None
********************************************************************************************************/
void RTCClearInt(u8 intType)
{		
    if ((intType & RTCCIF) == RTCCIF)
	{ 		                     
        LPC_RTC->ILR |= (0x01 << RTCCIF);                                     // �����ֵ�жϱ�־
	}

	if ((intType & RTCALF) == RTCALF)
	{
	    LPC_RTC->ILR |= (0x01 << RTCALF);                                     // ��������жϱ�־
	}
} 

/********************************************************************************************************
* FunctionName   : RTCCalibration()
* Description    : У׼����
* EntryParameter : mode - У׼ģʽ��0-�ӣ�1-������value - У׼ֵ����λΪ�루���ʱ�Ӽ�һ�����壬���ڵ���1ʹ�ܣ�����0��ֹ��
* ReturnValue    : None
********************************************************************************************************/
void RTCCalibration(u8 mode, u16 value) 
{
    LPC_RTC->CCR = (0x01 << CLKEN) | (0x00 << CCALEN); 						  // ʱ��ʹ�ܡ�У׼������ʹ��
    LPC_RTC->CALIBRATION = (mode << CALDIR) | value;						  // У׼����
}  

/********************************************************************************************************
  End Of File
********************************************************************************************************/
