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

extern void MCPWM_init(void);
extern void ADC0_init(void);
const STR_HALDrvHandle	 gS_MotorHalDrvHandleM[MAX_DRV_MODULE_USED] = {Motor_HALDrv_EPWM0};																   


u16		g_hDriverPolarity[MAX_DRV_MODULE_USED];		
// MAX_DRV_MODULE_USED > 1ʱ  0 --- EPWM0�� 1 --- EPWM0
// MAX_DRV_MODULE_USED = 1ʱ  0 --- Ϊ���õ�EPWM

/*******************************************************************************
 �������ƣ�    void ADC_1Shunt_NormalModeCFG(void)
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
void ADC_1Shunt_NormalModeCFG(void)
{ 
  ADC0_CHN0 = ADC_1SHUNT_CURR_CH | (ADC_1SHUNT_CURR_CH<<8);
  ADC0_CHN1 = ADC_1SHUNT_CURR_CH | (ADC_1SHUNT_CURR_CH<<8);
  ADC0_CHN2 = ADC_BUS_VOL_CHANNEL | (ADC0_TEMP_CHANNEL<<8);  
  ADC0_CHN3 = ADC0_TEMP_CHANNEL | (BEMF_CH_A<<8); 
  ADC0_CHN4 = BEMF_CH_B | (BEMF_CH_C<<8);  
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
    ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    ADC0_CHN1 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    ADC0_CHN2 = ADC_BUS_VOL_CHANNEL | (ADC_BUS_VOL_CHANNEL << 8);  //
    ADC0_CHN3 = ADC0_TEMP_CHANNEL | (BEMF_CH_A<<8); 
  	ADC0_CHN4 = BEMF_CH_B | (BEMF_CH_C<<8); 
 
    MCPWM_TMR0 = (s16)(100-PWM_PERIOD_M0); // 100-PWM_PERIOD
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
    ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    ADC0_CHN1 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    ADC0_CHN2 = ADC_BUS_VOL_CHANNEL | (M0_ADC_BUS_CURR_CH << 8);  
	ADC0_CHN3 = ADC0_TEMP_CHANNEL | (BEMF_CH_A<<8); 
  	ADC0_CHN4 = BEMF_CH_B | (BEMF_CH_C<<8); 
}


/*******************************************************************************
 �������ƣ�    void AcquireCurSamplingResults(STR_PubDatHDMD* tS_pCurAdcRes)
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
 ����������    AD����ͨ���л�
 ���������    tS_pMdToHd
 ���������    ��
 �� �� ֵ��    ��
 ����˵����		���������ʱ��̬�л�����ͨ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ���� 
 2024/5/27     V1.1           TongHua Li         ����
 *******************************************************************************/
#if (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd)
{

}


#elif (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd)
{

}

#else

void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd)
{
	if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_A_B)
	{
		ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
		ADC0_CHN1 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
	}
	else if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_B_C)
	{
		ADC0_CHN0 = ADC0_CURRETN_B_CHANNEL | (ADC0_CURRETN_C_CHANNEL << 8);
		ADC0_CHN1 = ADC0_CURRETN_B_CHANNEL | (ADC0_CURRETN_C_CHANNEL << 8);
	}
	else if(tS_pMdToHd->mMcTmr.eChannel == E_PHASE_A_C)
	{
		ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_C_CHANNEL << 8);
		ADC0_CHN1 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_C_CHANNEL << 8);
	}
}
#endif

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
{  // (MCPWM_EIF & BIT4)��ӦFALL0�¼���BIT5��ӦFALL1�¼�
    tS_pHd2Md->bBreakInStatus = (MCPWM_EIF & BIT5);  
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
    MCPWM_EIF = (MCPWM_EIF | BIT5);
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
	g_hDriverPolarity[0] = MCPWM_IO01;		/* ������������ */
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
    PWMOutputs(t_state);
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
	MCPWM_PRT		= 0x0000DEAD;
	
	if(t_bPhaseABC == 1)			//A������PWM
	{
		MCPWM_IO01		= 0x0C08 | g_hDriverPolarity[0];
		MCPWM_IO23		= 0x000C | g_hDriverPolarity[0];
	}
	else if(t_bPhaseABC == 2)		//B������PWM
	{
		MCPWM_IO01		= 0x080C | g_hDriverPolarity[0];
	}
	else if(t_bPhaseABC == 3)		//C������PWM
	{
		MCPWM_IO23		= 0x08 | g_hDriverPolarity[0];;
	}

	MCPWM_PRT		= 0x0000;
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
	MCPWM_init();					/* PWM��ʼ�� */
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
	MCPWM_TH00 = tS_pMdToHd->mMcPwm.nuTimePhBN;
    MCPWM_TH01 = tS_pMdToHd->mMcPwm.nuTimePhB;
    
    MCPWM_TH10 = tS_pMdToHd->mMcPwm.nuTimePhAN;
    MCPWM_TH11 = tS_pMdToHd->mMcPwm.nuTimePhA;

    MCPWM_TH20 = tS_pMdToHd->mMcPwm.nuTimePhCN;
    MCPWM_TH21 = tS_pMdToHd->mMcPwm.nuTimePhC;

    MCPWM_TMR0 = tS_pMdToHd->mMcTmr.nuPoint0;	// ��һ��������
    MCPWM_TMR1 = tS_pMdToHd->mMcTmr.nuPoint1;	// �ڶ���������	
}


