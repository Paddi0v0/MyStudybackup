/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc03x_IWDG.h
 * �ļ���ʶ��
 * ����ժҪ�� ���Ź�����ͷ�ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� YangZJ
 * ������ڣ� 2021��11��09��
 *
 *******************************************************************************/
#ifndef __lks32mc05x_IWDG_H
#define __lks32mc05x_IWDG_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct
{
    u32 WDG_EN;             // �������Ź�ʹ��
    u32 RTH;                // ���Ź���ʱ��λʱ��
}IWDG_InitTypeDef;

#define WDT_RTH_2S   3
#define WDT_RTH_4S   2
#define WDT_RTH_8S   1
#define WDT_RTH_64S  0

void IWDG_Init(IWDG_InitTypeDef *);                 // ���Ź���ʼ��
void IWDG_StrutInit(IWDG_InitTypeDef *);            // ���Ź����ýṹ���ʼ��
void IWDG_DISABLE(void);                            // �رտ��Ź�
void IWDG_ENABLE(void);                             // ʹ�ܿ��Ź�
void IWDG_Feed(void);                               // ���Ź�ι��
#endif /*__lks32mc03x_IWDG_H */
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
