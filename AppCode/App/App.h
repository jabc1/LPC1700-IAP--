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
#define APP_UPG_ADDR			 (0x00000010)				                 // ������־
#define APP_CODE_START_ADDR	     (0x00006000)				                 // �û�������ʼ��ַ
#define APP_CODE_END_ADDR	     (0x00040000)				                 // �û�������ֹ��ַ
#define APP_CODE_BUF_SIZE	     (4096)						  	             // �����С

#define APP_DECODED_VAL	         (0xA5)						  	             // ����ֵ

/********************************************************************************************************
* Constant definition                                               
********************************************************************************************************/ 
typedef enum
{
    APP_KV_NO = 0x00,					                                     // ��
	APP_KV_UP = 0x02,					                                     // ��
	APP_KV_DN = 0x04,					                                     // ��
	APP_KV_MD = 0x08,					                                     // ��
	APP_KV_LF = 0x10,					                                     // ��
	APP_KV_RT = 0x20,					                                     // ��

} APP_KEY_VALUE;														     // ����ֵ

typedef enum
{
    APP_DI_JAPP = 0x00,						        					     // ��ת���û�����
    APP_DI_SDUG = 0x01,													     // SD������
	APP_DI_SDWN = 0x02,													     // ��������
	APP_DI_DOWN = 0x03,                                                      // ����ģʽ

} APP_DISP_ITEM;

//-------------------------------------------------------------------------------------------------------
typedef struct _APP_PRMT
{
	u8 DispItem;										                     // ��ʾ��
	u8 WorkMode;															 // ����ģʽ

} APP_PRMT;

extern APP_PRMT AppPrmt;

/********************************************************************************************************
* Variable definition                                               
********************************************************************************************************/

				   
/********************************************************************************************************
* Internal function                           
********************************************************************************************************/
extern void App_Decoded(u8 *pBuf, u16 len);                                  // ����

/********************************************************************************************************
* Global function                           
********************************************************************************************************/
extern void AppInit(void);													 //
extern void AppInitSysPrmt(void);											 //

extern void AppDisJumToUser(void);											 //
extern void AppDisSDUpgrade(void);											 //
extern void AppDisSerialDown(void);											 //
extern void AppDisDown(HC_PRMT *prmt);										 // ��ʾ����

extern void AppFlashProgram(u32 startAddr, u32 codeLen);                     // Flash�������� 
extern u8   AppSDCardProgram(u8 *fileName);                                  // SD�������� 
extern void AppExecuteUserCode(void); 						                 // ִ���û�����
extern void AppNVICSetVectTab(u32 offset);									 // ��ӳ��
extern void AppDownload(void);												 // ����
									  
/*******************************************************************************************************/
#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
