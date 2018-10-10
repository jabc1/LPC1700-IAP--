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
* Description    : DS18B20驱动
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
* Description    : DS18B20延时1微秒,时序延时
* EntryParameter : us - 微秒
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
* Description    : DS18B20复位,DS18B20使用时先要复位
* EntryParameter : None
* ReturnValue    : 成功返回0，否则返回1
********************************************************************************************************/
u8 DST_Reset(void)
{  
    u8 value = 1;
    u16 count = 400;

	DST_IO_OUT();				 	                                            // 将端口设置为输出 

	DST_IO_SET();                                                               // DST复位
	DST_IO_CLR();                                                               // DST拉低
	DST_DelayUs(500);                                                           // 大于480us

	DST_IO_SET();                                                               // DST拉低
	DST_IO_IN();
	DST_DelayUs(20);                                                            // 精确延时15~60us

    do 
	{
	    count--;
	} while (DST_IO_READ() && (count > 0));
	
	if (count > 0)
	{
		DST_IO_SET();                                                           // 拉高
		DST_IO_OUT();
		DST_DelayUs(200);	                                                    // 60~240us
		value = 0; 
	}

	return (value);
} 

/********************************************************************************************************
* FunctionName   : DST_ReadByte()
* Description    : 从 1-wire 总线上读取一个字节,读总线上的数据有严格的时序
* EntryParameter : None
* ReturnValue    : value
********************************************************************************************************/ 
u8 DST_ReadByte(void)
{ 
	u8 i = 0;
	u8 value = 0;

	for (i=0; i<8; i++)
	{  
		DST_IO_CLR();                                                           // 给脉冲信号						 
		DST_IO_SET();                                                           // 给脉冲信号
		DST_IO_IN();				 	                                        // 将端口设置为输入

		if (DST_IO_READ() != 0) 
		{
		    value |= (1<<i);						                            // 写入"1"
		}
		else
		{
		    value &= ~(1<<i);
		}

		DST_DelayUs(50);                                                        // 15~60us
		DST_IO_OUT();				 	                                        // 将端口设置为输出
	}

  	return (value);
}

/********************************************************************************************************
* FunctionName   : DST_WriteByte()
* Description    : 向 1-WIRE 总线上写一个字节,写总线上的数据有严格的时序
* EntryParameter : val - 一字节数据
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
			DST_IO_SET();                                                       // 写入"1"
		}
		else
		{
			DST_IO_CLR();							                            // 写入"0"
		}

		DST_DelayUs(60);                                                        // 60~120us 
		DST_IO_SET();
   }
}

/********************************************************************************************************
* FunctionName   : DSTReadTempOne()
* Description    : 读温度并返回温度数据,返回的数值精确到小数点后一位，1-wire上只能有一个
* EntryParameter : None
* ReturnValue    : 温度值,如果值大于0x8000为负数，应减去0x8000
********************************************************************************************************/ 
u16 DSTReadTempOne(void) 
{
    fp32 res;
	u16 tvalue;                                                                 // 温度值
	
	DST_Reset();    
	DST_WriteByte(DST_SKIP_ROM);                                                // 跳过读序列号*
	DST_WriteByte(DST_CONVERT_T);                                               // 启动温度转换
	DST_DelayUs(10000);
	
	DST_Reset();    
	DST_WriteByte(DST_SKIP_ROM);                                                // 跳过读序列号 
	DST_WriteByte(DST_READ_SCRATCHPAD);                                         // 读取温度 
	
	tvalue  = DST_ReadByte();                                                   // 读出温度低8位
	tvalue |= (DST_ReadByte()<<8);                                              // 读出温度高8位，合并数据

  	if( tvalue >= 0x0FFF)							                            // 温度为负
   	{												     
     	tvalue = ~tvalue + 1;	
		tvalue |= 0x8000;                                                       // 最高位写"1",为负数 				     
   	}	

	switch (DST_Par.ConvBit)								                    // 分辨率判断
	{
		case DST_9BIT : tvalue >>= 3; res = DST_RSL_9BIT;  break;
		case DST_10BIT: tvalue >>= 2; res = DST_RSL_10BIT; break;
		case DST_11BIT: tvalue >>= 1; res = DST_RSL_11BIT; break;
		case DST_12BIT: tvalue >>= 0; res = DST_RSL_12BIT; break;		
		default:	    tvalue >>= 0; res = DST_RSL_12BIT; break;
	}
	    
	return (tvalue * ((res/10000)*10));                                         // 温度值扩大10倍，精确到1位小数
}

/********************************************************************************************************
* FunctionName   : DSInit()
* Description    : 初始化传感器，温度分辨率调整,此函数可以对温度的显示进行精度调节.
* EntryParameter : res - 精度设置(0x1F-0.5, 0x3F-0.25, 0x5F-0.125, 0x7F - 0.0625)
* ReturnValue    : 初始化成功返回0，否则返回1
********************************************************************************************************/
u8 DSTInit(u8 res)  
{
    u8 reValue = 1;

	if (DST_Reset() == 0)                                                       // 复位
	{
		DST_WriteByte(DST_SKIP_ROM);                                            // 跳过Rom
		DST_WriteByte(DST_WRITE_SCRATCHPAD);                                    // 写暂存器
	
		DST_WriteByte(0x02);                                                    // 写TH 
		DST_WriteByte(0x01);                                                    // 写TL
		DST_WriteByte(res);                                                     // 温度转换分辨率设置
		DST_Par.ConvBit = res;
	
		DST_Reset();                                                            // 复位
		DST_WriteByte(DST_SKIP_ROM);                                            // 跳过Rom 
		DST_WriteByte(DST_COPY_SCRATCHPAD);                                     // 把暂存器内容写到EPRam中
		reValue = 0;
	}

	return reValue;
}

/********************************************************************************************************
*                            多个DS18B20驱动部分                                               
********************************************************************************************************/
#if DST_MORE_EN

static DST_LAST DSTLast; 														// 搜索参数

/********************************************************************************************************
* FunctionName   : DST_ReadBit()
* Description    : 从 1-wire 总线上读取一位,读总线上的数据有严格的时序
* EntryParameter : None
* ReturnValue    : 返回位数据
********************************************************************************************************/ 
u8 DST_ReadBit(void)
{
    u8 bitDat;
	
    DST_IO_OUT();	                                                            // Maga控制总线
    DST_IO_CLR();	                                                            // 强制拉低
    DST_IO_IN();		                                                        // 释放总线,DS18B20会将总线强制拉低
    DST_DelayUs(3);
	
    if (DST_IO_READ())															// 读一位
	{
        bitDat = 1;
	}
    else
	{
        bitDat = 0;
	}
	
    DST_DelayUs(45);	 	                                                    // 45us
	
    DST_IO_IN();		                                                        // 释放总线
    DST_DelayUs(24);
	
    return (bitDat);
}

/********************************************************************************************************
* FunctionName   : DST_WriteBit()
* Description    : 向 1-WIRE 总线上写一个字节,写总线上的数据有严格的时序
* EntryParameter : bitDat - 写入一位数据
* ReturnValue    : None
********************************************************************************************************/
void DST_WriteBit(u8 bitDat)
{
    DST_IO_OUT();		                                                        // Maga 控制总线
    DST_IO_CLR();		                                                        // 强制拉低
	
    if (bitDat)
	{
        DST_IO_IN();	                                                        // 释放总线
	}
    else
	{
        DST_IO_CLR();	                                                        // 强制拉低
	}
	
    DST_DelayUs(60);	                        	                            // 60us
    
    DST_IO_IN();		                                                        // 释放总线
    DST_DelayUs(90);
}

/********************************************************************************************************
* FunctionName   : DST_Search()
* Description    : 搜索总线上的温度传感器
* EntryParameter : pID - 存放读取的8字节ID号
* ReturnValue    : 有器件返回1，否则返回0
********************************************************************************************************/
u8 DST_Search(u8 *pID)
{
    u8 crc8 = 0;
    u8 idBit;                                                                   // 数据位
	u8 cmpIdBit;                                                                // 数据位补码
	u8 idBitNumber = 1;                                                         // ID位计数值
    u8 lastZero = 0;                                                            // 最后一个"0"的位置
	u8 romByteNumber = 0;                                                       // ID数组下标
	u8 searchResult = 0;                                                        // 搜索结果                       
    u8 romByteMask = 1;                                                         // 每一字节待处理位标志
	u8 searchDirection;                                                         // 
	
    // if the last call was not the last one
    if (!DSTLast.DeviceFlag)                                                    // 判断是否已经搜索完毕
    {
        // 1-Wire reset
        if (DST_Reset())                                                        // 复位，等待器件响应 
        {
            // reset the search
            DSTLast.Discrepancy = 0;
            DSTLast.DeviceFlag = 0;
            DSTLast.FamilyDiscrepancy = 0;
            return 0;
        }
		
        // issue the search command
        DST_WriteByte(DST_SEARCH_ROM);                                          // 发送搜索指令
		
        // loop to do the search
        do
        {
            // read a bit and its complement
            idBit = DST_ReadBit();                                              // 读取数据位  
            cmpIdBit = DST_ReadBit();                                           // 读取补码位
			// 00 - 有器件连接，在此数据位上它们的值发生冲突；01 - 有器件连接，在此数据位上它们的值都为0；
			// 10 - 有器件连接，在此数据位上它们的值都为1；   11 - 无器件与单总线连接。
        	
            // check for no devices on 1-Wire
            if ((idBit == 1) && (cmpIdBit == 1))                                // "11" 判断有无器件    
            {
        	    break;                                                          // 无器件             
        	}
            else                                                                // 有器件
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
                    *(pID+romByteNumber) |= romByteMask;                          // 写入"1"
        		}
                else
        		{
                    *(pID+romByteNumber) &= ~romByteMask;                         // 写入"0"
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
        	
            searchResult = 1;                                                   // 搜索成功
        }
    }
	
    // if no device found then reset counters so next 'search' will be like a first
    if (!searchResult || !(*(pID+0)))
    {
        DSTLast.Discrepancy = 0;
        DSTLast.DeviceFlag = 0;
        DSTLast.FamilyDiscrepancy = 0;
        searchResult = 0;                                                       // 搜索失败
    }
	
    return searchResult;
}

/********************************************************************************************************
* FunctionName   : DSTFirst()
* Description    : 寻找总线上的第1个器件
* EntryParameter : pID - 存放读取的ID号
* ReturnValue    : 成功返回1，否则返回0
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
* Description    : 搜索下一个的ID号
* EntryParameter : pID - 存放读取的8字节ID号，一般存放在数组中，这里用数组名
* ReturnValue    : TRUE - device found, ROM number in ROM_None buffer;FALSE - no device present
********************************************************************************************************/
u8 DSTNext(u8 *pID)
{
    return DST_Search(pID);												         // leave the search state alone
}
	
/********************************************************************************************************
* FunctionName   : DSTAutoSearch()
* Description    : 自动搜索1-wire总线上DS18B20的ID号
* EntryParameter : pID - 存放读取的8字节ID号；flag - 标志（"0" 搜索第一个, "1" 搜索下一个）。
* ReturnValue    : 成功返回1，否则返回0
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
* Description    : 搜索1-wire总线上DS18B20的个数
* EntryParameter : None
* ReturnValue    : count - 器件个数
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
	    count++;                                                                // 计数器加1
	}
	
	return count;                                                               // 返回搜索到的器件个数
}
	
/********************************************************************************************************
* FunctionName   : DSTReadROMOne()
* Description    : 单个DS18B20的ID读取，1-wire上只能有一个DS18B20
* EntryParameter : *pID - 存放8字节ID号的数组
* ReturnValue    : None
********************************************************************************************************/ 
void DSTReadROMOne(u8* pID)
{
    u8 i;
	
	DST_Reset();                                                                // 复位
	DST_WriteByte(DST_READ_ROM);                                                // 读序列号
	
	for (i=0; i<8; i++)                                                         // 64位ID号
	{
		*(pID+i) = DST_ReadByte();                                              // 读一个字节
	}
}	

/********************************************************************************************************
* FunctionName   : DSTReadTempMore()
* Description    : 读温度并返回温度数据,返回的数值精确到小数点后一位，1-wire上只能有一个
* EntryParameter : p - DS18B20的8字节ID，一般存放在数组中，这里传递数组名即可
* ReturnValue    : 温度值,如果值大于0x8000为负数，应减去0x8000
********************************************************************************************************/ 
u16 DSTReadTempMore(u8 *pID) 
{
	u8 i;
	u16 tvalue;                                                                 // 温度值
	fp32 res;
	
	DST_Reset();    
	DST_WriteByte(DST_SKIP_ROM);                                                // 跳过读序列号*
	DST_WriteByte(DST_CONVERT_T);                                               // 启动温度转换
	
	DST_Reset();    
	DST_WriteByte(DST_MATCH_ROM);                                               // 匹配指令
	for (i=0; i<8; i++)
	{
	    DST_WriteByte(*(pID+i));                                                // 发送ID
	} 
	
	DST_WriteByte(DST_READ_SCRATCHPAD);                                         // 读取温度	
	tvalue  = DST_ReadByte();                                                   // 读出温度低8位
	tvalue |= (DST_ReadByte()<<8);                                              // 读出温度高8位,后合并数据

  	if( tvalue >= 0x0FFF)							                            // 温度为负
   	{												     
     	tvalue = ~tvalue + 1;	
		tvalue |= 0x8000;                                                       // 最高位写"1",为负数 				     
   	}	

	switch (DST_Par.ConvBit)								                    // 分辨率判断
	{
		case DST_9BIT : tvalue >>= 3; res = DST_RSL_9BIT;  break;
		case DST_10BIT: tvalue >>= 2; res = DST_RSL_10BIT; break;
		case DST_11BIT: tvalue >>= 1; res = DST_RSL_11BIT; break;
		case DST_12BIT: tvalue >>= 0; res = DST_RSL_12BIT; break;		
		default:	    tvalue >>= 0; res = DST_RSL_12BIT; break;
	}
	    
	return (tvalue * ((res/10000)*10));                                         // 温度值扩大10倍，精确到1位小数
}

/********************************************************************************************************
* FunctionName   : DST_Crc8()
* Description    : CRC计算 
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
