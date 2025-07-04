#include "Global_Variable.h"
#include "time_process.h"
#include "function_config.h"
#include "hardware_config.h"

/*******************************************************************************
 �������ƣ�    void Sys_State_Machine(void)
 ����������    ϵͳ״̬��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/16      V1.0           Howlet Li          ����
 *******************************************************************************/

void Sys_State_Machine(void)
{
    switch (struFOC_CtrProc.eSysState)
    {
    case IDLE:
    {
        if ((struFOC_CtrProc.bMC_RunFlg) && (stru_Faults.R == 0) && (strubreak_eabs.break_flg == 0))
        {
            struFOC_CtrProc.eSysState = INIT;
        }

        struFOC_CtrProc.nQVoltageFir = 0;
        break;
    }

    case INIT:
    {
        MCL_Init(&struFOC_CtrProc);
        PWMOutputs(ENABLE);
        struFOC_CtrProc.eSysState = RUN;
        break;
    }

    case RUN:
    {
        if ((struFOC_CtrProc.bMC_RunFlg == 0) || (stru_Faults.R))
        {
            struFOC_CtrProc.eSysState = STOP;
        }

        break;
    }

    case STOP:
    {
        StopMotorImmdly();
        struFOC_CtrProc.eSysState = WAIT;
        SetTime_TimeOut_Counter(300);
        break;
    }

    case WAIT:
    {
        if (SetTime_IsElapsed() == TRUE)
        {
            struFOC_CtrProc.eSysState = IDLE;
        }

        break;
    }

    default:
        break;
    }
}

/*******************************************************************************
 �������ƣ�    Task_Scheduler(void)
 ����������    ������Ⱥ���
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/2/25      V1.0           Howlet Li          ����
 *******************************************************************************/

void Task_Scheduler(void)
{
    if (struTaskScheduler.nTimeBaseFlg)
    {
        struTaskScheduler.nTimeBaseFlg = 0;

        if (++struTaskScheduler.bTimeCnt1ms == TASK_SCHEDU_1MS) 			/* һ�����һ�� */
        {
            struTaskScheduler.bTimeCnt1ms = 0;

            Speed_Power_Pro(&strucontrol_handle); 							/* ת�Ѵ��� */

			power_zero_Pro(); 												/* ����Ϊ0 */

			break_state_Pro(); 												/* ɲ�� */

			speed_ref_ramp(); 												/* ������д��� */

            Sys_State_Machine(); 											/* ϵͳ״̬�� */

            if (strupower_up.power_up_time > 10)
            {
                key_in_proc(); 												/* �ⲿ���ܶ˿� */

                check_Vol_Temp(&stru_VoltageProtect); 						/* ��ѹ��� */
            }
        }

        if (++struTaskScheduler.nTimeCnt10ms == TASK_SCHEDU_10MS)
        {
            struTaskScheduler.nTimeCnt10ms = 0;

            Motor_Block_Protect(); 											/* ��ת�������� */

            Power_Up_Init(); 												/* �ϵ���ʼ���봦�� */
			
            if (MCPWM_EIF & FAIL0_ERROR)
            {
                MCPWM_EIF = 0xffff;
                stru_Faults.B.HardCurretError = 1;
            }

            VoltageRampControl(&struFOC_CtrProc);							/* ĸ�ߵ�ѹ */

            Flash_Write_Pro(); 												/* flash�洢 */
        }
    }
    else
    {
        if (struTaskScheduler.bPWM_UpdateFlg)
        {
            struTaskScheduler.bPWM_UpdateFlg = 0;

            Verify_Hall_State(&struHallProcess);
        }
    }
}




