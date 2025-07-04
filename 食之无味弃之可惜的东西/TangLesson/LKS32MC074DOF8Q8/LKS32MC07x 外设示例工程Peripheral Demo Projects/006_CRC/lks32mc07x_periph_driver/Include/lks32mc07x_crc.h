/**
 * @file
 * @brief        CRC�⺯��ͷ�ļ�
 * @author       YangZJ
 * @date         2023-01-02
*  */
#ifndef __LKS32MC07x_CRC_H
#define __LKS32MC07x_CRC_H
///< Includes ------------------------------------------------------------------
#include "lks32mc07x.h"
#include "lks32mc07x_sys.h"
#include "string.h"

typedef struct
{
    __IO u32 DR;   ///< 0x00    CRC ���ݣ�������Ϣ��/������룩�Ĵ���
    __IO u32 CR;   ///< 0x04    CRC ���ƼĴ���
    __IO u32 INIT; ///< 0x08    CRC ��ʼ��Ĵ���
    __IO u32 POL;  ///< 0x0C    CRC ���ɶ���ʽ��Ӧ�Ķ�������Ĵ���
} CRC_TypeDef;

#ifndef CRC0
#define CRC0 ((CRC_TypeDef *)CRC0_BASE)
#endif

typedef struct
{
    u32 POLY;   ///< ����ʽ
    u32 INIT;   ///< ��ʼֵ
    u32 XOROUT; ///< ������ֵ
    u8 REFIN;   ///< �������ݷ�ת
    u8 REFOUT;  ///< ������ݷ�ת
    u8 WIDTH;   ///< ���
    u8 null;    ///< ��ַ����
} CRC_InitTypeDef;

#define CRC_MODEL_CRC_8 0              ///<  8   07          00          00          false   false
#define CRC_MODEL_CRC_8_ITU 1          ///<  8   07          00          55          false   false
#define CRC_MODEL_CRC_8_ROHC 2         ///<  8   07          FF          00          true    true
#define CRC_MODEL_CRC_8_MAXIMx 3       ///<  8   31          00          00          true    true
#define CRC_MODEL_CRC_16_IBMx1 4       ///<  16  8005        0000        0000        true    true
#define CRC_MODEL_CRC_16_MAXIM 5       ///<  16  8005        0000        FFFF        true    true
#define CRC_MODEL_CRC_16_USBx1 6       ///<  16  8005        FFFF        FFFF        true    true
#define CRC_MODEL_CRC_16_MODBU 7       ///<  16  8005        FFFF        0000        true    true
#define CRC_MODEL_CRC_16_CCITT 8       ///<  16  1021        0000        0000        true    true
#define CRC_MODEL_CRC_16_CCITT_FALSE 9 ///<  16  1021        FFFF        0000        false   false
#define CRC_MODEL_CRC_16_X25 10        ///<  16  1021        FFFF        FFFF        true    true
#define CRC_MODEL_CRC_16_XMODEM 11     ///<  16  1021        0000        0000        false   false
#define CRC_MODEL_CRC_16_DNP 12        ///<  16  3D65        0000        FFFF        true    true
#define CRC_MODEL_CRC_32 13            ///<  32  04C11DB7    FFFFFFFF    FFFFFFFF    true    true
#define CRC_MODEL_CRC_32_MPEG_2 14     ///<  32  04C11DB7    FFFFFFFF    00000000    false   false

#define CRC_WIDTH_32 0 ///<  32bits
#define CRC_WIDTH_16 1 ///<  16bits
#define CRC_WIDTH_8 2  ///<  8bits
#define CRC_WIDTH_7 3  ///<  7bits

void CRC_Init(CRC_TypeDef *CRCx, CRC_InitTypeDef *CRC_InitStruct);
void CRC_StructInit(CRC_InitTypeDef *CRC_InitStruct);

u32 CRC_Calc(CRC_TypeDef *CRCx, u8 *data, u32 len);
u32 CRC_CalcCommon(CRC_TypeDef *CRCx, u8 *data, u32 len, u16 CRC_MODEL_x);
void CRC_Enable(CRC_TypeDef *CRCx);
void CRC_Disable(CRC_TypeDef *CRCx);

#endif /*__LKS32MC08x_CRC_H */
