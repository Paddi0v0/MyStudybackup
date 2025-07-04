/*******************************************************************************
 * ��Ȩ���� (C)2021, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� main.c
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ��CRCУ������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ�
 * ������ڣ� 2021��3��12��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2021��3��12��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�yangzj
 * �޸����ݣ�����

 * �޸ļ�¼2��
 * �޸����ڣ�2021��7��05��
 * �� �� �ţ�V 1.1
 * �� �� �ˣ�yangzj
 * �޸����ݣ���ȫcrc��ʽ
 
 * �޸ļ�¼2��
 * �޸����ڣ�2021��3��24��
 * �� �� �ţ�V 1.2
 * �� �� �ˣ�yangzj
 * �޸����ݣ��޸�CRC16/CRC32�������ݱ���Ϊu16/u32������
 *
 *******************************************************************************/
#include "hardware_config.h"
#include "CRC_fun.h"
void Hardware_init(void);
/*******************************************************************************
 �������ƣ�    int main(void)
 ����������    ���������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/

#define crc_data_define crc_data
#define crc_data_len_define 5
u8 crc_data[5] = {1, 2, 3, 4, 5};
u8 crc8_answer, crc8_ITU_answer, crc8_ROHC_answer, crc8_MAXIM_answer;
u16 crc16_IBM_answer, crc16_MAXIM_answer, crc16_USB_answer, crc16_MODBUS_answer, crc16_CCITT_answer, crc16_CCITT_FALSE_answer, crc16_X25_answer, crc16_XMODEM_answer, crc16_DNP_answer;
u32 crc32_answer, crc32_MPEG_2_answer;

int main(void)
{
    Hardware_init(); /* Ӳ����ʼ�� */
    crc8_answer = crc8(crc_data_define, crc_data_len_define);
    crc8_ITU_answer = crc8_ITU(crc_data_define, crc_data_len_define);
    crc8_ROHC_answer = crc8_ROHC(crc_data_define, crc_data_len_define);
    crc8_MAXIM_answer = crc8_MAXIM(crc_data_define, crc_data_len_define);

    crc16_IBM_answer = crc16_IBM(crc_data_define, crc_data_len_define);
    crc16_MAXIM_answer = crc16_MAXIM(crc_data_define, crc_data_len_define);
    crc16_USB_answer = crc16_USB(crc_data_define, crc_data_len_define);
    crc16_MODBUS_answer = crc16_MODBUS(crc_data_define, crc_data_len_define);
    crc16_CCITT_answer = crc16_CCITT(crc_data_define, crc_data_len_define);
    crc16_CCITT_FALSE_answer = crc16_CCITT_FALSE(crc_data_define, crc_data_len_define);
    crc16_X25_answer = crc16_X25(crc_data_define, crc_data_len_define);
    crc16_XMODEM_answer = crc16_XMODEM(crc_data_define, crc_data_len_define);
    crc16_DNP_answer = crc16_DNP(crc_data_define, crc_data_len_define);

    crc32_answer = crc32(crc_data_define, crc_data_len_define);
    crc32_MPEG_2_answer = crc32_MPEG_2(crc_data_define, crc_data_len_define);
    for (;;)
    {
    }
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
