///*******************************************************************************
// * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
// *
// * �ļ����ƣ� HALDrv.h
// * �ļ���ʶ��
// * ����ժҪ�� Ӳ�������������ӿ�ȫ�ֱ��������ļ�
// * ����˵���� ��
// * ��ǰ�汾�� V 1.0
// * ��    �ߣ� Tonghua Li
// * ������ڣ� 2022��12��13��
// *
// *******************************************************************************/
#ifndef __HAL_DRV_DEFINE__
#define __HAL_DRV_DEFINE__


#include "basic.h"
#include "PubData_DriverInstance.h"

extern void AcquireCurSamplingResultsM0(STR_PubDatHDMD* tS_pCurAdcRes);
extern void ChangeADChanelCFGM0(STR_PubDatMDHD* tS_pMdToHd);
extern void AcquireVdcSamplingResultsM0(STR_PubDatHDMD* tS_pCurAdcRes);
extern void AcquireNTCSamplingResultsM0(STR_PubDatHDMD* tS_pAdcRes);
extern void AcquireBmefSamplingResultsM0(STR_PubDatHDMD* tS_pAdcRes);

extern void MCPWM0_RegUpdate(STR_PubDatMDHD* tS_pMdToHd);
extern void EPWM0_OutPut(FuncState t_state);
extern void EPWM0_Charge(u8 t_bPhaseABC);
extern void InitEPWM0ChargeEnd(void);
extern void GetEPWM0_breakInStatus(STR_PubDatHDMD* tS_pHd2Md);
extern void ClrEPWM0_breakInStatus(void);




#define Motor_HALDrv_EPWM0   \
{  		/* DRV handle */									\
	AcquireCurSamplingResultsM0,	/* ��ȡ����AD����	*/	\
	AcquireVdcSamplingResultsM0,	/* ��ȡĸ��AD����	*/	\
	AcquireNTCSamplingResultsM0,	/* ��ȡNTC AD����	*/	\
	AcquireBmefSamplingResultsM0,	/* ��ȡBMEF AD����	*/	\
	ChangeADChanelCFGM0,			/* �л�AD����ͨ��	*/	\
	MCPWM0_RegUpdate,				/* PWM����			*/	\
	EPWM0_OutPut,					/* PWM���ʹ��		*/	\
	EPWM0_Charge,					/* PWMԤ���		*/	\
	InitEPWM0ChargeEnd,				/* PWMԤ����ʼ��	*/	\
	GetEPWM0_breakInStatus,			/* ��ȡ BreakIn ״̬*/	\
	ClrEPWM0_breakInStatus			/* ��� BreakIn ״̬*/	\
}


// for inline function
static inline void AcquireCurSamplingResultsDRV0(STR_PubDatHDMD* tS_pCurAdcRes)
{
	tS_pCurAdcRes->nSampCurDat0 = GET_ADC0_DATA_M0;
	tS_pCurAdcRes->nSampCurDat1 = GET_ADC1_DATA_M0;
}

static inline void AcquireVdcSamplingResultsDRV0(STR_PubDatHDMD* tS_pCurAdcRes)
{
	tS_pCurAdcRes->nDcVolAdcDat = GET_M0_BUS_VOL_ADC_RESULT;
}

#endif
