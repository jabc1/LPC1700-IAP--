/******************************************************************************************************** ****************** 
*                             
*                                    ADDR0ess
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : adc.c
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
#include "../../UserCode/source/config.h"   			            // ͷ�ļ�

/******************************************************************************************************** 
* global variable                                              
********************************************************************************************************/

/******************************************************************************************************** 
* FunctionName   : ADCInit()
* Description    : ��ʼ��ADC
* EntryParameter : chNum -	ADͨ��ѡ��(0~7)��clk - ת��ʱ��ѡ��(1M~13M����λHz)
* ReturnValue    : �ɹ�����0�����򷵻�1
********************************************************************************************************/
u8 ADCInit(u8 chNum, u32 clk)
{
    u32 adcClock = SystemCoreClock / 4;										   // ADCʱ��

    if (chNum >= ADC_NUM)
	{
	    return 1;										                       // ͨ�����ܴ���7
	}

	if (clk > 13000000)
	{
	    clk = 13000000;										                   // ʱ�Ӳ��ܴ���13M
	}

	ADC_SetGpio(chNum); 								                       // �ܽ�����

	LPC_SC->PCONP |= 1 << 12;                                                  // ��ADC��Դ 
	LPC_ADC->ADCR  = (0x01 << chNum)                |                          // SEL=1,ѡ��ADCx
	                 ((adcClock/clk) << ADC_CLKDIV) |                          // ת��ʱ��: CLKDIV = Fpclk / clk - 1 
	                 (0 << ADC_BURST)               | 		                   // BURST = 0, no BURST, ������� 
					 (1 << ADC_PDN)                 |  		                   // PDN = 1����������ģʽ
					 (1 << ADC_START)               |  		                   // ����ֱ������ģʽ 
					 (0 << ADC_EDGE);		                                   // ����ģʽ��ֱ������ģʽ����Ч 
					
	return 0; 
}

/******************************************************************************************************** 
* FunctionName   : ADC_SetGpio()
* Description    : ADC�˿�����
* EntryParameter : chNum - ת��ͨ��
* ReturnValue    : None
********************************************************************************************************/
void ADC_SetGpio(u8 chNum)
{
    switch (chNum)
	{
	    case 0: GPIOSetMode(ADC_CH0_PORT, ADC_CH0_PIN, ADC_CH0_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	
		case 1: GPIOSetMode(ADC_CH1_PORT, ADC_CH1_PIN, ADC_CH1_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	 
		case 2: GPIOSetMode(ADC_CH2_PORT, ADC_CH2_PIN, ADC_CH2_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	 
		case 3: GPIOSetMode(ADC_CH3_PORT, ADC_CH3_PIN, ADC_CH3_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	
		case 4: GPIOSetMode(ADC_CH4_PORT, ADC_CH4_PIN, ADC_CH4_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	
		case 5: GPIOSetMode(ADC_CH5_PORT, ADC_CH5_PIN, ADC_CH5_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	
		case 6: GPIOSetMode(ADC_CH6_PORT, ADC_CH6_PIN, ADC_CH6_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	
		case 7: GPIOSetMode(ADC_CH7_PORT, ADC_CH7_PIN, ADC_CH7_FUN, GPIO_PULL_UP, GPIO_TPIU_DI); break;	 
		default: break;
	}
} 

/******************************************************************************************************** 
* FunctionName   : ADCRend()
* Description    : ��ȡADC���
* EntryParameter : chNum - ת��ͨ��
* ReturnValue    : ����ת�����(0~ADC_RF mV)
********************************************************************************************************/
u32 ADCRend(u8 chNum)
{
    u8 i;
	u32 addr;
    u32 value = 0, tmp;

	for (i=0; i<10; i++)
	{
		LPC_ADC->ADCR |= (1 << ADC_START) | (1 << chNum);                         // ����ת��
		addr = (u32)(&LPC_ADC->ADDR0) + chNum*ADC_OFFSET;
		while ((ADC_REG(addr) & ADC_DONE) == 0)
		{
		    ;                                                                     // ��һ��ת���������
		}

		LPC_ADC->ADCR |= (1 << ADC_START) | (1 << chNum);                         // ����ת��
		addr = (u32)(&LPC_ADC->ADDR0) + chNum*ADC_OFFSET;
		while ((ADC_REG(addr) & ADC_DONE) == 0)
		{
		    ;
		}

		addr = (u32)(&LPC_ADC->ADDR0) + chNum*ADC_OFFSET;
		tmp = ADC_REG(addr);                                                      // ��ȡȫ�����ݼĴ�����ֵ
		tmp = (tmp >> 4) & 0x0FFF;	                                              // ��ȡ12λ��A/Dת�����
		value += tmp;					                                          // ѭ��ת��10��
	}

	value /= 10;										                          // ����ƽ��ֵ

	return ((value * ADC_VREF) / 4096);                                           // ת��ΪmV,����ת�����(12λ4096��10λ1024)
}

/******************************************************************************************************** 
* End Of File                  
********************************************************************************************************/
