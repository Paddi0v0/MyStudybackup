/*!
 *-----------------------------------------------------------------------------
 * 	Copyright (c) Lks Corp., Ltd. All Rights Reserved. \n
 *  
 *
 *  @file    mc_declaration.h
 *
 *  @brief   MC_control declaration
 * 		  
 *		  
 *
 *  @author  zhao wencai
 *
 *  @version  v1.0
 * 
 *  @date    2019-11-07 23:25:30
 *  
 *-----------------------------------------------------------------------------
 */
#ifndef __MC_DECLARATION_H
#define __MC_DECLARATION_H

#include "basic.h"
#include "PubData_DriverInstance.h"

/*���������FOC���壺����������ѹ����԰���ռ�ʸ�����ơ�����������ʼλ�ü��*/
//marco define. use for motor drive layer.

///debug marco.
#define MOTOR_CTRL_SVC		(0)
#define MOTOR_CTRL_IF		(1)
#define MOTOR_CTRL_VF		(2)
#define MOTOR_CTRL_MODE		(MOTOR_CTRL_IF)

#define SAMP_NO               0
#define SAMP_IA               1
#define SAMP_IB               2
#define SAMP_IC               3
#define SAMP_NIA              4
#define SAMP_NIB              5
#define SAMP_NIC              6
#define SAMP_OLDA             7
#define SAMP_OLDB             8
#define SAMP_OLDC             9

typedef void (*ADCSampFunction)(void *this);		//AD���������ȡ����ָ��

//�����Ľṹ��
typedef struct
{
    s16 nPhaseU;
    s16 nPhaseV;
} STR_CurrPhaseUV;

/*****************************************************************************
 * �ṹ���� : STR_PIRegulator,*PSTR_PIRegulator
 * ˵��     : ������PI�������ݽṹ
 *          : 
 * 			:
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
typedef struct 
{
	s32 wUpperLimitOutput;			/* Lower Limit for Output limitation */
	s32 wLowerLimitOutput;			/* Lower Limit for Output limitation */
	u16 KP;							/* ����ϵ�� */
	u16 KI;							/* ����ϵ�� */
	s32 nError;						/* ������� */
	s32 nLastError;					/* �ϴ�������� */
	s16 nComp;
	s16 nOut;						/* ��� */
	s32 wIntegral;					/* ������ */
}STR_PIRegulator,*PSTR_PIRegulator;

/*****************************************************************************
 * �ṹ���� : STR_MotorLdqId
 * ˵��     : Ld/Lq��������ݽṹ
 *          : Ԥ��
 * 			:
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
typedef struct {
	int16_t nCurSet;
	int16_t nFreqSet;
	int16_t nCurStep;
	//uint16_t Ld;
	//uint16_t Lq;
	
	int16_t nLdCoef2Od;
	int16_t nLdCoef1OD;
	int16_t nLdCoef0;
	
	int16_t nLqCoef2Od;
	int16_t nLqCoef1Od;
	int16_t nLqCoef0;
	
	uint16_t nuSts;	
}STR_MotorLdqId;


/*****************************************************************************
 * �ṹ���� : STR_CurSamplingRes,*PSTR_CurSamplingRes
 * ˵��     : AD���� �����㼰�������� ���ݽṹ����
 *          : 
 * 			:
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
typedef struct
{
	u8 bSamplingEv1;
	u8 bSamplingEv2;
	ADCSampFunction pAdcSamp;						/* AD������ȡ���� */
} STR_CurSamplingEvent,*PSTR_CurSamplingEvent;

typedef struct
{
	STR_CurSamplingEvent		mCurSamplingEvent;	/* �����¼�����ȡ����ָ�� 	*/
	STR_PubDatHDMD* 			pAdcRes;			/* �����洢ָ��				*/
} STR_CurSamplingRes,*PSTR_CurSamplingRes;

/*****************************************************************************
 * �ṹ���� : STR_MotorFocHandle,*PSTR_MotorFocHandle
 * ˵��     : FOC����ʵ�ַ���ָ�뺯��
 *          : 
 * 			:
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
typedef struct{
	void (*Svpwm)(u8 t_bObjId,PSTR_VectorAB tS_pVolAB);		/* SVPWM����ָ��	*/
}STR_MotorFocHandle,*PSTR_MotorFocHandle;

/*****************************************************************************
 * �ṹ���� : STR_FluxObsGainCoefDef, *PSTR_FluxObsGainCoefDef
 * ˵��     : �۲����������ݽṹ���⺯��������
 *          : �޸�ʱȷ������Ӱ�쵽�⺯��Ѱַ
 * 			:
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
typedef struct
{   /* �۲��������������� */
	u16 nD_CurrLoopKP;         /* D�������KP����ֵ */
	u16 nD_CurrLoopKI;         /* D�������KI����ֵ */
	u16 nQ_CurrLoopKP;         /* Q�������KP����ֵ */
	u16 nQ_CurrLoopKI;         /* Q�������KI����ֵ */

	u16 nSTO_KP;               /* �۲���PLL KPֵ */
	u16 nSTO_KI;               /* �۲���PLL KPֵ */

	s32 CurrentPerUnitGAIN;    /* �������ۻ����������� */
	s32 VDC_PerUnitGAIN;       /* ��ѹ�������ۻ��������� */
	s32 FREQ_PerUnit_GAIN;     /* Ƶ�ʱ��ۻ��������� */
	s32 BEMF_PerUnit_GAIN;     /* �����Ʊ��ۻ��������� */
}STR_FluxObsGainCoefDef, *PSTR_FluxObsGainCoefDef;		//	�۲����������ݽṹ


/*****************************************************************************
 * �ṹ���� : STR_MotorFocElement,*PSTR_MotorFocElement
 * ˵��     : FOC��������
 *          : 
 * 			:
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
typedef struct {
	uint32_t	wElectAngle;			/* ʵ��ʹ�ó������ */
	uint32_t	wElectAngleEst; 		/* �������Ƕ� */
	uint32_t	wSensorAngle;			/* ���������Ƕ� */
	uint32_t	wElectAngleOpen;		/* �����Ƕ� */
}STR_MotorFocAngle,*PSTR_MotorFocAngle;

typedef struct {
	int32_t wSpeedEst;					/* �۲��ٶ� */
	int32_t wSpeedRef;					/* ָ���ٶ� */
}STR_MotorFocSpeed,*PSTR_MotorFocSpeed;

typedef struct {
	int16_t nFluxEst;
	uint32_t wuAngleEst;
	int16_t nSpdEst;
	int16_t nTorqEst;
	int16_t nPower;
	int16_t nEff;	
}STR_MotorObsResult;

typedef struct{
	//s16 nUdc;
	STR_VectorUVW			mStatCurrUVW;	/*���ӵ���UVW	*/
	STR_VectorAB			mStatCurrAB;	/*���ӵ���AB	*/
	STR_VectorDQ			mStatCurrDQ;	/*���ӵ���dq	*/
	STR_MotorFocAngle		mMotorAngle;	/*�ǶȽṹ��	*/
	STR_MotorFocSpeed		mMotorSpeed;	/*�ٶȽṹ��	*/
	STR_PIRegulator			mAcrD;			/*d���������	*/
	STR_PIRegulator			mAcrQ;			/*q���������	*/
	STR_VectorDQ			mAcrOutDQ;		/*���������	*/
	STR_VectorAB			mStatVolAB;		/*���ӵ�ѹAB	*/	
	STR_FluxObsGainCoefDef	mFluxObsGain;
}STR_MotorFocElement,*PSTR_MotorFocElement;;

/*****************************************************************************
 * �ṹ���� : STR_MotorFoc,*PSTR_MotorFoc
 * ˵��     : FOC�������ݽṹ���⺯��������
 *          : �޸�ʱȷ������Ӱ�쵽�⺯��Ѱַ
 * 			:
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
typedef struct{
	u8						m_bFOCObj_ID;		//��ǰ����ID
	u8						m_bPWM_ID;			//��ǰӦ�õ�PWM ID
	u8						m_bSampType;		//������������
	u8						m_bIPDFinsh;		//IPD��������
	u16						nuIPDAng;			//IPD�ǶȽ��
	u16						nuSysTimerPWM;
	bool					bObserWork;			//�۲���������־  true  ----- �۲�������
	STR_PubDatHDMD*			pHdToMd;			//HD���ݵ�MD��Ϣ������ָ��
	STR_PubDatMDMA*			pMdToMa;			//MD���ݵ�MA��Ϣ������ָ��
	STR_PubDatMAMD*			pMaToMd;			//MA���ݵ�MD��Ϣ������ָ��
	STR_PubDatMDHD*			pMdToHd;			//MD���ݵ�HD��Ϣ������ָ��

	PSTR_TransCoef			m_pTransCoef;		//���ݱ�����ָ��

	PSTR_FluxObsGainCoefDef	m_pFluxObsGain;		//�۲����ڲ��������ݽṹָ��

	PSTR_MotorFocParDef		m_pMotorFocPar;		//�۲������ò���ָ��

	ENUM_MotorStatus		eMotorStatusAck;	//���״̬
	ENUM_PWMStatus			ePWMStatusAck;		//PWM(MOE)״̬
	STR_CurSamplingRes		mCurSamplingRes;	//AD�����¼���ָ��

	STR_TrigComponents		mSinCosFocAngle;	//FOC�Ƕ�������ֵ
	STR_VectorUVW*			m_pStatCurrUVW;		//���ӵ���UVW		ָ��
	STR_VectorAB*			m_pStatCurrAB;		//���ӵ���AB		ָ��
	STR_VectorDQ*			m_pStatCurrDQ;		//���ӵ���dq		ָ��
	STR_VectorDQ*			m_pStatCurrDQRef;	//dq����ָ��ֵ		ָ��
	STR_MotorFocAngle*		m_pMotorAngle;		//����Ƕ�			ָ��

	STR_PIRegulator*		m_pAcrD;			//d���������		ָ��
	STR_PIRegulator*		m_pAcrQ;			//q���������		ָ��

	PSTR_VectorDQ			m_pVolDQ;			//���������dq��ѹ	ָ��
	PSTR_VectorAB			m_pVectVolAB;		//AB��ѹ			ָ��

	PSTR_FocSvpwmPar		m_pSvpwmPar;		//SVPWM����			ָ��
	PSTR_FocObjPar			m_pFocObjPar;		//���ƶ������		ָ��
	PSTR_FocObsPar			m_pFocObsPar;		//FOC����			ָ��

	PSTR_MotorFocSpeed		m_pMotorFocSpeed;	//FOC�ٶ�			ָ��

	PSTR_McpwmCmpr			m_pCmpr;			//MCPWM ����			ָ��	
	PSTR_McpwmTmr			m_pTmr;				//AD�����㼰ͨ������	ָ��
	PSTR_MotorFocHandle		m_pHandle;			//FOC����				����ָ��
	PSTR_HALDrvHandle		m_pHALHandle;		//HDӲ��������		����ָ��

}STR_MotorFoc,*PSTR_MotorFoc;		//	FOC���ƶ������ݽṹ

// �۲���������ʼ��
extern void InitPmFluxOB(u8 t_bObjId);

// ��ѹ����Բ���ƣ��ڲ������Զ������ƹ���
extern void CalcVolCircle(u8 t_bObjId,PSTR_VectorDQ tS_pVolDq,PSTR_VectorAB tS_pVolAB);

// ĸ�ߵ�ѹ���㼰���ۻ�
extern void CalcDcVoltPerUnit(u8 t_bObjId,s16 t_nUdcAdcRes);

extern void SVPWM_1Shunt(u8 t_bObjId,PSTR_VectorAB tS_pVolAB);
extern void SVPWM_2Shunt_En1(u8 t_bObjId,PSTR_VectorAB tS_pVolAB); 	//˫������ǿ1
extern void SVPWM_2Shunt_En2(u8 t_bObjId,PSTR_VectorAB tS_pVolAB); 	//˫������ǿ2
extern void SVPWM_3Shunt(u8 t_bObjId,PSTR_VectorAB tS_pVolAB);
extern void SVPWM_3Shunt_En(u8 t_bObjId,PSTR_VectorAB tS_pVolAB);

extern void SVPWM_2Shunt_En1_5Seg(u8 t_bObjId,PSTR_VectorAB tS_pVolAB); 	//˫������ǿ1
extern void SVPWM_2Shunt_En2_5Seg(u8 t_bObjId,PSTR_VectorAB tS_pVolAB); 	//˫������ǿ2
extern void SVPWM_3Shunt_5Seg(u8 t_bObjId,PSTR_VectorAB tS_pVolAB);
extern void SVPWM_3Shunt_En_5Seg(u8 t_bObjId,PSTR_VectorAB tS_pVolAB);


// �۲���������ʼ��
extern void FluxObserveParaCalc(u8 t_bObjId);
//
// �򻯴��롢����Ҫ���
extern void PmsmFluxObserveIDSimply(u8 t_bObjId,PSTR_VectorDQ tS_pCurDq);

// �򻯴��롢����Ҫ���
extern void PmsmFluxObserveSimply_Full(u8 t_bObjId,PSTR_VectorDQ tS_pCurDq);

// �򻯴��롢����Ҫ��ߣ���Χ����
extern void PmsmFluxObserveSimplyWeaken(u8 t_bObjId,PSTR_VectorDQ tS_pCurDq);

// ����Ҫ��ͣ�ִ��ʱ���Զ�
extern void PmsmFluxObserve(u8 t_bObjId,PSTR_VectorDQ tS_pCurDq);

// ����Ҫ��ߣ�ִ��ʱ���Գ�
void PmsmFluxObserve_Full(u8 t_bObjId,PSTR_VectorDQ tS_pCurDq);

// ����Ҫ��ߣ���Χ���ţ�ִ��ʱ�䳤
void PmsmFluxObserveWeaken(u8 t_bObjId,PSTR_VectorDQ tS_pCurDq);

// ����PmsmFluxObserve_Full����ת����
void PmsmFluxObserve_Full_En(u8 t_bObjId,PSTR_VectorDQ tS_pCurDq);


// ˳��������ʼ��
extern void OnTheFlyDetectInit(u8 t_bObjId);

// ˳�������㺯�� 
extern void OnTheFlyDetectPro(u8 t_bObjId);

// ����Ӧ�� IPD������ʼ������
extern void InitIPD(u8 t_bObjId);
// ����Ӧ�� IPD��⺯��
extern void IPDEst_DMotor(u8 t_bObjId);

// �����Ӧ�� IPD��⺯��
extern void IPDEst_SMotor(u8 t_bObjId);				//	�����IPD����


extern void SVPWM_GetPhaseCurrent(PSTR_CurSamplingRes this);


#if (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
#define Motor_FocMethod_M0   \
{  /* FOC Methods */									\
	SVPWM_2Shunt_En1,				/* SVPWM����	*/	\
}
#elif (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define Motor_FocMethod_M0   \
{  /* FOC Methods */									\
	SVPWM_1Shunt,					/* SVPWM����	*/	\
}
#else
#define Motor_FocMethod_M0   \
{  /* FOC Methods */									\
	SVPWM_3Shunt,					/* SVPWM����	*/	\
}

#endif

#if (EPWM1_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
#define Motor_FocMethod_M1								\
{  /* FOC Methods */									\
	SVPWM_2Shunt_En1,				/* SVPWM����	*/	\
}

#elif (EPWM1_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define Motor_FocMethod_M1								\
{  /* FOC Methods */									\
	SVPWM_1Shunt,					/* SVPWM����	*/	\
}
#else
#define Motor_FocMethod_M1								\
{  /* FOC Methods */									\
	SVPWM_3Shunt,					/* SVPWM����	*/	\
}

#endif


extern u16 getElectAngle(u8 t_bObjId);
extern u16 getElectAngleEst(u8 t_bObjId);


extern u32  Read_NVR(u32 addr);

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//���װ���ܺ���

//��֧�������ƶ�����Ŀ
extern u8 getLibMaxObjNumber(void);


//�����쳣������
extern bool motorStopOrExceptionHandle(u8 RunStop,pSTR_MotorExceptionMA this,s16 *PReturnCurSet);

//DQ�����ָ���ʼ������ ------ �������ſ���
extern void InitDQCurRefGen(u8 t_bObjId, s16 dCurSet,s16 qCurSet, s16 TransTime);

//DQ�����ָ���ʼ������ ------ �����ſ���
extern void InitDQCurRefGenWithWeakenFlux(u8 t_bObjId, s16 dCurSet, s16 qCurSet,s16 TransTime);

//D�����ָ�����   ------ �������ſ���
extern s16 getDCurRefGen(u8 t_bObjId);

//D�����ָ�����   ------ ����ǿ���ſ���,����ĸ��ʵ��ֵ�������Ŵ���
extern s16 getDCurRefGenWithEnWeakenFlux(u8 t_bObjId);

//�趨D�����ָ���ʼֵ 
extern void SetDCurRefInitValue(u8 t_bObjId, s16 dCurref);

//Q�����ָ�����
extern void QCurRefGenValue(u8 t_bObjId, s64 *pValue,u8 QFormat,s32 spdSet);

//�������ſ���PI����
extern void setFieldWeakenPara(u8 t_bObjId, u16 Kp, u16 Ki);

//����PLL����PI����
extern void setObserPLLKpKi(u8 t_bObjId,u16 Kp, u16 Ki);

//�Ƿ�ʹ��˳���۲⹦��
extern void setTrakingMode(u8 t_bObjId, bool bEnable);
extern	void	setTrakingModeEnable(u8 t_bObjId);
extern	void	setTrakingModeDisable(u8 t_bObjId);


//�鿴�Ƿ�����ٶȱջ�״̬
extern bool getRunStatus(u8 t_bObjId);

//�鿴�Ƿ��������ջ�״̬
extern bool getCurLoopCloseStatusFromOB(u8 t_bObjId);

//DQ��ѹ����ϵ������
extern void CalcVectorVFPara(u8 t_bObjId);

//DQ��ѹ��������
extern void vectorVFcontrol(u8 t_bObjId,s16 *pUd, s16 *pUq);

//������ѹ����ϵ��  1-----32767
extern void setDeadCompFactor(u8 t_bObjId, s16 factor);

//�ٶ�PI�����Զ�����
// t_bObjId ---- ���� ID
// KpFactor ---- ���õ��ٶ�PI�������Сϵ���� 1000 ------- 1.0��
// Ton		---- �ٶ��˲�ʱ�� 0.1ms
// s32 *pKp	---- ��дKp��ַ  Q9��ʽ
// s32 *pKi	---- ��дKi��ַ  Q14��ʽ    ������Ҫת��Ϊ���ʵ�Q��ʽ

extern void SpeedPIAutoTunning(u8 t_bObjId,s16 KpFactor, s16 Ton, s32 *pKp, s32 *pKi);

//����PI�����Զ�����
// t_bObjId		---- ���� ID
// s32 *pKpd	---- ��дKp��ַ  Q12��ʽ
// s32 *pKid	---- ��дKi��ַ  Q16��ʽ    ������Ҫת��Ϊ���ʵ�Q��ʽ
extern void CurPIAutoTunning(u8 t_bObjId,s32 *pKpd, s32 *pKid,s32 *pKpq, s32 *pKiq);


//���ù۲���PI����
// t_bObjId		---- ���� ID
// s32 Kpd		---- Kp			Q12��ʽ
// s32 Kid		---- Ki			Q16��ʽ
extern void SetCurPIPara(u8 t_bObjId,s32 Kpd, s32 Kid,s32 Kpq, s32 Kiq);


//�۲������Ƿ���ѡ��
// TRUE  ------ ����
extern void setLibWorkMode(u8 t_bObjId, bool bwork);
//�۲���1ms����
extern void PmsmFluxObserve1ms(u8 t_bObjId);

//�����Ƽ���ʼ��
//	nCompAngle   	----- �����Ƕȣ�1��
//  nBemfTrackVolt	----- ��С����ѹ��0.01V
extern void Bemf_CheckInit(u8 objId , u16 nCompAngle, u16 nBemfTrackVolt);

//�����Ƽ��
extern void Bemf_CheckProcess(u8 objId, s16 phaseC, s16 phaseB, s16 phaseA);
//  phaseC   ------  ADԭʼֵ

//�������л�����������, isWeakenUsed  TRUE ---- ���Ź۲���
extern void BemfSwitch(u8 objId, bool isWeakenUsed);

//��ʼ���۲����Ƕ�
extern void setOBElectAngleEst(u8 t_bObjId, u32 theta);

//���ù����Ƶ�ѹϵ��
// ���Ϊ		21845
// ����ԲʱΪ	18918
extern void setOverModuFac(u8 t_bObjId, s16 factor);

//�������ſ��Ƶ�ѹ������
// 100%  -------  4096
// Ĭ��95%����Ԥ��5%��ѹ
extern void setFieldWeakenVoltFac(u8 t_bObjId, s16 factor);

extern s32 getVolt2FreqCalcPara(u8 t_bObjId);		// �����ѹ��Ƶ�ʼ���ϵ�� Q12
													// ���ݷ�����ϵ�����㵱ǰ��ѹ�´�Ŷ�ӦƵ��ֵ

extern	s16 IQ15SqrtIQ31(s32 x);					// ƽ����������������

extern	s16 getElectSpdEst(u8 t_bObjId);
extern	s16 getPhaseCur_U(u8 t_bObjId);
extern	s16 getPhaseCur_V(u8 t_bObjId);
extern	s16		getMotorDaxisVoltFormObjId(u8 t_bObjId);
extern	s16		getMotorQaxisVoltFormObjId(u8 t_bObjId);

///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////new mc define/ end//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
