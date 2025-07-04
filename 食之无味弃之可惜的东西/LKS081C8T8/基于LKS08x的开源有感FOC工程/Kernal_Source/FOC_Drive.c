#include "Global_Variable.h"
#include "hardware_config.h"
#include "MC_hall.h"
#include "function_config.h" 
#include "PID_regulators.h"

/*******************************************************************************
 �������ƣ�    void LowPass_Filter_Init(stru_FOC_CtrProcDef *this)
 ����������    ��ͨ�˲���������ʼ��
 ���������    stru_FOC_CtrProcDef *this �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void LowPass_Filter_Init(stru_FOC_CtrProcDef *this)
{
    this->struFOC_CurrLoop.struCurrA_RC.yk_1 = 0;
    this->struFOC_CurrLoop.struCurrA_RC.coef = 47000; /* 1000Hz */
    this->struFOC_CurrLoop.struCurrB_RC.yk_1 = 0;
    this->struFOC_CurrLoop.struCurrB_RC.coef = 47000; /* 1000Hz */

    this->struBusCurr_RC.yk_1 = 0;
    this->struBusCurr_RC.coef = 5000;
}

/*******************************************************************************
 �������ƣ�    void FOC_InitstruParama(void)
 ����������    FOC ��ؿ��Ʊ������ṹ���ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void FOC_InitstruParama(void)
{
	struFOC_CtrProc.struFOC_CurrLoop.bget_offset_flg = 0;
	
    judgement_offset(&struFOC_CtrProc.struFOC_CurrLoop); /* �ж�ƫ���Ƿ����� */
	
    if (struFOC_CtrProc.struFOC_CurrLoop.bget_offset_flg) /* ���������»�ȡ */
    {
        CurrentOffsetCalibration();
    }
	
		/* Q��PI��ʼ�� */
	struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.hKp_Gain = PID_TORQUE_KP_DEFAULT;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.hKp_Divisor = TF_KPDIV;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.hKi_Gain = PID_TORQUE_KI_DEFAULT;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.hKi_Divisor = TF_KIDIV;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.hLower_Limit_Output = -MAX_VQ_LIM; /* Lower Limit for Output limitation */
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.hUpper_Limit_Output = MAX_VQ_LIM; /* Upper Limit for Output limitation */
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.wLower_Limit_Integral = -(MAX_VQ_LIM << TF_KIDIV);
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.wUpper_Limit_Integral = MAX_VQ_LIM << TF_KIDIV;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Torque.wIntegral = 0;

		/* D��PI��ʼ�� */	
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.hKp_Gain = PID_FLUX_KP_DEFAULT;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.hKp_Divisor = FLUX_KPDIV;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.hKi_Gain = PID_FLUX_KI_DEFAULT;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.hKi_Divisor = FLUX_KIDIV;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.hLower_Limit_Output = -MAX_VD_LIM; /* Lower Limit for Output limitation */
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.hUpper_Limit_Output = MAX_VD_LIM; /* Upper Limit for Output limitation */
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.wLower_Limit_Integral = -(MAX_VD_LIM << FLUX_KIDIV);
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.wUpper_Limit_Integral = MAX_VD_LIM << FLUX_KIDIV;
    struFOC_CtrProc.struFOC_CurrLoop.struPI_Flux.wIntegral = 0;
	
	/* Ť���˲���� */
	struFOC_CtrProc.struCur_lowFir_Q.coef = FRAC16(0.3);
    struFOC_CtrProc.struCur_lowFir_Q.yk_1 = 0;
	
	struFOC_CtrProc.max_bus_current = REG32(HALL_LEARN_ADDR + 12);/* ���ĸ�ߵ�����ȡ */
	struFOC_CtrProc.originalmax_bus_current = struFOC_CtrProc.max_bus_current;
	
	struFOC_CtrProc.eSysState = IDLE;/* ϵͳ״̬������ */

    struFOC_CtrProc.bMC_RunFlg = 0;
}

/*******************************************************************************
 �������ƣ�    void Get_PhaseCurrent_AndFilter(stru_FOC_CtrProcDef *this)
 ����������    ��ȡADC�������������ֵ��������б���˲��͵�ͨ�˲�
 ���������    stru_FOC_CtrProcDef *this �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void Get_PhaseCurrent_AndFilter(stru_FOC_CurrLoopDef *this)
{
    s32 t_value;
	s16 hPhaseCurrA, hPhaseCurrB;

    this->StatCurr_a_b_test = SVPWM_2PH_3ShuntGetPhaseCurrentValues(this);

    t_value = this->StatCurr_a_b_test.qI_Value1 - this->struLastStatCurr_a_b.qI_Value1; 
	
    if (ABS(t_value) > 1000)
    {
        hPhaseCurrA = this->struLastStatCurr_a_b.qI_Value1;
        this->bCntFirCurA++;
		
        if (this->bCntFirCurA > 3)
        {
            this->struLastStatCurr_a_b.qI_Value1 = this->StatCurr_a_b_test.qI_Value1;
			hPhaseCurrA = this->struLastStatCurr_a_b.qI_Value1;
            this->bCntFirCurA = 0;
        }
    }
    else
    {
        this->bCntFirCurA = 0;
        this->struLastStatCurr_a_b.qI_Value1 = this->StatCurr_a_b_test.qI_Value1;
		hPhaseCurrA = this->StatCurr_a_b_test.qI_Value1;
    }

    t_value = this->StatCurr_a_b_test.qI_Value2 - this->struLastStatCurr_a_b.qI_Value2;
	
    if (ABS(t_value) > 1000)
    {
        hPhaseCurrB = this->struLastStatCurr_a_b.qI_Value2;
        this->bCntFirCurB++;
		
        if (this->bCntFirCurB > 3)
        {
            this->struLastStatCurr_a_b.qI_Value2 = this->StatCurr_a_b_test.qI_Value2;
			hPhaseCurrB = this->struLastStatCurr_a_b.qI_Value2;
			this->bCntFirCurB = 0;
        }
    }
    else
    {
        this->bCntFirCurB = 0;
        this->struLastStatCurr_a_b.qI_Value2 = this->StatCurr_a_b_test.qI_Value2;
		hPhaseCurrB = this->StatCurr_a_b_test.qI_Value2;
    }

    hPhaseCurrA = lowPass_filter(&this->struCurrA_RC, hPhaseCurrA);
    hPhaseCurrB = lowPass_filter(&this->struCurrB_RC, hPhaseCurrB);
	
	this->struStatCurr_a_b.qI_Value1 = hPhaseCurrA >> 1;
	this->struStatCurr_a_b.qI_Value2 = hPhaseCurrB >> 1;
}

/*******************************************************************************
 �������ƣ�    FOC_CtrProcFilterInit(this)
 ����������    ����������е�ǰ4���˲�����ʼ��
 ���������    stru_FOC_CtrProcDef *this �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void FOC_CtrProcFilterInit(stru_FOC_CurrLoopDef *this)
{
	this->bFirstOpenMosFlgCnt++;
	this->struStatVolt_q_d.qV_Value2 = 0;
	this->struStatVolt_q_d.qV_Value1 = this->struPI_Torque.wIntegral >> this->struPI_Torque.hKi_Divisor;
	this->struLastStatCurr_a_b.qI_Value1 = this->StatCurr_a_b_test.qI_Value1;
	this->struLastStatCurr_a_b.qI_Value2 = this->StatCurr_a_b_test.qI_Value2;

	this->struStatCurr_a_b.qI_Value1 = 0;
	this->struStatCurr_a_b.qI_Value2 = 0;
	this->struCurrA_RC.yk_1 = 0;
	this->struCurrB_RC.yk_1 = 0;
	this->struStatCurr_q_d.qI_Value1 = 0;
	this->struStatCurr_q_d.qI_Value2 = 0;
}
/*******************************************************************************
 �������ƣ�    void FOC_Model(stru_FOC_CtrProcDef *pCtrProc)
 ����������    FOC���л�·
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/06/15      V1.0         LiuH              ����
 *******************************************************************************/
void FOC_Model(stru_FOC_CtrProcDef *pCtrProc)
{
    s16 theatae;
	
	stru_FOC_CurrLoopDef *this;
	
	this = &pCtrProc->struFOC_CurrLoop;
	
	Get_PhaseCurrent_AndFilter(this); /* ���������������е����˲�����*/
	
	if (struHallProcess.bCloseLoopAngleFlg)
	{
		closeLoopAnglePLL(&struHallProcess);
	}
	else
	{
		reduceToqueAnglePll(&struHallProcess);
	}
	
	theatae = GET_ELECTRICAL_ANGLE;	/* ��ȡ�Ƕ� */

		/* �����120�ȵĻ���3����������任��2��Ķ��Ӿ�ֹ����ϵ�� */
    this->struStatCurr_alfa_beta = Clarke(this->struStatCurr_a_b); 
	
		/* ��Ialpha��Ibeta�任����ת��dq����ϵ�� */
    this->struStatCurr_q_d = Park(this->struStatCurr_alfa_beta, theatae, &struFOC_CtrProc.struFOC_CurrLoop);

    if (this->bFirstOpenMosFlgCnt < 3)
    {   /* ����������е�ǰ3���˲�����ʼ�� */
        FOC_CtrProcFilterInit(this);
    }
    else
    {
		s32 t_value;

			/* D�� �ջ�PI���ƣ�Q�Ὺ������ */
		this->struStatVolt_q_d.qV_Value2 = HL_PI_AntiDump((this->nDCur_Reference >> 1),
									  this->struStatCurr_q_d.qI_Value2, &this->struPI_Flux);							
				
		t_value = this->nRequestPower;
		
		if (t_value < 0)
		{
			t_value = 0;
		}

		if (struHallProcess.bMotorDirtionCtrl)
		{
			this->struStatVolt_q_d.qV_Value1 = -t_value;
		}
		else
		{
			this->struStatVolt_q_d.qV_Value1 = t_value;
		}
		
		if(ABS(this->struStatVolt_q_d.qV_Value2) > t_value)
		{
			if(this->struStatVolt_q_d.qV_Value2 > 0)
			{
				this->struStatVolt_q_d.qV_Value2 = t_value;
			}
			else
			{
				this->struStatVolt_q_d.qV_Value2 = -t_value;
			}
		}
    }
	
	/* ��ѹ����Բ����, ��ֹVd Vq���󣬳������������� */
	RevPark_Circle_Limitation(pCtrProc);

	/* ����ת����ϵ�µ�Vd Vq �任����ֹ�����µ�Valpha��Vbeta */
	this->struStatVolt_alfa_beta = Rev_Park(this->struStatVolt_q_d, &struFOC_CtrProc.struFOC_CurrLoop);
	
	/*Valpha Vbeta ����SVPWM�ռ�ʸ������ */
	SVPWM_2PH(pCtrProc);

	/* ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ */
	MCPWM0_RegUpdate(this);
}

/*******************************************************************************
 �������ƣ�    void MCL_Init(stru_FOC_CtrProcDef *this)
 ����������    �������ǰ����������ʼ��
 ���������    stru_FOC_CtrProcDef *this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void MCL_Init(stru_FOC_CtrProcDef *pCtrProc)
{
    s32 t_speed_lim; 
    volatile u16 t_timer;
	
	stru_FOC_CurrLoopDef *this;

    this = &pCtrProc->struFOC_CurrLoop;

    LowPass_Filter_Init(&struFOC_CtrProc);

    t_speed_lim = 0;

    if (t_speed_lim > S16_MAX)
    {
        t_speed_lim = S16_MAX;
    }

    if (struHallProcess.bMotorDirtionCtrl)
    {   
        this->struPI_Torque.wIntegral = -(t_speed_lim << this->struPI_Torque.hKi_Divisor);
    }
    else
    {
        this->struPI_Torque.wIntegral = t_speed_lim << this->struPI_Torque.hKi_Divisor;
    }
	
    this->struPI_Flux.wIntegral = 0;

	this->nRequestPower = 100;

    if (struHallProcess.bCloseLoopAngleFlg == 0)
    {
       this->bFirstOpenMosFlgCnt = 0;
		struHallProcess.bReduceToqueFlg = 0;
		struHallProcess.wHallPWMTimerCnt = 0;
        struHallProcess.bHallRunFlg |= HALL_COM_ERR;
        struHallProcess.nPhaseShift = struHallProcess.nPhaseShiftOffset;
        HALL_Init_Electrical_Angle(&struHallProcess);
        struHallProcess.bFstHallSigFlg = 1;
    }
	
	struFOC_CtrProc.max_bus_current = struFOC_CtrProc.originalmax_bus_current; 

    __disable_irq();
    FOC_Model(&struFOC_CtrProc);
    MCPWM_UPDATE_REG();
    __enable_irq();

    EN_HPWM_FUNC();
    struFOC_CtrProc.nSys_TimerPWM = 0;

    while (struFOC_CtrProc.nSys_TimerPWM < 2)
    {
        ;
    }
}

/*****END OF FILE****/
