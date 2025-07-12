/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_dac.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DAC������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��18�� <td>1.0     <td>Zhu Jie    <td>����
 * </table>
 */
#ifndef __LKS32MC08x_DAC_H
#define __LKS32MC08x_DAC_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
typedef struct
{
    /**
     * @brief DAC�����λ��0��3V��1��1.2V��2��4.85V
     * @see DAC_RANGE_3V0
     * @see DAC_RANGE_1V2
     * @see DAC_RANGE_4V85
     */
    uint8_t DAC_GAIN;               
	uint8_t DACOUT_EN;                /**< DAC���ʹ�ܣ�DISABLE����ʹ�ܣ�ENABLE��ʹ�ܡ� */
}DAC_InitTypeDef;

/* ------------------------------DAC������ض��� ------------------------------ */
#define DAC_RANGE_3V0                  0                   /**< DAC 3.0V���� */
#define DAC_RANGE_1V2                  1                   /**< DAC 1.2V���� */
#define DAC_RANGE_4V85                 2                   /**< DAC 4.85V���� */

void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct);/* DAC�ṹ���ʼ�� */
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct);      /* DAC��ʼ�� */
void DAC_OutputValue(uint32_t DACValue);             /* DAC�����ֵ����--������ */
void DAC_OutputVoltage(uint16_t DACVoltage);         /* DAC���ģ������ֵ����--ģ���� */

#endif

