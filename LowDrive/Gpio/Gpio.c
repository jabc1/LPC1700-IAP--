/******************************************************************************************************** 
*                               
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : gpio.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*				   所有I/O口在复位后默认为上拉输入，PORT0和PORT2端口的每个引脚都可以提供中断功能
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                                       // 头文件

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
static LPC_GPIO_TypeDef(* const LPC_GPIO[5]) = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4};

/********************************************************************************************************
* FunctionName   : GPIOInit()
* Description    : 初始化GPIO
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void GPIOInit(void)
{
    u8 i;

	for (i=0; i<5; i++)
	{
	    LPC_GPIO[i]->FIOMASK = 0x00;			             // 不屏蔽引脚高速功能
	}
}

/********************************************************************************************************
* FunctionName   : GPIOSetMode()
* Description    : 设置管脚模式
* EntryParameter : portNum-端口，bitPosi-位，fun-功能，mode-模式(上,下拉)，OpenDrain-开漏
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetMode(u8 portNum, u8 bitPosi, u8 fun, u8 mode, u8 OpenDrain)
{
    u32 addr;

    if (bitPosi < 16)
    {
	    addr = (u32)(&LPC_PINCON->PINSEL0) + portNum*2*PIN_OFFSET;
        PIN_REG(addr) |= (fun << bitPosi*2);		                                        // 管脚功能配置
		addr = (u32)(&LPC_PINCON->PINMODE0) + portNum*2*PIN_OFFSET;
		PIN_REG(addr) |= (mode << bitPosi*2);		                                        // 管脚模式配置
    }
	else
	{
	    addr = (u32)(&LPC_PINCON->PINSEL1) + portNum*2*PIN_OFFSET;
        PIN_REG(addr)  |= (fun << (bitPosi-16)*2);
		addr = (u32)(&LPC_PINCON->PINMODE1) + portNum*2*PIN_OFFSET;
		PIN_REG(addr) |= (mode << bitPosi*2);		                                        // 上下拉设置
	}

	addr = (u32)(&LPC_PINCON->PINMODE_OD0) + portNum*2*PIN_OFFSET;
	PIN_REG(addr) |= (OpenDrain << bitPosi);	                                            // 开漏模式
}

/********************************************************************************************************
* FunctionName   : GPIOSetDir()
* Description    : 端口方向设置，只对GPIO有效，其他功能自动控制方向(默认上拉输入)
* EntryParameter : port num, bit position, direction (1 out, 0 input)
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetDir(u8 portNum, u8 bitPosi, u8 dir)
{
    if (dir)
	{																					 
	    LPC_GPIO[portNum]->FIODIR |= 1<<bitPosi;											// 设置为输出
	}
	else
	{																						 
		LPC_GPIO[portNum]->FIODIR &= ~(1<<bitPosi);											// 设置为输入
	}
} 

/********************************************************************************************************
* FunctionName   : GPIOSetMultiDir()
* Description    : 端口方向设置，只对GPIO有效，其他功能自动控制方向(默认上拉输入)
* EntryParameter : port num, bit position, direction (1 out, 0 input)
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetMultiDir(u8 portNum, u32 mBitPosi, u8 dir)
{
    if (dir)
	{																					 
	    LPC_GPIO[portNum]->FIODIR |= mBitPosi;											    // 设置为输出
	}
	else
	{																						 
		LPC_GPIO[portNum]->FIODIR &= ~(mBitPosi);											// 设置为输入
	}
}

/********************************************************************************************************
* FunctionName   : GPIOSetValue()
* Description    : 写端口数据(置1和清0是通过两个寄存器实现的)
* EntryParameter : port num, bit position, bit value
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetValue(u8 portNum, u8 bitPosi, u8 bitVal)
{
    if (bitVal)
	{
        LPC_GPIO[portNum]->FIOSET = (1 << bitPosi);											// 置1
	}
	else
	{
		LPC_GPIO[portNum]->FIOCLR = (1 << bitPosi);											// 清0
	}
}

/********************************************************************************************************
* FunctionName   : GPIOSetMultiValue()
* Description    : 设置多位值
* EntryParameter : portNum - 端口，mBitPosi - 数据端口，val - 写入值
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetMultiValue(u8 portNum, u32 mBitPosi, u32 val)
{
    LPC_GPIO[portNum]->FIOCLR = mBitPosi;					                                // 先清0
	LPC_GPIO[portNum]->FIOSET = val;					                                    // 再写1    
}

/********************************************************************************************************
* FunctionName   : GPIOStatusValue()
* Description    : 读端口数据
* EntryParameter : port num, bit position
* ReturnValue    : 读端口值，返回1和0
********************************************************************************************************/
u8 GPIOStatusValue(u8 portNum, u8 bitPosi)
{
	return (((LPC_GPIO[portNum]->FIOPIN) & (1 << bitPosi)) != 0x00)	?  1 : 0;				// 读端口，并返回值
}

/********************************************************************************************************
* FunctionName   : GPIOStatusMultiValue()
* Description    : 读并行端口数据
* EntryParameter : port num, shield - 屏蔽地址(1-为要读取位，0-为屏蔽位)
* ReturnValue    : 读端口值，返回1和0
********************************************************************************************************/
u32 GPIOStatusMultiValue(u8 portNum, u32 shield)
{
	LPC_GPIO[portNum]->FIOMASK = ~shield;													// 写入屏蔽值
    return (LPC_GPIO[portNum]->FIOPIN); 	                                                // 读端口
}

/********************************************************************************************************
* FunctionName   : GPIOIntSet()
* Description    : 设置管脚中断(只能设置边沿触发，不能设置为电平触发)
* EntryParameter : port num, bit position, rising edge, falling edge
* ReturnValue    : None
********************************************************************************************************/
void GPIOIntSet(u8 portNum, u8 bitPosi, u8 risEg, u8 falEg)
{
    u32 addr;

	addr = (u32)(&LPC_GPIOINT->IO0IntEnR) + portNum*PIN_OFFSET;
    if (risEg)
	{ 
        PIN_REG(addr) |= 1 << bitPosi;				                                        // 使能上升沿
	}
	else
	{
		PIN_REG(addr) &= ~(1 << bitPosi);				                                    // 禁止上升沿
	} 
	
	addr = (u32)(&LPC_GPIOINT->IO0IntEnF) + portNum*PIN_OFFSET;
    if (falEg)
	{
        PIN_REG(addr) |= 1 << bitPosi;				                                        // 使能下降沿
	}
	else
	{
		PIN_REG(addr) &= ~(1 << bitPosi);				                                    // 禁止下降沿
	}     
}

/********************************************************************************************************
* FunctionName   : GPIOIntStatus()
* Description    : 读取管脚中断状态 
* EntryParameter : port num, bit position
* ReturnValue    : 0 - 未中断产生，1 - 上升沿中断，2 - 下降沿中断，3 - 双边沿中断
********************************************************************************************************/
u8 GPIOIntStatus(u8 portNum, u8 bitPosi)
{
    u8 regVal = 0;
	u32 addr;

	if (LPC_GPIOINT->IntStatus & portNum)
	{
	    addr = (u32)(&LPC_GPIOINT->IO0IntStatR) + portNum*PIN_OFFSET;
	    if ((PIN_REG(addr) & (1 << bitPosi)) != 0x00)
		{
		    regVal |= 1 << 1;																// 有上升沿中断产生
		}

		addr = (u32)(&LPC_GPIOINT->IO0IntStatF) + portNum*PIN_OFFSET;
		if ((PIN_REG(addr) & (1 << bitPosi)) != 0x00)
		{
		    regVal |= 1 << 2;																// 有下降沿中断产生
		}
	}

	return (regVal);
}

/********************************************************************************************************
* FunctionName   : GPIOIntClear()
* Description    : 清管脚上的中断
* EntryParameter : port num(0,2), bit position
* ReturnValue    : None
********************************************************************************************************/
void GPIOIntClear(u8 portNum, u8 bitPosi)
{
    u32 addr;

	addr = (u32)(&LPC_GPIOINT->IO0IntClr) + portNum*PIN_OFFSET;
    PIN_REG(addr) |= (1 << bitPosi);
}

/********************************************************************************************************
*                                         End Of File                  
********************************************************************************************************/
