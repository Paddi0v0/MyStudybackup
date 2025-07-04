 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_sif.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� SIF����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>myx   <td>�޸�
 * </table>
 */
#ifndef _CONNIE_SIF_H_
#define _CONNIE_SIF_H_

#include "lks32mc07x_lib.h"

typedef struct
{
    uint8_t  SIF_DONE;           		/* ������ɺ�Ĭ�ϵ�ƽ 1���ߵ�ƽ 0���͵�ƽ */
    uint8_t  SIF_SYNC;               	/* ͬ���ź�Ĭ�ϵ�ƽ 1���ߵ�ƽ 0���͵�ƽ */
    uint8_t  SIF_SYNC_PULSE;          	/* ͬ���ź��Ƿ��е�ƽ��ת 1���� 0���� */
    uint8_t  SIF_DONE_VLD;             	/* ���޽����ź� 1���� 0���� */
    uint8_t  SIF_SYNC_VLD;           	/* ����ͬ���ź� 1���� 0���� */
    uint8_t  SIF_RATIO;               	/* ����ռ�ձ� 1��3��1  0��2��1 */
    uint8_t  SIF_MSB;                	/* ���ݴ���˳�� 1�����ֽ���ǰ 0�����ֽ���ǰ */
    uint8_t  SIF_EN;              		/* SIFģ��ʹ�ܣ�����Ч */
    uint16_t SIF_TOSC;                	/* ʱ������ SIF_TOSC+1 333NSΪ��λ ��� 4096 * 333 = 1364US */
    uint16_t SIF_TSTH1;                	/* ͬ��ʱ�� ��SIF_TSTH1 +1�� * SIF_TOSC ���1024 * SIF_TOSC * 32 */
	  uint8_t  SIF_TDTH1;                	/* �����ź�ʱ�� ��SIF_TDTH1 + 1��MS */
    uint8_t  SIF_IRQ_IF;               	/* SIF�жϱ�־λ 1���� 0���� */
    uint8_t  SIF_DMA_EN;              	/* DMA ����ʹ��  ����Ч */
	  uint8_t  SIF_IRQ_EN;              	/* SIF�ж�ʹ�� ����Ч */

} SIF_InitTypeDef;

typedef struct
{
    __IO uint32_t SIFx_CFG;
    __IO uint32_t SIFx_TOSC;
    __IO uint32_t SIFx_TSTH1;
    __IO uint32_t SIFx_TDTH1;
	__IO uint32_t SIFx_IRQ;
	
} SIF_RegTypeDef;

/*�ж�ʹ�ܶ���*/
#define SIF_IE_Enable BIT0   ///< SIF�ж�ʹ�ܿ��ء�Ĭ��ֵΪ0
#define SIF_DMA_Enable BIT1   ///< SIF DMA����ʹ�ܿ��ء�Ĭ��ֵΪ0


#define SIF0  ((SIF_RegTypeDef *) (SIF0_BASE))

/*�жϱ�־����*/
#define SPI_IF_CLEAR BIT4 ///< SPI���䣬����¼����ߵ�ƽ��Ч��д1�����

void SIF_StructInit(SIF_InitTypeDef *SIFInitStruct);
void SIF_Init(SIF_RegTypeDef *SIFx, SIF_InitTypeDef *SIFInitStruct);
void SIF_Timebase_set(u32 tosc);
void SIF_TH_set(u32 tsh,u32 tdh);
void SIF_Senddata(u8 Data);
void SIF_DMA_IsEnable(u8 state);
void SIF_Irq_IsEnable(u8 state);
void SIF_Clear_Irq(void);

#endif


