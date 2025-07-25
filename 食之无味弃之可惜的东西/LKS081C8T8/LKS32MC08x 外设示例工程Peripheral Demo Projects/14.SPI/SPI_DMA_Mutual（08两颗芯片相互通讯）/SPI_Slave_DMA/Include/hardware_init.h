/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： hardware_init.h
 * 文件标识：
 * 内容摘要： 硬件初始化头文件定义
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet Li
 * 完成日期： 2020年8月16日
 *
 * 修改记录1：
 * 修改日期：2020年8月16日
 * 版 本 号：V 1.0
 * 修 改 人：Howlet Li
 * 修改内容：创建
 *
 *******************************************************************************/
#ifndef __HARDWARE_INIT_H
#define __HARDWARE_INIT_H

#include "lks32mc08x_lib.h"


typedef struct
{
	 u8 SPI_TXRX_SIZE;
	 u8 SPI_Temp_Data[512];
	 u8 *SPI_TempTX_Data;
	 u8 *SPI_TempRX_Data;

}SPI_BuffData;

extern SPI_BuffData SPI_DATA;

#define start_PRES 1 //start
#define stop_PRES 2  //stop

#define key_start GPIO_ReadInputDataBit(GPIO2, GPIO_Pin_11)
#define key_stop GPIO_ReadInputDataBit(GPIO2, GPIO_Pin_12)


void GPIO_init(void);
void Clock_Init(void);
void Hardware_init(void);
void SoftDelay(u32 cnt);
void SPI_init(void);
void Spi_master_DMA(UINT8 *spi_txdma_data, UINT8 *spi_rxdma_data, u8 spi_len);
int GPIO_KEY_Scan(u8 mode);
uint8_t Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#endif

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

