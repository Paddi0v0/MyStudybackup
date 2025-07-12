/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� SpeedScan.c
 * �ļ���ʶ��
 * ����ժҪ�� ��λ�����٣�PWM���ٴ������
 * ����˵���� 
 * ��ǰ�汾�� V1.0
 * ��    �ߣ�   HuangMG
 * ������ڣ� 2022��5��30��
 *
 *******************************************************************************/
#include "speedscan.h"
#include "MC_type.h"
#include "fault_detection.h"
#include "Global_Variable.h"

/*****************************************************************************
 * ������   : VspSpeedScan(void)
 * ˵��     : 
 * ���˼· ��1.���ݼ�⵽��ѹ�����Ӧת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void VspSpeedScan(void)
{
    if(struAppCommData.nVspADC < VSP_OFF)
    {
        struFOC_CtrProc.bMC_RunFlg = 0;   //�ػ�
    }
    else if(struAppCommData.nVspADC < VSP_MIN)
    {
         struAppCommData.wVSPSpeedSet = VSP_SPEED_MIN;
        
        if((struFOC_CtrProc.bMC_RunFlg == 0) && (stru_Faults.R == 0))
        {
            struFOC_CtrProc.bMC_RunFlg = 1;
        }
    }
    else if(struAppCommData.nVspADC  < VSP_MAX)
    {
        struAppCommData.wVSPSpeedSet = (VSP_SPEED_SCALE * (struAppCommData.nVspADC - VSP_MIN)) + VSP_SPEED_MIN;
        
        if((struFOC_CtrProc.bMC_RunFlg == 0) && (stru_Faults.R == 0))
        {
            struFOC_CtrProc.bMC_RunFlg = 1;
        }
        
    }
    else
    {
        struAppCommData.wVSPSpeedSet = VSP_SPEED_MAX;

        if((struFOC_CtrProc.bMC_RunFlg == 0) && (stru_Faults.R == 0))
        {
            struFOC_CtrProc.bMC_RunFlg = 1;
        }
    }
    struAppCommData.wSpeedValue = App2CoreFreqTrans(User2AppFreqTrans(struAppCommData.wVSPSpeedSet));
 		
}


void KEY_Scan(void)
{
  if( GPIO_ReadInputDataBit(GPIO2, GPIO_Pin_11) == 0)
	{
		struFOC_CtrProc.bMC_RunFlg = 1;
	}
	
	 if(GPIO_ReadInputDataBit(GPIO2, GPIO_Pin_12) == 0)
	{
		struFOC_CtrProc.bMC_RunFlg = 0;
	
	}

}



