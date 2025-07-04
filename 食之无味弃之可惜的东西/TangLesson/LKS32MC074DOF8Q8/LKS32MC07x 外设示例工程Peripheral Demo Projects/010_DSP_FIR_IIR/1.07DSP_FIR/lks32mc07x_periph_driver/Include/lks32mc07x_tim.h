/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_tim.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� TIM����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0      <td>HungMG     <td>����
 * </table>
 */
#ifndef __lks32mc07x_TIM_H
#define __lks32mc07x_TIM_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"


typedef struct
{
    __IO uint32_t CFG;  /**<��ʱ�����üĴ���*/
    __IO uint32_t TH;   /**<��ʱ���������޼Ĵ���*/
    __IO uint32_t CNT;  /**<��ʱ������ֵ�Ĵ���*/
    __IO uint32_t CMP0; /**<��ʱ���Ƚ�/����Ĵ��� 0*/
    __IO uint32_t CMP1; /**<��ʱ���Ƚ�/����Ĵ��� 1*/
    __IO uint32_t EVT;  /**<��ʱ���ⲿ�¼�ѡ��Ĵ���*/
    __IO uint32_t FLT;  /**<��ʱ���˲����ƼĴ���*/
    __IO uint32_t IE;   /**<��ʱ���ж�ʹ�ܼĴ���*/
    __IO uint32_t IF;   /**<��ʱ���жϱ�־�Ĵ���*/
    __IO uint32_t IO;   /**<��ʱ��IO���ƼĴ���*/
} TIM_TimerTypeDef;

/**
 * TIMER0�ṹ�����ַ����
 */
#define UTIMER0         ((TIM_TimerTypeDef *) TIMER0_BASE)
/**
 * TIMER1�ṹ�����ַ����
 */
#define UTIMER1         ((TIM_TimerTypeDef *) TIMER1_BASE)
/**
 * TIMER2�ṹ�����ַ����
 */
#define UTIMER2         ((TIM_TimerTypeDef *) TIMER2_BASE)
/**
 * TIMER3�ṹ�����ַ����
 */
#define UTIMER3         ((TIM_TimerTypeDef *) TIMER3_BASE)


 /**
 * @brief �ж�ʹ�����ö���
 */
typedef enum
{
    Timer_IRQEna_None = 0,              /**< �ر������ж�*/
    Timer_IRQEna_CH0  = BIT0,           /**< ͨ�� 0 �ж�*/
    Timer_IRQEna_CH1  = BIT1,           /**< ͨ�� 1 �ж�*/
    Timer_IRQEna_ZC   = BIT2,           /**< �������� 0 �ж�*/
    Timer0_IRQEna_FAIL = BIT3,          /**< ��ʱ��0��FAIL�¼��ж�*/
	
	  Timer_IRQEna_CH0_RE = BIT8,       /**< ͨ��0�Ƚ�/����DMA����ʹ��*/
	  Timer_IRQEna_CH1_RE = BIT9,       /**< ͨ��1�Ƚ�/����DMA����ʹ��*/
	  Timer_IRQEna_ZC_RE = BIT10,       /**< ��������0 DMA����ʹ��*/
	  Timer0_IRQEna_FAIL_RE = BIT11,    /**< ��ʱ��0��FAIL�¼�DMA����ʹ��*/
	
    Timer_IRQEna_All  = 0xf             /**< ���������ж�*/
} Timer_IRQEnaDef;

/*�жϱ�־����*/
#define TIMER_IF_CH0               BIT0     /**< Timer0 CH0�жϣ��Ƚϻ򲶻��ж�*/
#define TIMER_IF_CH1               BIT1     /**< Timer0 CH1�жϣ��Ƚϻ򲶻��ж�*/
#define TIMER_IF_ZERO              BIT2     /**< Timer0 �����ж�*/
#define TIMER_IF_FAIL              BIT3     /**< Fail �¼��жϱ�־*/

#define TIMER_CLK_DIV1             0x00     /**< Timer������Ƶ��1��Ƶ*/
#define TIMER_CLK_DIV2             0x01     /**< Timer������Ƶ��2��Ƶ*/
#define TIMER_CLK_DIV4             0x02     /**< Timer������Ƶ��4��Ƶ*/
#define TIMER_CLK_DIV8             0x03     /**< Timer������Ƶ��8��Ƶ*/
#define TIMER_CLK_DIV16            0x04     /**< Timer������Ƶ��16��Ƶ*/
#define TIMER_CLK_DIV32            0x05     /**< Timer������Ƶ��32��Ƶ*/
#define TIMER_CLK_DIV64            0x06     /**< Timer������Ƶ��64��Ƶ*/
#define TIMER_CLK_DIV128           0x07     /**< Timer������Ƶ��128��Ƶ*/

#define CAP_SRC_TCH0               0X00     /**< ����ʱ����Ӧͨ��0�����ź�*/ 
#define CAP_SRC_TCH1               0X01     /**< ����ʱ����Ӧͨ��1�����ź�*/ 
#define CAP_SRC_CLU0               0X02     /**< ����CLU0����ź�*/ 
#define CAP_SRC_CLU1               0X03     /**< ����CLU1����ź�*/ 
#define CAP_SRC_CLU2               0X04     /**< ����CLU2����ź�*/ 
#define CAP_SRC_CLU3               0X05     /**< ����CLU3����ź�*/ 
#define CAP_SRC_CMP0               0X06     /**< ����Ƚ���0����ź�*/ 
#define CAP_SRC_CMP1               0X07     /**< ����Ƚ���1����ź�*/
#define CAP_SRC_CMP2               0X08     /**< ����Ƚ���2����ź�*/
#define CAP_SRC_CHXOR              0X09     /**< ����Timerͨ��0��1������ź�*/


#define TIMER_POL_HIGH             0x01     /**< �Ƚ�ģʽ�µ�����ߵ�ƽ*/
#define TIMER_POL_LOW              0x00     /**< �Ƚ�ģʽ�µ�����͵�ƽ*/

#define TIMER_OPMode_CAP ((uint32_t)0x0000001)    /**< ����ģʽ*/
#define TIMER_OPMode_CMP ((uint32_t)0x0000000)    /**< �Ƚ�ģʽ*/

#define TIMER0_EVT_CH0    0x00                    /**< TIMER0ͨ��0*/
#define TIMER0_EVT_CH1    0x01                    /**< TIMER0ͨ��1*/
#define TIMER1_EVT_CH0    0x02                    /**< TIMER1ͨ��0*/
#define TIMER1_EVT_CH1    0x03                    /**< TIMER1ͨ��1*/
#define TIMER2_EVT_CH0    0x04                    /**< TIMER2ͨ��0*/
#define TIMER2_EVT_CH1    0x05                    /**< TIMER2ͨ��1*/
#define TIMER3_EVT_CH0    0x06                    /**< TIMER3ͨ��0*/
#define TIMER3_EVT_CH1    0x07                    /**< TIMER3ͨ��1*/
#define TIMER_EVT_CLU0    0x08                    /**< CLU0 ����ź�*/
#define TIMER_EVT_CLU1    0x09                    /**< CLU1 ����ź�*/
#define TIMER_EVT_CLU2    0x0A                    /**< CLU2 ����ź�*/
#define TIMER_EVT_CLU3    0x0B                    /**< CLU3 ����ź�*/
#define TIMER_EVT_TADC0   0x0C                    /**< MCPWM TADC[0]�Ƚ��¼�*/
#define TIMER_EVT_TADC1   0x0D                    /**< MCPWM TADC[1]�Ƚ��¼�*/
#define TIMER_EVT_TADC2   0x0E                    /**< MCPWM TADC[2]�Ƚ��¼�*/
#define TIMER_EVT_TADC3   0x0F                    /**< MCPWM TADC[3]�Ƚ��¼�*/

#define TIMER_CH0_IF     BIT0                     /**< Timerͨ��0�Ƚ�/�����жϱ�־*/ 
#define TIMER_CH1_IF     BIT1                     /**< Timerͨ��1�Ƚ�/�����жϱ�־*/
#define TIMER_ZC_IF      BIT2                     /**< Timer��������0�жϱ�־*/
#define TIMER_FAIL_IF    BIT3                     /**< Timer Fail�¼��жϱ�־*/


typedef struct
{
	  FuncState Timer_EN;          /**< ��ʱ��ʹ�ܣ�ENABLEʹ�ܣ�DISABLEʧ��*/
	  uint32_t Timer_CAP1_CLR_EN;  /**< ������CAP1�����¼�ʱ������ Timer ������������Ч*/
	  uint32_t Timer_CAP0_CLR_EN;  /**< ������CAP0�����¼�ʱ������ Timer ������������Ч*/
	
	  uint32_t Timer_ONE_TRIG;     /**< ���η���ģʽ����λ��Ҫ��Timer�Ƚ�ģʽ��ʹ�ã��Ҷ�Ӧ��ʱ��ʹ��������Ϊ0*/
    
	  uint32_t Timer_CENTER;       /**< ���ļ���ģʽʹ��,0:Timer���ϴ�0������TH��Ȼ���0����Timer����
	                               ��TH������0��Ȼ��ص�TH��1:Timer���ϴ�0������TH��Ȼ�����¼�����0*/
	  uint32_t Timer_DIR;          /**< 0:0->TH ����������1��TH->�ݼ�����*/
     /**
     * @brief ��ʱ����Ƶ \n
     * @see TIM_Clk_Div1 \n
     * @see TIM_Clk_Div2 \n
     * @see TIM_Clk_Div4 \n
     * @see TIM_Clk_Div8 \n
     * @see TIM_Clk_Div16 \n
     * @see TIM_Clk_Div32 \n
     * @see TIM_Clk_Div64 \n
     * @see TIM_Clk_Div128 
     */
	  uint32_t Timer_ClockDiv;     
	  uint32_t Timer_ETON;         /**< Timer�����������ⲿ����ʹ��,0: �Զ�����,1:�ⲿ�¼���������*/
  	uint32_t Timer_GATE_EN;        /**< Timer��ͣʹ�ܣ�0������ͣ 1:���ⲿ�ź�Ϊ��ʱ��Timer��ͣ������
	                                 �ⲿ�źŸ���TIMER1_EVT.EVT_SRC ����ѡ��*/
	  uint32_t Timer_RL_EN;        /**< Timer��װʹ��,0�������ⲿ�¼���װ��1��ʹ���ⲿ�¼���װ*/ 
	  uint32_t Timer_XCLK_EN;      /**< Timerʱ��Դ,0: оƬ�ڲ�ʱ��,1:�ⲿʱ��*/
     /**
     * @brief ��ʱ����Ƶ \n
     * @see CAP_SRC_TCH0 \n
     * @see CAP_SRC_TCH1 \n
     * @see CAP_SRC_CLU0 \n
     * @see CAP_SRC_CLU1 \n
     * @see CAP_SRC_CLU2 \n
     * @see CAP_SRC_CLU3 \n
     * @see CAP_SRC_CMP0 \n
     * @see CAP_SRC_CMP1 \n
     * @see CAP_SRC_CMP2 \n
     * @see CAP_SRC_CHXOR 
     */	  
	  uint32_t Timer_SRC1;         
	  uint32_t Timer_CH1Output;    /**< Timer ͨ��1�ڱȽ�ģʽ�µ�������Կ��ƣ��������� CNT<CMP1ʱ�����ֵ��0���͵�ƽ��1���ߵ�ƽ*/
	  uint32_t Timer_CH1_WorkMode; /**< Timer ͨ��1�Ĺ���ģʽѡ��,0: �Ƚ�ģʽ ,1: ����ģʽ*/
	  uint32_t Timer_CH1_FE_CAP_EN;/**< Timerͨ��1�����ز����¼�ʹ��,1:ʹ��,0:�ر�*/
	  uint32_t Timer_CH1_RE_CAP_EN;/**< Timerͨ��1�����ز����¼�ʹ��,1:ʹ��,0:�ر�*/
     /**
     * @brief ��ʱ����Ƶ \n
     * @see CAP_SRC_TCH0 \n
     * @see CAP_SRC_TCH1 \n
     * @see CAP_SRC_CLU0 \n
     * @see CAP_SRC_CLU1 \n
     * @see CAP_SRC_CLU2 \n
     * @see CAP_SRC_CLU3 \n
     * @see CAP_SRC_CMP0 \n
     * @see CAP_SRC_CMP1 \n
     * @see CAP_SRC_CMP2 \n
     * @see CAP_SRC_CHXOR 
     */	  		
	  uint32_t Timer_SRC0;         /**< Timer ����ģʽͨ��0�ź���Դ*/
	  uint32_t Timer_CH0Output;    /**< Timer ͨ��0�ڱȽ�ģʽ�µ�������Կ��ƣ��������� CNT<CMP1ʱ�����ֵ��0���͵�ƽ��1���ߵ�ƽ*/
	  uint32_t Timer_CH0_WorkMode; /**< Timer ͨ��0�Ĺ���ģʽѡ��,0: �Ƚ�ģʽ ,1: ����ģʽ*/
	  uint32_t Timer_CH0_FE_CAP_EN;/**< Timerͨ��0�����ز����¼�ʹ��,1:ʹ��,0:�ر�*/
	  uint32_t Timer_CH0_RE_CAP_EN;/**< Timerͨ��0�����ز����¼�ʹ��,1:ʹ��,0:�ر�*/
		
	  uint32_t Timer_TH;           /**< Timer��������������*/
	  uint32_t Timer_CNT;          /**< Timer��������������*/
	  uint32_t Timer_CMP0;         /**< ͨ��0�Ƚϲ���Ĵ���*/
	  uint32_t Timer_CMP1;         /**< ͨ��1�Ƚϲ���Ĵ���*/
    /**
     * @brief ��ʱ����Ƶ \n
     * @see TIMER0_EVT_CH0 \n
     * @see TIMER0_EVT_CH1 \n
     * @see ......      \n
     * @see TIMER_EVT_TADC2 \n
     * @see TIMER_EVT_TADC3 
     */	  
    uint32_t Timer_EVT;            /**< Timer�ⲿ�¼�ѡ��*/
    uint32_t Timer_FLT;            /**< ͨ��0/1�ź��˲����ѡ��ȡֵ��Χ0~255,�˲���ʽ=Timer_FLT*8*��ʱ��ʱ��*/
    /**
     * @brief ��ʱ���ж�ʹ�� \n
     * @see Timer_IRQEna_CH0     \n
     * @see Timer_IRQEna_CH1     \n
     * @see Timer_IRQEna_ZC      \n
     * @see Timer0_IRQEna_FAIL   \n
     * @see Timer_IRQEna_CH0_RE  \n
     * @see Timer_IRQEna_CH1_RE  \n
     * @see Timer_IRQEna_ZC_RE   \n
     * @see Timer0_IRQEna_FAIL_RE 
    */     
    uint32_t Timer_IRQEna;         /**< �ж�ʹ��*/
    
    uint32_t Timer0_UPDATE;        /**< ������£�1���� TH/CMP0/CMP1 ��Ԥװ��ֵ���ص�Ӱ�ӼĴ��� STH/SCMP0/SCMP1 
                                    ֮�У�0��������*/
    uint32_t Timer0_SHADOW;        /**< 0�����д��TH/CMP0/CMP1 ʱֱ�Ӹ��¶�Ӧ��Ӱ�ӼĴ�����1������Ӱ�ӼĴ�����
                                    ���д�� TH/CMP0/CMP1 ʱ����������Ԥװ��ֵ����Timer���������¼�ʱ�Ž�Ԥ
                                    װ��ֵд��Ӱ�ӼĴ���*/
    uint32_t Timer0_CH1_DEFAULT;   /**< ��ʱ��0����Fail�¼�MOE�����CH1ͨ�����ֵ*/
	  uint32_t Timer0_CH0_DEFAULT;   /**< ��ʱ��0����Fail�¼�MOE�����CH0ͨ�����ֵ*/
    uint32_t Timer0_HALT_PRT;      /**< MCU ����HALT״̬��Timer���ֵѡ��1:���������0:��� CH1_DEFAULT �� CH0_DEFAULT��*/
    uint32_t Timer0_FAIL_SEL;      /**< FAIL�ź�ѡ��0��TIMER0_FAIL������GPIO��1��CLU0���*/
    uint32_t Timer0_FAIL_POL;      /**< FAIL�źż���,0���ߵ�ƽFAIL,1���͵�ƽFAIL*/
    uint32_t Timer0_FAIL_EN;       /**< FAIL �ź�ʹ��,0������ FAIL,1��ʹ�� FAIL*/
} TIM_TimerInitTypeDef;



/*Timer��ʼ��*/
void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *this);
void TIM_TimerStrutInit(TIM_TimerInitTypeDef *this);
void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state);
void TIM_CMP0(TIM_TimerTypeDef *TIMERx, uint32_t temp);
void TIM_CMP1(TIM_TimerTypeDef *TIMERx, uint32_t temp);
u32 TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIMERx);
u32 TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIMERx);
void TIM_ClearIRQFlag(TIM_TimerTypeDef *TIMERx,uint32_t tempFlag);
u16 TIM_GetIRQFlag(TIM_TimerTypeDef *UTIMERx, u32 TIMER_IF_x);
u16 TIM_GetFlag(TIM_TimerTypeDef *UTIMERx, u32 TIMER_IF_x);
uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIMERx);
void UTIMER_Enable(TIM_TimerTypeDef *UTIMERx);
void UTIMER_Disable(TIM_TimerTypeDef *UTIMERx);


#endif /*__lks32mc07x_TIM_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
