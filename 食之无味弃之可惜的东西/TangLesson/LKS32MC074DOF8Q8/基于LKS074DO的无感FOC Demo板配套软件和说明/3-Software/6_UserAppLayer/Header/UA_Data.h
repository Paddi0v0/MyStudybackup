/*******************************************************************************
 * ��Ȩ���� (C)2022, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� UA_Data.h
 * �ļ���ʶ��
 * ����ժҪ�� UA_Data
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��8��25��
 *******************************************************************************/
 
/*------------------------------prevent recursive inclusion -------------------*/ 
#ifndef __UA_DATA_H__
#define __UA_DATA_H__

#include "basic.h"
#include "PubData_MA_UA.h"
#include "MC_Math.h"
#include "MC_type.h"
#include "Time_Process.h"
#include "PubData_DriverInstance.h" 
#include "string.h"


#define UA_DEBUG


#define UA_OBJ_MAX_DEFINE	(PLANTFORM_DRV_MODULE_NUM)
#define UA_CFG_DONT_USED	(TRUE)

typedef enum
{
    E_UA_INIT			= 0,	/* UA״̬����ʼ״̬ */ 
    E_UA_STOP			= 1,	/* UA״̬��ֹͣ״̬ */
    E_UA_RUN			= 2,	/* UA״̬������״̬ */
    E_UA_FAULT			= 3,	/* UA״̬������״̬ */    
    E_UA_ILLG			= 4		/* UA״̬���Ƿ�״̬	*/     
} ENUM_UACtrlState;


typedef struct
{
	u8 m_bRunFlag		:1;		/* UA ���������־*/
	u8 m_bDirRevse		:1;		/* ����ȡ�������־*/
	u8 m_bFault			:1;		/* UA ���ϱ�־*/
	u8 m_bFaultClr		:1;		/* UA �����������*/
	u8 m_bMoeEnable		:1;		/* MOE ʹ�ܱ�־*/
	u8 m_bDownLoadStart	:1;		/* ��ʼ����*/

	u8 m_bResvd			:2;
}STR_UA_CtrCMD,*pSTR_UA_CtrCMD;


typedef struct
{
	// �󶨵Ŀ��ƶ���
	u8						mg_nUACtrlProcID;	/*��ǰUA���ƶ��� ID*/
	PSTR_ControlObject		mg_pCtrlObj;		/*���ƶ���*/

	// �ڲ�������Ϣ�洢��ˢ��
	STR_PubDatUAMA 			mUaToMa;
	STR_PubDatMAUA			mMaToUa;

	ENUM_UACtrlState		m_eUASysState;		/* ��ǰUA����״̬�� */
	STR_UA_CtrCMD			m_StruCmd;

	u8						m_bStopMode;		/*���ֹͣģʽ*/
	u8						m_bPWMStopMode;
	u8						m_bLoopMode;		/*����ģʽ*/

	s32						m_wSetpoint;		// ��ֵΪ���� 0.01Hz
	s32						m_wTqSetpoint;		// ��ֵΪ���� 0.001A
	s32						m_wPowerSetpoint;	// 0.01W

	u8						m_bFaultAutoClr;
	u16						m_nuFaultClrCnt;	/*�����������*/
	u16						m_nuFaultClrPeriod;	/*�����������*/
}STR_UACtrProc, *pSTR_UACtrProc;

#endif  /* __UA_DATA_H__*/

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* -----------------------------------END OF FILE------------------------------- */

