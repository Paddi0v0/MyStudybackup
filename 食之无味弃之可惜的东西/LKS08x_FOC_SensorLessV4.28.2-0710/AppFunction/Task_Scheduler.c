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
#include "Global_Variable.h"
#include "math.h"
#include "Time_Process.h"
#include "hardware_config.h"
#include "fault_detection.h"
#include "speedscan.h"

void Sys_State_Machine(void);
void StartUpProcess(void);
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

    if (struTaskScheduler.bTimeCnt1ms >= TASK_SCHEDU_1MS)
    {   /* 1�����¼���������� */

        struTaskScheduler.bTimeCnt1ms = 0;

        SetTime_CountDown();  /* ������������ʱ���� */

        FaultCheck();        //������

        if(stru_Faults.R != 0)   //���ֹ��������FAULT״̬
        {
            struFOC_CtrProc.eSysState = FAULT;
        }
        Sys_State_Machine(); /* ���0״̬������ */

        DcVoltPerUnitCalc(struFOC_CurrLoop.nBusVoltage);  /* ֱ��ĸ�ߵ�ѹ���ۻ����� */

    }

    if (struTaskScheduler.nTimeCnt10ms >= TASK_SCHEDU_10MS)
    {   /* 10�����¼���������� */
        struTaskScheduler.nTimeCnt10ms = 0;
#if ((ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR) || (HALL2SENSORLESS_ANGLE_VERIFY == TRUE))      
        Hall_learn_Process();   /* Hall��ѧϰ�㷨 */
#endif
//      KEY_Scan();
			  VspSpeedScan(); /*��λ������*/
		}

    if (struTaskScheduler.nTimeCnt500ms >= TASK_SCHEDU_500MS)
    {   /* 500�����¼���������� */
        struTaskScheduler.nTimeCnt500ms = 0;
        
    }

    if(struTaskScheduler.bPWM_UpdateFlg)
    {   /* ÿ��PWM���ڸ���һ�� */
        struTaskScheduler.bPWM_UpdateFlg = 0;
      
#if ((ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR) || (HALL2SENSORLESS_ANGLE_VERIFY == TRUE))                
        Verify_Hall_State(HALL, &struHallProcess);  /* hall ������ */
#endif
			
			
			
      
    }

}



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



