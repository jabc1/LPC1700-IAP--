/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : iap.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23              JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef __IAP_H
#define	__IAP_H

/********************************************************************************************************
* ����CCLKֵ��С����λΪKHz                                                
********************************************************************************************************/ 
#define IAP_FCCLK                        (25000)			   // Ƶ��(��λ��KHz)
#define IAP_ENTER_ADR                    (0x1FFF1FF1)          // IAP��ڵ�ַ����              

/********************************************************************************************************
* ����IAP������                                                
********************************************************************************************************/  
typedef enum   
{                           
    IAP_PREPARE      = 50,                                     // ѡ������                      
    IAP_RAMTOFLASH   = 51,                                     // ��������
    IAP_ERASESECTOR  = 52,                                     // ��������   
    IAP_BLANKCHK     = 53,                                     // �������                
    IAP_READPARTID   = 54,                                     // ������ID          
    IAP_BOOTCODEID   = 55,                                     // ��Boot�汾��         
    IAP_COMPARE      = 56,                                     // �Ƚ�����                     
    IAP_RECALLISP 	 = 57,				                       // ���µ���ISP                  
    IAP_READDEVICESN = 58,			  	                       // ��ȡ�豸���к�   

} IAP_CMD;            

/********************************************************************************************************
* ����IAP����״̬��                                                
********************************************************************************************************/
typedef enum 
{
    IAP_CMD_SUCCESS                             = 0,		   // ����ɹ�ִ�С�ֻ�е���������������ɹ�ִ����Ϻ󣬲���ISP����������
    IAP_INVALID_COMMAND                         = 1,		   // ��Ч����
    IAP_SRC_ADDR_ERROR                          = 2, 		   // Դ��ַû������Ϊ�߽�
    IAP_DST_ADDR_ERROR                          = 3,		   // Ŀ���ַ�ı߽����
    IAP_SRC_ADDR_NOT_MAPPED                     = 4,		   // Դ��ַû��λ�ڴ洢��ӳ���С�����ֵ���뿼�ǿ�����
    IAP_DST_ADDR_NOT_MAPPED                     = 5,		   // Ŀ���ַû���ڴ洢��ӳ���б�ӳ�䡣����ֵ���뿼�ǵ�������
    IAP_COUNT_ERROR                             = 6,		   // �ֽڼ���ֵ����4�ı�������һ���Ƿ�ֵ
    IAP_INVALID_SECTOR                          = 7,		   // ��������Ч�����������С����ʼ������
    IAP_SECTOR_NOT_BLANK                        = 8,		   // �����ǿ�
    IAP_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION = 9,		   // Ϊд����׼������������δִ��
    IAP_COMPARE_ERROR                           = 10,		   // Դ��Ŀ�����ݲ����
    IAP_BUSY                                    = 11,		   // Flash���Ӳ���ӿ�æ
    IAP_PARAM_ERROR                             = 12,		   // �����������Ч����
    IAP_ADDR_ERROR                              = 13,		   // ��ַû������Ϊ�߽�
    IAP_ADDR_NOT_MAPPED                         = 14,		   // ��ַû��λ�ڴ洢��ӳ���С�����ֵ���뿼�ǿ�����
    IAP_CMD_LOCKED                              = 15,		   // �������
    IAP_INVALID_CODE                            = 16,		   // ����������Ч
    IAP_INVALID_BAUD_RATE                       = 17,		   // ��Ч�������趨
    IAP_INVALID_STOP_BIT                        = 18,		   // ��Чֹͣλ�趨
    IAP_CODE_READ_PROTECTION_ENABLED            = 19,		   // ���������ʹ��

} IAP_REVAL;

/********************************************************************************************************
* IAP�������                                               
********************************************************************************************************/
extern void (*IAP_Entry) (u32 prmTab[], u32 rslTab[]);						// IAP���

/********************************************************************************************************
* Global function                           
********************************************************************************************************/
extern u32  IAPSectorPrepare(u8 sec1, u8 sec2);							    // IAP��������ѡ���������50
extern u32  IAPRamToFlash(u32 dst, u32 src, u32 no);						// ����RAM�����ݵ�FLASH���������51
extern u32  IAPSectorErase(u8 sec1, u8 sec2);							    // �����������������52
extern u32  IAPBlankChk(u8 sec1, u8 sec2);								    // ������գ��������53
extern u32  IAPParIdRead(void);											    // ������գ��������54
extern u32  IAPCodeIdBoot(void);										  	// ������գ��������55
extern u32  IAPDataCompare(u32 dst, u32 src, u32 no);					    // У�����ݣ��������56
extern u32  IAPReCallIsp(void);											    // ���µ���ISP 57
extern u32  IAPDeviceSNRead(void);										    // ���������к�58
extern void IAPReadFlash(u32 addr, u8 *pBuf, u16 len);					    // ��Flash

/*******************************************************************************************************/
#endif
									 
/********************************************************************************************************
* End Of File         
********************************************************************************************************/
