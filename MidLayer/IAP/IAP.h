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
* 定义CCLK值大小，单位为KHz                                                
********************************************************************************************************/ 
#define IAP_FCCLK                        (25000)			   // 频率(单位：KHz)
#define IAP_ENTER_ADR                    (0x1FFF1FF1)          // IAP入口地址定义              

/********************************************************************************************************
* 定义IAP命令字                                                
********************************************************************************************************/  
typedef enum   
{                           
    IAP_PREPARE      = 50,                                     // 选择扇区                      
    IAP_RAMTOFLASH   = 51,                                     // 拷贝数据
    IAP_ERASESECTOR  = 52,                                     // 擦除扇区   
    IAP_BLANKCHK     = 53,                                     // 查空扇区                
    IAP_READPARTID   = 54,                                     // 读器件ID          
    IAP_BOOTCODEID   = 55,                                     // 读Boot版本号         
    IAP_COMPARE      = 56,                                     // 比较命令                     
    IAP_RECALLISP 	 = 57,				                       // 重新调用ISP                  
    IAP_READDEVICESN = 58,			  	                       // 读取设备序列号   

} IAP_CMD;            

/********************************************************************************************************
* 定义IAP返回状态字                                                
********************************************************************************************************/
typedef enum 
{
    IAP_CMD_SUCCESS                             = 0,		   // 命令被成功执行。只有当主机发出的命令被成功执行完毕后，才由ISP处理器发送
    IAP_INVALID_COMMAND                         = 1,		   // 无效命令
    IAP_SRC_ADDR_ERROR                          = 2, 		   // 源地址没有以字为边界
    IAP_DST_ADDR_ERROR                          = 3,		   // 目标地址的边界错误
    IAP_SRC_ADDR_NOT_MAPPED                     = 4,		   // 源地址没有位于存储器映射中。计数值必须考虑可用性
    IAP_DST_ADDR_NOT_MAPPED                     = 5,		   // 目标地址没有在存储器映射中被映射。计数值必须考虑到可用性
    IAP_COUNT_ERROR                             = 6,		   // 字节计数值不是4的倍数或是一个非法值
    IAP_INVALID_SECTOR                          = 7,		   // 扇区号无效或结束扇区号小于起始扇区号
    IAP_SECTOR_NOT_BLANK                        = 8,		   // 扇区非空
    IAP_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION = 9,		   // 为写操作准备扇区的命令未执行
    IAP_COMPARE_ERROR                           = 10,		   // 源和目标数据不相等
    IAP_BUSY                                    = 11,		   // Flash编程硬件接口忙
    IAP_PARAM_ERROR                             = 12,		   // 参数不足或无效参数
    IAP_ADDR_ERROR                              = 13,		   // 地址没有以字为边界
    IAP_ADDR_NOT_MAPPED                         = 14,		   // 地址没有位于存储器映射中。计数值必须考虑可用性
    IAP_CMD_LOCKED                              = 15,		   // 命令被锁定
    IAP_INVALID_CODE                            = 16,		   // 解锁代码无效
    IAP_INVALID_BAUD_RATE                       = 17,		   // 无效波特率设定
    IAP_INVALID_STOP_BIT                        = 18,		   // 无效停止位设定
    IAP_CODE_READ_PROTECTION_ENABLED            = 19,		   // 代码读保护使能

} IAP_REVAL;

/********************************************************************************************************
* IAP函数入口                                               
********************************************************************************************************/
extern void (*IAP_Entry) (u32 prmTab[], u32 rslTab[]);						// IAP入口

/********************************************************************************************************
* Global function                           
********************************************************************************************************/
extern u32  IAPSectorPrepare(u8 sec1, u8 sec2);							    // IAP操作扇区选择，命令代码50
extern u32  IAPRamToFlash(u32 dst, u32 src, u32 no);						// 复制RAM的数据到FLASH，命令代码51
extern u32  IAPSectorErase(u8 sec1, u8 sec2);							    // 扇区擦除，命令代码52
extern u32  IAPBlankChk(u8 sec1, u8 sec2);								    // 扇区查空，命令代码53
extern u32  IAPParIdRead(void);											    // 扇区查空，命令代码54
extern u32  IAPCodeIdBoot(void);										  	// 扇区查空，命令代码55
extern u32  IAPDataCompare(u32 dst, u32 src, u32 no);					    // 校验数据，命令代码56
extern u32  IAPReCallIsp(void);											    // 重新调用ISP 57
extern u32  IAPDeviceSNRead(void);										    // 读器件序列号58
extern void IAPReadFlash(u32 addr, u8 *pBuf, u16 len);					    // 读Flash

/*******************************************************************************************************/
#endif
									 
/********************************************************************************************************
* End Of File         
********************************************************************************************************/

