/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� MA_interface.c
 * �ļ���ʶ��
 * ����ժҪ�� MA����ӿڵ��ü����ʺ���
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2020��8��16��
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "basic.h"
#include "MA_Static_Variable.h"
#include "math.h"
#include "Time_Process.h"
#include "state_machine.h"



//#define _POLLING_MODE

/*******************************************************************************/
//�ⲿ����������
extern bool				registerSubBlock2MACrtlProc(void);
extern u8				getMaxMAObjNbr(void);
extern PSTR_MACtrProc	getMACtrPtr(u8 ObjID);
extern void				FaultCheck(STR_MACtrProc *pObj);
extern void				handleMAStateMachine(STR_MACtrProc *pObj);
extern void				initMACtrProc (u8 ObjID);
extern PSTR_MACtrProc	directGetMACtrPtr(u8 ObjID);

/*******************************************************************************/
//�ֲ���̬ȫ�ֱ���������
#ifdef _POLLING_MODE
static u8 t_nuObjID = 0;
#endif

extern STR_MACtrProc			sS_MACtrProc[MA_OBJ_MAX_DEFINE];

/*******************************************************************************
 �������ƣ�    void handleMACtrlObj(u8 nuObjID)
 ����������    �����ƶ���ID����MA������
 ���������    �����ƶ���ID
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ú�����������ã���MD/UA���ݽ����ڸú��������
 			   ͨ��������������ʵ��

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void handleMACtrlObj(u8 nuObjID)
{
	PSTR_MACtrProc pObj = &sS_MACtrProc[0];
	SetTime_CountDown(pObj);			/* ������������ʱ����	*/
	FaultCheck(pObj);					/* ���ϼ�⼰��������	*/
	handleMAStateMachine(pObj); 		/* ���״̬������		*/
}


/*******************************************************************************
 �������ƣ�    MACtrlObjTask(void)
 ����������    �����ƶ������MA������
 ���������    ���ƶ���ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ú�����������������ã���MD/UA���ݽ����ڸú��������
 			   ͨ��������������ʵ��
 			   �ú���Ϊ����ֱ�ӵ��ú���
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void MACtrlObjTask(void)
{
	handleMACtrlObj(0);					//	MA������
}

/*******************************************************************************
 �������ƣ�    initMACtrlObj()
 ����������    �����ƶ������MA������
 ���������    ���ƶ���ָ��
 ���������    ��
 �� �� ֵ��    ������� true ---- ��ʼ���ɹ�
 ����˵����    ע�����ݽṹָ�롢���ƶ����
 			   ��ʼ��ȫ�����ã����ٶȻ�ģʽ���Ƿ�˳������
 			   
 			   �ú���Ϊ���Զ���ֱ�ӵ��ú�����ע���ʼ��˳��
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/

bool initMACtrlObj(void)
{
	bool result;	
	result = registerSubBlock2MACrtlProc();		//	ע���ڲ�������ָ�롢��ʼ���ڲ�����
	if(result)
	{
		initMACtrProc(0);						//	���ó�ʼ��
	}
	return result;
}

/*******************************************************************************
 �������ƣ�    void InitMotorAppLayer(void)
 ����������    MA���ʼ������
 ���������    void
 ���������    ��
 �� �� ֵ��    
 ����˵����    ע�����ݽṹָ�롢���ƶ����
 			   ��ʼ��ȫ�����ã����ٶȻ�ģʽ���Ƿ�˳������
 			   
 			   �ú���Ϊ����ֱ�ӵ��ú���
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void InitMotorAppLayer(void)
{
	initMACtrlObj();
}

/*******************************************************************************
 �������ƣ�    void setMADQCurref(s16 dRef, s16 qRef,u8 t_bObjId)
 ����������    ����MA��DQ����ָ��ֵ����
 ���������    s16 dRef, s16 qRef,u8 t_bObjId
 ���������    ��
 �� �� ֵ��    
 ����˵����    ����DQ����ָ��ο�ֵ
 				�б�Ҫʱ����ָ��ֵ�Լ������˲��м�ֵ
 			  
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void setMADQCurref(s16 dRef, s16 qRef,u8 t_bObjId)
{
	PSTR_MACtrProc pObj;

	pObj	= getMACtrPtr(t_bObjId);

	pObj->m_nDCur_Reference	= dRef;
	pObj->m_nQCur_Reference	= qRef;
	#if (DRV0_CLOSE_LOOP == SPEED_LOOP)
	pObj->m_pMotorSpeed->mSpeedPI.wIntegral	= (s64)qRef << 22;
	#endif

	pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToMd.mStatCurDQCmd.nAxisD = pObj->m_nDCur_Reference;
	pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToMd.mStatCurDQCmd.nAxisQ = pObj->m_nQCur_Reference;
	
}

/*******************************************************************************
 �������ƣ�    void setMADQCurrefInit(s16 dRef, s16 qRef,u8 t_bObjId)
 ����������    ����MA��DQ����ָ��ֵ����
 ���������    s16 dRef, s16 qRef,u8 t_bObjId
 ���������    ��
 �� �� ֵ��    
 ����˵����    ����DQ����ָ��ο�ֵ
 				�б�Ҫʱ����ָ��ֵ�Լ������˲��м�ֵ
 			  
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void setMADQCurrefInit(s16 dRef, s16 qRef,u8 t_bObjId)
{
	PSTR_MACtrProc pObj;
	
	pObj	= getMACtrPtr(t_bObjId);
		
	pObj->m_nQCur_Reference		= dRef;
	pObj->m_nDCur_Reference		= pObj->m_nDCurrentSet;

	pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToMd.mStatCurDQCmd.nAxisD = pObj->m_nDCur_Reference;
	pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToMd.mStatCurDQCmd.nAxisQ = pObj->m_nQCur_Reference;
}

/*******************************************************************************
 �������ƣ�    void setMAEst2Ref(u8 t_bObjId)
 ����������    ����MA���ٶȷ���ֵ
 ���������    u8 t_bObjId
 ���������    ��
 �� �� ֵ��    
 ����˵����    ����wSpeedRef
 				
 			  
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void setMAEst2Ref(u8 t_bObjId)
{
	PSTR_MACtrProc pObj;
	pObj	= getMACtrPtr(t_bObjId);
	pObj->m_pMotorSpeed->wSpeedRef	= pObj->m_pMotorSpeed->wSpeedEst;	
}

u16 getMAStatus(u8 t_bObjId)
{
	PSTR_MACtrProc pObj;
	pObj	= getMACtrPtr(t_bObjId);
	return (u16)pObj->m_eSysState;
}

u16 getMAMotorInfoStatus(u8 t_bObjId)
{
	PSTR_MACtrProc pObj;
	pObj	= getMACtrPtr(t_bObjId);
	return (u16)pObj->m_eMotorInfoState;
}

s32 minSpd2CoreData[MA_OBJ_MAX_DEFINE];
s32 minVolt2CoreData[MA_OBJ_MAX_DEFINE];


void MinMotorVoltSpd2Core(u8 tObjId)
{
	STR_MACtrProc *pObj						= getMACtrPtr(tObjId);
	PSTR_DrvCfgPara			t_pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_TransCoefElement 	*pAppToCore = &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);

	minSpd2CoreData[tObjId]	= App2CoreFreqTrans(pAppToCore,t_pDrvCfgPara->mS_FBSlvcCfg0.m_wMinRunFreq);	
	if(tObjId == 0)
	{
		minVolt2CoreData[tObjId]= App2CoreVoltTrans(pAppToCore,SLVC_MIN_VOLTAGE_M0);	
	}
	else
	{
		minVolt2CoreData[tObjId]= App2CoreVoltTrans(pAppToCore,SLVC_MIN_VOLTAGE_M0);
	}
}


s16 getMinQVoltSetting(u8 tObj)
{
	return minVolt2CoreData[tObj];
}

s16 getMinQSpdSetting(u8 tObj)
{
	return minSpd2CoreData[tObj];
}


s16 getMA_SpeedRef(u8 t_bObjId)    //��ȡMA����ת��
{
	return (s16)(sS_MACtrProc[t_bObjId].m_pMotorSpeed->wSpeedRef);
}

s16 getMA_Speedfbk(u8 t_bObjId)    //��ȡMA����ת��
{	
	return (s16)(sS_MACtrProc[t_bObjId].m_pMotorSpeed->wSpeedfbk);
}
s16 getMA_Speedest(u8 t_bObjId)    //��ȡMA����ת��
{	
	return (s16)(sS_MACtrProc[t_bObjId].m_pMotorSpeed->wSpeedEst);
}

s16 getMA_iqset(u8 t_bObjId)    //��ȡMA iqset
{	
	return (s16)(sS_MACtrProc[t_bObjId].m_nQCurrentSet);
}
s16 getMA_idset(u8 t_bObjId)    //��ȡMA idset
{	
	return (s16)(sS_MACtrProc[t_bObjId].m_nDCurrentSet);
}
s16 getMA_iqRef(u8 t_bObjId)    //��ȡMA iqRef
{	
	return (s16)(sS_MACtrProc[t_bObjId].m_nQCur_Reference);
}
s16 getMA_idRef(u8 t_bObjId)    //��ȡMA idRef
{	
	return (s16)(sS_MACtrProc[t_bObjId].m_nDCur_Reference);
}



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



