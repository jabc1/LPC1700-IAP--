/******************************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : uart.c
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
#include "../UserCode/source/config.h"    			                          // ͷ�ļ�

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
/* ע�����

   ����û��ʹ��LPC_UART1������ʹ�õ���LPC_UART1M����Ҫԭ����LPC_UART1������3�����������𣬶��˸�Modem�ӿڡ�
���ԣ���ͷ�ļ���ߵ���������һ��LPC_UART1_TypeDef�����������޷���LPC_UART1������3������������һ��������
���LPC_UART1Ҳ�����ΪLPC_UART_TypeDef��ʹ��4�����ڴ������ͨ�ã������������޷�ʵ��LPC_UART1�����⹦�ܡ�
���Ҫʹ��LPC_UART1�����⹦�ܣ���Ҫ�Լ����±�д�������롣*/

static LPC_UART_TypeDef(* const LPC_UART[4]) = {LPC_UART0, LPC_UART1M, LPC_UART2, LPC_UART3}; 

/********************************************************************************************************
* FunctionName   : UART_Dlab()
* Description    : ���ʳ���������
* EntryParameter : num-����(0~3), mode - ����ģʽ(DISABLE-��ֹ��ENABLE-ʹ��)
* ReturnValue    : None
********************************************************************************************************/
void UART_Dlab(u8 num, u8 mode)
{
    if (mode == ENABLE)
	{
        LPC_UART[num]->LCR |= LCR_DLAB;
	} 
	else
	{
	    LPC_UART[num]->LCR &= ~LCR_DLAB;
	}    
}

/********************************************************************************************************
* FunctionName   : UART_BaudSet()
* Description    : ����������
* EntryParameter : bps - ������
* ReturnValue    : �ɹ�����TRUE������FALSE
********************************************************************************************************/
u8 UART_BaudSet(u8 num, u32 bps)
{
	u32  fDiv;															     // 
	u32  uartClock = SystemCoreClock / 4;								     // UARTʱ��
	fp32 fFRest = 1.5, tFRest = 1.5, tAbs, min; 
	u8   i, k;
	u32  uDLest;
	u8   tmpStr[64];

	UART_DCM_TBL const tbl[] = 
	{
	    {1.000,  0,  1}, {1.067,  1, 15}, {1.071,  1, 14}, {1.077,  1, 13},
		{1.083,	 1, 12}, {1.091,  1, 11}, {1.100,  1, 10}, {1.111,  1,  9},
		{1.125,  1,  8}, {1.133,  2, 15}, {1.143,  1,  7}, {1.154,  2, 13},
		{1.167,  1,  6}, {1.182,  2, 11}, {1.200,  1,  5}, {1.214,  3, 14},
		{1.222,  2,  9}, {1.231,  3, 13}, {1.250,  1,  4}, {1.267,  4, 15},
		{1.273,  3, 11}, {1.286,  2,  7}, {1.300,  3, 10}, {1.308,  4, 13},
		{1.333,  1,  3}, {1.357,  5, 14}, {1.364,  4, 11}, {1.375,  3,  8},
		{1.385,  5, 13}, {1.400,  2,  5}, {1.417,  5, 12}, {1.429,  3,  7},
		{1.444,  4,  9}, {1.455,  5, 11}, {1.462,  6, 13}, {1.467,  7, 15},
		{1.500,  1,  2}, {1.533,  8, 15}, {1.538,  7, 13}, {1.545,  6, 11},
		{1.556,	 5,  9}, {1.571,  4,  7}, {1.583,  7, 12}, {1.600,  3,  5},
		{1.615,  8, 13}, {1.625,  5,  8}, {1.636,  7, 11}, {1.643,  9, 14},
		{1.667,  2,  3}, {1.692,  9, 13}, {1.700,  7, 10}, {1.714,  5,  7},
		{1.727,  8, 11}, {1.733, 11, 15}, {1.750,  3,  4}, {1.769, 10, 13},
		{1.778,  7,  9}, {1.786, 11, 14}, {1.800,  4,  5}, {1.818,  9, 11},
		{1.833,  5,  6}, {1.846, 11, 13}, {1.857,  6,  7}, {1.867, 13, 15},
		{1.875,  7,  8}, {1.889,  8,  9}, {1.900,  9, 10}, {1.909, 10, 11},
		{1.917, 11, 12}, {1.923, 12, 13}, {1.929, 13, 14}, {1.933, 14, 15},
	};

	if (uartClock % (16*bps) == 0)											 // PCLK / (16*bps)Ϊ����
	{
	    fDiv = uartClock / (16*bps);	                                     // baud rate
		LPC_UART[num]->DLM = fDiv / 256;
	    LPC_UART[num]->DLL = fDiv % 256; 
		LPC_UART[num]->FDR = 0x10;
	}																		 // �ر�С����Ƶ
	else
	{ 
	    do
		{
		    uDLest = (u32)(uartClock / (16*bps*fFRest));
			fFRest = (fp32)uartClock / (fp32)(16*bps*uDLest);
			if ((1.100<fFRest) && (fFRest<1.900))
			{	
			    k = 0;
				min = tbl[0].FR;
				for (i=0; i<sizeof(tbl)/sizeof(tbl[0]); i++)				 // �ӱ��в�����ӽ�fFRest��ֵ
				{
				    tAbs = fabs(tbl[i].FR-fFRest);
					if (min > tAbs)
					{
					    k = i;
					    min = tAbs;
					}
				}

				UART_Dlab(num, ENABLE);
			   	LPC_UART[num]->FDR = (tbl[k].Mul << 4) + tbl[k].Div;		// �ó�С����Ƶֵ
			    LPC_UART[num]->DLM = uDLest / 256;
		        LPC_UART[num]->DLL = uDLest	% 256;
				UART_Dlab(num, DISABLE);

				//sprintf((char *)(tmpStr), "Hello! Baud is %4d bps\n", bps);
				UARTSendStr(num, tmpStr);
				return TRUE;
			}
			else															// ��1.1��1.9��ѡ������tFRest��ֵ
			{
			    if (fFRest <= 1.1)
				{
				    tFRest += 0.1;
			        fFRest = tFRest;
				}
				else
				{
					tFRest -= 0.1;
			        fFRest = tFRest;
				}				  
			}     

		} while ((1.1<=fFRest) && (fFRest<=1.9));
	}

    return FALSE;
}

/********************************************************************************************************
* FunctionName   : UART_AutoBaud()
* Description    : �Զ�������
* EntryParameter : num - ����(0~3), mode - ģʽ(0-ģʽ0, 1-ģʽ1, 2-�ر��Զ�����)
* ReturnValue    : �����Զ�ʶ��Ĳ�����
********************************************************************************************************/
u32 UART_AutoBaud(u8 num, u8 mode)
{
    u8  i, k, dlm, dll;
	u32 tmpBaud, tAbs, min;
	u32 uartClock = SystemCoreClock / 4;								         // UARTʱ��
	u32 baud[] = {300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 43000, 57600, 115200};

    if (mode >= 2)
	{
	    LPC_UART[num]->ACR = (0x00 << ACR_START);							     // �ر��Զ�������
		return 0;
	}
	else
	{
		LPC_UART[num]->ACR = (0x01 << ACR_START) | (mode << ACR_MODE);     	     // �����Զ�������
		while (LPC_UART[num]->ACR & (0x01 << ACR_START) != 0)
		{
		    ;																     // �ȴ��Զ����������
		}

		UART_Dlab(num, ENABLE);
		dlm = LPC_UART[num]->DLM;												 // �������ʸ�8λ
		dll = LPC_UART[num]->DLL;												 // �������ʵ�8λ
		tmpBaud = uartClock / (16 * (256*dlm + dll));							 // ���㲨����
		UART_Dlab(num, DISABLE);

		k   = 0;
		min = baud[10];
		for (i=0; i<sizeof(baud); i++)
		{
		    tAbs = abs(baud[i]-tmpBaud);    									 // ѡ�������С������
			if (min > tAbs)
			{
			    k = i;
			    min = tAbs;
			}
		}

		return (baud[k]);									                    // ��������
	}  
}

/********************************************************************************************************
* FunctionName   : UARTInit()
* Description    : ���ڳ�ʼ��
* EntryParameter : num - ����(0~3), bps - ������(0-ʱΪ�Զ������ʣ�����Ϊ�ֶ�����), mod - ����ģʽ�� tri - ���崥���ֽ�
* ReturnValue    : �ɹ�����TRUE������FALSE
********************************************************************************************************/
u8 UARTInit(u8 num, u32 bps, u8 mod, u8 tri)
{
    u8  value = TRUE;
	u32	tmpBps;

	if (num > 3) 															 // ��������
	{
	    return FALSE;
	}

	UART_SetGpio(num);					                                     // �ܽ�����

	switch (num)
	{
	    case UART0: 
		{
		    NVIC_DisableIRQ(UART0_IRQn);
		    LPC_SC->PCONP |= (1 << 3);									     // ��UART0�Ĺ��ʿ���λ
			break;					         
		}

	    case UART1: 
		{
		    NVIC_DisableIRQ(UART1_IRQn);
		    LPC_SC->PCONP |= (1 << 4);									     // ��UART1�Ĺ��ʿ���λ
			break;					         
		}

	    case UART2: 
		{
		    NVIC_DisableIRQ(UART2_IRQn);
		    LPC_SC->PCONP |= (1 << 24);									     // ��UART2�Ĺ��ʿ���λ
			break;					         
		}

	    case UART3: 
		{
		    NVIC_DisableIRQ(UART3_IRQn);
		    LPC_SC->PCONP |= (1 << 25);									     // ��UART3�Ĺ��ʿ���λ
			break;					         
		}

		default: break;
	}

	LPC_UART[num]->LCR = DAT_BIT8|PARITY_DI|STOP_BIT1;				          // 8 bits, no Parity, 1 Stop bit

	if (bps > 0)
	{
	    tmpBps = bps;										                  // �ֶ����ò����� 
	}
	else
	{
		tmpBps = UART_AutoBaud(num, 1);										  // �Զ�������
	}

	if (UART_BaudSet(num, tmpBps) == TRUE)									  // ���ò�����
	{				 
		LPC_UART[num]->FCR  = tri|LCR_FIFO_EN|LCR_RX_RST|LCR_TX_RST;		  // Enable and reset TX and RX FIFO. 
		LPC_UART[num]->IER = mod;	                                          // ���ô��ڹ���ģʽ
		if (mod != UART_INQUIRY)
		{
			switch (num)												      // ʹ���ж�
			{
			    case UART0: NVIC_EnableIRQ(UART0_IRQn); break;
			    case UART1: NVIC_EnableIRQ(UART1_IRQn); break;	
			    case UART2: NVIC_EnableIRQ(UART2_IRQn); break;
			    case UART3: NVIC_EnableIRQ(UART3_IRQn); break;
				default: break;
			}	
		}
	} 
	else
	{
	    value = FALSE;
	} 
	
	return value;                                         
}

/********************************************************************************************************
* FunctionName   : UART_SetGpio()
* Description    : ���ڹܽ�����
* EntryParameter : num - ����ѡ��(0,1,2,3) 
* ReturnValue    : None
********************************************************************************************************/
void UART_SetGpio(u8 num)
{
    switch (num)
	{
	    case UART0:
		{   
		    GPIOSetMode(UART_TXD0_PORT, UART_TXD0_PIN, UART_TXD0_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
            GPIOSetMode(UART_RXD0_PORT, UART_RXD0_PIN, UART_RXD0_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
			break;
		}

		case UART1: 
		{  
		    GPIOSetMode(UART_TXD1_PORT, UART_TXD1_PIN, UART_TXD1_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
            GPIOSetMode(UART_RXD1_PORT, UART_RXD1_PIN, UART_RXD1_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
			break;
		}

		case UART2:
		{   
		    GPIOSetMode(UART_TXD2_PORT, UART_TXD2_PIN, UART_TXD2_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
            GPIOSetMode(UART_RXD2_PORT, UART_RXD2_PIN, UART_RXD2_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
			break;
		}

		case UART3:
		{   
		    GPIOSetMode(UART_TXD3_PORT, UART_TXD3_PIN, UART_TXD3_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
            GPIOSetMode(UART_RXD3_PORT, UART_RXD3_PIN, UART_RXD3_FUN, GPIO_PULL_UP, GPIO_TPIU_DI);
			break;
		}

		default: break;
	}
}

/********************************************************************************************************
* FunctionName   : UARTSendByte()
* Description    : ����һ�ֽ�����
* EntryParameter : num - ����ѡ��(0,1,2,3); ch - Ҫ���͵��ַ�
* ReturnValue    : None
********************************************************************************************************/
void UARTSendByte(u8 num, u8 ch)
{
	LPC_UART[num]->THR = ch;				                                 // д������
	while (!(LPC_UART[num]->LSR & LSR_THRE))
	{ 
	    ;	                                                                 // �ȴ����ݷ������
	}
}

/********************************************************************************************************
* FunctionName   : UARTSendStr()
* Description    : �����ַ���
* EntryParameter : num - ����ѡ��(0,1,2,3); pStr - ���ͻ���
* ReturnValue    : None
********************************************************************************************************/
void UARTSendStr(u8 num, u8 *pStr)
{
    while (*pStr != '\0')
	{
        UARTSendByte(num, *pStr++);							                 // �����ַ���
    }
}

/********************************************************************************************************
* FunctionName   : UARTSendMultiByte()
* Description    : ���Ͷ��ֽ�����
* EntryParameter : num - ����ѡ��(0,1,2,3); pStr - ���ͻ���; length - �������ݳ���
* ReturnValue    : None
********************************************************************************************************/
void UARTSendMultiByte(u8 num, u8 *pStr, u32 length)
{
    while (length--)
	{
        UARTSendByte(num, *pStr++);							                 // �����ַ���
    }
}

/********************************************************************************************************
* FunctionName   : UARTReceiveByte()
* Description    : ����һ�ֽ�����
* EntryParameter : num - ����ѡ��(0,1,2,3)
* ReturnValue    : None
********************************************************************************************************/
u8 UARTReceiveByte(u8 num)
{
	u8 rcvData = 0;

    while (!(LPC_UART[num]->LSR & LSR_RDR)) 
	{
	    ;	                                                                 // ��ѯ�����Ƿ�������
	}
	
	rcvData = LPC_UART[num]->RBR;                                            // ��������
	return (rcvData);										                 // ���ؽ�������
}

/********************************************************************************************************
* FunctionName   : UARTReceiveMultiByte()
* Description    : ���ݽ���
* EntryParameter : num - ����ѡ��(0,1,2,3); buf - ���ݻ���; length - ���ո���
* ReturnValue    : None
********************************************************************************************************/
void UARTReceiveMultiByte(u8 num, u8 *buf, u32 length)
{
	while (length--)
	{
	    *buf++ = UARTReceiveByte(num);                                       // �����ݷ��뻺��
	}
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
