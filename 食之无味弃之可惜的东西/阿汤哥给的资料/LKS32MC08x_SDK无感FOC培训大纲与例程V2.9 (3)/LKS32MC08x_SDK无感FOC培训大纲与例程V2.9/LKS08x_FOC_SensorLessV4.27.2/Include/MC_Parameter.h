/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� parameter.h
 * �ļ���ʶ��
 * ����ժҪ�� parameter config
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Andrew kong, Howlet Li
 * ������ڣ� 2020��8��18��
 *
 *******************************************************************************/
 
/*------------------------------prevent recursive inclusion -------------------*/ 
#ifndef __PARAMETER_H
#define __PARAMETER_H

#include "basic.h"
#include "MC_Type.h"

/* -----------------------------Hardware Parameter---------------------------- */
#define ADC_SUPPLY_VOLTAGE             (3.6)               /* ��λ: V  ADC��׼��ѹ��3.6����2.4,�󲿷�Ӧ��ѡ��3.6 */
/*����ƥ����� 1Kŷ�� 200:10=(200/(10.4+2)=16.129 */
/*����ƥ����� 40Kŷ�� 200:10=(200/(10.4+40)=3.96825 */
#define AMPLIFICATION_GAIN             (16.129)            /* �˷ŷŴ��� */  
#define RSHUNT                         (0.005)             /* ��λ: ��  ����������ֵ */
#define VOLTAGE_SHUNT_RATIO            (1.0/(20.0 * 2 + 1.0)) /* ĸ�ߵ�ѹ��ѹ�� */
#define BEMF_SHUNT_RATIO               (1.0/(20.0 * 2 + 1.0)) /* �����Ƶ�ѹ��ѹ�� */

#define CURRENT_SAMPLE_TYPE            CURRENT_SAMPLE_2SHUNT /* ����������ʽѡ�� */

#define ROTOR_SENSOR_TYPE              ROTOR_SENSORLESS       /* ���λ�ô���������ѡ�� */

/* ------------------------------Rated Parameter------------------------------ */

#define U_RATED_VOLTAGE                (24)               /* ��λ:V, ����������ѹ������Ϊ����������ѹ */
#define U_RATED_CURRENT                (6.0)              /* ��λ:A, ����������������������ֵ */
#define U_MAX_FREQ                     (600.0)            /* ��λ:Hz,  ����������ת�� */

/* ------------------------------Motor Parameter------------------------------ */
#define U_MOTOR_PP                     (5.0)              /* ��������� */
#define U_MOTOR_RS                     (1.0)              /* ��λ: �� �������� */
#define U_MOTOR_LD                     (2500.0)           /* ��λ: uH ���d���� */
#define U_MOTOR_LQ                     (2600.0)           /* ��λ: uH ���q���� */

/* ����������� ���㹫ʽ��Vpp/2/sqrt(3)/(2*PI)/f������VppΪ��ѹ���ֵ��fΪ��Ƶ�� 3.68V 25Hz
   �˲�����Ӱ��˳����������ٶȼ�⣬�Ƕȹ��㲻ʹ��Щ���� */
#define U_MOTOR_FLUX_CONST             (0.006763)       

/* ----------------------IPD ת�ӳ�ʼλ�ü�� ����ע��ʱ������---------------- */
#define SEEK_POSITION_STATUS           (TRUE)            /* ��ʼλ�ü��״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ�� */
#define U_START_ANGLE_COMP             (0)               /* ��λ:�� ��ʼλ�ü�ⲹ���Ƕ� */

#define IPD_PLUS_TIME_SETTING          (150)             /* ����ע��ʱ�������� ��λus */
#define IPD_WAIT_TIME_SETTING          (200)             /* ������еȴ�ʱ�������� ��λus */

#define IPD_PLUS_TIME_WIDTH            (u32)(IPD_PLUS_TIME_SETTING*(MCU_MCLK/1000000))   /* ����ע��ʱ�������� ��λclk */
#define IPD_PLUS_WAIT_TIME             (u32)(IPD_WAIT_TIME_SETTING*(MCU_MCLK/1000000))   /* ������еȴ�ʱ�������� ��λclk */

/* -------------------------------HALL��ѧϰ����------------------------------- */
#define HALL_LEARN_CURRENT_SETTINT     (1.0)              /* HALL��ѧϰ�����趨,��λ: A */

/* ----------------------------direction check Parameter----------------------- */
#define CW                             (0)                /* ���ת��˳ʱ�� */ 
#define CCW                            (1)                /* ���ת����ʱ��*/


/* ------------------------------ADCУ׼��ز�������---------------------------- */
#define CALIB_SAMPLES                  (512)              /* ADCƫ��У׼�����������޸� */
#define OFFSET_ERROR                   (3500)             /* ADCƫ�������ֵ�������޸� */

/* ----------------------------Ԥ���Ծٵ���Ԥ������--------------------------- */
#define CHARGE_TIME                    (100)              /* ÿ��Ԥ���ʱ�䣬����ʵ��Ӳ�������޸� */

/* -------------------------------- ˳��������------------------------------ */
#define DIR_CHECK_STATUS               (TRUE)            /* ˳�����״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ�� */
#define SPEED_TRACK_DELAYTIME          (500)              /* ��λ: ms ˳�����ʱ��  */
#define SPEED_TRACK_ON_FREQ_THH        (25)               /* ��λ: Hz ˳���бջ�Ƶ�� */
#define EBRK_ON_FREQ_THH               (50)               /* ��λ: Hz ���ɲ��Ƶ�� */

#define MOTOR_STOP_CUR_THD             (120)              /* ���ֹͣ��������ֵ */
#define MOTOR_STOP_CUR_DIF_THD         (12)               /* ���ֹͣ����������ֵ */

#define STOP_TIME                      (10)               /* ��λ��ms ���ֹͣ����˲�ʱ�䣬����ʵ�ʸ����޸� */
#define STOP_DELAY_TIME                (200)              /* ��λ: ms ���ֹͣ���ӳ�ʱ�䣬����ʵ�ʸ����޸ġ��޸����ݣ�������ж�Ϊֹͣ����ת���ͼӴ��ӳ�ʱ�� */

/*------------------------------------Ԥ��λ����---------------------------------*/
#define ALIGN_ANGLE                    (0)                /* ��λ:�� Ԥ��λ�Ƕ� */
#define U_START_CUR_SET_F              (1.5)              /* ��λ: A ��һ�ζ�λ���� */
#define U_START_CUR_SET_S              (3.0)              /* ��λ: A �ڶ��ζ�λ���� */
#define DC_HOLDTIME_TIME_LENTH         (50)               /* ��λ: ms ��һ�ζ�λʱ�� */
#define DC_HOLDTIME_TIME_LENTH_STAGE1  (100)               /* ��λ: ms �ڶ��ζ�λʱ�� */
#define DC_ALIGN_TOTAL_LENTH            \
       (DC_HOLDTIME_TIME_LENTH + DC_HOLDTIME_TIME_LENTH_STAGE1)/* ��λ��ʱ�� */

#define ALIGN_CURRENT_ACC              (0.5)              /* ��λ: (1/8)A/ms  ��λ�������ٵ���ֵ  ��ʼλ�ü��ʹ�ܺ�������ֵ�����ܳ���30���������ݻ������ */
#define ALIGN_CURRENT_DEC              (0.5)              /* ��λ: (1/8)A/ms  ��λ�������ٵ���ֵ  ��ʼλ�ü��ʹ�ܺ�������ֵ�����ܳ���30���������ݻ������ */

/*---------------------------------��������------------------------------------*/
#define OPEN_ANGLE_TAG_FREQ            (20.0)             /* ��λ��Hz �����϶�����Ƶ�� */
#define FREQ_ACC                       (5.0)              /* ��λ��(1/128)Hz/ms �����϶�Ƶ�ʼ��ٵ���ֵ */
#define FREQ_DEC                       (5.0)              /* ��λ��(1/128)Hz/ms �����϶�Ƶ�ʼ��ٵ���ֵ */

#define OPEN_RUN_STATUS                (FALSE)             /* ����״̬ TRUE = ��������, FALSE = �ջ����� */
  
#define MATCH_TIME                     (5)                /* ����͸�������ƥ����� */

/*---------------------- -----�����ջ��л����ɲ���------------------------------*/
#define OPEN2CLOSE_RUN_COV_TIME        (30)               /* �����ջ��л�����ʱ�䣺��λ��mS */
#define OPEN2CLOSE_RUN_CURRENT_RAMP    (0.1)             /* �����ջ��л������ڣ�D,Q������仯б�ʡ���λ��A/ms */

/*-----------------------------------��·ѡ��----------------------------------*/
#define CURRENT_LOOP                   (0)                /* ������ */
#define SPEED_LOOP                     (1)                /* �ٶȻ� */
#define POWER_LOOP                     (2)                /* ���ʻ� */
#define CLOSE_LOOP                     (SPEED_LOOP)       /* ��·ѡ�� */

/* ------------------------------�۲���PLL PI����----------------------------- */
#define PLL_KP_GAIN                    20                /* PLL_Kp ������Kp */
#define PLL_KI_GAIN                    10                 /* PLL_Ki ������Ki */

/*----------------------------------����������---------------------------------*/
#define IQ_SET                         (0.3)              /* ��λ��A IqRef��Iq����ֵ */
 
#define VQMAX                          (6000)             /* Q�����������ƣ�Q15��ʽ��ȡֵ��Χ0~6000 */
#define VQMIN                          (-6000)            /* Q����С������ƣ�Q15��ʽ��ȡֵ��Χ0~-6000 */

#define VDMAX                          (6000)             /* D�����������ƣ�Q15��ʽ��ȡֵ��Χ0~6000 */
#define VDMIN                          (-6000)            /* D����С������ƣ�Q15��ʽ��ȡֵ��Χ0~6000 */

#define P_CURRENT_KP                   (4000)             /* ������Kp��ʵ�����õ�Kp��������ֵ�͵��������������յ�Kp */
#define P_CURRENT_KI                   (2000)             /* ������Ki��ʵ�����õ�Kp��������ֵ�͵��������������յ�Ki */

#define AUTO_FW_LIM                    ((s16)0)            /* �Զ�����D��������ƣ�Q12��ʽ�����ֵ 4096 */

#define TORQUE_MODE_CURRENT_CHANGE_ACC (0.1)              /* ��λ��A/ms �������ٵ���ֵ */
#define TORQUE_MODE_CURRENT_CHANGE_DEC (0.1)              /* ��λ��A/ms �������ٵ���ֵ */

/*----------------------------------�ٶȻ�����-------------------------------*/
#define POWER_LIMIT_STATUS             (FALSE)            /* �޹���״̬�� TRUE = ʹ�ܣ� FALSE = ��ʹ�� */
#define POWER_LIMIT_VALUE              (20.0)             /* ��λ��W   ���ƹ��ʵĴ�С */
#define POWER_LIMIT_TIME               (5)                /* ��λ��ms  �ٶȻ����ڣ�  �޹��ʼ������� */
#define POWER_LIMIT_SPEED              (200)               /* ��λ��Hz  �޹���ת�ٸ���������ʵ��Ӧ�������ã� */

#define SPEED_SET                      (60)               /* ��λ��Hz  �ٶȸ���ֵ */
#define SPEED_LOOP_CNTR                (0)                /* ��λ��ms  �ٶȻ�·�������� */

#define STATE04_WAITE_TIME             (100)              /* Unit: ms �ٶȱ�����ʼ��ʱ�� */

#define P_ASR_KP                       (6000)             /* �ٶȻ�Kp */
#define P_ASR_KI                       (1000)              /* �ٶȻ�Ki */

#define IQMAX                          (6.0)              /* ��λ:A, �ٶȻ�������ֵ */
#define IQMIN                          (-6.0)             /* ��λ:A, �ٶȻ������Сֵ */

#define SPEED_RUN_ACC                  (0.05)              /* ��λ (1/128)Hz �ٶȼ��ٵ���ֵ */
#define SPEED_RUN_DEC                  (0.05)              /* ��λ (1/128)Hz �ٶȼ��ٵ���ֵ */

/*------------------------------------���ʻ�����-------------------------------*/
#define SPPED_LIMIT_STATUS             (FALSE)            /*  ��ת��״̬�� TRUE = ʹ�ܣ� FALSE = ��ʹ�� */
#define SPEED_LIMIT_VALUE              (100.0)            /* ��λ��Hz  ����ת�ٵĴ�С */
#define SPEED_LIMIT_TIME               (5)                /* ��λ��ms  ���ʻ����ڣ� ��ת�ټ������� */
#define SPEED_LIMIT_POWER_VALUE        (30)               /* ��λ��W   ��ת�ٹ��ʸ��� */

#define POWER_SET                      (30)               /* ��λ��W  ���ʸ���ֵ */
#define POWER_LOOP_CNTR                (1)                /* ��λ��ms  ���ʻ�·�������� */
                                                                  
#define POWER_KP                       (3000)             /* ���ʻ�Kp */
#define POWER_KI                       (1000)              /* ���ʻ�Ki */

#define POWER_IQMAX                    (6.0)              /* ��λ:A, ���ʻ�������ֵ */
#define POWER_IQMIN                    (-6.0)             /* ��λ:A, ���ʻ������Сֵ */
                                                                   
#define POWER_RUN_ACC                  (2.0)              /* ��λ w ���ʼ��ٵ���ֵ ע��POWER_RUN_ACC��POWER_RUN_DEC����̫С�������ϲ������ã�ʵ�ʳ����еļӼ���ֵ������С��0. */
#define POWER_RUN_DEC                  (2.0)              /* ��λ w ���ʼ��ٵ���ֵ */

/*------------------------------------FaultDetection---------------------------*/
/* ���������� */
#define I_PH_OVERCURRENT_FAULT         (10.0)              /* ��λ��A �����������趨ֵ */

/* ��Ƿѹ������ */
#define U_DCB_OVERVOLTAGE_FAULT        (60.0)               /* ��λ��V ��ѹ����趨ֵ */
#define U_DCB_OVERVOLTAGE_RECOVER      (59.5)               /* ��λ��V ��ѹ�ָ��趨ֵ */
#define U_DCB_UNDERVOLTAGE_FAULT       (20.0)               /* ��λ��V Ƿѹ����趨ֵ */
#define U_DCB_UNDERVOLTAGE_RECOVER     (20.5)               /* ��λ��V Ƿѹ�ָ��趨ֵ */

/* ��ˮ��ת���� */
#define I_PH_EMPTY_FAULT               (0.3)              /* ��λ��A ��ת�������趨ֵ */
#define SPEED_EMPTY_FAULT              (50.0)             /* ��λ��Hz ��ת���ת���趨ֵ  */

/* �¶ȼ����� */
#define TEMP_FAULT                     (150)              /* ���¼���趨ֵ */
#define TEMP_RECOVER                   (170)              /* ���»ָ��趨ֵ */
#define TEMP_BREAK                     (4000)             /* NTC��·�趨ֵ */
                                                                  
/* ��ת������ */
#define SPEED_STALL_MAX_FAULT          (300.0)            /* ��λ��Hz ��ת���ת�����ֵ */
#define SPEED_STALL_MIN_FAULT          (10.0)             /* ��λ��Hz ��ת���ת����Сֵ */

#define I_PH_STALL_FAULT               (1.5)              /* ��λ��A ��ת�������趨ֵ */

#define SPEED_STALL_FAULT              (20.0)             /* ��λ��Hz ��ת���ת���趨ֵ */
#define IQ_STALL_FAULT                 (0.2)              /* ��λ��A ��ת�������趨ֵ */

/* �������������� */
#define START_TIME_FAULT               (200)              /* ��λ��5ms ����֮��1s�ڻ�������ջ���������1s���ʱ�����ʵ��Ӧ�õ��� */

/* ȱ������� */
#define I_PHASE_LOSS_FAULT             (3000)             /* ������������ʵ�ʲ�����struCurrentAmplitude.nPhA/struCurrentAmplitude.nPhB/struCurrentAmplitude.nPhC */
                                                          /* �ļ���ֵ���趨����ȱ����������е��м�ȡֵ�� */
/* ���ϻָ�ʱ�� */
#define VOLT_FAULT_RECOVER_TIME        (2000)             /* ��λ��ms  ��Ƿѹ�ָ�ʱ�� */
#define CURRENT_FAULT_RECOVER_TIME     (2000)             /* ��λ��ms  �����ָ�ʱ�� */
#define STALL_FAULT_RECOVER_TIME       (2000)             /* ��λ��ms  ��ת�ָ�ʱ�� */
#define PHASELOSS_FAULT_RECOVER_TIME   (2000)             /* ��λ��ms  ȱ��ָ�ʱ�� */
#define TEMP_FAULT_RECOVER_TIME        (2000)             /* ��λ��ms  ���»ָ�ʱ�� */
#define START_FAULT_RECOVER_TIME       (1000)             /* ��λ��ms  ���������ָ�ʱ�� */
#define EMPTY_FAULT_RECOVER_TIME       (2000)             /* ��λ��ms  ��ˮ��ת�ָ�ʱ�� */



#endif  /* __PARAMETER_H */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* -----------------------------------END OF FILE------------------------------- */

