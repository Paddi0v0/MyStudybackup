/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_cmp.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� CMP������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author        <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>Olive Wang          <td>����
 * </table>
 */
#ifndef __lks32mc07x_CMP_H
#define __lks32mc07x_CMP_H
#include "lks32mc07x.h"
#include "basic.h"
/** 
 *@brief CMP��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
    u16 SELP;             /**< �Ƚ����ź�����ѡ��*/
	        /**
     * @brief �Ƚ��� �źŸ���ѡ��
     * @see CMP_SELN_IN 			            
     * @see CMP_SELN_REF 			           
     * @see CMP_SELN_DAC0 		           
     * @see CMP0_SELN_HALL0_MID
		 * @see CMP1_SELN_HALL1_MID
	   * @see CMP2_SELN_DAC1
     */    
    u16 SELN;   					/**< �Ƚ����źŸ���ѡ��*/
	   /**
     * @brief CMP �˲�ʱ��Ƶ��Freq(CMP_Filter) = (Freq(MCLK)/2^FIL_CLK_DIV2)/(FIL_CLK_DIV16+1) ��
     * ����MCLK Ϊϵͳ����ʱ�ӣ�ͨ��Ϊ 96MHz ȫ��ʱ�ӡ���Ҫע����ǣ����� CMP �˲�ʱ����Ҫʹ��CLK_EN��
     */
		u32 FIL_CLK_DIV16;    /**< �Ƚ���012�˲�ϵ������ֵ��Χ0-0xF */
    u32 FIL_CLK_DIV2;     /**< �Ƚ���012�˲�ϵ����0x0:1��Ƶ��0x1:2��Ƶ,0x2:4��Ƶ,0x3:8��Ƶ��0x4:16��Ƶ��0x5:32��Ƶ,0x6:64��Ƶ,0x7:128��Ƶ*/
	  /**
     * @brief �Ƚ����������ѡ��
     * @see CMP_HIGH_LEVEL 			            
     * @see CMP_LOW_LEVEL 			           
     */    
  	u16 POL;        			/**< �Ƚ�����Ч�������ѡ��*/
	 	 /**
     * @brief �Ƚ�����������ѡ��
     * @see W_PWM_HIGH_LEVEL 			            
     * @see W_PWM_LOW_LEVEL 			           
     */  
	  u16 W_PWM_POL;  			/**< �Ƚ������� PWM �źż���ѡ��,�Ƚ��� 0/1 */
		/**
     * @brief �Ƚ��������жϷ�ʽѡ��
     * @see IRQ_LEVEL_TRIG_MODE 			            
     * @see IRQ_EDGE_TRIG_MODE 			           
     */  
  	u16 IRQ_TRIG;   			/**< �Ƚ�������/��ƽ�����ж�ѡ��*/
    FuncState CLK_EN;     /**< �Ƚ���ʱ��ʹ�ܣ�DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
		FuncState EN;			    /**< �Ƚ���ʹ��\n DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    FuncState IE;         /**< �Ƚ����ж�ʹ�ܣ�DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    FuncState RE;         /**< �Ƚ���DMA ����ʹ�ܣ�DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    FuncState IN_EN;      /**< �Ƚ����ź�����ʹ�ܣ�DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    FuncState CHN3P_PWM_EN; /**< MCPWM ģ�� CHN3_P ͨ��ʹ�ܱȽ��� 0/1 ������DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    FuncState CHN2P_PWM_EN; /**< MCPWM ģ�� CHN2_P ͨ��ʹ�ܱȽ��� 0/1 ������DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    FuncState CHN1P_PWM_EN; /**< MCPWM ģ�� CHN1_P ͨ��ʹ�ܱȽ��� 0/1 ������DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
    FuncState CHN0P_PWM_EN; /**< MCPWM ģ�� CHN0_P ͨ��ʹ�ܱȽ��� 0/1 ������DISABLE:ʧ�ܣ�ENABLE��ʹ��*/
} CMP_SingleTypeDef;
typedef struct
{
		 /**
     * @brief �Ƚ����ز�ѡ��
     * @see CMP_HYS_20mV 			            
     * @see CMP_HYS_0mV 			           
     */  
		u8 HYS;                 /**< �Ƚ����ز�ѡ��*/
		FuncState FT;           /**< �Ƚ������ٱȽ�ʹ��  DISABLE/ENABLE*/
		u32 CLK_COM_DIV;        /**< �Ƚ������÷�Ƶϵ����ȡֵ0-4*/
    CMP_SingleTypeDef CMP0; /**< CMP0����*/
    CMP_SingleTypeDef CMP1; /**< CMP1����*/
    CMP_SingleTypeDef CMP2; /**< CMP2����*/

} CMP_InitTypeDef;
/** 
 *@brief CMP�Ĵ����ṹ����
 */
typedef struct
{
    __IO u32 IE;      /**< �Ƚ����ж�ʹ�ܼĴ���*/
    __IO u32 IF;      /**< �Ƚ����жϱ�־�Ĵ���*/
    __IO u32 TCLK;    /**< �Ƚ�����Ƶʱ�ӿ��ƼĴ���*/
    __IO u32 CFG;     /**< �Ƚ������ƼĴ���*/
    __IO u32 BLCWIN;  /**< �Ƚ����������ƼĴ���*/
    __IO u32 DATA;    /**< �Ƚ��������ֵ�Ĵ���*/
}CMP_TypeDef;
/**
 * CMP�ṹ�����ַ����
 */
#define CMP  ((CMP_TypeDef*)CMP_BASE)

//�Ƚ����ź�N��ѡ��
#define CMP_SELN_IN 			   		0      		 /**< N������ */
#define CMP_SELN_REF 			   		1      		 /**< N���ڲ��ο�REF���� */
#define CMP_SELN_DAC0 		   		2      		 /**< N���ڲ�DAC0���� */
#define CMP0_SELN_HALL0_MID  		3          /**< CMP0-N�˷����ƹ����е����� */
#define CMP1_SELN_HALL1_MID  		3          /**< CMP1-N�˷����ƹ����е����� */
#define CMP2_SELN_DAC1 		    	3      		 /**< CMP2-N���ڲ�DAC1���� */

//�Ƚ���0 P��ѡ��
#define CMP0_SELP_IP0            0        /**< P��IP0���� */
#define CMP0_SELP_OPA0_IP        1        /**< P��OPA0 IP����*/
#define CMP0_SELP_OPA0_OUT       2        /**< P��OPA0 OUT */
#define CMP0_SELP_OPA1_OUT       3        /**< P��OPA1 OUT */
#define CMP0_SELP_IP1            4        /**< P��IP1���� */
#define CMP0_SELP_IP2            5        /**< P��IP2���� */
#define CMP0_SELP_IP3            6        /**< P��IP3���� */
#define CMP0_SELP_IP4            7        /**< P��IP4����*/ 

//�Ƚ���1 P��ѡ��
#define CMP1_SELP_IP0            0        /**< P��IP0���� */
#define CMP1_SELP_OPA3_IP        1        /**< P��OPA3 IP����*/
#define CMP1_SELP_OPA2_OUT       2        /**< P��OPA2 OUT */
#define CMP1_SELP_OPA3_OUT       3        /**< P��OPA3 OUT */
#define CMP1_SELP_IP1            4        /**< P��IP1���� */
#define CMP1_SELP_IP2            5        /**< P��IP2���� */
#define CMP1_SELP_IP3            6        /**< P��IP3���� */
#define CMP1_SELP_DAC0           7        /**< P��DAC0����*/ 

//�Ƚ���2 P��ѡ��
#define CMP2_SELP_IP0            0        /**< P��IP0���� */
#define CMP2_SELP_IP1            1        /**< P��IP1���� */
#define CMP2_SELP_OPA2_OUT       2        /**< P��OPA2 OUT */
#define CMP2_SELP_OPA3_OUT       3        /**< P��OPA3 OUT */

//��Ч����ѡ��
#define  CMP_HIGH_LEVEL  			0						/**�ߵ�ƽ�����Ч */
#define  CMP_LOW_LEVEL  			1 					/**�͵�ƽ�����Ч */

//���� PWM �źż���ѡ��
#define  IRQ_LEVEL_TRIG_MODE   0		/**< ��ƽ�����ж�ģʽ */
#define  IRQ_EDGE_TRIG_MODE    1  	/**< ���ش����ж�ģʽ */

//���� PWM �źż���ѡ��
#define  W_PWM_HIGH_LEVEL    1		/**< �Ƚ��������ߵ�ƽ��Ч*/
#define  W_PWM_LOW_LEVEL     0		/**< �Ƚ��������͵�ƽ��Ч*/

//�Ƚ�ͨ��ѡ��
#define CMP_CHN_0 			0					/**< ͨ��0*/
#define CMP_CHN_1 			1					/**< ͨ��1*/
#define CMP_CHN_2 			2					/**< ͨ��2*/

/**< �Ƚ����ز�ѡ��*/
#define CMP_HYS_20mV            0        /**< 20mV*/
#define CMP_HYS_0mV             1        /**< 0mV*/

void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct);/* CMP�ṹ���ʼ��*/
void CMP_Init(CMP_InitTypeDef* CMP_InitStruct);/* CMP��ʼ��*/
void CMP_Cmd(u8 CMP_CHN_x,FuncState state);/*CMPʹ�� */
void CMP_ClearIRQFlag(u8 CMP_CHN_x);    /* ��CMP�ж� */
u8 CMP_GetIRQFlag(u8 CMP_CHN_x);        /* ��CMP�ж� */
uint8_t CMP_GetFilterValue(void);/*��ȡ�˲���ıȽ���ֵ*/
uint8_t CMP_GetValue(void);/*��ȡδ�˲��ıȽ���ֵ*/

#endif /*__lks32mc07x_OPA_H */
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
