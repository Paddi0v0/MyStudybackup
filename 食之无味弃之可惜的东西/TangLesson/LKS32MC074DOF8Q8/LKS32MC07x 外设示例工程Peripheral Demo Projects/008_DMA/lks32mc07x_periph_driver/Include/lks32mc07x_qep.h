/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_qep.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� QEP����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0    <td>HuangMG     <td>����
 * </table>
 */

#ifndef __lks32mc07x_QEP_H
#define __lks32mc07x_QEP_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"

/** 
 *@brief �������Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t CFG; /**<���������üĴ���*/
    __IO uint32_t TH;  /**<�������������޼Ĵ���*/
    __IO uint32_t CNT; /**<����������ֵ�Ĵ���*/
    __IO uint32_t IE;  /**<�������ж�ʹ��*/
    __IO uint32_t IF;  /**<�������жϱ�־λ*/
} TIM_QEPTypeDef;

 /**
 * QEP0�ṹ�����ַ����
 */
#define QEP0            ((TIM_QEPTypeDef *) QEP0_BASE)
 /**
 * QEP1�ṹ�����ַ����
 */

#define QEP1            ((TIM_QEPTypeDef *) QEP1_BASE)

typedef struct
{
      /**
     * @brief ������ģʽѡ�� \n
     * @see QEP_Mode_T1 \n
     * @see QEP_Mode_T1_T2 \n
     * @see QEP_Mode_CCWSIGN_UpDown \n
     * @see QEP_Mode_CCWCW_UpDown
     */  
    uint32_t QEP_Mode;   //������ģʽѡ��
	uint32_t QEP_FE_CNT_EN; //ʹ���½��ؼ��㣨������ʼ�ռ�������ENABLEʹ�� DISABLEʧ��
	uint32_t QEP_ZNC;    //Z�ź����㼫��ѡ�񣺵͵�ƽ/�½�������ʹ�ܣ�ENABLEʹ�� DISABLEʧ��
	uint32_t QEP_ZPC;    //Z�ź����㼫��ѡ�񣺸ߵ�ƽ/����������ʹ�� ��ENABLEʹ�� DISABLEʧ��
	uint32_t QEP_ZLC;    //Z�źŵ�ƽ����QEP������ʹ�ܣ�ENABLEʹ�� DISABLEʧ��
	uint32_t QEP_ZEC;    //Z�źű�������QEP������ʹ�ܣ�ENABLEʹ�� DISABLEʧ��
    uint16_t QEP_TH;     //QEP�������޼Ĵ���
    /**
     * @brief ��������Ӧʱ�ӷ�Ƶϵ�� \n
     * @see ECD_Clk_Div1  \n
     * @see ECD_Clk_Div2  \n
     * @see ECD_Clk_Div4  \n
     * @see ECD_Clk_Div8  \n
     * @see ECD_Clk_Div16 \n
     * @see ECD_Clk_Div32 \n
     * @see ECD_Clk_Div64 \n
     * @see ECD_Clk_Div128 
     */       
    uint32_t QEP_ClockDiv;//ʱ�ӷ�Ƶ
    uint32_t QEP_Filter;  //�ź������˲�
     /**
     * @brief �������ж�ʹ�� \n
     * @see QEP_IRQEna_None \n
     * @see QEP_IRQEna_UnderFlow \n
     * @see QEP_IRQEna_OverFlow 
     */        
    uint32_t QEP_IRQEna;  //�ж�ʹ��
} TIM_QEPInitTypeDef;

#define QEP_CLK_DIV1             0x00     /**<QEP������Ƶ��1��Ƶ*/
#define QEP_CLK_DIV2             0x01     /**<QEP������Ƶ��2��Ƶ*/
#define QEP_CLK_DIV4             0x02     /**<QEP������Ƶ��4��Ƶ*/
#define QEP_CLK_DIV8             0x03     /**<QEP������Ƶ��8��Ƶ*/
#define QEP_CLK_DIV16            0x04     /**<QEP������Ƶ��16��Ƶ*/
#define QEP_CLK_DIV32            0x05     /**<QEP������Ƶ��32��Ƶ*/
#define QEP_CLK_DIV64            0x06     /**<QEP������Ƶ��64��Ƶ*/
#define QEP_CLK_DIV128           0x07     /**<QEP������Ƶ��128��Ƶ*/


 /**
 * @brief �������ж�ʹ�����ö���
 */
#define QEP_IRQEna_None      0     /**<��ʹ��*/
 /**
 * @brief �������ж�ʹ�����ö���
 */
#define QEP_IRQEna_UnderFlow BIT0  /**<ʹ�������ж�*/
 /**
 * @brief �������ж�ʹ�����ö���
 */
#define QEP_IRQEna_OverFlow  BIT1  /**<ʹ�������ж�*/


 /**
 * @brief �������жϱ�־����
 */
#define QEP_IF_UNDERFLOW     BIT0  /**<Encoder�����ж�*/
 /**
 * @brief �������жϱ�־����
 */
#define QEP_IF_OVERFLOW      BIT1  /**<Encoder�����ж�*/


#define QEP_Mode_T1              ((uint32_t)0x00) /*counting on T1*/
#define QEP_Mode_T1_T2           ((uint32_t)0x01) /*counting on T1&T2*/
#define QEP_Mode_CCWSIGN_UpDown  ((uint32_t)0x02) /*CCW+SIGN ���ż������źż���ģʽ, ����Ҫ�����źŵ������أ���QEP_FE_CNT_EN��1*/
#define QEP_Mode_CCWCW_UpDown    ((uint32_t)0x03) /*CCW+CW   ���ż������źż���ģʽ, ����Ҫ�����źŵ������أ���QEP_FE_CNT_EN��1*/

/*QEP��ʼ��*/
void TIM_QEPInit(TIM_QEPTypeDef *QEPx, TIM_QEPInitTypeDef *TIM_QEPInitStruct);
void TIM_QEPStructInit(TIM_QEPInitTypeDef *TIM_QEPInitStruct);
void TIM_QEPCmd(TIM_QEPTypeDef *QEPx, FuncState state);
uint16_t TIM_QEP_GetCount(TIM_QEPTypeDef *QEPx);
void QEP_ClearIRQFlag(TIM_QEPTypeDef *QEPx,uint32_t tempFlag);
u16 QEP_GetIRQFlag(TIM_QEPTypeDef *QEPx, u32 QEP_IF_x);
u16 QEP_GetFlag(TIM_QEPTypeDef *QEPx, u32 QEP_IF_x);


#endif

