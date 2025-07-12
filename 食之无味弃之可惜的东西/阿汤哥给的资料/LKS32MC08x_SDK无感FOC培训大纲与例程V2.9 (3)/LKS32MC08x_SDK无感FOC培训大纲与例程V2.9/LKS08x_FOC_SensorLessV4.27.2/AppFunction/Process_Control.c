/*******************************************************************************
 * ��Ȩ���� (C)2016, LINKO Semiconduct
 *
 * �ļ����ƣ� Process_Control.c
 * �ļ���ʶ��
 * ����ժҪ�� ����������̺�����
 * ����˵���� ��
 * ��ǰ�汾�� V 2.0
 * ��    �ߣ� WhenCai Zhao Howlet Li
 * ������ڣ� 2020��9��10��
 *
 * �޸ļ�¼1��
 *    �޸����ڣ�2019��12��26��
 *    �� �� �ţ�V 1.0
 *    �� �� �ˣ�WhenCai Zhao
 *    �޸����ݣ�����
 *
 * �޸ļ�¼2��
 *    �޸����ڣ�2020��9��10��
 *    �� �� �ţ�V2.0
 *    �� �� �ˣ�Howlet Li
 *    �޸����ݣ���ʽ����
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Global_Variable.h"


/*****************************************************************************
 * ������   : s16 OpenLoopCurRamp(stru_OpenForceRunDef *this)
 * ˵��     : ������������ 
 * ���˼· ��1.��������
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s16 OpenLoopCurRamp(stru_OpenForceRunDef *this)
{
    s32 ax;
    ax = this->nStartCurSet << 3;         
    if(this->wStartCurRamp < ax)
    {
        this->wStartCurRamp += this->nCurrentAccStep;    
        if(this->wStartCurRamp >= ax)
        {
            this->wStartCurRamp = ax;
        }
    }
    if(this->wStartCurRamp > ax)
    {
        this->wStartCurRamp -= this->nCurrentDecStep;
        if(this->wStartCurRamp < ax)
        {
            this->wStartCurRamp = ax;
        }
    }
    this->nStartCurRef = this->wStartCurRamp >> 3;

    if(this->wStartCurRamp == ax)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

/*****************************************************************************
 * ������   : void SpeedReferenceGen(stru_OpenForceRunDef *this)
 * ˵��     : �ٶ����� 
 * ���˼· ��1.�ٶ�����
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void SpeedReferenceGen(stru_OpenForceRunDef *this)
{
    s32 ax;
    ax = this->wOpen2CloseFreq << 7;
    if(this->wRampFreqRef < (ax))
    {
        this->wRampFreqRef += this->nFreqAccStep;
        if(this->wRampFreqRef >= ax)
        {
            this->wRampFreqRef = ax;
        }                                             
    }
    if(this->wRampFreqRef > ax)
    {
        this->wRampFreqRef -= this->nFreqDecStep;
        if(this->wRampFreqRef <= ax)
        {
            this->wRampFreqRef = ax;  
        }
    }
}


/*******************************************************************************
 �������ƣ�    void ReadLKS08X_UniqueID(void)
 ����������    �õ�оƬΨһIDֵ
 ���������    ��
 ���������    ��
 �� �� ֵ��    LKS_ID1,LKS_ID2,LKS_ID3,LKS_ID4�ϳ�һ��128bitΨһоƬID
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void ReadLKS08X_UniqueID(void)
{
    volatile uint32_t LKS_ID1;
    volatile uint32_t LKS_ID2;
    volatile uint32_t LKS_ID3;
    volatile uint32_t LKS_ID4;
    LKS_ID1 = Read_Trim(0x0000084C);
    LKS_ID2 = Read_Trim(0x00000850);
    LKS_ID3 = Read_Trim(0x00000854);
    LKS_ID3 = Read_Trim(0x00000858);
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



