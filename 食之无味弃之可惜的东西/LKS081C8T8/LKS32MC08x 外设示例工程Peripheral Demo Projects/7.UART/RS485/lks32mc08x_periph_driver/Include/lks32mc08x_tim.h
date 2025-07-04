/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_tim.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� TIM����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��06��06�� <td>1.0     <td>cfwu        <td>����
 * </table>
 */
#ifndef __LKS32MC08x_TIM_H
#define __LKS32MC08x_TIM_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

/** 
 *@brief ��ʱ���Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t CFG;  /**<��ʱ�����üĴ���*/
    __IO uint32_t TH;   /**<��ʱ���������޼Ĵ���*/
    __IO uint32_t CNT;  /**<��ʱ������ֵ�Ĵ���*/
    __IO uint32_t CMPT0;/**<��ʱ���Ƚ�/����Ĵ��� 0*/
    __IO uint32_t CMPT1;/**<��ʱ���Ƚ�/����Ĵ��� 1*/
    __IO uint32_t EVT;  /**<��ʱ���ⲿ�¼�ѡ��Ĵ���*/
} TIM_TimerTypeDef;

/** 
 *@brief �������Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t CFG; /**<���������üĴ���*/
    __IO uint32_t TH;  /**<�������������޼Ĵ���*/
    __IO uint32_t CNT; /**<����������ֵ�Ĵ���*/
} TIM_ECDTypeDef;

/**
 * TIMER0�ṹ�����ַ����
 */
#ifndef TIMER0
#define TIMER0              ((TIM_TimerTypeDef *)  UTIMER_BASE)
#endif
/**
 * TIMER1�ṹ�����ַ����
 */
#ifndef TIMER1
#define TIMER1              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x20))
#endif
/**
 * TIMER2�ṹ�����ַ����
 */
#ifndef TIMER2
#define TIMER2              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x40))
#endif
/**
 * TIMER3�ṹ�����ַ����
 */
#ifndef TIMER3
#define TIMER3              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x60))
#endif
/**
 * ECD0�ṹ�����ַ����
 */
#ifndef ECD0
#define ECD0                ((TIM_ECDTypeDef *)   (UTIMER_BASE+0x80))
#endif
/**
 * ECD1�ṹ�����ַ����
 */
#ifndef ECD1
#define ECD1                ((TIM_ECDTypeDef *)   (UTIMER_BASE+0x90))
#endif

#define TIM_Clk_Div1          0x00         /**<Timer����Ƶ��96M*/
#define TIM_Clk_Div2          0x01         /**<Timer����Ƶ��48M*/
#define TIM_Clk_Div4          0x02         /**<Timer����Ƶ��24M*/
#define TIM_Clk_Div8          0x03         /**<Timer����Ƶ��12M*/

#define ECD_Clk_Div1          0x00         /**<ECD����Ƶ��96M*/
#define ECD_Clk_Div2          0x01         /**<ECD����Ƶ��48M*/
#define ECD_Clk_Div4          0x02         /**<ECD����Ƶ��24M*/
#define ECD_Clk_Div8          0x03         /**<ECD����Ƶ��12M*/

 /**
 * @brief �ж�ʹ�����ö���
 */
typedef enum
{
    Timer_IRQEna_None = 0,
    Timer_IRQEna_Zero = BIT0,     /**<ʹ�ܹ����ж�*/
    Timer_IRQEna_CH0 = BIT1,      /**<ʹ��CH0�жϣ����Ƚϡ������ж�*/
    Timer_IRQEna_CH1 = BIT2,      /**<ʹ��CH1�жϣ����Ƚϡ������ж�*/
    Timer_IRQEna_All = 0x07       /**<ʹ��Timerȫ���ж�*/
} Timer_IRQEnaDef;
														 											 
#define TIMER_IF_CH1            BIT2     /**<�жϱ�־����,Timer CH1�жϣ��Ƚϻ򲶻��ж�*/
#define TIMER_IF_CH0            BIT1     /**<�жϱ�־����,Timer CH0�жϣ��Ƚϻ򲶻��ж�*/
#define TIMER_IF_ZERO           BIT0     /**<�жϱ�־����,Timer �����ж�*/
																 												 

#define ECD_IRQEna_None          0     /**< �������ж�ʹ��,���ö�����ʹ��*/
#define ECD_IRQEna_UnderFlow     BIT0  /**< �������ж�ʹ��,ʹ�������ж�*/
#define ECD_IRQEna_OverFlow      BIT1  /**< �������ж�ʹ��,ʹ�������ж�*/
																           
#define ECD_IF_UNDERFLOW        BIT0 /**<�������жϱ�־����,Encoder�����ж�*/
#define ECD_IF_OVERFLOW         BIT1 /**<�������жϱ�־����,Encoder�����ж�*/
																 
#define TIMER_CapMode_None       ((uint32_t)0x00000000)   /**<����ģʽ����,�޲���*/
#define TIMER_CapMode_Rise       ((uint32_t)0x00000001)   /**<����ģʽ����,�����ز���*/
#define TIMER_CapMode_Fall       ((uint32_t)0x00000002)   /**<����ģʽ����,�½��ز���*/
#define TIMER_CapMode_RiseFall   ((uint32_t)0x00000003)   /**<����ģʽ����,˫�ز���*/
															   
#define TIMER_OPMode_CAP         ((uint32_t)0x0000001)    /**<����ģʽ,����ģʽ*/
#define TIMER_OPMode_CMP         ((uint32_t)0x0000000)    /**<����ģʽ,�Ƚ�ģʽ*/


#define ECD_Mode_T1              ((uint32_t)0x00) /**<counting  on T1*/
#define ECD_Mode_T1_T2           ((uint32_t)0x01) /**<counting  on T1&T2*/
#define ECD_Mode_CCWSIGN         ((uint32_t)0x02) /**<CCW+SIGN, �������ź�������*/
#define ECD_Mode_CCWCW           ((uint32_t)0x03) /**<CCW+CW,   �������ź�������*/
#define ECD_Mode_CCWSIGN_UpDown  ((uint32_t)0x06) /**<���ż������źż���ģʽ, �źŵ������ؾ�����*/
#define ECD_Mode_CCWCW_UpDown    ((uint32_t)0x07) /**<CCW+CW ˫�����źż���ģʽ���źŵ������ؾ�����*/


typedef struct
{
    FuncState EN;      /*��ʱ��ģ��ʹ��*/
    /**
     * @brief channel0����ģʽ:����ģʽ���Ƚ�ģʽ \n
     * @see TIMER_OPMode_CAP \n
     * @see TIMER_OPMode_CMP 
     */
    uint32_t Timer_CH0_WorkMode; 
    /**
     * @brief channel0����ģʽ:�����ء��½��ء�˫�� \n
     * @see TIMER_CapMode_None \n
     * @see TIMER_CapMode_Rise \n
     * @see TIMER_CapMode_Fall \n
     * @see TIMER_CapMode_RiseFall 
     */
    uint32_t Timer_CH0_CapMode;   
    uint32_t Timer_CH0Output;     /**<����������ʱ���Ƚ�ģʽIO��� 1���ߵ�ƽ 0���͵�ƽ*/
       /**
     * @brief channel0����ģʽ:����ģʽ���Ƚ�ģʽ \n
     * @see TIMER_OPMode_CAP \n
     * @see TIMER_OPMode_CMP 
     */
    uint32_t Timer_CH1_WorkMode; 
    /**
     * @brief channel0����ģʽ:�����ء��½��ء�˫�� \n
     * @see TIMER_CapMode_None \n
     * @see TIMER_CapMode_Rise \n
     * @see TIMER_CapMode_Fall \n
     * @see TIMER_CapMode_RiseFall 
     */
    uint32_t Timer_CH1_CapMode;   
    uint32_t Timer_CH1Output;     /**<����������ʱ���Ƚ�ģʽIO��� 1���ߵ�ƽ 0���͵�ƽ*/

    uint32_t Timer_TH;            /**<��ʱ����������*/
    uint32_t Timer_CMP0;          /**<CH0�Ƚϼ�����ֵ*/
    uint32_t Timer_CMP1;          /**<CH1�Ƚϼ�����ֵ*/

    uint32_t Timer_Filter0;       /**<CH0�˲���Ƶ, 0:���˲� N:�˲����õ�ʱ����ϵͳʱ��N��Ƶ*/
    uint32_t Timer_Filter1;       /**<CH1�˲���Ƶ  0:���˲� N:�˲����õ�ʱ����ϵͳʱ��N��Ƶ*/
   
     /**
     * @brief ��ʱ����Ƶ \n
     * @see TIM_Clk_Div1 \n
     * @see TIM_Clk_Div2 \n
     * @see TIM_Clk_Div4 \n
     * @see TIM_Clk_Div8 
     */
    uint32_t Timer_ClockDiv;    
     /**
     * @brief ��ʱ���ж�ʹ�� \n
     * @see Timer_IRQEnaDef
     */
    uint32_t Timer_IRQEna;        
} TIM_TimerInitTypeDef;

typedef struct
{
    /**
     * @brief ������ģʽѡ�� \n
     * @see ECD_Mode_T1 \n
     * @see ECD_Mode_T1_T2 \n
     * @see ECD_Mode_CCWSIGN \n
     * @see ECD_Mode_CCWCW \n
     * @see ECD_Mode_CCWSIGN_UpDown \n
     * @see ECD_Mode_CCWCW_UpDown
     */
    uint32_t ECD_Mode;    
    uint32_t ECD_TH;       /**< ��������������ֵ*/
    /**
     * @brief ��������ӦUTIMERʱ�ӷ�Ƶϵ�� \n
     * @see ECD_Clk_Div1 \n
     * @see ECD_Clk_Div2 \n
     * @see ECD_Clk_Div4 \n
     * @see ECD_Clk_Div8 
     */    
    uint32_t ECD_ClockDiv; 

    uint32_t ECD_Filter0;  /**< ������0�˲���Ƶ0:���˲� N:�˲����õ�ʱ����ϵͳʱ��N��Ƶ*/
    uint32_t ECD_Filter1;  /**< ������1�˲���Ƶ0:���˲� N:�˲����õ�ʱ����ϵͳʱ��N��Ƶ*/
     /**
     * @brief �������ж�ʹ�� \n
     * @see ECD_IRQEna_None \n
     * @see ECD_IRQEna_UnderFlow \n
     * @see ECD_IRQEna_OverFlow 
     */       
    uint32_t ECD_IRQEna;   
} TIM_ECDInitTypeDef;


/*Timer��ʼ��*/
void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *TIM_TimerInitStruct);
void TIM_TimerStrutInit(TIM_TimerInitTypeDef *TIM_TimerInitStruct);
// void TIM_StartTimer(TIM_TimerTypeDef *TIMERx);
// void TIM_StopTimer(TIM_TimerTypeDef *TIMERx);
void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state);

/*TIM��ȡ�жϱ�־*/
uint32_t TIM_GetIRQFlag(TIM_TimerTypeDef *TIMERx , u32 timer_if);
/*TIM�ж����*/
void TIM_ClearIRQFlag(TIM_TimerTypeDef *TIMERx ,uint32_t tempFlag);

uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIM_TIMERx);
uint32_t TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIM_TIMERx);
uint32_t TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIM_TIMERx);

/*ECD��ʼ��*/
void TIM_ECDInit(TIM_ECDTypeDef *ECDx, TIM_ECDInitTypeDef *TIM_ECDInitStruct);
void TIM_ECDStructInit(TIM_ECDInitTypeDef *TIM_ECDInitStruct);
void TIM_ECDCmd(TIM_ECDTypeDef *ECDx, FuncState state);

uint32_t TIM_ECD_GetCount(TIM_ECDTypeDef *TIM_ECDx);
/*ECD��ȡ�жϱ�־*/
uint32_t ECD_GetIRQFlag(TIM_ECDTypeDef *ECDx , u32 timer_if);
/*ECD�ж����*/
void ECD_ClearIRQFlag(TIM_ECDTypeDef *ECDx , uint32_t tempFlag);
#endif /*__lks32mc08x_TIM_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
