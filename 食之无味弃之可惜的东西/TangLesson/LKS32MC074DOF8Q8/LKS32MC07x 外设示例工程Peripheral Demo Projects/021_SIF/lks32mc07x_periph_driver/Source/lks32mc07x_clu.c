/**
 * @file
 * @defgroup clu
 * @brief        CLU�⺯��
 * @author       YangZJ
 * @date         2023-02-27
 * @{
 */
#include "lks32mc07x_clu.h"

/**
 * @brief CLU�����ʼ��
 */
void CLU_Init(CLU_TypeDef *CLUx, u8 CLU_MODEL_CLUx, CLU_InitTypeDef *this)
{
    CLU_Enable(CLUx);
    if ((CLU_MODEL_CLUx & CLU_MODEL_CLU0) != 0)
    {
        if (this->EN == 0)
        {
            CLUx->EN &= ~BIT0;
        }
        else
        {
            CLUx->EN |= BIT0;
        }
        if (this->FIE == 0)
        {
            CLUx->IE &= ~BIT0;
        }
        else
        {
            CLUx->IE |= BIT0;
        }
        if (this->RIE == 0)
        {
            CLUx->IE &= ~BIT1;
        }
        else
        {
            CLUx->IE |= BIT1;
        }
        CLUx->MX = (u32)((CLUx->MX & ~0x000f) | (this->MXA << 4) | (this->MXB));
        CLUx->FN = (u32)((CLUx->FN & ~0x000f) | (this->FN));
        CLUx->CF = (u32)((CLUx->CF & ~0x000f) | (this->OUTSEL << 7) | (this->OEN << 6) | (this->CLKINV << 2) | (this->CLKSEL));
        CLUx->CF |= BIT3;
    }
    if ((CLU_MODEL_CLUx & CLU_MODEL_CLU1) != 0)
    {
        if (this->EN == 0)
        {
            CLUx->EN &= ~BIT1;
        }
        else
        {
            CLUx->EN |= BIT1;
        }
        if (this->FIE == 0)
        {
            CLUx->IE &= ~BIT2;
        }
        else
        {
            CLUx->IE |= BIT2;
        }
        if (this->RIE == 0)
        {
            CLUx->IE &= ~BIT3;
        }
        else
        {
            CLUx->IE |= BIT3;
        }
        CLUx->MX = (u32)((CLUx->MX & ~0x00f0) | (this->MXA << 12) | (this->MXB << 8));
        CLUx->FN = (u32)((CLUx->FN & ~0x00f0) | (this->FN << 8));
        CLUx->CF = (u32)((CLUx->CF & ~0x00f0) | (this->OUTSEL << 15) | (this->OEN << 14) | (this->CLKINV << 10) | (this->CLKSEL << 8));
        CLUx->CF |= BIT11;
    }
    if ((CLU_MODEL_CLUx & CLU_MODEL_CLU2) != 0)
    {
        if (this->EN == 0)
        {
            CLUx->EN &= ~BIT2;
        }
        else
        {
            CLUx->EN |= BIT2;
        }
        if (this->FIE == 0)
        {
            CLUx->IE &= ~BIT4;
        }
        else
        {
            CLUx->IE |= BIT4;
        }
        if (this->RIE == 0)
        {
            CLUx->IE &= ~BIT5;
        }
        else
        {
            CLUx->IE |= BIT5;
        }
        CLUx->MX = (u32)((CLUx->MX & ~0x0f00) | (this->MXA << 20) | (this->MXB << 16));
        CLUx->FN = (u32)((CLUx->FN & ~0x0f00) | (this->FN << 16));
        CLUx->CF = (u32)((CLUx->CF & ~0x0f00) | (this->OUTSEL << 23) | (this->OEN << 22) | (this->CLKINV << 18) | (this->CLKSEL << 16));
        CLUx->CF |= BIT19;
    }
    if ((CLU_MODEL_CLUx & CLU_MODEL_CLU3) != 0)
    {
        if (this->EN == 0)
        {
            CLUx->EN &= ~BIT3;
        }
        else
        {
            CLUx->EN |= BIT3;
        }
        if (this->FIE == 0)
        {
            CLUx->IE &= ~BIT6;
        }
        else
        {
            CLUx->IE |= BIT6;
        }
        if (this->RIE == 0)
        {
            CLUx->IE &= ~BIT7;
        }
        else
        {
            CLUx->IE |= BIT7;
        }
        CLUx->MX = (u32)((CLUx->MX & ~0xf000) | (this->MXA << 28) | (this->MXB << 24));
        CLUx->FN = (u32)((CLUx->FN & ~0xf000) | (this->FN << 24));
        CLUx->CF = (u32)((CLUx->CF & ~0xf000) | (this->OUTSEL << 31) | (this->OEN << 30) | (this->CLKINV << 26) | (this->CLKSEL << 24));
        CLUx->CF |= BIT27;
    }
}
/**
 * @brief CLU��ʼ���ṹ���ʼ��
 */
void CLU_StructInit(CLU_InitTypeDef *this)
{
    this->FIE = 0;    ///< �½����ж�ʹ��
    this->RIE = 0;    ///< �������ж�ʹ��
    this->EN = 0;     ///< ģ��ʹ�� 0�ر�ģ�� 1��ģ��
    this->MXA = 0;    ///< A���븴��ѡ�� ��ʽ CLU_MXA_CLUa_b aȡֵ��ΧΪ[0 1 2 3] ��Ӧ��ͬ��CLU b��ʾѡ��ͬ�������ź�
    this->MXB = 0;    ///< B���븴��ѡ�� ��ʽ CLU_MXB_CLUa_b aȡֵ��ΧΪ[0 1 2 3] ��Ӧ��ͬ��CLU b��ʾѡ��ͬ�������ź�
    this->FN = 0;     ///< ��ֵ�� ��ʽ CLU_FN_x xȡֵ��ΧΪ[A NOTA A_NOTB ...]��ӦΪ������ʽ�µ���С�� ���� y=a+bc+!c ��ʾΪ CLU_FN_A|CLU_FN_B_C|CLU_FN_NOTC
    this->OUTSEL = 0; ///< ���ѡ�� 0: D ��������� 1: LUT ���
    this->OEN = 0;    ///< ���ʹ�� 0: �ر� 1: ʹ��
    this->CLKINV = 0; ///< D ������ʱ�ӵ�ƽ 0: ͬ�� 1: ����
    this->CLKSEL = 0; ///< D ������ʱ��ѡ�� 0x0: ��λ���� 0x1: MXA ���� 0x2: ALTCLK0 0x3: ALTCLK1
}
/**
 * @brief CLUģ��ʱ��ʹ��
 */
void CLU_Enable(CLU_TypeDef *CLUx)
{
    SYS_ModuleClockCmd(SYS_Module_CL0, ENABLE);
}
/**
 * @brief CLUģ��ʱ�ӹر�
 */
void CLU_Disable(CLU_TypeDef *CLUx)
{
    SYS_ModuleClockCmd(SYS_Module_CL0, DISABLE);
}
/**
 * @brief ��ȡCLU�����ֵ
 */
u8 CLU_ReadOutVolue(CLU_TypeDef *CLUx,u8 CLU_MODEL_CLUx)
{
    u8 OutVolue;
    if(CLUx->OUT & CLU_MODEL_CLUx)
    {
        OutVolue = 1;
    }
    else
    {
        OutVolue = 0;
    }
    return OutVolue;
}

/*! @} */
