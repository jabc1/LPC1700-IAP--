/******************************************************************************************************** 
*                           
*                                    Address
*  							    
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : W25Q80BV.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : W25Q80BV 1 Mbit(1M x 8) Serial Flash Memory
*
*
********************************************************************************************************/

/********************************************************************************************************
* header file                                                 
********************************************************************************************************/
#include "../UserCode/source/config.h" 

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : W25Q_SetGpio()
* Description    : �ܽ�����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void W25Q_SetGpio(void)
{
	W25Q_CS_OUT();			                                     // ����ƬѡΪ����˿�
}

/********************************************************************************************************
* FunctionName   : W25Q_Enable()
* Description    : W25QƬѡ
* EntryParameter : en - 0ʹ�ܣ�1��ֹ
* ReturnValue    : None
********************************************************************************************************/
void W25Q_Enable(u8 en)
{
    W25Q_CS_WRITE(en);    
}

/********************************************************************************************************
* FunctionName   : W25Q_WriteAddr()
* Description    : W25Qд��ַ
* EntryParameter : addr - 24λ��ַ(0x000000~0x0FFFFF)
* ReturnValue    : None
********************************************************************************************************/
void W25Q_WriteAddr(u32 addr)
{
	W25Q_WriteByte(((addr & 0xFFFFFF) >> 16));                        // ��
	W25Q_WriteByte(((addr & 0xFFFF) >> 8));                           // ��
	W25Q_WriteByte(addr & 0xFF);                                      // ��
}

/********************************************************************************************************
* FunctionName   : W25QInit()
* Description    : ��ʼ��W25Q
* EntryParameter : None
* ReturnValue    : ���ض�ȡ״ֵ̬
********************************************************************************************************/
void W25QInit(void)
{
	W25Q_SetGpio();						                             // �˿�����
	W25Q_Enable(1);						                             // ��Ƭѡ

#if !W25Q_READ_ONLY
    W25Q_EnableWrite();				                                 // дʹ��
	W25Q_EnableWriteReg();			                                 // ʹ�ܸ�д״̬�Ĵ�������
	W25Q_WriteStatusReg(0x02);		                                 // ȡ�����п鱣��
#endif
}

/********************************************************************************************************
* FunctionName   : W25Q_ReadStatusReg()
* Description    : ��״̬�Ĵ�����ֵ
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
u8 W25Q_ReadStatusReg(void)
{
	u8 dat = 0;
	
	W25Q_Enable(0);			                                          // ʹ��Ƭѡ
	W25Q_WriteByte(W25Q_RDSR);                                        // 0x05 send RDSR command ���Ͷ�״̬�Ĵ�����������
	dat = W25Q_ReadByte();                                            // �Ӷ˿ڶ�����״̬�Ĵ���������     
	W25Q_Enable(1);                                                   // ��ֹʹ��

	return dat;                                                       // ���ض���ֵ
}
#endif

/********************************************************************************************************
* FunctionName   : W25Q_EnableWriteReg()
* Description    : ʹ��д״̬�Ĵ���
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25Q_EnableWriteReg(void)
{
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_EWSR);                                          // 0x50 enable writing to the status register
	W25Q_Enable(1);
}
#endif

/********************************************************************************************************
* FunctionName   : W25Q_WriteStatusReg()
* Description    : д״̬�Ĵ���
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25Q_WriteStatusReg(u8 dat)
{
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_WRSR);		                                     // 0x01 select write to status register
	W25Q_WriteByte(dat);				                                 // data that will change the status of BPx or BPL (only bits 2,3,7 can be written)
	W25Q_Enable(1);
}
#endif

/********************************************************************************************************
* FunctionName   : W25Q_EnableWrite()
* Description    : дʹ�ܼĴ���
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25Q_EnableWrite(void)
{
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_WREN);                                          // 0x06 send WREN command
	W25Q_Enable(1);
}
#endif

/********************************************************************************************************
* FunctionName   : W25Q_DisableWrite()
* Description    : д��ֹ�Ĵ���
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25Q_DisableWrite(void)
{
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_WRDI);                                          // 0x04 send WRDI command
	W25Q_Enable(1);
}
#endif

/********************************************************************************************************
* FunctionName   : W25Q_WaitBusy()
* Description    : �ж�æ״̬,��Byte-Program, Sector-Erase, Block-Erase, Chip-Eraseʹ��
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25Q_WaitBusy(void)
{
	while ((W25Q_ReadStatusReg() & 0x01) == 0x01)
	{
	    ;															   // �ȴ�æ����
	}
}
#endif

/********************************************************************************************************
* FunctionName   : W25QWriteByte()
* Description    : д��һ�ֽ�����
* EntryParameter : addr - ��ַ(0x000000~0x0FFFFF); dat - ����
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QWriteByte(u32 addr, u8 dat)
{
	W25Q_EnableWrite();                                               // дʹ��
	
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_BYTE_PROGRAM);                                // 0x02 send Byte Program command
	W25Q_WriteAddr(addr);											  // д��ַ 
	W25Q_WriteByte(dat);                                              // send byte to be programmed
	W25Q_Enable(1);
	
	W25Q_WaitBusy();												  // �ȴ�æ���� 
}
#endif

/********************************************************************************************************
* FunctionName   : W25QWriteNByte()
* Description    : д�����ֽ�
* EntryParameter : addr - ��ַ(0x000000~0x0FFFFF)��len - ���ֽ�����buf�����ݴ洢����
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QWriteNByte(u32 addr, u8 *buf, u32 len)
{
	u32 i;
	
	if (len == 0) return;											   // û������д��
	
	for (i=0; i<len; i++)
	{
	    W25QWriteByte(addr+i, buf[i]);								   // д������
	}

	W25Q_DisableWrite();											   // ��ֹд
}
#endif

/********************************************************************************************************
* FunctionName   : W25QWriteNByteSecEra()
* Description    : д�����ֽ�(��������������)
* EntryParameter : addr - ��ַ(0x000000~0x0FFFFF)��buf�����ݴ洢���壻len - ���ֽ���
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QWriteNByteSecEra(u32 addr, u8 *buf, u32 len)
{
    if ((addr % 4096) == 0)
	{
	    W25QSectorErase(addr/4096);									 // ����һ������
	}

	W25QWriteNByte(addr, buf, len);
}
#endif

/********************************************************************************************************
* FunctionName   : W25QChipErase()
* Description    : оƬ��Ƭ����(0x000000~0x0FFFFF)
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QChipErase(void)
{
	W25Q_EnableWrite();                                               // дʹ��
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_CHIP_ERASE);                                  // 0x60 send Block Erase command
	W25Q_Enable(1);
	W25Q_WaitBusy();												  // �ȴ�æ����
}
#endif

/********************************************************************************************************
* FunctionName   : W25QSectorErase()
* Description    : ��������(1������4Kbyte)
* EntryParameter : sectorAddr - ���ַ(8Mbit��0x000~0x800)
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QSectorErase(u32 sectorAddr)
{
	/* 	�����ʵ�ʵ�ַ���Ͳ���Ҫ����ת����ֱ��д���ַ���ɣ�ֻ��Ҫע����Ч��ַ��Χ��Ams~A12Ϊ���ַ,
    A11~A0����Ϊ���⡣�磺������0���ǲ���0x0000~0x0FFF, ������1���ǲ���0x01000~0x01FFF...	*/

    u32 addr = sectorAddr << 12;									 // ��������ַת����ʵ�ʵ�ַ

	W25Q_EnableWrite();                                              // дʹ��
	
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_SECTOR_ERASE);                               // 0x20 send Block Erase command
	W25Q_WriteAddr(addr);											 // д��ַ
	W25Q_Enable(1);
	W25Q_WaitBusy();											     // �ȴ�æ����
}
#endif

/********************************************************************************************************
* FunctionName   : W25QBlockErase()
* Description    : �����(1����32Kbyte)
* EntryParameter : blockAddr - ��ַ(8Mbit��0x00~0xFF) �����ʵ�ʵ�ַ��
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QBlockErase(u32 blockAddr)
{
	/* 
	    �����ʵ�ʵ�ַ���Ͳ���Ҫ����ת����ֱ��д���ַ���ɣ�ֻ��Ҫ��Ҫ��Ч��ַ��Χ��Ams~A15Ϊ���ַ,
    A14~A0����Ϊ���⡣�磺������0���ǲ���0x000000~0x00FFFF, ������1���ǲ���0x010000~0x01FFFF...
	*/
	u32 addr = blockAddr << 15;									     // �ѿ��ַת����ʵ�ʵ�ַ

	W25Q_EnableWrite();                                              // дʹ��
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_BLOCK_ERASE);                                // 0x52 send Block Erase command
	W25Q_WriteAddr(addr);										     // д��ַ             
	W25Q_Enable(1);
	W25Q_WaitBusy();												 // �ȴ�æ����
}
#endif

/********************************************************************************************************
* FunctionName   : W25QReadDeviceID()
* Description    : ������/����ID ID1(Manufacture's ID = BFh,Device ID = 80h(W25Q25LF080))
* EntryParameter : idAddr��00H-����, 01H-����ID 
* ReturnValue    : ���ص�ַ
********************************************************************************************************/
u8 W25QReadDeviceID(u8 idAddr)
{
	u8 dat;

	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_READ_ID);                                    // 0x90 send read ID command (90h or ABh)
	W25Q_WriteByte(0x00);                                            // send address
	W25Q_WriteByte(0x00);                                            // send address

	/* ID_addr(A7~A1=0) Manufacturer��s ID is read with A0=0, Device ID is read with A0=1. */
	W25Q_WriteByte(idAddr);                                          // send address - either 00H or 01H
	dat = W25Q_ReadByte();                                           // receive byte
	W25Q_Enable(1);

	return dat;														 // ���ض�ȡ����
}

/********************************************************************************************************
* FunctionName   : W25QReadByte()
* Description    : ��ָ����ַ��ȡһ���ֽ�����(20MHz)
* EntryParameter : addr - ָ����ַ(0x000000~0x0FFFFF)
* ReturnValue    : ���ض�ȡ����
********************************************************************************************************/
u8 W25QReadByte(u32 addr)
{
	u8 dat; 

	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_READ);                                       // 0x03 read command 
	W25Q_WriteAddr(addr);											 // д��ַ
	dat = W25Q_ReadByte();
	W25Q_Enable(1);

	return dat;                                                      // return one byte read
}

/********************************************************************************************************
* FunctionName   : W25QReadNByte()
* Description    : ���ֽڶ�ȡ
* EntryParameter : addr - ��ַ(0x000000~0x0FFFFF)��len - ��ȡ�ֽ�����buf - ���ݴ洢����
* ReturnValue    : ���ز������
********************************************************************************************************/
void W25QReadNByte(u32 addr, u8 *buf, u32 len)
{
	u32 i;
	
	W25Q_Enable(0);													 // Ƭѡ
	W25Q_WriteByte(W25Q_READ);                                       // 0x03 read command
	W25Q_WriteAddr(addr);											 // д��ַ 

	for (i=0; i<len; i++)                                            // read until no_bytes is reached
	{
	    buf[i] = W25Q_ReadByte();                                    // receive byte and store at address 80H - FFH
	}
	
	W25Q_Enable(1);													 // ��Ƭѡ
}

/********************************************************************************************************
* FunctionName   : W25QHighSpeedReadByte()
* Description    : ���ٶ�1byte����(33MHz)
* EntryParameter : addr - ��ַ(0x000000~0x0FFFFF)
* ReturnValue    : ���ض�ȡ����
********************************************************************************************************/
u8 W25QHighSpeedReadByte(u32 addr)
{
	u8 dat = 0;
	
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_HIGH_SPEED_READ);                            // 0x0B read command
	W25Q_WriteAddr(addr);											 // д��ַ
	W25Q_ReadByte();                                                 // dummy byte,����������������
	dat = W25Q_ReadByte();
	W25Q_Enable(1);
	
	return dat;
}																	 // ���ض�ȡ����

/********************************************************************************************************
* FunctionName   : W25QHighSpeedReadNByte()
* Description    : ��������������
* EntryParameter : addr - ��ַ(0x000000~0x0FFFFF)��len - ��ȡ�ֽ�����buf - ���ݴ洢����
* ReturnValue    : None
********************************************************************************************************/
void W25QHighSpeedReadNByte(u32 addr, u8 *buf, u32 len)
{
    u32 i;
  
	W25Q_Enable(0);													 // Ƭѡ
	
	W25Q_WriteByte(W25Q_HIGH_SPEED_READ);                            // 0x0B read command
	W25Q_WriteAddr(addr);											 // д��ַ

	W25Q_ReadByte();                                                 // dummy byte
	for (i=0; i<len; i++)                                            // read until no_bytes is reached
	{
	    buf[i] = W25Q_ReadByte();                                    // receive byte and store at address 80H - FFH
	}
	
	W25Q_Enable(1);													 // ��Ƭѡ
}

/********************************************************************************************************
* FunctionName   : W25QPowerDown()
* Description    : �������ģʽ
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void W25QPowerDown(void)   
{ 
  	W25Q_Enable(0);                            		                 // ʹ������   
    W25Q_WriteByte(W25Q_POW_DOWN);    		                         // ���͵�������  
	W25Q_Enable(1);                            		                 // ȡ��Ƭѡ     	      
}

/********************************************************************************************************
* FunctionName   : W25QWakeUp()
* Description    : �˳�����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/   
void W25QWakeUp(void)   
{  
  	W25Q_Enable(0);                            		                  // ʹ������   
    W25Q_WriteByte(W25Q_RLS_POW_DOWN);                                // send W25Q_PowerDown command 0xAB    
	W25Q_Enable(1);                            		                  // ȡ��Ƭѡ     	      
}

/********************************************************************************************************
* FunctionName   : W25QWriteNByteEra()
* Description    : д�����ֽ�(��������)
* EntryParameter : addr - ��ַ(0x000000~0x0FFFFF)��len - ���ֽ�����buf�����ݴ洢����
* ReturnValue    : None
********************************************************************************************************/
void W25QWriteNByteEra(u32 addr, u8 *buf, u32 len)
{
	u32 i=0;
	u32 addr0, addr1;
	u8  tmpBuf[W25Q_BUF_SIZE] = {0};

	W25QReadNByte(addr, tmpBuf, len);								                  // �� len �ֽ�����,�ж��Ƿ�Ϊ0xFF
	for (i=0; i<len; i++)
	{
		if (tmpBuf[i] != 0xFF)
		{
			addr0 = addr / W25Q_BUF_SIZE;									          // ��ʼ������ַ
	        addr1 = (addr + len) / W25Q_BUF_SIZE;							          // д�����ݺ������
			W25QReadNByte(addr0*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);	          // ����ʼ������
			W25QSectorErase(addr0);									                  // ����ʼ��
		    
			if(addr0 < addr1)										                  // �п��
			{
				memcpy(&tmpBuf[addr%W25Q_BUF_SIZE], buf, (addr1*W25Q_BUF_SIZE - addr));
				W25QWriteNByte(addr0*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);       // д��ʼ������

				W25QReadNByte(addr1*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);        // ���������
				W25QSectorErase(addr1);								                  // �����
				
				memcpy(&tmpBuf[0], &buf[addr1*W25Q_BUF_SIZE - addr], (len + addr - addr1*W25Q_BUF_SIZE));
				W25QWriteNByte(addr1*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);       // д�������
			}
			else													                  // �޿��
			{
				memcpy(&tmpBuf[addr%W25Q_BUF_SIZE], buf, len);
				W25QWriteNByte(addr0*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);       // д��ʼ������
			}
			
			W25Q_DisableWrite();									                  // ��ֹд
			return;
		}
	}

	for (i=0; i<len; i++)
	{
	    W25QWriteByte(addr+i, buf[i]);								                  // ֱ��д������
	}

	W25Q_DisableWrite();											                  // ��ֹд
}

/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
