/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO Semiconductor
 *
 * �ļ����ƣ� mc_hall.c
 * �ļ���ʶ��
 * ����ժҪ�� Hall�źŴ���
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� ����
 * ������ڣ� 2016��2��16��
 *
 * �޸ļ�¼1��
 *    �޸����ڣ�2016��2��26��
 *    �� �� �ţ�V 1.0
 *    �� �� �ˣ�����
 *    �޸����ݣ�����
 *
 * �޸ļ�¼2��
 *    �޸����ڣ�
 *    �� �� �ţ�
 *    �� �� �ˣ�
 *    �޸����ݣ�
 *
 *******************************************************************************/


#ifndef __HALL_H
#define __HALL_H

#include "LKS32MC08x.h"
 
#define S16_360_PHASE_SHIFT 					(u16)(65535)
#define S16_330_PHASE_SHIFT 					(u16)(60075)
#define S16_318_PHASE_SHIFT 					(u16)(58000)
#define S16_315_PHASE_SHIFT 					(u16)(57344)
#define S16_300_PHASE_SHIFT 					(u16)(54613)
#define S16_270_PHASE_SHIFT 					(u16)(49152)
#define S16_240_PHASE_SHIFT 					(u16)(43691)
#define S16_210_PHASE_SHIFT 					(u16)(38220)
#define S16_180_PHASE_SHIFT 					(u16)(32768)
#define S16_150_PHASE_SHIFT 					(u16)(27300)
#define S16_120_PHASE_SHIFT 					(u16)(21845)
#define S16_90_PHASE_SHIFT  					(u16)(16384)
#define S16_75_PHASE_SHIFT  					(u16)(13653)
#define S16_70_PHASE_SHIFT  					(u16)(12743)
#define S16_63_PHASE_SHIFT  					(u16)(11468)
#define S16_60_PHASE_SHIFT  					(u16)(10923)
#define S16_45_PHASE_SHIFT  					(u16)(8192)
#define S16_30_PHASE_SHIFT  					(u16)(5461)
#define S16_20_PHASE_SHIFT  					(u16)(3641)
#define S16_15_PHASE_SHIFT  					(u16)(2731)
#define S16_10_PHASE_SHIFT  					(u16)(1820)
#define S16_5_PHASE_SHIFT   					(u16)(910)
#define S16_1_PHASE_SHIFT   					(u16)(182)								/* 1�Ƚǹ�һ������ 1/360*65536, �Ƕ�0~360�ȱ仯�� ��Ӧ��һ������0~65535 */

#define HALL_COM_TIMEOUT       		   			0x01           						 	/* ��ʱ������� */
#define HALL_COM_FLG           		   			0x02           						 	/* �����־��Hall�����仯ʱ��1 */
#define HALL_COM_ERR           		  	 		0x04           						 	/* Hall�仯��Ԥ��ֵ��һ�� */
#define HALL_DIR_FLG           		   			0x80           						 	/* ������з����־��0:��ת 1:��ת */

#define REDUCE_TOQUE_PUSE      		   			0x01           						 	/* ����ٶȽϵ�ʱ�����򽵵ͻ��������ת�ز�����־ */
#define REDUCE_TOQUE_MINUS     		   			0x02           						 	/* ����ٶȽϵ�ʱ�����򽵵ͻ��������ת�ز�����־ */ 

#endif /* __HALL_H */
/******************* *****END OF FILE****/
