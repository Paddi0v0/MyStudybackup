/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc081_flash.c
 * �ļ���ʶ��
 * ����ժҪ�� Flash������������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2019��3��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2019��3��5��
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

#include "lks32mc08x_cmp.h"
/*******************************************************************************
 �������ƣ�    void CMP_Init(CMP_InitTypeDef* CMP_InitStruct)
 ����������    CMP��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2018/11/26      V1.0           Howlet Li          ����
 *******************************************************************************/
void CMP_Init(CMP_InitTypeDef* this)
{
    // Comparator��ʼ��
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_CMP1,this->CMP1_EN);
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_CMP0,this->CMP0_EN);
    SYS_WR_PROTECT = 0x7A83;
    
    SYS_AFE_REG3 |= (u32)(this->CMP0_SELN << 2) | (u32)(this->CMP1_SELN << 4) | (u32)(this->CMP0_SELP << 8) |
                    (u32)(this->CMP1_SELP << 12) | (u32)(this->CMP_HYS << 7);

    /* CMP_HYS �Ƚ����ͻع��ܣ�Ĭ��20mV, д0�ر� */

    SYS_AFE_REG1 |= this->CMP_FT; /* ���ٱȽϣ�Enable�� 30nS */

    SYS_AFE_REG5 |= (u32)(this->CMP0_EN << 6) | (u32)(this->CMP1_EN << 7);
    
    SYS_WR_PROTECT = 0;
    
    CMP -> IE      = (u32)(this -> CMP1_IE           <<  1) | (u32)(this -> CMP0_IE);
    if(this->CLK10_EN == DISABLE)
    {
        if(this->CMP1_IN_EN || this->CMP0_IN_EN)
        {
            CMP->TCLK = BIT3;
        }
        else
        {
            CMP->TCLK = 0;
        }
    }
    else
    {
        CMP->TCLK  = (u32)(this -> FIL_CLK10_DIV16   <<  4) | (u32)(this -> CLK10_EN          <<  3) |
                     (u32)(this -> FIL_CLK10_DIV2);
    }
    CMP -> CFG     = (u32)(this -> CMP1_W_PWM_POL    <<  7) | (u32)(this -> CMP1_IRQ_TRIG     <<  6) |
                     (u32)(this -> CMP1_IN_EN        <<  5) | (u32)(this -> CMP1_POL          <<  4) |
                     (u32)(this -> CMP0_W_PWM_POL    <<  3) | (u32)(this -> CMP0_IRQ_TRIG     <<  2) |
                     (u32)(this -> CMP0_IN_EN        <<  1) | (u32)(this -> CMP0_POL);           
    CMP -> BLCWIN  = (u32)(this -> CMP1_CHN3P_WIN_EN <<  7) | (u32)(this -> CMP1_CHN2P_WIN_EN <<  6) |
                     (u32)(this -> CMP1_CHN1P_WIN_EN <<  5) | (u32)(this -> CMP1_CHN0P_WIN_EN <<  4) |
                     (u32)(this -> CMP0_CHN3P_WIN_EN <<  3) | (u32)(this -> CMP0_CHN2P_WIN_EN <<  2) |
                     (u32)(this -> CMP0_CHN1P_WIN_EN <<  1) | (u32)(this -> CMP0_CHN0P_WIN_EN);
    CMP -> IF = 3;
}

/*******************************************************************************
 �������ƣ�    void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
 ����������    CMP��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2018/11/26      V1.0           Howlet Li          ����
 *******************************************************************************/
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
{

    memset(CMP_InitStruct, 0, sizeof(CMP_InitTypeDef));
}

/*******************************************************************************
 �������ƣ�    u8 CMP_GetIRQFlag(u8 CMPx)
 ����������    ��ȡ�Ƚ����ж�
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    TIM�жϱ�־
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/04/21    V1.0           Yangzj            ����
 *******************************************************************************/
u8 CMP_GetIRQFlag(u8 CMPx)
{
	if(CMPx == CMP0)
    {
        if(CMP_IF&BIT0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else if(CMPx == CMP1)
    {
        if(CMP_IF&BIT1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

/*******************************************************************************
 �������ƣ�    void CMP_ClearIRQFlag(u8 CMPx)
 ����������    ���TIM�жϱ�־
 �����ı�    ��
 ���������    TIM�жϱ�־
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/04/11      V1.0         HuangMG             ����
 *******************************************************************************/
void CMP_ClearIRQFlag(u8 CMPx)
{
	if(CMPx == CMP0)
    {
        CMP_IF=BIT0;
    }
    else if(CMPx == CMP1)
    {
        CMP_IF=BIT1;
    }
}
