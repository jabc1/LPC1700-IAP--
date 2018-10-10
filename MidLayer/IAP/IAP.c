/********************************************************************************************************
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : iap.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23              JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                         // 头文件

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/ 
void (*IAP_Entry)(u32 *prmTab, u32 *rslTab) = (void(*)())IAP_ENTER_ADR;      // 定义函数指针                  

u32  IAP_PrmIn[5];                                                           // IAP入口参数缓冲区            
u32  IAP_PrmOut[5];                                                          // IAP出口参数缓冲区            

/********************************************************************************************************
* FunctionName   : IAPSectorPrepare()							   
* Description    : IAP操作扇区选择，命令代码50
* EntryParameter : sec1 - 起始扇区,	sec2 - 终止扇区
* ReturnValue    : None
********************************************************************************************************/
u32 IAPSectorPrepare(u8 sec1, u8 sec2)
{  
    IAP_PrmIn[0] = IAP_PREPARE;                                              // 设置命令字                   
    IAP_PrmIn[1] = sec1;                                                     // 设置参数                     
    IAP_PrmIn[2] = sec2;                            
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序             
   
    return (IAP_PrmOut[0]);                                                  // 返回状态码                   
}

/********************************************************************************************************
* FunctionName   : IAPRamToFlash()							   
* Description    : 复制RAM的数据到FLASH，命令代码51
* EntryParameter : dst - 目标地址, src - 源地址, no	- 复制字节个数
* ReturnValue    : None
********************************************************************************************************/
u32 IAPRamToFlash(u32 dst, u32 src, u32 no)
{  
    IAP_PrmIn[0] = IAP_RAMTOFLASH;                                           // 设置命令字                  
    IAP_PrmIn[1] = dst;                                                      // 目标地址，即FLASH起始地址。以256字节为分界                    
    IAP_PrmIn[2] = src;														 // 源地址，即RAM地址。地址必须字对齐
    IAP_PrmIn[3] = no;														 // 复制字节个数，为256/512/1024/4096
    IAP_PrmIn[4] = IAP_FCCLK;
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序              
    
    return (IAP_PrmOut[0]);                                                  // 返回状态码                   
}

/********************************************************************************************************
* FunctionName   : IAPSectorErase()							   
* Description    : 扇区擦除，命令代码52
* EntryParameter : sec1 - 起始扇区,	sec2 - 终止扇区92
* ReturnValue    : None
********************************************************************************************************/
u32 IAPSectorErase(u8 sec1, u8 sec2)
{  
    IAP_PrmIn[0] = IAP_ERASESECTOR;                                          // 设置命令字                   
    IAP_PrmIn[1] = sec1;                                                     // 设置参数                     
    IAP_PrmIn[2] = sec2;
    IAP_PrmIn[3] = IAP_FCCLK;
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序              
   
    return (IAP_PrmOut[0]);                                                  // 返回状态码                   
}

/********************************************************************************************************
* FunctionName   : IAPBlankChk()							   
* Description    : 扇区查空，命令代码53
* EntryParameter : sec1 - 起始扇区,	sec2 - 终止扇区92
* ReturnValue    : None
********************************************************************************************************/
u32 IAPBlankChk(u8 sec1, u8 sec2)
{  
    IAP_PrmIn[0] = IAP_BLANKCHK;                                             // 设置命令字                   
    IAP_PrmIn[1] = sec1;                                                     // 设置参数                     
    IAP_PrmIn[2] = sec2;
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序              

    return (IAP_PrmOut[0]);                                                  // 返回状态码                   
}

/********************************************************************************************************
* FunctionName   : IAPParIdRead()							   
* Description    : 读器件标识号54
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPParIdRead(void)
{  
    IAP_PrmIn[0] = IAP_READPARTID;                                           // 设置命令字                   
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序              

    return (IAP_PrmOut[0]);                                                  // 返回状态码                   
}

/********************************************************************************************************
* FunctionName   : IAPCodeIdBoot()							   
* Description    : 读Boot 代码版本号55
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPCodeIdBoot(void)
{  
    IAP_PrmIn[0] = IAP_BOOTCODEID;                                          // 设置命令字                 
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                    // 调用IAP服务程序            

    return (IAP_PrmOut[0]);                                                 // 返回状态码                 
}

/********************************************************************************************************
* FunctionName   : IAPDataCompare()							   
* Description    : 校验数据，命令代码56
* EntryParameter : dst - 目标地址, src - 源地址, no - 复制字节个数
* ReturnValue    : None
********************************************************************************************************/
u32 IAPDataCompare(u32 dst, u32 src, u32 no)
{  
    IAP_PrmIn[0] = IAP_COMPARE;                                              // 设置命令字                 
    IAP_PrmIn[1] = dst;                                                      // 目标地址，即RAM/FLASH起始地址。地址必须字对齐                   
    IAP_PrmIn[2] = src;														 // 源地址，即FLASH/RAM地址。地址必须字对齐
    IAP_PrmIn[3] = no;														 // 复制字节个数，必须能被4整除
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序            

    return (IAP_PrmOut[0]);                                                  // 返回状态码                 
}

/********************************************************************************************************
* FunctionName   : IAPReCallIsp()							   
* Description    : 重新调用ISP 57
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPReCallIsp(void)
{  
    IAP_PrmIn[0] = IAP_RECALLISP;                                            // 设置命令字                 
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序            

    return (IAP_PrmOut[0]);                                                  // 返回状态码                 
}

/********************************************************************************************************
* FunctionName   : IAPDeviceSNRead()							   
* Description    : 读器件序列号58
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
u32 IAPDeviceSNRead(void)
{  
    IAP_PrmIn[0] = IAP_READDEVICESN;                                         // 设置命令字                 
    (*IAP_Entry)(IAP_PrmIn, IAP_PrmOut);                                     // 调用IAP服务程序            

    return (IAP_PrmOut[0]);                                                  // 返回状态码                 
}

/********************************************************************************************************
* FunctionName   : IAPReadFlash()							   
* Description    : 读Flash
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void IAPReadFlash(u32 addr, u8 *pBuf, u16 len)
{
    u16 i;

	for (i=0; i<len; i++)
	{
	    *(pBuf+i) = *(u8 *)(addr+i);							              // 读直接地址
	}
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
