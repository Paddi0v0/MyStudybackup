/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC07x_mcpwm.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� MCPWM������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��3��06�� <td>1.0      <td>Zhujie    <td>����
 * </table>
 */

#ifndef __LKS08X_PWM_H
#define __LKS08X_PWM_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x_lib.h"


typedef struct
{
    __IO u32 TH00      ; /**< ƫ�Ƶ�ַ: 0x00 MCPWM CH0_P �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH01      ; /**< ƫ�Ƶ�ַ: 0x04 MCPWM CH0_N �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH10      ; /**< ƫ�Ƶ�ַ: 0x08 MCPWM CH1_P �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH11      ; /**< ƫ�Ƶ�ַ: 0x0C MCPWM CH1_N �Ƚ�����ֵ�Ĵ���*/

    __IO u32 TH20      ; /**< ƫ�Ƶ�ַ: 0x10 MCPWM CH2_P �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH21      ; /**< ƫ�Ƶ�ַ: 0x14 MCPWM CH2_N �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH30      ; /**< ƫ�Ƶ�ַ: 0x18 MCPWM CH3_P �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH31      ; /**< ƫ�Ƶ�ַ: 0x1C MCPWM CH3_N �Ƚ�����ֵ�Ĵ���*/

    __IO u32 TH40      ; /**< ƫ�Ƶ�ַ: 0x20 MCPWM CH4_P �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH41      ; /**< ƫ�Ƶ�ַ: 0x24 MCPWM CH4_N �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH50      ; /**< ƫ�Ƶ�ַ: 0x28 MCPWM CH5_P �Ƚ�����ֵ�Ĵ���*/
    __IO u32 TH51      ; /**< ƫ�Ƶ�ַ: 0x2C MCPWM CH5_N �Ƚ�����ֵ�Ĵ���*/

    __IO u32 TMR0      ; /**< ƫ�Ƶ�ַ: 0x30 ADC ������ʱ���Ƚ����� 0 �Ĵ���*/
    __IO u32 TMR1      ; /**< ƫ�Ƶ�ַ: 0x34 ADC ������ʱ���Ƚ����� 1 �Ĵ���*/
    __IO u32 TMR2      ; /**< ƫ�Ƶ�ַ: 0x38 ADC ������ʱ���Ƚ����� 2 �Ĵ���*/
    __IO u32 TMR3      ; /**< ƫ�Ƶ�ַ: 0x3C ADC ������ʱ���Ƚ����� 3 �Ĵ���*/

    __IO u32 TH0       ; /**< ƫ�Ƶ�ַ: 0x40 MCPWM ʱ�� 0 ����ֵ�Ĵ���*/
    __IO u32 TH1       ; /**< ƫ�Ƶ�ַ: 0x44 MCPWM ʱ�� 1 ����ֵ�Ĵ���*/
    __IO u32 CNT0      ; /**< ƫ�Ƶ�ַ: 0x48 MCPWM ʱ�� 0 �������Ĵ���*/
    __IO u32 CNT1      ; /**< ƫ�Ƶ�ַ: 0x4C MCPWM ʱ�� 1 �������Ĵ���*/

    __IO u32 UPDATE    ; /**< ƫ�Ƶ�ַ: 0x50 MCPWM ���ؿ��ƼĴ���*/
    __IO u32 FCNT      ; /**< ƫ�Ƶ�ַ: 0x54 MCPWM FAIL ʱ�� CNT ֵ*/
    __IO u32 EVT0      ; /**< ƫ�Ƶ�ַ: 0x58 MCPWM ʱ�� 0 �ⲿ����*/
    __IO u32 EVT1      ; /**< ƫ�Ƶ�ַ: 0x5C MCPWM ʱ�� 1 �ⲿ����*/

    __IO u32 DTH00     ; /**< ƫ�Ƶ�ַ: 0x60 MCPWM CH0/1/2 N ͨ��������ȿ��ƼĴ���*/
    __IO u32 DTH01     ; /**< ƫ�Ƶ�ַ: 0x64 MCPWM CH0/1/2 P ͨ��������ȿ��ƼĴ���*/
    __IO u32 DTH10     ; /**< ƫ�Ƶ�ַ: 0x68 MCPWM CH3/4/5 N ͨ��������ȿ��ƼĴ���*/
    __IO u32 DTH11     ; /**< ƫ�Ƶ�ַ: 0x6C MCPWM CH3/4/5 P ͨ��������ȿ��ƼĴ���*/

    __IO u32 FLT       ; /**< ƫ�Ƶ�ַ: 0x70 MCPWM �˲�ʱ�ӷ�Ƶ�Ĵ���*/
    __IO u32 SDCFG     ; /**< ƫ�Ƶ�ַ: 0x74 MCPWM �������üĴ���*/
    __IO u32 AUEN      ; /**< ƫ�Ƶ�ַ: 0x78 MCPWM �Զ�����ʹ�ܼĴ���*/
    __IO u32 TCLK      ; /**< ƫ�Ƶ�ַ: 0x7C MCPWM ʱ�ӷ�Ƶ���ƼĴ���*/

    __IO u32 IE0       ; /**< ƫ�Ƶ�ַ: 0x80 MCPWM ʱ�� 0 �жϿ��ƼĴ���*/
    __IO u32 IF0       ; /**< ƫ�Ƶ�ַ: 0x84 MCPWM ʱ�� 0 �жϱ�־λ�Ĵ���*/
    __IO u32 IE1       ; /**< ƫ�Ƶ�ַ: 0x88 MCPWM �жϿ��ƼĴ���*/
    __IO u32 IF1       ; /**< ƫ�Ƶ�ַ: 0x8C MCPWM �жϱ�־λ�Ĵ���*/

    __IO u32 EIE       ; /**< ƫ�Ƶ�ַ: 0x90 MCPWM �쳣�жϿ��ƼĴ���*/
    __IO u32 EIF       ; /**< ƫ�Ƶ�ַ: 0x94 MCPWM �쳣�жϱ�־λ�Ĵ���*/
    __IO u32 RE        ; /**< ƫ�Ƶ�ַ: 0x98 MCPWM DMA ������ƼĴ���*/
    __IO u32 PP        ; /**< ƫ�Ƶ�ַ: 0x9C MCPWM ����ģʽʹ�ܼĴ���*/

    __IO u32 IO01      ; /**< ƫ�Ƶ�ַ: 0xA0 MCPWM CH0 CH1 IO ���ƼĴ���*/
    __IO u32 IO23      ; /**< ƫ�Ƶ�ַ: 0xA4 MCPWM CH2 CH3 IO ���ƼĴ���*/
    __IO u32 IO45      ; /**< ƫ�Ƶ�ַ: 0xA8 MCPWM CH4 CH5 IO ���ƼĴ���*/
    __IO u32 null      ; /**< ƫ�Ƶ�ַ: 0xAC ��ַ����*/

    __IO u32 FAIL012   ; /**< ƫ�Ƶ�ַ: 0xB0 MCPWM CH0 CH1 CH2 ��·���ƼĴ���*/
    __IO u32 FAIL345   ; /**< ƫ�Ƶ�ַ: 0xB4 MCPWM CH3 CH4 CH5 ��·���ƼĴ���*/
    __IO u32 CH_DEF    ; /**< ƫ�Ƶ�ַ: 0xB8 MCPWM ��·����ͨ�����Ĭ��ֵ*/
    __IO u32 CH_MSK    ; /**< ƫ�Ƶ�ַ: 0xBC MCPWM ͨ�����μĴ���*/

    __IO u32 PRT       ; /**< ƫ�Ƶ�ַ: 0xC0 MCPWM �����Ĵ���*/
    __IO u32 STT_HYST  ; /**< ƫ�Ƶ�ַ: 0xC8 MCPWM ״̬ͣ����ʱ�Ĵ���*/
    __IO u32 ZCS_DELAY ; /**< ƫ�Ƶ�ַ: 0xCC MCPWM ZCS ״̬��ʱ�Ĵ���*/
} MCPWM_TypeDef;

#define MCPWM0         ((MCPWM_TypeDef *) MCPWM0_BASE)

typedef struct
{
    uint16_t TH0;           /**< MCPWMʱ��0����������ֵ��PWM���ڣ�*/
    uint16_t TH1;           /**< MCPWMʱ��1����������ֵ��PWM���ڣ�*/

    uint16_t TH00;           /**< MCPWMʱ��0 CH0P�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH01;           /**< MCPWMʱ��0 CH0N�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH10;           /**< MCPWMʱ��0 CH1P�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH11;           /**< MCPWMʱ��0 CH1N�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH20;           /**< MCPWMʱ��0 CH2P�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH21;           /**< MCPWMʱ��0 CH2N�Ƚ�����ֵ��PWMռ�ձȣ�*/

    uint16_t TH30;           /**< MCPWMʱ��1 CH3P�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH31;           /**< MCPWMʱ��1 CH3N�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH40;           /**< MCPWMʱ��1 CH4P�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH41;           /**< MCPWMʱ��1 CH4N�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH50;           /**< MCPWMʱ��1 CH5P�Ƚ�����ֵ��PWMռ�ձȣ�*/
    uint16_t TH51;           /**< MCPWMʱ��1 CH5N�Ƚ�����ֵ��PWMռ�ձȣ�*/

    uint8_t CMP_CTRL_CNT1;           /**< �Ƚ������Ƽ�����1ʹ�� */
    uint8_t CMP_CTRL_CNT0;           /**< �Ƚ������Ƽ�����0ʹ�� */

    uint8_t BASE_CNT1_EN;            /**< MCPWM ������1ʹ�ܿ��� */
    uint8_t BASE_CNT0_EN;            /**< MCPWM ������0ʹ�ܿ��� */

    uint8_t EVT_CNT1_EN;             /**<*< MCPWM ������1�ⲿ����ʹ�� */
    uint8_t EVT_CNT0_EN;             /**< MCPWM ������0�ⲿ����ʹ�� */

    uint8_t TMR3_TB;                 /**< MCPWM TMR3ʱ������������ѡ�� 0��ʱ��0��1��ʱ��1 */
    uint8_t TMR2_TB;                 /**< MCPWM TMR2ʱ������������ѡ�� 0��ʱ��0��1��ʱ��1 */

    uint8_t ZCS_EN;                  /**< ʹ��ZCS�¼���� ����Ч */

    uint8_t MCLK_EN;                 /**< MCPWM ʱ��ʹ�ܿ��� */
    uint8_t CLK_DIV;                 /**< MCPWM ��Ƶϵ�� */

    uint16_t MCPWM_WorkModeCH0;      /**< MCPWM CH0����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH1;      /**< MCPWM CH1����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH2;      /**< MCPWM CH2����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH3;      /**< MCPWM CH3����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH4;      /**< MCPWM CH4����ģʽ�����ض���/���Ķ��� */
    uint16_t MCPWM_WorkModeCH5;      /**< MCPWM CH5����ģʽ�����ض���/���Ķ��� */

    uint16_t TMR0;          /**< PWM����ADC�¼�0��ʱ������� */
    uint16_t TMR1;          /**< PWM����ADC�¼�1��ʱ������� */
    uint16_t TMR2;          /**< PWM����ADC�¼�2��ʱ������� */
    uint16_t TMR3;          /**< PWM����ADC�¼�3��ʱ������� */

    uint16_t DeadTimeCH012N;           /**< CH0N��CH1N��Ch2N����ʱ�����á�*/
    uint16_t DeadTimeCH012P;           /**< CH0P��CH1P��Ch2P����ʱ������*/
    uint16_t DeadTimeCH345N;           /**< CH3N��CH4N��Ch5N����ʱ������*/
    uint16_t DeadTimeCH345P;           /**< CH3P��CH4P��Ch5P����ʱ������*/

    uint16_t EVT0;                   /**< �ⲿ�¼�����MCPWM_CNT0 (ʱ��0)��0��ʼʹ�� */
    uint16_t EVT1;                   /**< �ⲿ�¼�����MCPWM_CNT1 (ʱ��1)��0��ʼʹ�� */

    uint8_t CH0N_Polarity_INV;       /**< CH0N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH0P_Polarity_INV;       /**< CH0P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH1N_Polarity_INV;       /**< CH1N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH1P_Polarity_INV;       /**< CH1P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH2N_Polarity_INV;       /**< CH2N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH2P_Polarity_INV;       /**< CH2P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH3N_Polarity_INV;       /**< CH3N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH3P_Polarity_INV;       /**< CH3P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH4N_Polarity_INV;       /**< CH4N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH4P_Polarity_INV;       /**< CH4P�������ȡ����0:���������1:ȡ����� */
    uint8_t CH5N_Polarity_INV;       /**< CH5N�������ȡ����0:���������1:ȡ����� */
    uint8_t CH5P_Polarity_INV;       /**< CH5P�������ȡ����0:���������1:ȡ����� */

    uint8_t CH0P_SCTRLP;             /**< ��CH0_PS = 1ʱ�������CH0_Pͨ����ֵ*/
    uint8_t CH0N_SCTRLN;             /**< ��CH0_NS = 1ʱ�������CH0_Nͨ����ֵ*/
    uint8_t CH1P_SCTRLP;             /**< ��CH1_PS = 1ʱ�������CH1_Pͨ����ֵ*/
    uint8_t CH1N_SCTRLN;             /**< ��CH1_NS = 1ʱ�������CH1_Nͨ����ֵ*/
    uint8_t CH2P_SCTRLP;             /**< ��CH2_PS = 1ʱ�������CH2_Pͨ����ֵ*/
    uint8_t CH2N_SCTRLN;             /**< ��CH2_NS = 1ʱ�������CH2_Nͨ����ֵ*/
    uint8_t CH3P_SCTRLP;             /**< ��CH3_PS = 1ʱ�������CH3_Pͨ����ֵ*/
    uint8_t CH3N_SCTRLN;             /**< ��CH3_NS = 1ʱ�������CH3_Nͨ����ֵ*/
    uint8_t CH4P_SCTRLP;             /**< ��CH4_PS = 1ʱ�������CH4_Pͨ����ֵ*/
    uint8_t CH4N_SCTRLN;             /**< ��CH4_NS = 1ʱ�������CH4_Nͨ����ֵ*/
    uint8_t CH5P_SCTRLP;             /**< ��CH5_PS = 1ʱ�������CH5_Pͨ����ֵ*/
    uint8_t CH5N_SCTRLN;             /**< ��CH5_NS = 1ʱ�������CH5_Nͨ����ֵ*/

    uint8_t CH0_PS;                  /**< CH0_P�ź���Դ��1������CH0_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH0_NS;                  /**< CH0_N�ź���Դ��1������CH0_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH1_PS;                  /**< CH1_P�ź���Դ��1������CH1_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH1_NS;                  /**< CH1_N�ź���Դ��1������CH1_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH2_PS;                  /**< CH2_P�ź���Դ��1������CH2_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH2_NS;                  /**< CH2_N�ź���Դ��1������CH2_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH3_PS;                  /**< CH3_P�ź���Դ��1������CH3_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH3_NS;                  /**< CH3_N�ź���Դ��1������CH3_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH4_PS;                  /**< CH4_P�ź���Դ��1������CH4_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH4_NS;                  /**< CH4_N�ź���Դ��1������CH4_SCTRLN��0������MCPWMʵ�����м�����*/
    uint8_t CH5_PS;                  /**< CH5_P�ź���Դ��1������CH5_SCTRLP��0������MCPWMʵ�����м�����*/
    uint8_t CH5_NS;                  /**< CH5_N�ź���Դ��1������CH5_SCTRLN��0������MCPWMʵ�����м�����*/

    uint8_t Switch_CH0N_CH0P;        /**< ����CH0N, CH0P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH1N_CH1P;        /**< ����CH1N, CH1P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH2N_CH2P;        /**< ����CH2N, CH2P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH3N_CH3P;        /**< ����CH3N, CH3P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH4N_CH4P;        /**< ����CH2N, CH2P�ź����ʹ�ܿ��� */
    uint8_t Switch_CH5N_CH5P;        /**< ����CH3N, CH3P�ź����ʹ�ܿ��� */

    uint8_t TR0_UP_INTV;             /**< MCPWM_CNT0 ���¼�� T0/T1�¼����� */
    uint8_t TR0_T0_UpdateEN;         /**< MCPWM_CNT0 T0�¼�����ʹ�� */
    uint8_t TR0_T1_UpdateEN;         /**< MCPWM_CNT0 T1�¼�����ʹ�� */
    uint8_t TR0_AEC;                 /**< MCPWM �����¼��Ƿ��Զ����Fail0��Fail1�¼����ָ�CH0CH1CH2�����  1��ʹ�� */

    uint8_t TR1_UP_INTV;             /**< MCPWM_CNT1 ���¼�� T0/T1�¼����� */
    uint8_t TR1_T0_UpdateEN;         /**< MCPWM_CNT1 T0�¼�����ʹ�� */
    uint8_t TR1_T1_UpdateEN;         /**< MCPWM_CNT1 T1�¼�����ʹ�� */
    uint8_t TR1_AEC;                 /**< MCPWM �����¼��Ƿ��Զ����Fail2��Fail3�¼����ָ�CH3CH4CH5�����  1��ʹ�� */

    uint8_t FAIL0_INPUT_EN;          /**< FAIL0 ���빦��ʹ�� */
    uint8_t FAIL1_INPUT_EN;          /**< FAIL1 ���빦��ʹ�� */
    uint8_t FAIL_0CAP;               /**< FAIL01�¼�����ʱ ��MCPWM0_CNT0ֵ����MCPWM0_FCNT ʹ�� */
    uint8_t FAIL0_Signal_Sel;        /**< FAIL0 �ź�ѡ�񣬱Ƚ���0��GPIO */
    uint8_t FAIL1_Signal_Sel;        /**< FAIL1 �ź�ѡ�񣬱Ƚ���0��GPIO */
    uint8_t FAIL0_Polarity;          /**< FAIL0 �źż������ã�����Ч�����Ч */
    uint8_t FAIL1_Polarity;          /**< FAIL1 �źż������ã�����Ч�����Ч */
    uint8_t HALT_PRT0;               /**<  MCU����HALT״̬, MCPWM(CH0CH1CH2)�ź��Ƿ�������� 1���������0ǿ���������ֵ */

    uint8_t FAIL2_INPUT_EN;          /**< FAIL2 ���빦��ʹ�� */
    uint8_t FAIL3_INPUT_EN;          /**< FAIL3 ���빦��ʹ�� */
    uint8_t FAIL_1CAP;               /**< FAIL23�¼�����ʱ ��MCPWM0_CNT1ֵ����MCPWM0_FCNT */
    uint8_t FAIL2_Signal_Sel;        /**< FAIL2 �ź�ѡ�񣬱Ƚ���0��GPIO��CLUOUT0��CLUOUT1 */
    uint8_t FAIL3_Signal_Sel;        /**< FAIL3 �ź�ѡ�񣬱Ƚ���1��GPIO��CLUOUT2��CLUOUT3 */
    uint8_t FAIL2_Polarity;          /**< FAIL2 �źż������ã�����Ч�����Ч */
    uint8_t FAIL3_Polarity;          /**< FAIL3 �źż������ã�����Ч�����Ч */
    uint8_t HALT_PRT1;               /**< MCU����HALT״̬, MCPWM(CH3CH4CH5)�ź��Ƿ�������� 1���������0ǿ���������ֵ */

    uint8_t CH0P_default_output;     /**< CH0P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH0N_default_output;     /**< CH0N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH1P_default_output;     /**< CH1P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH1N_default_output;     /**< CH1N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH2P_default_output;     /**< CH2P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH2N_default_output;     /**< CH2N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH3P_default_output;     /**< CH3P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH3N_default_output;     /**< CH3N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH4P_default_output;     /**< CH4P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH4N_default_output;     /**< CH4N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH5P_default_output;     /**< CH5P MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */
    uint8_t CH5N_default_output;     /**< CH5N MOEΪ0ʱ����FAIL�¼�ʱ��Ĭ�ϵ�ƽ��� */

    uint8_t CH0N_FAIL_EN;     /**< CH0Nͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH0P_FAIL_EN;     /**< CH0Pͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH1N_FAIL_EN;     /**< CH1Nͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH1P_FAIL_EN;     /**< CH1Pͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH2N_FAIL_EN;     /**< CH2Nͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH2P_FAIL_EN;     /**< CH2Pͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH3N_FAIL_EN;     /**< CH3Nͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH3P_FAIL_EN;     /**< CH3Pͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH4N_FAIL_EN;     /**< CH4Nͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH4P_FAIL_EN;     /**< CH4Pͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH5N_FAIL_EN;     /**< CH5Nͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */
    uint8_t CH5P_FAIL_EN;     /**< CH5Pͨ��FAIL�¼�ʹ�� 1��MOEΪ0ʱ����FAIL�¼�ʱ�����Ĭ�ϵ�ƽ��0���������Ӱ�� */

    uint8_t T0_Update0_INT_EN;        /**< MCPWM0_CNT0 ������-MCPWMTH (T0�����¼�)�ж�ʹ�� */
    uint8_t T1_Update0_INT_EN;        /**< MCPWM0_CNT0 ������0        (T1�����¼�)�ж�ʹ�� */

    uint8_t TMR0_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR0 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TMR1_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR1 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TMR2_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR2 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TMR3_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR3 �����ļ����¼�ƥ���¼��ж�ʹ�� */

    uint8_t TH00_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH00 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH01_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH01 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH10_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH10 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH11_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH11 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH20_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH20 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH21_Match0_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH21 �����ļ����¼�ƥ���¼��ж�ʹ�� */

    uint8_t Update0_INT_EN;           /**< MCPWM0_CNT0 ������0�����ж�ʹ�� */

    uint8_t T0_Update1_INT_EN;        /**< MCPWM0_CNT0 ������-MCPWMTH (T0�����¼�)�ж�ʹ�� */
    uint8_t T1_Update1_INT_EN;        /**< MCPWM0_CNT0 ������0        (T1�����¼�)�ж�ʹ�� */

    uint8_t TMR0_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR0 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TMR1_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR1 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TMR2_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR2 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TMR3_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TMR3 �����ļ����¼�ƥ���¼��ж�ʹ�� */

    uint8_t TH00_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH00 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH01_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH01 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH10_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH10 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH11_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH11 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH20_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH20 �����ļ����¼�ƥ���¼��ж�ʹ�� */
    uint8_t TH21_Match1_INT_EN;       /**< MCPWM0_CNT0 ������ MCPWM0_TH21 �����ļ����¼�ƥ���¼��ж�ʹ�� */

    uint8_t Update1_INT_EN;           /**< MCPWM0_CNT0 ������0�����ж�ʹ�� */

    uint8_t FAIL0_INT_EN;            /* FAIL0�¼��ж�ʹ�� */
    uint8_t FAIL1_INT_EN;            /* FAIL1�¼��ж�ʹ�� */
    uint8_t FAIL2_INT_EN;            /* FAIL0�¼��ж�ʹ�� */
    uint8_t FAIL3_INT_EN;            /* FAIL1�¼��ж�ʹ�� */

    uint8_t IO_FLT_CLKDIV;          /**<  GPIO�����˲�ʱ������1-16 */
    uint8_t CMP_FLT_CLKDIV;         /**<  �Ƚ���CMP�����˲�ʱ������1-16 */

    uint8_t ZCS0_INT_EN;            /**<  ZCS0�¼��ж�ʹ�� ���ֵ�ԴӦ�� */
    uint8_t ZCS1_INT_EN;            /**<  ZCS1�¼��ж�ʹ�� ���ֵ�ԴӦ�� */

    uint8_t IO0_PPE;                 /**<  IO0����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t IO1_PPE;                 /**<  IO1����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t IO2_PPE;                 /**<  IO2����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t IO3_PPE;                 /**<  IO3����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t IO4_PPE;                 /**<  IO4����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t IO5_PPE;                 /**<  IO5����ģʽʹ�ܣ�1ʹ�ܣ�0�ر� */

    uint8_t TR1_T1_RE1;              /**< MCPWM_CNT1  ������0 (T1)       �¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TR1_T0_RE1;              /**< MCPWM_CNT1  ������-MCPWM_TH(T0)�¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TR0_T1_RE1;              /**< MCPWM_CNT0  ������0 (T1)       �¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TR0_T0_RE1;              /**< MCPWM_CNT0  ������-MCPWM_TH(T0)�¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR3_RE1;                /**< MCPWM_CNT01 ������MCPWM_TMR3   �¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR2_RE1;                /**< MCPWM_CNT01 ������MCPWM_TMR2   �¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR1_RE1;                /**< MCPWM_CNT01 ������MCPWM_TMR1   �¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR0_RE1;                /**< MCPWM_CNT01 ������MCPWM_TMR0   �¼� DMA����1 ʹ�ܣ�1ʹ�ܣ�0�ر� */

    uint8_t TR1_T1_RE0;              /**< MCPWM_CNT1  ������0 (T1)       �¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TR1_T0_RE0;              /**< MCPWM_CNT1  ������-MCPWM_TH(T0)�¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TR0_T1_RE0;              /**< MCPWM_CNT0  ������0 (T1)       �¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TR0_T0_RE0;              /**< MCPWM_CNT0  ������-MCPWM_TH(T0)�¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR3_RE0;                /**< MCPWM_CNT01 ������MCPWM_TMR3   �¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR2_RE0;                /**< MCPWM_CNT01 ������MCPWM_TMR2   �¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR1_RE0;                /**< MCPWM_CNT01 ������MCPWM_TMR1   �¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */
    uint8_t TMR0_RE0;                /**< MCPWM_CNT01 ������MCPWM_TMR0   �¼� DMA����0 ʹ�ܣ�1ʹ�ܣ�0�ر� */

    uint16_t STT_HYST;               /**<  ״̬�л���ʱ��12λ�޷����� */
    uint16_t ZCS_DELAY;              /**<  ZCS״̬ͣ��ʱ�䣬15λ�޷����� ���ֵ�ԴӦ�� */

    uint32_t AUEN;                   /**<  �Զ�����ʹ�� */
} MCPWM_InitTypeDef;

/* MCPWM��� ���뷽ʽ */
#define MCPWM0_CENTRAL_PWM_MODE     0  /**<  ���Ķ���PWMģʽ */
#define MCPWM0_EDGE_PWM_MODE        1  /**<  ���ض���PWMģʽ */

#define MCPWM0_HIGH_LEVEL           1  /**< �ߵ�ƽ */
#define MCPWM0_LOW_LEVEL            0  /**< �ߵ�ƽ */

#define MCPWM0_HIGH_LEVEL_ACTIVE    0  /**< �ߵ�ƽ��Ч */
#define MCPWM0_LOW_LEVEL_ACTIVE     1  /**< �͵�ƽ��Ч */

#define MCPWM0_FAIL_SEL_CMP         1  /**< Fail�¼���Դ�Ƚ��� */
#define MCPWM0_FAIL_SEL_IO          0  /**< Fail�¼���Դ�Ƚ��� */

/* MCPWM0_AUEN  MCPWM �Զ�����ʹ�ܼĴ��� */
#define MCPWM0_TH00_AUPDATE  BIT0  /**< MCPWM_TH00 �Զ�����ʹ�� */
#define MCPWM0_TH01_AUPDATE  BIT1  /**< MCPWM_TH01 �Զ�����ʹ�� */
#define MCPWM0_TH10_AUPDATE  BIT2  /**< MCPWM_TH10 �Զ�����ʹ�� */
#define MCPWM0_TH11_AUPDATE  BIT3  /**< MCPWM_TH11 �Զ�����ʹ�� */
#define MCPWM0_TH20_AUPDATE  BIT4  /**< MCPWM_TH20 �Զ�����ʹ�� */
#define MCPWM0_TH21_AUPDATE  BIT5  /**< MCPWM_TH21 �Զ�����ʹ�� */
#define MCPWM0_TMR0_AUPDATE  BIT8  /**< MCPWM_TMR0 �Զ�����ʹ�� */
#define MCPWM0_TMR1_AUPDATE  BIT9  /**< MCPWM_TMR1 �Զ�����ʹ�� */
#define MCPWM0_TH0_AUPDATE   BIT10 /**< MCPWM_TH0 �Զ�����ʹ��  */
#define MCPWM0_CNT0_AUPDAT   BIT11 /**< MCPWM_CNT0 �Զ�����ʹ�� */

#define MCPWM0_TH30_AUPDATE  BIT16  /**< MCPWM_TH00 �Զ�����ʹ�� */
#define MCPWM0_TH31_AUPDATE  BIT17  /**< MCPWM_TH01 �Զ�����ʹ�� */
#define MCPWM0_TH40_AUPDATE  BIT18  /**< MCPWM_TH10 �Զ�����ʹ�� */
#define MCPWM0_TH41_AUPDATE  BIT19  /**< MCPWM_TH11 �Զ�����ʹ�� */
#define MCPWM0_TH50_AUPDATE  BIT20  /**< MCPWM_TH20 �Զ�����ʹ�� */
#define MCPWM0_TH51_AUPDATE  BIT20  /**< MCPWM_TH21 �Զ�����ʹ�� */
#define MCPWM0_TMR2_AUPDATE  BIT24  /**< MCPWM_TMR0 �Զ�����ʹ�� */
#define MCPWM0_TMR3_AUPDATE  BIT25  /**< MCPWM_TMR1 �Զ�����ʹ�� */
#define MCPWM0_TH1_AUPDATE   BIT26  /**< MCPWM_TH0 �Զ�����ʹ��  */
#define MCPWM0_CNT1_AUPDAT   BIT27  /**< MCPWM_CNT0 �Զ�����ʹ�� */
#define MCPWM0_ALL_AUPDAT    0xffffffff  /**< ȫ���Զ�����ʹ�� */


#define MCPWM0CNT0_T0_IRQ_IF       BIT0  /**< ������0����ֵ�ص�-MCPWM_TH�жϱ�־λ */
#define MCPWM0CNT0_T1_IRQ_IF       BIT1  /**< ������0����ֵ�ص�0�жϱ�־λ */
#define MCPWM0CNT0_TH00_IRQ_IF     BIT2  /**< ������0����ֵ�ص�MCPWM_TH00�жϱ�־λ */
#define MCPWM0CNT0_TH01_IRQ_IF     BIT3  /**< ������0����ֵ�ص�MCPWM_TH01�жϱ�־λ */
#define MCPWM0CNT0_TH10_IRQ_IF     BIT4  /**< ������0����ֵ�ص�MCPWM_TH10�жϱ�־λ */
#define MCPWM0CNT0_TH11_IRQ_IF     BIT5  /**< ������0����ֵ�ص�MCPWM_TH11�жϱ�־λ */
#define MCPWM0CNT0_TH20_IRQ_IF     BIT6  /**< ������0����ֵ�ص�MCPWM_TH20�жϱ�־λ */
#define MCPWM0CNT0_TH21_IRQ_IF     BIT7  /**< ������0����ֵ�ص�MCPWM_TH21�жϱ�־λ */

#define MCPWM0CNT0_TMR0_IRQ_IF     BIT10  /**< ������0����ֵ�ص�MCPWM_TMR0�жϱ�־λ */
#define MCPWM0CNT0_TMR1_IRQ_IF     BIT11  /**< ������0����ֵ�ص�MCPWM_TMR1�жϱ�־λ */
#define MCPWM0CNT0_TMR2_IRQ_IF     BIT12  /**< ������0����ֵ�ص�MCPWM_TMR2�жϱ�־λ */
#define MCPWM0CNT0_TMR3_IRQ_IF     BIT13  /**< ������0����ֵ�ص�MCPWM_TMR3�жϱ�־λ */
#define MCPWM0CNT0_UP_IRQ_IF       BIT14    /**< ������0�����жϱ�־λ */

#define MCPWM0CNT1_T0_IRQ_IF       BIT0  /**< ������0����ֵ�ص�-MCPWM_TH�жϱ�־λ */
#define MCPWM0CNT1_T1_IRQ_IF       BIT1  /**< ������0����ֵ�ص�0�жϱ�־λ */
#define MCPWM0CNT1_TH30_IRQ_IF     BIT2  /**< ������0����ֵ�ص�MCPWM_TH00�жϱ�־λ */
#define MCPWM0CNT1_TH31_IRQ_IF     BIT3  /**< ������0����ֵ�ص�MCPWM_TH01�жϱ�־λ */
#define MCPWM0CNT1_TH40_IRQ_IF     BIT4  /**< ������0����ֵ�ص�MCPWM_TH10�жϱ�־λ */
#define MCPWM0CNT1_TH41_IRQ_IF     BIT5  /**< ������0����ֵ�ص�MCPWM_TH11�жϱ�־λ */
#define MCPWM0CNT1_TH50_IRQ_IF     BIT6  /**< ������0����ֵ�ص�MCPWM_TH20�жϱ�־λ */
#define MCPWM0CNT1_TH51_IRQ_IF     BIT7  /**< ������0����ֵ�ص�MCPWM_TH21�жϱ�־λ */

#define MCPWM0CNT1_TMR2_IRQ_IF     BIT12  /**< ������0����ֵ�ص�MCPWM_TMR2�жϱ�־λ */
#define MCPWM0CNT1_TMR3_IRQ_IF     BIT13  /**< ������0����ֵ�ص�MCPWM_TMR3�жϱ�־λ */
#define MCPWM0CNT1_UP_IRQ_IF       BIT14    /**< ������0�����жϱ�־λ */

#define MCPWM0_FAIL0_IRQ_IF         BIT19 /**< MCPWMCH0_P��CH0Nͬʱ��Ч�жϱ�־λ */
#define MCPWM0_FAIL1_IRQ_IF         BIT20 /**< MCPWMCH0_P��CH0Nͬʱ��Ч�жϱ�־λ */
#define MCPWM0_FAIL2_IRQ_IF         BIT21 /**< MCPWMCH0_P��CH0Nͬʱ��Ч�жϱ�־λ */
#define MCPWM0_FAIL3_IRQ_IF         BIT22 /**< MCPWMCH0_P��CH0Nͬʱ��Ч�жϱ�־λ */
#define MCPWM0_ZCS0_IRQ_IF          BIT23 /**< MCPWMCH0_P��CH0Nͬʱ��Ч�жϱ�־λ */
#define MCPWM0_ZCS1_IRQ_IF          BIT24 /**< MCPWMCH0_P��CH0Nͬʱ��Ч�жϱ�־λ */

/**< ����MCPWM��ʱ�����*/
typedef enum
{
    MCPWM0CNT0 = 0,     /**< MCPWM0������0 */
    MCPWM0CNT1 = 1,     /**< MCPWM0������1 */
} MCPWM0CNTx;


void MCPWM_Init(MCPWM_InitTypeDef* MCPWM_InitStruct);
void MCPWM_StructInit(MCPWM_InitTypeDef* MCPWM_InitStruct);
void PWMOutputs(FuncState t_state);
void MCPWM_ClearIRQFlag(MCPWM0CNTx CNTx, uint32_t INT_flag);
u16 MCPWM_GetIRQFlag(MCPWM0CNTx CNTx, uint32_t INT_flag);
#endif /*__CHANHOM_PWM_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
