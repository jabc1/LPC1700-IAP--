/********************************************************************************************************    
*                                                                       
*                                     Address
*  
*                      (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : ili9320.h
* Version        : V1.0
* Programmer(s)  : zhaojun
* Parameters     : ARM7 LPC2148 11.0592MHz
* DebugTools     : JLINK V7.0 And Realview MDK V3.70
* Description    :
*
*
********************************************************************************************************/

#ifndef __ILI9481_H 
#define __ILI9481_H

/********************************************************************************************************
* Pin Definition                                                 
********************************************************************************************************/
#define TFT_D0_FUN                (PIO1_19_GPIO)    				                 // P1.19     
#define TFT_D0_PORT       	      (PORT1)							                 // 端口 
#define TFT_D0_PIN       	      (19)								                 // 管脚

#define TFT_D1_FUN                (PIO1_20_GPIO)    				                 // P1.20     
#define TFT_D1_PORT       	      (PORT1)							                 // 端口 
#define TFT_D1_PIN       	      (20)								                 // 管脚

#define TFT_D2_FUN                (PIO1_21_GPIO)    				                 // P1.21     
#define TFT_D2_PORT       	      (PORT1)							                 // 端口 
#define TFT_D2_PIN       	      (21)								                 // 管脚

#define TFT_D3_FUN                (PIO1_22_GPIO)    				                 // P1.22     
#define TFT_D3_PORT       	      (PORT1)							                 // 端口 
#define TFT_D3_PIN       	      (22)								                 // 管脚

#define TFT_D4_FUN                (PIO1_23_GPIO)    				                 // P1.23     
#define TFT_D4_PORT       	      (PORT1)							                 // 端口 
#define TFT_D4_PIN       	      (23)								                 // 管脚

#define TFT_D5_FUN                (PIO1_24_GPIO)    				                 // P1.24     
#define TFT_D5_PORT       	      (PORT1)							                 // 端口 
#define TFT_D5_PIN       	      (24)								                 // 管脚

#define TFT_D6_FUN                (PIO1_25_GPIO)    				                 // P1.25     
#define TFT_D6_PORT       	      (PORT1)							                 // 端口 
#define TFT_D6_PIN       	      (25)								                 // 管脚

#define TFT_D7_FUN                (PIO1_26_GPIO)    				                 // P1.26     
#define TFT_D7_PORT       	      (PORT1)							                 // 端口 
#define TFT_D7_PIN       	      (26)								                 // 管脚

#define TFT_DAT_L_PORT       	  (PORT1)							                 // 端口  
#define TFT_DAT_L_PIN       	  (19)						                         // 数据管脚起始
#define TFT_DAT_L       	      (0xFF << TFT_DAT_L_PIN)				             // 管脚 P1.19~P1.26

//-------------------------------------------------------------------------------------------------------
#define TFT_D8_FUN                (PIO0_15_GPIO)    				                 // P0.15     
#define TFT_D8_PORT       	      (PORT0)							                 // 端口 
#define TFT_D8_PIN       	      (15)								                 // 管脚

#define TFT_D9_FUN                (PIO0_16_GPIO)    				                 // P0.16     
#define TFT_D9_PORT       	      (PORT0)							                 // 端口 
#define TFT_D9_PIN       	      (16)								                 // 管脚

#define TFT_D10_FUN               (PIO0_17_GPIO)    				                 // P0.17     
#define TFT_D10_PORT       	      (PORT0)							                 // 端口 
#define TFT_D10_PIN       	      (17)								                 // 管脚

#define TFT_D11_FUN               (PIO0_18_GPIO)    				                 // P0.18     
#define TFT_D11_PORT       	      (PORT0)							                 // 端口 
#define TFT_D11_PIN       	      (18)								                 // 管脚

#define TFT_D12_FUN               (PIO0_19_GPIO)    				                 // P0.19     
#define TFT_D12_PORT       	      (PORT0)							                 // 端口 
#define TFT_D12_PIN       	      (23)								                 // 管脚

#define TFT_D13_FUN               (PIO0_20_GPIO)    				                 // P0.20     
#define TFT_D13_PORT       	      (PORT0)							                 // 端口 
#define TFT_D13_PIN       	      (20)								                 // 管脚

#define TFT_D14_FUN               (PIO0_21_GPIO)    				                 // P0.21     
#define TFT_D14_PORT       	      (PORT0)							                 // 端口 
#define TFT_D14_PIN       	      (21)								                 // 管脚

#define TFT_D15_FUN               (PIO0_22_GPIO)    				                 // P0.22     
#define TFT_D15_PORT       	      (PORT0)							                 // 端口 
#define TFT_D15_PIN       	      (22)								                 // 管脚

#define TFT_DAT_H_PORT       	  (PORT0)							                 // 端口  
#define TFT_DAT_H_PIN       	  (15)						                         // 数据管脚起始
#define TFT_DAT_H       	      (0xFF << TFT_DAT_H_PIN)					         // 管脚 P0.15~P0.22

//-------------------------------------------------------------------------------------------------------
#define TFT_CS_FUN                (PIO2_0_GPIO)    				                     // P2.0 
#define TFT_CS_PORT       	      (PORT2)							                 // 端口  
#define TFT_CS_PIN       	      (0)								                 // 管脚

#define TFT_RS_FUN                (PIO2_1_GPIO)    				                     // P2.1 
#define TFT_RS_PORT       	      (PORT2)							                 // 端口  
#define TFT_RS_PIN       	      (1)								                 // 管脚

#define TFT_WR_FUN                (PIO2_2_GPIO)    				                     // P2.2 
#define TFT_WR_PORT       	      (PORT2)							                 // 端口  
#define TFT_WR_PIN       	      (2)								                 // 管脚

#define TFT_RD_FUN                (PIO2_3_GPIO)    				                     // P2.3 
#define TFT_RD_PORT       	      (PORT2)							                 // 端口  
#define TFT_RD_PIN       	      (3)								                 // 管脚

#define TFT_RT_FUN                (PIO2_4_GPIO)    				                     // P2.4 
#define TFT_RT_PORT       	      (PORT2)							                 // 端口  
#define TFT_RT_PIN       	      (4)								                 // 管脚

#define TFT_EN_FUN                (PIO2_5_GPIO)    				                     // P2.5 
#define TFT_EN_PORT       	      (PORT2)							                 // 端口  
#define TFT_EN_PIN       	      (5)								                 // 管脚

//-------------------------------------------------------------------------------------------------------
#define TFT_SC_FUN                (PIO0_6_GPIO)    				                     // P0.6 
#define TFT_SC_PORT       	      (PORT0)							                 // 端口  
#define TFT_SC_PIN       	      (6)								                 // 管脚

#define TFT_INT_FUN               (PIO2_6_GPIO)    				                     // P2.6 
#define TFT_INT_PORT       	      (PORT2)							                 // 端口  
#define TFT_INT_PIN       	      (6)								                 // 管脚

#define TFT_LE_FUN                (PIO2_7_GPIO)    				                     // P2.7 
#define TFT_LE_PORT       	      (PORT2)							                 // 端口  
#define TFT_LE_PIN       	      (7)								                 // 管脚

#define TFT_FC_FUN                (PIO2_8_GPIO)    				                     // P2.8 
#define TFT_FC_PORT       	      (PORT2)							                 // 端口  
#define TFT_FC_PIN       	      (8)								                 // 管脚

#define TFT_TC_FUN                (PIO1_15_GPIO)    				                 // P1.15 
#define TFT_TC_PORT       	      (PORT1)							                 // 端口  
#define TFT_TC_PIN       	      (15)								                 // 管脚

//-------------------------------------------------------------------------------------------------------

/********************************************************************************************************
* Macro Definition                                                  
********************************************************************************************************/
#define TFT_RED                 (0xF800)						                     // 红色
#define TFT_GREEN               (0x03E0)						                     // 绿色
#define TFT_BLUE                (0x001F) 					                         // 蓝色
#define TFT_WRITE               (0xFFFF)						                     // 白色
#define TFT_BLACK               (0x0000)						                     // 黑色
#define	TFT_ORANGE              (0xF860)					                         // 橙色
#define	TFT_YELLOW		        (0xFFE0)						                     // 黄色
#define	TFT_CYAN			    (0x07FF)						                     // 青色
#define	TFT_PROPLE      	    (0xF81F)						                     // 紫色
#define TFT_NOCOLOR             (0x0001)            			                     // 无色

#define TFT_WIDTH			    (480)						                         // 屏宽
#define TFT_HEIGTH		        (320)						                         // 屏高

#define TFT_BLK_ON		        (0x01)									             // 背光开
#define TFT_BLK_OFF			    (0x00)									             // 背光关

//-------------------------------------------------------------------------------------------------------
// 不同指令有不同多个参数，按顺序写入即可
//
#define SOFT_RST                (0x01)						                         // Soft_reset (01h)
#define GET_POW_MODE            (0x0A)						                         // Get_power_mode (0Ah)
#define GET_ADDR_MODE           (0x0B)						                         // Get_address_mode (0Bh)
#define GET_PXL_FRMT            (0x0C)						                         // Get_pixel_format (0Ch)
#define GET_DISP_MODE           (0x0D)						                         // Get_display_mode (0Dh)
#define GET_SGL_MODE            (0x0E)						                         // Get_signal_mode (0Eh)
#define GET_DGST_RSLT           (0x0F)						                         // Get_diagnostic_result (0Fh)
#define ENT_SLP_MODE            (0x10)						                         // Enter_sleep_mode (10h)
#define EXT_SLP_MODE            (0x11)						                         // Exit_sleep_mode (11h)
#define ENT_PRTL_MODE           (0x12)						                         // Enter_Partial_mode (12h)
#define ENT_NML_MODE            (0x13)						                         // Enter_normal_mode (13h)
#define EXT_IVT_MODE            (0x20)						                         // Exit_invert_mode (20h)
#define ENT_IVT_MODE            (0x21)						                         // Enter_invert_mode (21h)
#define SET_DISP_OFF            (0x28)						                         // Set_display_off (28h)
#define SET_DISP_ON             (0x29)						                         // Set_display_on (29h)
#define SET_CLM_ADDR            (0x2A)						                         // Set_column_address (2Ah)
#define SET_PAGE_ADDR           (0x2B)						                         // Set_page_address (2Bh)
#define WRITE_MMR_START         (0x2C)						                         // Write_memory_start (2Ch)
#define READ_MMR_START          (0x2E)						                         // Read_memory_start (2Eh)
#define SET_PRTL_AREA           (0x30)						                         // Set_partial_area (30h)
#define SET_SCL_AREA            (0x33)						                         // Set_scroll_area (33h)
#define SET_TEAR_OFF            (0x34)						                         // Set_tear_off (34h)
#define SET_TEAR_ON             (0x35)						                         // Set_tear_on (35h)
#define SET_ADDR_MODE           (0x36)						                         // Set_address_mode (36h)
#define SET_SCL_START           (0x37)						                         // Set_scroll_start (37h)
#define EXT_IDEL_MODE           (0x38)						                         // Exit_idle_mode (38h)
#define ENT_IDLE_MODE           (0x39)						                         // Enter_idle_mode (39h)
#define SET_PXL_FRMT            (0x3A)						                         // Set_pixel_format (3Ah)
#define WRITE_MMR_CTNU          (0x3C)						                         // Write_Memory_Continue (3Ch)
#define READ_MMR_CTNU           (0x3E)						                         // Read_Memory_Continue (3Eh)
#define SET_TEAR_SCAN_LINE      (0x44)						                         // Set_Tear_Scanline (44h)
#define GET_SCAN_LINE           (0x45)						                         // Get_Scanline (45h)
#define READ_DDB_START          (0xA1)						                         // Read_DDB_Start (A1h)
#define CMD_ACS_PRTCT           (0xB0)						                         // Command Access Protect (B0h)
#define LOW_POW_MODE_CTRL       (0xB1)						                         // Low Power Mode Control (B1h)
#define FRM_MMR_ACS_ITFC_SET    (0xB3)						                         // Frame Memory Access and Interface Setting (B3h)
#define DISP_MODE_FRM_WRITE_SET (0xB4)						                         // Display Mode and Frame Memory Write Mode Setting (B4h)
#define DVC_CODE_READ           (0xBF)						                         // Device Code Read (BFh)
#define PNL_DRV_SET             (0xC0)						                         // Panel Driving Setting (C0h)
#define DISP_TIM_SET_NML_MODE   (0xC1)						                         // Display_Timing_Setting for Normal Mode (C1h)
#define DISP_TIM_SET_PRT_MODE   (0xC2)						                         // Display_Timing_Setting for Partial Mode (C2h)
#define DISP_TIM_SET_IDIL_MODE  (0xC3)					                             // Display_Timing_Setting for Idle Mode (C3h)
#define FRM_RATE_INV_CTRL       (0xC5)						                         // Frame Rate and Inversion Control (C5h)
#define ITFC_CTRL               (0xC6)						                         // Interface Control (C6h)
#define GMA_SET                 (0xC8)						                         // Gamma Setting (C8h)
#define POW_SET                 (0xD0)						                         // Power_Setting (D0h)
#define VCOM_CTRL               (0xD1)						                         // VCOM Control (D1h)
#define POW_SET_NML_MODE        (0xD2)						                         // Power_Setting for Normal Mode (D2h)
#define POW_SET_PRT_MODE        (0xD3)						                         // Power_Setting for Partial Mode (D3h)
#define POW_SET_IDLE_MODE       (0xD4)						                         // Power_Setting for Idle Mode (D4h)
#define NV_MMR_WRITE            (0xE0)						                         // NV Memory Write (E0h)
#define NV_MMR_CTRL             (0xE1)						                         // NV Memory Control (E1h)
#define NV_MMR_STUS_READ        (0xE2)						                         // NV Memory Status Read (E2h)
#define NV_MMR_PRT              (0xE3)						                         // NV Memory Protection (E3h)

/********************************************************************************************************
*                               InternalFunction                                                  
********************************************************************************************************/
static void TFT_SetGpio(void);				                                         // 设置端口方向
static void TFT_DelayUs(u32 us); 	                                                 // 液晶延时
static void TFT_WriteIndex(u16 idx);				                                 // 写地址
static void TFT_Reset(void);					                                     // 复位液晶
static void TFT_SetCursor(u16 x,u16 y);			                                     // 设置坐标
static void TFT_PutChar(u16 x, u16 y, u8 *pCh, u16 chColor, u16 bkColor);            // 显示单个字符
static void TFT_PutChinese(u16 x, u16 y, u8 *pStr, u16 chColor, u16 bkColor);	     // 显示中文

/********************************************************************************************************
*                               GlobalFunction                                                 
********************************************************************************************************/
extern void TFTInit(u16 color);                                                      // 初始化TFT
extern void TFTClear(u16 dat);					                                     // 清屏
extern void TFTBacklightCtrl(u8 blk);												 // 背光控制
extern void TFTSetPoint(u16 x, u16 y, u16 color);	                                 // 指定坐标画点
extern void TFTWriteData(u16 dat);				                                     // 写数据
extern void TFTBackLight(u8 status);                	                             // 背光控制
extern u16  TFTPutStr(u16 x,u16 y,u8 *ptr, u16 charColor,u16 bkColor);               // 显示字符串 
extern void TFTDrawPicture(u16 startX, u16 startY, u16 heigth, u16 width, uc8 *pic); // 显示图片
extern void TFTSetDisplayMode(u8 rlScan, u8 tbScan, u8 dirScan, u8 hFilp, u8 vFilp); // 设置图片显示及扫描方式

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                               End Of File                                                  
********************************************************************************************************/
