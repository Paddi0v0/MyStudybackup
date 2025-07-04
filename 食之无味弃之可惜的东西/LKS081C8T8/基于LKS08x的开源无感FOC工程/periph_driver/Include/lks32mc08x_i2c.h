/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc08x_spi.h
 * �ļ���ʶ��
 * ����ժҪ�� I2C����ͷ�ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� 
 * ������ڣ� 
 *
 * �޸ļ�¼1��
 * �޸����ڣ�
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
 
#ifndef __lks32mc08x_I2C_H
#define __lks32mc08x_I2C_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct
{
   __IO uint32_t ADDR;
   __IO uint32_t CFG;
   __IO uint32_t SCR;
   __IO uint32_t DATA;
   __IO uint32_t MSCR;
   __IO uint32_t BCR;
}I2C_TypeDef;

#ifndef I2C
#define I2C         ((I2C_TypeDef *) I2C0_BASE)
#endif

typedef struct
{
    u32 BaudRate        ;   //  I2C ������
    u8  ADRCMP          ;   //  I2C Ӳ����ַ�Ƚ�ʹ�ܿ��أ�ֻ���� DMA ģʽ�¿�������Ч��
    u8  MST_MODE        ;   //  I2C ��ģʽʹ��
    u8  SLV_MODE        ;   //  I2C ��ģʽʹ��
    u8  DMA             ;   //  I2C DMA����ʹ��
    u8  IE              ;   //  I2C �ж�ʹ��
    u8  TC_IE           ;   //  I2C ���ݴ�������ж�ʹ��
    u8  BUS_ERR_IE      ;   //  I2C ���ߴ����¼��ж�ʹ��
    u8  STOP_IE         ;   //  I2C STOP �¼��ж�ʹ��
    u8  BURST_NACK      ;   //  I2C ���䣬NACK �¼��ж�ʹ��
    u8  BURST_ADDR_CMP  ;   //  I2C ���䣬Ӳ����ַƥ���ж�ʹ��
    u16 null;// ��ַ����
}I2C_InitTypeDef;

void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct);

void    I2C_SendData(I2C_TypeDef *I2Cx, uint8_t n);
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx);

// i2c&dma����&����
void i2c_dma_rx(u8 addr, u8 *data, u8 len);
void i2c_dma_tx(u8 addr, u8 *data, u8 len);
void i2c_dma_init(void);
void i2c_dma_state_over(void);

#endif /*__lks32mc08x_I2C_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
