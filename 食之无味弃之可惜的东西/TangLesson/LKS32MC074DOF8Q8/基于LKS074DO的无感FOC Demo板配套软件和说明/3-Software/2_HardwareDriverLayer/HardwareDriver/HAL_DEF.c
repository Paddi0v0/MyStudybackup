/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� HAL_DEF.c
 * �ļ���ʶ��
 * ����ժҪ�� �������MCUӲ���ӿڲ㣬����Ӳ�������Ĺ��ú���
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� LKS/MC TEAM
 * ������ڣ� 2022��12��13��
 *******************************************************************************/
#include "project_config.h"
#include "PubData_DriverInstance.h"
#include "hardware_config.h"
#include "HALDrv.h"

extern void MCPWM_ch012_init(void);
extern void MCPWM_ch345_init(void);
extern void ADC0_init(void);
extern void ADC1_init(void);
const STR_HALDrvHandle	 gS_MotorHalDrvHandleM[MAX_DRV_MODULE_USED] = {Motor_HALDrv_EPWM0};																   


u16		g_hDriverPolarity[MAX_DRV_MODULE_USED];		
// MAX_DRV_MODULE_USED > 1ʱ  0 --- EPWM0�� 1 --- EPWM0
// MAX_DRV_MODULE_USED = 1ʱ  0 --- Ϊ���õ�EPWM

/*******************************************************************************
 �������ƣ�    void ADC_1Shunt_NormalModeCFG(void)
 ����������    ADC��������ģʽͨ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void ADC_1Shunt_NormalModeCFG(void)
{
    ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);      /*ADC��0�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);      /*ADC��1�β���ͨ������*/

    ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);      /*ADC��2�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);      /*ADC��3�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN4, ADC_BUS_VOL_CHANNEL, ADC_GAIN3V6);     /*ADC��4�β���ͨ������*/
	ADC_CHN_GAIN_CFG(ADC0, CHN5, ADC_BUS_VOL_CHANNEL, ADC_GAIN3V6);		/*ADC��5�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN6, ADC0_TEMP_CHANNEL, ADC_GAIN3V6);		/*ADC��6�β���ͨ������*/
    //ADC1
    ADC_CHN_GAIN_CFG(ADC1, CHN0, BEMF_CH_A, ADC_GAIN3V6);              /*ADC��0�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN1, BEMF_CH_A, ADC_GAIN3V6);              /*ADC��1�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN2, BEMF_CH_B, ADC_GAIN3V6);              /*ADC��2�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN3, BEMF_CH_C, ADC_GAIN3V6);              /*ADC��3�β���ͨ������*/
	
}

/*******************************************************************************
 �������ƣ�    void ADC_2Shunt_NormalModeCFG(void)
 ����������    ˫����ADC��������ģʽͨ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void ADC_2Shunt_NormalModeCFG(void)
{
	//ADC0
    ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN4, ADC_BUS_VOL_CHANNEL, ADC_GAIN3V6);     /*ADC��4�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN5, ADC_BUS_VOL_CHANNEL, ADC_GAIN3V6);		/*ADC��5�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN6, ADC0_TEMP_CHANNEL, ADC_GAIN3V6);		/*ADC��6�β���ͨ������*/
	
	//ADC1
    ADC_CHN_GAIN_CFG(ADC1, CHN0, BEMF_CH_A, ADC_GAIN3V6);              /*ADC��0�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN1, BEMF_CH_A, ADC_GAIN3V6);              /*ADC��1�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN2, BEMF_CH_B, ADC_GAIN3V6);              /*ADC��2�β���ͨ������*/
	ADC_CHN_GAIN_CFG(ADC1, CHN3, BEMF_CH_C, ADC_GAIN3V6);              /*ADC��3�β���ͨ������*/

	MCPWM0_TMR0 = (s16)(100-PWM_PERIOD_M0); 			// 100-PWM_PERIOD
}

/*******************************************************************************
 �������ƣ�    void ADC_3Shunt_NormalModeCFG(void)
 ����������    ������ADC��������ģʽͨ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void ADC_3Shunt_NormalModeCFG(void)
{
    ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);  /*ADC��0�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��1�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);	/*ADC��2�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��3�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN4, ADC_BUS_VOL_CHANNEL, ADC_GAIN3V6);     /*ADC��4�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN5, M0_ADC_BUS_CURR_CH, ADC_GAIN3V6);		/*ADC��5�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC0, CHN6, ADC0_TEMP_CHANNEL, ADC_GAIN3V6);		/*ADC��6�β���ͨ������*/

	//ADC1
    ADC_CHN_GAIN_CFG(ADC1, CHN0, BEMF_CH_A, ADC_GAIN3V6);              /*ADC��0�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN1, BEMF_CH_A, ADC_GAIN3V6);              /*ADC��1�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN2, BEMF_CH_B, ADC_GAIN3V6);              /*ADC��2�β���ͨ������*/
    ADC_CHN_GAIN_CFG(ADC1, CHN3, BEMF_CH_C, ADC_GAIN3V6);              /*ADC��3�β���ͨ������*/
}

/*******************************************************************************
 �������ƣ�    void AcquireCurSamplingResultsM0(STR_PubDatHDMD* tS_pCurAdcRes)
 			   void AcquireVdcSamplingResultsM0(STR_PubDatHDMD* tS_pCurAdcRes)
 			   void AcquireNTCSamplingResultsM0(STR_PubDatHDMD* tS_pAdcRes)
 			   void AcquireBmefSamplingResultsM0(STR_PubDatHDMD* tS_pAdcRes) 			   
 ����������    get adc sample result
 ���������    
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void AcquireCurSamplingResultsM0(STR_PubDatHDMD* tS_pCurAdcRes)
{
	tS_pCurAdcRes->nSampCurDat0 = GET_ADC0_DATA_M0;
	tS_pCurAdcRes->nSampCurDat1 = GET_ADC1_DATA_M0;
}


void AcquireVdcSamplingResultsM0(STR_PubDatHDMD* tS_pCurAdcRes)
{
	tS_pCurAdcRes->nDcVolAdcDat = GET_M0_BUS_VOL_ADC_RESULT;
}


void AcquireNTCSamplingResultsM0(STR_PubDatHDMD* tS_pAdcRes)
{
	tS_pAdcRes->nNTCVolAdcDat	= GET_M0_TEMP_DATA_A;	
}


void AcquireBmefSamplingResultsM0(STR_PubDatHDMD* tS_pAdcRes)
{
	#if (DRV0_BEMF_CHECK_EABLE==FUNCTION_ON)
	tS_pAdcRes->nBmefUVolAdcDat = GET_M0_BEMF_DATA_A - tS_pAdcRes->nBmefUOffset;
	tS_pAdcRes->nBmefVVolAdcDat = GET_M0_BEMF_DATA_B - tS_pAdcRes->nBmefVOffset;
	tS_pAdcRes->nBmefWVolAdcDat = GET_M0_BEMF_DATA_C - tS_pAdcRes->nBmefWOffset;
	#endif
}

/*******************************************************************************
 �������ƣ�    void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd)		   
 ����������    adc ͨ����̬����
 ���������    
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
#if (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_1SHUNT)
void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd)
{

}
#elif (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_2SHUNT)
void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd)
{

}

#else

void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd)
{
	if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_A_B)
	{
		ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);  /*ADC��0�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��1�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);	/*ADC��2�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��3�β���ͨ������*/
	}
	else if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_B_C)
	{
		ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);  /*ADC��0�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��1�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��2�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��3�β���ͨ������*/
	}
	else if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_A_C)
	{
		ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);  /*ADC��0�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��1�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);	/*ADC��2�β���ͨ������*/
    	ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��3�β���ͨ������*/		
	}
}
#endif

/*******************************************************************************
 �������ƣ�    void AcquireCurSamplingResultsM1(STR_PubDatHDMD* tS_pCurAdcRes)
 			   void AcquireVdcSamplingResultsM1(STR_PubDatHDMD* tS_pCurAdcRes)
 			   void AcquireNTCSamplingResultsM1(STR_PubDatHDMD* tS_pAdcRes)
 			   void AcquireBmefSamplingResultsM1(STR_PubDatHDMD* tS_pAdcRes) 			   
 ����������    get adc sample result
 ���������    
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void AcquireCurSamplingResultsM1(STR_PubDatHDMD* tS_pCurAdcRes)
{
	tS_pCurAdcRes->nSampCurDat0 = GET_ADC0_DATA_M0;
	tS_pCurAdcRes->nSampCurDat1 = GET_ADC1_DATA_M0;
}

void AcquireVdcSamplingResultsM1(STR_PubDatHDMD* tS_pCurAdcRes)
{
	tS_pCurAdcRes->nDcVolAdcDat = GET_M0_BUS_VOL_ADC_RESULT;
}

void AcquireNTCSamplingResultsM1(STR_PubDatHDMD* tS_pAdcRes)
{
	tS_pAdcRes->nNTCVolAdcDat	= GET_M0_TEMP_DATA_A;
}

void AcquireBmefSamplingResultsM1(STR_PubDatHDMD* tS_pAdcRes)
{
	#if (DRV0_BEMF_CHECK_EABLE==FUNCTION_ON)
	tS_pAdcRes->nBmefUVolAdcDat = GET_M0_BEMF_DATA_A - tS_pAdcRes->nBmefUOffset;
	tS_pAdcRes->nBmefVVolAdcDat = GET_M0_BEMF_DATA_B - tS_pAdcRes->nBmefVOffset;
	tS_pAdcRes->nBmefWVolAdcDat = GET_M0_BEMF_DATA_C - tS_pAdcRes->nBmefWOffset;
	#endif

}
/*******************************************************************************
 �������ƣ�    void ChangeADChanelCFGM1(STR_PubDatMDHD* tS_pMdToHd)		   
 ����������    adc ͨ����̬����
 ���������    
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
#if (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_1SHUNT)
void ChangeADChanelCFGM1(STR_PubDatMDHD* tS_pMdToHd)
{

}
#elif (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_2SHUNT)
void ChangeADChanelCFGM1(STR_PubDatMDHD* tS_pMdToHd)
{

}
#else
void ChangeADChanelCFGM1(STR_PubDatMDHD* tS_pMdToHd)
{
	if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_A_B)
	{
		ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);	/*ADC��0�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��1�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);	/*ADC��2�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��3�β���ͨ������*/
	}
	else if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_B_C)
	{
		ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��0�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��1�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_B_CHANNEL, ADC_GAIN3V6);	/*ADC��2�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��3�β���ͨ������*/
	}
	else if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_A_C)
	{
		ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);	/*ADC��0�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��1�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC0_CURRETN_A_CHANNEL, ADC_GAIN3V6);	/*ADC��2�β���ͨ������*/
		ADC_CHN_GAIN_CFG(ADC0, CHN3, ADC0_CURRETN_C_CHANNEL, ADC_GAIN3V6);	/*ADC��3�β���ͨ������*/		
	}
}
#endif

#if	(EPWM0_USED == FUNCTION_ON)
/*******************************************************************************
 �������ƣ�    void GetEPWM0_breakInStatus(STR_PubDatHDMD* tS_pHd2Md)
 ����������    ��ȡbreakin�ź�
 ���������    tS_pHd2Md
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   �鿴�Ƿ���ڹ����¼�
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void GetEPWM0_breakInStatus(STR_PubDatHDMD* tS_pHd2Md)
{
    tS_pHd2Md->bBreakInStatus = (MCPWM0_EIF & BIT4);
}

/*******************************************************************************
 �������ƣ�    void ClrEPWM0_breakInStatus(void)
 ����������    ���breakin�ź�
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void ClrEPWM0_breakInStatus(void)
{
    MCPWM0_EIF = (MCPWM0_EIF | BIT4);
}

/*******************************************************************************
 �������ƣ�    void registerEPWM0_DrvLevel(void)
 ����������    ��¼��������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void registerEPWM0_DrvLevel(void)
{
	g_hDriverPolarity[0] = MCPWM0_IO01;		/* ������������ */
}

/*******************************************************************************
 �������ƣ�    void EPWM0_OutPut(FuncState t_state) 
 ����������    MOE��������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   TRUE---- ʹ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void EPWM0_OutPut(FuncState t_state)
{
	MCPWM0_PRT = 0xDEAD;
    if(t_state == DISABLE)
    {
        MCPWM0_FAIL012 &= ~BIT6;
    }
    else
    {
        MCPWM0_FAIL012 |= BIT6;
    }
    MCPWM0_PRT = 0;
}

/*******************************************************************************
 �������ƣ�    void EPWM0_Charge(u8 t_bPhaseABC) 
 ����������    Ԥ��缫�Կ���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   TRUE---- ʹ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void EPWM0_Charge(u8 t_bPhaseABC)
{
	MCPWM0_PRT		= 0x0000DEAD;
	
	if(t_bPhaseABC == 1)			//A������PWM
	{
		MCPWM0_IO01		= 0x0C08 | g_hDriverPolarity[0];
		MCPWM0_IO23		= 0x000C | g_hDriverPolarity[0];
	}
	else if(t_bPhaseABC == 2)		//B������PWM
	{
		MCPWM0_IO01		= 0x080C | g_hDriverPolarity[0];
	}
	else if(t_bPhaseABC == 3)		//C������PWM
	{
		MCPWM0_IO23		= 0x08 | g_hDriverPolarity[0];;
	}

	MCPWM0_PRT		= 0x0000;
}

/*******************************************************************************
 �������ƣ�    void InitEPWM0ChargeEnd(void) 
 ����������    Ԥ�����ɺ��ʼ��Ӳ��(PWM��ADC)
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   TRUE---- ʹ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void InitEPWM0ChargeEnd(void)
{
	__disable_irq();
	ADC0_init();
	ADC1_init();
	MCPWM_ch012_init();					/* PWM��ʼ�� */
	ADC_STATE_RESET();
	__enable_irq();
}

/*******************************************************************************
 �������ƣ�    void MCPWM0_RegUpdate(void)
 ����������    ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void MCPWM0_RegUpdate(STR_PubDatMDHD* tS_pMdToHd)
{
	MCPWM0_TH00 = tS_pMdToHd->mMcPwm.nuTimePhAN;
    MCPWM0_TH01 = tS_pMdToHd->mMcPwm.nuTimePhA;
    
    MCPWM0_TH10 = tS_pMdToHd->mMcPwm.nuTimePhCN;
    MCPWM0_TH11 = tS_pMdToHd->mMcPwm.nuTimePhC;

    MCPWM0_TH20 = tS_pMdToHd->mMcPwm.nuTimePhBN;
    MCPWM0_TH21 = tS_pMdToHd->mMcPwm.nuTimePhB;

    MCPWM0_TMR0 = tS_pMdToHd->mMcTmr.nuPoint0;	// ��һ��������
    MCPWM0_TMR1 = tS_pMdToHd->mMcTmr.nuPoint1;	// �ڶ���������	
}

#endif


#if	(EPWM1_USED == FUNCTION_ON)
/*******************************************************************************
 �������ƣ�    void GetEPWM0_breakInStatus(STR_PubDatHDMD* tS_pHd2Md)
 ����������    ��ȡbreakin�ź�
 ���������    tS_pHd2Md
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   �鿴�Ƿ���ڹ����¼�
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void GetEPWM1_breakInStatus(STR_PubDatHDMD* tS_pHd2Md)
{
    tS_pHd2Md->bBreakInStatus = (MCPWM0_EIF & BIT7);
}

/*******************************************************************************
 �������ƣ�    void ClrEPWM1_breakInStatus(void)
 ����������    ���breakin�ź�
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void ClrEPWM1_breakInStatus(void)
{
    MCPWM0_EIF = (MCPWM0_EIF | BIT7);
}

/*******************************************************************************
 �������ƣ�    void registerEPWM1_DrvLevel(void)
 ����������    ��¼��������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void registerEPWM1_DrvLevel(void)
{
	g_hDriverPolarity[0] = MCPWM0_IO45;		/* ������������ */
}

/*******************************************************************************
 �������ƣ�    void EPWM1_OutPut(FuncState t_state) 
 ����������    MOE��������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   TRUE---- ʹ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void EPWM1_OutPut(FuncState t_state)
{
	MCPWM0_PRT = 0xDEAD;
    if(t_state == DISABLE)
    {
        MCPWM0_FAIL345 &= ~BIT6;
    }
    else
    {
        MCPWM0_FAIL345 |= BIT6;
    }
    MCPWM0_PRT = 0; 
}

/*******************************************************************************
 �������ƣ�    void EPWM1_Charge(u8 t_bPhaseABC) 
 ����������    Ԥ��缫�Կ���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   TRUE---- ʹ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void EPWM1_Charge(u8 t_bPhaseABC)
{
	MCPWM0_PRT		= 0x0000DEAD;
	
	if(t_bPhaseABC == 1)			//A������PWM
	{
		MCPWM0_IO23		= 0x080C | g_hDriverPolarity[1];
		MCPWM0_IO45		= 0x0C0C | g_hDriverPolarity[1];
	}
	else if(t_bPhaseABC == 2)		//B������PWM
	{
		MCPWM0_IO23		= 0x0C0C | g_hDriverPolarity[1];
		MCPWM0_IO45		= 0x0C08 | g_hDriverPolarity[1];
	}
	else if(t_bPhaseABC == 3)		//C������PWM
	{
		MCPWM0_IO23		= 0x0C0C | g_hDriverPolarity[1];
		MCPWM0_IO45		= 0x080C | g_hDriverPolarity[1];
	}

	MCPWM0_PRT		= 0x0000;
}

/*******************************************************************************
 �������ƣ�    void InitEPWM1ChargeEnd(void) 
 ����������    Ԥ�����ɺ��ʼ��Ӳ��(PWM��ADC)
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   TRUE---- ʹ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
void InitEPWM1ChargeEnd(void)
{
	__disable_irq();
	ADC0_init();
	ADC1_init();
    MCPWM_ch345_init();						/* PWM��ʼ�� */
	ADC_STATE_RESET();
	__enable_irq();
}

/*******************************************************************************
 �������ƣ�    void MCPWM1_RegUpdate(void)
 ����������    ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void MCPWM1_RegUpdate(STR_PubDatMDHD* tS_pMdToHd)
{
	MCPWM0_TH30 = tS_pMdToHd->mMcPwm.nuTimePhBN;
    MCPWM0_TH31 = tS_pMdToHd->mMcPwm.nuTimePhB;
    
    MCPWM0_TH40 = tS_pMdToHd->mMcPwm.nuTimePhAN;
    MCPWM0_TH41 = tS_pMdToHd->mMcPwm.nuTimePhA;

    MCPWM0_TH50 = tS_pMdToHd->mMcPwm.nuTimePhCN;
    MCPWM0_TH51 = tS_pMdToHd->mMcPwm.nuTimePhC;

    MCPWM0_TMR2 = tS_pMdToHd->mMcTmr.nuPoint0;		// ��һ��������
    MCPWM0_TMR3 = tS_pMdToHd->mMcTmr.nuPoint1;		// �ڶ���������
}
#endif

