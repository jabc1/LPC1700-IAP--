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
* Description    : SPI��ʽ��SD��������
*				   ֧��SD��SD v1.1��SD v2.0��SDHC(���Լ��ݶ��ֿ�)
*				   ע�⣺1G��SD��һ��û��MBR��2G�����ϵ�SDһ����MBR
********************************************************************************************************/

/********************************************************************************************************
* header file                                                 
********************************************************************************************************/
#include "../UserCode/source/config.h" 

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
SD_Info SDCard;						                                        // SD����Ϣ
SD_CID  SDCid;

/********************************************************************************************************
* FunctionName   : SD_SetGpio()
* Description    : SD�ܽŶ���
* EntryParameter : None
* ReturnValue    : ����״̬
********************************************************************************************************/
void SD_SetGpio(void)
{
	SD_CS_OUT();			                                                // ����ƬѡΪ����˿�
    SD_STU_IN();			                                                // ���ÿ����Ϊ����˿�
}

/********************************************************************************************************
* FunctionName   : SD_DelayUs()
* Description    : ��ʱ
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
* Description    : ��ȡSD���忨״̬״̬
* EntryParameter : None
* ReturnValue    : ����״̬
********************************************************************************************************/
u8 SDGetCardStatus(void)
{
    u8 rValue = 0x00;
	
	if (SD_STU_READ() == 0)	                                             // �忨���
	{
	    if (0 == 0)                                                      // ��״̬���
		{
		    rValue = 0x00;                                               // SD��OK
		}
		else
		{
			rValue = 0x04;                                               // SD������
		}
	}
	else
	{
		rValue = 0x02;                                                   // û��SD��
	}	

    return rValue;
}

/********************************************************************************************************
* FunctionName   : SD_PowerSwitch()
* Description    : ��Դ����
* EntryParameter : swh - ��Դ���أ�0 - �򿪵�Դ��1 - �رյ�Դ��
* ReturnValue    : None
********************************************************************************************************/
void SD_PowerSwitch(u8 swh)
{
    if (swh)
	{
	    ;  // �رյ�Դ
	}
	else
	{
	    ;  // �򿪵�Դ
	}        
}

/********************************************************************************************************
* FunctionName   : SD_SendCmd()
* Description    : SD�����
* EntryParameter : cmd - ����,arg - ����,crc - УӦ��
* ReturnValue    : ����ָ����Ӧ
********************************************************************************************************/
u8 SD_SendCmd(u8 cmd,u32 arg,u8 crc)
{
	u16 count = SD_OPERATE_NUM;										     // ��ʱֵ
	u8 rvalue = SD_RESPONSE_FAILURE;				                     // ����ֵ

	SD_CS_WRITE(1);
	SD_WriteByte(0xFF);								                     // 8��ʱ��������ӳ�
	SD_WriteByte(0xFF);
	SD_WriteByte(0xFF);
	SD_CS_WRITE(0);									                     // SD��ʹ��

	// ����6�ֽڵ�ָ������
    SD_WriteByte(cmd);			                                         // �ֽ� 1
	SD_WriteByte(arg >> 24);											 // �ֽ� 2
	SD_WriteByte(arg >> 16);											 // �ֽ� 3
	SD_WriteByte(arg >> 8);											     // �ֽ� 4
	SD_WriteByte(arg);													 // �ֽ� 5
	SD_WriteByte(crc);													 // �ֽ� 6
	
	SD_WriteByte(0xFF);

	do
	{
	    rvalue = SD_ReadByte();                                          // �ȴ��ظ�
		count--;
		SD_DelayUs(500);												 // ��ʱ��Ϊ�����ӶԿ��ļ�����(���Ӷ���Щ��������)
        if ((count % 200) == 0)
        {
            if (SDGetCardStatus() == SD_CARD_NO)                         // û��SD���˳�
            {
                break;
            }
        }
	}while ((rvalue == SD_RESPONSE_FAILURE) && count);
	
    return rvalue;
}

/********************************************************************************************************
* FunctionName   : SD_GetCSD()
* Description    : CSD-�Ĵ������Ĵ�������Ϊ128��16�ֽ�
* EntryParameter : csd �Ĵ���
* ReturnValue    : ���ز���״̬���ɹ�-0
********************************************************************************************************/
u8 SD_GetCSD(SD_CSD *csd)
{
    u8 i;
	u8 csdTable[16];
	u16 count = SD_OPERATE_NUM;
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // ����ֵ

	SD_CS_WRITE(0);									                    // SD��ʹ��
	
	if (SD_SendCmd(CMD9, 0x00, 0xFF) == SD_RESPONSE_NO_ERROR)
	{
	    while ((SD_ReadByte() != SD_START_SSREAD) && count)             // �ȴ����ݽ��տ�ʼ���յ�0xFE��ʾ��ʼ
		{
		    count--;		                                 		    // �ȴ���ʱ
		}

		if (count != 0x00)
		{
			for (i=0; i<16; i++)
			{
			    csdTable[i] = SD_ReadByte();							// ��ȡ16�ֽڼĴ�������
			}
		}

		SD_ReadByte();													// ��CRC
		SD_ReadByte();

		// �ѻ�ȡֵ����CSD�ṹ����												  
		csd->CSDStruct = (csdTable[0] & 0xC0) >> 6;				   		// ��ȡCSD�汾��Ϣ
	
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

		rvalue = SD_RESPONSE_NO_ERROR;				                    // ���óɹ���־
	}

	SD_CS_WRITE(1);									                    // ���SD��Ƭѡ									 
    SD_WriteByte(0xFF);								                    // 8��ʱ��������ӳ�
	
	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SD_GetCID()
* Description    : ��ȡSD��CID
* EntryParameter : id - SD����Ϣ
* ReturnValue    : None
********************************************************************************************************/
u8 SD_GetCID(SD_CID *cid)
{
    u8 i;
    u8 cidTable[16] = {0};
    u8 count = 0xFF;
    u8 rvalue = SD_RESPONSE_FAILURE;                                    // ����ֵ
    
    SD_CS_WRITE(0);                                                     // SD��ʹ��
    
    if (SD_SendCmd(CMD10, 0x00, 0xFF) == SD_RESPONSE_NO_ERROR)
    {
        while ((SD_ReadByte() != 0xFE) && count)                        // �ȴ����ݽ��տ�ʼ���յ�0xFE��ʾ��ʼ
        {
            count--;                                                    // �ȴ���ʱ
        }
    
        if (count != 0x00)
        {
            for (i=0; i<16; i++)
            {
                cidTable[i] = SD_ReadByte();
            }
        }
                      
        SD_ReadByte();                                                  // ��CRC
        SD_ReadByte();
        
        rvalue = SD_RESPONSE_NO_ERROR;                                  // ���óɹ���־
    }
    
    SD_CS_WRITE(1);                                                     // ���SD��Ƭѡ          
    SD_WriteByte(0xFF);                                                 // 8��ʱ��������ӳ�
    
    if (rvalue == SD_RESPONSE_NO_ERROR)
    {
        // �ѻ�ȡֵ����CID�ṹ����
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
* Description    : ��ȡSD����Ϣ
* EntryParameter : info - SD����Ϣ
* ReturnValue    : None
********************************************************************************************************/
void SDGetCardInfo(SD_Info *info)
{
    SD_CSD csd;
	
	SD_GetCSD(&csd);											        // ��ȡCSD����Ϣ
	
	if (csd.CSDStruct == 0)												   
	{
//	    info->Type = 0x10;									            // Version 1.01-1.10  Version 2.00/ Standard Capacity		   														  
		info->SectorSize = 512;					                        // SD��������С

		/* BLOCKNR = (C_SIZE+1) * MULT;
		   MULT = 2^(C_SIZE_MULT+2);
		   BLOCK_LEN = 2^READ_BL_LEN; */		   

		info->Capacity  = csd.DeviceSize + 1;					        // SD����������λΪ�ֽ�
	    info->Capacity *= (1 << (csd.DeviceSizeMul + 2));		        // 2��DeviceSizeMul+2�η�
	    info->Capacity *= (1 << csd.RdSectorSize);					    // 2��RdSectorSize�η�

		info->Sectors = info->Capacity / info->SectorSize;              // SD��������
	    info->Capacity >>= 20;								            // ת����MByte  
	}
	else
	{
//		info->Type = 0x20;									            // Version 2.00/ High Capacity
		info->SectorSize = 512;					                        // SD��������С
		
		/* memory capacity = (C_SIZE+1) * 512K byte */
		//info->Capacity = (u64)(csd.DeviceSize) * 512*1024;			// SD��������C_Size*512KByte
		info->Sectors  = csd.DeviceSize * 1024;                         // SD��������
	    info->Capacity = csd.DeviceSize >> 1;							// ת����MByte
	} 
}

/********************************************************************************************************
* FunctionName   : SD_IdleSta()
* Description    : SD���������ģʽ
* EntryParameter : None
* ReturnValue    : ���ز���״̬���ɹ�-0
********************************************************************************************************/
u8 SD_IdleSta(void)
{
	u16 i;
	u8 retry = 200;
    
	// �Ȳ���>74�����壬��SD���Լ���ʼ�����
    for (i=0; i<0x10; i++)
	{
	    SD_WriteByte(0xFF);
	} 

    //-----------------SD����λ��idle��ʼ-----------------
    // ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
    // ��ʱ��ֱ���˳�

    do
    {	   
        // ����CMD0����SD������IDLE״̬
        i = SD_SendCmd(CMD0, 0, 0x95);
        retry++;
    } while ((i != 0x01) && (retry > 0));

    // ����ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��
    return ((retry > 0) ? 0 : 1);						  
}

/********************************************************************************************************
* FunctionName   : SDInit()
* Description    : ��ʼ��SD����������״̬
* EntryParameter : None
* ReturnValue    : ���ز���״̬���ɹ�-0
********************************************************************************************************/
u8 SDInit(void)
{
	u8  buff[6];
	u16 count = SD_OPERATE_NUM;											// ��ʱֵ
	u8  rvalue = SD_RESPONSE_FAILURE;				                    // ����ֵ

	SD_SetGpio();														// �˿�����
  
	if (SDGetCardStatus() == SD_CARD_OK)								// ��ȡSD���忨״̬				  
	{ 
		SD_LowSpeed();													// ����
		SD_CS_WRITE(1);									                // ���SD��Ƭѡ
	 	if (SD_IdleSta())												// ���󷵻�
		{
		    return 1;													// ��ʱ����1 ���õ�idle ģʽʧ��
		}

		SD_CS_WRITE(0);									                // SD��ʹ��
		rvalue = SD_SendCmd(CMD8, 0x01AA, 0x87);
		// �����Ƭ�汾��Ϣ��v1.0�汾�ģ���rvalue=0x05����������³�ʼ��
		if (rvalue == 0x05)
		{
		    SDCard.Type = SD_V1;
			SD_CS_WRITE(1);
			SD_WriteByte(0xFF);
			do
			{
			   rvalue = SD_SendCmd(CMD55, 0, 0);						// �ȷ�CMD55��Ӧ����0x01���������
			   if (rvalue == 0xFF)
			   {
			   	   return rvalue;										// ֻҪ����0xff,�ͽ��ŷ���
			   }
			   
			   rvalue = SD_SendCmd(ACMD41, 0, 0);					    // �õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00
			   
			} while ((rvalue != 0x00) && count--);
		
			if (count == 0)												// MMC�������ʼ��������ʼ
			{
			    count = SD_OPERATE_NUM;
				do
				{
				    rvalue = SD_SendCmd(CMD1, 0, 0);    
				} while ((rvalue != 0x00) && count--);

				if (count == 0)											// MMC����ʼ����ʱ
				{
				    return 1;
				}

				SDCard.Type = SD_MMC;								    // д�뿨����
			}

		    SD_HighSpeed();												// ����SPIΪ����ģʽ
			SD_WriteByte(0xFF);
			rvalue = SD_SendCmd(CMD59, 0, 0x95);						// ��ֹCRCУ��
			if (rvalue != 0x00)											// ������󣬷��� 
			{
			    return rvalue;
			}

			rvalue = SD_SendCmd(CMD16, 512, 0x95);						// ����Sector Size
			if (rvalue != 0x00)											// ������󣬷��� 
			{
			    return rvalue;
			}
		}
		else
		{
		    if (rvalue == 0x01)											 // ������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC��
			{
			    buff[0] = SD_ReadByte();
				buff[1] = SD_ReadByte();
				buff[2] = SD_ReadByte();
				buff[3] = SD_ReadByte();
				SD_CS_WRITE(1);
				SD_WriteByte(0xFF);

				do 
				{
				    // ������ʼ��ָ��CMD55+ACMD41
					rvalue = SD_SendCmd(CMD55, 0, 0);
					if (rvalue != 0x01)
					{
					    return rvalue;
					}
			
					rvalue = SD_SendCmd(ACMD41, 0x40000000, 0x00);
				} while ((rvalue != 0x00) && count--);

				// ��ʼ��ָ�����ɣ���������ȡOCR��Ϣ
				rvalue = SD_SendCmd(CMD58, 0, 0);
				if (rvalue != 0x00)
				{
				    SD_CS_WRITE(1);										// �ͷ�SDƬѡ�ź�
					return rvalue;
				}

				// OCR������ɣ�Ƭѡ�ø�
				buff[0] = SD_ReadByte();
				buff[1] = SD_ReadByte();
				buff[2] = SD_ReadByte();
				buff[3] = SD_ReadByte();

				// �����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
                // ���CCS=1��SDHC   CCS=0��SD2.0
				
				SDCard.Type = (((buff[0] & 0x40) > 0) ? SD_HC : SD_V2);

				SD_CS_WRITE(1);
				SD_WriteByte(0xFF);	
			}
		}

		SD_HighSpeed();												    // ����SPIΪ����ģʽ
		SDGetCardInfo(&SDCard);										    // ��ȡSD����Ϣ
        SD_GetCID(&SDCid);
	}

	return rvalue;                      
}

/********************************************************************************************************
* FunctionName   : SDReadSector()
* Description    : ��ȡһ�������ݵ�pBuf������
* EntryParameter : readAddr - ��ַ����0��ʼ��0����Ϊ����������pBuf - ����ָ��
* ReturnValue    : ���ز���״̬���ɹ�-0
********************************************************************************************************/
u8 SDReadSector(u32 readAddr, u8 *pBuf)
{
    u16 i;
	u16 count = SD_OPERATE_NUM;										    // ��ʱֵ
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // ����ֵ
	
 	SD_CS_WRITE(0);									                    // SD��ʹ��

	// SDHC �Ķ�д��ַ����BLOCKΪ��λ�� SD���ֽ�Ϊ��λ�� ������512���Ĺ�ϵ
	if (SDCard.Type != SD_HC)										    // ��ͨSD�� 
	{
	    readAddr <<= 9;													// ���ֽڶ�����Ҫת��һ�µ�ַ
	}

	if (SD_SendCmd(CMD17, readAddr, 0xFF) == SD_RESPONSE_NO_ERROR)      // ����CMD17
	{
		while ((SD_ReadByte() != SD_START_SSREAD) && count)             // �ȴ����ݽ��տ�ʼ���յ�0xFE��ʾ��ʼ
		{
		    count--;		                                 		    // �ȴ���ʱ
		}															   
		
		if (count != 0)												    // ��ʱ�ж�
		{
			for (i=0; i<SD_SECTOR_SIZE; i++)                            // ��ȡһ��������
			{
			    *pBuf++ = SD_ReadByte();
			}
			
			SD_ReadByte();			                                    // ȡ��CRC�ֽڣ�16λ������������Ҫ�ģ�����SD����Ҫ��
			SD_ReadByte();

			rvalue = SD_RESPONSE_NO_ERROR;				                // ���óɹ���־
		}
	}
	
	SD_CS_WRITE(1);									                    // ���SD��Ƭѡ									 
    SD_WriteByte(0xFF);								                    // 8��ʱ��������ӳ�
	
	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SDReadNSector()
* Description    : ��ȡһ�������ݵ�pBuf������
* EntryParameter : readAddr - ��ַ����0��ʼ��pBuf - ����ָ�룻num - ������
* ReturnValue    : ���ز���״̬���ɹ�-0
********************************************************************************************************/
u8 SDReadNSector(u32 readAddr, u8 *pBuf, u8 num)
{
    u16 i;
    u16 count = SD_OPERATE_NUM;										    // ��ʱֵ
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // ����ֵ

	SD_CS_WRITE(0);									                    // SD��ʹ��
	
	// SDHC �Ķ�д��ַ����BLOCKΪ��λ�� SD���ֽ�Ϊ��λ�� ������512���Ĺ�ϵ
	if (SDCard.Type != SD_HC)										    // ��ͨSD�� 
	{
	    readAddr <<= 9;													// ���ֽڶ�����Ҫת��һ�µ�ַ
	}

	if (SD_SendCmd(CMD18, readAddr, 0xFF) == SD_RESPONSE_NO_ERROR)      // ����CMD18
	{
		do
		{
		    while ((SD_ReadByte() != SD_START_MSREAD) && count)         // �ȴ����ݽ��տ�ʼ���յ�0xFE��ʾ��ʼ
		    {
		        count--;			                             		// �ȴ���ʱ
		    }
		
		    if (count != 0)											    // ��ʱ�ж�
			{
			    for (i=0; i<SD_SECTOR_SIZE; i++)                        // ��ȡ����
			    {
			        *pBuf++ = SD_ReadByte();
			    }
			
			    SD_ReadByte();			                                // ȡ��CRC�ֽڣ�16λ������������Ҫ�ģ�����SD����Ҫ��
			    SD_ReadByte();											 
				
				rvalue = SD_RESPONSE_NO_ERROR;						    // ���óɹ���־
			}
	    } while (--num); 
	}
	
	SD_SendCmd(CMD12, 0x00, 0xFF);                                      // CMD12����ֹͣ����
	SD_CS_WRITE(1);									                    // ���SD��Ƭѡ									 
    SD_WriteByte(0xFF);								                    // 8��ʱ��������ӳ�

	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SDWriteSector()
* Description    : ��pBuf�е�����д��һ��������
* EntryParameter : writeAddr - ��ַ����0��ʼ��pBuf - ����ָ��
* ReturnValue    : ���ز���״̬���ɹ�-0
********************************************************************************************************/
u8 SDWriteSector(u32 writeAddr, u8 *pBuf)
{
	u16 i;
    u16 count = SD_OPERATE_NUM;										    // ��ʱֵ
	u8 rvalue = SD_RESPONSE_FAILURE;				                    // ����ֵ
	
	SD_CS_WRITE(0);														// SD��ʹ��

	// SDHC �Ķ�д��ַ����BLOCKΪ��λ�� SD���ֽ�Ϊ��λ�� ������512���Ĺ�ϵ
	if (SDCard.Type != SD_HC)										    // ��ͨSD�� 
	{
	    writeAddr <<= 9;												// ���ֽ�д����Ҫת��һ�µ�ַ
	}								  

	if (SD_SendCmd(CMD24, writeAddr, 0xFF) == SD_RESPONSE_NO_ERROR)     // ����CMD24��SD����ȥ
	{
		SD_WriteByte(0xFF);                                             // ��������ֽ�
		SD_WriteByte(0xFF);                                             // ��������ֽ�
		SD_WriteByte(SD_START_SSWRITE);                                 // �������ݿ�ʼ��־0xFE
		
		for (i=0; i<SD_SECTOR_SIZE; i++)                                // д������
		{
		    SD_WriteByte(*pBuf++);
		}
		
		SD_WriteByte(0xFF);			                                    // д��CRC�ֽڣ�16λ
		SD_WriteByte(0xFF);

		while ((SD_ReadByte() == SD_DATA_OK) && count)                  // ��ȡ0000 0101�ֽ�
		{
		    count--;													// �ȴ���ʱ
		}

	}

    count = SD_OPERATE_NUM;
	while (SD_ReadByte() == SD_RESPONSE_NO_ERROR)	                    // BUSY�ȴ�
	{
	    count--;
	}
    if (count != 0)
    {
        rvalue = SD_RESPONSE_NO_ERROR;					                // �ɹ���־
    }
	
	SD_CS_WRITE(1);									                    // ���SD��Ƭѡ									 
    SD_WriteByte(0xFF);								                    // 8��ʱ��������ӳ�

	return rvalue;
}

/********************************************************************************************************
* FunctionName   : SDWriteNSector()
* Description    : ��pBuf�е�����д��һ��������
* EntryParameter : writeAddr - ��ַ����0��ʼ��pBuf - ����ָ�룻num - ������
* ReturnValue    : ���ز���״̬���ɹ�-0
********************************************************************************************************/
u8 SDWriteNSector(u32 writeAddr, u8 *pBuf, u8 num)
{
    u16 i;
    u16 count = SD_OPERATE_NUM;							                // ��ʱֵ
    u8 rvalue = SD_RESPONSE_FAILURE;				                    // ����ֵ
    
    SD_CS_WRITE(0);								                        // SD��ʹ��

    // SDHC �Ķ�д��ַ����BLOCKΪ��λ�� SD���ֽ�Ϊ��λ�� ������512���Ĺ�ϵ
    if (SDCard.Type != SD_HC)						              	    // ��ͨSD�� 
    {
        writeAddr <<= 9;							                    // ���ֽ�д����Ҫת��һ�µ�ַ
    }
    
    if (SDCard.Type != SD_MMC)                                          // ���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����
    {   
        SD_SendCmd(ACMD23, num, 0x00);
    }

    if (SD_SendCmd(CMD25, writeAddr, 0xFF) == SD_RESPONSE_NO_ERROR)     // ����CMD25��SD����ȥ
    {
        SD_WriteByte(0xFF);                                             // ��������ֽ�
        SD_WriteByte(0xFF);                                             // ��������ֽ�
        
        do
        {
            SD_WriteByte(SD_START_MSWRITE);                             // �������ݿ�ʼ��־0xFC (0x11111100)
            
            for (i=0; i<SD_SECTOR_SIZE; i++)                            // д������
            {
                SD_WriteByte(*pBuf++);
            }
            
            SD_WriteByte(0xFF);			                                // д��CRC�ֽ�
            SD_WriteByte(0xFF);
            
            while ((SD_ReadByte() == SD_DATA_OK) && count)              // ��ȡ0000 0101�ֽ�
            {
                count--;							// �ȴ���ʱ
            }
            
            count = SD_OPERATE_NUM;
            while ((SD_ReadByte() != SD_RESPONSE_FAILURE) && count)	    // BUSY�ȴ�
            {
                count--;
            }
            
            if (count == 0)
            {
                break;                                                  // д��ʧ��
            }
            
        } while (--num);
    }
     
    SD_WriteByte(SD_STOP_MSWRITE);                                      // send 'stop transmission token' 0xFD

    count = SD_OPERATE_NUM;
    while ((SD_ReadByte() != SD_RESPONSE_FAILURE) && count)	            // BUSY�ȴ�
    {
        count--;
    }
    
    if (count != 0)
    {
        rvalue = SD_RESPONSE_NO_ERROR;					                // �ɹ���־
    }    

    SD_SendCmd(CMD12, 0x00, 0xFF);                                      // CMD12����ֹͣ����
    SD_CS_WRITE(1);								                        // ���SD��Ƭѡ									 
    SD_WriteByte(0xFF);								                    // 8��ʱ��������ӳ�

    return rvalue;
}


/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
