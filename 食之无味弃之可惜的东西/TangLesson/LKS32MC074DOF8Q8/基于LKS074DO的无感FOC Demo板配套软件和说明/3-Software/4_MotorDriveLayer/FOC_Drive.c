/*******************************************************************************
 * ��Ȩ���� (C)2016, LINKO Semiconduct
 *
 * �ļ����ƣ� FOC.Drive.c
 * �ļ���ʶ��
 * ����ժҪ�� ���ʸ�����Ƶ����ڻ����ؼ������㷨
 * ����˵���� ��
 * ��ǰ�汾�� V 2.0
 * ��    �ߣ� WhenCai Zhao Howlet Li
 * ������ڣ� 2020��9��10��
 *
 * �޸ļ�¼2��
 *    �޸����ڣ�2020��9��10��
 *    �� �� �ţ�V2.0
 *    �� �� �ˣ�Howlet Li
 *    �޸����ݣ���ʽ����
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "project_config.h"
#include "hardware_config.h"
#include "mc_math.h"
#include "MC_Parameter_M0.h"
#include "mc_declaration.h"
#include "MC_type.h"
#include "HALDrv.h"


extern STR_MotorFoc gS_FocObjM[MAX_DRV_MODULE_USED];
extern u8			g_buMdPWMONDelay[MAX_DRV_MODULE_USED];
/*******************************************************************************/
//�ⲿ����������

extern	void	InitFocVar(PSTR_MotorFoc tS_pFocObj);

static inline void	AdcCurSampleCalc(PSTR_MotorFoc tS_pFocObj);

extern	void	FaultCurrentCheck(STR_VectorUVW *pstruCurrent, u8 t_bPWM_ID);
extern	void	SetCmprDuty(PSTR_McpwmCmpr tS_Cmpr,u16 nuTime);

extern	s16		getMotorDaxisVoltFormObjId(u8 t_bObjId);
extern	s16		getMotorQaxisVoltFormObjId(u8 t_bObjId);

extern	s16		getMotorVoltFromObjId(u8 t_bObjId);
extern	s16		getMotorPowerFromObjId(u8 t_bObjId);

extern	s32		getOnFlyFreqResultFromObjId(u8 t_bObjId);
extern	s16		getMotorCurFromObjId(u8 t_bObjId);
extern	void	setTrakingMode(u8 t_bObjId, bool bEnable);

extern	s32		getOnFlyBemfFreqResultFromObjId(u8 t_bObjId);


extern	void	updateObserPLLVar1msLoop(PSTR_MotorFoc tS_pMotorFoc);
extern	void	vectorVFcontrol(u8 t_bObjId,s16 *pUd, s16 *pUq);
extern	s16		getDAxisVolt(u8 t_bObjId);
extern	s16		getQAxisVolt(u8 t_bObjId);
extern	void	SVPWM_3ShuntGetPhaseIaIb(PSTR_CurSamplingRes this);

extern	s16		getBemfAngComp(u8 t_objId);
extern	s16		getBemfTrackMinVolt(u8 t_objId);

extern	void	setTrakingModeEnable(u8 t_bObjId);
extern	void	setTrakingModeDisable(u8 t_bObjId);

extern	s16		getMotorActVoltFromObjId(u8 t_bObjId);
extern	s16		CurrentPIRegulator(STR_PIRegulator* tS_pAcr);

extern	s16 getMotorCurFromObjId1ms(u8 t_bObjId);
extern	s16 getMotorActVoltFromObjId1ms(u8 t_bObjId);

extern	s16		getParaVFCompCofig(u8 t_bObjId);

/*******************************************************************************
 �������ƣ�    void AdcSampleCal(stru_FOC_CtrProcDef *this)
 ����������    ��������
 ���������    PSTR_MotorFoc tS_pFocObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    1.�����������ɼ�AB���������ԭʼֵ��Ϊ���ۺ���ֵ
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0            Wencai.Zhao Howlet Li          ����
 2023/8/5      V1.1            Tonghua.Li          			  �޸����ݱ��۸�ʽ
 *******************************************************************************/
static inline void AdcCurSampleCalc(PSTR_MotorFoc tS_pFocObj)
{
    PSTR_CurSamplingRes this;
    this = &tS_pFocObj->mCurSamplingRes;  //ָ��
    STR_VectorUVW*	t_pStatCurrUVW = tS_pFocObj->m_pStatCurrUVW;

	this->mCurSamplingEvent.pAdcSamp(this);
    /* �������ۻ�����,����ADԭʼֵ��Q15��ʽ*/
	t_pStatCurrUVW->nPhaseU = this->pAdcRes->nSampDatU;
	t_pStatCurrUVW->nPhaseV = this->pAdcRes->nSampDatV;
	t_pStatCurrUVW->nPhaseW = this->pAdcRes->nSampDatW;

    /* clark�任 */
	tS_pFocObj->m_pStatCurrAB->nBeta = _IQ15mpy(18919, (t_pStatCurrUVW->nPhaseV - t_pStatCurrUVW->nPhaseW));
	tS_pFocObj->m_pStatCurrAB->nAlph = t_pStatCurrUVW->nPhaseU;
}

/*******************************************************************************
 �������ƣ�    static inline void RespondPwmCmd(PSTR_MotorFoc tS_pMotorFoc)
 ����������    ����MA�㷢�͵�MOE����
 ���������    PSTR_MotorFoc tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    1.IPDʱ��IPD��������ɿ���MOE
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0            Wencai.Zhao Howlet Li          ����
 2023/8/5      V1.1            Tonghua.Li          			  �޸�ע��
 *******************************************************************************/
static inline void RespondPwmCmd(PSTR_MotorFoc tS_pMotorFoc)
{
	/* ����MA��MD���MOE�����������Ӳ��������������MOE */
	if(E_PWM_ON == tS_pMotorFoc->pMaToMd->ePwmCmd)
	{
		if(++g_buMdPWMONDelay[tS_pMotorFoc->m_bFOCObj_ID] >= 2)
		{
			if(tS_pMotorFoc->pMaToMd->bIpdStart == 0)
			{
				
				tS_pMotorFoc->m_pHALHandle->PwmOutEnable(ENABLE);	//ʹ��MOE
			}
			tS_pMotorFoc->ePWMStatusAck		= E_PWM_ON;
			g_buMdPWMONDelay[tS_pMotorFoc->m_bFOCObj_ID] = 2;
		}
	}
	else
	{
		if(tS_pMotorFoc->pMaToMd->bIpdStart == 0)
		{
			tS_pMotorFoc->m_pHALHandle->PwmOutEnable(DISABLE);	//�ر�MOE
		}
		tS_pMotorFoc->ePWMStatusAck		= E_PWM_OFF;
		g_buMdPWMONDelay[tS_pMotorFoc->m_bFOCObj_ID] = 0;
	}
}

/*******************************************************************************
 �������ƣ�    static inline void RespondPWMStatus(PSTR_MotorFoc tS_pMotorFoc )
 ����������    ����MD��PWM״̬��MA��
 ���������    PSTR_MotorFoc tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/15      V1.0            Tonghua.Li          			  ����
 2023/9/05      V1.1            Tonghua.Li          			  �޸�ע��
 *******************************************************************************/
static inline void RespondPWMStatus(PSTR_MotorFoc tS_pMotorFoc )
{
	/* MD�㷵�ص�MA��MOE״̬ */
	tS_pMotorFoc->pMdToMa->ePwmStatus		= tS_pMotorFoc->ePWMStatusAck;
}

/*******************************************************************************
 �������ƣ�    static inline void RespondMotorStatus(PSTR_MotorFoc tS_pMotorFoc)		
 ����������    ����MD����״̬��������MA��
 ���������    PSTR_MotorFoc tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/15      V1.0            Tonghua.Li          			  ����
 2023/9/05      V1.1            Tonghua.Li          			  �޸�ע��
 *******************************************************************************/
static inline void RespondMotorStatus(PSTR_MotorFoc tS_pMotorFoc)
{
	/* MD�㷵�ص�MA����״̬ */
	tS_pMotorFoc->pMdToMa->eMotorStatusAck	= tS_pMotorFoc->eMotorStatusAck;
}

/*******************************************************************************
 �������ƣ�    static inline void updateMd2MaVar(PSTR_MotorFoc tS_pMotorFoc)
 ����������    ����MD�������Ʊ�����MA��
 ���������    PSTR_MotorFoc tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/15      V1.0            Tonghua.Li          			  ����
 2023/9/05      V1.1            Tonghua.Li          			  �޸�ע��
 *******************************************************************************/
static inline void updateMd2MaVar(PSTR_MotorFoc tS_pMotorFoc)
{
	u8 t_bObjId = tS_pMotorFoc->m_bFOCObj_ID;
	tS_pMotorFoc->pMdToMa->wFreq	= tS_pMotorFoc->m_pMotorFocSpeed->wSpeedEst;		//	����ת��(Ƶ��)
	tS_pMotorFoc->pMdToMa->nVdc		= tS_pMotorFoc->pHdToMd->nDcVolAdcDat;				//	ֱ����ѹ����ֵ

	tS_pMotorFoc->pMdToMa->mStatVolDQ.nAxisD = getMotorDaxisVoltFormObjId(t_bObjId);	//	d���ѹ
	tS_pMotorFoc->pMdToMa->mStatVolDQ.nAxisQ = getMotorQaxisVoltFormObjId(t_bObjId);	//	q���ѹ

	#if (QUICK_CURRENTKLOOP_FUNCTION == FUNCTION_ON)
	tS_pMotorFoc->pMdToMa->nMotorCur	= getMotorCurFromObjId1ms(t_bObjId);			//	����������ֵ
	tS_pMotorFoc->pMdToMa->nMotorVol	= getMotorActVoltFromObjId1ms(t_bObjId);		//	ʵ��ʩ�ӵ������ѹ SQRT(Ud^2 + Uq^2)
	#else
	tS_pMotorFoc->pMdToMa->nMotorCur	= getMotorCurFromObjId(t_bObjId);				//	����������ֵ
	tS_pMotorFoc->pMdToMa->nMotorVol	= getMotorActVoltFromObjId(t_bObjId);		//	ʵ��ʩ�ӵ������ѹ SQRT(Ud^2 + Uq^2)
	#endif

	
	tS_pMotorFoc->pMdToMa->nPowerDec	= getMotorPowerFromObjId(t_bObjId);			//	����ĵ������
	
	if(tS_pMotorFoc->eMotorStatusAck == E_MOTOR_TRACKING)
	{
		tS_pMotorFoc->pMdToMa->wFreq	= getOnFlyFreqResultFromObjId(t_bObjId);	//	˳���Ƶ��
	}
	
	#if (DRV0_BEMF_CHECK_EABLE==FUNCTION_ON)
	if(tS_pMotorFoc->eMotorStatusAck == E_MOTOR_BEMF_TRACKING)		//	�����Ƶ�·˳���
	{
		tS_pMotorFoc->pMdToMa->wFreq	= getOnFlyBemfFreqResultFromObjId(t_bObjId);	//	Bemf˳�����ת��
	}
	#endif
	
	tS_pMotorFoc->pMdToMa->bBreakInFault	= tS_pMotorFoc->pHdToMd->bBreakInStatus;	//	BreakIn�ź�(Ӳ������)
}

/*******************************************************************************
 �������ƣ�    static inline void updateMa2MdVar(PSTR_MotorFoc tS_pMotorFoc)
 ����������    ����MA�㷢�͵�MD�����״̬
 ���������    PSTR_MotorFoc tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/15      V1.0            Tonghua.Li          			  ����
 2023/9/05      V1.1            Tonghua.Li          			  �޸�ע��
 *******************************************************************************/
static inline void updateMa2MdVar(PSTR_MotorFoc tS_pMotorFoc)
{
	tS_pMotorFoc->m_pMotorFocSpeed->wSpeedRef	= tS_pMotorFoc->pMaToMd->wMotorSpeedRef;   /* ת��ָ�� */
}

/*******************************************************************************
 �������ƣ�    static inline void RegMotorCurrentDQInline(PSTR_MotorFoc tS_pFocObj)
 ����������    ������PI������������
 ���������    PSTR_MotorFoc tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �����������ڼ��ٺ�������ʱ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/15      V1.0            Tonghua.Li          			  ����
 2023/9/05      V1.1            Tonghua.Li          			  �޸�ע��
 *******************************************************************************/
static inline void RegMotorCurrentDQInline(PSTR_MotorFoc tS_pFocObj)
{
	if(!tS_pFocObj->bObserWork)
	{
		#if (VF_START_FUNCTION_ENABLE == FUNCTION_ON)		
		if(!getCurLoopCloseStatusFromOB(tS_pFocObj->m_bFOCObj_ID))	//����������
		{
			tS_pFocObj->m_pAcrQ->nError = 0;
			tS_pFocObj->m_pAcrD->nError = 0;
		}
		else														//�������ջ�
		#endif
		{		
			tS_pFocObj->m_pAcrQ->nError = (s32)tS_pFocObj->m_pStatCurrDQRef->nAxisQ - tS_pFocObj->m_pStatCurrDQ->nAxisQ; /* Q��������� */
			tS_pFocObj->m_pAcrD->nError = (s32)tS_pFocObj->m_pStatCurrDQRef->nAxisD - tS_pFocObj->m_pStatCurrDQ->nAxisD;  /* D��������� */
		}

		tS_pFocObj->m_pVolDQ->nAxisQ = CurrentPIRegulator(tS_pFocObj->m_pAcrQ);		/* Q�����PI���㣬���Vq */
		tS_pFocObj->m_pVolDQ->nAxisD = CurrentPIRegulator(tS_pFocObj->m_pAcrD);		/* D�����PI���㣬���Vd */
	}

	//	��ѹ����Բ���ƣ��ڲ������Զ������ƹ���
	CalcVolCircle(tS_pFocObj->m_bFOCObj_ID,tS_pFocObj->m_pVolDQ,tS_pFocObj->m_pVectVolAB);
}


/*******************************************************************************
 �������ƣ�    static void MDHandleCmdIn1msLoop(PSTR_MotorFoc tS_pMotorFoc) 
 ����������    MA��MD�����
 ���������    PSTR_MotorFoc tS_pMotorFoc    FOC����ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����		����1ms�������MA/MD�����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/07/15      V1.1       Tonghua Li			����Ϊ1ms�����������ж�ʱ��
 *******************************************************************************/
static void MDHandleCmdIn1msLoop(PSTR_MotorFoc tS_pMotorFoc)
{	
	u8	t_bObjId = tS_pMotorFoc->m_bFOCObj_ID;
	s16 t_compFactor = 0;
	
	if((E_FC_RESET_INIT_OBS == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_INIT_OBS != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		FluxObserveParaCalc(t_bObjId);				//�۲���������ʼ��
		#if (VECTOR_VF_COMP_ENABLE == FUNCTION_ON)
		CalcVectorVFPara(t_bObjId);						//VF������ʼ��

		t_compFactor = getParaVFCompCofig(t_bObjId);
		if(t_compFactor != 0)
		{
			setDeadCompFactor(t_bObjId, t_compFactor);	// ����ϵ��
		}
		#endif

		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_INIT_OBS;
	}
	else if((E_FC_RESET_INIT_ACR == tS_pMotorFoc->pMaToMd->nuFineCmd) 
			&& (E_FC_RESET_INIT_ACR != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		InitFocVar(tS_pMotorFoc);						//������������ʼ��
		InitPmFluxOB(t_bObjId);							//�۲���������ʼ��
		#if (DRV0_DIR_CHECK_ENABLE==FUNCTION_ON)	
		OnTheFlyDetectInit(t_bObjId);					//˳��������ʼ��
		#endif
				
		#if (DRV0_BEMF_CHECK_EABLE==FUNCTION_ON)
		Bemf_CheckInit(t_bObjId,getBemfAngComp(t_bObjId),getBemfTrackMinVolt(t_bObjId));   /*�����Ƽ�������ʼ��*/
		#endif
		
		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_INIT_ACR;
	}
	else if((E_FC_RESET_INIT_SVPWM == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_INIT_SVPWM != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		InitFocVar(tS_pMotorFoc);						//������������ʼ��
		InitPmFluxOB(t_bObjId);							//�۲���������ʼ��
		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_INIT_SVPWM;
	}
	
	else if((E_FC_RESET_INIT_IPD == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_INIT_IPD != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		InitIPD(t_bObjId);                /*IPD��ʼ��*/
		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_INIT_IPD;
	}
	
	else if((E_FC_IPD_CW_START == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_IPD_CCW_START != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		tS_pMotorFoc->pMdToMa->nuFineStatus = tS_pMotorFoc->pMdToMa->nuFineStatus;
	}	
	
	else if((E_FC_RESET_CHARGE_A == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_CHARGE_A != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		tS_pMotorFoc->m_pHALHandle->PwmCharge(1);		//A��Ԥ���
		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_CHARGE_A;
	}
	else if((E_FC_RESET_CHARGE_B == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_CHARGE_B != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		tS_pMotorFoc->m_pHALHandle->PwmCharge(2);		//B��Ԥ���
		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_CHARGE_B;
	}
	else if((E_FC_RESET_CHARGE_C == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_CHARGE_C != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		tS_pMotorFoc->m_pHALHandle->PwmCharge(3);		//C��Ԥ���
		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_CHARGE_C;
	}
	else if((E_FC_RESET_CHARGE_DONE_INIT == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_CHARGE_DONE_INIT != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		tS_pMotorFoc->m_pHALHandle->InitPwmChargeEnd();	//Ԥ�������󣬳�ʼ������
		tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_CHARGE_DONE_INIT;
	}
	else if((E_FC_RESET_CLR_BREAKIN_FAULT == tS_pMotorFoc->pMaToMd->nuFineCmd) 
		&& (E_FC_RESET_CLR_BREAKIN_FAULT != tS_pMotorFoc->pMdToMa->nuFineStatus))
	{
		tS_pMotorFoc->m_pHALHandle->ClrEPWMBreakIn();						//���breakin�ź�
		tS_pMotorFoc->m_pHALHandle->GetEPWMBreakIn(tS_pMotorFoc->pHdToMd);	//��ȡbreakin�ź�
		if(!tS_pMotorFoc->pHdToMd->bBreakInStatus)
		{				
			tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_RESET_CLR_BREAKIN_FAULT;
		}
	}
	else
	{
		tS_pMotorFoc->pMdToMa->nuFineStatus = tS_pMotorFoc->pMaToMd->nuFineCmd;
	}
			
	if(E_FC_NON == tS_pMotorFoc->pMaToMd->nuFineCmd)
	{
		tS_pMotorFoc->pMdToMa->nuFineStatus		= E_FC_NON;			//Ĭ������״̬
	}
}
/*******************************************************************************
 �������ƣ�    static void FOC_Model(PSTR_MotorFoc tS_pMotorFoc) 
 ����������    PMSM���Q��ת�ؿ��ƣ�D��������ƴ���ʵ��FOC���Ƶĺ��ĵ�����
 ���������    PSTR_MotorFoc tS_pMotorFoc    FOC����ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/10/16      V1.0       Wencai.Zhao				����
 2020/12/15      V1.1       Tonghua Li				����TRAKING/IPD�����������
 *******************************************************************************/
static inline void FOC_Model(PSTR_MotorFoc tS_pMotorFoc)
{	
	ENUM_MotorStatus t_eMotorStatus = E_MOTOR_RESET;

	s16 t_nTmp;
	u8					t_bObjId		= tS_pMotorFoc->m_bFOCObj_ID;
	PSTR_VectorAB		t_pStatCurrAB	=  tS_pMotorFoc->m_pStatCurrAB;		//���ݵ���ABָ��
	STR_TrigComponents	*t_pSinCos		= &tS_pMotorFoc->mSinCosFocAngle;	//sin cos������
	STR_PubDatMAMD*		t_pMaToMd		 =  tS_pMotorFoc->pMaToMd;			//����FOC����ָ��

	AdcCurSampleCalc(tS_pMotorFoc);            /*����������������*/

	if(tS_pMotorFoc->bObserWork)
	{
		tS_pMotorFoc->bObserWork = FALSE;
		CalcDcVoltPerUnit(t_bObjId,tS_pMotorFoc->pHdToMd->nDcVolAdcDat);
	}
	else
	{
		tS_pMotorFoc->bObserWork = TRUE;
	}
	
	setLibWorkMode(t_bObjId, tS_pMotorFoc->bObserWork);
	
	t_eMotorStatus = t_pMaToMd->eMotorStatus;

	switch(t_eMotorStatus)
	{
		case E_MOTOR_RESET:    /* �����λ�ȴ� */
		{			
			t_nTmp		= ((s32)t_pMaToMd->nMotorBrakePwmDuty * tS_pMotorFoc->m_pSvpwmPar->nuPwmPrd)>>12;
			SetCmprDuty(tS_pMotorFoc->m_pCmpr,t_nTmp);      /* ����PWM�Ƚ�װ����ֵ */
			
			tS_pMotorFoc->m_pHALHandle->MCPWMx_RegUpdate(tS_pMotorFoc->pMdToHd);  /* ����PWMģ��ռ�ձ�ֵ */

			#if ((DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_3SHUNT) || (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_MOSFET))
			tS_pMotorFoc->pMdToHd->mMcTmr.eChannel = E_PHASE_A_B;	/* ���ò���ͨ��ΪAB */
			tS_pMotorFoc->m_pHALHandle->AD_ChannelUpdate(tS_pMotorFoc->pMdToHd);
			#endif

			break;
		}

		#if (DRV0_BEMF_CHECK_EABLE==FUNCTION_ON)
		case E_MOTOR_BEMF_TRACKING:     /* ������˳����� */
		{
			tS_pMotorFoc->m_pHALHandle->PwmOutEnable(DISABLE);	//�ر�MOE,���������Ƶ�ѹ

			
			Bemf_CheckProcess(t_bObjId, 
								tS_pMotorFoc->pHdToMd->nBmefWVolAdcDat, 
								tS_pMotorFoc->pHdToMd->nBmefVVolAdcDat, 
								tS_pMotorFoc->pHdToMd->nBmefUVolAdcDat);    //C�� B�� A��
			

			BemfSwitch(t_bObjId,FALSE);   //�л���ֵ
			break;
		}
		#endif
			
		#if (DRV0_DIR_CHECK_ENABLE==FUNCTION_ON)		
		case E_MOTOR_TRACKING:        /* �۲���˳����� */
		{
			setTrakingModeEnable(t_bObjId);			/* ʹ��˳����� */
			if(!tS_pMotorFoc->bObserWork)
			{
				OnTheFlyDetectPro(t_bObjId);		/* ˳�������㺯�� */
			}
			/*�˴�����break��˳�������Ҫ�õ�����������*/
		}
		#endif
		case E_MOTOR_ASYN:
		case E_MOTOR_ALIGN:           /* ���� */
		case E_MOTOR_PRE_SYN:
		case E_MOTOR_SYN:
		{			
			if(t_eMotorStatus != E_MOTOR_TRACKING)
			{
				setTrakingModeDisable(t_bObjId);
			}
			//if(E_PWM_ON == tS_pMotorFoc->ePWMStatusAck)
			{
				tS_pMotorFoc->m_pMotorAngle->wElectAngle = tS_pMotorFoc->m_pMotorAngle->wElectAngleEst;          /* �۲����Ƕȴ��� */

				#if (MCU_DSP_SINCOS == FUNCTION_OFF)
				Trig_Functions((tS_pMotorFoc->m_pMotorAngle->wElectAngle >> 16),&tS_pMotorFoc->mSinCosFocAngle); /* sincos��� */
				#else
				DSP0_SC |= BIT2;
    			DSP0_THETA = (tS_pMotorFoc->m_pMotorAngle->wElectAngle >> 16);                           /* sincosͨ��DSP���� */
				tS_pMotorFoc->mSinCosFocAngle.hCos	= DSP0_COS;
				tS_pMotorFoc->mSinCosFocAngle.hSin	= DSP0_SIN;				
				#endif

				// alph/beta to dq ����任		
				tS_pMotorFoc->m_pStatCurrDQ->nAxisQ  = (((s32)t_pStatCurrAB->nBeta * t_pSinCos->hCos) -
				                        ((s32)t_pStatCurrAB->nAlph * t_pSinCos->hSin)) >> 15;
				tS_pMotorFoc->m_pStatCurrDQ->nAxisD  = (((s32)t_pStatCurrAB->nAlph* t_pSinCos->hCos) +
				                        ((s32)t_pStatCurrAB->nBeta * t_pSinCos->hSin)) >> 15;

				// dq������ PI ����
				RegMotorCurrentDQInline(tS_pMotorFoc);

				// SVPWM ����
				tS_pMotorFoc->m_pHandle->Svpwm(t_bObjId,tS_pMotorFoc->m_pVectVolAB);

				// PWM ռ�ձȸ���			
				tS_pMotorFoc->m_pHALHandle->MCPWMx_RegUpdate(tS_pMotorFoc->pMdToHd);
							
				//  AD ͨ������
				#if ((DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_3SHUNT) || (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_MOSFET))
				tS_pMotorFoc->m_pHALHandle->AD_ChannelUpdate(tS_pMotorFoc->pMdToHd);
				#endif
				

				// �۲������㺯��
				PmsmFluxObserve_Full(t_bObjId,tS_pMotorFoc->m_pStatCurrDQ);
				//PmsmFluxObserveIDSimply(t_bObjId,tS_pMotorFoc->m_pStatCurrDQ);
			}			
		break;
		}
		
		case E_MOTOR_BRAKE:
		{   /* PWM brake */

			// handle PWM brake duty
			t_nTmp		= ((s32)t_pMaToMd->nMotorBrakePwmDuty * tS_pMotorFoc->m_pSvpwmPar->nuPwmPrd)>>12;
			if(t_nTmp >= tS_pMotorFoc->m_pSvpwmPar->nuPwmPrd)
			{
				t_nTmp	= tS_pMotorFoc->m_pSvpwmPar->nuPwmPrd;    /* ռ�ձ��޷����� */
			}
			SetCmprDuty(tS_pMotorFoc->m_pCmpr,t_nTmp);          /* ռ�ձ���ֵ���� */
			
			// PWM ռ�ձȸ���	 
			tS_pMotorFoc->m_pHALHandle->MCPWMx_RegUpdate(tS_pMotorFoc->pMdToHd);
			
			// ���ݲ�����ʽ����Ĭ�ϲ���״̬
			#if ((DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_3SHUNT) || (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_MOSFET))
			tS_pMotorFoc->pMdToHd->mMcTmr.eChannel = E_PHASE_A_B;						/* ����ģʽѡ�� */
			tS_pMotorFoc->mCurSamplingRes.mCurSamplingEvent.bSamplingEv1 = SAMP_IA;		/* A���������  */
			tS_pMotorFoc->mCurSamplingRes.mCurSamplingEvent.bSamplingEv2 = SAMP_IB;		/* B���������  */
			tS_pMotorFoc->m_pHALHandle->AD_ChannelUpdate(tS_pMotorFoc->pMdToHd);		/* ADͨ������   */	
			tS_pMotorFoc->mCurSamplingRes.mCurSamplingEvent.pAdcSamp	 = (ADCSampFunction)&SVPWM_3ShuntGetPhaseIaIb;
			#endif

			InitFocVar(tS_pMotorFoc);           /* FOC������ʼ�� */
			InitPmFluxOB(t_bObjId);				/* �۲���������ʼ�� */
			break;
		}
		
		#if (DRV0_POS_SEEK_ENABLE==FUNCTION_ON)    /* �����IPD���� */
		#if 1
		case E_MOTOR_IPD:         /*IPD��ʼλ�ü��*/
		{
			if( (E_FC_IPD_CW_START == t_pMaToMd->nuFineCmd)
				||(E_FC_IPD_CCW_START == t_pMaToMd->nuFineCmd)
			  )
			{
				if(tS_pMotorFoc->m_bIPDFinsh == 0)
				{
					__disable_irq();                  /* �ر��ж� �ж��ܿ��� */
					ADC_SOFTWARE_TRIG_ONLY();
    				ADC_STATE_RESET();
					IPDEst_SMotor(t_bObjId);
					tS_pMotorFoc->m_pHALHandle->InitPwmChargeEnd();
					__enable_irq();                   /* �������ж� */

					tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_IPD_DOING;
				}
				else
				{
					// �Ƕȳ�ʼ����������IPD���
					tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_IPD_DONE;
				}
			}
			break;
		}
		#else									/* ˫���IPD���� */
		case E_MOTOR_IPD:          				/*IPD��ʼλ�ü��*/
		{
			IPDEst_DMotor(t_bObjId);			//	˫���IPD
			tS_pMotorFoc->m_pHALHandle->MCPWMx_RegUpdate(tS_pMotorFoc->pMdToHd);
			if( (E_FC_IPD_CW_START == tS_pMotorFoc->pMaToMd->nuFineCmd)
				||(E_FC_IPD_CCW_START == tS_pMotorFoc->pMaToMd->nuFineCmd)
			  )
			{
				if(tS_pMotorFoc->m_bIPDFinsh == 1)
				{
					// �Ƕȳ�ʼ����������IPD���
					tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_IPD_DONE;    /* ״̬��IPD_DONE */
				}
				else
				{
					tS_pMotorFoc->pMdToMa->nuFineStatus = E_FC_IPD_DOING;   /* ״̬��IPD_DOING */
				}
			}
			break;
		}
		#endif
		#endif
		
		default:
			
        break;
	}

	#if (QUICK_CURRENTKLOOP_FUNCTION == FUNCTION_ON)		//���ٵ�����ģʽ
		tS_pMotorFoc->eMotorStatusAck	= t_eMotorStatus;					//���״̬������
		tS_pMotorFoc->m_pHALHandle->GetEPWMBreakIn(tS_pMotorFoc->pHdToMd);  //PWMbreakin״̬����
		RespondPwmCmd(tS_pMotorFoc);
	#else													//���ٵ�����ģʽ�����д����ڵ��������
		if(E_FC_NON == t_pMaToMd->nuFineCmd)
		{
			tS_pMotorFoc->pMdToMa->nuFineStatus		= E_FC_NON;
		}
		updateMa2MdVar(tS_pMotorFoc);										//����MA to MD ״̬����ѹ�ٶȵ���Ϣ
		
		tS_pMotorFoc->m_pHALHandle->GetEPWMBreakIn(tS_pMotorFoc->pHdToMd);	//PWMbreakin״̬����
		RespondPwmCmd(tS_pMotorFoc);										//���� MOE����
		tS_pMotorFoc->eMotorStatusAck	= t_eMotorStatus;					//���״̬������
		RespondPWMStatus(tS_pMotorFoc);										// ����MA��MOE״̬
		RespondMotorStatus(tS_pMotorFoc);									// ����MA����״̬
		updateMd2MaVar(tS_pMotorFoc);										// ����MD��MA����״̬����Ϣ
		PmsmFluxObserve1ms(t_bObjId);										// �۲������ٴ��������ɷŵ�1ms��
	#endif
}

/*******************************************************************************
 �������ƣ�    void MDtaskHandleIn1msLoop(void)
 ����������    MD��1ms����
 ���������    void
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   ִ��1ms������
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/02/15      V1.1       Tonghua Li				����
 *******************************************************************************/
#if (QUICK_CURRENTKLOOP_FUNCTION == FUNCTION_ON)        //���ٵ�����ģʽ

void MDtaskHandleIn1msLoop(void)
{
	PSTR_MotorFoc tS_pMotorFoc = &gS_FocObjM[0];

	MDHandleCmdIn1msLoop(tS_pMotorFoc);   //1ms�������

	/* ����ָ�봫�� */
	updateMa2MdVar(tS_pMotorFoc);					//����MA to MD ״̬����ѹ�ٶȵ���Ϣ
		
	RespondPWMStatus(tS_pMotorFoc);					// ����MA��MOE״̬
	RespondMotorStatus(tS_pMotorFoc);				// ����MA����״̬
	updateMd2MaVar(tS_pMotorFoc);					// ����MD��MA����״̬����Ϣ

	PmsmFluxObserve1ms(0);							// �۲�������1ms������
		
	#if (OBSERVER_PLL_CHANGE_ENABLE == FUNCTION_ON)
	updateObserPLLVar1msLoop(tS_pMotorFoc);		// �۲���PLL����������
	#endif
		
	#if (VECTOR_VF_COMP_ENABLE == FUNCTION_ON)		// ��ѹ������
			vectorVFcontrol(0,&tS_pMotorFoc->m_pAcrD->nComp,&tS_pMotorFoc->m_pAcrQ->nComp);
		#endif
}

#else   //���ٵ�����ģʽ
	/* �����1ms�������� */
void MDtaskHandleIn1msLoop(void)
{
	PSTR_MotorFoc tS_pMotorFoc = &gS_FocObjM[0];		
	
	MDHandleCmdIn1msLoop(tS_pMotorFoc);     //1ms�������
	
	#if (OBSERVER_PLL_CHANGE_ENABLE == FUNCTION_ON)
	updateObserPLLVar1msLoop(tS_pMotorFoc);			// �۲���PLL����������
	#endif
	
	#if (VECTOR_VF_COMP_ENABLE == FUNCTION_ON)		// ��ѹ������
	vectorVFcontrol(0,&tS_pMotorFoc->m_pAcrD->nComp,&tS_pMotorFoc->m_pAcrQ->nComp);
	#endif
}

#endif

/*******************************************************************************
 �������ƣ�    void AdcEocIsrDRV0(void) 
 ����������    PMSM PWM0 FOC����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/10/16      V1.0       Wencai.Zhao				����
 2020/12/15      V1.1       Tonghua Li				����HAL�������ӿ�
 *******************************************************************************/
void AdcEocIsrDRV0(void)
{
	PSTR_MotorFoc tS_pMotorFoc;
	STR_PubDatHDMD* tS_pCurAdcRes;
	tS_pMotorFoc = &gS_FocObjM[0];

	// ���ݵ�ѹ��������AD��������
	tS_pCurAdcRes = tS_pMotorFoc->pHdToMd;
	#if 0   //ָ�봫��AD��ֵ���ٶ���
	tS_pMotorFoc->m_pHALHandle->AcquireCurrent(tS_pCurAdcRes);
	tS_pMotorFoc->m_pHALHandle->AcquireVdc(tS_pCurAdcRes);
	#else   //����������ȡAD��ֵ���ٶȿ�
	AcquireCurSamplingResultsDRV0(tS_pCurAdcRes);
	AcquireVdcSamplingResultsDRV0(tS_pCurAdcRes);	
	#endif
	
	FOC_Model(tS_pMotorFoc);    /* FOCʸ������ */

	#if 0
	// ʱ��Ҫ���ϸ�ʱ����1ms������ʱ����ͬ�Ĺ���
	if(tS_pMotorFoc->pMaToMd->eMotorStatus == E_MOTOR_SYN)
	{
		FaultCurrentCheck(tS_pMotorFoc->m_pStatCurrUVW, tS_pMotorFoc->m_bFOCObj_ID);
	}
	#endif
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

