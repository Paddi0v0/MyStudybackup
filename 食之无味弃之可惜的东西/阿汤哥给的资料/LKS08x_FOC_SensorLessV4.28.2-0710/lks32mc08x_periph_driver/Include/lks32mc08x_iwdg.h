/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_IWDG.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ���Ź�����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2021��11��09�� <td>1.0     <td>YangZJ      <td>����
 * </table>
 */
#ifndef __LKS32MC08x_IWDG_H
#define __LKS32MC08x_IWDG_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

/** 
 *@brief IWDG�Ĵ����ṹ����
 */
typedef struct
{
    u32 WDG_EN;             /**< �������Ź�ʹ�� */
    u32 RTH;                /**< ���Ź���ʱ��λʱ��*/
}IWDG_InitTypeDef;

#define WDT_RTH_2S   3 /**< ���Ź���ʱ��λʱ��1��*/
#define WDT_RTH_4S   2 /**< ���Ź���ʱ��λʱ��4��*/
#define WDT_RTH_8S   1 /**< ���Ź���ʱ��λʱ��8��*/
#define WDT_RTH_64S  0 /**< ���Ź���ʱ��λʱ��64��*/

void IWDG_Init(IWDG_InitTypeDef *);                 // ���Ź���ʼ��
void IWDG_StrutInit(IWDG_InitTypeDef *);            // ���Ź����ýṹ���ʼ��
void IWDG_DISABLE(void);                            // �رտ��Ź�
void IWDG_ENABLE(void);                             // ʹ�ܿ��Ź�
void IWDG_Feed(uint32_t timeOut);                   // ���Ź�ι��
#endif /*__lks32mc03x_IWDG_H */
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
