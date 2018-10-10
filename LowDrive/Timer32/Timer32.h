/********************************************************************************************************  
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : timer32.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef __TIMER32_H 
#define __TIMER32_H

/******************************************************************************************************** 
* 【寄存器】 Register                                              
********************************************************************************************************/

// IR中断寄存器
#define TIM_MR0                    (0x01 << 0)								 // MR0中断
#define TIM_MR1                    (0x01 << 1)								 // MR1中断
#define TIM_MR2                    (0x01 << 2)								 // MR2中断
#define TIM_MR3                    (0x01 << 3)								 // MR3中断

#define TIM_CR0                    (0x01 << 4)								 // CR0中断
#define TIM_CR1                    (0x01 << 5)								 // CR1中断

// TCR定时器控制寄存器
#define TIM_TCRE                   (0x01 << 0)								 // 使能(1-使能，0-禁止)
#define TIM_TCRR                   (0x01 << 1)								 // 复位

// CTCR计数器控制寄存器
#define TIM_TIMER			       (0x00 << 0)				                 // 定时计数器工作在定时模式
#define TIM_CRISE			       (0x01 << 0)				                 // 计数模式，上升沿输入捕获
#define TIM_CFALL			       (0x02 << 0)				                 // 计数模式，下降沿输入捕获
#define TIM_CDBLE			       (0x03 << 0)				                 // 计数模式，双边沿输入捕获
#define TIM_CAP0S			       (0x00 << 2)				                 // CAPn.0用于TIMERn
#define TIM_CAP1S			       (0x01 << 2)				                 // CAPn.1用于TIMERn

// MCR匹配控制寄存器
#define TIM_MR0I				   (0x01 <<  0)								 // MR0与TC值匹配时将产生中断
#define TIM_MR0R				   (0x01 <<  1)								 //	MR0与TC值的匹配时TC复位
#define TIM_MR0S				   (0x01 <<  2)								 //	MR0与TC值的匹配时TC和PC停止，TCR[0]清零
#define TIM_MR1I				   (0x01 <<  3)								 // MR1与TC值匹配时将产生中断
#define TIM_MR1R				   (0x01 <<  4)								 //	MR1与TC值的匹配时TC复位
#define TIM_MR1S				   (0x01 <<  5)								 //	MR1与TC值的匹配时TC和PC停止，TCR[0]清零
#define TIM_MR2I				   (0x01 <<  6)								 // MR2与TC值匹配时将产生中断
#define TIM_MR2R				   (0x01 <<  7)								 //	MR2与TC值的匹配时TC复位
#define TIM_MR2S				   (0x01 <<  8)								 //	MR2与TC值的匹配时TC和PC停止，TCR[0]清零
#define TIM_MR3I				   (0x01 <<  9)								 // MR3与TC值匹配时将产生中断
#define TIM_MR3R				   (0x01 << 10)								 //	MR3与TC值的匹配时TC复位
#define TIM_MR3S				   (0x01 << 11)								 //	MR3与TC值的匹配时TC和PC停止，TCR[0]清零

// EMR外部匹配寄存器
#define TIM_EM0				       (0)								         //	该位的值会被驱动到MATn.0
#define TIM_EM1				       (1)								         //	该位的值会被驱动到MATn.1
#define TIM_EM2				       (2)								         //	该位的值会被驱动到MATn.2
#define TIM_EM3				       (3)								         //	该位的值会被驱动到MATn.3

#define TIM_EMC0				   (4)								         //	外部匹配控制0
#define TIM_EMC1				   (6)								         //	外部匹配控制1
#define TIM_EMC2				   (8)								         //	外部匹配控制2
#define TIM_EMC3				   (10)								         //	外部匹配控制3

#define TIM_EM0N                   (0x00 << TIM_EMC0)						 // EMC0无操作
#define TIM_EM0L                   (0x01 << TIM_EMC0)						 // EMC0输出低电平
#define TIM_EM0H                   (0x02 << TIM_EMC0)						 //	EMC0输出高电平
#define TIM_EM0T                   (0x03 << TIM_EMC0)						 //	EMC0电平翻转

#define TIM_EM1N                   (0x00 << TIM_EMC1)						 // EMC1无操作
#define TIM_EM1L                   (0x01 << TIM_EMC1)						 // EMC1输出低电平
#define TIM_EM1H                   (0x02 << TIM_EMC1)						 //	EMC1输出高电平
#define TIM_EM1T                   (0x03 << TIM_EMC1)						 //	EMC1电平翻转

#define TIM_EM2N                   (0x00 << TIM_EMC2)						 // EMC2无操作
#define TIM_EM2L                   (0x01 << TIM_EMC2)						 // EMC2输出低电平
#define TIM_EM2H                   (0x02 << TIM_EMC2)						 //	EMC2输出高电平
#define TIM_EM2T                   (0x03 << TIM_EMC2)						 //	EMC2电平翻转

#define TIM_EM3N                   (0x00 << TIM_EMC3)						 // EMC3无操作
#define TIM_EM3L                   (0x01 << TIM_EMC3)						 // EMC3输出低电平
#define TIM_EM3H                   (0x02 << TIM_EMC3)						 //	EMC3输出高电平
#define TIM_EM3T                   (0x03 << TIM_EMC3)						 //	EMC3电平翻转

#define TIM_EM_ALL                 (0x00)						             //	EMC所有外部匹配无任何操作

// CCR捕获控制寄存器
#define TIM_CAP0IN				   (0x00)									 // CAPn.0作为计数器输入,后面3位必须为0
#define TIM_CAP0RE			       (0x01 << 0)				                 // 上升沿捕获CAPn.0,并把TC内容转入CR0
#define TIM_CAP0FE			       (0x01 << 1)				                 // 下降沿捕获CAPn.0,并把TC内容转入CR0
#define TIM_CAP0IT			       (0x01 << 2)				                 // 把TC内容转入CR0同时产生CAPn.0中断

#define TIM_CAP1IN				   (0x00)									 // CAPn.1作为计数器输入,后面3位必须为0
#define TIM_CAP1RE			       (0x01 << 3)				                 // 上升沿捕获CAPn.1,并把TC内容转入CR1
#define TIM_CAP1FE			       (0x01 << 4)				                 // 下降沿捕获CAPn.1,并把TC内容转入CR1
#define TIM_CAP1IT			       (0x01 << 5)				                 // 把TC内容转入CR1同时产生CAPn.1中断

/******************************************************************************************************** 
* Variable declaration                                                 
********************************************************************************************************/
typedef enum
{
    TIM0 = 0,
	TIM1 = 1,
	TIM2 = 2,
	TIM3 = 3,

} LPC_TIMER;																  // 定时器定义

//------------------------------------------------------------------------------------------------------

typedef struct _TIM_SET
{   
    u8  Mode;                                                          		  // 工作模式
    u8  IntClear;														      // 清除中断
	u32 InitValue;															  // 定时/计数器初始值
	u32 Prescaler;															  // 预分频

// 定时器
	u16 MatchSet;															  // 匹配设置
	u16 MatchOut;															  // 匹配输出设置
	u32 TimValue;															  // 定时值(单位：us)

// 计数器
    u8  CapCtrl;															  // 捕获控制
    u8  CaptureSet;															  // 捕获设置(计数模式及CAP引脚)
} TIM_SET;

/******************************************************************************************************** 
* Internal Function                                                  
********************************************************************************************************/

/******************************************************************************************************** 
* Global Function                                                 
********************************************************************************************************/
extern u8   TIM32Init(u8 timNum, TIM_SET *timPar);	                          // 初始化定时器，并设置定时时间ms
extern void TIM32ClearInt(u8 timNum, u8 intSou);							  // 清除中断
extern void TIM32Enable(u8 timNum);				                              // 使能定时器
extern void TIM32Disable(u8 timNum);				                          // 禁止定时器
extern void TIM32Reset(u8 timNum);				                              // 重启定时器

extern void TIM32ClearTC(u8 timNum);										  // 清除TC
extern u32  TIM32GetTC(u8 timNum);											  // 获取TC值

/*******************************************************************************************************/
#endif

/******************************************************************************************************** 
* End Of File         
********************************************************************************************************/
