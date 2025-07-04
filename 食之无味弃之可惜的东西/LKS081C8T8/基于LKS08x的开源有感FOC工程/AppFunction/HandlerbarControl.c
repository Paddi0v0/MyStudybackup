#include "Global_Variable.h"
#include "hardware_config.h"
#include "MC_hall.h"
#include "PID_regulators.h"
#include "function_config.h"

/*******************************************************************************
 �������ƣ�    void speed_ref_ramp(void)
 ����������    ������г���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void speed_ref_ramp(void)
{
    speed_adjustment(); 					/* ������٣��ɿ������ɱջ� */

    speed_adjustment_pi(&struFOC_CtrProc.struFOC_CurrLoop); 	/* �������PI */

    speed_up_down(&struFOC_CtrProc.struFOC_CurrLoop); 			/* �ٶȼӼ� */

    gliding_movement(); 										/* ���д��� */
}

/*******************************************************************************
 �������ƣ�    void speed_adjustment(void)
 ����������    �������б�ʣ��ٶȿ����ջ��л�
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void speed_adjustment(void)
{
    stru_motoradjust_speed.speed_add_slope = POWER_UP_STEP_STRONG;
    stru_motoradjust_speed.speed_dec_slope = 60 - (struHallProcess.nRotorFreqDpp >> 5);

    if(strucontrol_handle.actual_out_powre > 32760)
    {
        strucontrol_handle.actual_out_powre = 32760;
    } 
    else if(strucontrol_handle.actual_out_powre < 0)
    {
        strucontrol_handle.actual_out_powre = 0;
    }
}

/*******************************************************************************
 �������ƣ�    void speed_adjustment_pi(stru_FOC_CurrLoopDef *this)
 ����������    PI����
 ���������    stru_FOC_CurrLoopDef *this �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void speed_adjustment_pi(stru_FOC_CurrLoopDef *this)
{
    if (struFOC_CtrProc.eSysState == RUN)
    {
        struFOC_CtrProc.nQVoltageFir = lowPass_filter(&struFOC_CtrProc.struCur_lowFir_Q,
                                       this->struStatCurr_q_d.qI_Value1);
    }

    this->struPI_Flux.hKi_Gain = PID_FLUX_KI_DEFAULT;

    this->struPI_Flux.hKp_Divisor = FLUX_KPDIV;
    this->struPI_Torque.hKp_Divisor = TF_KPDIV;
}

/*******************************************************************************
 �������ƣ�    void speed_up_down(stru_FOC_CurrLoopDef *this)
 ����������    �Ӽ��ٴ���
 ���������    stru_FOC_CurrLoopDef *this �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void speed_up_down(stru_FOC_CurrLoopDef *this)
{
    if(this->nRequestPower > strucontrol_handle.actual_out_powre) /* ������ٴ��� */
    {
        if(stru_motorcomprehensive.motorblockflg)
        {
            if((this->nRequestPower - strucontrol_handle.actual_out_powre) > 150)
            {
                this->nRequestPower -= 150;
            }
            else
            {
                this->nRequestPower--;
            }
        }

        if((this->nRequestPower - strucontrol_handle.actual_out_powre) > stru_motoradjust_speed.speed_dec_slope)
        {
            if(struHallProcess.bMotorDirtionCtrl == 0)
            {
                if(struFOC_CtrProc.nQVoltageFir < -10)
                {
                    if(struFOC_CtrProc.nQVoltageFir > -150)
                    {
                        this->nRequestPower -= 2;
                    }
                }
                else
                {
                    this->nRequestPower -= stru_motoradjust_speed.speed_dec_slope;
                }
            }
            else
            {
                if(struFOC_CtrProc.nQVoltageFir > 10)
                {
                    this->nRequestPower -= 2;
                }
                else
                {
                    this->nRequestPower -= stru_motoradjust_speed.speed_dec_slope;
                }
            }
        }
        else
        {
            if(this->nRequestPower) this->nRequestPower --;
        }
    }
    else if (this->nRequestPower < strucontrol_handle.actual_out_powre) /* ������ٴ��� */
    {
        s32 t_speed_lim;

        stru_motoradjust_speed.nclosemosfetcnt = 0;

        if(struFOC_CtrProc.bMC_RunFlg == 0)
        {
            t_speed_lim = 0;

            if (t_speed_lim < S16_MAX)
            {
                if((strucontrol_handle.actual_out_powre > t_speed_lim) || ((strucontrol_handle.actual_out_powre < 1000) && (struHallProcess.nRotorFreqDpp < 7)) || (strucontrol_handle.actual_out_powre > 30000))
                {
                    if(strupower_up.power_up_time > 100)
                    {
                        struFOC_CtrProc.bMC_RunFlg = 1;
                    }
                }
            }
        }

        if ((struFOC_CtrProc.eSysState == RUN) || (this->nRequestPower < 100))
        {
            if(this->nBusVoltage > stru_VoltageProtect.hLowVoltageThd + VOLATAGE_0_2V) // 123456
            {
                stru_VoltageProtect.nsoft_undervoltage_inter = 0;

                if((strucontrol_handle.actual_out_powre - this->nRequestPower) > stru_motoradjust_speed.speed_add_slope)
                {
                    this->nRequestPower += stru_motoradjust_speed.speed_add_slope;
                }
                else
                {
                    this->nRequestPower++;
                }
            }
        }
        else
        {
            this->nRequestPower++;
        }
    }
}

/*******************************************************************************
 �������ƣ�    void gliding_movement(void)
 ����������    �����ų���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void gliding_movement(void)
{
    if (strucontrol_handle.actual_out_powre == 0)
    {
        u8 t_flg = 0;

        if(struHallProcess.bMotorDirtionCtrl) /* �����תʱ���ֵ�ת����ֹͣPWM��� */
        {
            if(struHallProcess.bHallRunFlg & HALL_DIR_FLG)
            {
                if((struFOC_CtrProc.struFOC_CurrLoop.nRequestPower - strucontrol_handle.actual_out_powre) > 50)
                {
                    struFOC_CtrProc.struFOC_CurrLoop.nRequestPower -= 50;
                }
                else
                {
                    struFOC_CtrProc.struFOC_CurrLoop.nRequestPower = 0;
                }
            }

            if(struFOC_CtrProc.nQVoltageFir > 0)
            {
                t_flg = 1;
                StopMotorImmdly();
            }
        }
        else
        {
            if((struHallProcess.bHallRunFlg & HALL_DIR_FLG) == 0)
            {
                if((struFOC_CtrProc.struFOC_CurrLoop.nRequestPower - strucontrol_handle.actual_out_powre) > 50)
                {
                    struFOC_CtrProc.struFOC_CurrLoop.nRequestPower -= 50;
                }
                else
                {
                    struFOC_CtrProc.struFOC_CurrLoop.nRequestPower = 0;
                }
            }

            if(struFOC_CtrProc.nQVoltageFir < 0)
            {
                t_flg = 1;
                StopMotorImmdly();
            }
        }

        if ((t_flg) || (struFOC_CtrProc.struFOC_CurrLoop.nBusCurrentadcfir < 300) || (struHallProcess.bMotorDirtionCtrl)) /* ��������� ����ĸ�ߵ���С��300��ʱ�ر���� */
        {
            if (stru_motoradjust_speed.nclosemosfetcnt < 250)
            {
                stru_motoradjust_speed.nclosemosfetcnt++;
            }
            else
            {
                StopMotorImmdly();
            }

            if((stru_motoradjust_speed.nclosemosfetcnt > 20) && (struHallProcess.nRotorFreqDpp > 300))
            {
                StopMotorImmdly();
            }
        }
    }
}

/******************* (C) COPYRIGHT 2015 LINKO SEMICONDUCTOR *****END OF FILE****/
