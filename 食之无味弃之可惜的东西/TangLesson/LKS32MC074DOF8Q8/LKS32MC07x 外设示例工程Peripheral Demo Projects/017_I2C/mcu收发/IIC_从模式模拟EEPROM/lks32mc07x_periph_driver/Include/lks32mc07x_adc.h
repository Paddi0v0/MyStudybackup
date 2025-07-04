/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC07x_adc.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ADC������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>zhujie    <td>����
 * </table>
 */
#ifndef _CONNIE_ADC_H_
#define _CONNIE_ADC_H_


/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"
#include "basic.h"

typedef struct
{
    __IO uint32_t DAT0      ;   /**< ƫ�Ƶ�ַ: 0x00 ADCx �� 0 �γ����������*/
    __IO uint32_t DAT1      ;   /**< ƫ�Ƶ�ַ: 0x04 ADCx �� 1 �γ����������*/
    __IO uint32_t DAT2      ;   /**< ƫ�Ƶ�ַ: 0x08 ADCx �� 2 �γ����������*/
    __IO uint32_t DAT3      ;   /**< ƫ�Ƶ�ַ: 0x0C ADCx �� 3 �γ����������*/
																/**<                                      */
    __IO uint32_t DAT4      ;   /**< ƫ�Ƶ�ַ: 0x10 ADCx �� 4 �γ����������*/
    __IO uint32_t DAT5      ;   /**< ƫ�Ƶ�ַ: 0x14 ADCx �� 5 �γ����������*/
    __IO uint32_t DAT6      ;   /**< ƫ�Ƶ�ַ: 0x18 ADCx �� 6 �γ����������*/
    __IO uint32_t DAT7      ;   /**< ƫ�Ƶ�ַ: 0x1C ADCx �� 7 �γ����������*/
																/**<                                      */
    __IO uint32_t DAT8      ;   /**< ƫ�Ƶ�ַ: 0x20 ADCx �� 8 �γ����������*/
    __IO uint32_t DAT9      ;   /**< ƫ�Ƶ�ַ: 0x24 ADCx �� 9 �γ����������*/
    __IO uint32_t DAT10     ;   /**< ƫ�Ƶ�ַ: 0x28 ADCx �� 10 �γ���������� */
    __IO uint32_t DAT11     ;   /**< ƫ�Ƶ�ַ: 0x2C ADCx �� 11 �γ���������� */
										
    __IO uint32_t DAT12     ;   /**< ƫ�Ƶ�ַ: 0x30 ADCx �� 12 �γ����������*/ 
    __IO uint32_t DAT13     ;   /**< ƫ�Ƶ�ַ: 0x34 ADCx �� 13 �γ���������� */
    __IO uint32_t IDAT0     ;   /**< ƫ�Ƶ�ַ: 0x38 ADCx �� 0 �ο��в�������*/  
    __IO uint32_t IDAT1     ;   /**< ƫ�Ƶ�ַ: 0x3C ADCx �� 1 �ο��в�������*/
															
    __IO uint32_t ICHN      ;   /**< ƫ�Ƶ�ַ: 0x40 ADCx ���в���ͨ������*/
    __IO uint32_t null      ;   /**< ƫ�Ƶ�ַ: 0x44 ��ַ���룬������*/
    __IO uint32_t null1     ;   /**< ƫ�Ƶ�ַ: 0x48 ��ַ���룬������*/
    __IO uint32_t null2     ;   /**< ƫ�Ƶ�ַ: 0x4C ��ַ���룬������*/
													
    __IO uint32_t CHN0      ;   /**< ƫ�Ƶ�ַ: 0x50 ADCx �� 0~ 3 �γ�����������ź�ѡ�� Signal 0 Signal 0 Signal 0 Signal 0 Signal 0 Signal 0*/
    __IO uint32_t CHN1      ;   /**< ƫ�Ƶ�ַ: 0x54 ADCx �� 4~ 7 �γ�����������ź�ѡ��*/
    __IO uint32_t CHN2      ;   /**< ƫ�Ƶ�ַ: 0x58 ADCx �� 8~11 �γ�����������ź�ѡ��*/
    __IO uint32_t CHN3      ;   /**< ƫ�Ƶ�ַ: 0x5C ADCx �� 12~15 �γ�����������ź�ѡ��*/
																
    __IO uint32_t CHNT      ;   /**< ƫ�Ƶ�ַ: 0x60 ADCx ���ִ���ģʽ�²�������*/
    __IO uint32_t GAIN      ;   /**< ƫ�Ƶ�ַ: 0x64 ADCx ����ѡ��*/
    __IO uint32_t null3     ;   /**< ƫ�Ƶ�ַ: 0x68 ��ַ���룬������*/
    __IO uint32_t null4     ;   /**< ƫ�Ƶ�ַ: 0x6C ��ַ���룬������*/
																
    __IO uint32_t null5     ;   /**< ƫ�Ƶ�ַ: 0x70 ��ַ���룬������*/
    __IO uint32_t CFG       ;   /**< ƫ�Ƶ�ַ: 0x74 ADCx ģʽ����*/
    __IO uint32_t TRIG      ;   /**< ƫ�Ƶ�ַ: 0x78 ADCx ������������*/
    __IO uint32_t SWT       ;   /**< ƫ�Ƶ�ַ: 0x7C ADCx �������*/
															
    __IO uint32_t DC0       ;   /**< ƫ�Ƶ�ַ: 0x80 ADCx ����Ϊ 0 ʱ DC offset*/
    __IO uint32_t AMC0      ;   /**< ƫ�Ƶ�ַ: 0x84 ADCx ����Ϊ 0 ʱ����У��ϵ��*/
    __IO uint32_t DC1       ;   /**< ƫ�Ƶ�ַ: 0x88 ADCx ����Ϊ 1 ʱ DC offset*/
    __IO uint32_t AMC1      ;   /**< ƫ�Ƶ�ַ: 0x8C ADCx ����Ϊ 1 ʱ����У��ϵ��*/
													
    __IO uint32_t IE        ;   /**< ƫ�Ƶ�ַ: 0x90 ADCx �ж�ʹ��*/
    __IO uint32_t IF        ;   /**< ƫ�Ƶ�ַ: 0x94 ADCx �жϱ�־*/
    __IO uint32_t null6     ;   /**< ƫ�Ƶ�ַ: 0x98 ��ַ���룬������*/
    __IO uint32_t null7     ;   /**< ƫ�Ƶ�ַ: 0x9C ��ַ���룬������*/
															
    __IO uint32_t null8     ;   /**< ƫ�Ƶ�ַ: 0xA0 ��ַ���룬������*/
    __IO uint32_t null9     ;   /**< ƫ�Ƶ�ַ: 0xA4 ��ַ���룬������*/
    __IO uint32_t null10    ;   /**< ƫ�Ƶ�ַ: 0xA8 ��ַ���룬������*/
    __IO uint32_t null11    ;   /**< ƫ�Ƶ�ַ: 0xAC ��ַ���룬������*/
											
    __IO uint32_t null12    ;   /**< ƫ�Ƶ�ַ: 0xB0 ��ַ���룬������*/
    __IO uint32_t null13    ;   /**< ƫ�Ƶ�ַ: 0xB4 ��ַ���룬������*/
    __IO uint32_t null14    ;   /**< ƫ�Ƶ�ַ: 0xB8 ��ַ���룬������*/
    __IO uint32_t null15    ;   /**< ƫ�Ƶ�ַ: 0xBC ��ַ���룬������*/
															
    __IO uint32_t null16    ;   /**< ƫ�Ƶ�ַ: 0xCC ��ַ���룬������*/
    __IO uint32_t LTH       ;   /**< ƫ�Ƶ�ַ: 0xC4 ADCx ���ݵ���ֵ*/
    __IO uint32_t HTH       ;   /**< ƫ�Ƶ�ַ: 0xC8 ADCx ���ݸ���ֵ*/
    __IO uint32_t GEN       ;   /**< ƫ�Ƶ�ַ: 0xCC ADCx ��ֵ���ʹ��*/
} ADC_TypeDef;

#define ADC0         ((ADC_TypeDef *) ADC0_BASE)
#define ADC1         ((ADC_TypeDef *) ADC1_BASE)

typedef struct
{
    u16 IE          ;           /**< �ж�ʹ��*/
    u16 RE          ;           /**< DMA����ʹ��*/
    u16 NSMP        ;           /**< ���β���ʹ��*/
    u16 DATA_ALIGN  ;           /**< DAT�Ҷ���ʹ��*/
    u16 CSMP        ;           /**< ��������ʹ��*/
    u16 TCNT        ;           /**< ����һ�β���������¼���*/
    u16 TROVS       ;           /**< �ֶ�����������ʹ�ܣ�������һ�β�����Ҫ��δ���*/
    u16 OVSR        ;           /**< ��������*/
    u16 TRIG        ;           /**< �����ź�*/
    u16 S1          ;           /**< ��һ�γ����������*/
    u16 S2          ;           /**< �ڶ��γ����������*/
    u16 IS1         ;           /**< ���в�������*/
    u16 GAIN        ;           /**< �����棨7.2V���̣�ʹ��*/
    u16 LTH         ;           /**< ADC ģ�⿴�Ź� 0 ����ֵ*/
    u16 HTH         ;           /**< ADC ģ�⿴�Ź� 0 ����ֵ*/
    u16 GEN         ;           /**< ADC ģ�⿴�Ź� 0 ��Ӧʹ��λ*/
} ADC_InitTypeDef;


#define ADC_ISF_RE      BIT15   /**< ���в������ DMA ����ʹ��*/
#define ADC_HERR_RE     BIT14   /**< Ӳ�����������ڷǿ���״̬ DMA ����ʹ��*/
#define ADC_SERR_RE     BIT13   /**< ������������ڷǿ���״̬ DMA ����ʹ��*/
#define ADC_AWD0_RE     BIT10   /**< ��ֵ��� 0 ���� DMA ����ʹ��*/
#define ADC_SF2_RE      BIT9    /**< �ڶ��β������ DMA ����ʹ��*/
#define ADC_SF1_RE      BIT8    /**< ��һ�β������ DMA ����ʹ��*/
        
#define ADC_ISF_IE      BIT7    /**< ���в�������ж�ʹ��*/
#define ADC_HERR_IE     BIT6    /**< Ӳ�����������ڷǿ���״̬�ж�ʹ��*/
#define ADC_SERR_IE     BIT5    /**< ������������ڷǿ���״̬�ж�ʹ��*/
#define ADC_AWD0_IE     BIT2    /**< ��ֵ��� 0 �����ж�ʹ��*/
#define ADC_SF2_IE      BIT1    /**< �ڶ��γ����������ж�ʹ��*/
#define ADC_SF1_IE      BIT0    /**< ��һ�γ����������ж�ʹ��*/


#define ADC_ISF_IF      BIT7    /**< ���в�������жϱ�־*/
#define ADC_HERR_IF     BIT6    /**< Ӳ�����������ڷǿ���״̬�жϱ�־*/
#define ADC_SERR_IF     BIT5    /**< ������������ڷǿ���״̬�жϱ�־*/
#define ADC_AWD0_IF     BIT2    /**< ��ֵ��� 0 �����жϱ�־*/
#define ADC_SF2_IF      BIT1    /**< �ڶ��γ����������жϱ�־*/
#define ADC_SF1_IF      BIT0    /**< ��һ�γ����������жϱ�־*/
#define ADC_ALL_IF      (0x7f)  /**< ADCȫ���жϱ�־λ���������ڳ�ʼ�� */

#define ADC_TRIG_CL_IT3        BIT15  /**< CL_OUTPUT[3]���� ADC ���в���ʹ�ܣ�����Ч*/
#define ADC_TRIG_CL_IT2        BIT14  /**< CL_OUTPUT[2]���� ADC ���в���ʹ�ܣ�����Ч*/
#define ADC_TRIG_CL_IT1        BIT13  /**< CL_OUTPUT[1]���� ADC ���в���ʹ�ܣ�����Ч*/
#define ADC_TRIG_CL_IT0        BIT12  /**< CL_OUTPUT[0]���� ADC ���в���ʹ�ܣ�����Ч*/
#define ADC_TRIG_CL_NT3        BIT11  /**< CL_OUTPUT[3]���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_CL_NT2        BIT10  /**< CL_OUTPUT[2]���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_CL_NT1        BIT9   /**< CL_OUTPUT[1]���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_CL_NT0        BIT8   /**< CL_OUTPUT[0]���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_UTIMER2_CMP1  BIT7   /**< UTIMER2 �Ƚ��¼� 1 ���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_UTIMER2_CMP0  BIT6   /**< UTIMER2 �Ƚ��¼� 0 ���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_UTIMER0_CMP1  BIT5   /**< UTIMER0 �Ƚ��¼� 1 ���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_UTIMER0_CMP0  BIT4   /**< UTIMER0 �Ƚ��¼� 0 ���� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_MCPWM0_T3     BIT3   /**< MCPWM0 T3 �¼����� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_MCPWM0_T2     BIT2   /**< MCPWM0 T2 �¼����� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_MCPWM0_T1     BIT1   /**< MCPWM0 T1 �¼����� ADC �������ʹ�ܣ�����Ч*/
#define ADC_TRIG_MCPWM0_T0     BIT0   /**< MCPWM0 T0 �¼����� ADC �������ʹ�ܣ�����Ч*/




#define ADC_CHANNEL_0   0x0     /**< OPA0 ���  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_1   0x1     /**< OPA1 ���  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_2   0x2     /**< OPA2 ���  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_3   0x3     /**< OPA3 ���  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_4   0x4     /**< ADC01_CH4  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_5   0x5     /**< ADC01_CH5  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_6   0x6     /**< ADC01_CH6  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_7   0x7     /**< ADC01_CH7  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_8   0x8     /**< ADC01_CH8  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_9   0x9     /**< ADC01_CH9  ADC0��ADC1ͨ��0-9����*/
#define ADC_CHANNEL_10  0xA     /**< ADC0_CH10  ADC1_CH10  ADC0��ADC1ͨ����ӦIO�ڲ�ͬ*/
#define ADC_CHANNEL_11  0xB     /**< ADC0_CH11  ADC1_CH11  ADC0��ADC1ͨ����ӦIO�ڲ�ͬ*/
#define ADC_CHANNEL_12  0xC     /**< ADC0_CH12  ADC1_CH12  ADC0��ADC1ͨ����ӦIO�ڲ�ͬ*/
#define ADC_CHANNEL_13  0xD     /**< ADC0_CH13  ADC1_CH13  ADC0��ADC1ͨ����ӦIO�ڲ�ͬ*/
#define ADC_CHANNEL_14  0xE     /**< ADC0ͨ��14��ӦDAC0��� ADC1ͨ��14��Ӧ�¶ȴ�����  */
#define ADC_CHANNEL_15  0xF     /**< ADC0ͨ��15��ӦDAC1��� ADC1ͨ��15��Ӧ�ڲ��� AVSS */

/** ADC����ͨ�����*/ 
typedef enum {
	CHN0 = 0, /**< ��0�β���ͨ�����*/
  CHN1,     /**< ��1�β���ͨ�����*/
  CHN2,     /**< ��2�β���ͨ�����*/
  CHN3,     /**< ��3�β���ͨ�����*/
  CHN4,     /**< ��4�β���ͨ�����*/
  CHN5,     /**< ��5�β���ͨ�����*/
  CHN6,     /**< ��6�β���ͨ�����*/
  CHN7,     /**< ��7�β���ͨ�����*/
  CHN8,     /**< ��8�β���ͨ�����*/
  CHN9,     /**< ��9�β���ͨ�����*/
	CHN10,    /**< ��10�β���ͨ�����*/
  CHN11,    /**< ��11�β���ͨ�����*/
  CHN12,    /**< ��12�β���ͨ�����*/
  CHN13,    /**< ��13�β���ͨ�����*/
  CHN14,    /**< ��14�β���ͨ�����*/
  CHN15,    /**< ��15�β���ͨ�����*/
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
} DATx;

	/*����ADC�������*/
typedef enum 
{
		ADC_GAIN3V6 = 0,     /**< 3.6V ���� */
		ADC_GAIN7V2 = 1,     /**< 7.2V ���� */
}ADC_GAINx; 

void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
u16 ADC_GetIRQFlag(ADC_TypeDef *ADCx, uint16_t INT_flag);
void ADC_ClearIRQFlag(ADC_TypeDef *ADCx, uint16_t INT_flag);
void ADC_CHN_GAIN_CFG(ADC_TypeDef *ADCx,CHNx CHNum,u8 nChannel, ADC_GAINx GAIN3V6_or_7V2);
s16 ADC_GetConversionValue(ADC_TypeDef *ADCx,DATx DATNum);
void ADC_SoftTrgEN(ADC_TypeDef *ADCx, FuncState state);

#endif /*_CONNIE_ADC_H_ */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
