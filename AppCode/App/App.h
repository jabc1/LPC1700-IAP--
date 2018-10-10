/********************************************************************************************************   
*                                                                      
*                                     Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : App.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef  __APP__H__
#define  __APP__H__	

/********************************************************************************************************
* Macro definition                                              
********************************************************************************************************/
#define APP_UPG_ADDR			 (0x00000010)				                 // 升级标志
#define APP_CODE_START_ADDR	     (0x00006000)				                 // 用户程序起始地址
#define APP_CODE_END_ADDR	     (0x00040000)				                 // 用户程序终止地址
#define APP_CODE_BUF_SIZE	     (4096)						  	             // 缓冲大小

#define APP_DECODED_VAL	         (0xA5)						  	             // 解码值

/********************************************************************************************************
* Constant definition                                               
********************************************************************************************************/ 
typedef enum
{
    APP_KV_NO = 0x00,					                                     // 无
	APP_KV_UP = 0x02,					                                     // 上
	APP_KV_DN = 0x04,					                                     // 下
	APP_KV_MD = 0x08,					                                     // 中
	APP_KV_LF = 0x10,					                                     // 左
	APP_KV_RT = 0x20,					                                     // 右

} APP_KEY_VALUE;														     // 按键值

typedef enum
{
    APP_DI_JAPP = 0x00,						        					     // 跳转到用户程序
    APP_DI_SDUG = 0x01,													     // SD卡升级
	APP_DI_SDWN = 0x02,													     // 串口下载
	APP_DI_DOWN = 0x03,                                                      // 下载模式

} APP_DISP_ITEM;

//-------------------------------------------------------------------------------------------------------
typedef struct _APP_PRMT
{
	u8 DispItem;										                     // 显示项
	u8 WorkMode;															 // 工作模式

} APP_PRMT;

extern APP_PRMT AppPrmt;

/********************************************************************************************************
* Variable definition                                               
********************************************************************************************************/

				   
/********************************************************************************************************
* Internal function                           
********************************************************************************************************/
extern void App_Decoded(u8 *pBuf, u16 len);                                  // 解码

/********************************************************************************************************
* Global function                           
********************************************************************************************************/
extern void AppInit(void);													 //
extern void AppInitSysPrmt(void);											 //

extern void AppDisJumToUser(void);											 //
extern void AppDisSDUpgrade(void);											 //
extern void AppDisSerialDown(void);											 //
extern void AppDisDown(HC_PRMT *prmt);										 // 显示下载

extern void AppFlashProgram(u32 startAddr, u32 codeLen);                     // Flash在线升级 
extern u8   AppSDCardProgram(u8 *fileName);                                  // SD在线升级 
extern void AppExecuteUserCode(void); 						                 // 执行用户程序
extern void AppNVICSetVectTab(u32 offset);									 // 重映射
extern void AppDownload(void);												 // 下载
									  
/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
