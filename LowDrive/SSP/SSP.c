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
#include "../../UserCode/source/config.h"    			                            // 头文件

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
static LPC_SSP_TypeDef(* const LPC_SSP[2]) = {LPC_SSP0, LPC_SSP1};             // 总线定义

/********************************************************************************************************
* FunctionName   : SSP_SetGpio()
* Description    : SSP端口设置
* EntryParameter : sspNum - 总线选择(0,1)
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
* Description    : 初始化SSP总线
* EntryParameter : sspNum - 总线选择(0,1)，clk - 时钟(2～254之间的偶数)
* ReturnValue    : 初始化成返回0，否则返回1
********************************************************************************************************/
u8 SSPInit(u8 sspNum, u8 clk)
{
    if (sspNum > 1)
	{
	    return (1);
	}

    SSP_SetGpio(sspNum);								                           // 端口设置

    if (sspNum == 0)			                                                   
	{
        LPC_SC->PCONP |= (1 << 21);						                           // 打开SSP电源
	}
	else						                                                  
	{
        LPC_SC->PCONP |= (1 << 10);						                           // 打开SSP1电源 
	}

    LPC_SSP[sspNum]->CR0 = (0x07 << CR0_DSS) |                                     // 每帧数据长度为8位，0111=8位 
		                   (0x00 << CR0_FRF) |                                     // 设置帧格式为00=SPI
		                   (0x00 << CR0_SPO) |                                     // CPOL时钟输出极性（00-低电平，01-高电平）
		                   (0x00 << CR0_SPH) |                                     // 数据在SCK的第一个时钟沿采样（00-第1个时钟，01-第2个时钟）
		                   (0x01 << CR0_SCR) ;                                     // 设置SSP通讯位速率为默认值
	
	LPC_SSP[sspNum]->CR1 = (0x00 << CR1_LBM) |                                     // 不使用回写模式
			               (0x01 << CR1_SSE) |                                     // 设置SSP使能
			               (0x00 << CR1_MS)  |                                     // 主从选择,0=主机,1=从机
		 	               (0x00 << CR1_SOD) ;                                     // 从机输出禁能

	LPC_SSP[sspNum]->CPSR = clk;                                                   // PCLK分频值
    LPC_SSP[sspNum]->ICR  = ICR_RORIC | ICR_RTIC;                                  // 中断清除寄存器 
    LPC_SSP[sspNum]->IMSC = IMSC_RORIM | IMSC_RTIM;                                // SSPn中断使能设置

	return (0);
}

/********************************************************************************************************
* FunctionName   : SSPSendByte()
* Description    : SSP发送一字节数据
* EntryParameter : sspNum - 总线选择(0,1)，dat - 发送数据
* ReturnValue    : 返回S0SPDR中的数据
********************************************************************************************************/
u8 SSPSendByte(u8 sspNum, u8 dat)
{
	LPC_SSP[sspNum]->DR = dat;                                                     // 发送数据data
	 
	while ((LPC_SSP[sspNum]->SR & (SR_TNF|SR_BSY)) != SR_TNF) 
	{ 
	    ;                                                                          // 等待发送完毕 
    }

	return (LPC_SSP[sspNum]->DR);
}

/********************************************************************************************************
* FunctionName   : SSPRcvByte()
* Description    : SSP接收一字节数据
* EntryParameter : sspNum - 总线选择(0,1)，dat - 发送数据
* ReturnValue    : 返回SxSPDR中的数据
********************************************************************************************************/
u8 SSPRcvByte(u8 sspNum)
{
	LPC_SSP[sspNum]->DR = 0xFF;                                                    // 发送数据data
	 
	while ((LPC_SSP[sspNum]->SR & (SR_TNF|SR_BSY)) != SR_TNF) 
	{ 
	    ;                                                                          // 等待发送完毕 
    }

    return (LPC_SSP[sspNum]->DR);
}

/********************************************************************************************************
* FunctionName   : SSPSendRcvByte()
* Description    : SSP接收或发送数据
* EntryParameter : sspNum - 总线选择(0,1)，dat - 发送数据
* ReturnValue    : 返回S0SPDR中的数据
********************************************************************************************************/
u8 SSPSendRcvByte(u8 sspNum, u8 dat)
{
	LPC_SSP[sspNum]->DR = dat;                                                    // 发送数据data
	 
	while ((LPC_SSP[sspNum]->SR & (SR_TNF|SR_BSY)) != SR_TNF) 
	{ 
	    ;                                                                         // 等待发送完毕 
    }

    return (LPC_SSP[sspNum]->DR);
}

/********************************************************************************************************
* FunctionName   : SSPSetSpeed()
* Description    : 速度设置
* EntryParameter : sspNum - 总线选择(0,1)，clk - 时钟(2~254)
* ReturnValue    : None
********************************************************************************************************/
void SSPSetSpeed(u8 sspNum, u8 clk)
{
    LPC_SSP[sspNum]->CPSR = clk;                                                 // PCLK分频值	
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
