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
#include "../UserCode/source/config.h"    			 // 头文件

/********************************************************************************************************
* FunctionName   : disk_initialize()
* Description    : 初始化磁盘
* EntryParameter : drv - 物理驱动器号（0..）
* ReturnValue    : None
********************************************************************************************************/
DSTATUS disk_initialize(BYTE drv)
{
    if (drv)
    {
        return STA_NOINIT;	                                                // 仅支持单个驱动器
    }

    return ((RES_OK == SDInit()) ? RES_OK : RES_ERROR);			        // 初始化SD
}

/********************************************************************************************************
* FunctionName   : disk_status()
* Description    : 读磁盘读状态
* EntryParameter : drv - 物理驱动器号（0..）
* ReturnValue    : 返回操作结果
********************************************************************************************************/
DSTATUS disk_status(BYTE drv)
{
    DSTATUS status;
    u8 tmp;

    if (drv)
    {
        return STA_NOINIT;                                                      // 仅支持单个驱动器
    }
    else
    {
        tmp = SDGetCardStatus();	                                        // 获取卡状态
        status = ((tmp == SD_CARD_OK) ? RES_OK : ((tmp == SD_CARD_PR) ? STA_PROTECT : STA_NODISK));
    }

    return status;
}

/********************************************************************************************************
* FunctionName   : disk_read()
* Description    : 读磁盘扇区
* EntryParameter : drv - 物理驱动器号；buff - 缓存；sector - 扇区地址；count - 扇区数
* ReturnValue    : 返回操作结果
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
* Description    : 磁盘扇区写
* EntryParameter : drv - 物理驱动器号；buff - 缓存；sector - 扇区地址；count - 扇区数
* ReturnValue    : 返回操作结果
********************************************************************************************************/
#if _READONLY == 0
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
{
    if (drv || (!count))
    {
        return RES_PARERR;
    }                

    return (((count == 1) ? SDWriteSector(sector, (u8 *)buff) : SDWriteNSector(sector, (u8 *)buff, count)) ? RES_ERROR : RES_OK);					// 返回操作结果
}
#endif /* _READONLY */
/********************************************************************************************************
* FunctionName   : disk_ioctl()
* Description    : 磁盘格式等使用
* EntryParameter : drv - 物理驱动器号；ctrl - 控制指令；buff - 缓存
* ReturnValue    : 返回操作结果
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
		case GET_SECTOR_COUNT: *(DWORD*)buff = SDCard.Sectors; res = RES_OK;   break; // 得到所有可用的扇区数目
		case GET_SECTOR_SIZE:  *(WORD*)buff = SDCard.SectorSize; res = RES_OK; break; // 得到每个扇区有多少个字节
		default:               res = RES_PARERR; break;                      
	}

	return res;
}

/**************************************************************************************
* FunctionName   : get_fattime()
* Description    : 获取时间函数
* EntryParameter : None
* ReturnValue    : 没有使用返回0，否则返回时间值
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
