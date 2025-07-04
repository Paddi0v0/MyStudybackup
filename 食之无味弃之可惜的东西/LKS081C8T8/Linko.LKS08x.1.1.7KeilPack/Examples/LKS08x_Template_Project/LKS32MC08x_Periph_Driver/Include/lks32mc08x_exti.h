/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc08x_exti.h
 * �ļ���ʶ��
 * ����ժҪ�� EXTI�ⲿ�ж�����ͷ�ļ�
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
 
#ifndef __lks32mc08x_EXTI_H
#define __lks32mc08x_EXTI_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct  
{
   __IO uint32_t CR0;
   __IO uint32_t CR1;
   __IO uint32_t IF;   
}EXTI_TypeDef;

typedef struct
{
   uint32_t Pad0Trig;   /*������ʽ*/
   uint32_t Pad1Trig;
   uint32_t Pad2Trig;
   uint32_t Pad3Trig;
   uint32_t Pad4Trig;
   uint32_t Pad5Trig;
   uint32_t Pad6Trig;
   uint32_t Pad7Trig;	
   uint32_t Pad8Trig;	
   uint32_t Pad9Trig;	
   uint32_t Pad10Trig;	
   uint32_t Pad11Trig;	
   uint32_t Pad12Trig;	
   uint32_t Pad13Trig;	
   uint32_t Pad14Trig;	
   uint32_t Pad15Trig;	
}EXTI_InitTypeDef;


#define EXTI_NONE_TRG     ((uint32_t)0x00) /*������*/
#define EXTI_FALL_TRG     ((uint32_t)0x01) /*�½��ش���*/
#define EXTI_RISE_TRG     ((uint32_t)0x02) /*�����ش���*/
#define EXTI_FALLRISE_TRG ((uint32_t)0x03) /*˫�ش���*/


void EXTI_Init(EXTI_TypeDef* EXTIx, EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);

uint32_t EXTI_ReadIFFlag(EXTI_TypeDef* EXTIx);
void EXTI_ResetIF(EXTI_TypeDef* EXTIx, uint32_t temp);

#endif /*__lks32mc08x_EXTI_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
