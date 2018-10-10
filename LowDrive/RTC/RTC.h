/********************************************************************************************************
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : rtc.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef __RTC_H 
#define __RTC_H

/******************************************************************************************************** 
* 【枚举变量】 Enumerations                                                
********************************************************************************************************/
typedef enum
{
    RTC_INT_NULL = 0x00,												  // 取消增值中断
	RTC_INT_SEC  = 0x01,												  // 秒值的增加
	RTC_INT_MIN  = 0x02,    											  // 分值的增加
	RTC_INT_HOUR = 0x04,												  // 时值的增加
	RTC_INT_DOM  = 0x08,												  // 日值的增加
	RTC_INT_DOW  = 0x10,												  // 周值的增加
	RTC_INT_DOY  = 0x20,    											  // 日期值的增加(一年中的第几天)
	RTC_INT_MON  = 0x40,												  // 月值的增加
	RTC_INT_YEAR = 0x80,												  // 年值的增

} RTC_INT;																  // 中断设置

//-------------------------------------------------------------------------------------------------------

typedef enum
{
    RTC_ALM_NULL = 0x00,												  // 取消报警中断
	RTC_ALM_SEC  = 0x01,												  // 秒值
	RTC_ALM_MIN  = 0x02,    											  // 分值
	RTC_ALM_HOUR = 0x04,												  // 时值
	RTC_ALM_DOM  = 0x08,												  // 日值
	RTC_ALM_DOW  = 0x10,												  // 周值
	RTC_ALM_DOY  = 0x20,    											  // 日期(一年中的第几天)
	RTC_ALM_MON  = 0x40,												  // 月值
	RTC_ALM_YEAR = 0x80,												  // 年值

} RTC_ALM;																  // 报警设置

/******************************************************************************************************** 
* 【寄存器】 Register                                              
********************************************************************************************************/

// 中断位置寄存器（ILR-0x4002 4000）
#define RTCCIF			     (0x01 << 0)						          // 计数器增量中断模块产生中断
#define RTCALF			     (0x01 << 1)						          // 报警寄存器产生中断

// 时钟控制寄存器（CCR-0x4002 4008）
#define CLKEN			     (0)						                  // 时钟使能
#define CTCRST			     (1)						                  // CTC复位
#define CCALEN			     (4)						                  // 校准计数器使能

// 计数器增量中断寄存器（CIIR-0x4002 400C）
#define IMSEC			     (0)						                  // 秒值的增加产生一次中断
#define IMMIN			     (1)						                  // 分值的增加产生一次中断
#define IMHOUR			     (2)						                  // 小时值的增加产生一次中断
#define IMDOM			     (3)						                  // 日期（月）值的增加产生一次中断
#define IMDOW			     (4)						                  // 星期值的增加产生一次中断
#define IMDOY			     (5)						                  // 日期（天）值的增加产生一次中断
#define IMMON			     (6)						                  // 月值的增加产生一次中断
#define IMYEAR			     (7)						                  // 年值的增加产生一次中断

// 报警屏蔽寄存器（AMR-0x4002 4010）
#define AMRSEC			     (0)						                  // 秒计数值不与报警寄存器比较
#define AMRMIN			     (1)						                  // 分计数值不与报警寄存器比较
#define AMRHOUR			     (2)						                  // 小时计数值不与报警寄存器比较
#define AMRDOM			     (3)						                  // 日期（月）计数值不与报警寄存器比较
#define AMRDOW			     (4)						                  // 星期计数值不与报警寄存器比较
#define AMRDOY			     (5)						                  // 日期（年）计数值不与报警寄存器比较
#define AMRMON			     (6)						                  // 月计数值不与报警寄存器比较
#define AMRYEAR			     (7)						                  // 年计数值不与报警寄存器比较

// RTC辅助控制寄存器（RTC_AUX-0x4002 405C）
#define RTC_OSCF		     (4)						                  // RTC振荡器失效探测标志

// RTC辅助使能寄存器（RTC_AUXEN-0x4002 4058）
#define RTC_OSCFEN		     (4)						                  // 振荡器失效探测中断使能

// RTC校准寄存器（RTC_AUXEN-0x4002 4040）
#define CALVAL			     (0)						                  // 如果校准使能，校准计数器会向该值递增计数。
#define CALDIR			     (17)						                  // 1-逆向校准; 0-正向校准

/********************************************************************************************************
* Global Function                                                 
********************************************************************************************************/
extern void RTCInit(u8 addInt, u8 almInt);								  // 初始化
extern void RTCStartClock(void);								          // 启动时钟
extern void RTCStopClock(void);									          // 停止时钟
extern void RTCSetClock(u8 *clock);		                                  // 设置时间
extern void RTCGetClock(u8 *clock);		                                  // 获取时间
extern void RTCSetAlarmClock(u8 *clock);				                  // 设置报警时间

extern void RTCSetAddInt(u8 value);										  // 增值中断
extern void RTCSetAlmInt(u8 value);										  // 报警中断
extern void RTCClearInt(u8 intType);									  // 清中断

extern void RTCCalibration(u8 mode, u16 value);					          // 时钟校准

/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
