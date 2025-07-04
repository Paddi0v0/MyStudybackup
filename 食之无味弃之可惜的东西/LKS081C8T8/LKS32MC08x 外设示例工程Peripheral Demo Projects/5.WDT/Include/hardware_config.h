/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_config.h
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2020��8��20��
 *
 * �޸ļ�¼1��
 * �޸����ڣ� 2020��8��20��
 * �� �� �ţ� V 2.0
 * �� �� �ˣ� Howlet
 * �޸����ݣ� ����
 *
 *******************************************************************************/

/*------------------------------prevent recursive inclusion -------------------*/
#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_

#include "lks32mc08x.h"
#include "lks32mc08x_MCPWM.h"
#include "lks32mc08x_sys.h"
#include "lks32mc08x_adc.h"
#include "lks32mc08x_MCPWM.h"
#include "lks32mc08x_flash.h"
#include "lks32mc08x_Gpio.h"
#include "lks32mc08x_cmp.h"
#include "lks32mc08x_uart.h"
#include "lks32mc08x_hall.h"
#include "lks32mc08x_tim.h"
#include "hardware_init.h"
#include "lks32mc08x_NVR.h"

#define P_HIGH__N_HIGH                   0                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#define P_HIGH__N_LOW                    1                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */

#define  CHIP_PART_NUMBER              LKS32MC081         /* оƬ�ͺ�ѡ��ѡ����ȷ��Ӱ��оƬģ��ĳ�ʼ�� */

#if ((CHIP_PART_NUMBER == LKS32MC084D)||(CHIP_PART_NUMBER == LKS32MC086D)||(CHIP_PART_NUMBER == LKS32MC087D)) 
#define  MCPWM_SWAP_FUNCTION           1
#define  PRE_DRIVER_POLARITY           P_HIGH__N_HIGH     /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#else
#define  PRE_DRIVER_POLARITY           P_HIGH__N_LOW      /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */
#endif

/* ----------------------PWM Ƶ�ʼ���������----------------------------------- */
#define MCU_MCLK                       (96000000LL)       /* PWMģ��������Ƶ */
#define PWM_MCLK                       ((u32)MCU_MCLK)    /* PWMģ��������Ƶ */
#define PWM_PRSC                       ((u8)0)            /* PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ                       ((u16)22000)       /* PWMն��Ƶ�� */

#endif  /* __HARDWARE_CONFIG_H_ */

 
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
 
