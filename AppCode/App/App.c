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
#include "../UserCode/source/config.h"    			                       // ͷ�ļ�

/********************************************************************************************************
* Variable Definition                            
********************************************************************************************************/
APP_PRMT AppPrmt;

/******************************************************************************************************** 
* FunctionName   : App_DelayMs()
* Description    : ��ʱ
* EntryParameter : ms - ����
* ReturnValue    : None
********************************************************************************************************/
void App_DelayMs(u16 ms)
{
    u16 i;

	while (ms--)
	{
	    for (i=16663; i>0; i--)	 // i = 14 Ϊus
		{
		    ;
		}
	}
}

/********************************************************************************************************
* ��ʼ������                 
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : AppInit()							   
* Description    : ��ʼ��������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppInit(void)
{
	App_DelayMs(10);															// �ȴ�΢��������λ���
	SystemInit();												                // ��ʼ��ϵͳʱ��
	SysTickInit(10);

	GPIOInit();											                        // GPIO
	I2CInit(I2C0, 100000);														// I2C
	SSPInit(SSP1, 0x02);
	UARTInit(UART0, 115200, UART_IT_RXDE, TRI_1BYTE);

	KeyInit(); 											                        // KEY
	LCMInit();																	// LCM
	W25QInit(); 																// Flash

	/* �������Ƭ�����ʼ������ */

    App_DelayMs(20);														    // �ȴ������������ʼ�����
}


/********************************************************************************************************
* FunctionName   : AppInitSysPrmt()							   
* Description    : ��ʼ��ϵͳ����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppInitSysPrmt(void)
{
    /* ������ӶԸ��ֱ����������ĳ�ʼ������ */;
}

/********************************************************************************************************
* ����Ӧ�ô���               
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : App_Decoded()							   
* Description    : ���뺯��
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
* Description    : ��Flash���
* EntryParameter : startAddr - ��ʼ��ַ��codeLen - ����ֽ�
* ReturnValue    : None
********************************************************************************************************/
void AppFlashProgram(u32 startAddr, u32 codeLen)
{
	u8  startSector, endSector, buf[APP_CODE_BUF_SIZE];
	u16 programCount;
	u32 addr = 0;
	
	startSector = APP_CODE_START_ADDR / 4096;				                // ��ʼ������ַ
	
	if (APP_CODE_END_ADDR <= 0x10000)						                // 0x00000~0x10000Ϊ4Kÿ����
	{
	    endSector = (APP_CODE_END_ADDR / 4096) - 1;
	}
	else													                // 0x10000�Ժ�Ϊ32Kÿ����
	{
	    endSector  = (0x10000 / 4096) - 1;
		endSector += (APP_CODE_END_ADDR - 0x10000) / 32768;
	}
	
	DisableIRQ(); 											                // ��ֹ�ж�
	IAPSectorPrepare(startSector, endSector);                               // ѡ������     
	IAPSectorErase(startSector, endSector);                                 // ��������
	EnableIRQ();											                // ʹ���ж�

	while (codeLen > 0)
	{
		if (codeLen > APP_CODE_BUF_SIZE)
		{
		    W25QReadNByte(startAddr + addr, buf, APP_CODE_BUF_SIZE);        // ��Flash�ж�ȡ
			App_Decoded(buf, APP_CODE_BUF_SIZE);
			programCount = APP_CODE_BUF_SIZE;
			codeLen -= APP_CODE_BUF_SIZE;
		}
		else
		{
		    memset(buf, 0xFF, APP_CODE_BUF_SIZE);						    // ���������0xFF
			W25QReadNByte(startAddr + addr, buf, codeLen);                  // ��Flash�ж�ȡ
		    App_Decoded(buf, codeLen);
			programCount = codeLen;
			codeLen = 0;

			// �������ֽ�����Ҫ��256��512��1024��4096��
			programCount = ((programCount > 1024) ? APP_CODE_BUF_SIZE :	
			               ((programCount > 512 ) ? 1024 : 
						   ((programCount > 256 ) ? 512  : 256)));
		}

		DisableIRQ(); 											                // ��ֹ�ж�
		IAPSectorPrepare(startSector, endSector);					            // ѡ������
		IAPRamToFlash(APP_CODE_START_ADDR + addr, (u32)buf, programCount);      // д���ݵ�FLASH
	    EnableIRQ();											                // ʹ���ж�
		addr += programCount;										            // д���ֽڼ�		
	}
}

/********************************************************************************************************
* FunctionName   : AppSDCardProgram()
* Description    : ��SD�����
* EntryParameter : fileName - Ӧ�ó�����SD���е�����
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
		startSector = APP_CODE_START_ADDR / 4096;				                 // ��ʼ������ַ
		
		if (APP_CODE_END_ADDR <= 0x10000)						                 // 0x00000~0x10000Ϊ4Kÿ����
		{
		    endSector = (APP_CODE_END_ADDR / 4096) - 1;
		}
		else													                 // 0x10000�Ժ�Ϊ32Kÿ����
		{
		    endSector  = (0x10000 / 4096) - 1;
			endSector += (APP_CODE_END_ADDR - 0x10000) / 32768;
		}	    
		
		DisableIRQ(); 											                 // ��ֹ�ж�
		IAPSectorPrepare(startSector, endSector);                                // ѡ������     
		IAPSectorErase(startSector, endSector);                                  // ��������
		EnableIRQ();

		while (1)
		{
		    res = f_read(&file, buf, 512, &br);	                                 // ��ȡ����

			DisableIRQ();
			IAPSectorPrepare(startSector, endSector);					         // ѡ������
			App_Decoded(buf, 512);
			IAPRamToFlash(APP_CODE_START_ADDR + addr, (u32)buf, 512);            // д���ݵ�FLASH
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
* Description    : ��ת��Ӧ�ó���
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
__asm void App_BootJump(u32 address)
{
	LDR SP, [R0]		                                                        // �����µĶ�ջָ??���ַ 
	LDR PC, [R0, #4]	                                                        // �����µĳ����������ַ
}

/********************************************************************************************************
* FunctionName   : AppNVICSetVectTab()
* Description    : �ж���������ӳ��
* EntryParameter : offset - ƫ�Ƶ�ַ
* ReturnValue    : None
********************************************************************************************************/
void AppNVICSetVectTab(u32 offset)
{
   SCB->VTOR = offset & 0x1FFFFF80;						                         // NVIC��������ӳ��
}

/********************************************************************************************************
* FunctionName   : AppExecuteUserCode()
* Description    : �����û�����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void AppExecuteUserCode(void) 
{                   
	AppNVICSetVectTab(APP_CODE_START_ADDR);										// ��ӳ��
    App_BootJump(APP_CODE_START_ADDR);										    // ��ת��Ӧ�ó���
}

/********************************************************************************************************
* FunctionName   : AppDisJumToUser()							   
* Description    : ��ʾ��ת���û�������ʾ
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
* Description    : ��ʾSD��������ʾ
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
* Description    : ��ʾ����������ʾ
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
* Description    : ��ʾ����
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
				 disBuf[5+i] += ((disBuf[5+i] > 9) ? 0x37 : 0x30);			   // ת����ASC
			 }
	
		     LCMDisplayStr1(0, LCM_LINE3, 0x00, disBuf);
		}
	}									    	 
}						 

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
