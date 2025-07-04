/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_i2c.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� I2C����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��05��21�� <td>1.0     <td>YangZJ     <td>����
 * </table>
 */
 
#ifndef __LKS32MC08x_I2C_H
#define __LKS32MC08x_I2C_H

#include "lks32mc08x_lib.h"

/** 
 *@brief I2C�Ĵ����ṹ����
 */
typedef struct
{
   __IO uint32_t ADDR;
   __IO uint32_t CFG;
   __IO uint32_t SCR;
   __IO uint32_t DATA;
   __IO uint32_t MSCR;
   __IO uint32_t BCR;
}I2C_TypeDef;


/**
 * I2C�ṹ�����ַ����
 */
#ifndef I2C
#define I2C         ((I2C_TypeDef *) I2C0_BASE)
#endif

/** 
 *@brief I2C��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
    u8  ADRCMP          ;   /**<   I2C Ӳ����ַ�Ƚ�ʹ�ܿ��أ�ֻ���� DMA ģʽ�¿�������Ч*/
    u8  MST_MODE        ;   /**<   I2C ��ģʽʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u8  SLV_MODE        ;   /**<   I2C ��ģʽʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u8  DMA             ;   /**<   I2C DMA����ʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u32 BaudRate        ;   /**<   I2C ������*/
    u8  IE              ;   /**<   I2C �ж�ʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u8  TC_IE           ;   /**<   I2C ���ݴ�������ж�ʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u8  BUS_ERR_IE      ;   /**<   I2C ���ߴ����¼��ж�ʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u8  STOP_IE         ;   /**<   I2C STOP �¼��ж�ʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u8  BURST_NACK      ;   /**<   I2C ���䣬NACK �¼��ж�ʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
    u8  BURST_ADDR_CMP  ;   /**<   I2C ���䣬Ӳ����ַƥ���ж�ʹ�ܣ�ENABLE:ʹ�ܣ�DISABLE:ʧ��*/
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
