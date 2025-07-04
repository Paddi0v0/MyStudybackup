/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Chanhom_adc.h
 * �ļ���ʶ��
 * ����ժҪ�� ADC������������ͷ�ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2015��11��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2015��11��5��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
 
#ifndef __LKS08X_PWM_H
#define __LKS08X_PWM_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"



typedef struct
{
    uint16_t MCPWM_PERIOD;           /* MCPWM�������*/
	
    uint16_t MCPWM_WorkModeCH0;      /* MCPWM CH0����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH1;      /* MCPWM CH0����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH2;      /* MCPWM CH0����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH3;      /* MCPWM CH0����ģʽ�����ض���/���Ķ��� */	
 
    uint16_t TriggerPoint0;          /* PWM����ADC�¼�0��ʱ������� */
    uint16_t TriggerPoint1;          /* PWM����ADC�¼�1��ʱ������� */
    uint16_t TriggerPoint2;          /* PWM����ADC�¼�2��ʱ������� */
    uint16_t TriggerPoint3;          /* PWM����ADC�¼�3��ʱ������� */
 
    uint16_t DeadTimeCH0N;           /* CH0N����ʱ�����á�*/
    uint16_t DeadTimeCH0P;           /* CH0P����ʱ�����á�*/
    uint16_t DeadTimeCH1N;           /* CH1N����ʱ�����á�*/
    uint16_t DeadTimeCH1P;           /* CH1P����ʱ�����á�*/
    uint16_t DeadTimeCH2N;           /* CH2N����ʱ�����á�*/
    uint16_t DeadTimeCH2P;           /* CH2P����ʱ�����á�*/
    uint16_t DeadTimeCH3N;           /* CH3N����ʱ�����á�*/
    uint16_t DeadTimeCH3P;           /* CH3P����ʱ�����á�*/	
 
    uint8_t CLK_DIV;                 /* MCPWM ��Ƶϵ�� */
    uint8_t MCLK_EN;                 /* MCPWM ʱ��ʹ�ܿ��� */
    uint8_t MCPWM_Cnt_EN;            /* MCPWM ��������ʹ�ܿ��� */
    uint8_t GPIO_BKIN_Filter;        /* GPIO�����˲�ʱ������1-16 */
    uint8_t CMP_BKIN_Filter;         /* �Ƚ���CMP�����˲�ʱ������1-16 */ 
    uint8_t CH0N_Polarity_INV;       /* CH0N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH0P_Polarity_INV;       /* CH0P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH1N_Polarity_INV;       /* CH1N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH1P_Polarity_INV;       /* CH1P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH2N_Polarity_INV;       /* CH2N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH2P_Polarity_INV;       /* CH2P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH3N_Polarity_INV;       /* CH3N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH3P_Polarity_INV;       /* CH3P�������ȡ����0:���������1:ȡ����� */	
   
    uint8_t CH0P_SCTRLP;             /* ��CH0_PS = 1ʱ�������CH0_Pͨ����ֵ*/
    uint8_t CH0N_SCTRLN;             /* ��CH0_NS = 1ʱ�������CH0_Nͨ����ֵ*/
    uint8_t CH1P_SCTRLP;             /* ��CH1_PS = 1ʱ�������CH1_Pͨ����ֵ*/
    uint8_t CH1N_SCTRLN;             /* ��CH1_NS = 1ʱ�������CH1_Nͨ����ֵ*/
    uint8_t CH2P_SCTRLP;             /* ��CH2_PS = 1ʱ�������CH2_Pͨ����ֵ*/
    uint8_t CH2N_SCTRLN;             /* ��CH2_NS = 1ʱ�������CH2_Nͨ����ֵ*/
    uint8_t CH3P_SCTRLP;             /* ��CH3_PS = 1ʱ�������CH3_Pͨ����ֵ*/
    uint8_t CH3N_SCTRLN;             /* ��CH3_NS = 1ʱ�������CH3_PNͨ����ֵ*/
   
    uint8_t CH0_PS;                  /* CH0_P�ź���Դ��1������CH0_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH0_NS;                  /* CH0_N�ź���Դ��1������CH0_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH1_PS;                  /* CH1_P�ź���Դ��1������CH1_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH1_NS;                  /* CH1_N�ź���Դ��1������CH1_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH2_PS;                  /* CH2_P�ź���Դ��1������CH2_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH2_NS;                  /* CH2_N�ź���Դ��1������CH2_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH3_PS;                  /* CH3_P�ź���Դ��1������CH3_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH3_NS;                  /* CH3_N�ź���Դ��1������CH3_SCTRLN��0������MCPWMʵ�����м�����*/
		
    uint8_t Switch_CH0N_CH0P;        /* ����CH0N, CH0P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH1N_CH1P;        /* ����CH1N, CH1P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH2N_CH2P;        /* ����CH2N, CH2P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH3N_CH3P;        /* ����CH3N, CH3P�ź����ʹ�ܿ��� */	

    uint8_t MCPWM_UpdateInterval;    /* MCPWM T0/T1�¼����¼�� */
    uint8_t MCPWM_T0_UpdateEN;       /* MCPWM T0�¼�����ʹ�� */
    uint8_t MCPWM_T1_UpdateEN;       /* MCPWM T1�¼�����ʹ�� */
    uint8_t MCPWM_Auto_ERR_EN;       /* MCPWM �����¼��Ƿ��Զ���MOE, ʹ�ܿ��� */
 
    uint8_t FAIL0_INPUT_EN;          /* FAIL0 ���빦��ʹ�� */
    uint8_t FAIL1_INPUT_EN;          /* FAIL1 ���빦��ʹ�� */	 
    uint8_t FAIL0_Signal_Sel;        /* FAIL0 �ź�ѡ�񣬱Ƚ���0��GPIO */
    uint8_t FAIL1_Signal_Sel;        /* FAIL1 �ź�ѡ�񣬱Ƚ���0��GPIO */	 
    uint8_t FAIL0_Polarity;          /* FAIL0 �źż������ã�����Ч�����Ч */	 
    uint8_t FAIL1_Polarity;          /* FAIL1 �źż������ã�����Ч�����Ч */
    uint8_t DebugMode_PWM_out;       /* Debugʱ��MCU����Halt, MCPWM�ź��Ƿ�������� */

    uint8_t CH0P_default_output;     /* CH0P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH0N_default_output;     /* CH0N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH1P_default_output;     /* CH1P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH1N_default_output;     /* CH1N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH2P_default_output;     /* CH2P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH2N_default_output;     /* CH2N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH3P_default_output;     /* CH3P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH3N_default_output;     /* CH3N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */	

    uint8_t T0_Update_INT_EN;        /* T0�����¼��Д�ʹ�� */
    uint8_t T1_Update_INT_EN;        /* T1�����¼��Д�ʹ�� */  
    uint8_t TMR0_Match_INT_EN;       /* TMR0�����¼�ƥ���¼��ж�ʹ�� */    
    uint8_t TMR1_Match_INT_EN;       /* TMR1�����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TMR2_Match_INT_EN;       /* TMR2�����¼�ƥ���¼��ж�ʹ�� */    
    uint8_t TMR3_Match_INT_EN;       /* TMR3�����¼�ƥ���¼��ж�ʹ�� */    

    uint8_t FAIL0_INT_EN;            /* FAIL0�¼��ж�ʹ�� */
    uint8_t FAIL1_INT_EN;            /* FAIL1�¼��ж�ʹ�� */	

    uint8_t IO0_PPE;                 /* IO0����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
		uint8_t IO1_PPE;                 /* IO1����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
		uint8_t IO2_PPE;                 /* IO2����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
		uint8_t IO3_PPE;                 /* IO3����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */

}MCPWM_InitTypeDef;


/* ----------------------PWM Ƶ�ʼ���������----------------------------------- */
#define MCPWM_MOE_ENABLE_MASK                     ((uint16_t)0x0040)   /* ��MOEλ */
#define MCPWM_MOE_DISABLE_MASK                    ((uint16_t)~0x0040)   /* ��MOEλ */


#define CENTRAL_PWM_MODE     0  /* ���Ķ���PWMģʽ */
#define EDGE_PWM_MODE        1  /* ���ض���PWMģʽ */

#define HIGH_LEVEL           1  /* �ߵ�ƽ */
#define LOW_LEVEL            0  /* �ߵ�ƽ */

#define HIGH_LEVEL_ACTIVE    0  /* �ߵ�ƽ��Ч */
#define LOW_LEVEL_ACTIVE     1  /* �͵�ƽ��Ч */

#define FAIL_SEL_CMP         1  /* Fail�¼���Դ�Ƚ��� */
#define FAIL_SEL_IO          0  /* Fail�¼���Դ�Ƚ��� */

#define MCPWM_T0_IRQ_EN        BIT0  /* ����������ֵ�ص�MCPWM_TH�ж�ʹ�� */
#define MCPWM_T1_IRQ_EN        BIT1  /* ����������ֵ�ص�0�ж�ʹ�� */
#define MCPWM_TH00_IRQ_EN      BIT2  /* ����������ֵ�ص�MCPWM_TH00�ж�ʹ�� */
#define MCPWM_TH01_IRQ_EN      BIT3  /* ����������ֵ�ص�MCPWM_TH01�ж�ʹ�� */
#define MCPWM_TH10_IRQ_EN      BIT4  /* ����������ֵ�ص�MCPWM_TH10�ж�ʹ�� */
#define MCPWM_TH11_IRQ_EN      BIT5  /* ����������ֵ�ص�MCPWM_TH11�ж�ʹ�� */
#define MCPWM_TH20_IRQ_EN      BIT6  /* ����������ֵ�ص�MCPWM_TH20�ж�ʹ�� */
#define MCPWM_TH21_IRQ_EN      BIT7  /* ����������ֵ�ص�MCPWM_TH21�ж�ʹ�� */
#define MCPWM_TH30_IRQ_EN      BIT8  /* ����������ֵ�ص�MCPWM_TH30�ж�ʹ�� */
#define MCPWM_TH31_IRQ_EN      BIT9  /* ����������ֵ�ص�MCPWM_TH31�ж�ʹ�� */
#define MCPWM_TMR0_IRQ_EN      BIT10 /* ����������ֵ�ص�MCPWM_TMR0�ж�ʹ�� */
#define MCPWM_TMR1_IRQ_EN      BIT11 /* ����������ֵ�ص�MCPWM_TMR1�ж�ʹ�� */
#define MCPWM_TMR2_IRQ_EN      BIT12 /* ����������ֵ�ص�MCPWM_TMR0�ж�ʹ�� */
#define MCPWM_TMR3_IRQ_EN      BIT13 /* ����������ֵ�ص�MCPWM_TMR1�ж�ʹ�� */
#define MCPWM_SHADE_IRQ_EN     BIT14 /* MCPWM_TH/MCPWM_TH00-MCPWM_TH31/
MCPWM_TR0-MCPWM_TR3�ȼĴ������µ�MCPWMʵ������ϵͳ�ж�ʹ�� */

#define MCPWM_CH0_SHORT_IRQ_EN      BIT0     /* MCPWMCH0_P��CH0Nͬʱ��Ч�ж�ʹ�� */
#define MCPWM_CH1_SHORT_IRQ_EN      BIT1     /* MCPWMCH1_P��CH1Nͬʱ��Ч�ж�ʹ�� */
#define MCPWM_CH2_SHORT_IRQ_EN      BIT2     /* MCPWMCH2_P��CH2Nͬʱ��Ч�ж�ʹ�� */
#define MCPWM_CH3_SHORT_IRQ_EN      BIT3     /* MCPWMCH3_P��CH3Nͬʱ��Ч�ж�ʹ�� */
#define MCPWM_FAIL0_IRQ_EN          BIT4     /* MCPWM_FAIL0�ж�ʹ�� */
#define MCPWM_FAIL1_IRQ_EN          BIT5     /* MCPWM_FAIL1�ж�ʹ�� */

#define MCPWM_T0_IRQ_IF        BIT0  /* ����������ֵ�ص�MCPWM_TH�жϱ�־λ */
#define MCPWM_T1_IRQ_IF        BIT1  /* ����������ֵ�ص�0�жϱ�־λ */
#define MCPWM_TH00_IRQ_IF      BIT2  /* ����������ֵ�ص�MCPWM_TH00�жϱ�־λ */
#define MCPWM_TH01_IRQ_IF      BIT3  /* ����������ֵ�ص�MCPWM_TH01�жϱ�־λ */
#define MCPWM_TH10_IRQ_IF      BIT4  /* ����������ֵ�ص�MCPWM_TH10�жϱ�־λ */
#define MCPWM_TH11_IRQ_IF      BIT5  /* ����������ֵ�ص�MCPWM_TH11�жϱ�־λ */
#define MCPWM_TH20_IRQ_IF      BIT6  /* ����������ֵ�ص�MCPWM_TH20�жϱ�־λ */
#define MCPWM_TH21_IRQ_IF      BIT7  /* ����������ֵ�ص�MCPWM_TH21�жϱ�־λ */
#define MCPWM_TH30_IRQ_IF      BIT8  /* ����������ֵ�ص�MCPWM_TH30�жϱ�־λ */
#define MCPWM_TH31_IRQ_IF      BIT9  /* ����������ֵ�ص�MCPWM_TH31�жϱ�־λ */
#define MCPWM_TMR0_IRQ_IF      BIT10 /* ����������ֵ�ص�MCPWM_TMR0�жϱ�־λ */
#define MCPWM_TMR1_IRQ_IF      BIT11 /* ����������ֵ�ص�MCPWM_TMR1�жϱ�־λ */
#define MCPWM_TMR2_IRQ_IF      BIT12 /* ����������ֵ�ص�MCPWM_TMR0�жϱ�־λ */
#define MCPWM_TMR3_IRQ_IF      BIT13 /* ����������ֵ�ص�MCPWM_TMR1�жϱ�־λ */
#define MCPWM_SHADE_IRQ_IF     BIT14 /* MCPWM_TH/MCPWM_TH00-MCPWM_TH31/
MCPWM_TR0-MCPWM_TR3�ȼĴ������µ�MCPWMʵ������ϵͳ�жϱ�־λ */

#define MCPWM_CH0_SHORT_IRQ_IF      BIT15     /* MCPWMCH0_P��CH0Nͬʱ��Ч�жϱ�־λ */
#define MCPWM_CH1_SHORT_IRQ_IF      BIT16     /* MCPWMCH1_P��CH1Nͬʱ��Ч�жϱ�־λ */
#define MCPWM_CH2_SHORT_IRQ_IF      BIT17     /* MCPWMCH2_P��CH2Nͬʱ��Ч�жϱ�־λ */
#define MCPWM_CH3_SHORT_IRQ_IF      BIT18     /* MCPWMCH3_P��CH3Nͬʱ��Ч�жϱ�־λ */
#define MCPWM_FAIL0_IRQ_IF          BIT19     /* MCPWM_FAIL0�жϱ�־λ */
#define MCPWM_FAIL1_IRQ_IF          BIT20     /* MCPWM_FAIL1�жϱ�־λ */

void PWMOutputs(FuncState t_state);
void MCPWM_Init(MCPWM_InitTypeDef* MCPWM_InitStruct);
void MCPWM_StructInit(MCPWM_InitTypeDef* MCPWM_InitStruct);
u16 MCPWM_GetIRQFlag( uint32_t INT_flag);
void MCPWM_ClearIRQFlag( uint32_t INT_flag);
#endif /*__CHANHOM_PWM_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
