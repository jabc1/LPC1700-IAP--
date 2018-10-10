/******************************************************************************************************** 
*                                 LPC1700 ¹¤³ÌÄ£¿é
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
*
*
********************************************************************************************************/

#ifndef  __IT__H__
#define  __IT__H__

/********************************************************************************************************
*                                      Pin definition                           
********************************************************************************************************/

/********************************************************************************************************
*                                     Macro definition                           
********************************************************************************************************/	 

/********************************************************************************************************
*                                    Variable definition                           
********************************************************************************************************/
				   
/********************************************************************************************************
*                                    Internal function                           
********************************************************************************************************/

/********************************************************************************************************
*                                     Global function                           
********************************************************************************************************/
//extern void Reset_Handler(void);
//extern void NMI_Handler(void);
//extern void HardFault_Handler(void);
//extern void MemManage_Handler(void);
//extern void BusFault_Handler(void);
//extern void UsageFault_Handler(void);
//extern void SVC_Handler(void);
//extern void DebugMonitor_Handler(void);
//extern void PendSV_Handler(void);
extern void SysTick_Handler(void);


extern void WDT_IRQHandler(void);
extern void TIMER0_IRQHandler(void);
extern void TIMER0_IRQHandler(void);
extern void TIMER0_IRQHandler(void);
extern void TIMER0_IRQHandler(void);
extern void UART0_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void UART2_IRQHandler(void);
extern void UART3_IRQHandler(void);
extern void PWM1_IRQHandler(void);
extern void NonMaskableInt_IRQHandler(void);
extern void I2C0_IRQHandler(void);
extern void I2C1_IRQHandler(void);
extern void I2C2_IRQHandler(void);
extern void SPI_IRQHandler(void);
extern void SSP0_IRQHandler(void);
extern void SSP1_IRQHandler(void);
extern void PLL0_IRQHandler(void);
extern void RTC_IRQHandler(void);
extern void EINT0_IRQHandler(void);
extern void EINT1_IRQHandler(void);
extern void EINT2_IRQHandler(void);
extern void EINT3_IRQHandler(void);
extern void ADC_IRQHandler(void);
extern void BOD_IRQHandler(void);
extern void USB_IRQHandler(void);
extern void CAN_IRQHandler(void);
extern void DMA_IRQHandler(void);
extern void I2S_IRQHandler(void);
extern void ENET_IRQHandler(void);
extern void RIT_IRQHandler(void);
extern void MCPWM_IRQHandler(void);
extern void QEI_IRQHandler(void);
extern void PLL1_IRQHandler(void);
extern void USBActivity_IRQHandler(void);
extern void CANActivity_IRQHandler(void);

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                                       End Of File         
********************************************************************************************************/
