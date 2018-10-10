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
#include "../../UserCode/source/config.h"   			            // 头文件

/******************************************************************************************************** 
* global variable                                              
********************************************************************************************************/

/******************************************************************************************************** 
* FunctionName   : ADCInit()
* Description    : 初始化ADC
* EntryParameter : chNum -	AD通道选择(0~7)，clk - 转换时钟选择(1M~13M，单位Hz)
* ReturnValue    : 成功返回0，否则返回1
********************************************************************************************************/
u8 ADCInit(u8 chNum, u32 clk)
{
    u32 adcClock = SystemCoreClock / 4;										   // ADC时钟

    if (chNum >= ADC_NUM)
	{
	    return 1;										                       // 通道不能大于7
	}

	if (clk > 13000000)
	{
	    clk = 13000000;										                   // 时钟不能大于13M
	}

	ADC_SetGpio(chNum); 								                       // 管脚配置

	LPC_SC->PCONP |= 1 << 12;                                                  // 打开ADC电源 
	LPC_ADC->ADCR  = (0x01 << chNum)                |                          // SEL=1,选择ADCx
	                 ((adcClock/clk) << ADC_CLKDIV) |                          // 转换时钟: CLKDIV = Fpclk / clk - 1 
	                 (0 << ADC_BURST)               | 		                   // BURST = 0, no BURST, 软件控制 
					 (1 << ADC_PDN)                 |  		                   // PDN = 1，正常工作模式
					 (1 << ADC_START)               |  		                   // 设置直接启动模式 
					 (0 << ADC_EDGE);		                                   // 设置模式，直接启动模式下无效 
					
	return 0; 
}

/******************************************************************************************************** 
* FunctionName   : ADC_SetGpio()
* Description    : ADC端口设置
* EntryParameter : chNum - 转换通道
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
* Description    : 读取ADC结果
* EntryParameter : chNum - 转换通道
* ReturnValue    : 返回转换结果(0~ADC_RF mV)
********************************************************************************************************/
u32 ADCRend(u8 chNum)
{
    u8 i;
	u32 addr;
    u32 value = 0, tmp;

	for (i=0; i<10; i++)
	{
		LPC_ADC->ADCR |= (1 << ADC_START) | (1 << chNum);                         // 启动转换
		addr = (u32)(&LPC_ADC->ADDR0) + chNum*ADC_OFFSET;
		while ((ADC_REG(addr) & ADC_DONE) == 0)
		{
		    ;                                                                     // 第一次转换结果丢弃
		}

		LPC_ADC->ADCR |= (1 << ADC_START) | (1 << chNum);                         // 启动转换
		addr = (u32)(&LPC_ADC->ADDR0) + chNum*ADC_OFFSET;
		while ((ADC_REG(addr) & ADC_DONE) == 0)
		{
		    ;
		}

		addr = (u32)(&LPC_ADC->ADDR0) + chNum*ADC_OFFSET;
		tmp = ADC_REG(addr);                                                      // 读取全局数据寄存器的值
		tmp = (tmp >> 4) & 0x0FFF;	                                              // 提取12位的A/D转换结果
		value += tmp;					                                          // 循环转换10次
	}

	value /= 10;										                          // 返回平均值

	return ((value * ADC_VREF) / 4096);                                           // 转换为mV,返回转换结果(12位4096，10位1024)
}

/******************************************************************************************************** 
* End Of File                  
********************************************************************************************************/
