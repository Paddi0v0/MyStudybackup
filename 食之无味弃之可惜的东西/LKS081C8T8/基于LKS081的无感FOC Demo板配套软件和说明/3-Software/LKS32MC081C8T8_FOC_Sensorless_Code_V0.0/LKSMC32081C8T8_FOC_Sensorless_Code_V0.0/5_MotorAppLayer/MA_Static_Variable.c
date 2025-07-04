/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� MA_Static_Variable.c
 * �ļ���ʶ��
 * ����ժҪ�� MA�ֲ�ȫ�ֱ��������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��8��19��
 *
 *******************************************************************************/
#include "MA_Static_Variable.h"
#include "time_process.h"
#include "PowerCalculation.h" 
#include "string.h"


const char sVersion[10] = "Ver_1.0";        						/* ����汾 */

STR_TaskScheduler		gS_TaskScheduler;    						/* ������Ƚṹ�� */

STR_MACtrProc			sS_MACtrProc[MA_OBJ_MAX_DEFINE];			/* MA ���ƶ��� */
STR_MASubBlock			sS_MASubBlock[MA_OBJ_MAX_DEFINE];			/* MA ���ƽṹ�� */

extern PSTR_ControlObject getCtrlObjectPtr(u8 objID);				//	��ÿ��ƶ���ָ��
extern void updateObParaMALayer(PSTR_ControlObject pCtrlObj);		//	MA����¹۲�������, MD�����MA�����Ӧ��
extern void updateAcrParaMALayer(PSTR_ControlObject pCtrlObj);		//	MA����µ�����������MD�����MA�����Ӧ��
extern void updateSVPWMParaMALayer(PSTR_ControlObject pCtrlObj);	//	MA�����SVPWM������MD�����MA�����Ӧ��


PSTR_MACtrProc	getMACtrPtr(u8 ObjID)			//	���MA�����ָ��
{
	if(ObjID < MA_OBJ_MAX_DEFINE)				//	����IDС�ڶ���Ķ�����ʱ����ָ�룬���򷵻�NULL��
	{
		return &sS_MACtrProc[ObjID];
	}
	else
	{
		return NULL;
	}
}

PSTR_MACtrProc	directGetMACtrPtr(u8 ObjID)		//	ֱ�ӻ��MA����ָ�룬������ID��Ч���ж�
{
	return &sS_MACtrProc[ObjID];		
}


u8 getMaxMAObjNbr(void)							//	���֧�ֵ����������
{
	return MA_OBJ_MAX_DEFINE;
}

/*******************************************************************************
 �������ƣ�    bool registerSubBlock2MACrtlProc(void)
 ����������    �����ƶ������ע����ģ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ������� true ---- ��ʼ���ɹ�
 ����˵����    ע�����ݽṹָ�롢���ƶ����
 			   ��ʼ��ȫ�����ã����ٶȻ�ģʽ���Ƿ�˳������ 			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
bool registerSubBlock2MACrtlProc(void)
{
	u8 objID = 0;
	bool returnResult = TRUE;
	
	memset(&sS_MACtrProc[objID], 0, sizeof(STR_MACtrProc));
	sS_MACtrProc[objID].mg_nMACtrlProcID		= objID;
	sS_MACtrProc[objID].m_eSysState				= E_DRIVER_INIT;							//	��ʼ״̬

	sS_MACtrProc[objID].m_pAppCommData			= &sS_MASubBlock[objID].m_struAppCommData;	//	�������̿��ƽṹ��ָ��
	sS_MACtrProc[objID].m_pMotorSpeed			= &sS_MASubBlock[objID].m_struMotorSpeed;	//	ת��ʸ���ṹ��ָ���ʼ��
	
	sS_MACtrProc[objID].m_pOnTheFlyDetect		= &sS_MASubBlock[objID].m_OnTheFlyDetect;	//	˳���ṹ��ָ���ʼ��
	
	#if(DRV0_UNDVOLT_PROT_ENABLE == 1)
	sS_MACtrProc[objID].m_pMotorException		= &sS_MASubBlock[objID].m_MotorException;	//	��ѹ�쳣�ṹ��ָ���ʼ��
	#endif
	
	sS_MACtrProc[objID].m_pTime					= &sS_MASubBlock[objID].m_struTime;			//	��ʱʱ��ṹ��ָ���ʼ��

	sS_MACtrProc[objID].m_pSTR_CurRampGen		= &sS_MASubBlock[objID].m_STR_CurRampGen;	//	����ָ��滮�ṹ��ָ���ʼ��

	
	#if ((DRV0_CLOSE_LOOP == POWER_LOOP) || ((DRV0_POWER_LIMIT_STATUS == TRUE) && (DRV0_CLOSE_LOOP == SPEED_LOOP)))		
	sS_MACtrProc[objID].m_pPower				= &sS_MASubBlock[objID].m_struPower;		//	���ʽṹ��ָ���ʼ��
	#endif

	sS_MACtrProc[objID].m_pFaults				= &sS_MASubBlock[objID].m_stru_Faults;				//	������־�ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pFaultValue			= &sS_MASubBlock[objID].m_stru_FaultValue;			//	���������ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruCurrentAmplitude	= &sS_MASubBlock[objID].m_struCurrentAmplitude;		//	������ֵ�����ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruImaxCurrent		= &sS_MASubBlock[objID].m_struImaxCurrent;			//	�������ֵ�����ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruFaultVoltTime	= &sS_MASubBlock[objID].m_struFaultVoltTime;		//	��ѹ���ʱ��ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruFaultEmptyTime	= &sS_MASubBlock[objID].m_struFaultEmptyTime;		//	��ת���ʱ��ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruFaultStallTime	= &sS_MASubBlock[objID].m_struFaultStallTime;		//	��ת���ʱ��ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruFaultCurrentTime	= &sS_MASubBlock[objID].m_struFaultCurrentTime;		//	�������ʱ��ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruFaultTempTime	= &sS_MASubBlock[objID].m_struFaultTempTime;		//	�¶ȼ��ʱ��ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruFaultPhaseTime	= &sS_MASubBlock[objID].m_struFaultPhaseTime;		//	ȱ����ʱ��ṹ��ָ���ʼ��
	sS_MACtrProc[objID].m_pstruFaultStartTime	= &sS_MASubBlock[objID].m_struFaultStartTime;		//	�������ʱ��ṹ��ָ���ʼ��

	
	sS_MACtrProc[objID].mg_pCtrlObj				= getCtrlObjectPtr(objID);					// ע��ʵ�ʵĿ��ƶ��󣬸���ID����
	if(sS_MACtrProc[objID].mg_pCtrlObj == NULL)
	{
		returnResult = FALSE;
	}
	return returnResult;
}
/*******************************************************************************
 �������ƣ�    void initMACtrProc (u8 ObjID)
 ����������    �����ƶ���ID��ʼ��
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    ��ʼ��ȫ�ֱ���			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void initMACtrProc (u8 ObjID)
{
	PSTR_DrvCfgPara pDrvCfgPara;
	pDrvCfgPara		= sS_MACtrProc[ObjID].mg_pCtrlObj->m_pDrvCfgPara;
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bAcrInitDone			= 0;		//	��������ʼ����ɱ�־	0 ----- δ���
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bAcrParaReq			= 1;		//	��������ʼ�������־ 	1 ----- �г�ʼ������
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bAcrParaUpdate		= 1;		//	������������Ҫ���±�־	1 ----- ������Ҫ����

	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bSVPWMInitDone		= 0;		//	SVPWM��ʼ����ɱ�־		0 ----- δ���
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bSVPWMParaReq		= 1;		//	SVPWM��ʼ�������־ 	1 ----- �г�ʼ������
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bSVPWMParaUpdate		= 1;		//	SVPWM������Ҫ���±�־	1 ----- ������Ҫ����

	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bObParaInitDone		= 0;		//	�۲�����ʼ����ɱ�־	0 ----- δ���
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bObParaReq			= 1;		//	�۲�����ʼ�������־ 	1 ----- �г�ʼ������
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bObParaUpdate		= 1;		//	�۲���������Ҫ���±�־	1 ----- ������Ҫ����	

	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bFaultInitDone		= 0;		//	����ģ���ʼ����ɱ�־	0 ----- δ���
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bFaultInitReq		= 1;		//	����ģ���ʼ�������־ 	1 ----- �г�ʼ������

	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bIPDFinish			= 0;		//	IPDģ���ʼ����ɱ�־	0 ----- δ���
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bIPDInitReq			= 0;		//	IPDģ���ʼ�������־ 	1 ----- �г�ʼ������
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bIPDInitDone			= 0;		//	IPDģ���ʼ����ɱ�־	0 ----- δ���

	// update default configure data MA2MD
	updateObParaMALayer(sS_MACtrProc[ObjID].mg_pCtrlObj);				//	MA���¹۲���������MDӦ��
	updateAcrParaMALayer(sS_MACtrProc[ObjID].mg_pCtrlObj);				//	MA���µ�����������MDӦ��
	updateSVPWMParaMALayer(sS_MACtrProc[ObjID].mg_pCtrlObj);			//	MA����SVPWM������MDӦ��

	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bNeedTracking	= pDrvCfgPara->mS_GlobalCfg.m_buDirCheckEnable || pDrvCfgPara->mS_GlobalCfg.m_buBemfDirEnable;
																	//	�������ó�ʼ���Ƿ���Ҫ˳��繦��
	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bNeedIPD			= pDrvCfgPara->mS_GlobalCfg.m_buIPDEnable;
																	//	�������ó�ʼ���Ƿ���ҪIPD����

	sS_MACtrProc[ObjID].m_SMACtrlBit.m_bNeedAlign		= pDrvCfgPara->mS_GlobalCfg.m_buAllignEnable;
																	//	�������ó�ʼ���Ƿ���Ҫ����ALLIGN����

	sS_MACtrProc[ObjID].m_nLoopMode						= pDrvCfgPara->mS_GlobalCfg.m_buCtrLoopmode;
																	//	�������ó�ʼ��ѡ�ÿ���ģʽ: 0 -- �ٶȻ�  1 -- ������  2 -- ���ʻ�

	if(sS_MACtrProc[ObjID].m_nLoopMode == SPEED_LOOP)				//	�ٶȻ�ģʽʱ���������ó�ʼ���Ƿ���Ҫ��������
	{
		sS_MACtrProc[ObjID].m_pAppCommData->bLoopLmtEnable	= pDrvCfgPara->mS_GlobalCfg.m_buPowerLmtInSpeedmode;
	}

	if(sS_MACtrProc[ObjID].m_nLoopMode == POWER_LOOP)				//	���ʻ�ģʽʱ���������ó�ʼ���Ƿ���Ҫ�ٶ�����
	{
		sS_MACtrProc[ObjID].m_pAppCommData->bLoopLmtEnable	= pDrvCfgPara->mS_GlobalCfg.m_buSpdLmtInPowermode;
	}
}

/*******************************************************************************
 �������ƣ�    s16 getMAEstSpeed(u8 t_bObjId)
 ����������    ����ID��ȡMA��õ���MD�۲��ٶ�
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    ���ع����ٶ� ---- core���ݸ�ʽ			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
s16 getMAEstSpeed(u8 t_bObjId)
{
	return (s16)(sS_MASubBlock[t_bObjId].m_struMotorSpeed.wSpeedEst);
}

/*******************************************************************************
 �������ƣ�    s16 getMARefSpeed(u8 t_bObjId)
 ����������    ����ID��ȡMA���ٶ��趨ָ��
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    �����趨�ٶ� ---- core���ݸ�ʽ			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
s16 getMARefSpeed(u8 t_bObjId)
{
	return (s16)(sS_MASubBlock[t_bObjId].m_struMotorSpeed.wSpeedRef);
}

/*******************************************************************************
 �������ƣ�    s16 getMAFdbkSpeed(u8 t_bObjId)
 ����������    ����ID��ȡMA��õ��Ĺ۲���˲��ٶ�,�˲�ʱ�������
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    �����趨�ٶ� ---- core���ݸ�ʽ			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
s16 getMAFdbkSpeed(u8 t_bObjId)
{
	return (s16)(sS_MASubBlock[t_bObjId].m_struMotorSpeed.wSpeedfbk);
}

/*******************************************************************************
 �������ƣ�    s16 getMAIqref(u8 t_bObjId)
 ����������    ����ID��ȡMA��������˲���Q�����ָ��ֵ
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    �����˲���ָ��ֵ ---- core���ݸ�ʽ			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
s16 getMAIqref(u8 t_bObjId)
{
	return (s16)(sS_MACtrProc[t_bObjId].m_nQCur_Reference);
}
/*******************************************************************************
 �������ƣ�    s16 getMAIdref(u8 t_bObjId)
 ����������    ����ID��ȡMA��������˲���D�����ָ��ֵ
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    �����˲���ָ��ֵ ---- core���ݸ�ʽ			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
s16 getMAIdref(u8 t_bObjId)
{
	return (s16)(sS_MACtrProc[t_bObjId].m_nDCur_Reference);
}

/*******************************************************************************
 �������ƣ�    s16 getMAIqSet(u8 t_bObjId)
 ����������    ����ID��ȡMA�������Q�����ָ���趨ֵ
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    ����Q��ָ��ֵ ---- core���ݸ�ʽ			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
s16 getMAIqSet(u8 t_bObjId)
{
	return (s16)(sS_MACtrProc[t_bObjId].m_nQCurrentSet);
}
/*******************************************************************************
 �������ƣ�    s16 getMAIdref(u8 t_bObjId)
 ����������    ����ID��ȡMA�������D�����ָ���趨ֵ
 ���������    u8 ObjID
 ���������    ��
 �� �� ֵ��    
 ����˵����    ����D��ָ��ֵ ---- core���ݸ�ʽ			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
s16 getMAIdSet(u8 t_bObjId)
{
	return (s16)(sS_MACtrProc[t_bObjId].m_nDCurrentSet);
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
