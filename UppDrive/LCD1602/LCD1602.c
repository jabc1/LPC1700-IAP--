/******************************************************************************************************** 
*                              
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : lcd1602.c
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
#include "../UserCode/source/config.h"    			                          // ͷ�ļ�

/********************************************************************************************************
* FunctionName   : LCD_Delay()
* Description    : LCD��ʾ��ʱ
* EntryParameter : us - ΢��
* ReturnValue    : None
********************************************************************************************************/
void LCD_Delay(u16 us)
{
    u8 i;

	while (us--)
	{
	    for (i=14; i>0; i--)	
		{
		    ;
		}
	}
}

/********************************************************************************************************
* FunctionName   : LCD_SetGpio()
* Description    : �˿�����ΪGPIO��������Ϊ����˿�
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCD_SetGpio(void)
{
    LCD_DAT_OUT();						                                     // ���ݶ˿ڷ�������

	LCD_EN_OUT();			                                                 // ʹ��
	LCD_RW_OUT();			                                                 // ��д
	LCD_RS_OUT();			                                                 // ����/ָ��ѡ��
	LCD_BK_OUT();			                                                 // ����
}

/********************************************************************************************************
* FunctionName   : LCDBacklightCtrl()
* Description    : ���⿪��
* EntryParameter : blk - 1����0��
* ReturnValue    : None
********************************************************************************************************/
void LCDBacklightCtrl(u8 blk)
{
    LCD_BK_WRITE(blk);
}

/********************************************************************************************************
* FunctionName   : LCD_WriteData()
* Description    : ��LCD1602��д������
* EntryParameter : dat - ����
* ReturnValue    : None
********************************************************************************************************/
void LCD_WriteData(u8 dat)
{
	LCD_Delay(500);                                                          // û��æ״̬�жϱ�־������ʱ         
	LCD_DAT_WRITE(dat);                                                      // д�����ݵ�LCD

	LCD_RS_WRITE(GPIO_SET);			                                         // ѡ������
	LCD_RW_WRITE(GPIO_CLR);			                                         // д����
	LCD_EN_WRITE(GPIO_CLR);						                             // �������ٶ�̫�߿���������С����ʱ
	LCD_Delay(500);
	LCD_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : WriteCommand()
* Description    : ��LCD1602��д��ָ��
* EntryParameter : cmd - ָ��
* ReturnValue    : None
********************************************************************************************************/
void LCD_WriteCommand(u8 cmd) 
{
	LCD_Delay(500);                                                          // û��æ״̬�жϱ�־������ʱ
	LCD_DAT_WRITE(cmd);                                                      // д��ָ�LCD

	LCD_RS_WRITE(GPIO_CLR);			                                         // ѡ��ָ��
	LCD_RW_WRITE(GPIO_CLR);			                                         // д����
	LCD_EN_WRITE(GPIO_CLR);						                             // �������ٶ�̫�߿���������С����ʱ
	LCD_Delay(500);
	LCD_EN_WRITE(GPIO_SET);
}

/********************************************************************************************************
* FunctionName   : LCDInit()
* Description    : LCD�ڹ���ǰ��Ҫ����ʾ����ʼ��,����ģ���޷���������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************************************/
void LCDInit(void) 
{
    LCD_SetGpio();			                                                 // ���ö˿ڷ���

	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);                               // ������ʾģʽ���ã������æ�ź�
	LCD_Delay(500); 
	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);                               // 0x38ָ���ʾ:8λ������ʾģʽ,���ж���ʾ
	LCD_Delay(500); 
	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);
	LCD_Delay(500); 

	LCD_WriteCommand(LCD_DISPLAY_DOUBLE_LINE);                               // ��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
	LCD_WriteCommand(LCD_DISPLAY_OFF);                                       // �ر���ʾ
	LCD_WriteCommand(LCD_CLEAR_SCREEN);                                      // ��ʾ����
	LCD_WriteCommand(LCD_AC_AUTO_INCREMENT);                                 // ��ʾ����ƶ�����
	LCD_WriteCommand(LCD_DISPLAY_ON);                                        // ��ʾ�����������

	LCDBacklightCtrl(LCD_BLK_ON);									         // ������
}

/********************************************************************************************************
* FunctionName   : LCD_DisplayChar()
* Description    : ��ָ��������д������
* EntryParameter : x - ������, y - ������, ch - ΪASCIIֵ
* ReturnValue    : ��ȷ����0�����򷵻�1
********************************************************************************************************/
u8 LCDDisplayChar(u8 x, u8 y, u8 ch)
{
	if ((y > LCD_LINE_MAX) || (x > LCD_LINE_LENGTH))						 // x,y�������
	{
	    return 1;
	}

	x |= ((y == 1) ? LCD_LINE2_HEAD : LCD_LINE1_HEAD);						 // ��ʾ����
	LCD_WriteCommand(x);                                                     // ���ﲻ���æ�źţ����͵�ַ��
	LCD_WriteData(ch);

	return 0;
}

/********************************************************************************************************
* FunctionName   : LCD_DisplayStr()
* Description    : ��ָ��������д���ַ��������Զ�������ʾ���ܣ�
* EntryParameter : x - x����(0~15), y - y����(0~1), *pStr - ��ʾ�ַ���
* ReturnValue    : ��ȷ����0�����󷵻�1
********************************************************************************************************/
u8 LCDDisplayStr(u8 x, u8 y, u8 *pStr)
{
    u8 i;
	uc8 lineAddr[] = {LCD_LINE1_HEAD, LCD_LINE2_HEAD};

	if ((y >= LCD_LINE_MAX) || (x >= LCD_LINE_LENGTH))						 // x, y�������
	{
	    return 1;
	}

	LCD_WriteCommand(lineAddr[y] + x);                                       // д���ַ

	for (i=x; ((i<LCD_LINE_LENGTH) && (*pStr!='\0')); i++)
	{
 		LCD_WriteData(*(pStr++));                                            // д������
	}

	if (*pStr != '\0')                                                       // �ж������Ƿ�д��
	{
 		x = 0;                                                               // ûд��д���2��
		if (++y < LCD_LINE_MAX)
		{
		    LCDDisplayStr(x, y, pStr);   									 // ��һ�м�����ʾ(�ݹ����)
		} 
	}

	return 0;
}

/********************************************************************************************************
*                                     End Of File                                                  
********************************************************************************************************/


