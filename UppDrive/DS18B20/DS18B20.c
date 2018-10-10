/******************************************************************************************************** 
*                              
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : DS18B20.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : DS18B20����
*
*
********************************************************************************************************/

/********************************************************************************************************
*                                  HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"

/********************************************************************************************************
* Variable Definition                            
********************************************************************************************************/
DST_PAR DST_Par;

/********************************************************************************************************
* FunctionName   : DST_DelayUs()
* Description    : DS18B20��ʱ1΢��,ʱ����ʱ
* EntryParameter : us - ΢��
* ReturnValue    : None
********************************************************************************************************/
void DST_DelayUs(u16 us)
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
* FunctionName   : DST_Reset()
* Description    : DS18B20��λ,DS18B20ʹ��ʱ��Ҫ��λ
* EntryParameter : None
* ReturnValue    : �ɹ�����0�����򷵻�1
********************************************************************************************************/
u8 DST_Reset(void)
{  
    u8 value = 1;
    u16 count = 400;

	DST_IO_OUT();				 	                                            // ���˿�����Ϊ��� 

	DST_IO_SET();                                                               // DST��λ
	DST_IO_CLR();                                                               // DST����
	DST_DelayUs(500);                                                           // ����480us

	DST_IO_SET();                                                               // DST����
	DST_IO_IN();
	DST_DelayUs(20);                                                            // ��ȷ��ʱ15~60us

    do 
	{
	    count--;
	} while (DST_IO_READ() && (count > 0));
	
	if (count > 0)
	{
		DST_IO_SET();                                                           // ����
		DST_IO_OUT();
		DST_DelayUs(200);	                                                    // 60~240us
		value = 0; 
	}

	return (value);
} 

/********************************************************************************************************
* FunctionName   : DST_ReadByte()
* Description    : �� 1-wire �����϶�ȡһ���ֽ�,�������ϵ��������ϸ��ʱ��
* EntryParameter : None
* ReturnValue    : value
********************************************************************************************************/ 
u8 DST_ReadByte(void)
{ 
	u8 i = 0;
	u8 value = 0;

	for (i=0; i<8; i++)
	{  
		DST_IO_CLR();                                                           // �������ź�						 
		DST_IO_SET();                                                           // �������ź�
		DST_IO_IN();				 	                                        // ���˿�����Ϊ����

		if (DST_IO_READ() != 0) 
		{
		    value |= (1<<i);						                            // д��"1"
		}
		else
		{
		    value &= ~(1<<i);
		}

		DST_DelayUs(50);                                                        // 15~60us
		DST_IO_OUT();				 	                                        // ���˿�����Ϊ���
	}

  	return (value);
}

/********************************************************************************************************
* FunctionName   : DST_WriteByte()
* Description    : �� 1-WIRE ������дһ���ֽ�,д�����ϵ��������ϸ��ʱ��
* EntryParameter : val - һ�ֽ�����
* ReturnValue    : None
********************************************************************************************************/
void DST_WriteByte(u8 val)
{
    u8 i = 0;

    for (i=0; i<8; i++)
    { 
		DST_IO_CLR();

		if (val & (1<<i))
		{
			DST_IO_SET();                                                       // д��"1"
		}
		else
		{
			DST_IO_CLR();							                            // д��"0"
		}

		DST_DelayUs(60);                                                        // 60~120us 
		DST_IO_SET();
   }
}

/********************************************************************************************************
* FunctionName   : DSTReadTempOne()
* Description    : ���¶Ȳ������¶�����,���ص���ֵ��ȷ��С�����һλ��1-wire��ֻ����һ��
* EntryParameter : None
* ReturnValue    : �¶�ֵ,���ֵ����0x8000Ϊ������Ӧ��ȥ0x8000
********************************************************************************************************/ 
u16 DSTReadTempOne(void) 
{
    fp32 res;
	u16 tvalue;                                                                 // �¶�ֵ
	
	DST_Reset();    
	DST_WriteByte(DST_SKIP_ROM);                                                // ���������к�*
	DST_WriteByte(DST_CONVERT_T);                                               // �����¶�ת��
	DST_DelayUs(10000);
	
	DST_Reset();    
	DST_WriteByte(DST_SKIP_ROM);                                                // ���������к� 
	DST_WriteByte(DST_READ_SCRATCHPAD);                                         // ��ȡ�¶� 
	
	tvalue  = DST_ReadByte();                                                   // �����¶ȵ�8λ
	tvalue |= (DST_ReadByte()<<8);                                              // �����¶ȸ�8λ���ϲ�����

  	if( tvalue >= 0x0FFF)							                            // �¶�Ϊ��
   	{												     
     	tvalue = ~tvalue + 1;	
		tvalue |= 0x8000;                                                       // ���λд"1",Ϊ���� 				     
   	}	

	switch (DST_Par.ConvBit)								                    // �ֱ����ж�
	{
		case DST_9BIT : tvalue >>= 3; res = DST_RSL_9BIT;  break;
		case DST_10BIT: tvalue >>= 2; res = DST_RSL_10BIT; break;
		case DST_11BIT: tvalue >>= 1; res = DST_RSL_11BIT; break;
		case DST_12BIT: tvalue >>= 0; res = DST_RSL_12BIT; break;		
		default:	    tvalue >>= 0; res = DST_RSL_12BIT; break;
	}
	    
	return (tvalue * ((res/10000)*10));                                         // �¶�ֵ����10������ȷ��1λС��
}

/********************************************************************************************************
* FunctionName   : DSInit()
* Description    : ��ʼ�����������¶ȷֱ��ʵ���,�˺������Զ��¶ȵ���ʾ���о��ȵ���.
* EntryParameter : res - ��������(0x1F-0.5, 0x3F-0.25, 0x5F-0.125, 0x7F - 0.0625)
* ReturnValue    : ��ʼ���ɹ�����0�����򷵻�1
********************************************************************************************************/
u8 DSTInit(u8 res)  
{
    u8 reValue = 1;

	if (DST_Reset() == 0)                                                       // ��λ
	{
		DST_WriteByte(DST_SKIP_ROM);                                            // ����Rom
		DST_WriteByte(DST_WRITE_SCRATCHPAD);                                    // д�ݴ���
	
		DST_WriteByte(0x02);                                                    // дTH 
		DST_WriteByte(0x01);                                                    // дTL
		DST_WriteByte(res);                                                     // �¶�ת���ֱ�������
		DST_Par.ConvBit = res;
	
		DST_Reset();                                                            // ��λ
		DST_WriteByte(DST_SKIP_ROM);                                            // ����Rom 
		DST_WriteByte(DST_COPY_SCRATCHPAD);                                     // ���ݴ�������д��EPRam��
		reValue = 0;
	}

	return reValue;
}

/********************************************************************************************************
*                            ���DS18B20��������                                               
********************************************************************************************************/
#if DST_MORE_EN

static DST_LAST DSTLast; 														// ��������

/********************************************************************************************************
* FunctionName   : DST_ReadBit()
* Description    : �� 1-wire �����϶�ȡһλ,�������ϵ��������ϸ��ʱ��
* EntryParameter : None
* ReturnValue    : ����λ����
********************************************************************************************************/ 
u8 DST_ReadBit(void)
{
    u8 bitDat;
	
    DST_IO_OUT();	                                                            // Maga��������
    DST_IO_CLR();	                                                            // ǿ������
    DST_IO_IN();		                                                        // �ͷ�����,DS18B20�Ὣ����ǿ������
    DST_DelayUs(3);
	
    if (DST_IO_READ())															// ��һλ
	{
        bitDat = 1;
	}
    else
	{
        bitDat = 0;
	}
	
    DST_DelayUs(45);	 	                                                    // 45us
	
    DST_IO_IN();		                                                        // �ͷ�����
    DST_DelayUs(24);
	
    return (bitDat);
}

/********************************************************************************************************
* FunctionName   : DST_WriteBit()
* Description    : �� 1-WIRE ������дһ���ֽ�,д�����ϵ��������ϸ��ʱ��
* EntryParameter : bitDat - д��һλ����
* ReturnValue    : None
********************************************************************************************************/
void DST_WriteBit(u8 bitDat)
{
    DST_IO_OUT();		                                                        // Maga ��������
    DST_IO_CLR();		                                                        // ǿ������
	
    if (bitDat)
	{
        DST_IO_IN();	                                                        // �ͷ�����
	}
    else
	{
        DST_IO_CLR();	                                                        // ǿ������
	}
	
    DST_DelayUs(60);	                        	                            // 60us
    
    DST_IO_IN();		                                                        // �ͷ�����
    DST_DelayUs(90);
}

/********************************************************************************************************
* FunctionName   : DST_Search()
* Description    : ���������ϵ��¶ȴ�����
* EntryParameter : pID - ��Ŷ�ȡ��8�ֽ�ID��
* ReturnValue    : ����������1�����򷵻�0
********************************************************************************************************/
u8 DST_Search(u8 *pID)
{
    u8 crc8 = 0;
    u8 idBit;                                                                   // ����λ
	u8 cmpIdBit;                                                                // ����λ����
	u8 idBitNumber = 1;                                                         // IDλ����ֵ
    u8 lastZero = 0;                                                            // ���һ��"0"��λ��
	u8 romByteNumber = 0;                                                       // ID�����±�
	u8 searchResult = 0;                                                        // �������                       
    u8 romByteMask = 1;                                                         // ÿһ�ֽڴ�����λ��־
	u8 searchDirection;                                                         // 
	
    // if the last call was not the last one
    if (!DSTLast.DeviceFlag)                                                    // �ж��Ƿ��Ѿ��������
    {
        // 1-Wire reset
        if (DST_Reset())                                                        // ��λ���ȴ�������Ӧ 
        {
            // reset the search
            DSTLast.Discrepancy = 0;
            DSTLast.DeviceFlag = 0;
            DSTLast.FamilyDiscrepancy = 0;
            return 0;
        }
		
        // issue the search command
        DST_WriteByte(DST_SEARCH_ROM);                                          // ��������ָ��
		
        // loop to do the search
        do
        {
            // read a bit and its complement
            idBit = DST_ReadBit();                                              // ��ȡ����λ  
            cmpIdBit = DST_ReadBit();                                           // ��ȡ����λ
			// 00 - ���������ӣ��ڴ�����λ�����ǵ�ֵ������ͻ��01 - ���������ӣ��ڴ�����λ�����ǵ�ֵ��Ϊ0��
			// 10 - ���������ӣ��ڴ�����λ�����ǵ�ֵ��Ϊ1��   11 - �������뵥�������ӡ�
        	
            // check for no devices on 1-Wire
            if ((idBit == 1) && (cmpIdBit == 1))                                // "11" �ж���������    
            {
        	    break;                                                          // ������             
        	}
            else                                                                // ������
            {
                // all devices coupled have 0 or 1
                if (idBit != cmpIdBit)                                          // "01 or 10"
                {
        		    searchDirection = idBit;                                    // bit write value for search
        		}
                else                                                            // "00"
                {
                    // if this discrepancy if before the Last Discrepancy
                    // on a previous next then pick the same as last time
                    if (idBitNumber < DSTLast.Discrepancy)
        			{
                        searchDirection = ((*(pID+romByteNumber) & romByteMask) > 0);
        			}
                    else
        			{
                        // if equal to last pick 1, if not then pick 0
                        searchDirection = (idBitNumber == DSTLast.Discrepancy);
        			}
        			
                    // if 0 was picked then record its position in LastZero
                    if (searchDirection == 0)
                    {
                        lastZero = idBitNumber;
                        // check for Last discrepancy in family
                        if (lastZero < 9)
        				{
                            DSTLast.FamilyDiscrepancy = lastZero;
        				}
                    }
                }
        		
                // set or clear the bit in the ROM byte rom_byte_number
                // with mask rom_byte_mask
                if (searchDirection == 1)
        		{
                    *(pID+romByteNumber) |= romByteMask;                          // д��"1"
        		}
                else
        		{
                    *(pID+romByteNumber) &= ~romByteMask;                         // д��"0"
        		}
        		
                // serial number search direction write bit
                DST_WriteBit(searchDirection);
        		
                // increment the byte counter id_bit_number
                // and shift the mask rom_byte_mask
                idBitNumber++;
                romByteMask <<= 1;
        		
                // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
                if (romByteMask == 0)
                {
                    crc8 = DST_Crc8(crc8, *(pID+romByteNumber));                // accumulate the CRC
                    romByteNumber++;
                    romByteMask = 1;
                }
            }
			
        }while (romByteNumber < 8); // loop until through all ROM bytes 0-7
	
        // if the search was successful then
        if (!((idBitNumber < 65) || (crc8 != 0)))
        {
            // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
            DSTLast.Discrepancy = lastZero;
            // check for last device
            if (DSTLast.Discrepancy == 0)
    		{
                DSTLast.DeviceFlag = 1;
    		}
    		
            // check for last family group
            if (DSTLast.FamilyDiscrepancy == DSTLast.Discrepancy)
        	{
                DSTLast.FamilyDiscrepancy = 0;
        	}
        	
            searchResult = 1;                                                   // �����ɹ�
        }
    }
	
    // if no device found then reset counters so next 'search' will be like a first
    if (!searchResult || !(*(pID+0)))
    {
        DSTLast.Discrepancy = 0;
        DSTLast.DeviceFlag = 0;
        DSTLast.FamilyDiscrepancy = 0;
        searchResult = 0;                                                       // ����ʧ��
    }
	
    return searchResult;
}

/********************************************************************************************************
* FunctionName   : DSTFirst()
* Description    : Ѱ�������ϵĵ�1������
* EntryParameter : pID - ��Ŷ�ȡ��ID��
* ReturnValue    : �ɹ�����1�����򷵻�0
********************************************************************************************************/	
u8 DSTFirst(u8 *pID)
{
    DSTLast.Discrepancy = 0;													  // reset the search state
    DSTLast.DeviceFlag = 0;
    DSTLast.FamilyDiscrepancy = 0;
	
    return DST_Search(pID);
}

/********************************************************************************************************
* FunctionName   : DSTNext()
* Description    : ������һ����ID��
* EntryParameter : pID - ��Ŷ�ȡ��8�ֽ�ID�ţ�һ�����������У�������������
* ReturnValue    : TRUE - device found, ROM number in ROM_None buffer;FALSE - no device present
********************************************************************************************************/
u8 DSTNext(u8 *pID)
{
    return DST_Search(pID);												         // leave the search state alone
}
	
/********************************************************************************************************
* FunctionName   : DSTAutoSearch()
* Description    : �Զ�����1-wire������DS18B20��ID��
* EntryParameter : pID - ��Ŷ�ȡ��8�ֽ�ID�ţ�flag - ��־��"0" ������һ��, "1" ������һ������
* ReturnValue    : �ɹ�����1�����򷵻�0
********************************************************************************************************/	
u8 DSTAutoSearch(u8 *pID, u8 flag)
{
    if (0 == flag)                                                     
	{
	    DSTLast.Discrepancy = 0;
        DSTLast.DeviceFlag = 0;
        DSTLast.FamilyDiscrepancy = 0;
	}
	
	return DST_Search(pID);
}

/********************************************************************************************************
* FunctionName   : DSTDevicesCount()
* Description    : ����1-wire������DS18B20�ĸ���
* EntryParameter : None
* ReturnValue    : count - ��������
********************************************************************************************************/ 
u8 DSTDevicesCount(void)
{
    u8 tmp[8];
	u8 count = 0;
	
    DSTLast.Discrepancy = 0;													// reset the search state
    DSTLast.DeviceFlag = 0;
    DSTLast.FamilyDiscrepancy = 0;
	
    while (DST_Search(tmp))
	{
	    count++;                                                                // ��������1
	}
	
	return count;                                                               // ��������������������
}
	
/********************************************************************************************************
* FunctionName   : DSTReadROMOne()
* Description    : ����DS18B20��ID��ȡ��1-wire��ֻ����һ��DS18B20
* EntryParameter : *pID - ���8�ֽ�ID�ŵ�����
* ReturnValue    : None
********************************************************************************************************/ 
void DSTReadROMOne(u8* pID)
{
    u8 i;
	
	DST_Reset();                                                                // ��λ
	DST_WriteByte(DST_READ_ROM);                                                // �����к�
	
	for (i=0; i<8; i++)                                                         // 64λID��
	{
		*(pID+i) = DST_ReadByte();                                              // ��һ���ֽ�
	}
}	

/********************************************************************************************************
* FunctionName   : DSTReadTempMore()
* Description    : ���¶Ȳ������¶�����,���ص���ֵ��ȷ��С�����һλ��1-wire��ֻ����һ��
* EntryParameter : p - DS18B20��8�ֽ�ID��һ�����������У����ﴫ������������
* ReturnValue    : �¶�ֵ,���ֵ����0x8000Ϊ������Ӧ��ȥ0x8000
********************************************************************************************************/ 
u16 DSTReadTempMore(u8 *pID) 
{
	u8 i;
	u16 tvalue;                                                                 // �¶�ֵ
	fp32 res;
	
	DST_Reset();    
	DST_WriteByte(DST_SKIP_ROM);                                                // ���������к�*
	DST_WriteByte(DST_CONVERT_T);                                               // �����¶�ת��
	
	DST_Reset();    
	DST_WriteByte(DST_MATCH_ROM);                                               // ƥ��ָ��
	for (i=0; i<8; i++)
	{
	    DST_WriteByte(*(pID+i));                                                // ����ID
	} 
	
	DST_WriteByte(DST_READ_SCRATCHPAD);                                         // ��ȡ�¶�	
	tvalue  = DST_ReadByte();                                                   // �����¶ȵ�8λ
	tvalue |= (DST_ReadByte()<<8);                                              // �����¶ȸ�8λ,��ϲ�����

  	if( tvalue >= 0x0FFF)							                            // �¶�Ϊ��
   	{												     
     	tvalue = ~tvalue + 1;	
		tvalue |= 0x8000;                                                       // ���λд"1",Ϊ���� 				     
   	}	

	switch (DST_Par.ConvBit)								                    // �ֱ����ж�
	{
		case DST_9BIT : tvalue >>= 3; res = DST_RSL_9BIT;  break;
		case DST_10BIT: tvalue >>= 2; res = DST_RSL_10BIT; break;
		case DST_11BIT: tvalue >>= 1; res = DST_RSL_11BIT; break;
		case DST_12BIT: tvalue >>= 0; res = DST_RSL_12BIT; break;		
		default:	    tvalue >>= 0; res = DST_RSL_12BIT; break;
	}
	    
	return (tvalue * ((res/10000)*10));                                         // �¶�ֵ����10������ȷ��1λС��
}

/********************************************************************************************************
* FunctionName   : DST_Crc8()
* Description    : CRC���� 
* EntryParameter : None
* ReturnValue    : Returns current global Crc8 value
********************************************************************************************************/	
u8 DST_Crc8(u8 crc8, u8 value)
{ 
	uc8 crcTable[] = 
	{
	      0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65, 
	    157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220, 
	     35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98, 
	    190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255, 
	     70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7, 
	    219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154, 
	    101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36, 
	    248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185, 
	    140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205, 
	     17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80, 
	    175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238, 
	     50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115, 
	    202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139, 
	     87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22, 
	    233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168, 
	    116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
	};

    crc8 = crcTable[crc8 ^ value];
    return crc8;
}

#endif

/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
