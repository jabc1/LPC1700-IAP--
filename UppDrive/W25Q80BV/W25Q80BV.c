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
* Description    : 管脚配置
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void W25Q_SetGpio(void)
{
	W25Q_CS_OUT();			                                     // 设置片选为输出端口
}

/********************************************************************************************************
* FunctionName   : W25Q_Enable()
* Description    : W25Q片选
* EntryParameter : en - 0使能；1禁止
* ReturnValue    : None
********************************************************************************************************/
void W25Q_Enable(u8 en)
{
    W25Q_CS_WRITE(en);    
}

/********************************************************************************************************
* FunctionName   : W25Q_WriteAddr()
* Description    : W25Q写地址
* EntryParameter : addr - 24位地址(0x000000~0x0FFFFF)
* ReturnValue    : None
********************************************************************************************************/
void W25Q_WriteAddr(u32 addr)
{
	W25Q_WriteByte(((addr & 0xFFFFFF) >> 16));                        // 高
	W25Q_WriteByte(((addr & 0xFFFF) >> 8));                           // 中
	W25Q_WriteByte(addr & 0xFF);                                      // 低
}

/********************************************************************************************************
* FunctionName   : W25QInit()
* Description    : 初始化W25Q
* EntryParameter : None
* ReturnValue    : 返回读取状态值
********************************************************************************************************/
void W25QInit(void)
{
	W25Q_SetGpio();						                             // 端口配置
	W25Q_Enable(1);						                             // 关片选

#if !W25Q_READ_ONLY
    W25Q_EnableWrite();				                                 // 写使能
	W25Q_EnableWriteReg();			                                 // 使能改写状态寄存器操作
	W25Q_WriteStatusReg(0x02);		                                 // 取消所有块保护
#endif
}

/********************************************************************************************************
* FunctionName   : W25Q_ReadStatusReg()
* Description    : 读状态寄存器的值
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
u8 W25Q_ReadStatusReg(void)
{
	u8 dat = 0;
	
	W25Q_Enable(0);			                                          // 使能片选
	W25Q_WriteByte(W25Q_RDSR);                                        // 0x05 send RDSR command 发送读状态寄存器命脉令字
	dat = W25Q_ReadByte();                                            // 从端口读出的状态寄存器的内容     
	W25Q_Enable(1);                                                   // 禁止使能

	return dat;                                                       // 返回读出值
}
#endif

/********************************************************************************************************
* FunctionName   : W25Q_EnableWriteReg()
* Description    : 使能写状态寄存器
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
* Description    : 写状态寄存器
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
* Description    : 写使能寄存器
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
* Description    : 写禁止寄存器
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
* Description    : 判断忙状态,被Byte-Program, Sector-Erase, Block-Erase, Chip-Erase使用
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25Q_WaitBusy(void)
{
	while ((W25Q_ReadStatusReg() & 0x01) == 0x01)
	{
	    ;															   // 等待忙结束
	}
}
#endif

/********************************************************************************************************
* FunctionName   : W25QWriteByte()
* Description    : 写入一字节数据
* EntryParameter : addr - 地址(0x000000~0x0FFFFF); dat - 数据
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QWriteByte(u32 addr, u8 dat)
{
	W25Q_EnableWrite();                                               // 写使能
	
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_BYTE_PROGRAM);                                // 0x02 send Byte Program command
	W25Q_WriteAddr(addr);											  // 写地址 
	W25Q_WriteByte(dat);                                              // send byte to be programmed
	W25Q_Enable(1);
	
	W25Q_WaitBusy();												  // 等待忙结束 
}
#endif

/********************************************************************************************************
* FunctionName   : W25QWriteNByte()
* Description    : 写入多个字节
* EntryParameter : addr - 地址(0x000000~0x0FFFFF)；len - 入字节数；buf：数据存储缓冲
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QWriteNByte(u32 addr, u8 *buf, u32 len)
{
	u32 i;
	
	if (len == 0) return;											   // 没有数据写入
	
	for (i=0; i<len; i++)
	{
	    W25QWriteByte(addr+i, buf[i]);								   // 写入数据
	}

	W25Q_DisableWrite();											   // 禁止写
}
#endif

/********************************************************************************************************
* FunctionName   : W25QWriteNByteSecEra()
* Description    : 写入多个字节(带扇区擦除功能)
* EntryParameter : addr - 地址(0x000000~0x0FFFFF)；buf：数据存储缓冲；len - 入字节数
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QWriteNByteSecEra(u32 addr, u8 *buf, u32 len)
{
    if ((addr % 4096) == 0)
	{
	    W25QSectorErase(addr/4096);									 // 擦除一个扇区
	}

	W25QWriteNByte(addr, buf, len);
}
#endif

/********************************************************************************************************
* FunctionName   : W25QChipErase()
* Description    : 芯片整片擦除(0x000000~0x0FFFFF)
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QChipErase(void)
{
	W25Q_EnableWrite();                                               // 写使能
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_CHIP_ERASE);                                  // 0x60 send Block Erase command
	W25Q_Enable(1);
	W25Q_WaitBusy();												  // 等待忙结束
}
#endif

/********************************************************************************************************
* FunctionName   : W25QSectorErase()
* Description    : 扇区擦除(1个扇区4Kbyte)
* EntryParameter : sectorAddr - 块地址(8Mbit：0x000~0x800)
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QSectorErase(u32 sectorAddr)
{
	/* 	如果用实际地址，就不需要下面转换，直接写入地址即可，只是要注意有效地址范围是Ams~A12为块地址,
    A11~A0可以为任意。如：擦除块0就是擦除0x0000~0x0FFF, 擦除块1就是擦除0x01000~0x01FFF...	*/

    u32 addr = sectorAddr << 12;									 // 把扇区地址转换成实际地址

	W25Q_EnableWrite();                                              // 写使能
	
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_SECTOR_ERASE);                               // 0x20 send Block Erase command
	W25Q_WriteAddr(addr);											 // 写地址
	W25Q_Enable(1);
	W25Q_WaitBusy();											     // 等待忙结束
}
#endif

/********************************************************************************************************
* FunctionName   : W25QBlockErase()
* Description    : 块擦除(1个块32Kbyte)
* EntryParameter : blockAddr - 地址(8Mbit：0x00~0xFF) 如果用实际地址：
* ReturnValue    : None
********************************************************************************************************/
#if !W25Q_READ_ONLY
void W25QBlockErase(u32 blockAddr)
{
	/* 
	    如果用实际地址，就不需要下面转换，直接写入地址即可，只是要主要有效地址范围是Ams~A15为块地址,
    A14~A0可以为任意。如：擦除块0就是擦除0x000000~0x00FFFF, 擦除块1就是擦除0x010000~0x01FFFF...
	*/
	u32 addr = blockAddr << 15;									     // 把块地址转换成实际地址

	W25Q_EnableWrite();                                              // 写使能
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_BLOCK_ERASE);                                // 0x52 send Block Erase command
	W25Q_WriteAddr(addr);										     // 写地址             
	W25Q_Enable(1);
	W25Q_WaitBusy();												 // 等待忙结束
}
#endif

/********************************************************************************************************
* FunctionName   : W25QReadDeviceID()
* Description    : 读厂商/器件ID ID1(Manufacture's ID = BFh,Device ID = 80h(W25Q25LF080))
* EntryParameter : idAddr：00H-厂商, 01H-器件ID 
* ReturnValue    : 返回地址
********************************************************************************************************/
u8 W25QReadDeviceID(u8 idAddr)
{
	u8 dat;

	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_READ_ID);                                    // 0x90 send read ID command (90h or ABh)
	W25Q_WriteByte(0x00);                                            // send address
	W25Q_WriteByte(0x00);                                            // send address

	/* ID_addr(A7~A1=0) Manufacturer’s ID is read with A0=0, Device ID is read with A0=1. */
	W25Q_WriteByte(idAddr);                                          // send address - either 00H or 01H
	dat = W25Q_ReadByte();                                           // receive byte
	W25Q_Enable(1);

	return dat;														 // 返回读取数据
}

/********************************************************************************************************
* FunctionName   : W25QReadByte()
* Description    : 在指定地址读取一个字节数据(20MHz)
* EntryParameter : addr - 指定地址(0x000000~0x0FFFFF)
* ReturnValue    : 返回读取数据
********************************************************************************************************/
u8 W25QReadByte(u32 addr)
{
	u8 dat; 

	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_READ);                                       // 0x03 read command 
	W25Q_WriteAddr(addr);											 // 写地址
	dat = W25Q_ReadByte();
	W25Q_Enable(1);

	return dat;                                                      // return one byte read
}

/********************************************************************************************************
* FunctionName   : W25QReadNByte()
* Description    : 多字节读取
* EntryParameter : addr - 地址(0x000000~0x0FFFFF)；len - 读取字节数；buf - 数据存储缓冲
* ReturnValue    : 返回操作结果
********************************************************************************************************/
void W25QReadNByte(u32 addr, u8 *buf, u32 len)
{
	u32 i;
	
	W25Q_Enable(0);													 // 片选
	W25Q_WriteByte(W25Q_READ);                                       // 0x03 read command
	W25Q_WriteAddr(addr);											 // 写地址 

	for (i=0; i<len; i++)                                            // read until no_bytes is reached
	{
	    buf[i] = W25Q_ReadByte();                                    // receive byte and store at address 80H - FFH
	}
	
	W25Q_Enable(1);													 // 关片选
}

/********************************************************************************************************
* FunctionName   : W25QHighSpeedReadByte()
* Description    : 高速读1byte数据(33MHz)
* EntryParameter : addr - 地址(0x000000~0x0FFFFF)
* ReturnValue    : 返回读取数据
********************************************************************************************************/
u8 W25QHighSpeedReadByte(u32 addr)
{
	u8 dat = 0;
	
	W25Q_Enable(0);
	W25Q_WriteByte(W25Q_HIGH_SPEED_READ);                            // 0x0B read command
	W25Q_WriteAddr(addr);											 // 写地址
	W25Q_ReadByte();                                                 // dummy byte,发送任意数都可以
	dat = W25Q_ReadByte();
	W25Q_Enable(1);
	
	return dat;
}																	 // 返回读取数据

/********************************************************************************************************
* FunctionName   : W25QHighSpeedReadNByte()
* Description    : 高速连续读数据
* EntryParameter : addr - 地址(0x000000~0x0FFFFF)；len - 读取字节数；buf - 数据存储缓冲
* ReturnValue    : None
********************************************************************************************************/
void W25QHighSpeedReadNByte(u32 addr, u8 *buf, u32 len)
{
    u32 i;
  
	W25Q_Enable(0);													 // 片选
	
	W25Q_WriteByte(W25Q_HIGH_SPEED_READ);                            // 0x0B read command
	W25Q_WriteAddr(addr);											 // 写地址

	W25Q_ReadByte();                                                 // dummy byte
	for (i=0; i<len; i++)                                            // read until no_bytes is reached
	{
	    buf[i] = W25Q_ReadByte();                                    // receive byte and store at address 80H - FFH
	}
	
	W25Q_Enable(1);													 // 关片选
}

/********************************************************************************************************
* FunctionName   : W25QPowerDown()
* Description    : 进入掉电模式
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void W25QPowerDown(void)   
{ 
  	W25Q_Enable(0);                            		                 // 使能器件   
    W25Q_WriteByte(W25Q_POW_DOWN);    		                         // 发送掉电命令  
	W25Q_Enable(1);                            		                 // 取消片选     	      
}

/********************************************************************************************************
* FunctionName   : W25QWakeUp()
* Description    : 退出掉电
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/   
void W25QWakeUp(void)   
{  
  	W25Q_Enable(0);                            		                  // 使能器件   
    W25Q_WriteByte(W25Q_RLS_POW_DOWN);                                // send W25Q_PowerDown command 0xAB    
	W25Q_Enable(1);                            		                  // 取消片选     	      
}

/********************************************************************************************************
* FunctionName   : W25QWriteNByteEra()
* Description    : 写入多个字节(擦除功能)
* EntryParameter : addr - 地址(0x000000~0x0FFFFF)；len - 入字节数；buf：数据存储缓冲
* ReturnValue    : None
********************************************************************************************************/
void W25QWriteNByteEra(u32 addr, u8 *buf, u32 len)
{
	u32 i=0;
	u32 addr0, addr1;
	u8  tmpBuf[W25Q_BUF_SIZE] = {0};

	W25QReadNByte(addr, tmpBuf, len);								                  // 读 len 字节数据,判断是否为0xFF
	for (i=0; i<len; i++)
	{
		if (tmpBuf[i] != 0xFF)
		{
			addr0 = addr / W25Q_BUF_SIZE;									          // 起始扇区地址
	        addr1 = (addr + len) / W25Q_BUF_SIZE;							          // 写入数据后的扇区
			W25QReadNByte(addr0*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);	          // 读起始块数据
			W25QSectorErase(addr0);									                  // 擦起始块
		    
			if(addr0 < addr1)										                  // 有跨块
			{
				memcpy(&tmpBuf[addr%W25Q_BUF_SIZE], buf, (addr1*W25Q_BUF_SIZE - addr));
				W25QWriteNByte(addr0*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);       // 写起始块数据

				W25QReadNByte(addr1*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);        // 读跨块数据
				W25QSectorErase(addr1);								                  // 擦跨块
				
				memcpy(&tmpBuf[0], &buf[addr1*W25Q_BUF_SIZE - addr], (len + addr - addr1*W25Q_BUF_SIZE));
				W25QWriteNByte(addr1*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);       // 写跨块数据
			}
			else													                  // 无跨块
			{
				memcpy(&tmpBuf[addr%W25Q_BUF_SIZE], buf, len);
				W25QWriteNByte(addr0*W25Q_BUF_SIZE, &tmpBuf[0], W25Q_BUF_SIZE);       // 写起始块数据
			}
			
			W25Q_DisableWrite();									                  // 禁止写
			return;
		}
	}

	for (i=0; i<len; i++)
	{
	    W25QWriteByte(addr+i, buf[i]);								                  // 直接写入数据
	}

	W25Q_DisableWrite();											                  // 禁止写
}

/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
