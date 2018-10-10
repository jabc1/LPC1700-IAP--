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
*				   RTC使用的是十进制，而设置和读取时间最终为BCD码
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"     			                           // 头文件

/********************************************************************************************************
* Variable definition                            
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : RTCInit()
* Description    : 初始化RTC设置
* EntryParameter : addInt - 增值中断，almInt - 报警中断
* ReturnValue    : None
********************************************************************************************************/
void RTCInit(u8 addInt, u8 almInt)
{
    NVIC_DisableIRQ(RTC_IRQn);				                                   // 中断禁止
	
	LPC_RTC->CCR  = 0x00; 						                               // 时钟使能、CTC复位、校准计数器使能					   			              
    LPC_RTC->ILR  = RTCCIF | RTCALF;                                           // 增值、报警中断
	RTCSetAddInt(addInt);                            					  	   // 增值中断
	RTCSetAlmInt(almInt);			                     					   // 报警中断
		
	NVIC_EnableIRQ(RTC_IRQn);				                                   // 中断使能
    RTCStartClock();														   // 启动定时器   
}

/********************************************************************************************************
* FunctionName   : RTCSetClock()
* Description    : 设置时间
* EntryParameter : pClock - 时钟值，顺序：秒,分,时,周,日,月,年，采用BCD码
* ReturnValue    : None
********************************************************************************************************/
void RTCSetClock(u8 *clock)
{
    u16 clockTmp[7];

	// 把BCD码转换成十进制
	*(clockTmp+0) = ((*(clock+6)>>4) * 10) + (*(clock+6) & 0x0F) + 2000;       // 年 
	*(clockTmp+1) = ((*(clock+5)>>4) * 10) + (*(clock+5) & 0x0F);              // 月
	*(clockTmp+2) = ((*(clock+4)>>4) * 10) + (*(clock+4) & 0x0F);              // 日
	*(clockTmp+3) = ((*(clock+3)>>4) * 10) + (*(clock+3) & 0x0F);              // 周
	*(clockTmp+4) = ((*(clock+2)>>4) * 10) + (*(clock+2) & 0x0F);              // 时
	*(clockTmp+5) = ((*(clock+1)>>4) * 10) + (*(clock+1) & 0x0F);              // 分
	*(clockTmp+6) = ((*(clock+0)>>4) * 10) + (*(clock+0) & 0x0F);              // 秒

	// 写入设置时间
	LPC_RTC->YEAR  = *(clockTmp+0);					                           // 年
	LPC_RTC->MONTH = *(clockTmp+1);				                               // 月
	LPC_RTC->DOM   = *(clockTmp+2);					                           // 日

	LPC_RTC->DOW   = *(clockTmp+3);					                           // 周

	LPC_RTC->HOUR  = *(clockTmp+4);					                           // 时
	LPC_RTC->MIN   = *(clockTmp+5);					                           // 分
	LPC_RTC->SEC   = *(clockTmp+6);					                           // 秒
}

/********************************************************************************************************
* FunctionName   : RTCGetClockr()
* Description    : 获取时间
* EntryParameter : pClock - 时钟值，顺序：秒,分,时,周,日,月,年，采用BCD码
* ReturnValue    : None
********************************************************************************************************/
void RTCGetClock(u8 *clock)
{
    u32 datas; 
    u32 times;
	u16 clockTmp[7];

	// 读取完整的时钟寄存器
    times = LPC_RTC->CTIME0;                                                    // SEC(5:0), MIN(13:8), HOUR(20:16), WEEK(26:24)                             
    datas = LPC_RTC->CTIME1;					                                // DATE(4:0), MONTH(11:8), YEAR(31:28)

	// 把时钟分别取出放入数组中
	*(clockTmp+0) = (datas >> 16) & 0xFFF;	                                    // 年
	*(clockTmp+1) = (datas >> 8 ) & 0x0F;		                                // 月
	*(clockTmp+2) = (datas >> 0 ) & 0x1F;			                            // 日
	*(clockTmp+3) = (times >> 24) & 0x07;		                                // 周
	*(clockTmp+4) = (times >> 16) & 0x1F;		                                // 时
	*(clockTmp+5) = (times >> 8 ) & 0x3F;		                                // 分
	*(clockTmp+6) = (times >> 0 ) & 0x3F;			                            // 秒

	// 转成BCD码，放入数组
	*(clock+0) = ((*(clockTmp+6)/10) << 4) | (*(clockTmp+6) % 10);              // 秒
	*(clock+1) = ((*(clockTmp+5)/10) << 4) | (*(clockTmp+5) % 10);              // 分
	*(clock+2) = ((*(clockTmp+4)/10) << 4) | (*(clockTmp+4) % 10);              // 时

	*(clock+3) = *(clockTmp+3);									                // 周
																	 
	*(clock+4) = ((*(clockTmp+2)/10) << 4) | (*(clockTmp+2) % 10);              // 日
	*(clock+5) = ((*(clockTmp+1)/10) << 4) | (*(clockTmp+1) % 10);              // 月
	*(clock+6) = (((*(clockTmp+0)%100)/10) << 4) | ((*(clockTmp+0)%100)% 10);   // 年
}

/********************************************************************************************************
* FunctionName   : RTCSetAlarmClock()
* Description    : 设置报警时间
* EntryParameter : pClock - 时钟值，顺序：秒,分,时,周,日,月,年，采用BCD码
* ReturnValue    : None
********************************************************************************************************/
void RTCSetAlarmClock(u8 *clock)
{
    u16 clockTmp[7];

	// 把BCD码转换成十进制
	*(clockTmp+0) = ((*(clock+6)>>4) * 10) + (*(clock+6) & 0x0F) + 2000;        // 年 
	*(clockTmp+1) = ((*(clock+5)>>4) * 10) + (*(clock+5) & 0x0F);               // 月
	*(clockTmp+2) = ((*(clock+4)>>4) * 10) + (*(clock+4) & 0x0F);               // 日
	*(clockTmp+3) = ((*(clock+3)>>4) * 10) + (*(clock+3) & 0x0F);               // 周
	*(clockTmp+4) = ((*(clock+2)>>4) * 10) + (*(clock+2) & 0x0F);               // 时
	*(clockTmp+5) = ((*(clock+1)>>4) * 10) + (*(clock+1) & 0x0F);               // 分
	*(clockTmp+6) = ((*(clock+0)>>4) * 10) + (*(clock+0) & 0x0F);               // 秒

	// 写入设置时间
	LPC_RTC->ALYEAR  = *(clockTmp+0);					                        // 年
	LPC_RTC->ALMON   = *(clockTmp+1);				                            // 月
	LPC_RTC->ALDOM   = *(clockTmp+2);					                        // 日

	LPC_RTC->ALDOW   = *(clockTmp+3);					                        // 周

	LPC_RTC->ALHOUR  = *(clockTmp+4);					                        // 时
	LPC_RTC->ALMIN   = *(clockTmp+5);					                        // 分
	LPC_RTC->ALSEC   = *(clockTmp+6);					                        // 秒
}

/********************************************************************************************************
* FunctionName   : RTCStartClock()
* Description    : 启动RTC时钟
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void RTCStartClock(void)
{
    LPC_RTC->CCR |= (0x01 << CLKEN); 						     			    // 置一
}

/********************************************************************************************************
* FunctionName   : RTCStopClock()
* Description    : 停止RTC时钟
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void RTCStopClock(void)
{
    LPC_RTC->CCR &= ~(0x01 << CLKEN); 						      			    // 清零
}

/********************************************************************************************************
* FunctionName   : RTCSetAddInt()
* Description    : 增值中断设置
* EntryParameter : value - 中断（对应位为1产生中断）
* ReturnValue    : None
********************************************************************************************************/
void RTCSetAddInt(u8 value)
{				                     
    LPC_RTC->CIIR = value;													   // 增值中断，0x00-不产生中断，0x01-产生中断

	/* 增值中断，0x00-不产生中断，0x01-产生中断 				                     
    LPC_RTC->CIIR  = (0x00 << IMSEC ) | 						               // 秒值的增加
	                 (0x00 << IMMIN ) | 						               // 分值的增加
					 (0x00 << IMHOUR) | 						               // 时值的增加
					 (0x00 << IMDOM ) | 						               // 日值的增加
	                 (0x00 << IMDOW ) | 						               // 周值的增加
					 (0x00 << IMDOY ) | 					                   // 日期值的增加(一年中的第几天)
					 (0x00 << IMMON ) | 						               // 月值的增加
					 (0x00 << IMYEAR);                   		               // 年值的增	 */
}

/********************************************************************************************************
* FunctionName   : RTCSetAlmInt()
* Description    : 报警中断设置
* EntryParameter : value - 中断（对应位设置为0屏蔽中断）
* ReturnValue    : None
********************************************************************************************************/
void RTCSetAlmInt(u8 value)
{				                     
    LPC_RTC->AMR = ~value;													   // 报警中断，0x00-不产生，0x01-产生

	/* 报警屏蔽，0x00-不屏蔽，0x01-屏蔽 				                
    LPC_RTC->AMR   = (0x01 << AMRSEC ) | 						               // 秒值的
	                 (0x01 << AMRMIN ) | 						               // 分值的
					 (0x01 << AMRHOUR) | 						               // 时值的
					 (0x01 << AMRDOM ) | 						               // 日值的
					 (0x01 << AMRDOW ) | 						               // 周值的
					 (0x01 << AMRDOY ) | 						               // 日期值(1~366)
					 (0x01 << AMRMON ) | 						               // 月值的
					 (0x01 << AMRYEAR);                                        // 年值的 	  */
}

/********************************************************************************************************
* FunctionName   : RTCClearAddInt()
* Description    : 清除增值中断
* EntryParameter : intType - 中断类型（1-增值中断，2-报警中断，3-两种中断都清）
* ReturnValue    : None
********************************************************************************************************/
void RTCClearInt(u8 intType)
{		
    if ((intType & RTCCIF) == RTCCIF)
	{ 		                     
        LPC_RTC->ILR |= (0x01 << RTCCIF);                                     // 清除增值中断标志
	}

	if ((intType & RTCALF) == RTCALF)
	{
	    LPC_RTC->ILR |= (0x01 << RTCALF);                                     // 清除报警中断标志
	}
} 

/********************************************************************************************************
* FunctionName   : RTCCalibration()
* Description    : 校准设置
* EntryParameter : mode - 校准模式（0-加，1-减），value - 校准值，单位为秒（相等时加减一个脉冲，大于等于1使能，等于0禁止）
* ReturnValue    : None
********************************************************************************************************/
void RTCCalibration(u8 mode, u16 value) 
{
    LPC_RTC->CCR = (0x01 << CLKEN) | (0x00 << CCALEN); 						  // 时钟使能、校准计数器使能
    LPC_RTC->CALIBRATION = (mode << CALDIR) | value;						  // 校准设置
}  

/********************************************************************************************************
  End Of File
********************************************************************************************************/
