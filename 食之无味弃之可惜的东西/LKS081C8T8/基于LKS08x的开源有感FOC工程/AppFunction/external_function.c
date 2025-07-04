#include "Global_Variable.h"
#include "hardware_config.h"
#include "function_config.h" 

/*******************************************************************************
 �������ƣ�    void key_in_proc(void) 
 ����������    �ⲿ���ܳ���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void key_in_proc(void)
{
	key_capture(&stru_motorcomprehensive);  	/* �˿�����ȥ�� */
	
	motor_break();								/* ɲ�� */

	Power_Options();							/* ������ѹ */
}

/*******************************************************************************
 �������ƣ�    void motor_break(void) 
 ����������    ɲ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void motor_break(void)
{
	if((stru_motorcomprehensive.nInputPort_State & BREAK_INPUT) == 0)		
	{   	
		strubreak_eabs.break_flg = 1;
	}
	else
	{	
		strubreak_eabs.break_flg = 0;
	}
}

/*******************************************************************************
 �������ƣ�    void Power_Options(void) 
 ����������    ������ѹ����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void Power_Options (void)
{
    if(strupower_up.power_up_time < 100)/* �ϵ�1�����жϵ�ѹѡ�� */
    {
		stru_VoltageProtect.hLowVoltageThd = VOLATAGE_42V;
		stru_VoltageProtect.hOverVoltageThd = VOLATAGE_62V;
		stru_VoltageProtect.hOverVoltageRsmThd = VOLATAGE_60V;
		stru_VoltageProtect.hLowVoltageRsmThd = stru_VoltageProtect.hLowVoltageThd + VOLATAGE_0_5V;
    }
}

/*******************************************************************************
 �������ƣ�    void key_capture(void) 
 ����������    ��ť��������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void key_capture(stru_motor_comprehensive *this)
{
	if(BREAK_INPUT_MASK) 		
	{
		if(this->bInputCnt[1] < 0) this->bInputCnt[1] = 0;
		
		if(this->bInputCnt[1] < KEYIN_FILTER_TIME)
		{
			this->bInputCnt[1]++;
		}
		else
		{
			this->nInputPort_State |= BIT1;
		}
	}
	else
	{
		if(this->bInputCnt[1] > 0) this->bInputCnt[1] = 0;
		
		if(this->bInputCnt[1] > KEYIN_FILTER_TIME1)
		{
			this->bInputCnt[1]--;
		}
		else
		{
			this->nInputPort_State &= ~BIT1;
		}
	}
}












