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
#include "../UserCode/source/config.h"    			                                 // ͷ�ļ�

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
static LPC_I2C_TypeDef(* const LPC_I2C[3]) = {LPC_I2C0, LPC_I2C1, LPC_I2C2};   	    // ���߶���

/********************************************************************************************************
* FunctionName   : I2C_DelayMs()
* Description    : ��ʱ����
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
* Description    : ��ʼ��I2C
* EntryParameter : num - ����ѡ��(0,1,2); fi2c - ��ʼ��I2C�������ʣ����ֵΪ400K
* ReturnValue    : �ɹ�����0�����򷵻�1
********************************************************************************************************/
u8 I2CInit(u8 num, u32 fi2c)
{
    u32 i2cClock = SystemCoreClock / 4;										         // I2Cʱ��

	if (num > 3)																	 // ��������
	{
	    return (1);
	}

	if (fi2c > 400000)
	{
	    fi2c = 400000;
	} 

	I2C_SetGpio(num);											                     // GPIO����

	switch (num)																	 // �򿪵�Դ
	{
	    case I2C0: LPC_SC->PCONP |= (1 << 7);  break;
	    case I2C1: LPC_SC->PCONP |= (1 << 19); break;	   
		case I2C2: LPC_SC->PCONP |= (1 << 26); break;
		default  : break;
	}

	LPC_I2C[num]->I2CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;
	LPC_I2C[num]->I2SCLH = (i2cClock / fi2c + 1) / 2;								// ����I2Cʱ��Ϊfi2c
	LPC_I2C[num]->I2SCLL = (i2cClock / fi2c) / 2;
    LPC_I2C[num]->I2CONSET = I2CONSET_I2EN;                                         // ʹ����I2C
	 
	I2C_Stop(num);

	return (0);
}

/********************************************************************************************************
* FunctionName   : I2C_SetGpio()
* Description    : �˿�����
* EntryParameter : num - ����ѡ��(0,1,2)
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
* Description    : ����
* EntryParameter : num - ����ѡ��(0,1,2)
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

	LPC_I2C[num]->I2CONCLR = I2CONCLR_STAC;                                          // ���SI��STAλ,��������
}

/********************************************************************************************************
* FunctionName   : I2C_Stop()
* Description    : ֹͣ
* EntryParameter : num - ����ѡ��(0,1,2)
* ReturnValue    : None
********************************************************************************************************/
void I2C_Stop(u8 num)
{
	LPC_I2C[num]->I2CONSET = I2CONSET_STO;                                           // ����ֹͣ��־ 
	LPC_I2C[num]->I2CONCLR = I2CONCLR_SIC;                                           // ���SI��־  
	
	while (LPC_I2C[num]->I2CONSET & I2CONSET_STO)									 // �ȴ�ֹͣ�ź�
	{
	    ;
	}
}

/********************************************************************************************************
* FunctionName   : I2C_WriteByte()
* Description    : дһ�ֽ�
* EntryParameter : num - ����ѡ��(0,1,2)��dat - Ҫ���͵����ݻ��ַ��mode��0-д��ַ��1-����ַ��2-�ڲ���ַ������
* ReturnValue    : ���ز������
********************************************************************************************************/
u8 I2C_WriteByte(u8 num, u8 dat, u8 mode)
{
    u8 ack = 0;
	
    LPC_I2C[num]->I2DAT = (mode != 2) ?  (dat + mode) : dat;						 // icAddr + ���0 - write��1 - read��, �����ڲ���ַ������
    LPC_I2C[num]->I2CONCLR = I2CONCLR_SIC;                                           // ��������

	switch (mode)
	{
	    case 0:  while (LPC_I2C[num]->I2STAT != 0x18); ack = 0; break;	             // �ѷ���SLA+W���ѽ���ACK
		case 1:  while (LPC_I2C[num]->I2STAT != 0x40); ack = 0; break;	             // �ѷ���SLA+R���ѽ���ACK
		case 2:  while (LPC_I2C[num]->I2STAT != 0x28); ack = 0; break;	             // �ѷ���I2DAT�е������ֽڣ��ѽ���ACK
		default: ack = 1; break;
	}

    return ack;
}

/********************************************************************************************************
* FunctionName   : I2C_ReadByte()
* Description    : ��һ�ֽ�
* EntryParameter : num - ����ѡ��(0,1,2)��last - ���һλ���ݱ�־
* ReturnValue    : ��ȡ����
********************************************************************************************************/
u8 I2C_ReadByte(u8 num, u8 last)
{
	u8  rValue = 0;

    if (last == 0)
	{
	    LPC_I2C[num]->I2CONSET = I2CONSET_AA;			                            // ����Ӧ��
	}
	else
	{
	    LPC_I2C[num]->I2CONCLR = I2CONCLR_AAC;			                            // ������Ӧ��
	}

	LPC_I2C[num]->I2CONCLR = I2CONCLR_SIC;                                          // ������������

	if (1 == last)							
	{
	    while (LPC_I2C[num]->I2STAT != 0x58) ;			                            // ���һλ���ݣ�������Ӧ��
	}
	else									
	{
		while (LPC_I2C[num]->I2STAT != 0x50) ;			                            // �������һλ������Ӧ��
	}

	rValue = LPC_I2C[num]->I2DAT;						                            // ���ؽ��յ���

	return 	rValue;
}

/********************************************************************************************************
* FunctionName   : I2CWriteByte()
* Description    : д��һ�ֽڵ�EEPROM
* EntryParameter : num - ����ѡ��(0,1,2)��icAddr - ������ַ��type - �ڲ���ַ���ͣ�addr - �ڲ���ַ��dat - ����
* ReturnValue    : None
********************************************************************************************************/
void I2CWriteByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 dat)
{
	I2C_Start(num);							                                        // ������ʼ�ź�
	I2C_WriteByte(num, icAddr, 0);                                                  // д��ַ��дģʽ
	if (type == 0x01)					                                            // 0x00 - û���ڲ���ַ
	{
		I2C_WriteByte(num, addr, 2);                                                // IC�ڲ���ַ��8λ
	}

	I2C_WriteByte(num, dat, 2);				                                        // ����д��
	I2C_Stop(num);							                                        // ����ֹͣ�ź�
	I2C_DelayMs(10);                                                                // ��ʱ10ms
}

/********************************************************************************************************
* FunctionName   : I2CReadByte()
* Description    : ��ȡһ�ֽ�
* EntryParameter : num - ����ѡ��(0,1,2)��icAddr - ������ַ��type - �ڲ���ַ���ͣ�addr - �ڲ���ַ
* ReturnValue    : ���ض�ȡ����
********************************************************************************************************/
u8 I2CReadByte(u8 num, u8 icAddr, u8 type, u8 addr)
{
	u8 dat = 0;
	
	if (type == 0x01)						                                        // 0x00 - û���ڲ���ַ
	{
		I2C_Start(num);						                                        // ������ʼ�ź�
		I2C_WriteByte(num, icAddr, 0);		                                        // д���ַ
		I2C_WriteByte(num, addr, 2);                                                // IC�ڲ���ַ��8λ
		I2C_Stop(num);						                                        // ��ַ����
	}
	
	I2C_DelayMs(180);						                                        // IC������ʱ�ͻ�����д����ַ�ĵط�
	I2C_Start(num);							                                        // ������ʼ�ź�
	I2C_WriteByte(num, icAddr, 1);			                                        // ���Ͷ�ȡ��ַ
	dat = I2C_ReadByte(num, 1);				                                        // ��ȡ����
	I2C_Stop(num);							                                        // ����

	return dat;								                                        // ��������
}

/********************************************************************************************************
* FunctionName   : I2CWriteNByte()
* Description    : д����ֽڵ�IC
* EntryParameter : num - ����ѡ��(0,1,2)��icAddr - ������ַ��type - IC�ڲ���ַ���ͣ�addr - �ڲ���ַ��*s - �ַ�����num - ����
* ReturnValue    : None
********************************************************************************************************/
void I2CWriteNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 n)
{
	u32 i;

	I2C_Start(num);							                                        // ������ʼ�ź�
	I2C_WriteByte(num, icAddr, 0);                                                  // д��ַ��дģʽ
	if (type == 0x01)					                                            // 0x00 - û���ڲ���ַ
	{
	    I2C_WriteByte(num, addr, 2);                                                // IC�ڲ���ַ
	}

	for (i=0; i<n; i++)
	{
	    I2C_WriteByte(num, *s++, 2);		                                        // ����д��
	}

	I2C_Stop(num);							                                        // ���ݶ��꣬����ֹͣ�ź�
	I2C_DelayMs(10);                                                                // ��ʱ10ms
}

/********************************************************************************************************
* FunctionName   : I2CReadNByte()
* Description    : ��IC��ȡ���ֽ�
* EntryParameter : num - ����ѡ��(0,1,2)��icAddr - ������ַ��type - IC�ڲ���ַ���ͣ�addr - �ڲ���ַ��*s - �ַ�����n - ����
* ReturnValue    : None
********************************************************************************************************/
void I2CReadNByte(u8 num, u8 icAddr, u8 type, u8 addr, u8 *s, u32 n)
{
	u32 i;
	
	if (type == 0x01)						                                        // 0x00 - û���ڲ���ַ
	{
		I2C_Start(num);						                                        // ������ʼ�ź�
		I2C_DelayMs(180);
		I2C_WriteByte(num, icAddr, 0);		                                        // д���ַ
		I2C_WriteByte(num, addr, 2);		                                        // IC�ڲ���ַ
		I2C_Stop(num);						                                        // ��ַ����
	}
	
	I2C_Start(num);							                                        // ������ʼ�ź�
	I2C_WriteByte(num, icAddr, 1);			                                        // ���Ͷ�ȡ��ַ
	for (i=0; i<n-1; i++)
	{
	    *s++ = I2C_ReadByte(num, 0);		                                        // ��ȡ����
	}

	*s = I2C_ReadByte(num, 1);				                                        // �����һ������
	I2C_Stop(num);							                                        // ֹͣ
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
