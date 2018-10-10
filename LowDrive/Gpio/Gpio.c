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
*				   ����I/O���ڸ�λ��Ĭ��Ϊ�������룬PORT0��PORT2�˿ڵ�ÿ�����Ŷ������ṩ�жϹ���
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "../UserCode/source/config.h"    			                                       // ͷ�ļ�

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
static LPC_GPIO_TypeDef(* const LPC_GPIO[5]) = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4};

/********************************************************************************************************
* FunctionName   : GPIOInit()
* Description    : ��ʼ��GPIO
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void GPIOInit(void)
{
    u8 i;

	for (i=0; i<5; i++)
	{
	    LPC_GPIO[i]->FIOMASK = 0x00;			             // ���������Ÿ��ٹ���
	}
}

/********************************************************************************************************
* FunctionName   : GPIOSetMode()
* Description    : ���ùܽ�ģʽ
* EntryParameter : portNum-�˿ڣ�bitPosi-λ��fun-���ܣ�mode-ģʽ(��,����)��OpenDrain-��©
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetMode(u8 portNum, u8 bitPosi, u8 fun, u8 mode, u8 OpenDrain)
{
    u32 addr;

    if (bitPosi < 16)
    {
	    addr = (u32)(&LPC_PINCON->PINSEL0) + portNum*2*PIN_OFFSET;
        PIN_REG(addr) |= (fun << bitPosi*2);		                                        // �ܽŹ�������
		addr = (u32)(&LPC_PINCON->PINMODE0) + portNum*2*PIN_OFFSET;
		PIN_REG(addr) |= (mode << bitPosi*2);		                                        // �ܽ�ģʽ����
    }
	else
	{
	    addr = (u32)(&LPC_PINCON->PINSEL1) + portNum*2*PIN_OFFSET;
        PIN_REG(addr)  |= (fun << (bitPosi-16)*2);
		addr = (u32)(&LPC_PINCON->PINMODE1) + portNum*2*PIN_OFFSET;
		PIN_REG(addr) |= (mode << bitPosi*2);		                                        // ����������
	}

	addr = (u32)(&LPC_PINCON->PINMODE_OD0) + portNum*2*PIN_OFFSET;
	PIN_REG(addr) |= (OpenDrain << bitPosi);	                                            // ��©ģʽ
}

/********************************************************************************************************
* FunctionName   : GPIOSetDir()
* Description    : �˿ڷ������ã�ֻ��GPIO��Ч�����������Զ����Ʒ���(Ĭ����������)
* EntryParameter : port num, bit position, direction (1 out, 0 input)
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetDir(u8 portNum, u8 bitPosi, u8 dir)
{
    if (dir)
	{																					 
	    LPC_GPIO[portNum]->FIODIR |= 1<<bitPosi;											// ����Ϊ���
	}
	else
	{																						 
		LPC_GPIO[portNum]->FIODIR &= ~(1<<bitPosi);											// ����Ϊ����
	}
} 

/********************************************************************************************************
* FunctionName   : GPIOSetMultiDir()
* Description    : �˿ڷ������ã�ֻ��GPIO��Ч�����������Զ����Ʒ���(Ĭ����������)
* EntryParameter : port num, bit position, direction (1 out, 0 input)
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetMultiDir(u8 portNum, u32 mBitPosi, u8 dir)
{
    if (dir)
	{																					 
	    LPC_GPIO[portNum]->FIODIR |= mBitPosi;											    // ����Ϊ���
	}
	else
	{																						 
		LPC_GPIO[portNum]->FIODIR &= ~(mBitPosi);											// ����Ϊ����
	}
}

/********************************************************************************************************
* FunctionName   : GPIOSetValue()
* Description    : д�˿�����(��1����0��ͨ�������Ĵ���ʵ�ֵ�)
* EntryParameter : port num, bit position, bit value
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetValue(u8 portNum, u8 bitPosi, u8 bitVal)
{
    if (bitVal)
	{
        LPC_GPIO[portNum]->FIOSET = (1 << bitPosi);											// ��1
	}
	else
	{
		LPC_GPIO[portNum]->FIOCLR = (1 << bitPosi);											// ��0
	}
}

/********************************************************************************************************
* FunctionName   : GPIOSetMultiValue()
* Description    : ���ö�λֵ
* EntryParameter : portNum - �˿ڣ�mBitPosi - ���ݶ˿ڣ�val - д��ֵ
* ReturnValue    : None
********************************************************************************************************/
void GPIOSetMultiValue(u8 portNum, u32 mBitPosi, u32 val)
{
    LPC_GPIO[portNum]->FIOCLR = mBitPosi;					                                // ����0
	LPC_GPIO[portNum]->FIOSET = val;					                                    // ��д1    
}

/********************************************************************************************************
* FunctionName   : GPIOStatusValue()
* Description    : ���˿�����
* EntryParameter : port num, bit position
* ReturnValue    : ���˿�ֵ������1��0
********************************************************************************************************/
u8 GPIOStatusValue(u8 portNum, u8 bitPosi)
{
	return (((LPC_GPIO[portNum]->FIOPIN) & (1 << bitPosi)) != 0x00)	?  1 : 0;				// ���˿ڣ�������ֵ
}

/********************************************************************************************************
* FunctionName   : GPIOStatusMultiValue()
* Description    : �����ж˿�����
* EntryParameter : port num, shield - ���ε�ַ(1-ΪҪ��ȡλ��0-Ϊ����λ)
* ReturnValue    : ���˿�ֵ������1��0
********************************************************************************************************/
u32 GPIOStatusMultiValue(u8 portNum, u32 shield)
{
	LPC_GPIO[portNum]->FIOMASK = ~shield;													// д������ֵ
    return (LPC_GPIO[portNum]->FIOPIN); 	                                                // ���˿�
}

/********************************************************************************************************
* FunctionName   : GPIOIntSet()
* Description    : ���ùܽ��ж�(ֻ�����ñ��ش�������������Ϊ��ƽ����)
* EntryParameter : port num, bit position, rising edge, falling edge
* ReturnValue    : None
********************************************************************************************************/
void GPIOIntSet(u8 portNum, u8 bitPosi, u8 risEg, u8 falEg)
{
    u32 addr;

	addr = (u32)(&LPC_GPIOINT->IO0IntEnR) + portNum*PIN_OFFSET;
    if (risEg)
	{ 
        PIN_REG(addr) |= 1 << bitPosi;				                                        // ʹ��������
	}
	else
	{
		PIN_REG(addr) &= ~(1 << bitPosi);				                                    // ��ֹ������
	} 
	
	addr = (u32)(&LPC_GPIOINT->IO0IntEnF) + portNum*PIN_OFFSET;
    if (falEg)
	{
        PIN_REG(addr) |= 1 << bitPosi;				                                        // ʹ���½���
	}
	else
	{
		PIN_REG(addr) &= ~(1 << bitPosi);				                                    // ��ֹ�½���
	}     
}

/********************************************************************************************************
* FunctionName   : GPIOIntStatus()
* Description    : ��ȡ�ܽ��ж�״̬ 
* EntryParameter : port num, bit position
* ReturnValue    : 0 - δ�жϲ�����1 - �������жϣ�2 - �½����жϣ�3 - ˫�����ж�
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
		    regVal |= 1 << 1;																// ���������жϲ���
		}

		addr = (u32)(&LPC_GPIOINT->IO0IntStatF) + portNum*PIN_OFFSET;
		if ((PIN_REG(addr) & (1 << bitPosi)) != 0x00)
		{
		    regVal |= 1 << 2;																// ���½����жϲ���
		}
	}

	return (regVal);
}

/********************************************************************************************************
* FunctionName   : GPIOIntClear()
* Description    : ��ܽ��ϵ��ж�
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
