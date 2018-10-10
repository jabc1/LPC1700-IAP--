/******************************************************************************************************** 
*                           
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : AppCode.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                       // 头文件

/********************************************************************************************************
* Variable Definition                            
********************************************************************************************************/
APP_PRMT AppPrmt;

/******************************************************************************************************** 
* FunctionName   : App_DelayMs()
* Description    : 延时
* EntryParameter : ms - 长度
* ReturnValue    : None
********************************************************************************************************/
void App_DelayMs(u16 ms)
{
    u16 i;

	while (ms--)
	{
	    for (i=16663; i>0; i--)	 // i = 14 为us
		{
		    ;
		}
	}
}

/********************************************************************************************************
* 初始化处理                 
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : AppInit()							   
* Description    : 初始化内外设
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppInit(void)
{
	App_DelayMs(10);															// 等待微处理器复位完成
	SystemInit();												                // 初始化系统时钟
	SysTickInit(10);

	GPIOInit();											                        // GPIO
	I2CInit(I2C0, 100000);														// I2C
	SSPInit(SSP1, 0x02);
	UARTInit(UART0, 115200, UART_IT_RXDE, TRI_1BYTE);

	KeyInit(); 											                        // KEY
	LCMInit();																	// LCM
	W25QInit(); 																// Flash

	/* 这里添加片内设初始化函数 */

    App_DelayMs(20);														    // 等待各种内外设初始化完成
}


/********************************************************************************************************
* FunctionName   : AppInitSysPrmt()							   
* Description    : 初始化系统参数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppInitSysPrmt(void)
{
    /* 这里添加对各种变量及参数的初始化函数 */;
}

/********************************************************************************************************
* 各种应用代码               
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : App_Decoded()							   
* Description    : 解码函数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void App_Decoded(u8 *pBuf, u16 len)
{
    u16 i;

	for (i=0; i<len; i++)
	{
        pBuf[i] ^= APP_DECODED_VAL;
	}
}


/********************************************************************************************************
* FunctionName   : AppFlashProgram()
* Description    : 从Flash编程
* EntryParameter : startAddr - 起始地址，codeLen - 编程字节
* ReturnValue    : None
********************************************************************************************************/
void AppFlashProgram(u32 startAddr, u32 codeLen)
{
	u8  startSector, endSector, buf[APP_CODE_BUF_SIZE];
	u16 programCount;
	u32 addr = 0;
	
	startSector = APP_CODE_START_ADDR / 4096;				                // 起始扇区地址
	
	if (APP_CODE_END_ADDR <= 0x10000)						                // 0x00000~0x10000为4K每扇区
	{
	    endSector = (APP_CODE_END_ADDR / 4096) - 1;
	}
	else													                // 0x10000以后为32K每扇区
	{
	    endSector  = (0x10000 / 4096) - 1;
		endSector += (APP_CODE_END_ADDR - 0x10000) / 32768;
	}
	
	DisableIRQ(); 											                // 禁止中断
	IAPSectorPrepare(startSector, endSector);                               // 选择扇区     
	IAPSectorErase(startSector, endSector);                                 // 擦除扇区
	EnableIRQ();											                // 使能中断

	while (codeLen > 0)
	{
		if (codeLen > APP_CODE_BUF_SIZE)
		{
		    W25QReadNByte(startAddr + addr, buf, APP_CODE_BUF_SIZE);        // 从Flash中读取
			App_Decoded(buf, APP_CODE_BUF_SIZE);
			programCount = APP_CODE_BUF_SIZE;
			codeLen -= APP_CODE_BUF_SIZE;
		}
		else
		{
		    memset(buf, 0xFF, APP_CODE_BUF_SIZE);						    // 缓冲内填充0xFF
			W25QReadNByte(startAddr + addr, buf, codeLen);                  // 从Flash中读取
		    App_Decoded(buf, codeLen);
			programCount = codeLen;
			codeLen = 0;

			// 满足编程字节数的要求256、512、1024、4096等
			programCount = ((programCount > 1024) ? APP_CODE_BUF_SIZE :	
			               ((programCount > 512 ) ? 1024 : 
						   ((programCount > 256 ) ? 512  : 256)));
		}

		DisableIRQ(); 											                // 禁止中断
		IAPSectorPrepare(startSector, endSector);					            // 选择扇区
		IAPRamToFlash(APP_CODE_START_ADDR + addr, (u32)buf, programCount);      // 写数据到FLASH
	    EnableIRQ();											                // 使能中断
		addr += programCount;										            // 写入字节加		
	}
}

/********************************************************************************************************
* FunctionName   : AppSDCardProgram()
* Description    : 从SD卡编程
* EntryParameter : fileName - 应用程序在SD卡中的名字
* ReturnValue    : None
********************************************************************************************************/
u8 AppSDCardProgram(u8 *fileName)
{
    u32 addr = 0;
	FATFS fs;               
    FIL file;               
	UINT  br;               
	FRESULT res;
	u8 startSector, endSector;
	u8 buf[512];

    f_mount(0, &fs);
	res = f_open(&file, (const TCHAR *)fileName, FA_OPEN_EXISTING|FA_READ);	    

    if (res == FR_OK)
    {
		startSector = APP_CODE_START_ADDR / 4096;				                 // 起始扇区地址
		
		if (APP_CODE_END_ADDR <= 0x10000)						                 // 0x00000~0x10000为4K每扇区
		{
		    endSector = (APP_CODE_END_ADDR / 4096) - 1;
		}
		else													                 // 0x10000以后为32K每扇区
		{
		    endSector  = (0x10000 / 4096) - 1;
			endSector += (APP_CODE_END_ADDR - 0x10000) / 32768;
		}	    
		
		DisableIRQ(); 											                 // 禁止中断
		IAPSectorPrepare(startSector, endSector);                                // 选择扇区     
		IAPSectorErase(startSector, endSector);                                  // 擦除扇区
		EnableIRQ();

		while (1)
		{
		    res = f_read(&file, buf, 512, &br);	                                 // 读取数据

			DisableIRQ();
			IAPSectorPrepare(startSector, endSector);					         // 选择扇区
			App_Decoded(buf, 512);
			IAPRamToFlash(APP_CODE_START_ADDR + addr, (u32)buf, 512);            // 写数据到FLASH
			EnableIRQ();
			addr += 512;

			if ((res != FR_OK) || (br < 512))
			{
				break;
			}
		}
	}

   	f_close(&file);                                                         
    f_mount(0, 0);

    return FR_OK;
}

/********************************************************************************************************
* FunctionName   : App_BootJump()
* Description    : 跳转到应用程序
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
__asm void App_BootJump(u32 address)
{
	LDR SP, [R0]		                                                        // 承载新的堆栈指??针地址 
	LDR PC, [R0, #4]	                                                        // 加载新的程序计数器地址
}

/********************************************************************************************************
* FunctionName   : AppNVICSetVectTab()
* Description    : 中断向量表重映射
* EntryParameter : offset - 偏移地址
* ReturnValue    : None
********************************************************************************************************/
void AppNVICSetVectTab(u32 offset)
{
   SCB->VTOR = offset & 0x1FFFFF80;						                         // NVIC的向量重映射
}

/********************************************************************************************************
* FunctionName   : AppExecuteUserCode()
* Description    : 运行用户程序
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppExecuteUserCode(void) 
{                   
	AppNVICSetVectTab(APP_CODE_START_ADDR);										// 重映射
    App_BootJump(APP_CODE_START_ADDR);										    // 跳转到应用程序
}

/********************************************************************************************************
* FunctionName   : AppDisJumToUser()							   
* Description    : 显示跳转到用户程序提示
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppDisJumToUser(void)
{
    LCMDisplayStr1(0, LCM_LINE2, 0x00, "  Jump to user   ");
	LCMDisplayStr1(0, LCM_LINE3, 0x00, "   Please wait   ");
}

/********************************************************************************************************
* FunctionName   : AppDisSDUpgrade()							   
* Description    : 显示SD卡升级提示
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppDisSDUpgrade(void)
{
    LCMDisplayStr1(0, LCM_LINE2, 0x00, " SDCard upgrade ");
	LCMDisplayStr1(0, LCM_LINE3, 0x00, "  Please wait   ");
}

/********************************************************************************************************
* FunctionName   : AppDisSerialDown()							   
* Description    : 显示串口下载提示
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppDisSerialDown(void)
{
    LCMDisplayStr1(0, LCM_LINE2, 0x00, "   Open serial  ");
	LCMDisplayStr1(0, LCM_LINE3, 0x00, "     Download   ");
}

/********************************************************************************************************
* FunctionName   : AppDisDown()							   
* Description    : 显示下载
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppDisDown(HC_PRMT *prmt)
{
    u8 i, disBuf[16] = {"                "};
	u16 tmp;
	static u16 tmpCur = 0xFF;

	if (tmpCur != prmt->CurPack)
	{
	    tmpCur = prmt->CurPack;
		if (prmt->DownMode == 0x01)
		{
		    LCMDisplayStr1(0, LCM_LINE2, 0x00, " Download Font: ");    
		}                                      
		else
		{
			LCMDisplayStr1(0, LCM_LINE2, 0x00, " Download Code: ");
		}
		
		if (prmt->CurPack > 0)
		{								   
			if (prmt->TtlPack != prmt->CurPack)
			{
			    tmp = prmt->CurPack;
			    disBuf[ 4] = tmp / 1000 + '0';
                tmp %= 1000;
			    disBuf[ 5] = (tmp / 100) + '0';
				tmp %= 100;
				disBuf[ 6] = (tmp / 10) + '0';
				disBuf[ 7] = (tmp % 10) + '0';

				disBuf[ 8] = '/'; 

				tmp = prmt->TtlPack;
			    disBuf[ 9] = tmp / 1000 + '0';
                tmp %= 1000;
			    disBuf[10] = (tmp / 100) + '0';
				tmp %= 100;
				disBuf[11] = (tmp / 10) + '0';
				disBuf[12] = (tmp % 10) + '0';

				LCMDisplayStr1(0, LCM_LINE3, 0x00, disBuf);	
			}
			else
			{								   
			    LCMDisplayStr1(0, LCM_LINE3, 0x00, " -= The End =- ");
			}
		}
		else
		{
		     memcpy(disBuf, "   0x", 5);
			 for (i=0; i<8; i++)
			 {
			     disBuf[5+i]  = ((prmt->StartAddr >> ((7-i)*4)) & 0x0F);	   // 
				 disBuf[5+i] += ((disBuf[5+i] > 9) ? 0x37 : 0x30);			   // 转换成ASC
			 }
	
		     LCMDisplayStr1(0, LCM_LINE3, 0x00, disBuf);
		}
	}									    	 
}						 

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
