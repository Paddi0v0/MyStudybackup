/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� PowerCalculation.c
 * �ļ���ʶ��
 * ����ժҪ�� ���ʻ�����
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� andrew kong
 * ������ڣ� 2020��10��9��
 *
 *******************************************************************************/

#include "PowerCalculation.h"
#include "MA_Static_Variable.h"
#include "MC_math.h"


stru_Ramp_t PowerRamp;

/*******************************************************************************
 �������ƣ�    void PowerLimitCalc(STR_MechQuantity *pstruMotorSpeed, s32 PowerValue)
 ����������    �޹��ʼ���
 ���˼·��    1.���ʵ�ʹ��ʳ�������ֵ��������ٶȻ����ٶȲο�ֵ��һ�������ٶȻ���ʱ���޹��ʴ���
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.08.17      V1.0           Tonghua Li           ����
 2020.10.09      V2.0           Tonghua Li         ���������ṹ
 ********************************************************************/
#if (DRV0_POWER_LIMIT_STATUS==TRUE) 
void PowerLimitCalc(STR_MechQuantity *pstruMotorSpeed, s32 PowerValue)
{
	if(++pstruMotorSpeed->nPowerLimitCntr >= pstruMotorSpeed->nPowerLimitPeriod)			//�޹��ʼ�������
	{
        pstruMotorSpeed->nPowerLimitCntr  = 0;
        if(PowerValue > pstruMotorSpeed->wPowerLimitValue)                           //ʵ�ʹ��ʴ������ƹ������С�ٶȸ���ֵ
        {
        	//	ATTENTION:
        	//	
        	//	wSpeedRampACCStepȡ�Աջ�����ٶȻ��Ӽ����趨ֵ
        	//	�ٶȻ��Ӽ����趨ֵ���ù���ʱ(�ٶȻ����ټӼ��ٵ���Ҫ)����Ҫ�޸Ĵ�������ٶȵ�����
        	//		��ֹ�����ٶ��趨ֵ�Ĺ��󲨶�����
        	
            if(pstruMotorSpeed->wPowerLimitSpeedChangeSet < pstruMotorSpeed->wSpeedSet)    //���ʳ������ƺ������޹������
            {
            	pstruMotorSpeed->wPowerLimitSpeedChangeSetInc += pstruMotorSpeed->wSpeedRampACCStep;
                pstruMotorSpeed->wPowerLimitSpeedChangeSet	+= pstruMotorSpeed->wPowerLimitSpeedChangeSetInc >> 16;
				pstruMotorSpeed->wPowerLimitSpeedChangeSetInc &= 0xFFFF0000;
            }
            else
            {
            	pstruMotorSpeed->wPowerLimitSpeedChangeSetInc	= 0;
                pstruMotorSpeed->wPowerLimitSpeedChangeSet = pstruMotorSpeed->wSpeedSet;
            }
        }
        else                                                                  //���ʻָ������󣬼�С�޹������
        {
            if(pstruMotorSpeed->wPowerLimitSpeedChangeSet > 0)
            {
            	pstruMotorSpeed->wPowerLimitSpeedChangeSetInc += pstruMotorSpeed->wSpeedRampDECStep;
                pstruMotorSpeed->wPowerLimitSpeedChangeSet -= pstruMotorSpeed->wPowerLimitSpeedChangeSetInc >> 16;
				pstruMotorSpeed->wPowerLimitSpeedChangeSetInc &= 0xFFFF0000;
            }
            else
            {
            	pstruMotorSpeed->wPowerLimitSpeedChangeSetInc	= 0;
                pstruMotorSpeed->wPowerLimitSpeedChangeSet = 0;
            }
        }
    }
}
#endif
/*******************************************************************************
 �������ƣ�   void SpeedLimitCalc(s32 wSpeedFbk, stru_PowerBasic_t *pstruPower)
 ����������    ��ת�ټ���
 ���˼·��    1.���ʵ��ת�ٳ�������ֵ������͹��ʻ��Ĺ��ʲο�ֵ 
 �����ı�    ��
 ���������    s32 wSpeedFbk, stru_PowerBasic_t *pstruPower
 ���������    ��
 �� �� ֵ��    wSpeedLimitPowerChangeSet    ------��С�Ĺ����趨ֵ
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.08.17      V1.0           Tonghua Li           ����
 2020.10.09      V2.0           Tonghua Li         ���������ṹ
 2023.03.28      V3.0           Tonghua Li         �޸�
 ********************************************************************/
#if (DRV0_SPPED_LIMIT_STATUS==TRUE)
void SpeedLimitCalc(s32 wSpeedFbk, pSTR_PowerDef pstruPower)
{
	if(++pstruPower->nSpeedLimitCntr >= pstruPower->nSpeedLimitPeriod)		//��ת�ټ�������
	{
		pstruPower->nSpeedLimitCntr  = 0;
		if(wSpeedFbk >= pstruPower->wSpeedLimitValue)						 //ʵ��ת�ٴ�������ת�����С���ʸ���ֵ
		{
			if(pstruPower->wSpeedLimitPowerChangeSet < pstruPower->wPowerSet) 	 //ת�ٳ������ƺ������������
			{
				pstruPower->wSpeedLimitPowerChangeSet += pstruPower->struPowerRamp.nACCStep;
			}
			else
			{
				pstruPower->wSpeedLimitPowerChangeSet = pstruPower->wPowerSet;
			}
		}
		else																  //ת�ٻָ������󣬼�С��ת�����
		{
			if(pstruPower->wSpeedLimitPowerChangeSet > 0)
			{
				pstruPower->wSpeedLimitPowerChangeSet -= pstruPower->struPowerRamp.nDECStep;
			}
			else
			{
				pstruPower->wSpeedLimitPowerChangeSet = 0;
			}
		}
	}
}
#endif

/*******************************************************************************
 �������ƣ�   void SpeedLimitCalc(s32 wSpeedFbk, stru_PowerBasic_t *pstruPower)
 ����������    ��ת�ټ���
 ���˼·��    1.���¼���
 �����ı�    ��
 ���������    stru_Ramp_t *pstruRamp ------���½ṹ���Ա
 ���������    ��
 �� �� ֵ��    pstruRamp->wRef -------�������ֵ
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.08.17      V1.0           Tonghua Li           ����
 ********************************************************************/
s32 RampControl(stru_Ramp_t *pstruRamp)
{
    if(pstruRamp->wRef < pstruRamp->wSet)
    {
        if((pstruRamp->wRef + pstruRamp->nACCStep) <= pstruRamp->wSet)  //����ֵС���趨ֵ������
        {
            pstruRamp->wRef += pstruRamp->nACCStep;
        }
        else
        {
            pstruRamp->wRef = pstruRamp->wSet;
        }
    }
    else if(pstruRamp->wRef > pstruRamp->wSet)                         //����ֵ�����趨ֵ���С
    {
        if((pstruRamp->wRef - pstruRamp->nDECStep) >= pstruRamp->wSet)
        {
            pstruRamp->wRef -= pstruRamp->nDECStep;
        }
        else
        {
            pstruRamp->wRef = pstruRamp->wSet;
        }
    }

    return(pstruRamp->wRef);
}

/*******************************************************************************
�������ƣ�    s16 PowerPIRegulator(stru_PIRegulator *Regulator)
����������    ���ʻ�PI  ����ʽPI
 ���˼·��    ����ʽPI���
 �����ı�    ��
���������    stru_PIRegulator *Regulator �ṹ��ָ��
���������    PI���ڽ��
 �� �� ֵ��    PI���ڽ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����    ��
�޸�����      �汾��          �޸���            �޸�����
-----------------------------------------------------------------------------
2020/8/5      V1.0           WenCai Zhao          ����
 ********************************************************************/
s16 PowerPIRegulator(stru_PIRegulator *Reg)
{
    long    ACC;
    int AX;

    ACC = (long)(Reg->wInError - Reg->wLastError) * Reg->KP;   /* ��������� */
    ACC = (ACC << 0) + (long)(Reg->wInError) * Reg->KI;        /* ��������� */
    Reg->wIntegral = ACC + Reg->wIntegral;

    if(Reg->wIntegral > Reg->wUpperLimitOutput)                /* ������ֵ�޷� */
    {
        Reg->wIntegral = Reg->wUpperLimitOutput;
    }
    else if(Reg->wIntegral < Reg->wLowerLimitOutput)         /* �����Сֵ�޷� */
    {
        Reg->wIntegral = Reg->wLowerLimitOutput;
    }

    AX = Reg->wIntegral >> 16;

    Reg->wLastError = Reg->wInError;                           /* ��¼�ϴ����ֵ */

    return(AX);
}

/*******************************************************************************
 �������ƣ�  void PowerLoopReg(pSTR_PowerDef pstru_Power)
����������    ���ʻ�����
 ���˼·��    ���ʻ�����
 �����ı�    ��
���������    stru_PowerBasic_t *this  �ṹ��ָ��
���������    ��
�� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����    ��
�޸�����      �汾��          �޸���            �޸�����
-----------------------------------------------------------------------------
 2020.08.17      V1.0           Tonghua Li           ����
 ********************************************************************/
void PowerLoopReg(pSTR_PowerDef pstru_Power)
{
    pstru_Power->struPowerPI.wInError = pstru_Power->wPowerRef - pstru_Power->wPowerValue;
	
	// 0.1W
	//pstru_Power->struPowerPI.wInError = (s32)(pstru_Power->struPowerPI.wInError/10);

    /* ����޷����ɸ���ʵ�ʸ��ص�����Ӱ��PI��Ӧ�Ŀ��� */
    //pstru_Power->struPowerPI.wInError = sat(pstru_Power->struPowerPI.wInError, -1000, 1000);
		//���ʻ����Iq
    pstru_Power->nQCurrentSet = PowerPIRegulator(&pstru_Power->struPowerPI);
}

/*******************************************************************************
 �������ƣ�  void PowerLoopInit(void)
 ����������    ���ʼ��������ʼ��
 ���˼·��    1.������ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
#if (DRV0_CLOSE_LOOP==POWER_LOOP)
void PowerLoopInit(STR_MACtrProc *pObj)
{
	
    //�ֲ�ָ��
    pSTR_PowerDef			pstru_Power ;
    PSTR_TransCoefElement	pApp2Core;
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;

    //��ֵ
    pstru_Power = pObj->m_pPower;
    pApp2Core = &pObj->mg_pCtrlObj->mTransCoef.mAppToCore;


    /*���ʻ�PI������ʼ��*/
    pstru_Power->struPowerPI.KP = pDrvCfgPara->mS_FBPwrLoop.nPowerKp;       //���ʻ� Kp POWER_KP_M0
    pstru_Power->struPowerPI.KI = pDrvCfgPara->mS_FBPwrLoop.nPowerKi;		//���ʻ� Ki POWER_KI_M0
    pstru_Power->struPowerPI.wInError = 0;
    pstru_Power->struPowerPI.wIntegral = 0;
    pstru_Power->struPowerPI.wLastError = 0;
    pstru_Power->struPowerPI.wUpperLimitOutput = (App2CoreCurTrans(pApp2Core, pDrvCfgPara->mS_FBPwrLoop.m_wPLoopIqMax) << 16); //���ʻ�������ֵ POWER_IQMAX_M0
    pstru_Power->struPowerPI.wLowerLimitOutput = (App2CoreCurTrans(pApp2Core, pDrvCfgPara->mS_FBPwrLoop.m_wPLoopIqMin) << 16); //���ʻ������Сֵ POWER_IQMIN_M0

    /*���ʻ����������ʼ��*/

    pstru_Power->nSpeedLimitCntr = 0;
	pstru_Power->nSpeedLimitPeriod = pDrvCfgPara->mS_FBPwrLoop.nPLoopSpdLmtCalcCycle;
    pstru_Power->wPowerRef = 0;
    pstru_Power->wPowerSet 			 = pDrvCfgPara->mS_FBPwrLoop.m_wPowerSetting;
    pstru_Power->wSpeedLimitPowerRef = pDrvCfgPara->mS_FBPwrLoop.m_wLmdSpdPowerSetting;	//SPEED_LIMIT_POWER_VALUE_M0
    pstru_Power->wSpeedLimitPowerChangeSet = 0;
    pstru_Power->wSpeedLimitValue	= App2CoreFreqTrans(pApp2Core, pDrvCfgPara->mS_FBPwrLoop.m_wSpdLmtValue);//SPEED_LIMIT_VALUE_M0
    pstru_Power->nQCurrentSet=0;   //���ʻ����

    pObj->m_pAppCommData->nPowerFistFlag=0;
    /*���ʼ��������ʼ��*/
    pstru_Power->wPowerValue = 0;   

    /*�������²�����ʼ��*/
    pstru_Power->struPowerRamp.nACCStep = (s16)(pDrvCfgPara->mS_FBPwrLoop.m_wPowerRunAcc);	//POWER_RUN_ACC_M0
    pstru_Power->struPowerRamp.nDECStep = (s16)(pDrvCfgPara->mS_FBPwrLoop.m_wPowerRunDec);	//POWER_RUN_DEC_M0
    pstru_Power->struPowerRamp.wRef = 0;
    pstru_Power->struPowerRamp.wSet = 0;
}
#endif

