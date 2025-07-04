/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� KeyScan.h
 * �ļ���ʶ��
 * ����ժҪ�� KeyScan
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� andrew kong
 * ������ڣ� 2019��12��27��
 *
 *******************************************************************************/
#ifndef __PWMSCAN_H
#define __PWMSCAN_H
 
#include "basic.h"
#include "FOC_Drive.h"

#define PWM_TIME_PERIOD      9600//����Ƶ�ʶ�ӦTimer����ֵ����Բ���PWM��ʱ�����
#define DUTY_ZERO            0  //0ռ�ձ�
#define DUTY_FULL            1000//100%ռ�ձȣ���ֵ������10��

#define PWM_HIGH             (GPIO2_PDI & BIT5) //PWM����IO��ƽΪ�ߣ�һ����˵���ռ�ձ�Ϊ100%�����Ϊ0%

#define DIR_FLITER           10//��λ��ms ���򰴼��˲�ʱ�䣬���ڰ���ȥ����ʱ�䳤�̾���������
#define DIR_CW               (GPIO0_PDI & BIT11)//��������IO�ڵ�ƽΪ��

#define DIR_DELAY            (500)//��λ:ms

#define FG_HIGH              (GPIO0_PDO |= 0x8000) //FG���IO�ڵ�ƽΪ��
#define FG_LOW               (GPIO0_PDO &= 0x7FFF) //FG���IO�ڵ�ƽΪ��

#define RESET_TIME            (5000) //��λ��ms ��λʱ��

/*********************PWM����趨******************************/ 
#define PWM_MIN               (200)//��Сռ�ձȣ�20%
#define PWM_MAX               (900)//���ռ�ձȣ�90%

#define SPEED_MIN             (s32)(1000) //��λ��0.01Hz ��Сת��
#define SPEED_MAX             (s32)(10000)//��λ��0.01Hz ���ת��

#define SPEED_SCALE           (float)(SPEED_MAX - SPEED_MIN)/(PWM_MAX - PWM_MIN)  //PWM ����б��

/***********************VSP����趨****************************/ 
#define VSP_MIN               (4550)//��λ����ѹ��Сֵ   ���㹫ʽ�� 0.5/3.6*32767 ��0.5Ϊ��С��ѹ
#define VSP_MAX               (27305)//��λ����ѹ���ֵ  ���㹫ʽ�� 3.0/3.6*32767 ��3.0Ϊ����ѹ

#define POWER_MIN             (142)  //������Сֵ ���չ���������߱궨
#define POWER_MAX             (12850)//�������ֵ ���չ���������߱궨

#define POWER_SCALE           (float)(POWER_MAX - POWER_MIN)/(VSP_MAX - VSP_MIN) //�����趨б��

typedef struct
{
  s16 nHighLevelTime;    //�ߵ�ƽ����ֵ
  s16 nLowLevelTime;     //�͵�ƽ����ֵ
  s16 nPeriod;           //���ڼ���ֵ
  u16 nDuty;             //ռ�ձ�
  u16 nFlagPeriod;       //���ڼӲ��־λ
  s16 nPreTime;          //�ϴμ���ֵ
  u16 nFlagReset;        //��λ��־λ
  u16 nResetCntr;        //��λ����
  
  s16  nVSPValue;        //��λ����ѹ
  s32  wPowerValue;      //�����趨ֵ
}stru_PWMTime_t;



extern volatile u8 g_bMC_RunFlg;

extern stru_PWMTime_t  PWMTime;
extern void delay(u32 cnt);

extern void PWMScan(void);
extern void PWMScanInit(void);
extern void PWMDutyScan(void);
extern void SpeedScan(void);
extern void MotorDirScan(void);
extern void FGScan(UINT16 Theta);
extern void FGScanInit(void);
extern void PowerScan(void);

extern void PWMOutputs(FuncState t_state);

#endif

