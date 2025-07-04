/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� LKS32MC081_DMA.c
 * �ļ���ʶ��
 * ����ժҪ�� DMA������������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2020��08��1��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��08��1��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
#include "lks32mc08x_DMA.h"
#include "string.h"

/*******************************************************************************
 �������ƣ�    void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct)
 ����������    DMA�ṹ���ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.08.1      V1.0           howlet              ����
 *******************************************************************************/
void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct)
{
    memset(DMAInitStruct, 0, sizeof(DMA_InitTypeDef));
}

/*******************************************************************************
 �������ƣ�    void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct)
 ����������    DMA��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.08.1      V1.0           howlet              ����
 *******************************************************************************/
void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct)
{
    /* ͨ�����üĴ��� ��ֵ */
    DMAx->DMA_CCR = 0;
    DMAx->DMA_CTMS = DMAInitStruct->DMA_TIMES | (DMAInitStruct->DMA_ROUND<<16); /* ��������Ĵ��� */
    DMAx->DMA_CPAR = DMAInitStruct->DMA_CPAR;   /* �����ַ�Ĵ��� */
    DMAx->DMA_CMAR = DMAInitStruct->DMA_CMAR;   /* �ڴ��ַ�Ĵ��� */
    DMAx->DMA_CCR  =  (DMAInitStruct->DMA_IRQ_EN<<1) | (DMAInitStruct->DMA_DIR<<4) |\
                     (DMAInitStruct->DMA_CIRC<<5) | (DMAInitStruct->DMA_PINC<<6) | (DMAInitStruct->DMA_MINC<<7) |\
                     (DMAInitStruct->DMA_PBTW<<8) | (DMAInitStruct->DMA_MBTW<<10) | (DMAInitStruct->DMA_REQ_EN<<12);

    DMA_CTRL  = 0x0001;  /*enable dma, mcu has higher priorit */
}  

/*******************************************************************************
 �������ƣ�    void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
 ����������    ʹ��DMAͨ��
 �����ı�    ��
 ���������    DMAx��DMAͨ��ѡ��
              Channel_EN��ENABLE��ʹ��DMAͨ����DISABLE���ر�DMAͨ��ʹ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            ����
 *******************************************************************************/
void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
{
   if(Channel_EN)
	 {
	   DMAx->DMA_CCR  |= BIT0;/*ʹ��DMA����*/
	 }else{
	   DMAx->DMA_CCR  &= ~BIT0; /*�ر�ͨ��ʹ��*/
	 }
}

/*******************************************************************************
 �������ƣ�    uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if)
 ����������    ��ȡDMA�жϱ�־
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    DMA�жϱ�־
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            ����
 *******************************************************************************/
uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if)
{
	 u8 reg = 0;	 
   if((DMAx->DMA_CCR) & BIT1)/*��������ж�ʹ��*/
	 {
		  reg = timer_if & 0x0f;
	    if(DMA_IF & reg)
			{
			  return 1;
			}
	 }
   if((DMAx->DMA_CCR) & BIT2)/*�����ж�ʹ��*/
	 {
		  reg = timer_if & 0xf0;
	    if(DMA_IF & reg)
			{
			  return 1;
			}
	 }
	 return 0;
}
/*******************************************************************************
 �������ƣ�    void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag)
 ����������    ���DMA�жϱ�־
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            ����
 *******************************************************************************/
void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag)
{
    DMA_IF = tempFlag;
}
