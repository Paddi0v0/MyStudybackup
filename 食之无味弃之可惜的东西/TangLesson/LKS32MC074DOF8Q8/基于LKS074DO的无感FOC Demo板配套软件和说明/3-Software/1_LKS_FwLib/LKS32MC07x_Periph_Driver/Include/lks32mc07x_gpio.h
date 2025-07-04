 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_gpio.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� GPIO������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��21��     <td>1.0     <td>cakezhang      <td>����
 * </table>
 */
#ifndef __lks32mc07x_GPIO_H
#define __lks32mc07x_GPIO_H


///< Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"
/** 
 *@brief GPIO�Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t PIE     ; /*!< 0x00 GPIO x ����ʹ��*/
    __IO uint32_t POE     ; /*!< 0x04 GPIO x ���ʹ��*/
    __IO uint32_t PDI     ; /*!< 0x08 GPIO x ��������*/
    __IO uint32_t PDO     ; /*!< 0x0C GPIO x �������*/
    __IO uint32_t PUE     ; /*!< 0x10 GPIO x ����ʹ��*/
    __IO uint32_t null    ; /*!< 0x14 �ṹ���ַ���룬������*/
    __IO uint32_t PODE    ; /*!< 0x18 GPIO x ��©ʹ��*/
    __IO uint32_t PFLT    ; /*!< 0x1c GPIO x �˲�ʹ��*/
    __IO uint32_t F3210   ; /*!< 0x20 GPIO x [3:0]����ѡ��*/
    __IO uint32_t F7654   ; /*!< 0x24 GPIO x [7:4]����ѡ��*/
    __IO uint32_t FBA98   ; /*!< 0x28 GPIO x [11:8]����ѡ��*/
    __IO uint32_t FFEDC   ; /*!< 0x2C GPIO x [15:12]����ѡ��*/
    __IO uint32_t BSRR    ; /*!< 0x30 GPIO x λ�����Ĵ���*/
    __IO uint32_t BRR     ; /*!< 0x34 GPIO x λ����Ĵ���*/
} GPIO_TypeDef;

/**
 * GPIO0�ṹ�����ַ����
 */
#define GPIO0            ((GPIO_TypeDef *) GPIO0_BASE)

/**
 * GPIO1�ṹ�����ַ����
 */
#define GPIO1            ((GPIO_TypeDef *) GPIO1_BASE)

/**
 * GPIO2�ṹ�����ַ����
 */
#define GPIO2            ((GPIO_TypeDef *) GPIO2_BASE)

/**
 * GPIO3�ṹ�����ַ����
 */
#define GPIO3            ((GPIO_TypeDef *) GPIO3_BASE)

/** 
 *@brief GPIOģʽѡ��ö��
 */
typedef enum
{
   GPIO_Mode_IN     = 0x00,     /*!< GPIO ����&���裬ģ�������ͬʱ��Ч*/
   GPIO_Mode_OUT    = 0x01,     /*!< GPIO ���*/
   GPIO_Mode_ANA    = 0x02      /*!< GPIO ����*/
}GPIOMode_TypeDef;


/** 
 *@brief GPIO���ģʽѡ��ö��
 */
typedef enum
{
   GPIO_PuPd_NOPULL = 0x00,  /*!< ����̬ */
   GPIO_PuPd_UP     = 0x01,  /*!< ���� */
}GPIOPuPd_TypeDef;

/** 
 *@brief GPIO�Ĵ���д����ö��
 */
typedef enum
{ 
   Bit_RESET = 0,
   Bit_SET
}BitAction;


/** 
 *@brief  GPIO�������ýṹ����
 */
typedef struct
{
   u32 GPIO_Pin;        ///< Ҫ���õ�����                            
   u32 GPIO_Mode;       ///< GPIOģʽ
   u32 GPIO_PuPd;       ///< ����ʹ��
   u32 GPIO_PODEna;     ///< ��©ʹ��
   u32 GPIO_PFLT;
}GPIO_InitTypeDef;

#define AF0_GPIO         0      /*!< GPIO���ù���0��ģ�⹦�� */
#define AF1_SYSAF        1      /*!< GPIO���ù���1��SYS_AF��CMP��CLOCK�������ź�������� */
#define AF2_HALL         2      /*!< GPIO���ù���2��HALL���� */
#define AF3_MCPWM        3      /*!< GPIO���ù���3��MCPWM���� */
#define AF4_UART         4      /*!< GPIO���ù���4��UART���� */
#define AF5_SPI          5      /*!< GPIO���ù���5��SPI���� */
#define AF6_I2C          6      /*!< GPIO���ù���6��I2C���� */
#define AF7_TIMER01      7      /*!< GPIO���ù���7��Timer0��Timer1���� */
#define AF8_TIMER23      8      /*!< GPIO���ù���8��Timer2��Timer3��QEP0��QEP1���� */
#define AF9_DEBUG        9      /*!< GPIO���ù���9��ADC trigger debug���� */
#define AF10_CAN         10     /*!< GPIO���ù���10��CAN���� */
#define AF11_SIF         11     /*!< GPIO���ù���11��SIF���� */
#define AF12_CL          12     /*!< GPIO���ù���12��CL���� */

/*GPIO_Pad define*/
#define GPIO_Pin_NONE     0        /*!<GPIO pin�Ŷ��壬��pin��  */
#define GPIO_Pin_0        BIT0     /*!<GPIO pin�Ŷ��壬pin_0��  */
#define GPIO_Pin_1        BIT1     /*!<GPIO pin�Ŷ��壬pin_1��  */
#define GPIO_Pin_2        BIT2     /*!<GPIO pin�Ŷ��壬pin_2��  */
#define GPIO_Pin_3        BIT3     /*!<GPIO pin�Ŷ��壬pin_3��  */
#define GPIO_Pin_4        BIT4     /*!<GPIO pin�Ŷ��壬pin_4��  */
#define GPIO_Pin_5        BIT5     /*!<GPIO pin�Ŷ��壬pin_5��  */
#define GPIO_Pin_6        BIT6     /*!<GPIO pin�Ŷ��壬pin_6��  */
#define GPIO_Pin_7        BIT7     /*!<GPIO pin�Ŷ��壬pin_7��  */
#define GPIO_Pin_8        BIT8     /*!<GPIO pin�Ŷ��壬pin_8��  */
#define GPIO_Pin_9        BIT9     /*!<GPIO pin�Ŷ��壬pin_9��  */
#define GPIO_Pin_10       BIT10    /*!<GPIO pin�Ŷ��壬pin_10�� */
#define GPIO_Pin_11       BIT11    /*!<GPIO pin�Ŷ��壬pin_11�� */
#define GPIO_Pin_12       BIT12    /*!<GPIO pin�Ŷ��壬pin_12�� */
#define GPIO_Pin_13       BIT13    /*!<GPIO pin�Ŷ��壬pin_13�� */
#define GPIO_Pin_14       BIT14    /*!<GPIO pin�Ŷ��壬pin_14�� */
#define GPIO_Pin_15       BIT15    /*!<GPIO pin�Ŷ��壬pin_15�� */
#define GPIO_Pin_ALL      (uint32_t)0xFFFF  /*!<GPIO pin�Ŷ��壬����pin��*/

/*GPIO_Pad_sources define*/
#define GPIO_PinSource_0  0     /*!<GPIO pin�Ŷ��壬pin_0�� */
#define GPIO_PinSource_1  1     /*!<GPIO pin�Ŷ��壬pin_1�� */
#define GPIO_PinSource_2  2     /*!<GPIO pin�Ŷ��壬pin_2�� */
#define GPIO_PinSource_3  3     /*!<GPIO pin�Ŷ��壬pin_3�� */
#define GPIO_PinSource_4  4     /*!<GPIO pin�Ŷ��壬pin_4�� */
#define GPIO_PinSource_5  5     /*!<GPIO pin�Ŷ��壬pin_5�� */
#define GPIO_PinSource_6  6     /*!<GPIO pin�Ŷ��壬pin_6�� */
#define GPIO_PinSource_7  7     /*!<GPIO pin�Ŷ��壬pin_7�� */
#define GPIO_PinSource_8  8     /*!<GPIO pin�Ŷ��壬pin_8�� */
#define GPIO_PinSource_9  9     /*!<GPIO pin�Ŷ��壬pin_9�� */
#define GPIO_PinSource_10 10    /*!<GPIO pin�Ŷ��壬pin_10�� */
#define GPIO_PinSource_11 11    /*!<GPIO pin�Ŷ��壬pin_11�� */
#define GPIO_PinSource_12 12    /*!<GPIO pin�Ŷ��壬pin_12�� */
#define GPIO_PinSource_13 13    /*!<GPIO pin�Ŷ��壬pin_13�� */
#define GPIO_PinSource_14 14    /*!<GPIO pin�Ŷ��壬pin_14�� */
#define GPIO_PinSource_15 15    /*!<GPIO pin�Ŷ��壬pin_15�� */

/* ��ʼ�������ú��� *********************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);

void GPIO_LockRegister(GPIO_TypeDef* GPIOx);
void GPIO_UnlockRegister(GPIO_TypeDef* GPIOx);

/* GPIO��д���� *************************************/
uint8_t  GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint32_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint32_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint32_t Val);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

/* GPIO����ѡ���� ***************************/
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint32_t GPIO_PinSource, uint32_t GPIO_AF);

#endif /*__lks32mc07x_GPIO_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
