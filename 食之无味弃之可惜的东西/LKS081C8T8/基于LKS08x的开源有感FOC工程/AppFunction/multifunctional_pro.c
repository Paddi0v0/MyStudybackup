#include "Global_Variable.h"
#include "hardware_config.h"
#include "function_config.h" 

/*******************************************************************************
 �������ƣ�    void functional_variables_init(void)
 ����������    ������ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void functional_variables_init(void)
{
	stru_motorcomprehensive.nInputPort_State = 0xffff;
	
	strupower_up.power_up_time = 0;
}

/*******************************************************************************
 �������ƣ�    void Speed_Power_Pro(void)
 ����������    ת��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void Speed_Power_Pro (stru_speed_control_handle *this)
{
    u16 t_power = 0, t_power1 = 0;

    this->hand_speed_adc_fir = (this->hand_speed_adc_fir * 7 + this->hand_speed_adc) >> 3;

    if((this->hand_speed_adc > HBSPD_OVERHIGH) || (this->hand_speed_adc < HBSPD_LOW))
    {
        this->hand_speed_adc_fir = 0;
        stru_motoronekeyrun.break_hand_err_flg |= BIT0;
        stru_motoronekeyrun.hand_err_clear_cnt = 0;
    }
    else
    {
        if(stru_motoronekeyrun.break_hand_err_flg & BIT0)
        {
            if(stru_motoronekeyrun.power_up_hand_err_flg & BIT0)
            {
                if((this->hand_speed_adc > HBSPD_LOW) && (this->hand_speed_adc < HBSPD_BASE))
                {
                    if(stru_motoronekeyrun.hand_err_clear_cnt < 100)
                    {
                        stru_motoronekeyrun.hand_err_clear_cnt++;
                    }

                    if(stru_motoronekeyrun.hand_err_clear_cnt == 100)
                    {
                        stru_motoronekeyrun.power_up_hand_err_flg &= ~BIT0;
                        stru_motoronekeyrun.hand_err_clear_cnt = 0;
                    }
                }
            }
            else
            {
                if((this->hand_speed_adc > HBSPD_LOW) && (this->hand_speed_adc < HBSPD_OVERHIGH))
                {
                    if(stru_motoronekeyrun.hand_err_clear_cnt < 100)
                    {
                        stru_motoronekeyrun.hand_err_clear_cnt++;
                    }

                    if(stru_motoronekeyrun.hand_err_clear_cnt == 100)
                    {
                        stru_motoronekeyrun.break_hand_err_flg &= ~BIT0;
                        stru_motoronekeyrun.hand_err_clear_cnt = 0;
                    }
                }
            }
        }
        else
        {
            stru_motoronekeyrun.hand_err_clear_cnt = 0;
        }
    }

	t_power1 = this->hand_speed_adc_fir;

    if(t_power1 >= HBSPD_MAX_EFFECTIVE) 
    {
        t_power1 = HBSPD_EFFECTIVE_AMPLITUDE;
    }
    else if(t_power1 <= HBSPD_BASE)
    {
        t_power1 = 0;
    }
    else
    {
        t_power1 = t_power1 - HBSPD_BASE;
    }

	t_power = (THIRD_GEAR * t_power1) >> 7;

    if (t_power >= 32760)
    {
        this->hand_out_powre = 32760;
		
		this->given_hdcur_ref = 32760 - t_power;
    }
    else
    {
        this->given_hdcur_ref = 0;
        this->hand_out_powre = t_power;
    }
	
	this->actual_out_powre = strucontrol_handle.hand_out_powre;
}

/*******************************************************************************
 �������ƣ�    void break_state_Pro(void)
 ����������    ɲ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void break_state_Pro(void)
{
    if(strubreak_eabs.break_flg) 
    {
		strucontrol_handle.given_hdcur_ref = 0;
        strucontrol_handle.actual_out_powre = 0;
    }
}

/*******************************************************************************
 �������ƣ�    void power_zero_Pro(void)
 ����������    ���ƶ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void power_zero_Pro(void)
{
    if ((strucontrol_handle.actual_out_powre == 0) && (strucontrol_handle.hand_speed_adc < HBSPD_BASE) && (strupower_up.power_up_time > 120))
    {
		stru_Faults.B.SoftCurretError = 0;
		stru_Faults.B.HardCurretError = 0;
		stru_motorcomprehensive.motorblockflg = 0;
    }

    if (stru_motorcomprehensive.motorblockflg)
    {
        if (strucontrol_handle.actual_out_powre == 0)
        {
            stru_motorcomprehensive.motorblockflg = 0;
        }

        strucontrol_handle.actual_out_powre = 0;
    }
	
	if(stru_Faults.R)
    {
        strucontrol_handle.actual_out_powre = 0;
    }
	
	if(stru_motoronekeyrun.break_hand_err_flg)  
    {
        strucontrol_handle.actual_out_powre = 0;
    }
}

/*******************************************************************************
 �������ƣ�    void Motor_Block_Protect(void)
 ����������    ��ת����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void Motor_Block_Protect(void)
{
    if ((struFOC_CtrProc.eSysState == RUN) && (strucontrol_handle.actual_out_powre > 10000))
    {
        if((struHallProcess.nRotorFreqDpp < 40) && (struFOC_CtrProc.struFOC_CurrLoop.nBusCurrentadcfir > CURRENT_ADC_PER_A * 2))
        {
            if(stru_motorcomprehensive.smotorblockcnt > TIME_1S6_BASE)
            {
                stru_motorcomprehensive.motorblockflg = 1;
            }
            else
            {
                stru_motorcomprehensive.smotorblockcnt++; 
            }
        }
        else
        {
            stru_motorcomprehensive.smotorblockcnt = 0;
        }
    }
    else
    {
        stru_motorcomprehensive.smotorblockcnt = 0;
    }
}

/*******************************************************************************
 �������ƣ�    void StopMotorImmdly(void)
 ����������    �رն�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void StopMotorImmdly(void)
{
    PWMOutputs(DISABLE); 
	
    struFOC_CtrProc.bMC_RunFlg = 0;
	
    if (struFOC_CtrProc.eSysState == RUN)
    {
        struFOC_CtrProc.eSysState = STOP;
    }
	
    struFOC_CtrProc.struFOC_CurrLoop.nRequestPower = 0;
    struFOC_CtrProc.nQVoltageFir = 0;
}




