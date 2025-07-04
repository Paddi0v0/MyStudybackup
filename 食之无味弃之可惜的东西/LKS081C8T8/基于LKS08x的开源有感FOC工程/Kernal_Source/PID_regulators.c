#include "Global_Variable.h"
#include "PID_regulators.h"

/*******************************************************************************
 �������ƣ�    s16 HL_PI_AntiDump(s16 DesiredValue, s16 MeasuredValue, stru_PIparams *pParams)
 ����������    PI����
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
s16 HL_PI_AntiDump(s16 DesiredValue, s16 MeasuredValue, stru_PIparams *pParams)
{
    s32 ProportionalPortion, PIoutput, IntegralPortion, DumpPortion;
    s32 InputError;
    s64 dwAux;

    InputError = (s32)(DesiredValue - MeasuredValue);

    ProportionalPortion = (((pParams->hKp_Gain) * InputError) >> (pParams->hKp_Divisor));

    IntegralPortion = (pParams->hKi_Gain) * InputError; /* ���㱾�εĻ����� */

    dwAux = (s64) IntegralPortion + pParams->wIntegral; /* ��������Ļ����� */

    if (dwAux > pParams->wUpper_Limit_Integral)
    {
        IntegralPortion = pParams->wUpper_Limit_Integral;
    }
    else if (dwAux < pParams->wLower_Limit_Integral)
    {
        IntegralPortion = pParams->wLower_Limit_Integral;
    }
    else
    {
        IntegralPortion = (s32)(dwAux);
    }

    PIoutput = (IntegralPortion >> pParams->hKi_Divisor) + ProportionalPortion; /* �������+���ֲ��� */

    /* controller output limitation */
    if (PIoutput > pParams->hUpper_Limit_Output)
    {
        DumpPortion = PIoutput - (pParams->hUpper_Limit_Output);
        PIoutput = pParams->hUpper_Limit_Output;
		
        if (IntegralPortion > 0)
        {
            IntegralPortion = IntegralPortion - (DumpPortion << pParams->hKi_Divisor);
        }

    }
    else if (PIoutput < pParams->hLower_Limit_Output)
    {
        DumpPortion = PIoutput - (pParams->hLower_Limit_Output);
        PIoutput = pParams->hLower_Limit_Output;
		
        if (IntegralPortion < 0)
        {
            IntegralPortion = IntegralPortion - (DumpPortion  << pParams->hKi_Divisor);
        }
    }

    pParams->wIntegral = (IntegralPortion);

    return (s16) PIoutput; /* controller output with limitation */
}


/******************** (C) COPYRIGHT 2008 LINKO Semiconductor *******************/

