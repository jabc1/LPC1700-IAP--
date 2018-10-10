/********************************************************************************************************    
*                                                                      
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : st7920.c
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
*                                  HeaderFiles                                                  
********************************************************************************************************/
#include "../UserCode/source/config.h" 

/********************************************************************************************************
* global variable                                              
********************************************************************************************************/

/********************************************************************************************************
* FunctionName   : LCM_Delay()
* Description    : 延时
* EntryParameter : us - us延时
* ReturnValue    : None
********************************************************************************************************/
void LCM_Delay(u16 us)
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
* FunctionName   : LCM_SetGpio()
* Description    : 端口设置为GPIO，并设置为输出端口
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCM_SetGpio(void)
{
	LCM_DAT_OUT();						                                    // 数据端口方向设置

	LCM_EN_OUT();			                                                // 使能
	LCM_RW_OUT();			                                                // 读写
	LCM_RS_OUT();			                                                // 数据/指令选择
	LCM_BK_OUT();			                                                // 背光
	LCM_PS_OUT();			                                                // PSB
	LCM_RT_OUT();			                                                // 复位
}

/********************************************************************************************************
* FunctionName   : LCMBacklightCtrl()
* Description    : 背光开关
* EntryParameter : blk - 1开，0关
* ReturnValue    : None
********************************************************************************************************/
void LCMBacklightCtrl(u8 blk)
{
    LCM_BK_WRITE(blk);
}

/********************************************************************************************************
* FunctionName   : LCM_CommMode()
* Description    : 通讯模式
* EntryParameter : mode - 通讯模式(0-串行，1-并行)
* ReturnValue    : None
********************************************************************************************************/
void LCM_CommMode(u8 mode)
{
    LCM_PS_WRITE(mode);
}

/********************************************************************************************************
* FunctionName   : LCM_Reset()
* Description    : 复位LCM
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCM_Reset(void)
{
    LCM_RT_WRITE(GPIO_CLR);
	LCM_Delay(50);
	LCM_RT_WRITE(GPIO_SET); 
	LCM_Delay(200);
}

/********************************************************************************************************
* FunctionName   : LCM_WriteCmd()
* Description    : 向LCM中写入指令
* EntryParameter : cmd - 指令
* ReturnValue    : None
********************************************************************************************************/
void LCM_WriteCmd(u8 cmd)
{
	LCM_Delay(10);                                                           // 没有忙状态判断标志，需延时
	LCM_DAT_WRITE(cmd);                                                      // 写入指令到LCM

    LCM_RS_WRITE(GPIO_CLR);			                                         // 选择指令
	LCM_RW_WRITE(GPIO_CLR);			                                         // 写数据
	LCM_EN_WRITE(GPIO_CLR);						                             // 若晶振速度太高可以在这后加小的延时
	LCM_Delay(50);
	LCM_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : LCM_WriteData()
* Description    : 向LCM1602中写入数据
* EntryParameter : dat - 数据
* ReturnValue    : None
********************************************************************************************************/
void LCM_WriteData(u8 dat)
{
	LCM_Delay(10);                                                           // 没有忙状态判断标志，需延时         
	LCM_DAT_WRITE(dat);                                                      // 写入数据到LCM

	LCM_RS_WRITE(GPIO_SET);			                                         // 选择数据
	LCM_RW_WRITE(GPIO_CLR);			                                         // 写数据
	LCM_EN_WRITE(GPIO_CLR);						                             // 若晶振速度太高可以在这后加小的延时
	LCM_Delay(50);
	LCM_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : LCMInit()
* Description    : LCM在工作前先要对显示屏初始化,否则模块无法正常工作
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCMInit(void)
{
    LCM_SetGpio();					                                         // 方向控制
	LCM_Reset();														     // 复位

    LCM_CommMode(LCM_PSB_PRL);  	                                         // 选择并行传输方式
    LCM_Delay(500); 

    LCM_WriteCmd(LCM_BASIC_INS);  			                                 // 选择基本指令集
    LCM_Delay(250);        
    LCM_WriteCmd(LCM_DISPLAY_ON);  		  	                                 // 开显示
    LCM_Delay(250);
    LCM_WriteCmd(LCM_CLR_SCREEN);  			                                 // 清显示并设地址指针为00H
    LCM_Delay(800);
	LCMBacklightCtrl(LCM_BLK_ON);											 // 开背光
    LCM_Delay(500);	 

	LCMClear(LCM_LINE_ALL, LCM_DIS_NML);									 // 清屏
}

/********************************************************************************************************
* FunctionName   : LCMDisplayStr()
* Description    : 向指点的地址写入字符串(显示中文字符必须从偶数地址开始)，存在安全隐患，可以指定长度保证安全
* EntryParameter : x-横坐标(0~15)，y-纵坐标(0~3)，pStr-字符串
* ReturnValue    : 正确返回0，否则返回1
********************************************************************************************************/
u8 LCMDisplayStr(u8 x, u8 y, u8 *pStr)
{
	u8  i;                                                                   // 显示地址
	uc8 lineAddr[] = {LCM_LINE1_HEAD, LCM_LINE2_HEAD, LCM_LINE3_HEAD, LCM_LINE4_HEAD}; 

	if ((y >= LCM_LINE_MAX) || (x >= LCM_LINE_LENGTH))						 // x, y坐标溢出
	{
	    return 1;
	}

	LCM_WriteCmd(LCM_BASIC_INS);                                             // 基本指令
	LCM_WriteCmd(lineAddr[y] + x);                                           // 写入地址 

    for (i=x; ((i<LCM_LINE_LENGTH) && (*pStr != '\0')); i++)                  	                                     
    {
    	LCM_WriteData(*pStr++);												 // 写入字符串
    }

	if (*pStr != '\0')                                                       // 判断数据是否写完
	{
 		x = 0;                                                               // 没写完写入下1行
		if (++y < LCM_LINE_MAX)
		{
		    LCMDisplayStr(x, y, pStr); 										 // 继续显示(递归调用)
		}
	}

	return 0;
}

/********************************************************************************************************
* FunctionName   : LCM_GetLattice()
* Description    : 把一行字符串点阵放入缓冲
* EntryParameter : pBuf - 点阵存放缓冲(pBuf[16][16])，pStr - 字符串，rvsBit - 反白位
* ReturnValue    : 返回字符串长度
********************************************************************************************************/
u8 LCM_GetLattice(u8 (*pBuf)[16], u8 *pStr, u16 rvsBit)
{
	u8 len = 0, k;
	u32 offSet;

	while ((len<LCM_LINE_LENGTH) && (*pStr))
	{
	    if (*pStr <= 0x80)													 // 英文(ASCII < 128)
		{
		    if (rvsBit & (0x01 << len))
			{
			    for (k=0; k<16; k++)
				{
			        pBuf[k][len] = 0xFF - St8x16[((*pStr-0x20)*16) + k];	 // 获取点阵
				}
			}
			else
			{
				for (k=0; k<16; k++)
				{
			        pBuf[k][len] = St8x16[((*pStr-0x20)*16) + k];	         // 获取点阵
				}
			}

			pStr++;
			len++;
		}
		else																 // 中文(ASCII > 128)
		{
		    if (len < (LCM_LINE_LENGTH-1))									 // 有空间存放一个汉字
			{
			    offSet  = ((*pStr - 0xA1)*94 + (*(pStr+1) - 0xA1))*32;		 // 汉字的地址是从0xA1A1开始
				offSet += W25Q_CN_START_ADDR;

				if (rvsBit & (0x01 << len))
				{
				    for (k=0; k<16; k++)
					{
					    pBuf[k][len  ] = 0xFF - W25QReadByte(offSet + k*2);	
						pBuf[k][len+1] = 0xFF - W25QReadByte(offSet + k*2+1); 
					}
				}
				else
				{
				    for (k=0; k<16; k++)
					{
					    pBuf[k][len  ] = W25QReadByte(offSet + k*2);	
						pBuf[k][len+1] = W25QReadByte(offSet + k*2+1); 
					}
				}

				pStr += 2;
				len  += 2;
			}
			else
			{
			    break;														 // 空间不够，跳出循环
			}
		}
	}
	
	return len;	
}

/********************************************************************************************************
* FunctionName   : LCM_DisplayBuf()
* Description    : 显示一行点阵(注意图形模式下坐标格式)
* EntryParameter : lin - 显示行(0~3), pBuf - 要显示的缓冲
* ReturnValue    : None
********************************************************************************************************/
void LCM_DisplayBuf(u8 x, u8 y, u8 len, u8 (*pBuf)[16])
{
    u8 i, k;
	u8 startX, startY;

	startX = LCM_LINE1_HEAD + x;											 //
	startY = LCM_LINE1_HEAD;												 //

	switch (y)
	{
	    case LCM_LINE1:	startX += 0; startY += 0;  break;
		case LCM_LINE2:	startX += 0; startY += 16; break;
		case LCM_LINE3: startX += 8; startY += 0;  break;
		case LCM_LINE4:	startX += 8; startY += 16; break;
		default: break;
	}

	if ((len % 2) == 0)													     // 显示字符长度
	{
	    len /= 2;
	}
	else
	{
	    len = (len+1) / 2; 
	}

   	LCM_WriteCmd(LCM_EXTEN_INS);     	                                     // 扩展指令

	for (k=0; k<16; k++)
	{      
	    LCM_WriteCmd(k + startY);   	                                     // 先写入垂直坐标
	    LCM_WriteCmd(0 + startX); 	                                         // 再写入水平坐标

	    for (i=0; i<len; i++)
	    {
	       	LCM_WriteData(pBuf[k][i*2]);  	                             	 // 写入两字节数据
	       	LCM_WriteData(pBuf[k][i*2+1]);			
		}
	}
	
	LCM_WriteCmd(LCM_BASIC_INS);   	                                         // 基本指令		  	                                
}

/********************************************************************************************************
* FunctionName   : LCMDisplayStr1()
* Description    : 显示一行
* EntryParameter : lin - 显示行(0~3), rvsBit - 显示模式(对应的反白位), pBuf - 要显示的缓冲
* ReturnValue    : None
********************************************************************************************************/
void LCMDisplayStr1(u8 x, u8 y, u16 rvsBit, u8 *pStr)
{
    u8 buf[16][16] = {0};
	u8 len;

	len = LCM_GetLattice(buf, pStr, rvsBit);										 // 把字符点阵放入缓冲
	LCM_DisplayBuf(x, y, len, buf); 
}

/********************************************************************************************************
* FunctionName   : LCMClear()
* Description    : 清行
* EntryParameter : lin - 行(1~4)，dat - 数据
* ReturnValue    : None
********************************************************************************************************/
void LCMClear(u8 lin, u16 rvsBit)
{
    u8 i;

    if (lin > (LCM_LINE_MAX-1))												 // 图形方式清屏
	{
	    for (i=0; i<LCM_LINE_MAX; i++)
		{
	        LCMDisplayStr1(0, i, rvsBit, "                ");
		}	
	}
	else
	{
		LCMDisplayStr1(0, lin, rvsBit, "                ");	
	}

	LCM_WriteCmd(LCM_DISPLAY_ON);  		  	                                 // 开显示
    LCM_Delay(250);
    LCM_WriteCmd(LCM_CLR_SCREEN);  			                                 // 清显示并设地址指针为00H
    LCM_Delay(800);
	LCMBacklightCtrl(LCM_BLK_ON);											 // 开背光
    LCM_Delay(500);
}

/********************************************************************************************************
* FunctionName   : LCMDispTempSym()
* Description    : 温度符号显示
* EntryParameter : x，y - 坐标
* ReturnValue    : None
********************************************************************************************************/
void LCMDispTempSym(u8 x, u8 y)
{
    u8 k, sym[] = {"°C"};
	u8 buf[16][16] = {0};
    u32 offSet;

    offSet = ((*sym - 0xA1)*94 + (*(sym+1) - 0xA1))*32;		
    offSet += W25Q_CN_START_ADDR;

	for (k=0; k<16; k++)
	{
	    buf[k][0] = W25QReadByte(offSet + k*2);	
		//pBuf[k][1] = W25QReadByte(offSet + k*2+1); 
	}

	for (k=0; k<16; k++)
	{
	    buf[k][1] = St8x16[((*(sym+2)-0x20)*16) + k];
	}

	LCM_DisplayBuf(x, y, 2, buf);
}

/********************************************************************************************************
* FunctionName   : LCMDisplayPicture()
* Description    : 写入一幅128x64的图片
* EntryParameter : pImg - 图形数据
* ReturnValue    : None
********************************************************************************************************/
void LCMDisplayPicture(u8 *pImg)
{
    u16 k;
    u8  x, y, n = 2;
	u8  startX = 0;
   
    do
	{
	    if (2 == n)
		{
		    k = 0;										                     // 上半屏首地址 
		}
		else
		{
			k = 512;									                     // 下半屏首地址
			startX = 8;
		}

		LCM_WriteCmd(LCM_EXTEN_INS);     	                                 // 扩展指令
	    for (y=0; y<32; y++)                	                             // 垂直坐标32位
		{	
		    LCM_WriteCmd(y + LCM_LINE1_HEAD);   	                         // 先写入垂直坐标
		    LCM_WriteCmd(0 + LCM_LINE1_HEAD + startX); 	                     // 再写入水平坐标

		    for (x=0; x<8; x++)            	                                 // 水平坐标16字节
		    {
		       	LCM_WriteData(pImg[k++]);  	                                 // 连续写入16位数据
		       	LCM_WriteData(pImg[k++]);	      
		    }
		}

	} while (--n); 
	
	LCM_WriteCmd(LCM_BASIC_INS);   	                                         // 基本指令
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/
