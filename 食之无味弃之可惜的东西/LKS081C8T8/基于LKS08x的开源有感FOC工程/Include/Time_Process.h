/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Time_Process.h
 * �ļ���ʶ��
 * ����ժҪ�� ��ʱ��غ���
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2020��8��16��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��8��16��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet Li
 * �޸����ݣ�����
 *
 *******************************************************************************/
/*------------------------------prevent recursive inclusion -------------------*/
#ifndef __TIME_PROCESS_H
#define __TIME_PROCESS_H

#define PWM_TIME_500uS                    (u16)(PWM_FREQ/1000/2)
#define PWM_TIME_1MS                      (u16)(PWM_FREQ/1000)
#define PWM_TIME_2MS                      (u16)(2*PWM_FREQ/1000)
#define PWM_TIME_4MS                      (u16)(4*PWM_FREQ/1000)
#define COMM_PER_TIME_5MS              	  ((u16)10)   
#define PWM_TIME_10MS                     (u16)(20*PWM_FREQ/1000)
#define PWM_TIME_20MS      				  (u16)(40*PWM_FREQ/1000)

#define TASK_SCHEDU_1MS                   (2)                                      /* �������1ms����ʱ�� */
#define TASK_SCHEDU_2MS                   (4)                                      /* �������2ms����ʱ�� */
#define TASK_SCHEDU_10MS                  (20)                                     /* �������10ms����ʱ�� */
#define TASK_SCHEDU_240MS                 (480)                                    /* �������240ms����ʱ�� */
#define TASK_SCHEDU_500MS                 (1000)                                   /* �������500ms����ʱ�� */

#endif
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
