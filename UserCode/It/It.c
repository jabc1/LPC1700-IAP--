/******************************************************************************************************** 
*                                 LPC1700 工程模块
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : It.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23              JLINK V8(SWD)
* Description    : 
*				   所有中断服务函数具体内容不在这了编写，应在其他地方编写后把函数放入中断服务中
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                        // 头文件

/********************************************************************************************************
* Variable Definition                            
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : SysTick_Handler()							   
* Description    : System Tick Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SysTick_Handler(void)
{
    TPCRemarks(TaskComps);				                // 任务时间片调度		
}

/********************************************************************************************************
*                                     External Interrupts                           
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : WDT_IRQHandler()							   
* Description    : Watchdog Timer Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void WDT_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : TIMER0_IRQHandler()							   
* Description    : Timer0 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TIMER0_IRQHandler(void)
{
    ;			
}

/********************************************************************************************************
* FunctionName   : TIMER1_IRQHandler()							   
* Description    : Timer1 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TIMER1_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : TIMER2_IRQHandler()							   
* Description    : Timer2 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TIMER2_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : TIMER3_IRQHandler()							   
* Description    : Timer3 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void TIMER3_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : UART0_IRQHandler()							   
* Description    : UART0 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void UART0_IRQHandler(void)
{
	if ((LPC_UART0->IIR & 0x01) == 0)	                       // 判断是否有中断挂起 
	{
        switch (LPC_UART0->IIR & 0x0E)					       // 判断中断标志
		{
		    case 0x04: HCRcvData(LPC_UART0->RBR); break;       // 接收数据中断
			case 0x0C: /* Add your code here */	break;		   // 字符超时中断
			default: break;
		}
	}			
}

/********************************************************************************************************
* FunctionName   : UART1_IRQHandler()							   
* Description    : UART1 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void UART1_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : UART2_IRQHandler()							   
* Description    : UART2 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void UART2_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : UART3_IRQHandler()							   
* Description    : UART3 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void UART3_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : PWM1_IRQHandler()							   
* Description    : PWM1 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void PWM1_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : I2C0_IRQHandler()							   
* Description    : I2C0 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void I2C0_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : I2C1_IRQHandler()							   
* Description    : I2C1 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void I2C1_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : I2C2_IRQHandler()							   
* Description    : I2C2 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void I2C2_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : SPI_IRQHandler()							   
* Description    : SPI Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SPI_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : SSP0_IRQHandler()							   
* Description    : SSP0 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SSP0_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : SSP1_IRQHandler()							   
* Description    : SSP1 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SSP1_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : PLL0_IRQHandler()							   
* Description    : PLL0 Lock (Main PLL) Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void PLL0_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : RTC_IRQHandler()							   
* Description    : Real Time Clock Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void RTC_IRQHandler(void)
{
    LPC_RTC->ILR = 0x01;                                      // 清除中断标志			
}

/********************************************************************************************************
* FunctionName   : EINT0_IRQHandler()							   
* Description    : External Interrupt 0 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void EINT0_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : EINT1_IRQHandler()							   
* Description    : External Interrupt 1 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void EINT1_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : EINT2_IRQHandler()							   
* Description    : External Interrupt 2 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void EINT2_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : EINT3_IRQHandler()							   
* Description    : External Interrupt 3 Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void EINT3_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : ADC_IRQHandler()							   
* Description    : A/D Converter Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void ADC_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : BOD_IRQHandler()							   
* Description    : Brown-Out Detect Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void BOD_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : USB_IRQHandler()							   
* Description    : USB Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void USB_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : CAN_IRQHandler()							   
* Description    : CAN Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void CAN_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : DMA_IRQHandler()							   
* Description    : General Purpose DMA Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void DMA_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : I2S_IRQHandler()							   
* Description    : I2S Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void I2S_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : ENET_IRQHandler()							   
* Description    : Ethernet Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void ENET_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : RIT_IRQHandler()							   
* Description    : Repetitive Interrupt Timer Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void RIT_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : MCPWM_IRQHandler()							   
* Description    : Motor Control PWM Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void MCPWM_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : QEI_IRQHandler()							   
* Description    : Quadrature Encoder Interface Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void QEI_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : PLL1_IRQHandler()							   
* Description    : PLL1 Lock (USB PLL) Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void PLL1_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : USBActivity_IRQHandler()							   
* Description    : USB Activity Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void USBActivity_IRQHandler(void)
{
			
}

/********************************************************************************************************
* FunctionName   : CANActivity_IRQHandler()							   
* Description    : CAN Activity Interrupt
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void CANActivity_IRQHandler(void)
{
			
}												 

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
