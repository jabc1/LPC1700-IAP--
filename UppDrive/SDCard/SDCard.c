/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : sdCard.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : SPI方式下SD驱动代码
*				   支持SD卡SD v1.1、SD v2.0和SDHC(可以兼容多种卡)
*				   注意：1G的SD卡一般没有MBR，2G及以上的SD一般有MBR
********************************************************************************************************/

/********************************************************************************************************
* header file                                                 
********************************************************************************************************/
#include "../UserCode/source/config.h" 

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
SD_Info SDCard;						                                        // SD卡信息
SD_CID  SDCid;

/********************************************************************************************************
* FunctionName   : SD_SetGpio()
* Description    : SD管脚定义
* EntryParameter : None
* ReturnValue    : 返回状态
********************************************************************************************************/
void SD_SetGpio(void)
{
	SD_CS_OUT();			                                                // 设置片选为输出端口
    SD_STU_IN();			                                                // 设置卡检测为输入端口
}

/********************************************************************************************************
* FunctionName   : SD_DelayUs()
* Description    : 延时
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void SD_DelayUs(u32 us)
{
    u16 i;

	while (us--)
	{
	    for (i=14; i>0; i--)	 
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : SDGetCardStatus()
* Description    : 获取SD卡插卡状态状态
* EntryParameter : None
* ReturnValue    : 返回状态
********************************************************************************************************/
u8 SDGetCardStatus(void)
{
    u8 rValue = 0x00;
	
	if (SD_STU_READ() == 0)	                                             // 插卡检测
	{
	    if (0 == 0)                                                      // 卡状态检测
		{
		    rValue = 0x00;                                               // SD卡OK
		}
		else
		{
			rValue = 0x04;                                               // SD卡保护
		}
	}
	else
	{
		rValue = 0x02;                                                   // 没有SD卡
	}	

    return rValue;
}

/********************************************************************************************************
* FunctionName   : SD_PowerSwitch()
* Description    : 电源开关
* EntryParameter : swh - 电源开关（0 - 打开电源；1 - 关闭电源）
* ReturnValue    : None
********************************************************************************************************/
void SD_PowerSwitch(u8 swh)
{
    if (swh)
	{
	    ;  // 关闭电源
	}
	else
	{
	    ;  // 打开电源
	}        
}

/********************************************************************************************************
* FunctionName   : SD_SendCmd()
* Description    : SD命令发送
* EntryParameter : cmd - 命令,arg - 参数,crc - 校应和
* ReturnValue    : 返回指令响应
********************************************************************************************************/
u8 SD_SendCmd(u8 cmd,u32 arg,u8 crc)
{
	u16 count = SD_OPERATE_NUM;										     // 超时值
	u8 rvalue = SD_RESPONSE_FAILURE;				                     // 返回值

	SD_CS_WRITE(1);
	SD_WriteByte(0xFF);								                     // 8个时钟脉冲的延迟
	SD_WriteByte(0xFF);
	SD_WriteByte(0xFF);
	SD_CS_WRITE(0);									                     // SD卡使能

	// 发送6字节的指令数据
    SD_WriteByte(cmd);			                                         // 字节 1
	SD_WriteByte(arg >> 24);											 // 字节 2
	SD_WriteByte(arg >> 16);											 // 字节 3
	SD_WriteByte(arg >> 8);											     // 字节 4
	SD_WriteByte(arg);													 // 字节 5
	SD_WriteByte(crc);													 // 字节 6
	
	SD_WriteByte(0xFF);

	do
	{
	    rvalue = SD_ReadByte();                                          // 等待回复
		count--;
		SD_DelayUs(500);												 // 延时是为了增加对卡的兼容性(不加对有些卡不兼容)
        if ((count % 200) == 0)
        {
            if (SDGetCardStatus() == SD_CARD_NO)                         // 没有SD卡退出
            {
                break;
            }
        }
	}while ((rvalue == SD_RESPONSE_FAILURE) && count);
	
    return rvalue;
}

/********************************************************************************************************
* FunctionName   : SD_GetCSD()
* Description    : CSD-寄存器。寄存器长度为128，16字节
* EntryParameter : csd 寄存器
* ReturnValue    : 返回操作状态：成功-0
********************************************************************************************************/
u8 SD_GetCSD(SD_CSD *csd)
{
    u8 i;
	u8 csdTable[16];
	u16 count = SD_OPERATE_NUM;
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // 返回值

	SD_CS_WRITE(0);									                    // SD卡使能
	
	if (SD_SendCmd(CMD9, 0x00, 0xFF) == SD_RESPONSE_NO_ERROR)
	{
	    while ((SD_ReadByte() != SD_START_SSREAD) && count)             // 等待数据接收开始，收到0xFE表示开始
		{
		    count--;		                                 		    // 等待超时
		}

		if (count != 0x00)
		{
			for (i=0; i<16; i++)
			{
			    csdTable[i] = SD_ReadByte();							// 读取16字节寄存器数据
			}
		}

		SD_ReadByte();													// 读CRC
		SD_ReadByte();

		// 把获取值放入CSD结构体中												  
		csd->CSDStruct = (csdTable[0] & 0xC0) >> 6;				   		// 获取CSD版本信息
	
		if (csd->CSDStruct == 0)												   
		{
			csd->RdSectorSize = csdTable[5] & 0x0F;					    // READ_BL_LEN
																	     
			csd->DeviceSize  = (csdTable[6] & 0x03) << 10;			    // C_SIZE
			csd->DeviceSize |= (csdTable[7]) << 2;					   
			csd->DeviceSize |= (csdTable[8] & 0xC0) >> 6;			  
			 
			csd->DeviceSizeMul  = (csdTable[9] & 0x03) << 1;		    // C_SIZE_MULT
			csd->DeviceSizeMul |= (csdTable[10] & 0x80) >> 7;	
		}
		else
		{
			csd->DeviceSize  = (csdTable[8]<<8) + csdTable[9] + 1;	    // C_SIZE
	    }

		rvalue = SD_RESPONSE_NO_ERROR;				                    // 设置成功标志
	}

	SD_CS_WRITE(1);									                    // 清除SD卡片选									 
    SD_WriteByte(0xFF);								                    // 8个时钟脉冲的延迟
	
	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SD_GetCID()
* Description    : 获取SD卡CID
* EntryParameter : id - SD卡信息
* ReturnValue    : None
********************************************************************************************************/
u8 SD_GetCID(SD_CID *cid)
{
    u8 i;
    u8 cidTable[16] = {0};
    u8 count = 0xFF;
    u8 rvalue = SD_RESPONSE_FAILURE;                                    // 返回值
    
    SD_CS_WRITE(0);                                                     // SD卡使能
    
    if (SD_SendCmd(CMD10, 0x00, 0xFF) == SD_RESPONSE_NO_ERROR)
    {
        while ((SD_ReadByte() != 0xFE) && count)                        // 等待数据接收开始，收到0xFE表示开始
        {
            count--;                                                    // 等待超时
        }
    
        if (count != 0x00)
        {
            for (i=0; i<16; i++)
            {
                cidTable[i] = SD_ReadByte();
            }
        }
                      
        SD_ReadByte();                                                  // 读CRC
        SD_ReadByte();
        
        rvalue = SD_RESPONSE_NO_ERROR;                                  // 设置成功标志
    }
    
    SD_CS_WRITE(1);                                                     // 清除SD卡片选          
    SD_WriteByte(0xFF);                                                 // 8个时钟脉冲的延迟
    
    if (rvalue == SD_RESPONSE_NO_ERROR)
    {
        // 把获取值放入CID结构体中
        cid->MID    = cidTable[ 0];                                     // Byte 0
        cid->OID[0] = cidTable[ 1];                                     // Byte 1
        cid->OID[1] = cidTable[ 2];                                     // Byte 2
        cid->PNM[0] = cidTable[ 3];                                     // Byte 3
        cid->PNM[1] = cidTable[ 4];                                     // Byte 4
        cid->PNM[2] = cidTable[ 5];                                     // Byte 5
        cid->PNM[3] = cidTable[ 6];                                     // Byte 6
        cid->PNM[4] = cidTable[ 7];                                     // Byte 7
        cid->PRV    = cidTable[ 8];                                     // Byte 8
        cid->PSN[0] = cidTable[ 9];                                     // Byte 9
        cid->PSN[1] = cidTable[10];                                     // Byte 10
        cid->PSN[2] = cidTable[11];                                     // Byte 11
        cid->PSN[3] = cidTable[12];                                     // Byte 12
        cid->MDT[0] = cidTable[13] & 0x0F;                              // Byte 13
        cid->MDT[1] = cidTable[14];                                     // Byte 14
        cid->Crc    = (cidTable[15] & 0xFE) >> 1;                       // Byte 15
    }
    
    return rvalue;
}

/********************************************************************************************************
* FunctionName   : SDGetCardInfo()
* Description    : 获取SD卡信息
* EntryParameter : info - SD卡信息
* ReturnValue    : None
********************************************************************************************************/
void SDGetCardInfo(SD_Info *info)
{
    SD_CSD csd;
	
	SD_GetCSD(&csd);											        // 获取CSD卡信息
	
	if (csd.CSDStruct == 0)												   
	{
//	    info->Type = 0x10;									            // Version 1.01-1.10  Version 2.00/ Standard Capacity		   														  
		info->SectorSize = 512;					                        // SD卡扇区大小

		/* BLOCKNR = (C_SIZE+1) * MULT;
		   MULT = 2^(C_SIZE_MULT+2);
		   BLOCK_LEN = 2^READ_BL_LEN; */		   

		info->Capacity  = csd.DeviceSize + 1;					        // SD卡容量，单位为字节
	    info->Capacity *= (1 << (csd.DeviceSizeMul + 2));		        // 2的DeviceSizeMul+2次方
	    info->Capacity *= (1 << csd.RdSectorSize);					    // 2的RdSectorSize次方

		info->Sectors = info->Capacity / info->SectorSize;              // SD卡扇区数
	    info->Capacity >>= 20;								            // 转换成MByte  
	}
	else
	{
//		info->Type = 0x20;									            // Version 2.00/ High Capacity
		info->SectorSize = 512;					                        // SD卡扇区大小
		
		/* memory capacity = (C_SIZE+1) * 512K byte */
		//info->Capacity = (u64)(csd.DeviceSize) * 512*1024;			// SD卡容量：C_Size*512KByte
		info->Sectors  = csd.DeviceSize * 1024;                         // SD卡扇区数
	    info->Capacity = csd.DeviceSize >> 1;							// 转换成MByte
	} 
}

/********************************************************************************************************
* FunctionName   : SD_IdleSta()
* Description    : SD卡进入空闲模式
* EntryParameter : None
* ReturnValue    : 返回操作状态：成功-0
********************************************************************************************************/
u8 SD_IdleSta(void)
{
	u16 i;
	u8 retry = 200;
    
	// 先产生>74个脉冲，让SD卡自己初始化完成
    for (i=0; i<0x10; i++)
	{
	    SD_WriteByte(0xFF);
	} 

    //-----------------SD卡复位到idle开始-----------------
    // 循环连续发送CMD0，直到SD卡返回0x01,进入IDLE状态
    // 超时则直接退出

    do
    {	   
        // 发送CMD0，让SD卡进入IDLE状态
        i = SD_SendCmd(CMD0, 0, 0x95);
        retry++;
    } while ((i != 0x01) && (retry > 0));

    // 跳出循环后，检查原因：初始化成功？or 重试超时？
    return ((retry > 0) ? 0 : 1);						  
}

/********************************************************************************************************
* FunctionName   : SDInit()
* Description    : 初始化SD卡，并返回状态
* EntryParameter : None
* ReturnValue    : 返回操作状态：成功-0
********************************************************************************************************/
u8 SDInit(void)
{
	u8  buff[6];
	u16 count = SD_OPERATE_NUM;											// 超时值
	u8  rvalue = SD_RESPONSE_FAILURE;				                    // 返回值

	SD_SetGpio();														// 端口设置
  
	if (SDGetCardStatus() == SD_CARD_OK)								// 获取SD卡插卡状态				  
	{ 
		SD_LowSpeed();													// 低速
		SD_CS_WRITE(1);									                // 清除SD卡片选
	 	if (SD_IdleSta())												// 错误返回
		{
		    return 1;													// 超时返回1 设置到idle 模式失败
		}

		SD_CS_WRITE(0);									                // SD卡使能
		rvalue = SD_SendCmd(CMD8, 0x01AA, 0x87);
		// 如果卡片版本信息是v1.0版本的，即rvalue=0x05，则进行以下初始化
		if (rvalue == 0x05)
		{
		    SDCard.Type = SD_V1;
			SD_CS_WRITE(1);
			SD_WriteByte(0xFF);
			do
			{
			   rvalue = SD_SendCmd(CMD55, 0, 0);						// 先发CMD55，应返回0x01；否则出错
			   if (rvalue == 0xFF)
			   {
			   	   return rvalue;										// 只要不是0xff,就接着发送
			   }
			   
			   rvalue = SD_SendCmd(ACMD41, 0, 0);					    // 得到正确响应后，发ACMD41，应得到返回值0x00
			   
			} while ((rvalue != 0x00) && count--);
		
			if (count == 0)												// MMC卡额外初始化操作开始
			{
			    count = SD_OPERATE_NUM;
				do
				{
				    rvalue = SD_SendCmd(CMD1, 0, 0);    
				} while ((rvalue != 0x00) && count--);

				if (count == 0)											// MMC卡初始化超时
				{
				    return 1;
				}

				SDCard.Type = SD_MMC;								    // 写入卡类型
			}

		    SD_HighSpeed();												// 设置SPI为高速模式
			SD_WriteByte(0xFF);
			rvalue = SD_SendCmd(CMD59, 0, 0x95);						// 禁止CRC校验
			if (rvalue != 0x00)											// 命令错误，返回 
			{
			    return rvalue;
			}

			rvalue = SD_SendCmd(CMD16, 512, 0x95);						// 设置Sector Size
			if (rvalue != 0x00)											// 命令错误，返回 
			{
			    return rvalue;
			}
		}
		else
		{
		    if (rvalue == 0x01)											 // 其中需要读取OCR数据，判断是SD2.0还是SD2.0HC卡
			{
			    buff[0] = SD_ReadByte();
				buff[1] = SD_ReadByte();
				buff[2] = SD_ReadByte();
				buff[3] = SD_ReadByte();
				SD_CS_WRITE(1);
				SD_WriteByte(0xFF);

				do 
				{
				    // 发卡初始化指令CMD55+ACMD41
					rvalue = SD_SendCmd(CMD55, 0, 0);
					if (rvalue != 0x01)
					{
					    return rvalue;
					}
			
					rvalue = SD_SendCmd(ACMD41, 0x40000000, 0x00);
				} while ((rvalue != 0x00) && count--);

				// 初始化指令发送完成，接下来获取OCR信息
				rvalue = SD_SendCmd(CMD58, 0, 0);
				if (rvalue != 0x00)
				{
				    SD_CS_WRITE(1);										// 释放SD片选信号
					return rvalue;
				}

				// OCR接收完成，片选置高
				buff[0] = SD_ReadByte();
				buff[1] = SD_ReadByte();
				buff[2] = SD_ReadByte();
				buff[3] = SD_ReadByte();

				// 检查接收到的OCR中的bit30位（CCS），确定其为SD2.0还是SDHC
                // 如果CCS=1：SDHC   CCS=0：SD2.0
				
				SDCard.Type = (((buff[0] & 0x40) > 0) ? SD_HC : SD_V2);

				SD_CS_WRITE(1);
				SD_WriteByte(0xFF);	
			}
		}

		SD_HighSpeed();												    // 设置SPI为高速模式
		SDGetCardInfo(&SDCard);										    // 获取SD卡信息
        SD_GetCID(&SDCid);
	}

	return rvalue;                      
}

/********************************************************************************************************
* FunctionName   : SDReadSector()
* Description    : 读取一扇区数据到pBuf缓冲中
* EntryParameter : readAddr - 地址，从0开始，0扇区为启动扇区；pBuf - 数据指针
* ReturnValue    : 返回操作状态：成功-0
********************************************************************************************************/
u8 SDReadSector(u32 readAddr, u8 *pBuf)
{
    u16 i;
	u16 count = SD_OPERATE_NUM;										    // 超时值
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // 返回值
	
 	SD_CS_WRITE(0);									                    // SD卡使能

	// SDHC 的读写地址是以BLOCK为单位， SD是字节为单位； 两者是512倍的关系
	if (SDCard.Type != SD_HC)										    // 普通SD卡 
	{
	    readAddr <<= 9;													// 按字节读的需要转换一下地址
	}

	if (SD_SendCmd(CMD17, readAddr, 0xFF) == SD_RESPONSE_NO_ERROR)      // 发送CMD17
	{
		while ((SD_ReadByte() != SD_START_SSREAD) && count)             // 等待数据接收开始，收到0xFE表示开始
		{
		    count--;		                                 		    // 等待超时
		}															   
		
		if (count != 0)												    // 超时判断
		{
			for (i=0; i<SD_SECTOR_SIZE; i++)                            // 读取一扇区数据
			{
			    *pBuf++ = SD_ReadByte();
			}
			
			SD_ReadByte();			                                    // 取走CRC字节，16位（不是我们需要的，但是SD卡需要）
			SD_ReadByte();

			rvalue = SD_RESPONSE_NO_ERROR;				                // 设置成功标志
		}
	}
	
	SD_CS_WRITE(1);									                    // 清除SD卡片选									 
    SD_WriteByte(0xFF);								                    // 8个时钟脉冲的延迟
	
	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SDReadNSector()
* Description    : 读取一扇区数据到pBuf缓冲中
* EntryParameter : readAddr - 地址，从0开始；pBuf - 数据指针；num - 扇区数
* ReturnValue    : 返回操作状态：成功-0
********************************************************************************************************/
u8 SDReadNSector(u32 readAddr, u8 *pBuf, u8 num)
{
    u16 i;
    u16 count = SD_OPERATE_NUM;										    // 超时值
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // 返回值

	SD_CS_WRITE(0);									                    // SD卡使能
	
	// SDHC 的读写地址是以BLOCK为单位， SD是字节为单位； 两者是512倍的关系
	if (SDCard.Type != SD_HC)										    // 普通SD卡 
	{
	    readAddr <<= 9;													// 按字节读的需要转换一下地址
	}

	if (SD_SendCmd(CMD18, readAddr, 0xFF) == SD_RESPONSE_NO_ERROR)      // 发送CMD18
	{
		do
		{
		    while ((SD_ReadByte() != SD_START_MSREAD) && count)         // 等待数据接收开始，收到0xFE表示开始
		    {
		        count--;			                             		// 等待超时
		    }
		
		    if (count != 0)											    // 超时判断
			{
			    for (i=0; i<SD_SECTOR_SIZE; i++)                        // 读取数据
			    {
			        *pBuf++ = SD_ReadByte();
			    }
			
			    SD_ReadByte();			                                // 取走CRC字节，16位（不是我们需要的，但是SD卡需要）
			    SD_ReadByte();											 
				
				rvalue = SD_RESPONSE_NO_ERROR;						    // 设置成功标志
			}
	    } while (--num); 
	}
	
	SD_SendCmd(CMD12, 0x00, 0xFF);                                      // CMD12发送停止命令
	SD_CS_WRITE(1);									                    // 清除SD卡片选									 
    SD_WriteByte(0xFF);								                    // 8个时钟脉冲的延迟

	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SDWriteSector()
* Description    : 把pBuf中的数据写入一个扇区中
* EntryParameter : writeAddr - 地址，从0开始；pBuf - 数据指针
* ReturnValue    : 返回操作状态：成功-0
********************************************************************************************************/
u8 SDWriteSector(u32 writeAddr, u8 *pBuf)
{
	u16 i;
    u16 count = SD_OPERATE_NUM;										    // 超时值
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // 返回值
	
	SD_CS_WRITE(0);														// SD卡使能

	// SDHC 的读写地址是以BLOCK为单位， SD是字节为单位； 两者是512倍的关系
	if (SDCard.Type != SD_HC)										    // 普通SD卡 
	{
	    writeAddr <<= 9;												// 按字节写的需要转换一下地址
	}								  

	if (SD_SendCmd(CMD24, writeAddr, 0xFF) == SD_RESPONSE_NO_ERROR)     // 发送CMD24到SD卡中去
	{
		SD_WriteByte(0xFF);                                             // 发送填冲字节
		SD_WriteByte(0xFF);                                             // 发送填冲字节
		SD_WriteByte(SD_START_SSWRITE);                                 // 发送数据开始标志0xFE
		
		for (i=0; i<SD_SECTOR_SIZE; i++)                                // 写入数据
		{
		    SD_WriteByte(*pBuf++);
		}
		
		SD_WriteByte(0xFF);			                                    // 写入CRC字节，16位
		SD_WriteByte(0xFF);

		while ((SD_ReadByte() == SD_DATA_OK) && count)                  // 读取0000 0101字节
		{
		    count--;													// 等待超时
		}

	}

    count = SD_OPERATE_NUM;
	while (SD_ReadByte() == SD_RESPONSE_NO_ERROR)	                    // BUSY等待
	{
	    count--;
	}
    if (count != 0)
    {
        rvalue = SD_RESPONSE_NO_ERROR;					                // 成功标志
    }
	
	SD_CS_WRITE(1);									                    // 清除SD卡片选									 
    SD_WriteByte(0xFF);								                    // 8个时钟脉冲的延迟

	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SDWriteNSector()
* Description    : 把pBuf中的数据写入一个扇区中
* EntryParameter : writeAddr - 地址，从0开始；pBuf - 数据指针；num - 扇区数
* ReturnValue    : 返回操作状态：成功-0
********************************************************************************************************/
u8 SDWriteNSector(u32 writeAddr, u8 *pBuf, u8 num)
{
    u16 i;
    u16 count = SD_OPERATE_NUM;							                // 超时值
    u8 rvalue = SD_RESPONSE_FAILURE;				                    // 返回值
    
    SD_CS_WRITE(0);								                        // SD卡使能

    // SDHC 的读写地址是以BLOCK为单位， SD是字节为单位； 两者是512倍的关系
    if (SDCard.Type != SD_HC)						              	    // 普通SD卡 
    {
        writeAddr <<= 9;							                    // 按字节写的需要转换一下地址
    }
    
    if (SDCard.Type != SD_MMC)                                          // 如果目标卡不是MMC卡，启用ACMD23指令使能预擦除
    {   
        SD_SendCmd(ACMD23, num, 0x00);
    }

    if (SD_SendCmd(CMD25, writeAddr, 0xFF) == SD_RESPONSE_NO_ERROR)     // 发送CMD25到SD卡中去
    {
        SD_WriteByte(0xFF);                                             // 发送填冲字节
        SD_WriteByte(0xFF);                                             // 发送填冲字节
        
        do
        {
            SD_WriteByte(SD_START_MSWRITE);                             // 发送数据开始标志0xFC (0x11111100)
            
            for (i=0; i<SD_SECTOR_SIZE; i++)                            // 写入数据
            {
                SD_WriteByte(*pBuf++);
            }
            
            SD_WriteByte(0xFF);			                                // 写入CRC字节
            SD_WriteByte(0xFF);
            
            while ((SD_ReadByte() == SD_DATA_OK) && count)              // 读取0000 0101字节
            {
                count--;							// 等待超时
            }
            
            count = SD_OPERATE_NUM;
            while ((SD_ReadByte() != SD_RESPONSE_FAILURE) && count)	    // BUSY等待
            {
                count--;
            }
            
            if (count == 0)
            {
                break;                                                  // 写入失败
            }
            
        } while (--num);
    }
     
    SD_WriteByte(SD_STOP_MSWRITE);                                      // send 'stop transmission token' 0xFD

    count = SD_OPERATE_NUM;
    while ((SD_ReadByte() != SD_RESPONSE_FAILURE) && count)	            // BUSY等待
    {
        count--;
    }
    
    if (count != 0)
    {
        rvalue = SD_RESPONSE_NO_ERROR;					                // 成功标志
    }    

    SD_SendCmd(CMD12, 0x00, 0xFF);                                      // CMD12发送停止命令
    SD_CS_WRITE(1);								                        // 清除SD卡片选									 
    SD_WriteByte(0xFF);								                    // 8个时钟脉冲的延迟

    return rvalue;
}


/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
