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

 /** 
 *@brief SIF �Ĵ�������
 */
typedef struct 
{
    /* data */
    __IO u32 CFG       ;     //0x00
    __IO u32 TOSC      ;     //0x04
    __IO u32 STH1     ;     //0x08
    __IO u32 DTH1     ;     //0x0C
    __IO u32 IRQ       ;     //0x10
    __IO u32 WDATA     ;     //0x14

} STF_Typedef;

 /** 
 *@brief SIFģ�����ַ����
 */
#define SIF ((STF_Typedef*)(SIF0_BASE + 0x00))
#define SIF0 ((STF_Typedef *)SIF0_BASE)

 /** 
 *@brief SPI��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
    u8 EN;          ///<  SIFģ��ʹ�ܣ�0���رգ�1������
    u8 MSB;        ///<  ���ݿ��С�˶���
    u8 RATIO;        ///<  ģ������ռ�ձ�����
    u8 SYS_VLD;  ///<  ����ͬ���ź�
    u16 DONE_VLD;   ///<  ���޽����ź�
    u16 SYS_PLUSE;  ///<  ͬ���ź����������ź�
    u16 SYSD;     ///<  ͬ���ź�Ĭ�ϵ�ƽ
    u16 DONED;       ///< �����ź�Ĭ�ϵ�ƽ
} SIF_InitTypeDef;

/*�ж�ʹ�ܶ���*/
#define SIF_IE_Enable BIT0   ///< SIF�ж�ʹ�ܿ��ء�Ĭ��ֵΪ0
#define SIF_DMA_Enable BIT1   ///< SIF DMA����ʹ�ܿ��ء�Ĭ��ֵΪ0

/*�жϱ�־����*/
#define SPI_IF_CLEAR BIT4 ///< SPI���䣬����¼����ߵ�ƽ��Ч��д1�����

void SIF_Init(STF_Typedef* SIFx ,SIF_InitTypeDef* SIF_InitStruct);
void SIF_StructInit(SIF_InitTypeDef* SPI_InitStruct);
void SIF_Timebase_set(u32 tosc);
void SIF_TH_set(u32 tsh,u32 tdh);
void SIF_Senddata(u8 Data);
void SIF_DMA_IsEnable(u8 state);
void SIF_Irq_IsEnable(u8 state);
void SIF_Clear_Irq(void);

#endif


