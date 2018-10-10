/**************************************************************************************    
*                       Crazy ARM LPC2148 V2.0 ������ʵ�����                                                 
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
* Description    : ��ʱ����
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
* Description    : ��ʼ��ADS
* EntryParameter : NO
* ReturnValue    : NO
**************************************************************************************/
void ADSInit(void)
{
   ADS_SCK_DDR() |= (1<<ADS_SCK);							  // ����˿�����
   ADS_DIN_DDR() |= (1<<ADS_DIN);
   ADS_CS_DDR()  |= (1<<ADS_CS);

   ADS_INT_DDR() &= ~(1<<ADS_INT);							  // ����˿�����
   ADS_DOUT_DDR() &= ~(1<<ADS_DOUT);
   
    ADS_SCK_SET();
	ADS_DIN_SET();
	ADS_CS_SET();						  
}

/**************************************************************************************
* FunctionName   : ADS_Start()
* Description    : ����ADS7843
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
* Description    : ��ȡADS
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

	    ADS_SCK_SET();	   	      // �½�����Ч
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
* Description    : д���ݵ�ADS
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
		ADS_SCK_SET(); 	     // ��������Ч
	    ADS_Delay(1);
		
		dat <<= 1;		   
	}
}

/**************************************************************************************
* FunctionName   : ADS_Coordinate()
* Description    : ��ȡ���꣬������
* EntryParameter : NO
* ReturnValue    : xy - ��16λΪx���꣬��16λΪy����
**************************************************************************************/
uint32 ADS_Coordinate(void)
{
    uint8 i;
	uint16 x[8],y[8];
	uint32 xy = 0;

	ADS_Delay(200);                 // ��ʱ����
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
		 x[i] = ADS_Read();       // ��x������

		 ADS_Wirte(CHY); 
		 ADS_SCK_SET(); 
		 ADS_Delay(2);
		 ADS_SCK_CLR(); 
		 ADS_Delay(2);
		 y[i] = ADS_Read();		 // ��y������
	}

	xy = ADS_Average(x)<<16;
	xy |= ADS_Average(y);

	ADS_CS_SET();

	return xy;
}

/**************************************************************************************
* FunctionName   : ADS_Average()
* Description    : ��ƽ��ֵ����ȥ��������Сֵ
* EntryParameter : arr[8] - ��Ҫ��ȡ����ֵ
* ReturnValue    : ����ƽ��ֵ
**************************************************************************************/
uint16 ADS_Average(uint16 *p)
{
	uint8 i;
	uint16 max = *(p+0),min = *(p+0),ave = 0,sum = 0;

	for (i=0; i<8; i++)
	{
	    if (max < *(p+i))						  // ȡ���ֵ
		{
		    max = *(p+i);
		}

		if (min > *(p+i))						  // ȡ��Сֵ
		{
		    min = *(p+i);
		}

	    sum += *(p+i);
	}
	    
	ave = (sum - (min+max))/6;					  // ȥ��������Сֵ

	return ave;
}

/**************************************************************************************
* FunctionName   : ADSGetConversion()
* Description    : ��ȡת���������ֵ
* EntryParameter : *px - x ����,*py - y ����
* ReturnValue    : NO
**************************************************************************************/
void ADSGetConversion(uint16 *px,uint16 *py)
{
     uint32 tmp = 0;
     uint16 x = 0,y = 0;

     tmp = ADS_Coordinate();					                // ��ȡ���� 
	 x = (uint16)(tmp>>16);			                            // ��ȡx����
	 y = (uint16)(tmp&0x0000FFFF);							    // ��ȡy����

	 // ������ͨ�����Ĵ�С�ͷֱ��ʵó�����������
	 x = ((x-Tch_Xmin)*Tch_LCDWidth)/(Tch_Xmax-Tch_Xmin);
	 y = ((y-Tch_Ymin)*Tch_LCDHeight)/(Tch_Ymax-Tch_Ymin);

	 // ��������ԭ��ʹ�������ԭ�㲻�غϣ����Ա�����ת��һ�Σ�������ֵ�غ�
	 // ����x��y��
	 *py = Tch_LCDWidth-x;	                                   // ����x����
	 *px = y;	                                               // ����y���꣬
}

/**************************************************************************************
*                                     End Of File                                                  
**************************************************************************************/
