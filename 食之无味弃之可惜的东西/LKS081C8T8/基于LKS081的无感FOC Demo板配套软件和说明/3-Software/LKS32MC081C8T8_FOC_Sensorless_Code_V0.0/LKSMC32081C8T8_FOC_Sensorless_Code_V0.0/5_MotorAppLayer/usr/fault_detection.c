/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� fault_detection.c
 * �ļ���ʶ��
 * ����ժҪ�� fault detectiong: volt ,current, temperature, stall, empty, phaseloss,\
 *
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� andrew kong
 * ������ڣ� 2019��11��28��
 * �޸���ʷ:
 *				2022-11-22 ��ӱ����������ù��ܣ��޸Ĳ��ֱ������룬Li Tonghua
 *******************************************************************************/
#include "fault_detection.h"
#include "MC_type.h"
#include "MC_math.h"
#include "MA_Static_Variable.h"
#include "state_machine.h"

/*******************************************************************************/
//�ⲿ����������
extern PSTR_MACtrProc	directGetMACtrPtr(u8 ObjID);
extern s16 getMotorCurFromObjId1ms(u8 t_bObjId);
extern s32 getVolt2FreqCalcPara(u8 t_bObjId);
extern s16 getMotorActVoltFromObjId1ms(u8 t_bObjId);


/*******************************************************************************/
//�ֲ���̬ȫ�ֱ�������������
static void CurrentAmplitudeCalc(STR_VectorUVW *pstruCurrPhaseUVW, STR_MACtrProc *pObj);

/*******************************************************************************/

static void updateFaultInfo2MACtrl(STR_MACtrProc *pObj)
{
	pObj->mg_UFault.R		= pObj->m_pFaults->R;		//	���´����־
	pObj->mg_eFaultPrio		= E_FAULT_PRIO_HIGH;		//	���ü����
	pObj->mg_eFaultStopMode = E_STOP_MOE_OFF;			//	�ر�MOEֹͣ���
}

/*****************************************************************************
 * ������   : FaultCurrentCheck(STR_VectorUVW *pstruCurrent, u8 t_bObj_ID)
 * ˵��     : ����������
 * ���˼· ��1. ��������������ֵ�����趨���������ֵ�����ж�Ϊ���������
 * ����     ��STR_VectorUVW *pstruCurrent, u8 t_bObj_ID
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.11.22
 * �޸�ʱ�� ��2023.09.04
 *             ��AD�е���ʱ���Ứ�Ѳ���MAָ���ʱ��
 *			   �����ʱ��Ҫ���ϸ���1ms�е���ִ��FaultCurrentCheckMA()
 *****************************************************************************/
void FaultCurrentCheck(STR_VectorUVW *pstruCurrent, u8 t_bObj_ID)
{
	PSTR_MACtrProc pObj = directGetMACtrPtr(t_bObj_ID);
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	
    /*�������˲ʱֵ�����趨��ֵ���ۼ�һ��ʱ����ж�Ϊ�������*/
    if((ABS(pstruCurrent->nPhaseU) > pObj->m_pFaultValue->nOverCurrent) 
		|| (ABS(pstruCurrent->nPhaseV) > pObj->m_pFaultValue->nOverCurrent) 
		|| (ABS(pstruCurrent->nPhaseW) > pObj->m_pFaultValue->nOverCurrent)
		)
    {
    	if(pObj->m_pstruFaultCurrentTime->nCheckCnt1++ >= pDrvCfgPara->mS_FBCurSense.nSoftOvCurTimes)
    	{
            pObj->m_pFaults->B.SoftCurretError = 1;				/*���������־��1*/
    	}
    }
	else
	{
		pObj->m_pstruFaultCurrentTime->nCheckCnt1 = 0;
	}
}

/*****************************************************************************
 * ������   : FaultCurrentCheckMA(STR_VectorUVW *pstruCurrent, PSTR_MACtrProc pObj)
 * ˵��     : ����������
 * ���˼· ��1. ��������������ֵ�����趨���������ֵ�����ж�Ϊ���������
 * ����     ��STR_VectorUVW *pstruCurrent, PSTR_MACtrProc pObj
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.11.22
 * �޸�ʱ�� ��2023.09.04
 *             ��AD�е���ʱ���Ứ�Ѳ���MAָ���ʱ��
 *             �����ʱ��Ҫ���ϸ���1ms�е���ִ��FaultCurrentCheckMA()
 *****************************************************************************/
static void FaultCurrentCheckMA(STR_VectorUVW *pstruCurrent, PSTR_MACtrProc pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	
    /*�������˲ʱֵ�����趨��ֵ���ۼ�һ��ʱ����ж�Ϊ�������*/
    if((ABS(pstruCurrent->nPhaseU) > pObj->m_pFaultValue->nOverCurrent) 
		|| (ABS(pstruCurrent->nPhaseV) > pObj->m_pFaultValue->nOverCurrent) 
		|| (ABS(pstruCurrent->nPhaseW) > pObj->m_pFaultValue->nOverCurrent))
    {
        if(pObj->m_pstruFaultCurrentTime->nCheckCnt1++ >= pDrvCfgPara->mS_FBCurSense.nSoftOvCurTimes)         /*���������־��1*/
    	{
			pObj->m_pFaults->B.SoftCurretError = 1;			/*���������־��1*/
    	}
    }
	else
	{
		pObj->m_pstruFaultCurrentTime->nCheckCnt1 = 0;
	}
}

/*****************************************************************************
 * ������   : FaultVoltageCheck(STR_MACtrProc *pObj)
 * ˵��     : ��Ƿѹ��� 
 * ���˼· ��1.��ĸ�ߵ�ѹ���ڹ�ѹ�趨ֵ���ж�Ϊ��ѹ����;��ĸ�ߵ�ѹС��Ƿѹ�趨ֵ \
 *              ���ж�ΪǷѹ���ϡ��˲�ʱ��Ϊ1s������ɸ���ʵ�������޸ġ�
 * ����     ��STR_MACtrProc *pObj
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.11.22
 * �޸�ʱ�� ��2023.09.04
 *****************************************************************************/
static void FaultVoltageCheck(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_FaultTime 			*pFaultVoltTime		= pObj->m_pstruFaultVoltTime;
	STR_FaultStatus			*t_pFaults			= pObj->m_pFaults;
	STR_FaultVariable		*t_pFaultValue		= pObj->m_pFaultValue;
	s16 					t_nVdc 				= pObj->mMdToMa.nVdc ;
	
	
    if(t_nVdc >= t_pFaultValue->nOverVoltage)    	/*ĸ�ߵ�ѹ���ڹ�ѹ�趨ֵ��ʱ���ۼ�*/
    {
        pFaultVoltTime->nCheckCnt1 += FAULT_DETECT_CYCLE;
    }
    else if(t_nVdc < t_pFaultValue->nUnderVoltage) /* ĸ�ߵ�ѹС��Ƿѹ�趨ֵ��ʱ���ۼ�*/
    {
        pFaultVoltTime->nCheckCnt2 += FAULT_DETECT_CYCLE;
    }
    else
    {
        pFaultVoltTime->nCheckCnt1 = 0;				// ��ѹ������0
        pFaultVoltTime->nCheckCnt2 = 0;
    }	
	
    if(pFaultVoltTime->nCheckCnt1 >= pDrvCfgPara->mS_FBDcUnit0.nOvVoltageDetectTime)	/* ʱ�䳬����ֵ���ж�Ϊ��ѹ */ 
    {																					/* pDrvCfgPara->mS_FBDcUnit0.nOvVoltageDetectTime * 5ms */
        pFaultVoltTime->nCheckCnt1 = 0;
        t_pFaults->B.VoltOverError = 1;													/* ��ѹ�����־λ��1 */
    }

    if(pFaultVoltTime->nCheckCnt2 >= pDrvCfgPara->mS_FBDcUnit0.nUndVoltageDetectTime)	/* ʱ�䳬����ֵ���ж�ΪǷѹ */
    {																					/* pDrvCfgPara->mS_FBDcUnit0.nUndVoltageDetectTime * 5ms */
        pFaultVoltTime->nCheckCnt2 = 0;
        t_pFaults->B.VoltUnderError = 1;												/*Ƿѹ�����־λ��1 */
    }

	
                                                                                                 
	if(pDrvCfgPara->mS_FBDcUnit0.bUndVoltProtectEnbale == 0)							/* Ƿѹ��ⲻʹ�������� */
	{
		pFaultVoltTime->nCheckCnt2	= 0;
		t_pFaults->B.VoltUnderError	= 0;
	}
	
	if(pDrvCfgPara->mS_FBDcUnit0.bOvVoltProtectEnbale == 0)								/* ��ѹ��ⲻʹ�������� */
	{
		pFaultVoltTime->nCheckCnt1	= 0;
		t_pFaults->B.VoltOverError	= 0;
	}
}


/*****************************************************************************
 * ������   : FaultEmptyCheck(STR_MACtrProc *pObj)
 * ˵��     : ��ˮ��ת��⣬ˮ��Ӧ�ã�����Ӧ�ÿ��Բ�ʹ��
 * ���˼· ��1.���IqС�ڿ�ת�����趨ֵ������ת�ٴ��ڿ�תת���趨ֵ�����ж�Ϊ��ˮ��ת��\
 *              �˲�ʱ��Ϊ1s������ɸ���ʵ�������޸ġ� 
 * ����     ��STR_MACtrProc *pObj
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 * �޸�ʱ�� ��2023.09.04
 *****************************************************************************/
static void FaultEmptyCheck(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_FaultTime		*pFaultEmptyTime 	= pObj->m_pstruFaultEmptyTime;
	
	if(pDrvCfgPara->mS_FBEmptyStall.bEmptyProtectEnable == 1)
	{
	    if((ABS(pObj->m_nQCur_Reference) < pObj->m_pFaultValue->nEmptyCurrent) 
			&& (ABS(pObj->mMdToMa.wFreq) > pObj->m_pFaultValue->wEmptySpeed))		/*���IqС���趨ֵ��ת�ٴ����趨ֵ��ʱ���ۼӣ�����ʱ����0 */
	    {
	       pFaultEmptyTime->nCheckCnt1 += FAULT_DETECT_CYCLE;
	    }
	    else
	    {
	        pFaultEmptyTime->nCheckCnt1 = 0;
	    }

		

	    if(pFaultEmptyTime->nCheckCnt1 >= pDrvCfgPara->mS_FBEmptyStall.nEmptyDetectTime)  //�ۼ�ʱ�䳬��* 5ms ���ж�Ϊ��ˮ��ת
	    {
	        pFaultEmptyTime->nCheckCnt1 = 0;
	        pObj->m_pFaults->B.EmptyError = 1;						//��ת�����־λ��1
	    }
	}
	else
	{
		pFaultEmptyTime->nCheckCnt1 = 0;
		pObj->m_pFaults->B.EmptyError = 0;       					//��ת�����־λ��0
	}
}

/*****************************************************************************
 * ������   : FaultStallCheck(STR_MACtrProc *pObj)
 * ˵��     : ��ת&�������
 * ���˼· ��
 *            1.ת�ٺ͵�ѹ�����쳣���ж�Ϊ��ת ;                 \
 *            2.���������ʱЧ���ж�Ϊ�������ϣ������ж�Ϊ��ת   \
 * ����     ��STR_MACtrProc *pObj
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.11.24
 * �޸�ʱ�� ��2023.09.04
 *****************************************************************************/
static void FaultStallCheck(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara		= pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_FaultTime		*pStallFaultTime 	= pObj->m_pstruFaultStallTime;
	STR_FaultVariable	*pFaultValue		= pObj->m_pFaultValue;

	s32 wSpeedfbkABS;
	s32 wSpeedfromVolt;
	s32 wSpeedfromVolt1;

	wSpeedfbkABS	= ABS(pObj->m_pMotorSpeed->wSpeedfbk);
	

	s16 t_nCurrent		= getMotorCurFromObjId1ms(pObj->mg_nMACtrlProcID);
	
	
	if(pDrvCfgPara->mS_FBEmptyStall.bStallProtectEnable== 1)
	{
	    if(pStallFaultTime->nStartDelay < pDrvCfgPara->mS_FBEmptyStall.nStallDelaytime ) //��ʱ* 5ms���ܿ������϶���Ӱ��
	    {
	        pStallFaultTime->nStartDelay += FAULT_DETECT_CYCLE;
	    }
	    else
	    {
	    	// need to check. if necessary use?
	        if( t_nCurrent > pFaultValue->wStallCurMax )  			//����������ڶ�ת������ CUR_STALL_MAX_FAULT_M0	(1.8)	��λ��A  ��ת������ƫ�����ֵ 
	        {
	        	pFaultValue->nStallCurFlag = 1;	           
	        }
	        else
	        {
	        	if( t_nCurrent < pFaultValue->wStallCurMin )		//�������С�ڶ�ת�������� CUR_STALL_MIN_FAULT_M0	(1.2)	��λ��Hz ��ת����˳�������Сֵ 
        		{
        			pFaultValue->nStallCurFlag = 0;        			
        		}	            
	        }

			if(pFaultValue->nStallCurFlag == 1)
	        {
	            if(pStallFaultTime->nCheckCnt1 < pDrvCfgPara->mS_FBEmptyStall.nStallDetectTime)      //�˲�ʱ��*5ms
	            {
	                pStallFaultTime->nCheckCnt1 += FAULT_DETECT_CYCLE;	//ʱ���ʱ
	            }
	            else												//�ж�Ϊ�����ת
	            {                                   
	                pStallFaultTime->nCheckCnt1 = 0;
	                pObj->m_pFaults->B.StallError		= 1;		//��ת�����־λ��1
	                pFaultValue->nStallFlag				= 1;		//��ת��־λΪ1���������ִ��������ֶ�ת
	            }
	        }
	        else
	        {
				pStallFaultTime->nCheckCnt1	-= FAULT_DETECT_CYCLE;	//ʱ�����
				if(pStallFaultTime->nCheckCnt1 < 0)
				{
	            pStallFaultTime->nCheckCnt1 = 0;
	        }
			}

			wSpeedfromVolt = (getVolt2FreqCalcPara(pObj->mg_nMACtrlProcID) * getMotorActVoltFromObjId1ms(pObj->mg_nMACtrlProcID)) >> 15;
			wSpeedfromVolt1 = (wSpeedfromVolt * pFaultValue->wStallSpeedfactor) >> 8;

	        if( wSpeedfbkABS > wSpeedfromVolt1 ) 					//�����ѹ��ת�ٲ�ƥ�䣬��ѹСת�ٸߣ����ж�Ϊ��ת��
	        {
	            if(pStallFaultTime->nCheckCnt2 < pDrvCfgPara->mS_FBEmptyStall.nStallDetectTime)      //�˲�ʱ��5ms 
	            {
	                pStallFaultTime->nCheckCnt2 += FAULT_DETECT_CYCLE;
	            }
	            else                                 //�ж�Ϊ�����ת 
	            {
	                pStallFaultTime->nCheckCnt2			= 0;
	                pObj->m_pFaults->B.StallError		= 1;		//��ת�����־λ��1
	                pFaultValue->nStallFlag				= 2;		//��ת��־λΪ2���������ִ��������ֶ�ת
	            }
	        }
	        else
	        {
	            pStallFaultTime->nCheckCnt2 = 0;
	        }
			// need check end
	    }
	}
	else
	{		
		pObj->m_pFaults->B.StallError			= 0;		//��ת��������
		pFaultValue->nStallFlag					= 0;		//������־����	
		pStallFaultTime->nCheckCnt1				= 0;
		pStallFaultTime->nCheckCnt2				= 0;
	}
}

/*****************************************************************************
 * ������   : FaultSencondStartCheck(STR_MACtrProc *pObj)
 * ˵��     : ��������
 * ���˼· �������������ڽ�������5s���жϣ�ʱ�����ʵ���޸�
 *          ��1.����open֮��2.5s�ڻ�û����ջ���ʱ��Ҫ����ʵ������޸�                 \
 *          ��2.ת�ٹ��߻��߹��ͣ��趨��ֵ�Ͷ�ת�����ͬ                             \
 *          : 3.���������ֵ���ڵ����趨ֵ���趨ֵ�Ͷ�ת�����ͬ
 * ����     ��s32 wSpeedFbk, s16 nIq, STR_CurrPhaseUVW *pstruIabcMax, STR_FaultTime *pstruFaultTime
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.11.23
 *****************************************************************************/
static void FaultStartCheck(STR_MACtrProc *pObj)
{
	//s32 wSpeedFbk								= pObj->m_pMotorSpeed->wSpeedfbk;
	//s16 nIq										= pObj->m_nQCur_Reference;
	STR_VectorUVW 		*pstruIabcMax			= pObj->m_pstruImaxCurrent;
	STR_FaultTime 		*pstruFaultTime			= pObj->m_pstruFaultStartTime;
	PSTR_DrvCfgPara		pDrvCfgPara 			= pObj->mg_pCtrlObj->m_pDrvCfgPara;
	
	if(pDrvCfgPara->mS_FBEmptyStall.bStartProtectEnable == 1)
	{
    	if(pstruFaultTime->nStartDelay < pDrvCfgPara->mS_FBEmptyStall.nStartFaultDetectTime)      //����10s�ڼ�⣬10s���ټ��  5ms*2000
	    {
	        pstruFaultTime->nStartDelay += FAULT_DETECT_CYCLE;

	        if((pstruIabcMax->nPhaseU > pObj->m_pFaultValue->nStallCurrent) 
				|| (pstruIabcMax->nPhaseV > pObj->m_pFaultValue->nStallCurrent) 
				|| (pstruIabcMax->nPhaseW > pObj->m_pFaultValue->nStallCurrent)
				)//���������ֵ������ת�����趨ֵ���ж�Ϊ��ת
	        {
	            pObj->m_pFaults->B.StartError		= 1;				//���������־λ��1
	            pObj->m_pFaultValue->nStartFlag		= 3;             	//���������־λΪ3
	        }
	        
	        if((pObj->m_pAppCommData->bOpenRunFlag == 0)
				&&(pstruFaultTime->nStartDelay > pDrvCfgPara->mS_FBEmptyStall.nStartClosedDetectTime)
				)     //����600*5ms = 3s��û����ջ������ж�Ϊ����ʧ�ܣ�����ʱ�䰴��ʵ����Ŀ����
	        {
	            pObj->m_pFaults->B.StartError		= 1;              //���������־λ��1
	            pObj->m_pFaultValue->nStartFlag		= 4;            //���������־λΪ4
	        }
	    }
	}
	else
	{
		pstruFaultTime->nStartDelay					= 0;
		pObj->m_pFaults->B.StartError				= 0;				//���������־λ��0
		pObj->m_pFaultValue->nStartFlag				= 0;             	//���������־λΪ0

	}
}

/***********************************************************************************
 * ������   : FaultPhaseCheck(STR_MACtrProc *pObj)
 * ˵��     : ȱ���� 
 * ���˼· ��1.���������ֵ���3�������ж�Ϊ���ȱ�࣬���ʱ��ͼ�ⱶ������ʵ���޸�
 *            2.һ��ʱ���ڵ�����ֵС���趨��ֵ�����ж�Ϊȱ�࣬ ���ʱ��ͼ�ⱶ������ʵ���޸�
 * ����     ��STR_MACtrProc *pObj
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17

 * �޸�ʱ�� : 2023.09.04
 * �޸����� ��������������
 ***********************************************************************************/
static void FaultPhaseCheck(STR_MACtrProc *pObj)
{
	STR_FaultTime 		*pstruFaultTime 		= pObj->m_pstruFaultPhaseTime;
	STR_VectorUVW	  	*pstruImaxCurrent		= pObj->m_pstruImaxCurrent;
	STR_CurrentAmplitude *pstruCurrentAmplitude	= pObj->m_pstruCurrentAmplitude;
	PSTR_DrvCfgPara		pDrvCfgPara 			= pObj->mg_pCtrlObj->m_pDrvCfgPara;

	if(pDrvCfgPara->mS_FBEmptyStall.bLossPHProtectEnable == 1)
	{
	    if(pstruFaultTime->nCheckCnt1 < pDrvCfgPara->mS_FBEmptyStall.nLossPHDetectTime)//*���ʱ��Ϊ pDrvCfgPara->mS_FBEmptyStall.nLossPHDetectTime * 5ms */
	    {
	        pstruFaultTime->nCheckCnt1 += FAULT_DETECT_CYCLE;
	    }
	    else
	    {
	        pstruFaultTime->nCheckCnt1 = 0;
	        
	        if(((pstruImaxCurrent->nPhaseU >> 2) > pstruImaxCurrent->nPhaseV) || (((pstruImaxCurrent->nPhaseV >> 2) > pstruImaxCurrent->nPhaseW)) ||
	          (((pstruImaxCurrent->nPhaseW >> 2) > pstruImaxCurrent->nPhaseU)))				/*������������ֵ���3�����ж�Ϊȱ�࣬������Ҫ����ʵ�ʸ��ص���*/
	        {
	            pObj->m_pFaults->B.PhaseLossError 		= 1;								/* ȱ������־λ��1 */
	            pObj->m_pFaultValue->nPhaseLossFlag		= 1;								/* ��һ��ȱ�ౣ�� */
	        }
	        
	        pstruImaxCurrent->nPhaseU = 0;
	        pstruImaxCurrent->nPhaseV = 0;
	        pstruImaxCurrent->nPhaseW = 0;
	    }

	    if((pstruCurrentAmplitude->nPhA < pObj->m_pFaultValue->nPhaseLossCurrent)			/* ���������ֵС���趨��ֵ���ۻ�ʱ��С���趨��ֵ���ж�Ϊȱ��*/
			||(pstruCurrentAmplitude->nPhB < pObj->m_pFaultValue->nPhaseLossCurrent)
			||(pstruCurrentAmplitude->nPhC < pObj->m_pFaultValue->nPhaseLossCurrent))
	    {
	        if(pstruFaultTime->nCheckCnt2 < pDrvCfgPara->mS_FBEmptyStall.nLossPHDetectTime)     /*���ʱ��Ϊ pDrvCfgPara->mS_FBEmptyStall.nLossPHDetectTime * 5ms*/
	        {
	            pstruFaultTime->nCheckCnt2 += FAULT_DETECT_CYCLE;
	        }
	        else
	        {
	            pstruFaultTime->nCheckCnt2				= 0;
	            pObj->m_pFaults->B.PhaseLossError 		= 1;						/* ȱ������־λ��1 */
	            pObj->m_pFaultValue->nPhaseLossFlag		= 2;						 /*�ڶ���ȱ�ౣ��   */
	        }
	    }
	    else
	    {
	        pstruFaultTime->nCheckCnt2					= 0;
	    }
	}
	else
    {                                                     /* ���� */
		pstruFaultTime->nCheckCnt1				= 0;
		pstruFaultTime->nCheckCnt2				= 0;
		pObj->m_pFaults->B.PhaseLossError 		= 0;
		pObj->m_pFaultValue->nPhaseLossFlag		= 0;

	}
}

/***********************************************************************************
 * ������   : FaultTempCheck(STR_MACtrProc *pObj)
 * ˵��     : �¶ȱ�����⣬���NTC���
 * ���˼· ��1.����NTC����ֵȥ�ж��Ƿ������¹���,������·�ж�
 * ����     ��pObj
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.11.22
 ***********************************************************************************/

static s16 getPowerUnitTemp (STR_MACtrProc *pObj)
{
	return 0;
}

static void FaultTempCheck(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;

	pObj->m_nInvTemp = getPowerUnitTemp(pObj);

	if(pDrvCfgPara->mS_FBTempNTC.bOvTempProtectEnable == 1)
	{
		/* �¶ȴ����趨ֵ�����ж�Ϊ���¹���; */
        /* �¶�ֵС��һ��ֵ���ж�ΪNTC��·   */
	    if( (pObj->m_nInvTemp  > (s16)pDrvCfgPara->mS_FBTempNTC.m_wMaxAllowedWorkTemp)
			|| (pObj->m_nInvTemp  < (s16)pDrvCfgPara->mS_FBTempNTC.m_wMinAllowedWorkTemp)
		  )
	    {
	        pObj->m_pstruFaultTempTime->nCheckCnt1 += FAULT_DETECT_CYCLE;
	    }
	    else
	    {
	        pObj->m_pstruFaultTempTime->nCheckCnt1 = 0;
	    }

	    if(pObj->m_pstruFaultTempTime->nCheckCnt1 > pDrvCfgPara->mS_FBTempNTC.nDetectionTime) /*�˲�ʱ�� pDrvCfgPara->mS_FBTempNTC.nDetectionTime * 5ms */
	    {
	        pObj->m_pFaults->B.TempOverError		= 1;				/*�¶ȹ��ϱ�־λ��1*/
	        pObj->m_pstruFaultTempTime->nCheckCnt1	= 0;
	    }
	}
    else                                                                /* ���� */
	{
		pObj->m_pFaults->B.TempOverError 			= 0;
		pObj->m_pstruFaultTempTime->nCheckCnt1 		= 0;
	}
}

/***********************************************************************************
 * ������   : VoltFaultInit(void)
 * ˵��     : ��ѹ�������������ʼ��
 * ���˼· �����ʱ�����������0
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 ***********************************************************************************/
void VoltFaultInit(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_TransCoefElement *pAppToCore	= &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);

    /******��Ƿѹ���ϼ�������ʼ��*********/
    pObj->m_pstruFaultVoltTime->nCheckCnt1 = 0;
    pObj->m_pstruFaultVoltTime->nCheckCnt2 = 0;
    pObj->m_pFaultValue->nOverVoltage	= App2CoreVoltTrans(pAppToCore,pDrvCfgPara->mS_FBDcUnit0.m_wDCOverVoltage);
    pObj->m_pFaultValue->nUnderVoltage	= App2CoreVoltTrans(pAppToCore,pDrvCfgPara->mS_FBDcUnit0.m_wDCUnderVoltage);
}

/***********************************************************************************
 * ������   : FaultInitTask(void)
 * ˵��     : �����������ʼ������
 * ���˼· ��1.������ʼ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 ***********************************************************************************/
void FaultInitTask(STR_MACtrProc *pObj)
{
	STR_TransCoefElement *pAppToCore = &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);

	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	
    /********�������ϼ�������ʼ��*****/
	pObj->m_pFaultValue->nOverCurrent = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBCurSense.m_wSoftOvCur);
    
    /******��ˮ��ת���ϼ�������ʼ��********/
    pObj->m_pstruFaultEmptyTime->nCheckCnt1 = 0;
    pObj->m_pFaultValue->nEmptyCurrent = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBEmptyStall.m_wEmptyCurrent);
    pObj->m_pFaultValue->wEmptySpeed = App2CoreFreqTrans(pAppToCore,pDrvCfgPara->mS_FBEmptyStall.m_wEmptySpeed);

    /******��ת���ϼ�������ʼ��*********/
    pObj->m_pstruFaultStallTime->nCheckCnt1 = 0;
    pObj->m_pstruFaultStallTime->nCheckCnt2 = 0;
    pObj->m_pstruFaultStallTime->nStartDelay = 0;
    pObj->m_pFaultValue->nStallCurrent		= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBEmptyStall.m_wStallPHCurrent);
    pObj->m_pFaultValue->wStallCurMax		= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBEmptyStall.m_wStallCurMax);
    pObj->m_pFaultValue->wStallCurMin		= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBEmptyStall.m_wStallCurMin);
    pObj->m_pFaultValue->wStallSpeedfactor	= pDrvCfgPara->mS_FBEmptyStall.m_wStallSpeedFactor;
    pObj->m_pFaultValue->nStallFlag = 0;
	pObj->m_pFaultValue->nStallCurFlag = 0;

    /******ȱ����ϼ�������ʼ��*********/
    pObj->m_pstruFaultPhaseTime->nCheckCnt1 = 0;
	pObj->m_pFaultValue->nPhaseLossCurrent	= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBEmptyStall.m_wLosePhaseCurrent);
    
	memset(pObj->m_pstruCurrentAmplitude, 0, sizeof(STR_CurrentAmplitude));
	memset(pObj->m_pstruImaxCurrent, 0, sizeof(STR_VectorUVW));
    
    /*****�¶ȹ��ϼ�������ʼ��****/
    pObj->m_pstruFaultTempTime->nCheckCnt1 = 0;

    /*�������ϼ�������ʼ��*/ 
    //pObj->m_pstruFaultStartTime->nCheckCnt1 = 0;
    //pObj->m_pstruFaultStartTime->nCheckCnt2 = 0;
    //pObj->m_pstruFaultStartTime->nStartDelay = 0;
	memset(pObj->m_pstruFaultStartTime,0,sizeof(STR_FaultTime));
}


/***********************************************************************************
 * ������   : FaultInit(void)
 * ˵��     : �����������ʼ��
 * ���˼· ��1.������ʼ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 ***********************************************************************************/
void FaultInit(STR_MACtrProc *pObj)
{
	VoltFaultInit(pObj);
	FaultInitTask(pObj);	
}


/***********************************************************************************
 * ������   : FaultCheck(void)
 * ˵��     : ���ϼ�⴦��
 * ���˼· ��1.ÿ�����ϼ��Ĵ�������Ϊ5ms
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 ***********************************************************************************/
void FaultCheck(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_PubDatHDMD *pHdToMd		= &pObj->mg_pCtrlObj->mGlobalDatPackage.mHdToMd;
	
	if(pObj->m_SMACtrlBit.m_bFaultInitDone == 0)	//	������ʼ���Ž��б������
	{
		return;
	}
    if(pObj->t_nFlagCheck < FAULT_DETECT_CYCLE)		//	������ѯ
    {
        pObj->t_nFlagCheck ++;
    }
    else
    {
        pObj->t_nFlagCheck = 1;
    }

	if(pObj->mMdToMa.bBreakInFault)					// Ӳ����������
	{
		pObj->m_pFaults->B.HardCurretError		= 1;		//	������־��1
	}

	
	if( (ABS(pHdToMd->nPhaseUOffset)		>= pDrvCfgPara->mS_GlobalCfg.m_nuADCOffsetError)
		|| (ABS(pHdToMd->nPhaseVOffset) 	>= pDrvCfgPara->mS_GlobalCfg.m_nuADCOffsetError)
		|| (ABS(pHdToMd->nPhaseWOffset)		>= pDrvCfgPara->mS_GlobalCfg.m_nuADCOffsetError)
		|| (ABS(pHdToMd->nBusShuntOffset)	>= pDrvCfgPara->mS_GlobalCfg.m_nuADCOffsetError)
	  )			
	{
		pObj->m_pFaults->B.OffsetError		= 1;		//	������־��1
	}

	FaultCurrentCheckMA(&pObj->mMdToMa.mStatCurrUVW, pObj);		//	����������
	CurrentAmplitudeCalc(&pObj->mMdToMa.mStatCurrUVW, pObj);	//	������ֵ���
	
    switch(pObj->t_nFlagCheck)
    {
        case 1:
        { 
            FaultVoltageCheck(pObj);  					//��Ƿѹ���
            break;
        }
        case 2:
        {
            if(pObj->m_eSysState == E_DRIVER_RUN)          
            {
//				FaultStallCheck(pObj);					//��ת���  �����Ѳ�2022-1124
            }
            break;
        }
        case 3:
        {
            if(pObj->m_eSysState == E_DRIVER_RUN)
            {
//                FaultEmptyCheck(pObj);  				//��ˮ��ת���   �Ѳ�2022-1124
            }
            
            FaultTempCheck(pObj);						//���¼��
            break;
        }
        case 4:
        {
            if(pObj->m_eSysState == E_DRIVER_RUN)
            {
//				FaultPhaseCheck(pObj);					//ȱ����		�Ѳ�2022-1124
            }
            break;
        }
        case 5:
        {
 
            if((pObj->m_eSysState == E_DRIVER_RUN))
            {
//			FaultStartCheck(pObj);						//�����������	�Ѳ�2022-1123
            }
            break;
        }
        default:
            break;
    }

	updateFaultInfo2MACtrl(pObj);
}

/***********************************************************************************
 * ������   : FaultRecoverInit(void)
 * ˵��     : ���ϻָ�������ʼ��
 * ���˼· ��������ʼ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 ***********************************************************************************/
void FaultRecoverInit(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_TransCoefElement 	*pAppToCore	= &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);
    pObj->m_pFaults->R = 0;
    pObj->m_pstruFaultEmptyTime->nRecoverCntr   = 0;
    pObj->m_pstruFaultStallTime->nRecoverCntr   = 0;
    pObj->m_pstruFaultCurrentTime->nRecoverCntr = 0;
    pObj->m_pstruFaultCurrentTime->nRecoverTime = 0;
    pObj->m_pstruFaultVoltTime->nRecoverCntr    = 0;
    pObj->m_pstruFaultTempTime->nRecoverCntr    = 0;
    pObj->m_pFaultValue->nOverVoltageRecover	= App2CoreVoltTrans(pAppToCore,pDrvCfgPara->mS_FBDcUnit0.m_wDCOverVoltageRecover);
    pObj->m_pFaultValue->nUnderVoltageRecover	= App2CoreVoltTrans(pAppToCore,pDrvCfgPara->mS_FBDcUnit0.m_wDCUnderVoltageRecover);
}

/***********************************************************************************
 * ������   : FaultRecover(void)
 * ˵��     : ���ϻָ�������
 * ���˼· ��1.��Ƿѹ�ָ���       ĸ�ߵ�ѹ������������Χ�ڣ�����2s�������Ƿѹ���ϱ�־λ; \
 *            2.���&Ӳ�������ָ���2s������������ϱ�־λ;                                 \
 *            3.��ת�ָ���         2s�������ת���ϱ�־λ;                                 \
 *            4.ȱ��ָ���         2s������������ϱ�־λ;                                 \
 *            5.���»ָ���         NTC������������Χ�ڣ�����2s��������¹��ϱ�־λ; \
 *            6.�����ָ���         1s������
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 ***********************************************************************************/
void FaultRecover(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_FaultStatus			*t_pFaults			= pObj->m_pFaults;
	STR_FaultVariable		*t_pFaultValue		= pObj->m_pFaultValue;
	s16 t_nTmp;
    /****************��Ƿѹ�ָ�****************/
    if((t_pFaults->B.VoltOverError == 1) || (t_pFaults->B.VoltUnderError == 1))
    {   //ĸ�ߵ�ѹ��Ƿѹ�ָ�ֵ�͹�ѹ�ָ�ֵ֮�����ж�Ϊ����״̬
        if((pObj->mMdToMa.nVdc	>= t_pFaultValue->nUnderVoltageRecover) 
          && (pObj->mMdToMa.nVdc <= t_pFaultValue->nOverVoltageRecover))
        {
            if(pObj->m_pstruFaultVoltTime->nRecoverCntr < pDrvCfgPara->mS_FBDcUnit0.nVoltageRecoverTime)     /*��ѹ�ָ������������趨ʱ�������Ƿѹ���ϱ�־λ*/
            {
                pObj->m_pstruFaultVoltTime->nRecoverCntr ++;
            }
            else
            {
                t_pFaults->B.VoltOverError = 0;
                t_pFaults->B.VoltUnderError = 0;
                pObj->m_pstruFaultVoltTime->nRecoverCntr = 0;
            }
        }
        else
        {
            pObj->m_pstruFaultVoltTime->nRecoverCntr = 0;
        }
    }

    /****************�����ָ�****************/
    if((t_pFaults->B.HardCurretError == 1) || (t_pFaults->B.SoftCurretError == 1))
    {
    	if(pDrvCfgPara->mS_FBCurSense.nOvCurRecoverTime != 0xFFFF)
    	{
	        if(pObj->m_pstruFaultCurrentTime->nRecoverCntr < pDrvCfgPara->mS_FBCurSense.nOvCurRecoverTime)	/*�����趨ʱ������������ϱ�־λ*/
	        {
	            pObj->m_pstruFaultCurrentTime->nRecoverCntr ++;
	        }
	        else
	        {
	            if(t_pFaults->B.HardCurretError == 1)
	            {	            	
	                t_pFaults->B.HardCurretError = 0;
					pObj->m_SMACtrlBit.m_bBreakInClrReq = 1;		// Ӳ�������������������BreakIn�������
	            }

	            if(t_pFaults->B.SoftCurretError == 1)
	            {
	                t_pFaults->B.SoftCurretError = 0;
					pObj->m_pstruFaultCurrentTime->nCheckCnt1 = 0;
	            }

	            pObj->m_pstruFaultCurrentTime->nRecoverTime ++;
	            pObj->m_pstruFaultCurrentTime->nRecoverCntr = 0;
	        }
    	}
    }

    /****************��ת�ָ�****************/
    if(t_pFaults->B.StallError == 1)
    {
    	if(pDrvCfgPara->mS_FBEmptyStall.nStallRecoverTime != 0xFFFF)
    	{
	        if(pObj->m_pstruFaultStallTime->nRecoverCntr < pDrvCfgPara->mS_FBEmptyStall.nStallRecoverTime)		/*�����趨ʱ�������ת���ϱ�־λ*/
	        {
	            pObj->m_pstruFaultStallTime->nRecoverCntr ++;
	        }
	        else
	        {
	            t_pFaults->B.StallError = 0;	            
	            t_pFaultValue->nStallFlag = 0;
				t_pFaultValue->nStallCurFlag = 0;
				memset(pObj->m_pstruFaultStallTime,0,sizeof(STR_FaultTime));
	        }
    	}
    }

    /****************��ˮ��ת�ָ�****************/
    if(t_pFaults->B.EmptyError == 1)
    {
    	if(pDrvCfgPara->mS_FBEmptyStall.nEmptyRecoverTime != 0xFFFF)
    	{
	        if(pObj->m_pstruFaultEmptyTime->nRecoverCntr < pDrvCfgPara->mS_FBEmptyStall.nEmptyRecoverTime)/*�����趨ʱ��������ˮ���ϱ�־λ*/
	        {
	            pObj->m_pstruFaultEmptyTime->nRecoverCntr ++;
	        }
	        else
	        {
	            pObj->m_pFaults->B.EmptyError				= 0;
	            pObj->m_pstruFaultEmptyTime->nRecoverCntr	= 0;
				pObj->m_pstruFaultEmptyTime->nCheckCnt1		= 0;
	        }
    	}
    }

    /****************ȱ��ָ�****************/
    if(t_pFaults->B.PhaseLossError == 1)
    {
    	if(pDrvCfgPara->mS_FBEmptyStall.nLossPHRecoverTime != 0xFFFF)
    	{
	        if(pObj->m_pstruFaultPhaseTime->nRecoverCntr < pDrvCfgPara->mS_FBEmptyStall.nLossPHRecoverTime)/*�����趨ʱ������ȱ����ϱ�־λ*/
	        {
	            pObj->m_pstruFaultPhaseTime->nRecoverCntr ++;
	        }
	        else
	        {
	            t_pFaults->B.PhaseLossError = 0;
	            pObj->m_pstruFaultPhaseTime->nRecoverCntr = 0;
				pObj->m_pstruFaultPhaseTime->nCheckCnt1 = 0;
	        }
    	}
    }

    /****************���»ָ�****************/
    if(t_pFaults->B.TempOverError == 1)
    {
    	t_nTmp	= (s16)(pDrvCfgPara->mS_FBTempNTC.m_wMaxAllowedWorkTemp) - pDrvCfgPara->mS_FBTempNTC.HysterTemp;
        if( pObj->m_nInvTemp <= t_nTmp )				// �¶�С��max - hy 
        {
            if(pObj->m_pstruFaultTempTime->nRecoverCntr < pDrvCfgPara->mS_FBTempNTC.nFaultRecoverTime)/*�����趨ʱ�������¶ȹ��ϱ�־λ*/
            {
                pObj->m_pstruFaultTempTime->nRecoverCntr ++;
            }
            else
            {
                t_pFaults->B.TempOverError = 0;
                pObj->m_pstruFaultTempTime->nRecoverCntr = 0;
            }
        }
        else
        {
            pObj->m_pstruFaultTempTime->nRecoverCntr = 0;
        }
    }

    /****************����ʧ�ָܻ�****************/
    if(t_pFaults->B.StartError == 1)
    {
    	if(pDrvCfgPara->mS_FBEmptyStall.nStartFaultRecoverTime != 0xFFFF)
    	{
	        if(pObj->m_pstruFaultStartTime->nRecoverCntr < pDrvCfgPara->mS_FBEmptyStall.nStartFaultRecoverTime)//START_FAULT_RECOVER_TIME) //1s������ȱ����ϱ�־λ
	        {
	            pObj->m_pstruFaultStartTime->nRecoverCntr ++;
	        }
	        else
	        {
	            t_pFaults->B.StartError = 0;	            
	            t_pFaultValue->nStartFlag = 0;
				memset(pObj->m_pstruFaultStartTime,0,sizeof(STR_FaultTime));
	        }
		}
    }

}

/***********************************************************************************
 * ������   : static CurrentAmplitudeCalc(STR_CurrPhaseUVW *pstruCurrPhaseUVW,stru_SWLIBS_3wSyst *pCurrentAmplitude )
 * ˵��     : ������Чֵ���㣬 
 * ���˼· ��������������ڶԵ�������
 * ����     ��STR_CurrPhaseUVW *pstruCurrPhaseUVW, stru_SWLIBS_3wSyst *pCurrentAmplitude
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.10.9
 ***********************************************************************************/
static void CurrentAmplitudeCalc(STR_VectorUVW *pstruCurrPhaseUVW, STR_MACtrProc *pObj)
{
	STR_VectorUVW	 		*pstruCurrentMAX 		= pObj->m_pstruImaxCurrent;
	STR_CurrentAmplitude 	*pstruCurrentAmplitude	= pObj->m_pstruCurrentAmplitude;

	s16 t_nPhaseU_ABS		= ABS(pstruCurrPhaseUVW->nPhaseU);
	s16 t_nPhaseV_ABS		= ABS(pstruCurrPhaseUVW->nPhaseV);
	s16 t_nPhaseW_ABS		= ABS(pstruCurrPhaseUVW->nPhaseW);
	
	/***********ȡ����������ֵ��ȱ����������õ�************/
	
    if(t_nPhaseU_ABS > pstruCurrentMAX->nPhaseU)
    {
        pstruCurrentMAX->nPhaseU = t_nPhaseU_ABS;
    }

    if(t_nPhaseV_ABS > pstruCurrentMAX->nPhaseV)
    {
        pstruCurrentMAX->nPhaseV = t_nPhaseV_ABS;
    }

    if(t_nPhaseW_ABS > pstruCurrentMAX->nPhaseW)
    {
        pstruCurrentMAX->nPhaseW = t_nPhaseW_ABS;
    }

	// �������1600���ٶȻ����ڣ�ʱ�䲻����ʱ�������ʱ��
	if(pstruCurrentAmplitude->nTheta <= 100 * 16)
	{
		pstruCurrentAmplitude->nTheta++;
		if(t_nPhaseU_ABS > pstruCurrentAmplitude->wPhATemp)
		{
			pstruCurrentAmplitude->wPhATemp = t_nPhaseU_ABS;
		}
		
		if(t_nPhaseV_ABS > pstruCurrentAmplitude->wPhBTemp)
		{
			pstruCurrentAmplitude->wPhBTemp = t_nPhaseV_ABS;
		}

		if(t_nPhaseW_ABS > pstruCurrentAmplitude->wPhCTemp)
		{
			pstruCurrentAmplitude->wPhCTemp = t_nPhaseW_ABS;
		}
	}
	else
	{
	pstruCurrentAmplitude->nPhA			= pstruCurrentAmplitude->wPhATemp;
	pstruCurrentAmplitude->nPhB			= pstruCurrentAmplitude->wPhBTemp;
	pstruCurrentAmplitude->nPhC			= pstruCurrentAmplitude->wPhCTemp;

	pstruCurrentAmplitude->wPhATemp		= 0;
	pstruCurrentAmplitude->wPhBTemp		= 0;
	pstruCurrentAmplitude->wPhCTemp		= 0;
	pstruCurrentAmplitude->nTheta		= 0;
	}
}
 

