/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_cmp.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� CMP������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author        <th>Description
 * <tr><td>2018��07��25�� <td>1.0  <td>William Zhang       <td>����
 * </table>
 *@par �޸���־:���ӿ������á����ӱȽ����ж���غ���
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author        <th>Description
 * <tr><td>2022��04��26�� <td>2.0     <td>YangZJ          <td>����
 * </table>
 */
#ifndef __LKS32MC08x_CMP_H
#define __LKS32MC08x_CMP_H
/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
#include "string.h"
 
/**< �Ƚ����ز�ѡ��*/
#define CMP_HYS_20mV            0        /**< 20mV*/
#define CMP_HYS_0mV             1        /**< 0mV*/

//�Ƚ���N��
#define CMP_SELN_IN             0        /**< �Ƚ���N�ˣ�N������ */
#define CMP_SELN_REF            1        /**< �Ƚ���N�ˣ�N���ڲ��ο�REF���� */
#define CMP_SELN_DAC            2        /**< �Ƚ���N�ˣ�N���ڲ�DAC���� */
#define CMP_SELN_HALL_MID       3        /**< �Ƚ���N�ˣ�N�˷����ƹ����е����� */
          
//�Ƚ���0 P��
#define CMP0_SELP_IP0            0        /**< P��IP0���� */
#define CMP0_SELP_OPA0_IP        1        /**< P��OPA0 IP���� */
#define CMP0_SELP_OPA0_OUT       2        /**< P��OPA0 OUT */
#define CMP0_SELP_OPA1_OUT       3        /**< P��OPA1 OUT */
#define CMP0_SELP_IP1            4        /**< P��IP1���� */
#define CMP0_SELP_IP2            5        /**< P��IP2���� */
#define CMP0_SELP_IP3            6        /**< P��IP3���� */
#define CMP0_SELP_IP4            7        /**< P��IP4���� */

//�Ƚ���1 P��
#define CMP1_SELP_IP0            0        /**< P��IP0���� */
#define CMP1_SELP_OPA3_IP        1        /**< P��OPA3 IP���� */
#define CMP1_SELP_OPA2_OUT       2        /**< P��OPA2 OUT */
#define CMP1_SELP_OPA3_OUT       3        /**< P��OPA3 OUT */
#define CMP1_SELP_IP1            4        /**< P��IP1���� */
#define CMP1_SELP_IP2            5        /**< P��IP2���� */
#define CMP1_SELP_IP3            6        /**< P��IP3���� */
#define CMP1_SELP_AVSS           7        /**< оƬ�� */

//Ϊ��֤������ԣ������޸ĵĺ궨��
#define SELN_CMP_IN             0        /**< N�����루Ϊ��֤������ԣ������޸ĵĺ궨�壩*/
#define SELN_REF                1        /**< N���ڲ��ο�REF���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELN_DAC                2        /**< N���ڲ�DAC���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELN_HALL_MID           3        /**< N�˷����ƹ����е����루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */

#define SELP_CMP_OPA0IP         1        /**< P��OPA0_IP���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_OPA0OUT        2        /**< P��OPA0_OUT���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_OPA1OUT        3        /**< P��OPA1_OUT���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_OPA3IP         1        /**< P��OPA3_IP���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_OPA2OUT        2        /**< P��OPA2_OUT���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_OPA3OUT        3        /**< P��OPA3_OUT���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_IP0            0        /**< P��IP0���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_IP1            4        /**< P��IP1���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_IP2            5        /**< P��IP2���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_IP3            6        /**< P��IP3���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */
#define SELP_CMP_IP4            7        /**< P��IP4���루Ϊ��֤������ԣ������޸ĵĺ궨�壩 */

/** 
 *@brief CMP�Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t IE;       /**< �Ƚ����ж�ʹ�ܼĴ���*/
    __IO uint32_t IF;       /**< �Ƚ����жϱ�־�Ĵ���*/
    __IO uint32_t TCLK;     /**< �Ƚ�����Ƶʱ�ӿ��ƼĴ���*/
    __IO uint32_t CFG;      /**< �Ƚ������ƼĴ���*/
    __IO uint32_t BLCWIN;   /**< �Ƚ����������ƼĴ���*/
    __IO uint32_t DATA;     /**< �Ƚ��������ֵ�Ĵ���*/
} CMP_TypeDef;
/**
 * CMP�ṹ�����ַ����
 */
#ifndef CMP
#define CMP             ((CMP_TypeDef *) CMP_BASE)
#endif
/**
 * �Ƚ���0
 */
#define CMP0            0
/**
 * �Ƚ���1
 */
#define CMP1            1

/** 
 *@brief CMP��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
    /**
     * @brief CMP �˲�ʱ��Ƶ��Freq(CMP_Filter) = (Freq(MCLK)/2^FIL_CLK10_DIV2)/(FIL_CLK10_DIV16+1) ��
     * ����MCLK Ϊϵͳ����ʱ�ӣ�ͨ��Ϊ 96MHz ȫ��ʱ�ӡ���Ҫע����ǣ����� CMP �˲�ʱ����Ҫʹ��CLK10_EN��
     */
    u32         FIL_CLK10_DIV16;    /**< �Ƚ��� 1/0 �˲�ʱ�ӷ�Ƶ\n ��ֵ��Χ0-0xF*/
    FuncState   CLK10_EN;           /**< �Ƚ��� 1/0 �˲�ʱ��ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    /**
     * @brief CMP �˲�ʱ��Ƶ��Freq(CMP_Filter) = (Freq(MCLK)/2^FIL_CLK10_DIV2)/(FIL_CLK10_DIV16+1) ��
     * ����MCLK Ϊϵͳ����ʱ�ӣ�ͨ��Ϊ 96MHz ȫ��ʱ�ӡ���Ҫע����ǣ����� CMP �˲�ʱ����Ҫʹ��CLK10_EN��
     */
    u32         FIL_CLK10_DIV2;     /**< �Ƚ��� 1/0 �˲�ʱ�ӷ�Ƶ\n 0x0:1��Ƶ��0x1:2��Ƶ,0x2:4��Ƶ,0x3:8��Ƶ*/
    FuncState   CMP_FT;             /**< �Ƚ������ٱȽ�ʹ��\n  DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    u32         CMP_HYS;            /**< �Ƚ����ز�ѡ��\n @see CMP_HYS_0mV \n  @see CMP_HYS_20mV  */
    																																				
    FuncState   CMP0_EN;            /**< �Ƚ��� 0 ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    /**
     * @brief �Ƚ��� 0 �ź�����ѡ��
     * @see CMP_SELP_IP0           
     * @see CMP_SELP_OPA_IP        
     * @see CMP_SELP_OPA_OUT       
     * @see CMP_SELP_IP1           
     * @see CMP_SELP_IP2           
     * @see CMP_SELP_IP3           
     * @see CMP_SELP_IP4  
     */
    u32         CMP0_SELP;  
        /**
     * @brief �Ƚ��� 0 �źŸ���ѡ��
     * @see CMP_SELN_IN                   
     * @see CMP_SELN_REF                 
     * @see CMP_SELN_DAC                 
     * @see CMP_SELN_HALL_MID 
     */       
    u32         CMP0_SELN;       
    FuncState   CMP0_IE;            /**< �Ƚ��� 0 �ж�ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    u32         CMP0_W_PWM_POL;     /**< �Ƚ��� 0 ���� PWM �źż���ѡ��\n  0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч*/
    FuncState   CMP0_IRQ_TRIG;      /**< �Ƚ��� 0 ���ش���ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    FuncState   CMP0_IN_EN;         /**< �Ƚ��� 0 �ź�����ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    u32         CMP0_POL;           /**< �Ƚ��� 0 ����ѡ��\n 0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч*/
    FuncState   CMP0_CHN3P_WIN_EN;  /**< MCPWM ģ�� CHN3_P ͨ��ʹ�ܱȽ���\n 0 ���� DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    FuncState   CMP0_CHN2P_WIN_EN;  /**< MCPWM ģ�� CHN2_P ͨ��ʹ�ܱȽ���\n 0 ���� DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    FuncState   CMP0_CHN1P_WIN_EN;  /**< MCPWM ģ�� CHN1_P ͨ��ʹ�ܱȽ���\n 0 ���� DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    FuncState   CMP0_CHN0P_WIN_EN;  /**< MCPWM ģ�� CHN0_P ͨ��ʹ�ܱȽ���\n 0 ���� DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
																																					
    FuncState   CMP1_EN;            /**< �Ƚ��� 1 ʹ��\n ʹ�� DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
     /**
     * @brief �Ƚ��� 0 �ź�����ѡ��
     * @see CMP_SELP_IP0           
     * @see CMP_SELP_OPA_IP        
     * @see CMP_SELP_OPA_OUT       
     * @see CMP_SELP_IP1           
     * @see CMP_SELP_IP2           
     * @see CMP_SELP_IP3           
     * @see CMP_SELP_IP4  
     */   
    u32         CMP1_SELP;  
     /**
     * @brief �Ƚ��� 0 �źŸ���ѡ��
     * @see CMP_SELN_IN                   
     * @see CMP_SELN_REF                 
     * @see CMP_SELN_DAC                 
     * @see CMP_SELN_HALL_MID 
     */          
    u32         CMP1_SELN;         
    FuncState   CMP1_IE;            /**< �Ƚ��� 1 �ж�ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    u32         CMP1_W_PWM_POL;     /**< �Ƚ��� 1 ���� PWM �źż���ѡ��\n 0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч*/
    FuncState   CMP1_IRQ_TRIG;      /**< �Ƚ��� 1 ���ش���ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    FuncState   CMP1_IN_EN;         /**< �Ƚ��� 1 �ź�����ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ�� */
    u32         CMP1_POL;           /**< �Ƚ��� 1 ����ѡ�� 0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч*/
    FuncState   CMP1_CHN3P_WIN_EN;  /**< MCPWM ģ�� CHN3_P ͨ��ʹ�ܱȽ��� 1 ����\n 0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч */
    FuncState   CMP1_CHN2P_WIN_EN;  /**< MCPWM ģ�� CHN2_P ͨ��ʹ�ܱȽ��� 1 ����\n 0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч */
    FuncState   CMP1_CHN1P_WIN_EN;  /**< MCPWM ģ�� CHN1_P ͨ��ʹ�ܱȽ��� 1 ����\n 0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч */
    FuncState   CMP1_CHN0P_WIN_EN;  /**< MCPWM ģ�� CHN0_P ͨ��ʹ�ܱȽ��� 1 ����\n 0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч */
}CMP_InitTypeDef;

void CMP_Init(CMP_InitTypeDef *);           // �Ƚ�����ʼ��
void CMP_StructInit(CMP_InitTypeDef *);     // �Ƚ������ýṹ���ʼ��
void CMP_ClearIRQFlag(u8 CMPx);             // ��CMP�ж�
u8   CMP_GetIRQFlag(u8 CMPx);               // ��CMP�ж�

#endif /*__LKS32MC05x_CMP_H */
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/

