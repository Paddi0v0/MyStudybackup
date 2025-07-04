/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc081_adc.c
 * �ļ���ʶ��
 * ����ժҪ�� ADC������������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2015��11��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2015��11��5��
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
#include "lks32mc08x_adc.h"
#include "lks32mc08x.h"

/*******************************************************************************
 �������ƣ�    void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
 ����������    ADC��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/26      V1.0           Howlet Li          ����
 *******************************************************************************/
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{

    uint16_t t_reg;

    ADCx->IE = ADC_InitStruct->IE;

    t_reg = (ADC_InitStruct->Align) | (ADC_InitStruct->DAT0_TH_Type << 1);
    ADCx->CFG = t_reg;

    t_reg = (ADC_InitStruct->MCPWM_Trigger_En) | (ADC_InitStruct->UTIMER_Trigger_En << 4) |
            (ADC_InitStruct->Trigger_Cnt << 8) | (ADC_InitStruct->Trigger_Mode << 12) |
            (ADC_InitStruct->Con_Sample << 14);

    ADCx->TRG = t_reg;//TRIG

    ADCx->CHNT0 = ADC_InitStruct->FirSeg_Ch | (ADC_InitStruct->SecSeg_Ch << 8);
    ADCx->CHNT1 = ADC_InitStruct->ThrSeg_Ch | (ADC_InitStruct->FouSeg_Ch << 8);


    if(ADC_InitStruct->Align == 0)
    {
        FLASH_CFG |= 0x00000800;
        FLASH_ADDR     = 0x59000000;
        FLASH_ADDR     = 0x00000310;
        ADC0_DC_A0 = FLASH_RDATA<<4;
			  FLASH_ADDR     = 0x00000314;
        ADC0_DC_A1 = FLASH_RDATA<<4;
			  FLASH_ADDR     = 0x00000320;
        ADC0_DC_B0 = FLASH_RDATA<<4;
			  FLASH_ADDR     = 0x00000324;
        ADC0_DC_B1 = FLASH_RDATA<<4;

        FLASH_ADDR     = 0x95000000;

        FLASH_CFG &=~0x00000800;
    }
		else
		{
			  FLASH_CFG |= 0x00000800;
        FLASH_ADDR     = 0x59000000;
        FLASH_ADDR     = 0x00000310;
        ADC0_DC_A0 = FLASH_RDATA;
			  FLASH_ADDR     = 0x00000314;
        ADC0_DC_A1 = FLASH_RDATA;
			  FLASH_ADDR     = 0x00000320;
        ADC0_DC_B0 = FLASH_RDATA;
			  FLASH_ADDR     = 0x00000324;
        ADC0_DC_B1 = FLASH_RDATA;

        FLASH_ADDR     = 0x95000000;

        FLASH_CFG &=~0x00000800;
		}
		




}

/*******************************************************************************
 �������ƣ�    void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
 ����������    ADC�ṹ���ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/26      V1.0           Howlet Li          ����
 *******************************************************************************/
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
    ADC_InitStruct->IE = 0;
    ADC_InitStruct->Align = 0;
    ADC_InitStruct->Con_Sample = 0;
    ADC_InitStruct->Trigger_Cnt = 0;
    ADC_InitStruct->FirSeg_Ch = 0;
    ADC_InitStruct->SecSeg_Ch = 0;
    ADC_InitStruct->ThrSeg_Ch = 0;
    ADC_InitStruct->FouSeg_Ch = 0;
    ADC_InitStruct->Trigger_Mode = 0;
    ADC_InitStruct->MCPWM_Trigger_En = 0;
    ADC_InitStruct->UTIMER_Trigger_En = 0;
    ADC_InitStruct->DAT0_TH_Type = 0;

}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
