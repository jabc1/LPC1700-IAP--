/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : Led.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef __LED_H 
#define __LED_H

/********************************************************************************************************
* Pin Definition                                                 
********************************************************************************************************/
#define LED_DB0_FUN            (PIO0_15_GPIO)    				                 // P0.15     
#define LED_DB0_PORT       	   (PORT0)							                 // 端口 
#define LED_DB0_PIN       	   (15)								                 // 管脚

#define LED_DB1_FUN            (PIO0_16_GPIO)    				                 // P0.16     
#define LED_DB1_PORT       	   (PORT0)							                 // 端口 
#define LED_DB1_PIN       	   (16)								                 // 管脚

#define LED_DB2_FUN            (PIO0_17_GPIO)    				                 // P0.17     
#define LED_DB2_PORT       	   (PORT0)							                 // 端口 
#define LED_DB2_PIN       	   (17)								                 // 管脚

#define LED_DB3_FUN            (PIO0_18_GPIO)    				                 // P0.18     
#define LED_DB3_PORT       	   (PORT0)							                 // 端口 
#define LED_DB3_PIN       	   (18)								                 // 管脚

#define LED_DB4_FUN            (PIO0_19_GPIO)    				                 // P0.19     
#define LED_DB4_PORT       	   (PORT0)							                 // 端口 
#define LED_DB4_PIN       	   (19)								                 // 管脚

#define LED_DB5_FUN            (PIO0_20_GPIO)    				                 // P0.20     
#define LED_DB5_PORT       	   (PORT0)							                 // 端口 
#define LED_DB5_PIN       	   (20)								                 // 管脚

#define LED_DB6_FUN            (PIO0_21_GPIO)    				                 // P0.21     
#define LED_DB6_PORT       	   (PORT0)							                 // 端口 
#define LED_DB6_PIN       	   (21)								                 // 管脚

#define LED_DB7_FUN            (PIO0_22_GPIO)    				                 // P0.22     
#define LED_DB7_PORT       	   (PORT0)							                 // 端口 
#define LED_DB7_PIN       	   (22)								                 // 管脚

/********************************************************************************************************
* 【管脚方向】 Pin direction                                                
********************************************************************************************************/
#define LED_DB0_OUT()          GPIOSetDir(LED_DB0_PORT, LED_DB0_PIN, GPIO_OUT)	 // 设置端口方向
#define LED_DB1_OUT()          GPIOSetDir(LED_DB1_PORT, LED_DB1_PIN, GPIO_OUT)					             
#define LED_DB2_OUT()          GPIOSetDir(LED_DB2_PORT, LED_DB2_PIN, GPIO_OUT)
#define LED_DB3_OUT()          GPIOSetDir(LED_DB3_PORT, LED_DB3_PIN, GPIO_OUT)

#define LED_DB4_OUT()          GPIOSetDir(LED_DB4_PORT, LED_DB4_PIN, GPIO_OUT)
#define LED_DB5_OUT()          GPIOSetDir(LED_DB5_PORT, LED_DB5_PIN, GPIO_OUT)
#define LED_DB6_OUT()          GPIOSetDir(LED_DB6_PORT, LED_DB6_PIN, GPIO_OUT)
#define LED_DB7_OUT()          GPIOSetDir(LED_DB7_PORT, LED_DB7_PIN, GPIO_OUT)

/********************************************************************************************************
* 【管脚写】 Pin Write                                                
********************************************************************************************************/
#define LED_DB0_WRITE(sw)      GPIOSetValue(LED_DB0_PORT, LED_DB0_PIN, sw)	     // 端口输出
#define LED_DB1_WRITE(sw)      GPIOSetValue(LED_DB1_PORT, LED_DB1_PIN, sw)					             
#define LED_DB2_WRITE(sw)      GPIOSetValue(LED_DB2_PORT, LED_DB2_PIN, sw)
#define LED_DB3_WRITE(sw)      GPIOSetValue(LED_DB3_PORT, LED_DB3_PIN, sw)

#define LED_DB4_WRITE(sw)      GPIOSetValue(LED_DB4_PORT, LED_DB4_PIN, sw)
#define LED_DB5_WRITE(sw)      GPIOSetValue(LED_DB5_PORT, LED_DB5_PIN, sw)
#define LED_DB6_WRITE(sw)      GPIOSetValue(LED_DB6_PORT, LED_DB6_PIN, sw)
#define LED_DB7_WRITE(sw)      GPIOSetValue(LED_DB7_PORT, LED_DB7_PIN, sw)

/********************************************************************************************************
* 宏定义                                              
********************************************************************************************************/
typedef enum
{
    LED_ON  = 0x00,							                                     // LED开
    LED_OFF	= 0x01,							                                     // LED关

} LED_CTRL;

//------------------------------------------------------------------------------------------------------

typedef enum
{
    LED_DB0	= (0x01 << 0),							                             // LED0
    LED_DB1	= (0x01 << 1),							                             // LED1
    LED_DB2	= (0x01 << 2),							                             // LED2
    LED_DB3	= (0x01 << 3),							                             // LED3

    LED_DB4	= (0x01 << 4),							                             // LED4
    LED_DB5	= (0x01 << 5),							                             // LED5
    LED_DB6	= (0x01 << 6),							                             // LED6
    LED_DB7	= (0x01 << 7),							                             // LED7

} LED_DB;

/********************************************************************************************************
* Global Function                                                 
********************************************************************************************************/
extern void LEDInit(void);										                 // 初始化
extern void LEDSwitchCtrl(u8 ledNum, u8 sw);				                     // led开关控制

/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
