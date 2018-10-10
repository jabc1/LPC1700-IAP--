/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : DS18B20.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*				   支持一总线上单个和多个器件的温度获取
*
********************************************************************************************************/

#ifndef __DS18B20_H
#define __DS18B20_H  

/********************************************************************************************************
* Switch compile                                                
********************************************************************************************************/
#define DST_MORE_EN			  (0x01)											   // 0x00-单个，0x01-多个

/********************************************************************************************************
* Pin Definition                                                
********************************************************************************************************/
#define DST_IO_FUN            (PIO0_5_GPIO)                                        // P0.5    
#define DST_IO_PORT       	  (PORT0)							                   // 端口
#define DST_IO_PIN       	  (5)							                 	   // 管脚

/********************************************************************************************************
* 【管脚方向】 Pin direction                                                
********************************************************************************************************/
#define DST_IO_OUT()          (GPIOSetDir(DST_IO_PORT, DST_IO_PIN, GPIO_OUT))	   // 设置为输出
#define DST_IO_IN()           (GPIOSetDir(DST_IO_PORT, DST_IO_PIN, GPIO_IN))       // 设置为输入

/********************************************************************************************************
* 【管脚读/写】 Pin Read/Write                                                
********************************************************************************************************/
#define DST_IO_SET()          (GPIOSetValue(DST_IO_PORT, DST_IO_PIN, GPIO_SET))    // 置“1” 
#define DST_IO_CLR()          (GPIOSetValue(DST_IO_PORT, DST_IO_PIN, GPIO_CLR))    // 清“0”
#define DST_IO_READ()         (GPIOStatusValue(DST_IO_PORT, DST_IO_PIN))           // 读数据  

/********************************************************************************************************
* Instruction Definition                                                  
********************************************************************************************************/
typedef enum
{
    DST_READ_ROM          = 0x33,                                                  // 读取单个器件的ROM
    DST_MATCH_ROM         = 0x55,                                                  // 匹配ROM
    DST_SKIP_ROM          = 0xCC,                                                  // 跳过ROM
    DST_SEARCH_ROM        = 0xF0,                                                  // 搜索ROM
    DST_ALARM_SEARCH      = 0xEC,                                                  // 报警搜索

    DST_READ_SCRATCHPAD   = 0xBE,                                                  // 读暂存寄存器
    DST_REOALL_EE         = 0xB8,                                                  // 重新调出EE
    DST_READ_POWER_SUPPLY = 0xB4,                                                  // 读电源

    DST_WRITE_SCRATCHPAD  = 0x4E,                                                  // 写暂存寄存器
    DST_COPY_SCRATCHPAD   = 0x48,                                                  // 复制暂存寄存器
    DST_CONVERT_T         = 0x44,                                                  // 温度变换

} DST_CMD;

//------------------------------------------------------------------------------------------------------

typedef enum
{
    DST_9BIT  = 0x1F,							                                   // 9位
    DST_10BIT = 0x3F,							                                   // 10位
    DST_11BIT = 0x5F,							                                   // 11位
    DST_12BIT = 0x7F,							                                   // 12位

} DST_CONV;																		   // 位设置

//-------------------------------------------------------------------------------------------------------

typedef enum
{
    DST_RSL_9BIT  = 500,							                               // 0.5
    DST_RSL_10BIT =	250,							                               // 0.25
    DST_RSL_11BIT =	125,							                               // 0.125
    DST_RSL_12BIT =	625,							                               // 0.0625

} DST_RSL;                                                                         // 分辨率(放大10000倍)

/********************************************************************************************************
* Variable definition                                               
********************************************************************************************************/
typedef struct
{
    u8 ConvBit;                                                                    // 转换位

} DST_PAR;

//-------------------------------------------------------------------------------------------------------

#if DST_MORE_EN
typedef struct
{
    u8 Discrepancy;                                                                // 最后差别
    u8 FamilyDiscrepancy;                                                          // 最后系列差别
    u8 DeviceFlag;                                                                 // 最后一个器件标志
	u8 Crc8;

} DST_LAST;
#endif

/********************************************************************************************************
*                               InternalFunction                                                  
********************************************************************************************************/
static void DST_Delay(u16 us);                                                     // DS18B20延时函数，产生时序 
static u8   DST_Reset(void);                                                       // DS18B20复位
static u8   DST_ReadByte(void);                                                    // DS18B20读一个字节数据
static void DST_WriteByte(u8 val);                                                 // DS18B20写一个字节数据/指令

#if DST_MORE_EN	
static u8   DST_ReadBit(void);                                                     // 读取一位数据 
static void DST_WriteBit(u8 bitDat);                                               // 写一位数据   
static u8   DST_Crc8(u8 crc8, u8 value);                                           // CRC8
static u8   DST_Search(u8 *pID);                                                   // 搜索ID
#endif

/********************************************************************************************************
*                               GlobalFunction                                                 
********************************************************************************************************/ 
extern u8   DSTInit(u8 res);                                                       // 初始化DS18B20
extern u16  DSTReadTempOne(void);                                                  // 读取温度值，总线上只能有一个器件

#if DST_MORE_EN
extern u16  DSTReadTempMore(u8 *pID);                                              // 读取温度值，有ID匹配的 
extern void DSTReadROMOne(u8* pID);                                                // 读取ROM，即ID号.总线上只能有一个器件
extern u8   DSTNext(u8 *pID);                                                      // 搜索下一个
extern u8   DSTFirst(u8 *pID);                                                     // 搜索第一个 
extern u8   DSTDevicesCount(void);                                                 // 搜索器件个数
extern u8   DSTAutoSearch(u8 *pID, u8 flag);                                       // 自动搜索ID 
#endif   

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
