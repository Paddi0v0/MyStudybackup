/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Time_Process.c
 * �ļ���ʶ��
 * ����ժҪ�� ��ʱ��غ���
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2020��8��16��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��8��16��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet Li
 * �޸����ݣ�����
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "basic.h"
#include "project_config.h"
#include "MA_Static_Variable.h"
#include "math.h"
#include "Time_Process.h"
#include "state_machine.h"

extern void MACtrlObjTask(void);
extern void UACtrlObjTask(void);

extern STR_TaskScheduler gS_TaskScheduler; 
extern void MDtaskHandleIn1msLoop(void);

extern volatile bool runM0UaCmd;

#if _TIME_TEST_DEFINED
extern s16 Debug1MsTimes;
extern s16 Debug1MsTimesRecord;
#endif
/*******************************************************************************
 �������ƣ�    void Task_Scheduler(void)
 ����������    ��ʱ��Ƭ������Ⱥ���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Task_Scheduler(void)
{
    volatile u32 t_data;

    if (gS_TaskScheduler.bTimeCnt1ms >= TASK_SCHEDU_1MS)
    {   /* 1�����¼���������� */
        gS_TaskScheduler.bTimeCnt1ms = 0;

		MDtaskHandleIn1msLoop();			// MD ��������ʱ���ϸ�ʱ�����жϴ���
		UACtrlObjTask();					/* UA״̬�����ȼ������� */
		MACtrlObjTask();					/* MA״̬�����ȼ������� */

		#if _TIME_TEST_DEFINED				// ʱ�����
		Debug1MsTimesRecord = Debug1MsTimes;
		Debug1MsTimes		= 0;
		#endif
    }

    if (gS_TaskScheduler.nTimeCnt10ms >= TASK_SCHEDU_10MS)
    {   /* 10�����¼���������� */
        gS_TaskScheduler.nTimeCnt10ms = 0;
        Start_speed();
    }

    if (gS_TaskScheduler.nTimeCnt500ms >= TASK_SCHEDU_500MS)
    {   /* 500�����¼���������� */
        gS_TaskScheduler.nTimeCnt500ms = 0;
    }

    if(gS_TaskScheduler.bPWM_UpdateFlg)
    {   /* ÿ��PWM���ڸ���һ�� */
        gS_TaskScheduler.bPWM_UpdateFlg = 0;
    }

}

void Start_speed(void)
	{
if(GPIO_ReadInputDataBit(GPIO2,GPIO_Pin_11)==0)
	{
	runM0UaCmd =1;
}
	if(GPIO_ReadInputDataBit(GPIO2,GPIO_Pin_12)==0)
		{
	runM0UaCmd =0;
}
	}
	
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



