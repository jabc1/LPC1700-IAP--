/********************************************************************************************************  
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : timer32.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : 
*
*
********************************************************************************************************/

#ifndef __TIMER32_H 
#define __TIMER32_H

/******************************************************************************************************** 
* ���Ĵ����� Register                                              
********************************************************************************************************/

// IR�жϼĴ���
#define TIM_MR0                    (0x01 << 0)								 // MR0�ж�
#define TIM_MR1                    (0x01 << 1)								 // MR1�ж�
#define TIM_MR2                    (0x01 << 2)								 // MR2�ж�
#define TIM_MR3                    (0x01 << 3)								 // MR3�ж�

#define TIM_CR0                    (0x01 << 4)								 // CR0�ж�
#define TIM_CR1                    (0x01 << 5)								 // CR1�ж�

// TCR��ʱ�����ƼĴ���
#define TIM_TCRE                   (0x01 << 0)								 // ʹ��(1-ʹ�ܣ�0-��ֹ)
#define TIM_TCRR                   (0x01 << 1)								 // ��λ

// CTCR���������ƼĴ���
#define TIM_TIMER			       (0x00 << 0)				                 // ��ʱ�����������ڶ�ʱģʽ
#define TIM_CRISE			       (0x01 << 0)				                 // ����ģʽ�����������벶��
#define TIM_CFALL			       (0x02 << 0)				                 // ����ģʽ���½������벶��
#define TIM_CDBLE			       (0x03 << 0)				                 // ����ģʽ��˫�������벶��
#define TIM_CAP0S			       (0x00 << 2)				                 // CAPn.0����TIMERn
#define TIM_CAP1S			       (0x01 << 2)				                 // CAPn.1����TIMERn

// MCRƥ����ƼĴ���
#define TIM_MR0I				   (0x01 <<  0)								 // MR0��TCֵƥ��ʱ�������ж�
#define TIM_MR0R				   (0x01 <<  1)								 //	MR0��TCֵ��ƥ��ʱTC��λ
#define TIM_MR0S				   (0x01 <<  2)								 //	MR0��TCֵ��ƥ��ʱTC��PCֹͣ��TCR[0]����
#define TIM_MR1I				   (0x01 <<  3)								 // MR1��TCֵƥ��ʱ�������ж�
#define TIM_MR1R				   (0x01 <<  4)								 //	MR1��TCֵ��ƥ��ʱTC��λ
#define TIM_MR1S				   (0x01 <<  5)								 //	MR1��TCֵ��ƥ��ʱTC��PCֹͣ��TCR[0]����
#define TIM_MR2I				   (0x01 <<  6)								 // MR2��TCֵƥ��ʱ�������ж�
#define TIM_MR2R				   (0x01 <<  7)								 //	MR2��TCֵ��ƥ��ʱTC��λ
#define TIM_MR2S				   (0x01 <<  8)								 //	MR2��TCֵ��ƥ��ʱTC��PCֹͣ��TCR[0]����
#define TIM_MR3I				   (0x01 <<  9)								 // MR3��TCֵƥ��ʱ�������ж�
#define TIM_MR3R				   (0x01 << 10)								 //	MR3��TCֵ��ƥ��ʱTC��λ
#define TIM_MR3S				   (0x01 << 11)								 //	MR3��TCֵ��ƥ��ʱTC��PCֹͣ��TCR[0]����

// EMR�ⲿƥ��Ĵ���
#define TIM_EM0				       (0)								         //	��λ��ֵ�ᱻ������MATn.0
#define TIM_EM1				       (1)								         //	��λ��ֵ�ᱻ������MATn.1
#define TIM_EM2				       (2)								         //	��λ��ֵ�ᱻ������MATn.2
#define TIM_EM3				       (3)								         //	��λ��ֵ�ᱻ������MATn.3

#define TIM_EMC0				   (4)								         //	�ⲿƥ�����0
#define TIM_EMC1				   (6)								         //	�ⲿƥ�����1
#define TIM_EMC2				   (8)								         //	�ⲿƥ�����2
#define TIM_EMC3				   (10)								         //	�ⲿƥ�����3

#define TIM_EM0N                   (0x00 << TIM_EMC0)						 // EMC0�޲���
#define TIM_EM0L                   (0x01 << TIM_EMC0)						 // EMC0����͵�ƽ
#define TIM_EM0H                   (0x02 << TIM_EMC0)						 //	EMC0����ߵ�ƽ
#define TIM_EM0T                   (0x03 << TIM_EMC0)						 //	EMC0��ƽ��ת

#define TIM_EM1N                   (0x00 << TIM_EMC1)						 // EMC1�޲���
#define TIM_EM1L                   (0x01 << TIM_EMC1)						 // EMC1����͵�ƽ
#define TIM_EM1H                   (0x02 << TIM_EMC1)						 //	EMC1����ߵ�ƽ
#define TIM_EM1T                   (0x03 << TIM_EMC1)						 //	EMC1��ƽ��ת

#define TIM_EM2N                   (0x00 << TIM_EMC2)						 // EMC2�޲���
#define TIM_EM2L                   (0x01 << TIM_EMC2)						 // EMC2����͵�ƽ
#define TIM_EM2H                   (0x02 << TIM_EMC2)						 //	EMC2����ߵ�ƽ
#define TIM_EM2T                   (0x03 << TIM_EMC2)						 //	EMC2��ƽ��ת

#define TIM_EM3N                   (0x00 << TIM_EMC3)						 // EMC3�޲���
#define TIM_EM3L                   (0x01 << TIM_EMC3)						 // EMC3����͵�ƽ
#define TIM_EM3H                   (0x02 << TIM_EMC3)						 //	EMC3����ߵ�ƽ
#define TIM_EM3T                   (0x03 << TIM_EMC3)						 //	EMC3��ƽ��ת

#define TIM_EM_ALL                 (0x00)						             //	EMC�����ⲿƥ�����κβ���

// CCR������ƼĴ���
#define TIM_CAP0IN				   (0x00)									 // CAPn.0��Ϊ����������,����3λ����Ϊ0
#define TIM_CAP0RE			       (0x01 << 0)				                 // �����ز���CAPn.0,����TC����ת��CR0
#define TIM_CAP0FE			       (0x01 << 1)				                 // �½��ز���CAPn.0,����TC����ת��CR0
#define TIM_CAP0IT			       (0x01 << 2)				                 // ��TC����ת��CR0ͬʱ����CAPn.0�ж�

#define TIM_CAP1IN				   (0x00)									 // CAPn.1��Ϊ����������,����3λ����Ϊ0
#define TIM_CAP1RE			       (0x01 << 3)				                 // �����ز���CAPn.1,����TC����ת��CR1
#define TIM_CAP1FE			       (0x01 << 4)				                 // �½��ز���CAPn.1,����TC����ת��CR1
#define TIM_CAP1IT			       (0x01 << 5)				                 // ��TC����ת��CR1ͬʱ����CAPn.1�ж�

/******************************************************************************************************** 
* Variable declaration                                                 
********************************************************************************************************/
typedef enum
{
    TIM0 = 0,
	TIM1 = 1,
	TIM2 = 2,
	TIM3 = 3,

} LPC_TIMER;																  // ��ʱ������

//------------------------------------------------------------------------------------------------------

typedef struct _TIM_SET
{   
    u8  Mode;                                                          		  // ����ģʽ
    u8  IntClear;														      // ����ж�
	u32 InitValue;															  // ��ʱ/��������ʼֵ
	u32 Prescaler;															  // Ԥ��Ƶ

// ��ʱ��
	u16 MatchSet;															  // ƥ������
	u16 MatchOut;															  // ƥ���������
	u32 TimValue;															  // ��ʱֵ(��λ��us)

// ������
    u8  CapCtrl;															  // �������
    u8  CaptureSet;															  // ��������(����ģʽ��CAP����)
} TIM_SET;

/******************************************************************************************************** 
* Internal Function                                                  
********************************************************************************************************/

/******************************************************************************************************** 
* Global Function                                                 
********************************************************************************************************/
extern u8   TIM32Init(u8 timNum, TIM_SET *timPar);	                          // ��ʼ����ʱ���������ö�ʱʱ��ms
extern void TIM32ClearInt(u8 timNum, u8 intSou);							  // ����ж�
extern void TIM32Enable(u8 timNum);				                              // ʹ�ܶ�ʱ��
extern void TIM32Disable(u8 timNum);				                          // ��ֹ��ʱ��
extern void TIM32Reset(u8 timNum);				                              // ������ʱ��

extern void TIM32ClearTC(u8 timNum);										  // ���TC
extern u32  TIM32GetTC(u8 timNum);											  // ��ȡTCֵ

/*******************************************************************************************************/
#endif

/******************************************************************************************************** 
* End Of File         
********************************************************************************************************/
