/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : ssp.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../../UserCode/source/config.h"    			                            // ͷ�ļ�

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
static LPC_SSP_TypeDef(* const LPC_SSP[2]) = {LPC_SSP0, LPC_SSP1};             // ���߶���

/********************************************************************************************************
* FunctionName   : SSP_SetGpio()
* Description    : SSP�˿�����
* EntryParameter : sspNum - ����ѡ��(0,1)
* ReturnValue    : None
********************************************************************************************************/
void SSP_SetGpio(u8 sspNum)
{
	if (sspNum == 0)
	{
		GPIOSetMode(SSP_SCK0_PORT,  SSP_SCK0_PIN,  SSP_SCK0_FUN,  GPIO_PULL_UP, GPIO_TPIU_DI);    
		GPIOSetMode(SSP_MISO0_PORT, SSP_MISO0_PIN, SSP_MISO0_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);	
		GPIOSetMode(SSP_MOSI0_PORT, SSP_MOSI0_PIN, SSP_MOSI0_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);	
	}
	else
	{
		GPIOSetMode(SSP_SCK1_PORT,  SSP_SCK1_PIN,  SSP_SCK1_FUN,  GPIO_PULL_UP, GPIO_TPIU_DI);    
		GPIOSetMode(SSP_MISO1_PORT, SSP_MISO1_PIN, SSP_MISO1_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);	
		GPIOSetMode(SSP_MOSI1_PORT, SSP_MOSI1_PIN, SSP_MOSI1_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);	
	}
}

/********************************************************************************************************
* FunctionName   : SSPInit()
* Description    : ��ʼ��SSP����
* EntryParameter : sspNum - ����ѡ��(0,1)��clk - ʱ��(2��254֮���ż��)
* ReturnValue    : ��ʼ���ɷ���0�����򷵻�1
********************************************************************************************************/
u8 SSPInit(u8 sspNum, u8 clk)
{
    if (sspNum > 1)
	{
	    return (1);
	}

    SSP_SetGpio(sspNum);								                           // �˿�����

    if (sspNum == 0)			                                                   
	{
        LPC_SC->PCONP |= (1 << 21);						                           // ��SSP��Դ
	}
	else						                                                  
	{
        LPC_SC->PCONP |= (1 << 10);						                           // ��SSP1��Դ 
	}

    LPC_SSP[sspNum]->CR0 = (0x07 << CR0_DSS) |                                     // ÿ֡���ݳ���Ϊ8λ��0111=8λ 
		                   (0x00 << CR0_FRF) |                                     // ����֡��ʽΪ00=SPI
		                   (0x00 << CR0_SPO) |                                     // CPOLʱ��������ԣ�00-�͵�ƽ��01-�ߵ�ƽ��
		                   (0x00 << CR0_SPH) |                                     // ������SCK�ĵ�һ��ʱ���ز�����00-��1��ʱ�ӣ�01-��2��ʱ�ӣ�
		                   (0x01 << CR0_SCR) ;                                     // ����SSPͨѶλ����ΪĬ��ֵ
	
	LPC_SSP[sspNum]->CR1 = (0x00 << CR1_LBM) |                                     // ��ʹ�û�дģʽ
			               (0x01 << CR1_SSE) |                                     // ����SSPʹ��
			               (0x00 << CR1_MS)  |                                     // ����ѡ��,0=����,1=�ӻ�
		 	               (0x00 << CR1_SOD) ;                                     // �ӻ��������

	LPC_SSP[sspNum]->CPSR = clk;                                                   // PCLK��Ƶֵ
    LPC_SSP[sspNum]->ICR  = ICR_RORIC | ICR_RTIC;                                  // �ж�����Ĵ��� 
    LPC_SSP[sspNum]->IMSC = IMSC_RORIM | IMSC_RTIM;                                // SSPn�ж�ʹ������

	return (0);
}

/********************************************************************************************************
* FunctionName   : SSPSendByte()
* Description    : SSP����һ�ֽ�����
* EntryParameter : sspNum - ����ѡ��(0,1)��dat - ��������
* ReturnValue    : ����S0SPDR�е�����
********************************************************************************************************/
u8 SSPSendByte(u8 sspNum, u8 dat)
{
	LPC_SSP[sspNum]->DR = dat;                                                     // ��������data
	 
	while ((LPC_SSP[sspNum]->SR & (SR_TNF|SR_BSY)) != SR_TNF) 
	{ 
	    ;                                                                          // �ȴ�������� 
    }

	return (LPC_SSP[sspNum]->DR);
}

/********************************************************************************************************
* FunctionName   : SSPRcvByte()
* Description    : SSP����һ�ֽ�����
* EntryParameter : sspNum - ����ѡ��(0,1)��dat - ��������
* ReturnValue    : ����SxSPDR�е�����
********************************************************************************************************/
u8 SSPRcvByte(u8 sspNum)
{
	LPC_SSP[sspNum]->DR = 0xFF;                                                    // ��������data
	 
	while ((LPC_SSP[sspNum]->SR & (SR_TNF|SR_BSY)) != SR_TNF) 
	{ 
	    ;                                                                          // �ȴ�������� 
    }

    return (LPC_SSP[sspNum]->DR);
}

/********************************************************************************************************
* FunctionName   : SSPSendRcvByte()
* Description    : SSP���ջ�������
* EntryParameter : sspNum - ����ѡ��(0,1)��dat - ��������
* ReturnValue    : ����S0SPDR�е�����
********************************************************************************************************/
u8 SSPSendRcvByte(u8 sspNum, u8 dat)
{
	LPC_SSP[sspNum]->DR = dat;                                                    // ��������data
	 
	while ((LPC_SSP[sspNum]->SR & (SR_TNF|SR_BSY)) != SR_TNF) 
	{ 
	    ;                                                                         // �ȴ�������� 
    }

    return (LPC_SSP[sspNum]->DR);
}

/********************************************************************************************************
* FunctionName   : SSPSetSpeed()
* Description    : �ٶ�����
* EntryParameter : sspNum - ����ѡ��(0,1)��clk - ʱ��(2~254)
* ReturnValue    : None
********************************************************************************************************/
void SSPSetSpeed(u8 sspNum, u8 clk)
{
    LPC_SSP[sspNum]->CPSR = clk;                                                 // PCLK��Ƶֵ	
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
