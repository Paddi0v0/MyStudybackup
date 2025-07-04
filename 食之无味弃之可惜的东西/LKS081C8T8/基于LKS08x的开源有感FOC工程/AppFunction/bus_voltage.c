#include "Global_Variable.h"
#include "hardware_config.h"

/*******************************************************************************
 �������ƣ�    void VoltageRampControl(stru_FOC_CtrProcDef *pCtrProc)
 ����������    ��ѹб�¿���
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void VoltageRampControl(stru_FOC_CtrProcDef *pCtrProc)
{
	stru_FOC_CurrLoopDef *this;

    this = &pCtrProc->struFOC_CurrLoop;
	
    if (this->nBusVoltagefir > this->nBusVoltage)
    {   /* ��ѹ�������б���źż���*/
        if ((this->nBusVoltagefir - this->nBusVoltage) > VOLATAGE_RAMP)
        {
            this->nBusVoltagefir -= VOLATAGE_RAMP;
        }
        else
        {
            this->nBusVoltagefir--;
        }
    }
    else if (this->nBusVoltagefir < this->nBusVoltage)
    {
        if ((this->nBusVoltage - this->nBusVoltagefir) > VOLATAGE_RAMP)
        {   /* 20180830 HL ���Ƿѹ���ٿ�������� */
            this->nBusVoltagefir += VOLATAGE_RAMP;
        }
        else
        {
            this->nBusVoltagefir++;
        }
    }
}

/*******************************************************************************
 �������ƣ�    void check_Vol_Temp(VoltageProtect_Struct *this)
 ����������    ��Ƿѹ����
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void check_Vol_Temp(VoltageProtect_Struct *this)
{
    if (struFOC_CtrProc.struFOC_CurrLoop.nBusVoltagefir < this->hLowVoltageThd)
    {
        if(this->slowvoltagecnt < 500)
		{
			this->slowvoltagecnt++;
		}
		else
		{
			this->low_error_flg = 1;
		}
    }
    else if (struFOC_CtrProc.struFOC_CurrLoop.nBusVoltagefir > this->hLowVoltageRsmThd)
    {
        this->low_error_flg = 0;
        this->slowvoltagecnt = 0;
    }

    if (struFOC_CtrProc.struFOC_CurrLoop.nBusVoltagefir > this->hOverVoltageThd)
    {
		StopMotorImmdly();
		this->shighvoltagecnt = 0; 
		stru_Faults.B.VoltOverError = 1;
    }
    else if (struFOC_CtrProc.struFOC_CurrLoop.nBusVoltagefir < this->hOverVoltageRsmThd)
    {
        if(stru_Faults.B.VoltOverError)
        {
            if(this->shighvoltagecnt < 1000)
            {
                this->shighvoltagecnt ++;
            }
            else
            {
                this->shighvoltagecnt = 0;
                stru_Faults.B.VoltOverError = 0;
            }
        }
    }

    if (struFOC_CtrProc.struFOC_CurrLoop.nBusVoltage < LOW_VOLATAGE_THD)
    {
        StopMotorImmdly(); /* ����ص���������MOS���� 181012 */
    }
}















