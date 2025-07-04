/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� state_machine.h
 * ����ժҪ�� state machine
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��8��19��
 *
 *******************************************************************************/
#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include "basic.h"
#include "mc_type.h"

typedef enum 
{
	MOTOR_INFO_TRACKING,				//	˳�������
	MOTOR_INFO_TRACKING_RESULT,			//	�ȴ�˳�������
	MOTOR_INFO_TRACKING_BRAKE,			//	˳�������ƶ�
	MOTOR_INFO_TRACKING_RUN,			//	˳����������
	MOTOR_INFO_IPD,						//	��ʼλ��IPD���
	MOTOR_INFO_IPD_RESULT,				//	��ʼλ��IPD�����
	MOTOR_INFO_ILLEGE					//	�Ƿ�״̬
}ENU_MOTOR_INFO_STATE;				// �����Ϣ���״̬

typedef enum 
{
	TRACKING,							//	�����
	TRACKING_2_RUN,						//	�����������
	TRACKING_2_BRAKE,					//	������Ҫ�ƶ�
	TRACKING_ILLEGE						//	��Ч���
}ENU_TRAKING_RESULT;				//	˳�������


typedef enum						//  �������MA״̬
{
	E_DRIVER_INIT			= 0,		//	��ʼ״̬
	E_DRIVER_POWER_ON_WAIT	= 1,		//	�ϵ�ȴ�״̬
	E_DRIVER_POWER_OFF		= 2,		//	����״̬
	E_DRIVER_CALIB			= 3,		//	����·У��״̬
	E_DRIVER_STOP			= 4,		//	STOP״̬
	E_DRIVER_CHARGE			= 5,		//	Ԥ���״̬
	E_DRIVER_MOTOR_INFO		= 6,		//	�����Ϣ���״̬(˳����IPD���)
	E_DRIVER_READY_RUN		= 7,		//	����ALLIGN״̬
	E_DRIVER_RUN			= 8,		//	����״̬
	E_DRIVER_READY_STOP		= 9,		//	׼��ֹͣ״̬
	E_DRIVER_FAULT			= 10		//	FAULT����״̬
}ENU_DRIVER_STATUS;

#endif

///* ********************** (C) COPYRIGHT LINKO SEMICONDUCTOR ******************** */
///* ------------------------------END OF FILE------------------------------------ */


