/******************************************************************************************************** 
*                           
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : diskio.c
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
#include "../UserCode/source/config.h"    			 // ͷ�ļ�

/********************************************************************************************************
* FunctionName   : disk_initialize()
* Description    : ��ʼ������
* EntryParameter : drv - �����������ţ�0..��
* ReturnValue    : None
********************************************************************************************************/
DSTATUS disk_initialize(BYTE drv)
{
    if (drv)
    {
        return STA_NOINIT;	                                                // ��֧�ֵ���������
    }

    return ((RES_OK == SDInit()) ? RES_OK : RES_ERROR);			        // ��ʼ��SD
}

/********************************************************************************************************
* FunctionName   : disk_status()
* Description    : �����̶�״̬
* EntryParameter : drv - �����������ţ�0..��
* ReturnValue    : ���ز������
********************************************************************************************************/
DSTATUS disk_status(BYTE drv)
{
    DSTATUS status;
    u8 tmp;

    if (drv)
    {
        return STA_NOINIT;                                                      // ��֧�ֵ���������
    }
    else
    {
        tmp = SDGetCardStatus();	                                        // ��ȡ��״̬
        status = ((tmp == SD_CARD_OK) ? RES_OK : ((tmp == SD_CARD_PR) ? STA_PROTECT : STA_NODISK));
    }

    return status;
}

/********************************************************************************************************
* FunctionName   : disk_read()
* Description    : ����������
* EntryParameter : drv - �����������ţ�buff - ���棻sector - ������ַ��count - ������
* ReturnValue    : ���ز������
********************************************************************************************************/
DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count)
{
    if (drv || (!count))
    {
        return RES_PARERR;
    }

    return (((count == 1) ? SDReadSector(sector, (u8 *)buff) : SDReadNSector(sector, (u8 *)buff, count)) ? RES_ERROR : RES_OK);
}

/********************************************************************************************************
* FunctionName   : disk_write()
* Description    : ��������д
* EntryParameter : drv - �����������ţ�buff - ���棻sector - ������ַ��count - ������
* ReturnValue    : ���ز������
********************************************************************************************************/
#if _READONLY == 0
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
{
    if (drv || (!count))
    {
        return RES_PARERR;
    }                

    return (((count == 1) ? SDWriteSector(sector, (u8 *)buff) : SDWriteNSector(sector, (u8 *)buff, count)) ? RES_ERROR : RES_OK);					// ���ز������
}
#endif /* _READONLY */
/********************************************************************************************************
* FunctionName   : disk_ioctl()
* Description    : ���̸�ʽ��ʹ��
* EntryParameter : drv - �����������ţ�ctrl - ����ָ�buff - ����
* ReturnValue    : ���ز������
********************************************************************************************************/
DRESULT disk_ioctl(BYTE drv,BYTE ctrl,void *buff)
{
	DRESULT res;

	if (drv)
	{
		return RES_PARERR;
	}

	switch (ctrl)
	{
	    case CTRL_SYNC:        res = RES_OK; break;
		case GET_SECTOR_COUNT: *(DWORD*)buff = SDCard.Sectors; res = RES_OK;   break; // �õ����п��õ�������Ŀ
		case GET_SECTOR_SIZE:  *(WORD*)buff = SDCard.SectorSize; res = RES_OK; break; // �õ�ÿ�������ж��ٸ��ֽ�
		default:               res = RES_PARERR; break;                      
	}

	return res;
}

/**************************************************************************************
* FunctionName   : get_fattime()
* Description    : ��ȡʱ�亯��
* EntryParameter : None
* ReturnValue    : û��ʹ�÷���0�����򷵻�ʱ��ֵ
**************************************************************************************/
/* RTC function */
#if !_FS_READONLY
DWORD get_fattime (void)
{
    return 0;
}

#endif

/********************************************************************************************************
* End Of File         
********************************************************************************************************/
