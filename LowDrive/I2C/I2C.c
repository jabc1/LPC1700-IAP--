/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : i2c.c
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
#include "../UserCode/source/config.h"    			                                 // 头文件

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
static LPC_I2C_TypeDef(* const LPC_I2C[3]) = {LPC_I2C0, LPC_I2C1, LPC_I2C2};   	    // 总线定义

/********************************************************************************************************
* FunctionName   : I2C_DelayMs()
* Description    : 延时函数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void I2C_DelayMs(u16 t)
{
    u8 i;

	while (t--)
	{
	    for (i=0; i<200; i++)
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : I2CInit()
* Description    : 初始化I2C
* EntryParameter : num - 总线选择(0,1,2); fi2c - 初始化I2C总线速率，最大值为400K
* ReturnValue    : 成功返回0，否则返回1
********************************************************************************************************/
u8 I2CInit(u8 num, u32 fi2c)
{
    u32 i2cClock = SystemCoreClock / 4;										         // I2C时钟

	if (num > 3)																	 // 错误设置
	{
	    return (1);
	}

	if (fi2c > 400000)
	{
	    fi2c = 400000;
	} 

	I2C_SetGpio(num);											                     // GPIO设置

	switch (num)																	 // 打开电源
	{
	    case I2C0: LPC_SC->PCONP |= (1 << 7);  break;
	    case I2C1: LPC_SC->PCONP |= (1 << 19); break;	   
		case I2C2: LPC_SC->PCONP |= (1 << 26); break;
		default  : break;
	}

	LPC_I2C[num]->I2CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;
	LPC_I2C[num]->I2SCLH = (i2cClock / fi2c + 1) / 2;								// 设置I2C时钟为fi2c
	LPC_I2C[num]->I2SCLL = (i2cClock / fi2c) / 2;
    LPC_I2C[num]->I2CONSET = I2CONSET_I2EN;                                         // 使能主I2C
	 
	I2C_Stop(num);

	return (0);
}

/********************************************************************************************************
* FunctionName   : I2C_SetGpio()
* Description    : 端口设置
* EntryParameter : num - 总线选择(0,1,2)
* ReturnValue    : None
********************************************************************************************************/
void I2C_SetGpio(u8 num)
{
    switch (num)
	{
	    case I2C0:     // I2C0
		{
		    GPIOSetMode(I2C_SDA0_PORT, I2C_SDA0_PIN, I2C_SDA0_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
            GPIOSetMode(I2C_SCL0_PORT, I2C_SCL0_PIN, I2C_SCL0_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
			break;
		}

		case I2C1:	   // I2C1
		{
			GPIOSetMode(I2C_SDA1_PORT, I2C_SDA1_PIN, I2C_SDA1_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
            GPIOSetMode(I2C_SCL1_PORT, I2C_SCL1_PIN, I2C_SCL1_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
			break;
		}

		case I2C2:	   // I2C2
		{
			GPIOSetMode(I2C_SDA2_PORT, I2C_SDA2_PIN, I2C_SDA2_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
            GPIOSetMode(I2C_SCL2_PORT, I2C_SCL2_PIN, I2C_SCL2_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
			break;
		}

		default: break;
	}
}

/********************************************************************************************************
* FunctionName   : I2C_Start()
* Description    : 启动
* EntryParameter : num - 总线选择(0,1,2)
* ReturnValue    : None
********************************************************************************************************/
void I2C_Start(u8 num)
{
	LPC_I2C[num]->I2CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;
	LPC_I2C[num]->I2CONSET = I2CONSET_I2EN | I2CONSET_STA;

	while (0x08 != LPC_I2C[num]->I2STAT)
	{
	    ;
	}

	LPC_I2C[num]->I2CONCLR = I2CONCLR_STAC;                                          // 清除SI和STA位,结束过程
}

/********************************************************************************************************
* FunctionName   : I2C_Stop()
* Description    : 停止
* EntryParameter : num - 总线选择(0,1,2)
* ReturnValue    : None
********************************************************************************************************/
void I2C_Stop(u8 num)
{
	LPC_I2C[num]->I2CONSET = I2CONSET_STO;                                           // 设置停止标志 
	LPC_I2C[num]->I2CONCLR = I2CONCLR_SIC;                                           // 清除SI标志  
	
	while (LPC_I2C[num]->I2CONSET & I2CONSET_STO)									 // 等待停止信号
	{
	    ;
	}
}

/********************************************************************************************************
* FunctionName   : I2C_WriteByte()
* Description    : 写一字节
* EntryParameter : num - 总线选择(0,1,2)；dat - 要发送的数据或地址；mode：0-写地址；1-读地址；2-内部地址或数据
* ReturnValue    : 返回操作结果
********************************************************************************************************/
u8 I2C_WriteByte(u8 num, u8 dat, u8 mode)
{
    u8 ack = 0;
	
    LPC_I2C[num]->I2DAT = (mode != 2) ?  (dat + mode) : dat;						 // icAddr + 命令（0 - write，1 - read）, 放入内部地址或数据
    LPC_I2C[num]->I2CONCLR = I2CONCLR_SIC;                                           // 启动发送

	switch (mode)
	{
	    case 0:  while (LPC_I2C[num]->I2STAT != 0x18); ack = 0; break;	             // 已发送SLA+W；已接收ACK
		case 1:  while (LPC_I2C[num]->I2STAT != 0x40); ack = 0; break;	             // 已发送SLA+R；已接收ACK
		case 2:  while (LPC_I2C[num]->I2STAT != 0x28); ack = 0; break;	             // 已发送I2DAT中的数据字节；已接收ACK
		default: ack = 1; break;
	}

    return ack;
}

/********************************************************************************************************
* FunctionName   : I2C_ReadByte()
* Description    : 读一字节
* EntryParameter : num - 总线选择(0,1,2)；last - 最后一位数据标志
* ReturnValue    : 读取数据
********************************************************************************************************/
u8 I2C_ReadByte(u8 num, u8 last)
{
	u8  rValue = 0;

    if (last == 0)
	{
	    LPC_I2C[num]->I2CONSET = I2CONSET_AA;			                            // 产生应答
	}
	else
	{
	    LPC_I2C[num]->I2CONCLR = I2CONCLR_AAC;			                            // 不产生应答
	}

	LPC_I2C[num]->I2CONCLR = I2CONCLR_SIC;                                          // 启动接收数据

	if (1 == last)							
	{
	    while (LPC_I2C[num]->I2STAT != 0x58) ;			                            // 最后一位数据，不产生应答
	}
	else									
	{
		while (LPC_I2C[num]->I2STAT != 0x50) ;			                            // 不是最后一位，产生应答
	}

	rValue = LPC_I2C[num]->I2DAT;						                            // 返回接收的数

	return 	rValue;
}

/********************************************************************************************************
* FunctionName   : I2CWriteByte()
* Description    : 写入一字节到EEPROM
* EntryParameter : num - 总线选择(0,1,2)；icAddr - 器件地址；type - 内部地址类型；addr - 内部地址；dat - 数据
* ReturnValue    : None
********************************************************************************************************/
void I2CWriteByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 dat)
{
	I2C_Start(num);							                                        // 发送起始信号
	I2C_WriteByte(num, icAddr, 0);                                                  // 写地址和写模式
	if (type == 0x01)					                                            // 0x00 - 没有内部地址
	{
		I2C_WriteByte(num, addr, 2);                                                // IC内部地址低8位
	}

	I2C_WriteByte(num, dat, 2);				                                        // 数据写入
	I2C_Stop(num);							                                        // 发送停止信号
	I2C_DelayMs(10);                                                                // 延时10ms
}

/********************************************************************************************************
* FunctionName   : I2CReadByte()
* Description    : 读取一字节
* EntryParameter : num - 总线选择(0,1,2)；icAddr - 器件地址；type - 内部地址类型；addr - 内部地址
* ReturnValue    : 返回读取数据
********************************************************************************************************/
u8 I2CReadByte(u8 num, u8 icAddr, u8 type, u8 addr)
{
	u8 dat = 0;
	
	if (type == 0x01)						                                        // 0x00 - 没有内部地址
	{
		I2C_Start(num);						                                        // 发送起始信号
		I2C_WriteByte(num, icAddr, 0);		                                        // 写入地址
		I2C_WriteByte(num, addr, 2);                                                // IC内部地址低8位
		I2C_Stop(num);						                                        // 地址操作
	}
	
	I2C_DelayMs(180);						                                        // IC卡不延时就会死在写读地址的地方
	I2C_Start(num);							                                        // 发送起始信号
	I2C_WriteByte(num, icAddr, 1);			                                        // 发送读取地址
	dat = I2C_ReadByte(num, 1);				                                        // 读取数据
	I2C_Stop(num);							                                        // 结束

	return dat;								                                        // 返回数据
}

/********************************************************************************************************
* FunctionName   : I2CWriteNByte()
* Description    : 写入多字节到IC
* EntryParameter : num - 总线选择(0,1,2)；icAddr - 器件地址；type - IC内部地址类型；addr - 内部地址；*s - 字符串；num - 长度
* ReturnValue    : None
********************************************************************************************************/
void I2CWriteNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 n)
{
	u32 i;

	I2C_Start(num);							                                        // 发送起始信号
	I2C_WriteByte(num, icAddr, 0);                                                  // 写地址和写模式
	if (type == 0x01)					                                            // 0x00 - 没有内部地址
	{
	    I2C_WriteByte(num, addr, 2);                                                // IC内部地址
	}

	for (i=0; i<n; i++)
	{
	    I2C_WriteByte(num, *s++, 2);		                                        // 数据写入
	}

	I2C_Stop(num);							                                        // 数据读完，发送停止信号
	I2C_DelayMs(10);                                                                // 延时10ms
}

/********************************************************************************************************
* FunctionName   : I2CReadNByte()
* Description    : 从IC读取多字节
* EntryParameter : num - 总线选择(0,1,2)；icAddr - 器件地址；type - IC内部地址类型；addr - 内部地址；*s - 字符串；n - 长度
* ReturnValue    : None
********************************************************************************************************/
void I2CReadNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 n)
{
	u32 i;
	
	if (type == 0x01)						                                        // 0x00 - 没有内部地址
	{
		I2C_Start(num);						                                        // 发送起始信号
		I2C_DelayMs(180);
		I2C_WriteByte(num, icAddr, 0);		                                        // 写入地址
		I2C_WriteByte(num, addr, 2);		                                        // IC内部地址
		I2C_Stop(num);						                                        // 地址操作
	}
	
	I2C_Start(num);							                                        // 发送起始信号
	I2C_WriteByte(num, icAddr, 1);			                                        // 发送读取地址
	for (i=0; i<n-1; i++)
	{
	    *s++ = I2C_ReadByte(num, 0);		                                        // 读取数据
	}

	*s = I2C_ReadByte(num, 1);				                                        // 读最后一个数据
	I2C_Stop(num);							                                        // 停止
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
