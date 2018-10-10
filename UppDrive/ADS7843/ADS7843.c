/**************************************************************************************    
*                       Crazy ARM LPC2148 V2.0 开发板实验程序                                                 
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ADS7843.c
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V4.02
* Description    :
*
*
**************************************************************************************/

/**************************************************************************************
*                                  HeaderFiles                                                  
**************************************************************************************/
#include "../source/config.h"    

/**************************************************************************************
* FunctionName   : ADS_Delay()
* Description    : 延时消抖
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void ADS_Delay(uint8 t)
{
    while (t--)
	{
	    __asm
		     {
			     nop
				 nop
				 nop
				 nop
				 nop
			 }
	}
}
				
/**************************************************************************************
* FunctionName   : ADSInit()
* Description    : 初始化ADS
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void ADSInit(void)
{
   ADS_SCK_DDR() |= (1<<ADS_SCK);							  // 输入端口设置
   ADS_DIN_DDR() |= (1<<ADS_DIN);
   ADS_CS_DDR()  |= (1<<ADS_CS);

   ADS_INT_DDR() &= ~(1<<ADS_INT);							  // 输出端口设置
   ADS_DOUT_DDR() &= ~(1<<ADS_DOUT);
   
    ADS_SCK_SET();
	ADS_DIN_SET();
	ADS_CS_SET();						  
}

/**************************************************************************************
* FunctionName   : ADS_Start()
* Description    : 启动ADS7843
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void ADS_Start(void)
{
    ADS_SCK_CLR();
	ADS_CS_SET();
	ADS_DIN_SET();
	ADS_SCK_SET();
	ADS_CS_CLR();
}

/**************************************************************************************
* FunctionName   : ADS_Read()
* Description    : 读取ADS
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
uint16 ADS_Read(void)
{
    uint8 i;
	uint16 dat = 0;

	for (i=0; i<12; i++)
	{
	    dat <<= 1;

	    ADS_SCK_SET();	   	      // 下降沿有效
		ADS_Delay(1);
		ADS_SCK_CLR();
		ADS_Delay(1);

		if (ADS_DOUT_READ())
		{
		    dat++;
		}
	}

	return dat;
}

/**************************************************************************************
* FunctionName   : ADS_Wirte()
* Description    : 写数据到ADS
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void ADS_Wirte(uint8 dat)
{
    uint8 i;

	for (i=0; i<8; i++)
	{
	    ADS_SCK_CLR();
	    if (dat & 0x80)
		{
		    ADS_DIN_SET();    
		}
		else
		{
		    ADS_DIN_CLR();   
		}
   
		ADS_Delay(1);
		ADS_SCK_SET(); 	     // 上升沿有效
	    ADS_Delay(1);
		
		dat <<= 1;		   
	}
}

/**************************************************************************************
* FunctionName   : ADS_Coordinate()
* Description    : 读取坐标，并返回
* EntryParameter : NO
* ReturnValue    : xy - 高16位为x坐标，低16位为y坐标
**************************************************************************************/
uint32 ADS_Coordinate(void)
{
    uint8 i;
	uint16 x[8],y[8];
	uint32 xy = 0;

	ADS_Delay(200);                 // 延时消抖
	ADS_CS_CLR();

    for (i=0; i<8; i++)
	{
	     ADS_Start();
		 ADS_Delay(2);

		 ADS_Wirte(CHX); 
		 ADS_SCK_SET(); 
		 ADS_Delay(2);
		 ADS_SCK_CLR(); 
		 ADS_Delay(2);
		 x[i] = ADS_Read();       // 读x轴坐标

		 ADS_Wirte(CHY); 
		 ADS_SCK_SET(); 
		 ADS_Delay(2);
		 ADS_SCK_CLR(); 
		 ADS_Delay(2);
		 y[i] = ADS_Read();		 // 读y轴坐标
	}

	xy = ADS_Average(x)<<16;
	xy |= ADS_Average(y);

	ADS_CS_SET();

	return xy;
}

/**************************************************************************************
* FunctionName   : ADS_Average()
* Description    : 求平均值，并去掉最大和最小值
* EntryParameter : arr[8] - 需要求取的数值
* ReturnValue    : 返回平均值
**************************************************************************************/
uint16 ADS_Average(uint16 *p)
{
	uint8 i;
	uint16 max = *(p+0),min = *(p+0),ave = 0,sum = 0;

	for (i=0; i<8; i++)
	{
	    if (max < *(p+i))						  // 取最大值
		{
		    max = *(p+i);
		}

		if (min > *(p+i))						  // 取最小值
		{
		    min = *(p+i);
		}

	    sum += *(p+i);
	}
	    
	ave = (sum - (min+max))/6;					  // 去掉最大和最小值

	return ave;
}

/**************************************************************************************
* FunctionName   : ADSGetConversion()
* Description    : 获取转换后的坐标值
* EntryParameter : *px - x 坐标,*py - y 坐标
* ReturnValue    : NO
**************************************************************************************/
void ADSGetConversion(uint16 *px,uint16 *py)
{
     uint32 tmp = 0;
     uint16 x = 0,y = 0;

     tmp = ADS_Coordinate();					                // 获取坐标 
	 x = (uint16)(tmp>>16);			                            // 获取x坐标
	 y = (uint16)(tmp&0x0000FFFF);							    // 获取y坐标

	 // 以下是通过屏的大小和分辨率得出的坐标数据
	 x = ((x-Tch_Xmin)*Tch_LCDWidth)/(Tch_Xmax-Tch_Xmin);
	 y = ((y-Tch_Ymin)*Tch_LCDHeight)/(Tch_Ymax-Tch_Ymin);

	 // 由于屏的原点和触摸屏的原点不重合，所以必须在转换一次，是坐标值重合
	 // 交换x与y轴
	 *py = Tch_LCDWidth-x;	                                   // 存入x坐标
	 *px = y;	                                               // 存入y坐标，
}

/**************************************************************************************
*                                     End Of File                                                  
**************************************************************************************/
