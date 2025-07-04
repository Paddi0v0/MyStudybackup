/**
 * @file
 * @brief        I2C�⺯��ͷ�ļ�
 * @author       YangZJ
 * @date         2023-01-02
 *  */
#ifndef __lks32mc45x_I2C_H
#define __lks32mc45x_I2C_H
///< Includes ------------------------------------------------------------------
#include "lks32mc07x.h"
#include "lks32mc07x_dma.h"
#include "basic.h"

// I2C_BUS_STATE   I2C����״̬
#define I2C_BUS_STATE_STT_ERR BIT7  ///< ���ߴ���
#define I2C_BUS_STATE_LOST_ARB BIT6 ///< �����ٲö�ʧ
#define I2C_BUS_STATE_STOP_EVT BIT5 ///< STOP�¼���¼

#define I2C_BUS_STATE_ADDR_DATA BIT3 ///< ���ڴ����ַ����

#define I2C_BUS_STATE_RX_ACK BIT1 ///< ���յ�ACK��Ӧ
#define I2C_BUS_STATE_Done BIT0   ///< �������

#define I2C_IF_TC 0       ///< ��������ж�
#define I2C_IF_BUS_ERR 1  ///< ���ߴ����ж�
#define I2C_IF_STOP 2     ///< STOP�¼��ж�
#define I2C_IF_NACK 3     ///< NACK�¼��ж�
#define I2C_IF_ADDR_CMP 4 ///< Ӳ����ַƥ���ж�

typedef struct
{
    __IO u32 ADDR;  ///< 0x00 I2C ��ַ�Ĵ���
    __IO u32 CFG;   ///< 0x04 I2C ���üĴ���
    __IO u32 SCR;   ///< 0x08 I2C ״̬�Ĵ���
    __IO u32 DATA;  ///< 0x0C I2C ���ݼĴ���
    __IO u32 MSCR;  ///< 0x10 I2C ��ģʽ�Ĵ���
    __IO u32 BCR;   ///< 0x14 I2C ������ƼĴ���
    __IO u32 BSIZE; ///< 0x18 I2C ���䳤�ȼĴ���

} I2C_TypeDef;

#ifndef I2C0
#define I2C0 ((I2C_TypeDef *)I2C0_BASE)
#endif
typedef struct
{
    u32 ADRCMP;         ///< I2C Ӳ����ַ�Ƚ�ʹ�ܿ���
    u32 MST_MODE;       ///< I2C ��ģʽʹ���ź�
    u32 SLV_MODE;       ///< I2C ��ģʽʹ���ź�
    u32 BUSRT_EN;       ///< I2C DMA����ʹ��
    u32 BaudRate;       ///< I2C ������
    u32 IE;             ///< I2C �ж�ʹ���ź�
    u32 TC_IE;          ///< I2C ���ݴ�������ж�ʹ��
    u32 BUS_ERR_IE;     ///< I2C ���ߴ����¼��ж�ʹ���ź�
    u32 STOP_IE;        ///< I2C STOP �¼��ж�ʹ���ź�
    u32 BURST_NACK;     ///< NACK �¼��ж�ʹ��
    u32 BURST_ADDR_CMP; ///< Ӳ����ַƥ���ж�ʹ��
} I2C_InitTypeDef;

void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *this); ///< I2C��ʼ��
void I2C_StructInit(I2C_InitTypeDef *);                  ///< I2C���ýṹ���ʼ��
void I2C_ClearIRQFlag(I2C_TypeDef *I2Cx, u16 I2C_IF_x);
u8 I2C_GetIRQFlag(I2C_TypeDef *I2Cx, u16 I2C_IF_x);
u8 I2C_GetFlag(I2C_TypeDef *I2Cx, u16 I2C_IF_x);

void I2C_Enable(I2C_TypeDef *I2Cx);
void I2C_Disable(I2C_TypeDef *I2Cx);
void I2C_SetBaudRate(u32 BaudRate);

u8 I2c_ReadBusState(I2C_TypeDef *I2Cx, u16);    ///< ��I2C����״̬
void I2c_ClearBusState(I2C_TypeDef *I2Cx, u16); ///< I2C����״̬��λ
void I2C_SendData(I2C_TypeDef *I2Cx, u8 n);
u8 I2C_ReadData(I2C_TypeDef *I2Cx);
#endif /*__lks32mc03x_I2C_H */
