#include "Global_Variable.h"
#include "time_process.h"
#include "MC_hall.h"
#include "PID_regulators.h"
#include "hardware_config.h"
#include "function_config.h" 

/*******************************************************************************
 �������ƣ�    void HALL_InitHallMeasure( stru_HallProcessDef *this )
 ����������    û��HALL�źŷ���
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void HALL_InitHallMeasure( stru_HallProcessDef *this )  /* û��HALL�źŷ��� */
{
    u8 t_i;
	volatile u32 t_data;
	
	this->nReduceToqueDpp = 0;
	
	this->wCloseLoopPeriodThd = 5000000; //  4000000
		
    this->wOpenLoopPeriodThd = 6500000;  //  5500000

    this->nPhaseShiftOffset = S16_330_PHASE_SHIFT;
	
	t_data = REG32(HALL_LEARN_ADDR); /* ��ȡHALL���� */
    CopyFromBuffer((u8*) & (struHallProcess.bHallCommTab[0]), (u8*)&t_data, 4);
    t_data = REG32(HALL_LEARN_ADDR + 4);
    CopyFromBuffer((u8*) & (struHallProcess.bHallCommTab[4]), (u8*)&t_data, 4);
    t_data = REG32(HALL_LEARN_ADDR + 8);
	
	this->bMotorDirtionCtrl = 0;  /* ��ʼ��������� 0Ϊ��ת */

    hall_comm_VariableInit(&struHallProcess); /* �������ʱHALL�ж� */

    this->bSpeedFIFO_Index = HALL_SPEED_FIFO_SIZE - 1;

    this->bHallRunFlg |= HALL_COM_ERR;
	
	this->wMotorSpeedAvgCnt = ROTOR_SPEED_FACTOR / 3;

    for(t_i = 0; t_i < HALL_SPEED_FIFO_SIZE; t_i++)
    {
        this->wSensorPeriod[t_i] = this->wMotorSpeedAvgCnt;
    }
    
    HALL_Init_Electrical_Angle(&struHallProcess);
    this->bHallRunFlg &= ~HALL_COM_ERR;
}

/*******************************************************************************
 �������ƣ�    void hall_comm_VariableInit(stru_HallProcessDef *this)
 ����������    �������ʱHALL�ж�
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void hall_comm_VariableInit(stru_HallProcessDef *this)
{
    u8 t_i, t_j;

    for (t_i = 1; t_i < 7; t_i++)
    {
        for (t_j = 0; t_j < 6; t_j++)
        {
            if (this->bHallCommTab[t_j] == t_i)
            {
                if (t_j != 5)
                {
                    this->bHallPossableTab[t_i] = this->bHallCommTab[t_j + 1];
                }
                else
                {
                    this->bHallPossableTab[t_i] = this->bHallCommTab[0];
                }
				
                break;
            }
        }

        for (t_j = 0; t_j < 6; t_j++)
        {
            if (this->bHallCommTab[t_j] == t_i)
            {
                if (t_j == 0)
                {
                    this->bHallPossableTab2[t_i] = this->bHallCommTab[5];
                }
                else
                {
                    this->bHallPossableTab2[t_i] = this->bHallCommTab[t_j - 1];
                }
				
                break;
            }
        }
    }
}

/*******************************************************************************
 �������ƣ�    void HALL_Init_Electrical_Angle(stru_HallProcessDef *this)
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void HALL_Init_Electrical_Angle(stru_HallProcessDef *this)
{
	if((this->wMotorSpeedAvgCnt < this->wCloseLoopPeriodThd))
    {   /* �ٶȴ��ڱջ����ޣ���ͨ�ջ����� */
        this->bCloseLoopAngleFlg = 1;

		if(this->bHallRunFlg & HALL_DIR_FLG)
		{
			this->nPhaseShift = this->nPhaseShiftOffset - S16_30_PHASE_SHIFT;
		}
		else
		{   /* �����ת���� */
			this->nPhaseShift = this->nPhaseShiftOffset + S16_30_PHASE_SHIFT;
		}
    }
    else
    {
        /* �ٶ�С�ڿ������ޣ���ͨ�������� */
        this->bCloseLoopAngleFlg = 0;
        this->nPhaseShift = this->nPhaseShiftOffset;
    }

    if(this->bHallState == this->bHallCommTab[0])
    {
        this->nTabElectrical_Angle = S16_60_PHASE_SHIFT;
    }
    else if(this->bHallState == this->bHallCommTab[1])
    {
        this->nTabElectrical_Angle = S16_120_PHASE_SHIFT;
    }
    else if(this->bHallState == this->bHallCommTab[2])
    {
        this->nTabElectrical_Angle = S16_180_PHASE_SHIFT;
    }
    else if(this->bHallState == this->bHallCommTab[3])
    {
        this->nTabElectrical_Angle = S16_240_PHASE_SHIFT;
    }
    else if(this->bHallState == this->bHallCommTab[4])
    {
        this->nTabElectrical_Angle = S16_300_PHASE_SHIFT;
    }
    else if(this->bHallState == this->bHallCommTab[5])
    {
        this->nTabElectrical_Angle = 0;
    }

    this->nTabElectrical_Angle += this->nPhaseShift;

    if(this->bCloseLoopAngleFlg)
    {
        if(this->wMotorSpeedAvgCnt > this->wOpenLoopPeriodThd)
        {
            this->nElectrical_Angle = this->nTabElectrical_Angle;
        }

        closeLoopAnglePLLInit(&struHallProcess);
        this->nMaxIncAngle = 0;
    }
    else
    {
        this->nRotorFreqDpp = ROTOR_SPEED_FACTOR / this->wMotorSpeedAvgCnt;

        if(this->bHallRunFlg & HALL_COM_ERR)
        {   /* �������ֱ�Ӹ����Ƕ� */
            this->nElectrical_Angle = this->nTabElectrical_Angle;
            this->bReduceToqueFlg = 0;
        }
        else
        {   /* ��������ʱ���������ջ���� */
            calc_first_ElectAngle(this->nTabElectrical_Angle, &struHallProcess);
        }
    }
}

/*******************************************************************************
 �������ƣ�    void closeLoopAnglePLLInit(stru_HallProcessDef *this)
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void closeLoopAnglePLLInit(stru_HallProcessDef *this)
{
    s16 t_detAngle;
    s32 t_RotorSpeedFactor;

    if (this->bMotorDirtionCtrl)
    {
        t_detAngle = this->nOldElectrical_Angle - this->nTabElectrical_Angle;
    }
    else
    {
        t_detAngle = this->nTabElectrical_Angle - this->nOldElectrical_Angle;
    }

    if (ABS(t_detAngle) > S16_20_PHASE_SHIFT)
    {
		this->nElectrical_Angle = this->nTabElectrical_Angle;
        this->nRotorFreqDpp = ROTOR_SPEED_FACTOR / this->wMotorSpeedAvgCnt;
    }
	else
	{
		t_RotorSpeedFactor = t_detAngle * ROTOR_SPEED_FACTOR_PER_UNIT;
		t_RotorSpeedFactor += ROTOR_SPEED_FACTOR;
		this->nRotorFreqDpp = ((t_RotorSpeedFactor / this->wMotorSpeedAvgCnt) + this->nRotorFreqDpp) >> 1;
	}
}

/*******************************************************************************
 �������ƣ�    void calc_first_ElectAngle(u16 angle, stru_HallProcessDef *this)
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void calc_first_ElectAngle(u16 angle, stru_HallProcessDef *this)
{
    u16 t_hElectrical_Angle;

    t_hElectrical_Angle = angle;
	
	this->nReduceToqueAngle = 0;
    this->nMaxReduceIncAngle = S16_60_PHASE_SHIFT;

    if (this->bMotorDirtionCtrl)
    {   /* �����ת���� */
        if (this->bHallRunFlg & HALL_DIR_FLG)
        {   /* ���ʵ����ת���Ƕ������ۼ� */
            this->nElectrical_Angle = t_hElectrical_Angle - S16_60_PHASE_SHIFT;
			this->bReduceToqueFlg = REDUCE_TOQUE_PUSE;
        }
        else
        {   /* ���ʵ����ת���Ƕȵݼ� */
            this->nElectrical_Angle = t_hElectrical_Angle + S16_60_PHASE_SHIFT;
            this->bReduceToqueFlg = REDUCE_TOQUE_MINUS;
        }
    }
    else
    {   /* �����ת���� */
        if (this->bHallRunFlg & HALL_DIR_FLG)
        {   /* �����ת */
            this->bReduceToqueFlg = REDUCE_TOQUE_PUSE;
            this->nElectrical_Angle = t_hElectrical_Angle - S16_60_PHASE_SHIFT;
        }
        else
        {
            this->bReduceToqueFlg = REDUCE_TOQUE_MINUS;
            this->nElectrical_Angle = t_hElectrical_Angle + S16_60_PHASE_SHIFT;
        }
    }
}

/*******************************************************************************
 �������ƣ�    s16 HALL_GetElectricalAngle(stru_HallProcessDef *this)
 ����������    �����ǶȻ�ȡ
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/

s16 HALL_GetElectricalAngle(stru_HallProcessDef *this)
{
    return(this->nElectrical_Angle);
}

/*******************************************************************************
 �������ƣ�    u8 check_hall_state(u8 t_hall, stru_HallProcessDef *this)
 ����������    ��ѯ����״̬
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
u8 check_hall_state(u8 t_hall, stru_HallProcessDef *this)
{
    u8 t_ReturnFlg;

    t_ReturnFlg = 1;
	
	if ((t_hall == 0) || (t_hall == 7))
	{
		this->ErrCnt++;
		this->bHallRunFlg = HALL_COM_ERR;
		
		if (this->ErrCnt > 3)
		{
			this->ErrCnt = 0;
			this->bHallState = t_hall;
		}

		t_ReturnFlg = 0;
	}
	else
	{
		if (t_hall == this->bHallPossableTab[this->bHallState])
		{	 
			this->ErrCnt = 0;
			
			this->bHallState = t_hall;

			this->bHallRunFlg &= ~HALL_COM_ERR;

			this->bHallRunFlg |= HALL_DIR_FLG;
		}
		else if (t_hall == this->bHallPossableTab2[this->bHallState])
		{	
			this->ErrCnt = 0;
			
			this->bHallState = t_hall;
			
			this->bHallRunFlg &= (~HALL_COM_ERR);
			
			this->bHallRunFlg &= (~HALL_DIR_FLG);
		}	

		this->bHallState_org = t_hall;
	}

    return t_ReturnFlg;
}

/*******************************************************************************
 �������ƣ�    void HALL_IRQHandler(void)
 ����������    Hall�жϴ�����
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void HALL_IRQProcess(stru_HallProcessDef *this)
{
    unsigned char temp_state, t_FuncState;
    volatile u8 t_i, t_cnt;

    if(HALL_INFO & BIT16)
    {   /* Hall �仯�ж� */
        this->wHallCapCnt += HALL_WIDTH;
        HALL_INFO = BIT16;
 
        temp_state = ReadHallState();

		/* ����Ϊ0�Ǹ��Ŵ����࣬��Ϊ0���������� */ 
        t_FuncState = check_hall_state(temp_state, &struHallProcess);

        if(t_FuncState)
        {
            hall_sensor_period(&struHallProcess);

            if(this->bFstHallSigFlg)
            {
                this->bFstHallSigFlg = 0;
            }
            else
            {
				if(this->wHallCapCnt < MAX_SPEED_DPP)
				{
					this->wHallCapCnt = MAX_SPEED_DPP;
				}

                this->wSensorPeriod[this->bSpeedFIFO_Index] = this->wHallCapCnt;
            }

            this->wHallCapCnt = 0;
            this->wHallPWMTimerCnt = 0;

            this->wMotorSpeedAvgCnt = GetAvrgHallPeriod(&struHallProcess);

            HALL_Init_Electrical_Angle(&struHallProcess);
        }
    }

    if(HALL_INFO & BIT17)
    {   /* Hall��ʱ�ж� */
        HALL_INFO = BIT17;

        this->bHallRunFlg |= HALL_COM_ERR;

        this->bSpeedFIFO_Index = 0;
		
		this->wMotorSpeedAvgCnt = ROTOR_SPEED_FACTOR / 3;

        for(t_i = 0; t_i < HALL_SPEED_FIFO_SIZE; t_i++)
        {
            this->wSensorPeriod[t_i] = this->wMotorSpeedAvgCnt;
        }

        this->nRotorFreqDpp = 3;

        temp_state = ReadHallStateMix(); 

        t_FuncState = check_hall_state(temp_state, &struHallProcess);

        HALL_Init_Electrical_Angle(&struHallProcess);

        this->bCloseLoopAngleFlg = 0;
    }

    HALL_INFO = 0;
}

/*******************************************************************************
 �������ƣ�    u32 GetAvrgHallPeriod(stru_HallProcessDef *this)
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
u32 GetAvrgHallPeriod(stru_HallProcessDef *this)
{
    u32 wAvrgBuffer;

    wAvrgBuffer = 0;

    wAvrgBuffer = (this->wOldSensorPeriod + this->wSensorPeriod[this->bSpeedFIFO_Index]) >> 1;
		
	if(wAvrgBuffer < ROTOR_SPEED_FACTOR >> 11)
	{
		wAvrgBuffer = ROTOR_SPEED_FACTOR / 3;
	}
	
    this->wOldSensorPeriod = this->wSensorPeriod[this->bSpeedFIFO_Index];

    return (wAvrgBuffer);
}

/*******************************************************************************
 �������ƣ�    void closeLoopAnglePLL(stru_HallProcessDef *this)
 ����������    ��ѹб�¿���
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void closeLoopAnglePLL(stru_HallProcessDef *this)
{
    if (this->bCloseLoopAngleFlg)
    {
        if (this->bMotorDirtionCtrl)
        {   /* �����ת */
            if (this->nMaxIncAngle < S16_75_PHASE_SHIFT)
            {
                this->nMaxIncAngle += this->nRotorFreqDpp;
                this->nElectrical_Angle -= this->nRotorFreqDpp;
            }
        }
        else
        {   /* �����ת */
            if (this->nMaxIncAngle < S16_75_PHASE_SHIFT)
            {
                this->nMaxIncAngle += this->nRotorFreqDpp;
                this->nElectrical_Angle += this->nRotorFreqDpp;
            }
        }

        this->nOldElectrical_Angle = this->nElectrical_Angle;
    }
}

/*******************************************************************************
 �������ƣ�    reduceToqueAnglePll(stru_HallProcessDef *this)
 ����������    ��СŤ�ز���ʱ��ɽǶȵ��ۼӻ�ݼ�
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2014/9/19      V1.0           Howlet Li          ����
 *******************************************************************************/
void reduceToqueAnglePll(stru_HallProcessDef *this)
{
    if ((this->bCloseLoopAngleFlg == 0) && this->bReduceToqueFlg)
    {
        if (this->nReduceToqueAngle < this->nMaxReduceIncAngle)
        {
            /* ȷ���Ƕ��ۼӲ��������� */
            this->nReduceToqueAngle += this->nReduceToqueDpp;

            if (this->bReduceToqueFlg == REDUCE_TOQUE_MINUS)
            {
                this->nElectrical_Angle -= this->nReduceToqueDpp;
            }
            else
            {
                this->nElectrical_Angle += this->nReduceToqueDpp;
            }
        }
        else
        {
            this->bReduceToqueFlg = 0;
        }
    }
}

/*******************************************************************************
 �������ƣ�    void Verify_Hall_State(stru_HallProcessDef *this)
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void Verify_Hall_State(stru_HallProcessDef *this)
{
    u8 temp_state1, temp_state;
    u8 t_cnt = 0, t_i;
    u8 t_funcState;
    u32 t_CapCnt;

    temp_state = ReadHallStateMix();

    if(temp_state != this->bHallState_org)
	{
		for (t_i = 0; t_i < 4; t_i++)
		{
			temp_state1 = ReadHallStateMix(); /* ������״̬ */

			if (temp_state == temp_state1)
			{
				t_cnt++;
			}
		}

		if (t_cnt > 2)
		{
			NVIC_DisableIRQ (HALL_IRQn);
			t_funcState = check_hall_state(temp_state, &struHallProcess); /* ��ѯ����״̬���� */
			
			if (t_funcState)
			{
				hall_sensor_period(&struHallProcess);

				t_CapCnt = this->wHallPWMTimerCnt * ROTOR_SPEED_FACTOR_PER_UNIT;
				this->wSensorPeriod[this->bSpeedFIFO_Index] = t_CapCnt;

				this->wMotorSpeedAvgCnt = GetAvrgHallPeriod(&struHallProcess);

				this->wHallPWMTimerCnt = 0;
				HALL_Init_Electrical_Angle(&struHallProcess);
			}
			
			NVIC_EnableIRQ (HALL_IRQn);
		}
	}
}

/*******************************************************************************
 �������ƣ�    void Hall_ElecAnglePWM_Process(stru_HallProcessDef *this)
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void Hall_ElecAnglePWM_Process(stru_HallProcessDef *this)
{
	if(this->bFastAddElecAngleFlg)
    {
        u16 t_addAngleStep;

        t_addAngleStep = COMM_PER_TIME_5MS;

        if(this->nEndElectricalAngle  > 0)
        {
            if(this->nEndElectricalAngle  > t_addAngleStep)
            {
                this->nEndElectricalAngle  -= t_addAngleStep;
                this->nElectrical_Angle += t_addAngleStep;
            }
            else
            {
                this->bFastAddElecAngleFlg = 0;
            }
        }
        else
        {
            if(this->nEndElectricalAngle  < -t_addAngleStep)
            {
                this->nEndElectricalAngle += t_addAngleStep;
                this->nElectrical_Angle -= t_addAngleStep;
            }
            else
            {
                this->bFastAddElecAngleFlg = 0;
            }
        }
    }
}

/*******************************************************************************
 �������ƣ�    u8 ReadHallStateMix(void)
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
u8 ReadHallStateMix(void)
{
    u8 t_hall;

    t_hall = ReadHallState();
	
    return t_hall;
}

/*******************************************************************************
 �������ƣ�    void hall_sensor_period(stru_HallProcessDef *this) 
 ����������    
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void hall_sensor_period(stru_HallProcessDef *this) 
{
	if (this->bSpeedFIFO_Index < (HALL_SPEED_FIFO_SIZE - 1))
	{
		this->bSpeedFIFO_Index++;
	}
	else
	{
		this->bSpeedFIFO_Index = 0;
	}
}

/*******************************************************************************
 �������ƣ�    u8 ReadHallState(void)
 ����������    ��ȡ��ǰ�����ź�
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
u8 ReadHallState(void)
{
    volatile u8 ReadValue;
	
	ReadValue = HALL_INFO & 0x07;

    return(ReadValue);
}

/******************* (C) COPYRIGHT 2008 LINKO Semiconductor *****END OF FILE****/
