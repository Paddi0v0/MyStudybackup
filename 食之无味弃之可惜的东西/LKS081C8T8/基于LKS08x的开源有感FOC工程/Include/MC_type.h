

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MC_TYPE_H
#define __MC_TYPE_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

/* Exported types ------------------------------------------------------------*/
/*! Macro converting a signed fractional [-1,1) number into a fixed point 16-bit
 * number in the format Q1.15. */
#define FRAC16(x)           ((s16) ((x) < (S16_MAX) ? ((x) >= S16_MIN ? (x)*0x8000 : 0x8000) : 0x7fff))

/*! Macro converting a signed fractional [-1,1) number into a fixed point 32-bit
 * number in the format Q1.31. */
#define FRAC32(x)           ((s32) ((x) < (S32_MAX) ? ((x) >= S32_MIN ? (x)*0x80000000 : 0x80000000) : 0x7fffffff))

#define ABS(X)              ( ( (X) >= 0 ) ? (X) : -(X) )

#define sat(x,ll,ul) ( (x) > (ul) ) ? (ul) : ( ( (x) < (ll) ) ? (ll) : (x) )

#define HALL_SPEED_FIFO_SIZE 	 	((u8)6)		/* Hall�źŴ���Buffer��С��6����ƽ�� */

typedef struct
{
    s16 qI_Value1;
    s16 qI_Value2;
} stru_CurrVoctor;

typedef struct
{
    s16 qV_Value1;
    s16 qV_Value2;
} stru_VoltVoctor;

typedef struct
{
    s16 u;
    s16 v;
    s16 w;
} stru_VoltPhaseUVW; 

typedef struct
{
    s16 hCos;
    s16 hSin;
} stru_TrigComponents;

typedef struct 
{
    s32 Kp;
    s32 Ki;
    s32 Kd;
	
} sPID;

typedef union
{
    u16 R;
    struct
    {
        u16 SoftCurretError : 1; 				/* ������������־λ */
        u16 HardCurretError : 1; 				/* Ӳ�����������־λ */
        u16 VoltOverError   : 1; 				/* ��ѹ�����־λ */
        u16 OffsetError     : 1; 				/* ƫ�ô����־λ */
        u16                 : 12;				/* �������ɸ���������������Ӵ����־λ��ע�ⲻҪ����16λ��������Ҫ�޸�u16 R --> u32 R */
    } B;
} stru_FaultStatus_t;

typedef struct
{
    s16 hKp_Gain;
    u16 hKp_Divisor;
    s16 hKi_Gain;
    u16 hKi_Divisor;
    s16 hLower_Limit_Output;     					//Lower Limit for Output limitation
    s16 hUpper_Limit_Output;     					//Lower Limit for Output limitation
    s32 wLower_Limit_Integral;   					//Lower Limit for Integral term limitation
    s32 wUpper_Limit_Integral;   					//Lower Limit for Integral term limitation
    s32 wIntegral;
} stru_PIparams;

//typedef struct
//{
//    u16 nCofA;      								/* �¶�ϵ��A */
//    u16 nOffsetB;   								/* �¶�ϵ��ƫ�� */
//	volatile s16 Temperture_adc;					/* оƬ�¶�ADCֵ */
//	volatile s16 Temperture;						/* оƬ��ǰ�¶� */
//} Stru_TempertureCof_Def;

typedef struct
{
    s32 yk_1;
    u16 coef;
} stru_RC_Def;


typedef struct
{
    volatile u8 nTimeBaseFlg;        				/* Timer �жϱ�ǣ�ȡֵ 0��1 */
    volatile u8  bPWM_UpdateFlg;      				/* PWM���ڸ��±�־��һ�μ��Ϊһ��PWM���� */
    u8  bTimeCnt1ms;         						/* 1mS������ */
	u8  bTimeCnt2ms;         						/* 2mS������ */
    u16 nTimeCnt10ms;        						/* 10mS������ */
    u16 nTimeCnt500ms;       						/* 500mS������ */
	
} stru_TaskSchedulerDef;

/* ****************************** ת�� ****************************************** */

typedef struct
{
	u8  bgear_shift; 								/* ��λ�л� */
	u16 inching_shift_time;							/* �㶯�л�ȥ�� */
	volatile s16 hand_speed_adc; 					/* ת�ѵ�ѹ */
	s16 hand_speed_adc_fir; 						/* ת�ѵ�ѹ�˲��� */
	s16 hand_out_powre; 							/* ת�����������ֵ */
	volatile s16 actual_out_powre; 					/* ʵ�����������ֵ */
	volatile s16 given_hdcur_ref; 					/* ����������� */
	s16 Proportional_drop_Slope;					/* �ٽ������������ */
	
} stru_speed_control_handle;

/* ****************************** �Ӽ��ٴ��� ****************************************** */

typedef struct
{
	u8  speed_add_slope; 							/* ����б�� */
	u8  speed_dec_slope; 							/* ����б�� */
	u8  second_cut_in; 								/* �������� */
	volatile u16 nclosemosfetcnt; 					/* ��ת�ѹ��������� */
	volatile u16 nsoft_voltage_cnt; 				/* ��Ƿѹ�ص���� */
	volatile u16 ndqnegcnt; 						/* ����ת�Ѹ��������� */
	
} stru_motor_adjust_speed;

/* ****************************** ɲ�� ****************************************** */

typedef struct
{
	u8  break_flg; 									/* ɲ����־ */
	
} stru_break_eabs;

/* ****************************** һ���޸� ****************************************** */

typedef struct
{
	u8  break_hand_err_flg; 						/* ɲ��ת�ѹ��ϱ�־λ */
	u8  power_up_hand_err_flg; 						/* �ϵ���ɳ���־λ */
	u8  onekeyrunflg; 								/* һ���޸���־ */
	u16 in_yjxf_time; 								/* ����һ���޸���ʱʱ�� */
	u16 hand_err_clear_cnt; 						/* ת�ѹ��ϻָ��˲� */
	
} stru_motor_onekeyrun;

/* ****************************** �洢����ת���ܽṹ�� *************************** */

typedef struct
{
	u8  setup_parameters_flg;						/* ����洢��־ */ 
	u8  motorblockflg;								/* �����ת��־ */ 
	u16 smotorblockcnt;								/* ��������תʱ����� */ 
	u16 nInputPort_State;							/* �˿�����״̬ */ 
	s8  bInputCnt[12];								/* �˿�����ȥ�� */ 
	
} stru_motor_comprehensive;

/* ****************************** �ϵ紦��  ****************************************** */

typedef struct
{
	u16 power_up_time; 								/* �ϵ�ʱ����� */ 
	
} stru_power_up;

typedef enum
{
    IDLE       = 0,   								/* ����״̬ */
    INIT       = 1,   								/* ��ʼ��״̬ */
    RUN        = 2,   								/* ��������״̬ */
    STOP       = 3,   								/* ֹͣ״̬ */
    WAIT       = 4,  								/* �ȴ�״̬ */
} enum_SystStatus;

typedef struct
{
	u8  bFstHallSigFlg;                      		/* ��һ��Hall�仯��־������Hall�ź�ȥ�� */
    u32 wOldSensorPeriod;                    		/* �ϴ�Hall�ź��˲�������ֵ */
    u32 wMotorSpeedAvgCnt;                   		/* Hall�ź��˲�������ֵ, ��HAllģ��ʱ�����ڼ��� */
    u32 wSensorPeriod[HALL_SPEED_FIFO_SIZE]; 		/* Hall�ź����ڱ仯���飬�������ź�ƽ��ֵ */
    u8  bSpeedFIFO_Index;                    		/* Hall�ź����ڱ仯����ָ�� */
    volatile u32 wHallPWMTimerCnt;           		/* Hall�仯������PWM�������������������ֵ */
    volatile u16 nElectrical_Angle;             	/* ��ǰת��λ�ýǶ�ֵ��0~65535��Ӧ0~360�� */
    volatile u16 nOldElectrical_Angle;          	/* ��һ��ת��λ�ýǶ�ֵ��0~65535��Ӧ0~360�� */  
    volatile s16 nEndElectricalAngle;           	/* ���Ƕ���������ֵ����Hall�������ʱ�����������һ���ȷ�Ƕ� */
    volatile u16 nTabElectrical_Angle;          	/* ֱ�Ӳ��ȵ��ĵ�ǰת�ӵ�Ƕ�ֵ */
    u16 nRotorFreqDpp;                       		/* ��ǰ���ٶ�ֵ, һ��PWM�����µĽǶ�����ֵ��0~65536��Ӧ0~360�� */
    u16 nReduceToqueDpp;                     		/* ������ģʽ�µ�ǰ���ٶ�ֵ, һ��PWM�����µĽǶ�����ֵ��0~65536��Ӧ0~360�� */
    u16 nReduceToqueAngle;                   		/* ����ģʽ��, �Ƕ����� */
    volatile u32 wHallCapCnt;                   	/* Hallģ�鲶׽�ۼ�ֵ */
    s16 nMaxReduceIncAngle;                  		/* ��Reduce Toqueģʽ�£��Ƕ�������� 0~65536��Ӧ0~360�� */
  
    volatile u8 bHallState;                     	/* ��ǰ�˲����Hall״̬ */

    u8 bMotorDirtionCtrl;                    		/* �������Ƶĵ�����з��� */

    u8 bHallRunFlg;                          		/* Hall����״̬��־�����ϡ����򡢳�ʱ��״̬ HALL_COM_TIMEOUT��
														HALL_COM_FLG��HALL_COM_ERR��HALL_DIR_FLG*/
    s16 ErrCnt;                 	 				/* HALL������� */

    u8 bReduceToqueFlg;                      		/* Reduce Toqueģʽ������־������ٶȹ��ͣ������ڴ�ģʽ */ 
    u8 bFastAddElecAngleFlg;                 		/* ��Hall�������ʱ�����������ۼӵ���ȷ�ĽǶ� */
    u8 bCloseLoopAngleFlg;                   		/* Hall�Ƕ���ϣ�PLL�ջ�������־  */
  
    u16 nPhaseShift;                         		/* ��ǰHall�Ƕȼ���Ƕ�ƫ��ֵ */
    u16 nPhaseShiftOffset;                   		/* ��ǰHall�Ƕ�ƫ��ֵ���� */
    
    u32 wCloseLoopPeriodThd;                 		/* HALL PLL�ջ���������ֵ */
    u32 wOpenLoopPeriodThd;                  		/* HALL PLL������������ֵ */
    
    u16 nMaxIncAngle;                        		/* ���Ƕ����� */

    u16 nHallOffsetArry[6];                  		/* Hallƫ�ƽ����������໷���������Hall��װ����Ļ�е��װƫ�ƽ� */
    
    u8  bHallPossableTab[8];                 		/* Hall������洢��һ��Hallֵ */
    u8  bHallPossableTab2[8];               		/* Hall������洢��һ��Hallֵ */
    u8  bHallCommTab[8];                     		/* Hall����� */
    
    u8 bHallState_org;       						/* Hallԭʼ�ź� */
	
} stru_HallProcessDef;

typedef struct
{
	u8  low_error_flg;  							/* ��ѹ��־ */
	u16 slowvoltagecnt;  							/* ��ѹ�ж�ʱ����� */
	u16 shighvoltagecnt;  							/* ��ѹ�ж�ʱ����� */
    u16 hLowVoltageThd;  							/* ��ѹ��ֵ */
    u16 hLowVoltageRsmThd;  						/* �˳���ѹ��ֵ */
    u16 hOverVoltageThd;  							/* ��ѹ��ֵ */
    u16 hOverVoltageRsmThd;  						/* �˳���ѹ��ֵ */
	u16 nsoft_undervoltage_inter;  					/* ��Ƿѹ���ƻ��� */
} VoltageProtect_Struct;

typedef struct
{
    u16 ADC0_A_B_Cur_First; 						/* ADC0�����ʱ�� */
    u16 ADC0_A_C_Cur_First; 						/* ADC0�����ʱ�� */
    u16 ADC0_B_C_Cur_First; 						/* ADC0�����ʱ�� */
} Stru_ADC_sample;

typedef struct
{
	volatile Stru_ADC_sample Stru_ADC_sampParama;   /* ���ߵ����������� */
	volatile stru_CurrVoctor StatCurr_a_b_test;     /* ���ε������A��B����� */
    volatile stru_CurrVoctor struStatCurr_a_b;      /* �������A��B����� */
    volatile stru_CurrVoctor struLastStatCurr_a_b;  /* ��һ�ε������A��B�����  */
    volatile stru_CurrVoctor struStatCurr_q_d;      /* �������Q��D�����  */
    volatile stru_CurrVoctor struStatCurr_alfa_beta;/* �������alfa beta�����  */

    volatile stru_VoltVoctor struStatVolt_q_d;      /* �������Q��D���ѹ  */
    volatile stru_VoltVoctor struStatVolt_alfa_beta;/* �������alfa beta���ѹ�� */

    volatile stru_TrigComponents struTrigSinCos;    /* ���ӽǶ�sin Cosֵ */

    s16 nBusVoltage;                             	/* ֱ��ĸ�ߵ�ѹ */
	s16 nBusVoltagefir;                          	/* ֱ��ĸ�ߵ�ѹ�˲�ֵ */
	s16 nBusCurrentadc;                     		/* ֱ��ĸ�ߵ�������ֵ */
	s16 nBusCurrentadcfir;                          /* ֱ��ĸ�ߵ����˲�ֵ */
                                                                          
    volatile stru_VoltPhaseUVW struVoltUVW_PWM;     /* UVW PWM�����ѹ */

    volatile u8 bSector;                          	/* ��ǰ���� */

	u8 bget_offset_flg;                          	/* ����ƫ��״̬ */
    s16 nPhaseAOffset;                           	/* A��ADC���� Offsetֵ */
    s16 nPhaseBOffset;                           	/* B��ADC���� Offsetֵ */
	s16 nPhaseBOffset1;                          	/* B��ADC���� Offsetֵ */
    s16 nPhaseCOffset;                           	/* C��ADC���� Offsetֵ */
	s16 nphaseAoffsetShift;                      	/* �������1 A��offset ֵ */
	s16 nphaseCoffsetShift;                      	/* �������1 C��offset ֵ */
	s16 nBusCurrentOffset;                       	/* ĸ��ADC���� Offsetֵ */

    volatile s16 nDCur_Reference;                   /* D��������� */

    volatile s16 nRequestPower;                     /* Q���ѹ���� */

    volatile s8 bCntFirCurA;                        /* A������˲������� */
    volatile s8 bCntFirCurB;                        /* B������˲������� */
    stru_RC_Def struCurrA_RC;                    	/* A�������RC�˲��ṹ��  */
    stru_RC_Def struCurrB_RC;                    	/* B�������RC�˲��ṹ��  */

    u8 bFirstOpenMosFlgCnt;                      	/* ��MOS��ǰ�������ڣ����������ܿ����� */

    stru_PIparams struPI_Torque;                 	/* Q�������PI���� */
    stru_PIparams struPI_Flux;                   	/* D�������PI���� */

} stru_FOC_CurrLoopDef;

typedef struct
{
	volatile u16 nSys_TimerPWM;                  	/* PWM���ڼ���Cnt */

    u8 bMC_RunFlg;                               	/* �������ָ�� */
	
    enum_SystStatus eSysState;                   	/* ��ǰ�������״̬�� */
	
    u16 nSetTimeLeftCnt;                         	/* ״̬���ȴ�ʱ������� */
	
	s16 originalmax_bus_current;              	 	/* ԭʼ���ĸ�ߵ�������ֵ */
    s16 max_bus_current;                      	 	/* ���ĸ�ߵ�������ֵ */
	
	s16 nQVoltageFir; 					 		 	/* Ť�ص�ѹ�˲��� */
	stru_RC_Def struCur_lowFir_Q;                	/* Ť�ص�ѹ�˲��ṹ�� */

    stru_RC_Def struBusCurr_RC;                  	/* ĸ�ߵ���RC�˲��ṹ��  */ 
	
	stru_FOC_CurrLoopDef struFOC_CurrLoop;       	/* �����ڻ��ṹ�� */
	
} stru_FOC_CtrProcDef;

typedef struct
{
    u8 hall_Sequence[8]; 							/* hall���� */
    u8 hallType; 									/* Hall ���� 60�Ȼ�120�� */
    u8 motorType; 									/* ��� ���� 60�ȳ�ǰ��-60�ȳ�ǰ */
    u8 null0[2]; 									/* ��֤�ֳ����� */
    u32 busCurrentLim; 								/* ĸ�ߵ������� | addr 12 byte*/
    u8 version[4]; 									/* Program Version  | addr 16 byte */
    u32 BEMF_coefficient; 							/* ������ϵ��   | addr 20 byte  */
    u32 hall_offsetAngle; 							/* Hall ��ǰ��  | addr 24 byte */
    u32 null; 										/* addr 28 byte */
    u32 null1; 										/* addr 32 Byte */
    u32 null2; 										/* addr 36 Byte */
    u32 phaseAoffset; 								/* A ��offset ֵ */
    u32 phaseBoffset; 								/* B ��offset ֵ */
    u32 phaseCoffset; 								/* C ��offset ֵ */
    u32 phaseBusOffset; 							/* bus ��offset ֵ */
    u32 phaseB1offset; 								/* B1 ��offset ֵ */
    u32 phaseAoffset1; 								/* �������1 A��offset ֵ */
    u32 phaseCoffset1; 								/* �������1 C��offset ֵ */
} FlashParamaStruct_o;

typedef struct
{   
    u8 hall_Sequence[8]; 							/* hall���� */
    u8 hallType; 									/* Hall ���� 60�Ȼ�120�� */
    u8 motorType; 									/* ��� ���� 60�ȳ�ǰ��-60�ȳ�ǰ */
    u8 null0[2]; 									/* ��֤�ֳ����� null0[0] ��ռ��������ٱ�־*/
    u32 busCurrentLim; 								/* ĸ�ߵ������� | addr 12 byte*/
    u8 version[4]; 									/* Program Version  | addr 16 byte */
    u32 BEMF_coefficient; 							/* ������ϵ��   | addr 20 byte  */
    u32 hall_offsetAngle; 							/* Hall ��ǰ��  | addr 24 byte */
    u32 hallBroken_flg; 							/* addr 28 byte zkw flg*/
    u32 null1; 										/* addr 32 Byte */
    u32 null2; 										/* addr 36 Byte */
    u32 phaseAoffset; 								/* A ��offset ֵ */
    u32 phaseBoffset; 								/* B ��offset ֵ */
    u32 phaseCoffset; 								/* C ��offset ֵ */
    u32 phaseBusOffset; 							/* bus ��offset ֵ */
    u32 phaseB1offset; 								/* B1 ��offset ֵ */
    u32 phaseAoffset1; 								/* �������1 A��offset ֵ */
    u32 phaseCoffset1; 								/* �������1 C��offset ֵ */

    u8 buffer[512 - sizeof(FlashParamaStruct_o)];
}FlashParamaStruct;

/* Exported constants --------------------------------------------------------*/

#define U8_MAX     								((u8)255)
#define S8_MAX     								((s8)127)
#define S8_MIN     								((s8)-128)
#define U16_MAX    								((u16)65535u)
#define S16_MAX    								((s16)32767)
#define S16_MIN    								((s16)-32768)
#define U32_MAX    								((u32)4294967295uL)
#define S32_MAX    								((s32)2147483647)
#define S32_MIN    								((s32)-2147483648)


#endif /* __MC_TYPE_H */
/******************* (C) COPYRIGHT 2008 LINKO Semiconductor *****END OF FILE****/
