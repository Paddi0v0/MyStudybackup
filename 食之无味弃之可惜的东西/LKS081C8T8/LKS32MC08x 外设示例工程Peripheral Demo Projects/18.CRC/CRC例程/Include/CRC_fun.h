/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� CRC_fun.h
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ��CRC����
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� yangzj
 * ������ڣ� 2021��7��05��
 *
 * �޸ļ�¼1��
 * �޸����ڣ� 2021��7��05��
 * �� �� �ţ� V 1.0
 * �� �� �ˣ� yangzj
 * �޸����ݣ� ����
 *
 *******************************************************************************/

/*------------------------------prevent recursive inclusion -------------------*/
#include "hardware_config.h"
#ifndef __CRC_FUN_H_
#define __CRC_FUN_H_
u8 crc8(u8 *pData,u32 uLen);
u8 crc8_ITU(u8 *pData,u32 uLen);
u8 crc8_ROHC(u8 *pData,u32 uLen);
u8 crc8_MAXIM(u8 *pData,u32 uLen);

u16 crc16_IBM(u8 *pData,u32 uLen);
u16 crc16_MAXIM(u8 *pData,u32 uLen);
u16 crc16_USB(u8 *pData,u32 uLen);
u16 crc16_MODBUS(u8 *pData,u32 uLen);
u16 crc16_CCITT(u8 *pData,u32 uLen);
u16 crc16_CCITT_FALSE(u8 *pData,u32 uLen);
u16 crc16_X25(u8 *pData,u32 uLen);
u16 crc16_XMODEM(u8 *pData,u32 uLen);
u16 crc16_DNP(u8 *pData,u32 uLen);

u32 crc32(u8 *pData,u32 uLen);
u32 crc32_MPEG_2(u8 *pData,u32 uLen);
#endif
