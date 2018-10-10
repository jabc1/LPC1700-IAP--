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
#include "../UserCode/source/config.h"    			                          // 头文件

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/
/* 注意事项：

   这里没有使用LPC_UART1，而是使用的是LPC_UART1M，主要原因是LPC_UART1与其他3个串口有区别，多了个Modem接口。
所以，在头文件里边单独定义了一个LPC_UART1_TypeDef，导致我们无法把LPC_UART1与其他3个串口整合在一起，所以特
别把LPC_UART1也定义成为LPC_UART_TypeDef，使得4个串口代码可以通用，但是这样将无法实现LPC_UART1的特殊功能。
如果要使用LPC_UART1的特殊功能，需要自己重新编写驱动代码。*/

static LPC_UART_TypeDef(* const LPC_UART[4]) = {LPC_UART0, LPC_UART1M, LPC_UART2, LPC_UART3}; 

/********************************************************************************************************
* FunctionName   : UART_Dlab()
* Description    : 访问除数锁存器
* EntryParameter : num-串口(0~3), mode - 访问模式(DISABLE-禁止，ENABLE-使能)
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
* Description    : 波特率设置
* EntryParameter : bps - 波特率
* ReturnValue    : 成功返回TRUE，否则FALSE
********************************************************************************************************/
u8 UART_BaudSet(u8 num, u32 bps)
{
	u32  fDiv;															     // 
	u32  uartClock = SystemCoreClock / 4;								     // UART时钟
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

	if (uartClock % (16*bps) == 0)											 // PCLK / (16*bps)为整数
	{
	    fDiv = uartClock / (16*bps);	                                     // baud rate
		LPC_UART[num]->DLM = fDiv / 256;
	    LPC_UART[num]->DLL = fDiv % 256; 
		LPC_UART[num]->FDR = 0x10;
	}																		 // 关闭小数分频
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
				for (i=0; i<sizeof(tbl)/sizeof(tbl[0]); i++)				 // 从表中查找最接近fFRest的值
				{
				    tAbs = fabs(tbl[i].FR-fFRest);
					if (min > tAbs)
					{
					    k = i;
					    min = tAbs;
					}
				}

				UART_Dlab(num, ENABLE);
			   	LPC_UART[num]->FDR = (tbl[k].Mul << 4) + tbl[k].Div;		// 得出小数分频值
			    LPC_UART[num]->DLM = uDLest / 256;
		        LPC_UART[num]->DLL = uDLest	% 256;
				UART_Dlab(num, DISABLE);

				//sprintf((char *)(tmpStr), "Hello! Baud is %4d bps\n", bps);
				UARTSendStr(num, tmpStr);
				return TRUE;
			}
			else															// 从1.1到1.9中选择其他tFRest的值
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
* Description    : 自动波特率
* EntryParameter : num - 串口(0~3), mode - 模式(0-模式0, 1-模式1, 2-关闭自动波特)
* ReturnValue    : 返回自动识别的波特率
********************************************************************************************************/
u32 UART_AutoBaud(u8 num, u8 mode)
{
    u8  i, k, dlm, dll;
	u32 tmpBaud, tAbs, min;
	u32 uartClock = SystemCoreClock / 4;								         // UART时钟
	u32 baud[] = {300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 43000, 57600, 115200};

    if (mode >= 2)
	{
	    LPC_UART[num]->ACR = (0x00 << ACR_START);							     // 关闭自动波特率
		return 0;
	}
	else
	{
		LPC_UART[num]->ACR = (0x01 << ACR_START) | (mode << ACR_MODE);     	     // 启动自动波特率
		while (LPC_UART[num]->ACR & (0x01 << ACR_START) != 0)
		{
		    ;																     // 等待自动波特率完成
		}

		UART_Dlab(num, ENABLE);
		dlm = LPC_UART[num]->DLM;												 // 读波特率高8位
		dll = LPC_UART[num]->DLL;												 // 读波特率低8位
		tmpBaud = uartClock / (16 * (256*dlm + dll));							 // 计算波特率
		UART_Dlab(num, DISABLE);

		k   = 0;
		min = baud[10];
		for (i=0; i<sizeof(baud); i++)
		{
		    tAbs = abs(baud[i]-tmpBaud);    									 // 选择误差最小的设置
			if (min > tAbs)
			{
			    k = i;
			    min = tAbs;
			}
		}

		return (baud[k]);									                    // 返回设置
	}  
}

/********************************************************************************************************
* FunctionName   : UARTInit()
* Description    : 串口初始化
* EntryParameter : num - 串口(0~3), bps - 波特率(0-时为自动波特率，其他为手动设置), mod - 工作模式， tri - 缓冲触发字节
* ReturnValue    : 成功返回TRUE，否则FALSE
********************************************************************************************************/
u8 UARTInit(u8 num, u32 bps, u8 mod, u8 tri)
{
    u8  value = TRUE;
	u32	tmpBps;

	if (num > 3) 															 // 错误设置
	{
	    return FALSE;
	}

	UART_SetGpio(num);					                                     // 管脚配置

	switch (num)
	{
	    case UART0: 
		{
		    NVIC_DisableIRQ(UART0_IRQn);
		    LPC_SC->PCONP |= (1 << 3);									     // 打开UART0的功率控制位
			break;					         
		}

	    case UART1: 
		{
		    NVIC_DisableIRQ(UART1_IRQn);
		    LPC_SC->PCONP |= (1 << 4);									     // 打开UART1的功率控制位
			break;					         
		}

	    case UART2: 
		{
		    NVIC_DisableIRQ(UART2_IRQn);
		    LPC_SC->PCONP |= (1 << 24);									     // 打开UART2的功率控制位
			break;					         
		}

	    case UART3: 
		{
		    NVIC_DisableIRQ(UART3_IRQn);
		    LPC_SC->PCONP |= (1 << 25);									     // 打开UART3的功率控制位
			break;					         
		}

		default: break;
	}

	LPC_UART[num]->LCR = DAT_BIT8|PARITY_DI|STOP_BIT1;				          // 8 bits, no Parity, 1 Stop bit

	if (bps > 0)
	{
	    tmpBps = bps;										                  // 手动设置波特率 
	}
	else
	{
		tmpBps = UART_AutoBaud(num, 1);										  // 自动波特率
	}

	if (UART_BaudSet(num, tmpBps) == TRUE)									  // 设置波特率
	{				 
		LPC_UART[num]->FCR  = tri|LCR_FIFO_EN|LCR_RX_RST|LCR_TX_RST;		  // Enable and reset TX and RX FIFO. 
		LPC_UART[num]->IER = mod;	                                          // 设置串口工作模式
		if (mod != UART_INQUIRY)
		{
			switch (num)												      // 使能中断
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
* Description    : 串口管脚设置
* EntryParameter : num - 串口选择(0,1,2,3) 
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
* Description    : 发送一字节数据
* EntryParameter : num - 串口选择(0,1,2,3); ch - 要发送的字符
* ReturnValue    : None
********************************************************************************************************/
void UARTSendByte(u8 num, u8 ch)
{
	LPC_UART[num]->THR = ch;				                                 // 写入数据
	while (!(LPC_UART[num]->LSR & LSR_THRE))
	{ 
	    ;	                                                                 // 等待数据发送完毕
	}
}

/********************************************************************************************************
* FunctionName   : UARTSendStr()
* Description    : 发送字符串
* EntryParameter : num - 串口选择(0,1,2,3); pStr - 发送缓冲
* ReturnValue    : None
********************************************************************************************************/
void UARTSendStr(u8 num, u8 *pStr)
{
    while (*pStr != '\0')
	{
        UARTSendByte(num, *pStr++);							                 // 发送字符串
    }
}

/********************************************************************************************************
* FunctionName   : UARTSendMultiByte()
* Description    : 发送多字节数据
* EntryParameter : num - 串口选择(0,1,2,3); pStr - 发送缓冲; length - 发送数据长度
* ReturnValue    : None
********************************************************************************************************/
void UARTSendMultiByte(u8 num, u8 *pStr, u32 length)
{
    while (length--)
	{
        UARTSendByte(num, *pStr++);							                 // 发送字符串
    }
}

/********************************************************************************************************
* FunctionName   : UARTReceiveByte()
* Description    : 接收一字节数据
* EntryParameter : num - 串口选择(0,1,2,3)
* ReturnValue    : None
********************************************************************************************************/
u8 UARTReceiveByte(u8 num)
{
	u8 rcvData = 0;

    while (!(LPC_UART[num]->LSR & LSR_RDR)) 
	{
	    ;	                                                                 // 查询数据是否接收完毕
	}
	
	rcvData = LPC_UART[num]->RBR;                                            // 接收数据
	return (rcvData);										                 // 返回接收数据
}

/********************************************************************************************************
* FunctionName   : UARTReceiveMultiByte()
* Description    : 数据接收
* EntryParameter : num - 串口选择(0,1,2,3); buf - 数据缓冲; length - 接收个数
* ReturnValue    : None
********************************************************************************************************/
void UARTReceiveMultiByte(u8 num, u8 *buf, u32 length)
{
	while (length--)
	{
	    *buf++ = UARTReceiveByte(num);                                       // 把数据放入缓冲
	}
}

/********************************************************************************************************
* End Of File                  
********************************************************************************************************/
