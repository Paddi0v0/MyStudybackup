#ifndef _PUBDATA_MD_MA_H_
#define _PUBDATA_MD_MA_H_

#include "basic.h"
#include "MC_type.h"

typedef enum
{
    E_PWM_RESET = 0,E_PWM_ON,E_PWM_OFF
} ENUM_PWMStatus;				//	PWM���״̬(MOE)


typedef enum
{
	E_FC_NON = 0x00,										//	�޲���
	E_FC_RESET_INIT_ACR = (E_MOTOR_RESET<<8)+0x01,			//	RESET״̬ ��ʼ��������
	E_FC_RESET_INIT_OBS = (E_MOTOR_RESET<<8)+0x0002,		//	RESET״̬ ��ʼ���۲���
	E_FC_RESET_INIT_SVPWM = (E_MOTOR_RESET<<8)+0x03,		//	RESET״̬ ��ʼ��SVPWM
	E_FC_RESET_INIT_IPD = (E_MOTOR_RESET<<8)+0x04,			//	RESET״̬ ��ʼ��IPD
	E_FC_RESET_CHARGE_A = (E_MOTOR_RESET<<8)+0x05,			//	RESET״̬ A��Ԥ���
	E_FC_RESET_CHARGE_B = (E_MOTOR_RESET<<8)+0x06,			//	RESET״̬ B��Ԥ���
	E_FC_RESET_CHARGE_C = (E_MOTOR_RESET<<8)+0x07,			//	RESET״̬ C��Ԥ���
	E_FC_RESET_CHARGE_DONE_INIT = (E_MOTOR_RESET<<8)+0x08,	//	RESET״̬ Ԥ������
	E_FC_RESET_CLR_BREAKIN_FAULT = (E_MOTOR_RESET<<8)+0x09,	//	RESET״̬ ���Breakin
	

	E_FC_TRACKING_FWD = (E_MOTOR_TRACKING<<8)+0x01,			//	Ԥ��
	E_FC_TRACKING_REV = (E_MOTOR_TRACKING<<8)+0x02,			//	Ԥ��
	E_FC_TRACKING_FWS = (E_MOTOR_TRACKING<<8)+0x03,			//	Ԥ��
	E_FC_TRACKING_UPS = (E_MOTOR_TRACKING<<8)+0x04,			//	Ԥ��
	E_FC_TRACKING_WAIT = (E_MOTOR_TRACKING<<8)+0x05,		//	Ԥ��

	E_FC_IPD_CW_START = (E_MOTOR_IPD<<8)+0x01,				//	IPD CW
	E_FC_IPD_CCW_START = (E_MOTOR_IPD<<8)+0x02,				//	IPD CCW
	E_FC_IPD_DOING = (E_MOTOR_IPD<<8)+0x03,					//	IPD �����
	E_FC_IPD_DONE  = (E_MOTOR_IPD<<8)+0x04,					//	IPD ����
	  
	E_FC_ALIGN_SetUp = (E_MOTOR_ALIGN<<8)+0x01,				//	Ԥ��
	E_FC_ALIGN_WAIT = (E_MOTOR_ALIGN<<8)+0x02,				//	Ԥ��
	
	E_FC_BRAKE_SetUp = (E_MOTOR_BRAKE<<8)+0x01,				//	Ԥ��
	E_FC_BRAKE_WAIT = (E_MOTOR_BRAKE<<8)+0x02,				//	Ԥ��
} ENUM_FineCmd;					//	MA�������MD�ظ�����



typedef struct {	
	int32_t wRs;
	int32_t wLd;
	int32_t wLq;
	int32_t wFlux;
	int16_t nPllKp;
	int16_t nPllKi;
	int16_t nFilterCoeff;
}STR_MotorObsParOnLine;			//	�۲���ʵʱ���ò���	Ԥ��


typedef struct {	
	int16_t					gWrFlag;				//�������
	ENUM_MotorStatus		eMotorStatus;			//MD��״ָ̬��
	STR_VectorDQ			mStatCurDQCmd;			//DQ����ָ��
	int32_t					wMotorSpeedRef;			//�ٶ�ָ��ֵ
	
	ENUM_PWMStatus			ePwmCmd;				//PWM�������� ON---MOEʹ��
	int16_t					nMoeActionBlankTime;	//����ʱ��5ms..	
	STR_MotorObsParOnLine 	mMotorParRT;	
	uint8_t					bIpdStart;				//IPDʹ��
	int16_t					nMotorBrakePwmDuty;		//�ƶ���� q12
	ENUM_FineCmd			nuFineCmd;				//0:none
													//E_MOTOR_RESET: 0x01: init Acr   0x02: init obs   0x03:init svpwm  0x04:init ipd ...
													//E_MOTOR_TRACKING E_MOTOR_ALIGN,E_MOTOR_ASYN,E_MOTOR_PRE_SYN,E_MOTOR_SYN:
													//0xF0:    0xF1:       ...
													//E_MOTOR_BRAKE,E_MOTOR_IPD
													//0x0100:Duty brake    0x0101:short brake       ...
}STR_PubDatMAMD, *PSTR_PubDatMAMD;			//	MA�������ݽṹ(���͸� MD)


typedef struct {	
	int16_t					gWrFlag;				//�����־
	ENUM_PWMStatus			ePwmStatus;				//moe״̬
	bool					bBreakInFault;			//BreakIn��־ 
	
	int32_t					wFreq;					//����Ƶ��ֵ�(����Ϊ�����Ƽ���Ƶ��ֵ)
	int16_t					nVdc;					//ĸ�ߵ�ѹ
	STR_VectorDQ			mStatCurDQ;				//DQ����ֵ
	STR_VectorDQ			mStatVolDQ;				//DQ��ѹֵ
	STR_VectorUVW			mStatCurrUVW;			//���ӵ���UVW
	int16_t					nMotorCur;				//����������Чֵ
	int16_t					nMotorVol;				//�����ѹ(����Ϊ�����Ƽ��ķ�ֵ)������ΪSQRT(Ud^2+Uq^2)
	int16_t					nPowerDec;				//���㹦��
	int16_t					nTemp;					//�¶�

	ENUM_MotorStatus		eMotorStatusAck;		//MD��״̬
	
	ENUM_FineCmd			nuFineStatus;			//0:none 													
}STR_PubDatMDMA;								//	MD״̬���ݽṹ(���ظ� MA)

typedef struct
{   /* ˳�����������ת���ٶ�λ�÷����� */
    s32 wFreqAvg;				/* ת�ӵ�Ƶ��ƽ��ֵ */
	s16 nMotorVolMagAvg;		/* ��ǰ�������˲����ѹƽ��ֵ */
    s16 nMotorStopCurrentThd;	/* ���ͣת���������ֵ */
    s16 nMotorStopCurDifThd;	/* ���ͣת������ֵ�����ֵ */
	u8  bMotorStopDecFlag;		/* ���ͣת��־ */
	s32 s32MotorCurrentMagfilt;
	s32 s32MotorCurrentMagfilt1;
	s32 s32MotorCurrentMagDifAvg;	
} STR_OnTheFlyDetectMA,*pSTR_OnTheFlyDetectMA;	//	MA˳�����ṹ��

typedef enum
{
	E_SLVC_RUN_NORMAL = 0,
	E_SLVC_RUN_2_STOP,
	E_SLVC_RUN_2_CHARGE
}ENUM_Run_Status;

typedef struct
{
	ENUM_Run_Status 	eRunStatus;				//����״̬
	s16 				brakeCurrent;			//�ƶ�����
	
	s16 				chargeStartVoltage;		//�����ʼ��ѹ
	s16 				chargeFinishVoltage;	//�����ֹ��ѹ
	s16 				ChargeRestoreVoltage;	//���ָ���ѹ

	s16 				nBusVoltage;			//��ǰĸ�ߵ�ѹ
	s32 				*pwSpeedRef;			//�ٶȸ���ֵ
	s32 				*pwSpeedfbk;			//�ٶȷ���ֵ
	
}STR_MotorExceptionMA,*pSTR_MotorExceptionMA;		//	��ѹ�쳣����ṹ��


// handle function from MA 2 MD : MA write function
static __inline void	SetPWMCmdMA2MD(STR_PubDatMAMD *pMA2MD, ENUM_PWMStatus cmd)	//	MA����MD��PWM����
{
	pMA2MD->ePwmCmd	= cmd;
}

static __inline void	SetMotorCmdMA2MD(STR_PubDatMAMD *pMA2MD, ENUM_MotorStatus cmd)	//	MA����MD����״̬
{
	pMA2MD->eMotorStatus	= cmd;
}
static __inline void	SetIPDStartCmdMA2MD(STR_PubDatMAMD *pMA2MD, bool start)			//	MA����MD��IPD����
{
	pMA2MD->bIpdStart	= start;
}


static __inline void	SetBrakePWMDutyMA2MD(STR_PubDatMAMD *pMA2MD, int16_t duty)		//	MA����MD���ƶ����
{
	pMA2MD->nMotorBrakePwmDuty	= duty;
}

static __inline void	SetSpeedRefMA2MD(STR_PubDatMAMD *pMA2MD, int32_t speedRef)		//	MA����MD���ٶ�ָ��ֵ
{
	pMA2MD->wMotorSpeedRef	= speedRef;
}

static __inline void	SetCurRefMA2MD(STR_PubDatMAMD *pMA2MD, int16_t IdRef,int16_t IqRef)	//	MA����MD��DQ����ָ��ֵ
{
	pMA2MD->mStatCurDQCmd.nAxisD	= IdRef;
	pMA2MD->mStatCurDQCmd.nAxisQ	= IqRef;
}

static __inline void	SetMoeBlankTimeMA2MD(STR_PubDatMAMD *pMA2MD, int16_t blankTimeMs)	//	MA����MD��ָ������ʱ��
{
	pMA2MD->nMoeActionBlankTime		= blankTimeMs;
}

static __inline void	SetIPDEnableMA2MD(STR_PubDatMAMD *pMA2MD, uint8_t start)			//	MA����MD��IPD����	
{
	pMA2MD->bIpdStart	= start;
}

static __inline void	setDefaultCmdMA2MD(STR_PubDatMAMD *pMA2MD)			//	MA����MD��Ĭ��ָ��(�޲���)
{
	pMA2MD->nuFineCmd	= E_FC_NON;
}


static __inline void	setObsevInitFCmdMA2MD(STR_PubDatMAMD *pMA2MD)		//	MA����MD��۲�����ʼ��ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_INIT_OBS;
}

static __inline void	setAcrInitFCmdMA2MD(STR_PubDatMAMD *pMA2MD)			//	MA����MD���������ʼ��ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_INIT_ACR;
}

static __inline void	setSVPWMInitFCmdMA2MD(STR_PubDatMAMD *pMA2MD)		//	MA����MD��SVPWM��ʼ��ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_INIT_SVPWM;
}

static __inline void	setChargeAFCmdMA2MD(STR_PubDatMAMD *pMA2MD)			//	MA����MD��A��Ԥ���ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_CHARGE_A;
}
static __inline void	setChargeBFCmdMA2MD(STR_PubDatMAMD *pMA2MD)			//	MA����MD��B��Ԥ���ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_CHARGE_B;
}
static __inline void	setChargeCFCmdMA2MD(STR_PubDatMAMD *pMA2MD)			//	MA����MD��C��Ԥ���ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_CHARGE_C;
}

static __inline void	setChargeDoneInitFCmdMA2MD(STR_PubDatMAMD *pMA2MD)	//	MA����MD��Ԥ������ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_CHARGE_DONE_INIT;
}

static __inline void	setClrBreakInFaultFCmdMA2MD(STR_PubDatMAMD *pMA2MD)	//	MA����MD�����BreakInָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_CLR_BREAKIN_FAULT;
}

static __inline void	setIPDInitFCmdMA2MD(STR_PubDatMAMD *pMA2MD)			//	MA����MD��IPD��ʼ��ָ��
{
	pMA2MD->nuFineCmd	= E_FC_RESET_INIT_IPD;
}

static __inline void	setIPDCWStartFCmdMA2MD(STR_PubDatMAMD *pMA2MD)		//	MA����MD��IPD��ʼ��CWָ��
{
	pMA2MD->nuFineCmd	= E_FC_IPD_CW_START;
}

static __inline void	setIPDCCWStartFCmdMA2MD(STR_PubDatMAMD *pMA2MD)		//	MA����MD��IPD��ʼ��CCWָ��
{
	pMA2MD->nuFineCmd	= E_FC_IPD_CCW_START;
}











// handle function from MD 2 MA :  MA read function
static __inline ENUM_PWMStatus	getPWMStatMD2MA(STR_PubDatMDMA *pMD2MA)			//	MA��ȡMD��PWM״̬
{
	return pMD2MA->ePwmStatus;
}

static __inline ENUM_MotorStatus getMotorAckStatMD2MA(STR_PubDatMDMA *pMD2MA)	//	MA��ȡMD����״̬
{
	return pMD2MA->eMotorStatusAck;
}

static __inline bool getObsevInitResultMD2MA(STR_PubDatMDMA *pMD2MA)			//	MA��ȡMD��۲�����ʼ�����
{
	return (pMD2MA->nuFineStatus == E_FC_RESET_INIT_OBS);
}

static __inline bool getAcrInitResultMD2MA(STR_PubDatMDMA *pMD2MA)				//	MA��ȡMD���������ʼ�����
{
	return (pMD2MA->nuFineStatus == E_FC_RESET_INIT_ACR);
}

static __inline bool getSVPWMInitResultMD2MA(STR_PubDatMDMA *pMD2MA)			//	MA��ȡMD��SVPWM��ʼ�����
{
	return (pMD2MA->nuFineStatus == E_FC_RESET_INIT_SVPWM);
}

static __inline bool getNonInitResultMD2MA(STR_PubDatMDMA *pMD2MA)				//	MA��ȡMD�����״̬
{
	return (pMD2MA->nuFineStatus == E_FC_NON);
}

static __inline bool getIPDDoneResultMD2MA(STR_PubDatMDMA *pMD2MA)				//	MA��ȡMD��IPD��ɽ��
{
	return (pMD2MA->nuFineStatus == E_FC_IPD_DONE);
}

static __inline bool getBreakInClrResultMD2MA(STR_PubDatMDMA *pMD2MA)			//	MA��ȡMD��������BreakInָ����
{
	return (pMD2MA->nuFineStatus == E_FC_RESET_CLR_BREAKIN_FAULT);
}

static __inline bool getIPDInitResultMD2MA(STR_PubDatMDMA *pMD2MA)				//	MA��ȡMD��IPD��ʼ�����
{
	return (pMD2MA->nuFineStatus == E_FC_RESET_INIT_IPD);
}




#endif // _PUBDATA_MD_MA_H_
/////////////////////
