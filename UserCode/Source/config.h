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
*				   ϵͳ�����ļ�������ϵͳ���������õ�ͷ�ļ�
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
	                                        
} BOOL;															   // ��������

typedef enum
{
    ERROR   = 0,					                          
    SUCCESS = !ERROR 
	                                           
} ERROR_STATUS;													   // ����״̬

typedef enum 
{
    RESET = 0, 
	SET   = !RESET 

} FLAG_STATUS;													   // ��־λ״̬

typedef enum 
{ 
    DISABLE = 0, 
    ENABLE  = !DISABLE 

} FUN_STATE;													   // ����״̬

/********************************************************************************************************
* Interrupt switch                               
********************************************************************************************************/
#define EnableIRQ()     __enable_irq()		                       // ʹ���ж�
#define DisableIRQ()    __disable_irq()		                       // ��ֹ�ж�


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
#include "LPC17xx.h" 					                   // ICͷ�ļ�

#include <string.h>                                        // strlen strcat memset memcpy memcmp
#include <stdlib.h>							               // 
#include <stdio.h>							               // 
#include <math.h>							               //

/********************************************************************************************************
* UserHeaderFiles            
********************************************************************************************************/
#include "../It/It.h"		                               //

// �ײ�����		       
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

// �ϲ�����
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

// �м��
#include "../MidLayer/TPC/TPC.h" 	
#include "../MidLayer/IAP/IAP.h" 
#include "../MidLayer/CMath/CMath.h"      
#include "../MidLayer/St8x16/St8x16.h"

// FatFs�ļ�ϵͳ 
#include "../MidLayer/FatFs/ff.h"				           // �ļ�����
#include "../MidLayer/FatFs/integer.h"			           // ��������
#include "../MidLayer/FatFs/ffconf.h"				       // ϵͳ����
#include "../MidLayer/FatFs/diskio.h"				       // �ӿں���	             

// Ӧ�ô���
#include "../AppCode/HostCom/HostCom.h"	
#include "../AppCode/App/App.h"		                               //
#include "../AppCode/Task/Task.h"		                           //


/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
