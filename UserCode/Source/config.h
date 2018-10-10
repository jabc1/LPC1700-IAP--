/********************************************************************************************************    
*                                                                      
*                                     Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : config.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*				   系统配置文件，设置系统参数及调用的头文件
*
********************************************************************************************************/

#ifndef __CONFIG_H 
#define __CONFIG_H

/********************************************************************************************************
* MacroDeFinition                                                
********************************************************************************************************/
typedef enum
{
    FALSE = 0,					                          
    TRUE  = !FALSE 
	                                        
} BOOL;															   // 布尔变量

typedef enum
{
    ERROR   = 0,					                          
    SUCCESS = !ERROR 
	                                           
} ERROR_STATUS;													   // 错误状态

typedef enum 
{
    RESET = 0, 
	SET   = !RESET 

} FLAG_STATUS;													   // 标志位状态

typedef enum 
{ 
    DISABLE = 0, 
    ENABLE  = !DISABLE 

} FUN_STATE;													   // 功能状态

/********************************************************************************************************
* Interrupt switch                               
********************************************************************************************************/
#define EnableIRQ()     __enable_irq()		                       // 使能中断
#define DisableIRQ()    __disable_irq()		                       // 禁止中断


/********************************************************************************************************
* DataType                               
********************************************************************************************************/
typedef signed   int                  s32; 
typedef signed   short                s16; 
typedef signed   char                 s8;
 
typedef signed   int const            sc32;  
typedef signed   short const          sc16; 
typedef signed   char const           sc8;
  
typedef volatile signed int           vs32; 
typedef volatile signed short         vs16; 
typedef volatile signed char          vs8;
 
typedef volatile signed int const     vsc32;
typedef volatile signed short const   vsc16; 
typedef volatile signed char const    vsc8;
  
typedef unsigned int                  u32; 
typedef unsigned short                u16; 
typedef unsigned char                 u8; 

typedef unsigned int const            uc32;  
typedef unsigned short const          uc16; 
typedef unsigned char const           uc8;

typedef volatile unsigned int         vu32; 
typedef volatile unsigned short       vu16; 
typedef volatile unsigned char        vu8;
 
typedef volatile unsigned int   const vuc32;
typedef volatile unsigned short const vuc16; 
typedef volatile unsigned char  const vuc8;

typedef float                         fp32;                             
typedef double                        fp64;

/********************************************************************************************************
* SystemHeaderFiles                    
********************************************************************************************************/
#include "LPC17xx.h" 					                   // IC头文件

#include <string.h>                                        // strlen strcat memset memcpy memcmp
#include <stdlib.h>							               // 
#include <stdio.h>							               // 
#include <math.h>							               //

/********************************************************************************************************
* UserHeaderFiles            
********************************************************************************************************/
#include "../It/It.h"		                               //

// 底层驱动		       
#include "../LowDrive/Gpio/Gpio.h"		                   //
#include "../LowDrive/SSP/SSP.h"
#include "../LowDrive/I2C/I2C.h"		                   
#include "../LowDrive/Uart/Uart.h"
#include "../LowDrive/RIT/RIT.h"
#include "../LowDrive/RTC/RTC.h"
#include "../LowDrive/Wdt/Wdt.h"
#include "../LowDrive/ADC/ADC.h"
#include "../LowDrive/DAC/DAC.h"
#include "../LowDrive/Timer32/Timer32.h"
#include "../LowDrive/SysTick/SysTick.h"   

// 上层驱动
#include "../UppDrive/Key/Key.h"		                   
#include "../UppDrive/Led/Led.h"	
#include "../UppDrive/SDCard/SDCard.h"
//#include "../UppDrive/ILI9320/ILI9320.h"
#include "../UppDrive/ILI9481/ILI9481.h"
#include "../UppDrive/ST7920/ST7920.h"
#include "../UppDrive/LCD1602/LCD1602.h"		
#include "../UppDrive/Buzzer/Buzzer.h"
#include "../UppDrive/AT24C02/AT24C02.h"
#include "../UppDrive/W25Q80BV/W25Q80BV.h"
#include "../UppDrive/IRControl/IRControl.h"	
#include "../UppDrive/DS18B20/DS18B20.h"

// 中间层
#include "../MidLayer/TPC/TPC.h" 	
#include "../MidLayer/IAP/IAP.h" 
#include "../MidLayer/CMath/CMath.h"      
#include "../MidLayer/St8x16/St8x16.h"

// FatFs文件系统 
#include "../MidLayer/FatFs/ff.h"				           // 文件操作
#include "../MidLayer/FatFs/integer.h"			           // 数据类型
#include "../MidLayer/FatFs/ffconf.h"				       // 系统配置
#include "../MidLayer/FatFs/diskio.h"				       // 接口函数	             

// 应用代码
#include "../AppCode/HostCom/HostCom.h"	
#include "../AppCode/App/App.h"		                               //
#include "../AppCode/Task/Task.h"		                           //


/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
