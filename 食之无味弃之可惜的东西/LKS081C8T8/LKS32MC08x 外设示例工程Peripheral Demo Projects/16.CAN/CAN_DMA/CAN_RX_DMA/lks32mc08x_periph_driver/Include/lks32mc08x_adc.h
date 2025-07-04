/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_adc.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ADC����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2018��07��25�� <td>1.0     <td>Howlet      <td>����
 * </table>
 */
#ifndef _LKS32MC08x_ADC_H_
#define _LKS32MC08x_ADC_H_


/** 
 *@brief ADC�Ĵ����ṹ����
 */
#include "lks32mc08x_lib.h"
typedef struct
{
    __IO uint32_t DAT0;   /**< ADC���ݼĴ���0 */
    __IO uint32_t DAT1;   /**< ADC���ݼĴ���1 */
    __IO uint32_t DAT2;   /**< ADC���ݼĴ���2 */
    __IO uint32_t DAT3;   /**< ADC���ݼĴ���3 */
    __IO uint32_t DAT4;   /**< ADC���ݼĴ���4 */
    __IO uint32_t DAT5;   /**< ADC���ݼĴ���5 */
    __IO uint32_t DAT6;   /**< ADC���ݼĴ���6 */
    __IO uint32_t DAT7;   /**< ADC���ݼĴ���7 */
    __IO uint32_t DAT8;   /**< ADC���ݼĴ���8 */
    __IO uint32_t DAT9;   /**< ADC���ݼĴ���9 */
    __IO uint32_t DAT10;  /**< ADC���ݼĴ���10 */
    __IO uint32_t DAT11;  /**< ADC���ݼĴ���11 */
    __IO uint32_t DAT12;  /**< ADC���ݼĴ���12 */
    __IO uint32_t DAT13;  /**< ADC���ݼĴ���13 */
    __IO uint32_t DAT14;  /**< ADC���ݼĴ���14 */
    __IO uint32_t DAT15;  /**< ADC���ݼĴ���15 */
    __IO uint32_t DAT16;  /**< ADC���ݼĴ���16 */
    __IO uint32_t DAT17;  /**< ADC���ݼĴ���17 */
    __IO uint32_t DAT18;  /**< ADC���ݼĴ���18 */
    __IO uint32_t DAT19;  /**< ADC���ݼĴ���19 */

    __IO uint32_t CHN0;   /**< ADC�����ź���Դ�Ĵ���0 */
    __IO uint32_t CHN1;   /**< ADC�����ź���Դ�Ĵ���1 */
    __IO uint32_t CHN2;   /**< ADC�����ź���Դ�Ĵ���2 */
    __IO uint32_t CHN3;   /**< ADC�����ź���Դ�Ĵ���3 */
    __IO uint32_t CHN4;   /**< ADC�����ź���Դ�Ĵ���4 */
    __IO uint32_t CHN5;   /**< ADC�����ź���Դ�Ĵ���5 */
    __IO uint32_t CHN6;   /**< ADC�����ź���Դ�Ĵ���6 */
    __IO uint32_t CHN7;   /**< ADC�����ź���Դ�Ĵ���7 */
    __IO uint32_t CHN8;   /**< ADC�����ź���Դ�Ĵ���8 */
    __IO uint32_t CHN9;   /**< ADC�����ź���Դ�Ĵ���9 */

    __IO uint32_t CHNT0;   /**< ADC�ֶ�ͨ�����Ĵ���0 ����ÿ�β������� */
    __IO uint32_t CHNT1;   /**< ADC�ֶ�ͨ�����Ĵ���1 ����ÿ�β������� */
    __IO uint32_t SD0;     /**< ���� */
    __IO uint32_t SD1;     /**< ���� */
    __IO uint32_t GAIN_0;  /**< ADC����ѡ��Ĵ���0 */
    __IO uint32_t GAIN_1;  /**< ADC����ѡ��Ĵ���1 */
		/* �������ֵ�·A��Ӧͨ����Χ�ǵ�0��2��4��ż�����ͨ�� */
    __IO uint32_t DC_A0;   /**< ADCֱ��ƫ�üĴ���0 �������ֵ�·A��Ӧ2/3������ͨ��ƫ�� */
    __IO uint32_t DC_A1;   /**< ADCֱ��ƫ�üĴ���1 �������ֵ�·A��Ӧ 1 ������ͨ��ƫ�� */
    __IO uint32_t AMC_A0;  /**< ADC����У���Ĵ���0 �������ֵ�·A��Ӧ2/3������ͨ������У�� */
    __IO uint32_t AMC_A1;  /**< ADC����У���Ĵ���0 �������ֵ�·A��Ӧ 1 ������ͨ������У�� */
		/* �������ֵ�·B��Ӧͨ����Χ�ǵ�1��3��5���������ͨ�� */
    __IO uint32_t DC_B0;   /**< ADCֱ��ƫ�üĴ���0 �������ֵ�·B��Ӧ2/3������ͨ��ƫ�� */
    __IO uint32_t DC_B1;   /**< ADCֱ��ƫ�üĴ���1 �������ֵ�·B��Ӧ1������ͨ��ƫ�� */
    __IO uint32_t AMC_B0;  /**< ADC����У���Ĵ���0 �������ֵ�·B��Ӧ2/3������ͨ������У�� */
    __IO uint32_t AMC_B1;  /**< ADC����У���Ĵ���0 �������ֵ�·B��Ӧ 1 ������ͨ������У�� */

    __IO uint32_t IE;      /**< ADC�жϼĴ��� */  
    __IO uint32_t IF;      /**< ADC�жϱ�־λ�Ĵ��� */ 
    __IO uint32_t CFG;     /**< ADCģʽ���üĴ��� */
    __IO uint32_t TRG;     /**< ADC�������ƼĴ��� */
    __IO uint32_t SWT;     /**< ADC��������Ĵ��� */
    __IO uint32_t DAT0_TH; /**< ADCͨ��0��ֵ�Ĵ��� */
} ADC_TypeDef;

/**
 * ADC�ṹ�����ַ����
 */
#ifndef  ADC0
#define  ADC0     ((ADC_TypeDef *) ADC0_BASE)
#endif
/** 
 *@brief ADC��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
    /** 
     * @brief ADC�ж�ʹ�� \n
     * @see ADC_DAT0_OV_IRQ_EN  \n   
     * @see ADC_H_CONFLICT_IRQ_EN  \n
     * @see ADC_S_CONFLICT_IRQ_EN  \n
     * @see ADC_EOS3_IRQ_EN     \n   
     * @see ADC_EOS2_IRQ_EN     \n     
     * @see ADC_EOS1_IRQ_EN     \n   
     * @see ADC_EOS0_IRQ_EN        
     */  
    uint16_t IE; 
     /**
     * @brief �������ݶ��뷽ʽ \n
     * @see ADC_LEFT_ALIGN  \n
     * @see ADC_RIGHT_ALIGN   \n  
     */ 
    uint16_t Align;               
    /**
     * @brief ADCʱ�ӷ�Ƶ \n
     * @see ADC_Clock_48M \n
     * @see ADC_Clock_24M  \n
     * @see ADC_Clock_12M  \n 
     */ 
    uint16_t ADC_CLK_DIV;  
    uint16_t Con_Sample;                   /**< ��������ģʽ DISABLE:�رգ�ENABLE:ʹ��*/
    /**
     * @brief Trigger_Cnt����1������Ҫ����ADC���βɽ���һ��ADC����ת�������÷�Χ0~15����ʾ��Ҫ1~16�δ����¼�
     */ 
    uint16_t Trigger_Cnt;                  /**< ����ģʽ�´�����һ�β�������Ĵ����¼��� */

     /**
     * @brief ʹ�øöβ�����Ӧ�εĲ���������������Ϊ0�����δ�øöβ�������������������Ϊ0��
     */ 
    uint16_t FirSeg_Ch;                    /**< ��һ�β�������ͨ���� */ 
    /**
     * @brief ʹ�øöβ�����Ӧ�εĲ���������������Ϊ0�����δ�øöβ�������������������Ϊ0��
     */ 
    uint16_t SecSeg_Ch;                    /**< �ڶ��β�������ͨ���� */
    /**
     * @brief ʹ�øöβ�����Ӧ�εĲ���������������Ϊ0�����δ�øöβ�������������������Ϊ0��
     */ 
    uint16_t ThrSeg_Ch;                    /**< �����β�������ͨ���� */
    /**
     * @brief ʹ�øöβ�����Ӧ�εĲ���������������Ϊ0�����δ�øöβ�������������������Ϊ0��
     */ 
    uint16_t FouSeg_Ch;                    /**< ���Ķβ�������ͨ���� */
     /**
     * @brief ADC ����ģʽ ���Ρ�˫�λ��Ķ� \n
     * @see ADC_1SEG_TRG  \n
     * @see ADC_2SEG_TRG  \n  
     * @see ADC_4SEG_TRG  
     */
    uint16_t Trigger_Mode;          
     /**
     * @brief MCPWM��������ʹ�� \n
     * @see ADC_MCPWM_T0_TRG \n 
     * @see ADC_MCPWM_T1_TRG \n  
     * @see ADC_MCPWM_T2_TRG \n 
     * @see ADC_MCPWM_T3_TRG 
     */  
    uint16_t MCPWM_Trigger_En;          
    /**
     * @brief UTIMER��������ʹ�� \n
     * @see ADC_UTIMER_T0_TRG \n
     * @see ADC_UTIMER_T1_TRG \n 
     * @see ADC_UTIMER_T2_TRG \n 
     * @see ADC_UTIMER_T3_TRG \n 
     */    
    uint16_t UTIMER_Trigger_En;          
    uint16_t DAT0_TH_Type;                 /**< ����DAT0_TH��Ϊ��1����ֵ��0����ֵ */
	  uint16_t DAT0_TH;                      /**< ͨ��0��ֵ���� */
	  uint16_t Gain0;                        /**< ADC_DAT0-ADC_DAT9�������ֵ�·���� BIT0~BIT9���Ի�BIT0|BIT1*/
	  uint16_t Gain1;                        /**< ADC_DAT10-ADC_DAT19�������ֵ�·���� BIT0~BIT9���Ի�BIT0|BIT1 */
    /**
     * @brief ��׼��ѹѡ�� \n
     * @see RefVol_1V2 \n
     * @see RefVol_2V4   
     */    
    uint16_t RefVol;                
} ADC_InitTypeDef;


#define ADC_Clock_48M                     (0x00)  /*48MHz*/  
#define ADC_Clock_24M                     (0x03)  /*24MHz*/     
#define ADC_Clock_12M                     (0x02)  /*12MHz*/     

#define RefVol_2V4                        ((uint16_t)0x00)/*ADC��׼��ѹ2.4V*/
#define RefVol_1V2                        ((uint16_t)0x01)/*ADC��׼��ѹ1.2V*/

#define ADC_1_TIMES_SAMPLE                ((u8)1)  /**< ADC����ͨ����1�β��� */
#define ADC_2_TIMES_SAMPLE                ((u8)2)  /**< ADC����ͨ����2�β��� */
#define ADC_3_TIMES_SAMPLE                ((u8)3)  /**< ADC����ͨ����3�β��� */
#define ADC_4_TIMES_SAMPLE                ((u8)4)  /**< ADC����ͨ����4�β��� */
#define ADC_5_TIMES_SAMPLE                ((u8)5)  /**< ADC����ͨ����5�β��� */
#define ADC_6_TIMES_SAMPLE                ((u8)6)  /**< ADC����ͨ����6�β��� */
#define ADC_7_TIMES_SAMPLE                ((u8)7)  /**< ADC����ͨ����7�β��� */
#define ADC_8_TIMES_SAMPLE                ((u8)8)  /**< ADC����ͨ����8�β��� */
#define ADC_9_TIMES_SAMPLE                ((u8)9)  /**< ADC����ͨ����9�β��� */
#define ADC_10_TIMES_SAMPLE               ((u8)10) /**< ADC����ͨ����10�β��� */
#define ADC_11_TIMES_SAMPLE               ((u8)11) /**< ADC����ͨ����11�β��� */
#define ADC_12_TIMES_SAMPLE               ((u8)12) /**< ADC����ͨ����12�β��� */
#define ADC_13_TIMES_SAMPLE               ((u8)13) /**< ADC����ͨ����13�β��� */
#define ADC_14_TIMES_SAMPLE               ((u8)14) /**< ADC����ͨ����14�β��� */
#define ADC_15_TIMES_SAMPLE               ((u8)15) /**< ADC����ͨ����15�β��� */
#define ADC_16_TIMES_SAMPLE               ((u8)16) /**< ADC����ͨ����16�β��� */
#define ADC_17_TIMES_SAMPLE               ((u8)17) /**< ADC����ͨ����17�β��� */
#define ADC_18_TIMES_SAMPLE               ((u8)18) /**< ADC����ͨ����18�β��� */
#define ADC_19_TIMES_SAMPLE               ((u8)19) /**< ADC����ͨ����19�β��� */
#define ADC_20_TIMES_SAMPLE               ((u8)20) /**< ADC����ͨ����20�β��� */

#define ADC_DAT0_OV_IRQ_EN                ((uint16_t)0x40) /**< ADC0_DAT0����ֵ�ж� */
#define ADC_H_CONFLICT_IRQ_EN             ((uint16_t)0x20) /**< Ӳ��������ͻ */
#define ADC_S_CONFLICT_IRQ_EN             ((uint16_t)0x10) /**< ���������ͻ */
#define ADC_EOS3_IRQ_EN                   ((uint16_t)0x08) /**< ���Ķ�ɨ������ж� */
#define ADC_EOS2_IRQ_EN                   ((uint16_t)0x04) /**< ������ɨ������ж� */
#define ADC_EOS1_IRQ_EN                   ((uint16_t)0x02) /**< �ڶ���ɨ������ж� */
#define ADC_EOS0_IRQ_EN                   ((uint16_t)0x01) /**< ��һ��ɨ������ж� */

#define ADC_DAT0_OV_IRQ_IF                ((uint16_t)0x40) /**< ADC_DAT0����ֵ�жϱ�־λ */
#define ADC_H_CONFLICT_IRQ_IF             ((uint16_t)0x20) /**< Ӳ��������ͻ��־ */
#define ADC_S_CONFLICT_IRQ_IF             ((uint16_t)0x10) /**< ���������ͻ��־ */
#define ADC_EOS3_IRQ_IF                   ((uint16_t)0x08) /**< ���Ķ�ɨ������жϱ�־ */
#define ADC_EOS2_IRQ_IF                   ((uint16_t)0x04) /**< ������ɨ������жϱ�־ */
#define ADC_EOS1_IRQ_IF                   ((uint16_t)0x02) /**< �ڶ���ɨ������жϱ�־ */
#define ADC_EOS0_IRQ_IF                   ((uint16_t)0x01) /**< ��һ��ɨ������жϱ�־ */
#define ADC_ALL_IRQ_IF                    ((uint16_t)0x7f) /**< ADCȫ���жϱ�־λ���������ڳ�ʼ�� */

#define ADC_LEFT_ALIGN                    ((uint16_t)0x0000) /**< ADC������������ */
#define ADC_RIGHT_ALIGN                   ((uint16_t)0x0001) /**< ADC��������Ҷ��� */

#define ADC_DAT0_HTH                      ((uint16_t)0x0001) /**< ADC_DAT0_TH��Ϊ����ֵ */
#define ADC_DAT0_LTH                      ((uint16_t)0x0000) /**< ADC_DAT0_TH��Ϊ����ֵ */

#define ADC_MCPWM_T0_TRG                  ((uint16_t)0x01)   /**< ADC����MCPWM T0�¼����� */
#define ADC_MCPWM_T1_TRG                  ((uint16_t)0x02)   /**< ADC����MCPWM T1�¼����� */
#define ADC_MCPWM_T2_TRG                  ((uint16_t)0x04)   /**< ADC����MCPWM T2�¼����� */
#define ADC_MCPWM_T3_TRG                  ((uint16_t)0x08)   /**< ADC����MCPWM T3�¼����� */
#define ADC_UTIMER_T0_TRG                 ((uint16_t)0x01)   /**< ADC����UTIMER T0�¼����� */
#define ADC_UTIMER_T1_TRG                 ((uint16_t)0x02)   /**< ADC����UTIMER T1�¼����� */
#define ADC_UTIMER_T2_TRG                 ((uint16_t)0x04)   /**< ADC����UTIMER T2�¼����� */
#define ADC_UTIMER_T3_TRG                 ((uint16_t)0x08)   /**< ADC����UTIMER T3�¼����� */

#define ADC_1SEG_TRG                      ((uint16_t)0x00)   /**< ADC���õ���ģʽ */
#define ADC_2SEG_TRG                      ((uint16_t)0x01)   /**< ADC����2��ģʽ */
#define ADC_4SEG_TRG                      ((uint16_t)0x03)   /**< ADC����4��ģʽ */

#define ADC_CHANNEL_0                     ((uint16_t)0x00)   /**< ADCͨ��0-OPA0_OUT */
#define ADC_CHANNEL_1                     ((uint16_t)0x01)   /**< ADCͨ��1-OPA1_OUT */
#define ADC_CHANNEL_2                     ((uint16_t)0x02)   /**< ADCͨ��2-OPA2_OUT */
#define ADC_CHANNEL_3                     ((uint16_t)0x03)   /**< ADCͨ��3-OPA3_OUT */
#define ADC_CHANNEL_4                     ((uint16_t)0x04)   /**< ADCͨ��4 */
#define ADC_CHANNEL_5                     ((uint16_t)0x05)   /**< ADCͨ��5 */
#define ADC_CHANNEL_6                     ((uint16_t)0x06)   /**< ADCͨ��6 */
#define ADC_CHANNEL_7                     ((uint16_t)0x07)   /**< ADCͨ��7 */
#define ADC_CHANNEL_8                     ((uint16_t)0x08)   /**< ADCͨ��8 */
#define ADC_CHANNEL_9                     ((uint16_t)0x09)   /**< ADCͨ��9 */
#define ADC_CHANNEL_10                    ((uint16_t)0x0a)   /**< ADCͨ��10 */
#define ADC_CHANNEL_11                    ((uint16_t)0x0b)   /**< ADCͨ��11 */
#define ADC_CHANNEL_12                    ((uint16_t)0x0c)   /**< ADCͨ��12 */
#define ADC_CHANNEL_13                    ((uint16_t)0x0d)   /**< ADCͨ��13 */
#define ADC_CHANNEL_14                    ((uint16_t)0x0e)   /**< ADCͨ��14 */
#define ADC_CHANNEL_15                    ((uint16_t)0x0f)   /**< ADCͨ��15 */
#define ADC_CHANNEL_16                    ((uint16_t)0x10)   /**< ADCͨ��16 */
#define ADC_CHANNEL_17                    ((uint16_t)0x11)   /**< ADCͨ��17 */
#define ADC_CHANNEL_18                    ((uint16_t)0x12)   /**< ADCͨ��18-Temp */
#define ADC_CHANNEL_19                    ((uint16_t)0x13)   /**< ADCͨ��19-VSS */


#define ADC_HARDWARE_T0_TRG               ((uint16_t)0x01)   /**< ADC����Ӳ��T0�¼����� */
#define ADC_HARDWARE_T1_TRG               ((uint16_t)0x02)   /**< ADC����Ӳ��T1�¼����� */
#define ADC_HARDWARE_T2_TRG               ((uint16_t)0x04)   /**< ADC����Ӳ��T2�¼����� */
#define ADC_HARDWARE_T3_TRG               ((uint16_t)0x08)   /**< ADC����Ӳ��T3�¼����� */

/** ADC�����ź���Դ�Ĵ������*/ 
typedef enum {
	CHN0 = 0, /**< ��0�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN1,     /**< ��1�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN2,     /**< ��2�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN3,     /**< ��3�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN4,     /**< ��4�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN5,     /**< ��5�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN6,     /**< ��6�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN7,     /**< ��7�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN8,     /**< ��8�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN9,     /**< ��9�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
	CHN10,    /**< ��10�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN11,    /**< ��11�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN12,    /**< ��12�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN13,    /**< ��13�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN14,    /**< ��14�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN15,    /**< ��15�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN16,    /**< ��16�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN17,    /**< ��17�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN18,    /**< ��18�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
  CHN19,    /**< ��19�β����źŶ�Ӧ�ź���Դ�Ĵ������*/
} CHNx;    

/** ADC���ݼĴ������*/ 
typedef enum {
  DAT0 = 0,  /**< ��0�β����źŶ�Ӧ����Ĵ������*/
  DAT1,      /**< ��1�β����źŶ�Ӧ����Ĵ������*/
  DAT2,      /**< ��2�β����źŶ�Ӧ����Ĵ������*/
  DAT3,      /**< ��3�β����źŶ�Ӧ����Ĵ������*/
  DAT4,      /**< ��4�β����źŶ�Ӧ����Ĵ������*/
  DAT5,      /**< ��5�β����źŶ�Ӧ����Ĵ������*/
  DAT6,      /**< ��6�β����źŶ�Ӧ����Ĵ������*/
  DAT7,      /**< ��7�β����źŶ�Ӧ����Ĵ������*/
  DAT8,      /**< ��8�β����źŶ�Ӧ����Ĵ������*/
  DAT9,      /**< ��9�β����źŶ�Ӧ����Ĵ������*/
  DAT10,     /**< ��10�β����źŶ�Ӧ����Ĵ������*/
  DAT11,     /**< ��11�β����źŶ�Ӧ����Ĵ������*/
  DAT12,     /**< ��12�β����źŶ�Ӧ����Ĵ������*/
  DAT13,     /**< ��13�β����źŶ�Ӧ����Ĵ������*/
  DAT14,     /**< ��14�β����źŶ�Ӧ����Ĵ������*/
  DAT15,     /**< ��15�β����źŶ�Ӧ����Ĵ������*/
  DAT16,     /**< ��16�β����źŶ�Ӧ����Ĵ������*/
  DAT17,     /**< ��17�β����źŶ�Ӧ����Ĵ������*/
  DAT18,     /**< ��18�β����źŶ�Ӧ����Ĵ������*/
  DAT19,     /**< ��19�β����źŶ�Ӧ����Ĵ������*/
} DATx;

void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_ClearIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag);
void ADC_ChannelConfig(ADC_TypeDef* ADCx,CHNx CHNum, uint16_t nChannel);
u16 ADC_GetIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag);
s16 ADC_GetConversionValue(DATx DATNum);/* ȡ��Ӧ���ADC���ݼĴ�����ֵ */
void ADC_SoftTrgEN(ADC_TypeDef* ADCx, FuncState state);
#endif /*_CONNIE_ADC_H_ */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
