/******************************************************************************************************** 
*                              
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : sdCard.h
* Version        : V2.0
* Programmer(s)  : zhaojun_xf
* Parameters     : LPC1700 12.000MHz
* DebugTools     : Realview MDK V4.23  JLINK V8(SWD)
* Description    : SPI方式下SD驱动代码
*				   支持SD卡SD v1.1、SD v2.0和SDHC(可以兼容多种卡)
*				   注意：1G的SD卡一般没有MBR，2G及以上的SD一般有MBR
********************************************************************************************************/

#ifndef __SDCARD_H 
#define __SDCARD_H

/********************************************************************************************************
*                               Pin Definition                                                 
********************************************************************************************************/
// 卡检测
#define SD_STU_FUN            (PIO1_16_GPIO)                                // P1.16 SD卡检测  
#define SD_STU_PORT       	  (PORT1)							            // 端口
#define SD_STU_PIN       	  (16)								            // 管脚

// 片选
#define SD_CS_FUN             (PIO1_17_GPIO)                                // P1.17 片选   
#define SD_CS_PORT       	  (PORT1)							            // 端口
#define SD_CS_PIN       	  (17)								            // 管脚	

/*                            这里两个管脚没有使用
// 写保护
#define SD_PRT_FUN            (PIO0_23_GPIO)                                // P0.23 SD写保护  
#define SD_PRT_PORT       	  (PORT0)							            // 端口
#define SD_PRT_PIN       	  (23)								            // 管脚

// 电源开关

#define SD_POW_FUN            (PIO0_23_GPIO)                                // P0.23 电源开关   
#define SD_POW_PORT       	  (PORT0)							            // 端口
#define SD_POW_PIN       	  (23)								            // 管脚 */

/********************************************************************************************************
* 【管脚方向】 Pin direction                                                
********************************************************************************************************/
#define SD_STU_IN()           GPIOSetDir(SD_STU_PORT, SD_STU_PIN, GPIO_IN)	// 设置端口方向
#define SD_CS_OUT()           GPIOSetDir(SD_CS_PORT, SD_CS_PIN, GPIO_OUT)					             

/********************************************************************************************************
* 【管脚读/写】 Pin Read/Write                                                
********************************************************************************************************/
#define SD_STU_READ()         GPIOStatusValue(SD_STU_PORT, SD_STU_PIN)	    // 
#define SD_CS_WRITE(cs)       GPIOSetValue(SD_CS_PORT, SD_CS_PIN, cs);	    // 端口输出

/********************************************************************************************************
* 【结构体】 Structure                                                
********************************************************************************************************/
typedef struct
{
    u8  Type;					                                            // SDHC or SD
	u32 Capacity;                                                           // SD卡容量：MByte
	u32 SectorSize;                                                         // SD卡扇区：Byte
	u32 Sectors;					                                        // SD卡扇区数

} SD_Info;											                        // SD卡信息

extern SD_Info SDCard;						                                // SD卡信息

//-------------------------------------------------------------------------------------------------------
typedef struct
{
  u8  CSDStruct;                                                            // CSD结构
  u8  RdSectorSize;                                                         // 最大读取数据块长
  u32 DeviceSize;                                                           // 设备容量
  u8  DeviceSizeMul;                                                        // 设备容量乘积因子

} SD_CSD;											                        // SD卡CSD寄存器

//-------------------------------------------------------------------------------------------------------
typedef struct
{
    u8  MID;                                                                // 生产标识ID
    u8  OID[2];                                                             // OEM/应用 ID
    u8  PNM[5];                                                             // 产品名称
    u8  PRV;                                                                // 产品版本
    u8  PSN[4];                                                             // 产品序号
    u8  Res1;                                                               // 保留
    u8  MDT[2];                                                             // 生产日期
    u8  Crc;                                                                // CID CRC
    u8  Res2;                                                               // always 1

} SD_CID;									                                // SD卡CSD寄存器

extern SD_CID  SDCid;

//-------------------------------------------------------------------------------------------------------

// SD卡错误信息
typedef enum
{
/**
  * @brief  SD reponses and error flags
  */
	SD_RESPONSE_NO_ERROR      = (0x00),		                                // 没有错误
	SD_IN_IDLE_STATE          = (0x01),				                        //
	SD_ERASE_RESET            = (0x02),				                        //
	SD_ILLEGAL_COMMAND        = (0x04),			 	                        //
	SD_COM_CRC_ERROR          = (0x08),				                        //
	SD_ERASE_SEQUENCE_ERROR   = (0x10),				                        //
	SD_ADDRESS_ERROR          = (0x20),				                        // 地址错误
	SD_PARAMETER_ERROR        = (0x40),				                        //
	SD_RESPONSE_FAILURE       = (0xFF),				                        //

/**
  * @brief  Data response error
  */
	SD_DATA_OK                = (0x05),				                        // 数据正常
	SD_DATA_CRC_ERROR         = (0x0B),				                        // CRCc错误
	SD_DATA_WRITE_ERROR       = (0x0D),				                        // 写数据错误
	SD_DATA_OTHER_ERROR       = (0xFF),				                        // 其他错误

/**
  * @brief  SD Card Status
  */
    SD_CARD_OK                = (0x00),				                        // SD卡正常
	SD_CARD_NO                = (0x02),				                        // SD卡没有插入
	SD_CARD_PR                = (0x04)				                        // SD卡保护
} SD_Error;

/********************************************************************************************************
*  SD Command
********************************************************************************************************/
typedef enum
{
    SD_START_SSREAD  = (0xFE),                                              // 开始单块读   
    SD_START_MSREAD  = (0xFE),                                              // 开始多块读  
    SD_START_SSWRITE = (0xFE),                                              // 开始单块写  
    SD_START_MSWRITE = (0xFC),                                              // 开始多块写
    SD_STOP_MSWRITE  = (0xFD),                                              // 停止多块写 
    
} SD_SOCMD;

//-------------------------------------------------------------------------------------------------------

// SD卡类型定义  
typedef enum
{
    SD_MMC = (0x10),		                                                // MMC
    SD_V1  = (0x11),		                                                // SD1.0卡
    SD_V2  = (0x12),	                                                    // SD2.0
    SD_HC  = (0x20),		                                                // SDHC卡
    
} SD_TYPE;

/********************************************************************************************************
* 【宏定义】 macro definition                                                 
********************************************************************************************************/
#define SD_BUFF_SIZE                  (512)                                 // SD buffer size
#define SD_SECTOR_SIZE                (0x200)					            // SD 扇区大小
#define SD_OPERATE_NUM	              (0xF000)					            // 错误时，循环操作次数

/********************************************************************************************************
*  SD Command
********************************************************************************************************/
typedef enum
{
    // Class0：卡的识别、初始化等基本命令集
    CMD0   = (0x40 + 0 ),	                                                // 复位SD卡          
    CMD1   = (0x40 + 1 ),	                                                // 读OCR寄存器                   
    CMD9   = (0x40 + 9 ),	                                                // 读CSD寄存器               
    CMD10  = (0x40 + 10),	                                                // 读CID寄存器              
    CMD12  = (0x40 + 12),	                                                // 停止读多块时的数据传输      
    CMD13  = (0x40 + 13),	                                                // 读Card_Status寄存器

    // Class2：读卡命令集             
    CMD16  = (0x40 + 16),	                                                // 设置块的长度         
    CMD17  = (0x40 + 17),	                                                // 读单块      
    CMD18  = (0x40 + 18),	                                                // 读多块,直至主机发送CMD12为止    

    // Class4：写卡命令集 
    CMD24  = (0x40 + 24),	                                                // 写单块            
    CMD25  = (0x40 + 25),	                                                // 写多块
    CMD27  = (0x40 + 27),                                                   // 写CSD寄存器 

    // Class5：擦除卡命令集  
    CMD32  = (0x40 + 32),	                                                // 设置擦除块的起始地址 
    CMD33  = (0x40 + 33),	                                                // 设置擦除块的终止地址   
    CMD38  = (0x40 + 38),	                                                // 擦除所选择的块

    // Class6：写保护命令集  
    CMD28  = (0x40 + 28),                                                   // 设置写保护块的地址.
    CMD29  = (0x40 + 29),                                                   // 擦除写保护块的地址.
    CMD30  = (0x40 + 30),                                                   // Ask the card for the status of the write protection bits

    // class7：卡的锁定，解锁功能命令集
    CMD8   = (0x40 + 8 ),	                                                // 申请特定命令集

    // class8：应用相关（Application Specific）命令       
    CMD55  = (0x40 + 55),	                                                // 告诉卡接下来的命令是应用相关命令，而非标准命令                
    CMD56  = (0x40 + 56),	                                                // 应用相关(通用目的),的数据块读写命令
    CMD58  = (0x40 + 58),	                                                //
    CMD59  = (0x40 + 59),	                                                //

    // ACMA指令
    ACMD13 = (0x40 + 13),	                                                // 设置SD卡状态 
    ACMD18 = (0x40 + 18),	                                                // 保留作为SD安全应用
    ACMD22 = (0x40 + 22),	                                                // 发送写数据块的数目。响应为32位+CRC
    ACMD23 = (0x40 + 23),	                                                // 设置写前预擦除的数据块数目(用来加速多数据块写操作)。“1”=默认(一个块)(1) 
    ACMD41 = (0x40 + 41),	                                                // 要求访问的卡发送它的操作条件寄存器(OCR)内容
    ACMD42 = (0x40 + 42),	                                                // 连接[1]/断开[0]卡上CD/DAT3(pin 1]的50K欧姆上拉电阻。上拉电阻可用来检测卡
    ACMD51 = (0x40 + 51),	                                                // 读取SD配置寄存器SCR 

} SD_CMD;

//-------------------------------------------------------------------------------------------------------
typedef enum
{
    SD_POW_CLS	      = 0x00,							                    // 电源关
    SD_POW_OPN	      = 0x01,						                     	// 电源开
    
} SD_POW;
/********************************************************************************************************
* 【外部函数】 SPI function                                                 
********************************************************************************************************/
/* 1. 在移植SD卡部分程序代码时，只需要提供下面5个函数即可；
   2. SPI读写函数实际上是一样的，只是这里分开比较容易理解。                                            */

#define SD_WriteByte(dat)	(SSPSendRcvByte(SSP1, dat) )			        // 写一字节数据
#define SD_ReadByte()	    (SSPSendRcvByte(SSP1, 0xFF))		            // 读一字节数据
#define SD_SPIInit()        (SSPInit(SSP1)             )			        // 初始化总线
#define SD_LowSpeed()       (SSPSetSpeed(SSP1, 254)    )			        // 低速
#define SD_HighSpeed()      (SSPSetSpeed(SSP1, 2)      )			        // 高速

/********************************************************************************************************
* 【局部函数】 Local function                                                 
********************************************************************************************************/
static void SD_SetGpio(void);										        // 端口设置               
static u8   SD_SendCmd(u8 cmd,u32 arg,u8 crc);                              // 写命令
static u8   SD_GetCSD(SD_CSD *csd);                                         // 读CSD寄存器，并获取SD卡信息
static u8   SD_GetCID(SD_CID *cid);                                         // 读CID寄存器
static void SD_DelayUs(u32 us);									            // 延时

/********************************************************************************************************
* 【全局函数】 global function                                                 
********************************************************************************************************/
extern u8   SDInit(void);                                                   // SD卡初始化
extern u8   SDGetCardStatus(void);								            // 卡状态
extern void SDGetCardInfo(SD_Info *cardInfo);						        // 获取SD卡信息
extern u8   SDReadSector(u32 readAddr, u8 *pBuf);                           // 读一扇区数据
extern u8   SDWriteSector(u32 writeAddr, u8 *pBuf);                         // 写一扇区数据
extern u8   SDReadNSector(u32 readAddr, u8 *pBuf, u8 num);                  // 读多扇区
extern u8   SDWriteNSector(u32 writeAddr, u8 *pBuf, u8 num);                // 写多扇区

/******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/

