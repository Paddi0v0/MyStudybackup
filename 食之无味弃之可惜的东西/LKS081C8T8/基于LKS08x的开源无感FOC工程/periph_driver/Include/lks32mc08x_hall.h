/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc08x_hall.h
 * �ļ���ʶ��
 * ����ժҪ�� HALL����ͷ�ļ�
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

#ifndef __lks32mc08x_HALL_H
#define __lks32mc08x_HALL_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct
{
   __IO uint32_t CFG;  
   __IO uint32_t INFO;
   __IO uint32_t WIDTH;
   __IO uint32_t TH;
   __IO uint32_t CNT;
} HALL_TypeDef;

#ifndef HALL
#define HALL     ((HALL_TypeDef *) HALL_BASE)
#endif

typedef struct
{
   uint16_t FilterLen;       /*�˲�����,0��Ӧ����1��32767��Ӧ����32768�˲�����*/
   uint8_t ClockDivision;    /*��Ƶ 0~3:/1 /2 /4 /8*/
   uint8_t Filter75_Ena;     /*ʹ�ܵ�һ��7/5�˲�,�ߵ�ƽ��Ч*/
   uint8_t HALL_Ena;         /*ʹ��HALL,�ߵ�ƽ��Ч*/
   uint8_t Capture_IRQ_Ena;  /*HALL�źű仯�ж�ʹ��,�ߵ�ƽ��Ч*/
   uint8_t OverFlow_IRQ_Ena; /*HALL����������ж�ʹ��,�ߵ�ƽ��Ч*/
   uint8_t softIE;           /* ����ж�ʹ�� */
   uint32_t CountTH;         /*HALL����������ֵ*/
} HALL_InitTypeDef;


#define HALL_CLK_DIV1 ((uint32_t)0x00)  /*HALLʱ��Ϊϵͳ��ʱ��1��Ƶ*/
#define HALL_CLK_DIV2 ((uint32_t)0x01)  /*HALLʱ��Ϊϵͳ��ʱ��2��Ƶ*/
#define HALL_CLK_DIV4 ((uint32_t)0x02)  /*HALLʱ��Ϊϵͳ��ʱ��4��Ƶ*/
#define HALL_CLK_DIV8 ((uint32_t)0x03)  /*HALLʱ��Ϊϵͳ��ʱ��8��Ƶ*/
/*HALL�ж�ʹ�ܺͱ�־����*/
#define HALL_CAPTURE_EVENT ((uint32_t)0x00010000)   /*HALL�źű仯�ж�*/
#define HALL_OVERFLOW_EVENT ((uint32_t)0x00020000)  /*HALL��������ж�*/

void HALL_Init(HALL_InitTypeDef *HALL_InitStruct);
void HALL_StructInit(HALL_InitTypeDef *HALL_InitStruct);

uint32_t HALL_GetFilterValue(void);
uint32_t HALL_GetCaptureValue(void);
uint32_t HALL_WIDCount(void);
uint32_t HALL_GetCount(void);

uint32_t HALL_GetIRQFlag(uint32_t tempFlag);
void HALL_ClearIRQFlag(uint32_t tempFlag);

#endif /*__lks32mc08x_HALL_H */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
