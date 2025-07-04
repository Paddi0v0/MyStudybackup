/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Main.c
 * �ļ���ʶ��
 * ����ժҪ�� ����������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2020��8��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��8��16��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet Li
 * �޸����ݣ�����
 *
 *******************************************************************************/
#include "LKS32MC07x_lib.h" // Device header

void Hardware_init(void);
/*******************************************************************************
 �������ƣ�    int main(void)
 ����������    ���������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ʹ��Ӳ��CRC���㳣����CRC����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/3/10     V1.0           YangZJ            ����
 *******************************************************************************/

u8 crc_data[5] = {1, 2, 3, 4, 5};
u8 crc8_answer, crc8_ITU_answer, crc8_ROHC_answer, crc8_MAXIM_answer;
u16 crc16_IBM_answer, crc16_MAXIM_answer, crc16_USB_answer, crc16_MODBUS_answer, crc16_CCITT_answer, crc16_CCITT_FALSE_answer, crc16_X25_answer, crc16_XMODEM_answer, crc16_DNP_answer;
u32 crc32_answer, crc32_MPEG_2_answer;

int main(void)
{
    //    Hardware_init(); /* CRC����Ҫ��ʼ��������ֱ��ʹ�� */

    crc8_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_8);                           ///< ����CRC_8
    crc8_ITU_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_8_ITU);                   ///< ����CRC_8_ITU
    crc8_ROHC_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_8_ROHC);                 ///< ����CRC_8_ROHC
    crc8_MAXIM_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_8_MAXIMx);              ///< ����CRC_8_MAXIMx
    crc16_IBM_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_IBMx1);               ///< ����CRC_16_IBMx1
    crc16_MAXIM_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_MAXIM);             ///< ����CRC_16_MAXIM
    crc16_USB_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_USBx1);               ///< ����CRC_16_USBx1
    crc16_MODBUS_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_MODBU);            ///< ����CRC_16_MODBU
    crc16_CCITT_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_CCITT);             ///< ����CRC_16_CCITT
    crc16_CCITT_FALSE_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_CCITT_FALSE); ///< ����CRC_16_CCITT_FALSE
    crc16_X25_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_X25);                 ///< ����CRC_16_X25
    crc16_XMODEM_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_XMODEM);           ///< ����CRC_16_XMODEM
    crc16_DNP_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_16_DNP);                 ///< ����CRC_16_DNP
    crc32_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_32);                         ///< ����CRC_32
    crc32_MPEG_2_answer = CRC_CalcCommon(CRC0, crc_data, sizeof(crc_data), CRC_MODEL_CRC_32_MPEG_2);           ///< ����CRC_32_MPEG_2
    for (;;)
    {
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
