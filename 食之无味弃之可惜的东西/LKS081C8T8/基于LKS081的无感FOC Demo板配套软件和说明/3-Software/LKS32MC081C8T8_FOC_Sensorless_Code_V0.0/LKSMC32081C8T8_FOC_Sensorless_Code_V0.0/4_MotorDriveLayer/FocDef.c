/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� FocDef.c
 * �ļ���ʶ��
 * ����ժҪ�� Define the basic function of motor vector control
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� LKS/MC TEAM
 * ������ڣ� 2022��8��11��
 *******************************************************************************/

/* Standard include ----------------------------------------------------------*/
#include "mc_declaration.h"
#include "MathLIB_Inlines.h"
#include "math.h"
#include "MC_type.h"
#include "MC_Math.h"
#include "hardware_config.h"
#include "PubData_DriverInstance.h"

// ������ƶ����±����
// ����꾲̬ʵ��
// ��̬ʵ�ַ�ʽ��������

/*******************************************************************************/
//ȫ�ֱ���������

STR_MotorFoc 						gS_FocObjM[MAX_DRV_MODULE_USED];			// FOC ���ƶ�����
STR_MotorFocElement					gS_FocElementObjM[MAX_DRV_MODULE_USED];		// FOC ��������
u8									g_buMdPWMONDelay[MAX_DRV_MODULE_USED];		// PWM ON delay ����

const STR_MotorFocHandle			gS_MotorFocHandleM[MAX_DRV_MODULE_USED] = {		// FOC ģ��ʵ�ֺ�����Ŀǰ��ΪSVPWM����
																	Motor_FocMethod_M0
																	};

extern const STR_HALDrvHandle	 	gS_MotorHalDrvHandleM[MAX_DRV_MODULE_USED];		//	Ӳ������ʵ�ֺ������ݽṹ

void SVPWM_1ShuntGetPhaseIcNIa(PSTR_CurSamplingRes this);
void SVPWM_2ShuntGetPhaseIaIb(PSTR_CurSamplingRes this);
void SVPWM_3ShuntGetPhaseIaIb(PSTR_CurSamplingRes this);


/*******************************************************************************/
//�ⲿ����������
extern	PSTR_ControlObject	getCtrlObjectPtr(u8 objID);
extern	STR_ControlObject	gS_PhyObjInstanceM[MAX_DRV_MODULE_USED];

extern	void				RegisterObsDatStr(PSTR_MotorFoc tS_pMotorFoc);

extern	s16					Sqrt_Functions(s16 x,s16 y);
extern	void				setObserPLLKpKi(u8 t_bObjId,u16 Kp, u16 Ki);
extern	s32					User2AppFreqTrans(STR_TransCoefElement* pUser2App,float val);

extern	bool				getCurLoopCloseStatusFromOB(u8 t_bObjId);
extern	s16					getVdcCirCleLim(u8 tobjId);

/*******************************************************************************/
//��̬�ֲ�������
/*****************************************************************************************/	 

/*******************************************************************************
 �������ƣ�    RegisterMdGlobal
 ����������    ע����ƶ������ݽṹ
 ���������    �����ƶ���ָ��tS_pMotorFoc��ȫ������ָ��tS_pControlObject
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    MD����ƶ������ݽṹע�ᣬ���ݽ����������۱任ϵ����FOC����

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           WenCai Zhao          ����
 *******************************************************************************/
static void RegisterMdGlobal(PSTR_MotorFoc tS_pMotorFoc,PSTR_ControlObject tS_pControlObject)
{
	tS_pMotorFoc->m_bPWM_ID			= tS_pControlObject->m_bPWM_Id;							//PWM ID
	tS_pMotorFoc->m_bSampType		= (u8)tS_pControlObject->m_pDrvCfgPara->mS_GlobalCfg.m_bSampType; //����������ʽ
	tS_pMotorFoc->pHdToMd			= &tS_pControlObject->mGlobalDatPackage.mHdToMd;		//Hd�㵽Md��Dataָ�� ��ֵ
	tS_pMotorFoc->pMdToHd			= &tS_pControlObject->mGlobalDatPackage.mMdToHd;		//Md�㵽Hd��Dataָ�� ��ֵ
	
	tS_pMotorFoc->pMaToMd			= &tS_pControlObject->mGlobalDatPackage.mMaToMd;		//Ma�㵽Md��Dataָ�� ��ֵ
	tS_pMotorFoc->pMdToMa			= &tS_pControlObject->mGlobalDatPackage.mMdToMa;		//Md�㵽Ma��Dataָ�� ��ֵ
	
	tS_pMotorFoc->m_pTransCoef		= &tS_pControlObject->mTransCoef;						//����ϵ��ָ�븳ֵ
	
	tS_pMotorFoc->m_pMotorFocPar	= tS_pControlObject->m_pMotorFocPar;					//FOC����ָ�븳ֵ
		
}

/*******************************************************************************
 �������ƣ�    RegisterMethodHandle
 ����������    FOC����ʵ�ַ���ָ�뺯��
 ���������    �����ƶ���ָ��tS_pFocObj��ȫ������ָ��tS_pMotorFocHandle
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           WenCai Zhao          ����
 *******************************************************************************/
static void RegisterMethodHandle(PSTR_MotorFoc tS_pFocObj, PSTR_MotorFocHandle tS_pMotorFocHandle)
{
	tS_pFocObj->m_pHandle		= tS_pMotorFocHandle;
}

/*******************************************************************************
 �������ƣ�    RegisterHalDrvHandle
 ����������    MD2HD�������ú����ӿ�
 ���������    �����ƶ���ָ��tS_pFocObj��ȫ������ָ��tS_pMotorHalDrvHandle
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           WenCai Zhao          ����
 *******************************************************************************/
static void RegisterHalDrvHandle(PSTR_MotorFoc tS_pFocObj, PSTR_HALDrvHandle tS_pMotorHalDrvHandle)
{
	tS_pFocObj->m_pHALHandle	= tS_pMotorHalDrvHandle;
}

/*******************************************************************************
 �������ƣ�    RegisterMdLocal
 ����������    ע����ƶ��󱾵����ݽṹ
 ���������    �����ƶ���ָ��tS_pMotorFoc��local����ָ��tS_pMotorFocElement
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    MD����ƶ������ݽṹע�ᣬMD��FOC����local����

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           WenCai Zhao          ����
 *******************************************************************************/
static void RegisterMdLocal(PSTR_MotorFoc tS_pMotorFoc,PSTR_MotorFocElement tS_pMotorFocElement)
{
	tS_pMotorFoc->m_pStatCurrUVW	= &tS_pMotorFoc->pMdToMa->mStatCurrUVW;			//���ӵ���UVW
	tS_pMotorFoc->m_pStatCurrDQ		= &tS_pMotorFoc->pMdToMa->mStatCurDQ;			//���ӵ���DQ
	tS_pMotorFoc->m_pStatCurrDQRef	= &tS_pMotorFoc->pMaToMd->mStatCurDQCmd;		//���ӵ�������

	tS_pMotorFoc->m_pAcrD			= &tS_pMotorFocElement->mAcrD;					//d���������
	tS_pMotorFoc->m_pVolDQ			= &tS_pMotorFocElement->mAcrOutDQ;				//���������
	tS_pMotorFoc->m_pAcrQ			= &tS_pMotorFocElement->mAcrQ;					//q���������
	tS_pMotorFoc->m_pMotorAngle		= &tS_pMotorFocElement->mMotorAngle;			//����Ƕ�

	tS_pMotorFoc->m_pCmpr			= &tS_pMotorFoc->pMdToHd->mMcPwm;				//��������㷵��ֵ
	tS_pMotorFoc->m_pTmr			= &tS_pMotorFoc->pMdToHd->mMcTmr;				//��������㷵��ֵ

	tS_pMotorFoc->m_pMotorFocSpeed	= &tS_pMotorFocElement->mMotorSpeed;			//���ת��
	tS_pMotorFoc->m_pVectVolAB		= &tS_pMotorFocElement->mStatVolAB;				//���ӽǶ�
	tS_pMotorFoc->m_pFluxObsGain	= &tS_pMotorFocElement->mFluxObsGain;			//�۲���������������
	
	tS_pMotorFoc->m_pSvpwmPar		= &tS_pMotorFoc->m_pMotorFocPar->mSvpwmPar;		//SvPwm����
	tS_pMotorFoc->m_pFocObjPar		= &tS_pMotorFoc->m_pMotorFocPar->mObjPar;		//FOCӲ������
	tS_pMotorFoc->m_pFocObsPar		= &tS_pMotorFoc->m_pMotorFocPar->mObsPar;		//FOC�۲�������
	
	tS_pMotorFoc->mCurSamplingRes.pAdcRes	= tS_pMotorFoc->pHdToMd;				//����ֵ
	tS_pMotorFoc->m_pStatCurrAB				= &tS_pMotorFocElement->mStatCurrAB;	//���ӵ���AB
	
}

/*******************************************************************************
 �������ƣ�    InitCmpr
 ����������    �Ƚ������ݳ�ʼ��
 ���������    �����ƶ���ָ��tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��ʼ��PWM�Ƚ�������ΪĬ��ֵ

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           WenCai Zhao          ����
 *******************************************************************************/
static void InitCmpr(PSTR_MotorFoc tS_pFocObj)
{
	u16 t_cmpr;
	t_cmpr = tS_pFocObj->m_pSvpwmPar->nuPwmPrd>>1;
	tS_pFocObj->m_pCmpr->nuTimePhA  =  t_cmpr;    //PhA ��ֵ
	tS_pFocObj->m_pCmpr->nuTimePhB  =  t_cmpr;    //PhB ��ֵ
	tS_pFocObj->m_pCmpr->nuTimePhC  =  t_cmpr;    //PhC ��ֵ
	tS_pFocObj->m_pCmpr->nuTimePhAN = -t_cmpr;    //PhAN ��ֵ
	tS_pFocObj->m_pCmpr->nuTimePhBN = -t_cmpr;    //PhBN ��ֵ
	tS_pFocObj->m_pCmpr->nuTimePhCN = -t_cmpr;    //PhCN ��ֵ
}

/*******************************************************************************
 �������ƣ�    InitMotorDriveLayer
 ����������    MD����������ʼ������
 ���������    void
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��ʼ��MD�����ݽṹ��

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           WenCai Zhao          ����
 2023/2/19      V1.01          Tonghua Li           �޸Ĳ��ֹ���
 *******************************************************************************/
/* �����MD���ʼ������ */
void InitMotorDriveLayer(void)
{
	s16 factor;
	gS_FocObjM[0].m_bFOCObj_ID	= 0;
	RegisterMdGlobal(&gS_FocObjM[0],&gS_PhyObjInstanceM[0]); //ע����ƶ���ȫ�����ݽṹ
	#if (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_1SHUNT)        //�����������ʽ
		gS_FocObjM[0].mCurSamplingRes.mCurSamplingEvent.pAdcSamp = (ADCSampFunction)&SVPWM_1ShuntGetPhaseIcNIa;
	#elif (DRV0_CUR_SAMP_TYPE == CURRENT_SAMPLE_2SHUNT)	    //˫���������ʽ	
		gS_FocObjM[0].mCurSamplingRes.mCurSamplingEvent.pAdcSamp = (ADCSampFunction)&SVPWM_2ShuntGetPhaseIaIb;
	#else                                                   //�������MOS���������ʽ
		gS_FocObjM[0].mCurSamplingRes.mCurSamplingEvent.pAdcSamp = (ADCSampFunction)&SVPWM_3ShuntGetPhaseIaIb;
	#endif
	
	RegisterMdLocal(&gS_FocObjM[0],&gS_FocElementObjM[0]);  //ע����ƶ��󱾵����ݽṹ
	RegisterMethodHandle(&gS_FocObjM[0],(PSTR_MotorFocHandle)&gS_MotorFocHandleM[0]);   //ע��FOC����ʵ�ַ���ָ�뺯��

	RegisterObsDatStr(&gS_FocObjM[0]);                      //ע��۲�������ָ��

	RegisterHalDrvHandle(&gS_FocObjM[0],(PSTR_HALDrvHandle)&gS_MotorHalDrvHandleM[0]);  //ע��MD2HD�������ú����ӿ�

	factor = gS_PhyObjInstanceM[0].m_pDrvCfgPara->mS_GlobalCfg.m_nOverModuFactor;
	setOverModuFac(0, factor);
}

PSTR_MotorFoc getFocObject(u8 t_bPWMId)
{
	return &gS_FocObjM[0];	
}

/*******************************************************************************
 �������ƣ�    getAngleErrOpen2Close
 ����������    �������ǶȺ�ǿ�ϸ����Ƕȵ����
 ���������    t_bObjId
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    MA����øú�������ǶȺ�ǿ�ϸ����Ƕȵ����

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/10/19     V1.0          Tonghua Li         �޸Ĳ��ֹ���
 *******************************************************************************/
s32 getAngleErrOpen2Close(u8 t_bObjId)
{
	PSTR_MotorFoc	tS_pMotorFoc;
	s32 tS_err;
	tS_pMotorFoc	= &gS_FocObjM[t_bObjId];
	if(tS_pMotorFoc != NULL)
	{
		tS_err		= tS_pMotorFoc->m_pMotorAngle->wElectAngleEst - tS_pMotorFoc->m_pMotorAngle->wElectAngleOpen;
		return tS_err;
	}
	else
	{
		return 0;		
	}	
}


/*****************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
///////////////////init funcs glb///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************/	 



/*****************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
///////////////////init funcs loc///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************/	 

/*******************************************************************************
 �������ƣ�    void setCurrentPIParameter(u8 t_ObjId, u16 Kp_D,u16 Ki_d, u16 Kp_Q,u16 Ki_Q)
 ����������    FOC��·�����ݳ�ʼ��
 ���������    �����ƶ��� t_ObjId
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��ʼ��������PI��������Ҫִ����InitFocVar����ֵ֮��

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/10/19     V1.1          Tonghua Li           �޸Ĳ��ֹ��� memset
 *******************************************************************************/
void setCurrentPIParameter(u8 t_ObjId, u16 Kp_D,u16 Ki_D, u16 Kp_Q,u16 Ki_Q)
{
	PSTR_MotorFoc tS_pFocObj	= &gS_FocObjM[t_ObjId];

	tS_pFocObj->m_pAcrD->KP		= Kp_D;  //d�������Kp��ʼ��
	tS_pFocObj->m_pAcrD->KI		= Ki_D;  //d�������Ki��ʼ��
	
	tS_pFocObj->m_pAcrQ->KP		= Kp_Q;  //q�������Kp��ʼ��
	tS_pFocObj->m_pAcrQ->KI		= Ki_Q;  //q�������Ki��ʼ��
}


/*******************************************************************************
 �������ƣ�    InitFocVar
 ����������    FOC���ݳ�ʼ��
 ���������    �����ƶ���ָ��tS_pMotorFoc
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��ʼ��PWM�Ƚ�������ΪĬ��ֵ

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0          WenCai Zhao          ����
 2023/10/19     V1.1          Tonghua Li           �޸Ĳ��ֹ��� memset
 *******************************************************************************/
void InitFocVar(PSTR_MotorFoc tS_pFocObj)
{
	tS_pFocObj->eMotorStatusAck			= E_MOTOR_RESET;		//��ʼ��״̬��
	
	STR_PIRegulator*	tS_pAcrD		= tS_pFocObj->m_pAcrD;  //d�������ָ�봫��
	STR_PIRegulator*	tS_pAcrQ		= tS_pFocObj->m_pAcrQ;  //q�������ָ�봫��



	memset(tS_pAcrD,0,sizeof(STR_PIRegulator));           //���d��PI������������
	// if necessary,	manual change currentloop PI parameter 
	tS_pAcrD->KP						= tS_pFocObj->m_pFluxObsGain->nD_CurrLoopKP; //  �����¸�ֵ������PI
	tS_pAcrD->KI						= tS_pFocObj->m_pFluxObsGain->nD_CurrLoopKI; //	
	tS_pAcrD->wLowerLimitOutput			=(s32)tS_pFocObj->m_pFocObsPar->nVdMin<<16;		//���������޷�
	tS_pAcrD->wUpperLimitOutput			=(s32)tS_pFocObj->m_pFocObsPar->nVdMax<<16;		//���������޷�
	
	
	memset(tS_pAcrQ,0,sizeof(STR_PIRegulator));           //���q��PI������������
	// if necessary,	manual change currentloop PI parameter 
	tS_pAcrQ->KP						= tS_pFocObj->m_pFluxObsGain->nQ_CurrLoopKP; //	  �����¸�ֵ������PI
	tS_pAcrQ->KI						= tS_pFocObj->m_pFluxObsGain->nQ_CurrLoopKI; //
	tS_pAcrQ->wLowerLimitOutput			=(s32)tS_pFocObj->m_pFocObsPar->nVqMin<<16;		//���������޷�
	tS_pAcrQ->wUpperLimitOutput			=(s32)tS_pFocObj->m_pFocObsPar->nVqMax<<16;		//���������޷�
	


	tS_pFocObj->m_pVolDQ->nAxisD				= 0;				//d���ѹ��ʼ��
	tS_pFocObj->m_pVolDQ->nAxisQ				= 0;				//q���ѹ��ʼ��

	memset(tS_pFocObj->m_pMotorAngle,0,sizeof(STR_MotorFocAngle));	//��յ���Ƕȱ���


	tS_pFocObj->mSinCosFocAngle.hSin			= 0;				//�Ƕ�sinֵ��ʼ��
	tS_pFocObj->mSinCosFocAngle.hCos			= 32767;			//�Ƕ�cosֵ��ʼ��

	//tS_pFocObj->m_pStatCurrAB->nAlph			= 0;				//Alph�������ʼ��
	//tS_pFocObj->m_pStatCurrAB->nBeta			= 0;				//Beta�������ʼ��

	tS_pFocObj->m_pStatCurrDQ->nAxisD			= 0;				//d�������ʼ��
	tS_pFocObj->m_pStatCurrDQ->nAxisQ			= 0;				//q�������ʼ��
	
	tS_pFocObj->m_pMotorFocSpeed->wSpeedRef		= 0;				//����ת�ٳ�ʼ��
	tS_pFocObj->m_pMotorFocSpeed->wSpeedEst		= 0;				//�۲�ת�ٳ�ʼ��
	
	tS_pFocObj->m_pStatCurrDQRef->nAxisD		= 0;				//����d�������ʼ��
	tS_pFocObj->m_pStatCurrDQRef->nAxisQ		= 0;				//����q�������ʼ��
	
	tS_pFocObj->m_pVectVolAB->nAlph				= 0;				//Alph���ѹ��ʼ��
	tS_pFocObj->m_pVectVolAB->nBeta				= 0;				//Beta���ѹ��ʼ��

	tS_pFocObj->m_bIPDFinsh						= 0;				//IPD��־λ��ʼ��

	InitCmpr(tS_pFocObj);                                  //�Ƚ������ݳ�ʼ��
}


/*****************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
///////////////////init funcs loc///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************/	 




	 
/*****************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
///////////////////init funcs..end///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************/	 	 

/*******************************************************************************
 �������ƣ�    void SVPWM_GetPhaseCurrent(PSTR_CurSamplingRes this)
 ����������    ������������
 ���������    PSTR_CurSamplingRes this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   �Ż�Ϊͨ������ָ�����ʵ�֣���С����ʱ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/03/23    V1.0           Tonghua Li          ����
 *******************************************************************************/
void SVPWM_GetPhaseCurrent(PSTR_CurSamplingRes this)
{
    this->mCurSamplingEvent.pAdcSamp(this);
}

/*******************************************************************************
 �������ƣ�    void SVPWM_1ShuntGetPhaseCurrentxx(PSTR_CurSamplingRes this)
 ����������    ����������㷨����SVPWM�����������õ�U, V�����
 ���������    PSTR_CurSamplingRes this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����	   �Ż�Ϊͨ������ָ�����ʵ�֣���С����ʱ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/03/23    V1.0           Tonghua Li          ����
 *******************************************************************************/
//sector 1 ic---Nia
void SVPWM_1ShuntGetPhaseIcNIa(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatW = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatU =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatV = -(this->pAdcRes->nSampDatW + this->pAdcRes->nSampDatU);
}

void SVPWM_1ShuntGetPhaseOldIcNIa(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatU =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatV = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseIcNOldIa(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatW = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatV = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseOldIcNOldIa(PSTR_CurSamplingRes this)
{
	return;
}

//sector 2 Ib NIa
void SVPWM_1ShuntGetPhaseIbNIa(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatV = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatU =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}

void SVPWM_1ShuntGetPhaseOldIbNIa(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatU =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}

void SVPWM_1ShuntGetPhaseIbNOldIa(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatV = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}

void SVPWM_1ShuntGetPhaseOldIbNOldIa(PSTR_CurSamplingRes this)
{
	return;
}

//sector 3 Ib NIc
void SVPWM_1ShuntGetPhaseIbNIc(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatV = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatW =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatU = -(this->pAdcRes->nSampDatV + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseOldIbNIc(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatW =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatU = -(this->pAdcRes->nSampDatV + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseIbNOldIc(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatV = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatU = -(this->pAdcRes->nSampDatV + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseOldIbNOldIc(PSTR_CurSamplingRes this)
{
	return;
}

//sector 4 Ic NIb
void SVPWM_1ShuntGetPhaseIcNIb(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatW = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatV =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatU = -(this->pAdcRes->nSampDatV + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseOldIcNIb(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatV =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatU = -(this->pAdcRes->nSampDatV + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseIcNOldIb(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatW = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatU = -(this->pAdcRes->nSampDatV + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseOldIcNOldIb(PSTR_CurSamplingRes this)
{
	return;
}

//sector 5 Ia NIb
void SVPWM_1ShuntGetPhaseIaNIb(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatU = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatV =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}

void SVPWM_1ShuntGetPhaseOldIaNIb(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatV =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}

void SVPWM_1ShuntGetPhaseIaNOldIb(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatU = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}

void SVPWM_1ShuntGetPhaseOldIaNOldIb(PSTR_CurSamplingRes this)
{
	return;
}

//sector 6 Ia NIc
void SVPWM_1ShuntGetPhaseIaNIc(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatU = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatW =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatV = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseOldIaNIc(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatW =(s32)(this->pAdcRes->nBusShuntOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatV = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseIaNOldIc(PSTR_CurSamplingRes this)
{
	this->pAdcRes->nSampDatU = this->pAdcRes->nSampCurDat0 - (s32)(this->pAdcRes->nBusShuntOffset);
	this->pAdcRes->nSampDatV = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatW);
}

void SVPWM_1ShuntGetPhaseOldIaNOldIc(PSTR_CurSamplingRes this)
{
	return;
}

/*******************************************************************************
 �������ƣ�    void SVPWM_2ShuntGetPhaseCurrent(PSTR_CurSamplingRes this)
 ����������    ˫��������㷨����SVPWM�����������õ�U, V�����
 ���������    PSTR_CurSamplingRes this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �Ż�Ϊͨ������ָ�����ʵ�֣���С����ʱ��
 �޸�����	   �汾��		   �޸���			 �޸�����
 -----------------------------------------------------------------------------
 2023/03/23    V1.0 		  Tonghua Li		  ����
 *******************************************************************************/
void SVPWM_2ShuntGetPhaseCurrent(PSTR_CurSamplingRes this)
{
    this->mCurSamplingEvent.pAdcSamp(this);
}

void SVPWM_2ShuntGetPhaseIaIb(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatU = (s32)(this->pAdcRes->nPhaseUOffset) - this->pAdcRes->nSampCurDat0;
	this->pAdcRes->nSampDatV = (s32)(this->pAdcRes->nPhaseVOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}


/*******************************************************************************
 �������ƣ�    void SVPWM_3ShuntGetPhasexx(PSTR_CurSamplingRes this)
 ����������    ����������㷨����SVPWM�����������õ�U, V�����
 ���������    PSTR_CurSamplingRes this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �Ż�Ϊͨ������ָ�����ʵ�֣���С����ʱ��
 �޸�����	   �汾��		   �޸���			 �޸�����
 -----------------------------------------------------------------------------
 2023/03/23    V1.0 		  Tonghua Li		  ����
 *******************************************************************************/
void SVPWM_3ShuntGetPhaseIaIb(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatU = (s32)(this->pAdcRes->nPhaseUOffset) - this->pAdcRes->nSampCurDat0;
	this->pAdcRes->nSampDatV = (s32)(this->pAdcRes->nPhaseVOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatW = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatV);
}

void SVPWM_3ShuntGetPhaseOldIaOldIb(PSTR_CurSamplingRes this)
{	
	return;
}


void SVPWM_3ShuntGetPhaseIaIc(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatU = (s32)(this->pAdcRes->nPhaseUOffset) - this->pAdcRes->nSampCurDat0;
	this->pAdcRes->nSampDatW = (s32)(this->pAdcRes->nPhaseWOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatV = -(this->pAdcRes->nSampDatU + this->pAdcRes->nSampDatW);
}

void SVPWM_3ShuntGetPhaseOldIaOldIc(PSTR_CurSamplingRes this)
{	
	return;
}

void SVPWM_3ShuntGetPhaseIbIc(PSTR_CurSamplingRes this)
{	
	this->pAdcRes->nSampDatV = (s32)(this->pAdcRes->nPhaseVOffset) - this->pAdcRes->nSampCurDat0;
	this->pAdcRes->nSampDatW = (s32)(this->pAdcRes->nPhaseWOffset) - this->pAdcRes->nSampCurDat1;
	this->pAdcRes->nSampDatU = -(this->pAdcRes->nSampDatV + this->pAdcRes->nSampDatW);
}

void SVPWM_3ShuntGetPhaseOldIbOldIc(PSTR_CurSamplingRes this)
{	
	return;
}

//////////////////////////////////
///////////////////////////////////////////////////

/*******************************************************************************
 �������ƣ�    void AdcSampleCal(stru_FOC_CtrProcDef *this)
 ����������    ��������
 ���������    stru_FOC_CtrProcDef *this �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    1.�����������ɼ�AB�������
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0            Wencai.Zhao Howlet Li          ����
 *******************************************************************************/
#if 0
void AdcCurSampleCalc(PSTR_MotorFoc tS_pFocObj)
{
    PSTR_CurSamplingRes this;
    this = &tS_pFocObj->mCurSamplingRes;  //ָ��

	tS_pFocObj->m_pHandle->ReConstructCurrent(&tS_pFocObj->mCurSamplingRes);
    /* �������ۻ����� */
	tS_pFocObj->m_pStatCurrUVW->nPhaseU = _IQ15mpy(this->pAdcRes->nSampDatU, tS_pFocObj->m_pFluxObsGain->CurrentPerUnitGAIN);
	tS_pFocObj->m_pStatCurrUVW->nPhaseV = _IQ15mpy(this->pAdcRes->nSampDatV, tS_pFocObj->m_pFluxObsGain->CurrentPerUnitGAIN);
	tS_pFocObj->m_pStatCurrUVW->nPhaseW = -(tS_pFocObj->m_pStatCurrUVW->nPhaseU + tS_pFocObj->m_pStatCurrUVW->nPhaseV);

    /* clark�任 */
	tS_pFocObj->m_pStatCurrAB->nBeta = _IQ15mpy(18919, (tS_pFocObj->m_pStatCurrUVW->nPhaseV - tS_pFocObj->m_pStatCurrUVW->nPhaseW));
	tS_pFocObj->m_pStatCurrAB->nAlph = tS_pFocObj->m_pStatCurrUVW->nPhaseU;
}
#endif

 /*******************************************************************************
 �������ƣ�    s16 CurrentPIRegulator(stru_PIRegulator *Regulator)
 ����������    ������PI  ����ʽPI
 ���������    stru_PIRegulator *Regulator �ṹ��ָ��
 ���������    PI���ڽ��
 �� �� ֵ��    ��
 ����˵����		Ki -Q16 /Kp -Q12
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           WenCai Zhao          ����
 *******************************************************************************/
s16 CurrentPIRegulator(STR_PIRegulator* tS_pAcr)
{
    s32		ACC;
	s32		wTemp;
    s32		AX;
	
    ACC = (tS_pAcr->nError - tS_pAcr->nLastError) * tS_pAcr->KP;	/* ��������� */
    ACC = (ACC << 4) + (s32)(tS_pAcr->nError) * tS_pAcr->KI;		/* ��������� */
    tS_pAcr->wIntegral = ACC + tS_pAcr->wIntegral;
	
	wTemp					= (((s32)tS_pAcr->nComp) << 16) + tS_pAcr->wIntegral;
	
	if(wTemp > tS_pAcr->wUpperLimitOutput)                /* ������ֵ�޷� */
    {        
		wTemp				= tS_pAcr->wUpperLimitOutput;
		tS_pAcr->wIntegral	= tS_pAcr->wUpperLimitOutput - (((s32)tS_pAcr->nComp) << 16);
		
    }
    else if(wTemp < tS_pAcr->wLowerLimitOutput)         /* �����Сֵ�޷� */
    {
        wTemp				= tS_pAcr->wLowerLimitOutput;
		tS_pAcr->wIntegral	= tS_pAcr->wLowerLimitOutput - (((s32)tS_pAcr->nComp) << 16);
    }
	AX = wTemp >> 16;

    tS_pAcr->nLastError = tS_pAcr->nError;				/* ��¼�ϴ����ֵ */

    return(AX);                                                          
}


/*******************************************************************************
 �������ƣ�    RegMotorCurrentDQ(stru_FOC_CurrLoopDef *this)
 ����������    D��Q��������㣬���Vd��Vq
 ���������    stru_FOC_CurrLoopDef *this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/08/05      V1.0           Howlet  Li          ����
 2023/03/23      V1.1           Tonghua Li          �޸�Ϊ��ѡ��ģʽ
 *******************************************************************************/
void RegMotorCurrentDQ(PSTR_MotorFoc tS_pFocObj)
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

	//@call:
	CalcVolCircle(tS_pFocObj->m_bFOCObj_ID,tS_pFocObj->m_pVolDQ,tS_pFocObj->m_pVectVolAB);
}

/*******************************************************************************
 �������ƣ�    void SetCmprDuty(PSTR_McpwmCmpr tS_Cmpr,u16 nuTime)
 ����������    ����PWM�Ƚ�װ����ֵ
 ���������    PSTR_McpwmCmpr tS_Cmpr  �ṹ��ָ��
 			   u16 nuTime
 			   
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/10/15      V1.0          			          ����
 *******************************************************************************/
void SetCmprDuty(PSTR_McpwmCmpr tS_Cmpr,u16 nuTime)
{
	tS_Cmpr->nuTimePhA		= nuTime;
	tS_Cmpr->nuTimePhB		= nuTime;
	tS_Cmpr->nuTimePhC		= nuTime;
	
	tS_Cmpr->nuTimePhAN		= -nuTime;
	tS_Cmpr->nuTimePhBN		= -nuTime;
	tS_Cmpr->nuTimePhCN		= -nuTime;
}

/*******************************************************************************
 �������ƣ�    updateObserPLLVar1msLoop(PSTR_MotorFoc tS_pMotorFoc)
 ����������    PLL Kp Ki����
 ���������    PSTR_MotorFoc tS_pMotorFoc  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/1/20      V1.0           Li Tonghua          ����
 *******************************************************************************/
void updateObserPLLVar1msLoop(PSTR_MotorFoc tS_pMotorFoc)
{
	s16 PLLKp;
	s16 PLLKi;

	s32 FreqStart;
	s32 FreqRef;	
	s32 deltHz;
	s32 deltFreq;
	
	STR_TransCoefElement *PCoreToApp	= &(tS_pMotorFoc->m_pTransCoef->mCoreToApp);  //���ۻ�ϵ��CoreToAppָ�봫��
	PSTR_FocObsPar			 t_pFocObsPar = tS_pMotorFoc->m_pFocObsPar;                 //�۲�������ָ�봫��

	// ���ݸ�ʽ0.1Hz
	deltFreq	= (s32)t_pFocObsPar->nPLLFreq1 - t_pFocObsPar->nPLLFreq0;
	FreqStart	= t_pFocObsPar->nPLLFreq0;
	
	PLLKp		= t_pFocObsPar->nPLLKp1 - t_pFocObsPar->nPLLKp0;
	PLLKi		= t_pFocObsPar->nPLLKi1 - t_pFocObsPar->nPLLKi0;

	// user to app ��ʾΪ0.01Hz      ------> 0.1Hz
	FreqRef			= Core2AppFreqTrans(PCoreToApp,tS_pMotorFoc->m_pMotorFocSpeed->wSpeedRef);
	FreqRef			= (FreqRef * _IQ15(0.1)) >> 15;
	FreqRef			= ABS(FreqRef);
	
	if(FreqRef >= FreqStart)
	{
		deltHz		= (FreqRef - FreqStart);
		deltHz		= deltHz << 15;
	
		deltFreq	= deltHz/deltFreq;
		
		PLLKp		= (deltFreq  * PLLKp) >> 15;		
		PLLKi		= (deltFreq  * PLLKi) >> 15;
	}
	else
	{
		PLLKp		= 0;
		PLLKi		= 0;
	}

	/* �ֶ�ʽPI���㺯�� */
	PLLKp		= t_pFocObsPar->nPLLKp0 + PLLKp;
	PLLKi		= t_pFocObsPar->nPLLKi0 + PLLKi;	

	PLLKp		= PLLKp > t_pFocObsPar->nPLLKp1 ? t_pFocObsPar->nPLLKp1: PLLKp;
	PLLKp		= PLLKp < t_pFocObsPar->nPLLKp0 ? t_pFocObsPar->nPLLKp0: PLLKp;

	PLLKi		= PLLKi > t_pFocObsPar->nPLLKi1 ? t_pFocObsPar->nPLLKi1: PLLKi;
	PLLKi		= PLLKi < t_pFocObsPar->nPLLKi0 ? t_pFocObsPar->nPLLKi0: PLLKi;

	setObserPLLKpKi(tS_pMotorFoc->m_bFOCObj_ID,PLLKp,PLLKi);
}

/*******************************************************************************
 �������ƣ�    bool	getIPD_CW_AngComp(PSTR_MotorFoc tS_pFocObj)
 			   bool	getIPD_CCW_AngComp(PSTR_MotorFoc tS_pFocObj)
 ����������    IPD �����Ƕ� CW/CCW�ж�
 ���������    PSTR_MotorFoc tS_pMotorFoc  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �⺯������
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/12/20      V1.0           Li Tonghua          ����
 *******************************************************************************/
bool	getIPD_CW_AngComp(PSTR_MotorFoc tS_pFocObj)
{
	return (tS_pFocObj->pMaToMd->nuFineCmd == E_FC_IPD_CW_START);    //  CW IPD �����Ƕ��ж�
}

bool	getIPD_CCW_AngComp(PSTR_MotorFoc tS_pFocObj)
{
	return (tS_pFocObj->pMaToMd->nuFineCmd == E_FC_IPD_CCW_START);   // CCW IPD �����Ƕ��ж�
}

/*******************************************************************************
 �������ƣ�    s16 getMotorCurFromObjId(u8 t_bObjId)
 			   s16 getPhaseCur_U(u8 t_bObjId)
 			   s16 getPhaseCur_V(u8 t_bObjId)
 			   s16 getPhaseRawCur_U(u8 t_bObjId)
 			   s16 getPhaseRawCur_V(u8 t_bObjId)
 			   
 ����������    ��ȡ������װ����
 ���������    u8 t_bObjId      ���ƶ���ID
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �⺯������
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/12/10      V1.0           Li Tonghua          ����
 *******************************************************************************/
s16 getMotorCurFromObjId(u8 t_bObjId)
{
	s16 t_nTmp;
	t_nTmp = Sqrt_Functions(gS_FocObjM[t_bObjId].m_pStatCurrAB->nAlph,gS_FocObjM[t_bObjId].m_pStatCurrAB->nBeta);
	return t_nTmp;
}

s16 getMotorCurFromObjId1ms(u8 t_bObjId)			// return Q15
{	
	s32 t_nTmp;
	t_nTmp	= (s32)gS_FocObjM[t_bObjId].m_pStatCurrAB->nAlph * gS_FocObjM[t_bObjId].m_pStatCurrAB->nAlph
				+ (s32)gS_FocObjM[t_bObjId].m_pStatCurrAB->nBeta * gS_FocObjM[t_bObjId].m_pStatCurrAB->nBeta;

	t_nTmp = IQ15SqrtIQ31(t_nTmp);

	t_nTmp = (t_nTmp * 23170) >> 14;			// 1.414

	if(t_nTmp > 32767)
	{
		t_nTmp = 32767;
	}
	
	return (s16)t_nTmp;
}

s16 getMotorActVoltFromObjId1ms(u8 t_bObjId)		// return Q15
{
	s32 t_nTmp;
	t_nTmp	= (s32)getMotorDaxisVoltFormObjId(t_bObjId) * getMotorDaxisVoltFormObjId(t_bObjId) 
					+ (s32)getMotorQaxisVoltFormObjId(t_bObjId) * getMotorQaxisVoltFormObjId(t_bObjId);

	t_nTmp = IQ15SqrtIQ31(t_nTmp);

	t_nTmp = (t_nTmp * 23170) >> 14;			// 1.414

	if(t_nTmp > 32767)
	{
		t_nTmp = 32767;
	}
	
	return (s16)t_nTmp;
}




s16 getPhaseCur_U(u8 t_bObjId)   //��ȡU�����ֵ
{
	return (s16)(gS_FocObjM[t_bObjId].m_pStatCurrUVW->nPhaseU);
}

s16 getPhaseCur_V(u8 t_bObjId)   //��ȡV�����ֵ
{
	return (s16)(gS_FocObjM[t_bObjId].m_pStatCurrUVW->nPhaseV);
}

s16 getPhaseCur_W(u8 t_bObjId)   //��ȡW�����ֵ
{
	return (s16)(gS_FocObjM[t_bObjId].m_pStatCurrUVW->nPhaseW);
}

s16 getiq_ref(u8 t_bObjId)   //��ȡU�����ֵ
{
	return (s16)(gS_FocObjM[t_bObjId].m_pStatCurrDQRef->nAxisQ);
}
 s16 getid_ref(u8 t_bObjId)   //��ȡU�����ֵ
{
	return (s16)(gS_FocObjM[t_bObjId].m_pStatCurrDQRef->nAxisD);
}


s16 getPWMstatus(u8 t_bObjId)   
{
	return (s16)gS_PhyObjInstanceM[t_bObjId].mGlobalDatPackage.mMdToMa.ePwmStatus;
}

s16 getUd(u8 t_bObjId)   //��ȡUd
{
	return (s16)(gS_FocObjM[t_bObjId].m_pVolDQ->nAxisD);
}

s16 getUq(u8 t_bObjId)   //��ȡUq
{
	return (s16)(gS_FocObjM[t_bObjId].m_pVolDQ->nAxisQ);
}

s16 getVdc(u8 t_bObjId)   //��ȡVdc
{
	return (s16)(gS_PhyObjInstanceM[t_bObjId].mGlobalDatPackage.mMaToUa.wVdcDec);
}

s16 getid(u8 t_bObjId)   //��ȡid
{
	return (s16)(gS_FocObjM[t_bObjId].m_pStatCurrDQ->nAxisD);
}

s16 getiq(u8 t_bObjId)   //��ȡiq
{
	return (s16)(gS_FocObjM[t_bObjId].m_pStatCurrDQ->nAxisQ);
}

s16 getPhaseRawCur_U(u8 t_bObjId) //��ȡU���������ԭʼֵ
{
	return (s16)(gS_FocObjM[t_bObjId].pHdToMd->nSampCurDat0);
}

s16 getPhaseRawCur_V(u8 t_bObjId) //��ȡV���������ԭʼֵ
{
	return (s16)(gS_FocObjM[t_bObjId].pHdToMd->nSampCurDat1);
}
s16 getMotorStatus(u8 t_bObjId) 
{
  return (s16)(gS_FocObjM[t_bObjId].pMaToMd->eMotorStatus);
}
/*******************************************************************************
 �������ƣ�    s16 getEstSpeed(u8 t_bObjId)
 			         s16 getRefSpeed(u8 t_bObjId)
 ����������    ��ȡת�ٷ�װ����
 ���������    u8 t_bObjId      ���ƶ���ID
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �⺯������
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/12/10      V1.0           Li Tonghua          ����
 *******************************************************************************/
s16 getEstSpeed(u8 t_bObjId)    //��ȡ�۲�ת��
{
	return (s16)(gS_FocObjM[t_bObjId].m_pMotorFocSpeed->wSpeedEst);
}

s16 getRefSpeed(u8 t_bObjId)    //��ȡ����ת��
{
	return (s16)(gS_FocObjM[t_bObjId].m_pMotorFocSpeed->wSpeedRef);
}

u16 getFocObjStatus(u8 t_bObjId)
{
	return (s16)(gS_FocObjM[t_bObjId].eMotorStatusAck);
}
/******************** (C) COPYRIGHT 2019, Bright Power Semiconductor Co.ltd*******************/


