/**
 * LKS32MC081C8T8 GPIO ����ָ��
 * ===========================================================
 *
 * һ��GPIO ����ģʽ (GPIOMode_TypeDef)
 * -----------------------------------------------------------
 * 1. ����ģʽ (GPIO_Mode_IN)   : ��ȡ�ⲿ�����ź�
 *    - ��������: GPIO_PuPd_NOPULL
 *    - ��������: GPIO_PuPd_UP
 *
 * 2. ���ģʽ (GPIO_Mode_OUT)  : �����ⲿ�豸
 *    - �������: GPIO_PODEna = 0 (Ĭ��)
 *    - ��©���: GPIO_PODEna = 1 (�������������)
 *
 * 3. ģ��ģʽ (GPIO_Mode_ANA)  : ADC/DAC ģ���źŴ���
 *    - �������ֹ���
 *
 * 4. ͨ��IOģʽ (GPIO_Mode_IO) : ˫�����ݴ���
 *
 * �������ģʽ���
 * -----------------------------------------------------------
 * ������� vs ��©���:
 *
 * | ����         | �������                  | ��©���                  |
 * |--------------|--------------------------|--------------------------|
 * | ��·�ṹ     | P-MOS + N-MOS            | ��N-MOS                  |
 * | �ߵ�ƽ����   | ����������VDD            | �����ⲿ����             |
 * | �͵�ƽ����   | ����������GND            | ����������GND            |
 * | ��������     | ǿ��˫��������            | �����������ͣ�            |
 * | ���߳�ͻ���� | ��                       | ��Ȼ����·               |
 * | ��ƽת��     | ��֧��                   | ֧�֣��ı�������ѹ��      |
 * | ����Ӧ��     | LED/�̵�������           | I2C����/��ƽת��         |
 *
 * �����ؼ����ýṹ�� (GPIO_InitTypeDef)
 * -----------------------------------------------------------
 * typedef struct {
 *   uint32_t GPIO_Pin;          // ����ѡ�� (GPIO_Pin_0~15)
 *   GPIOMode_TypeDef GPIO_Mode; // ����ģʽ (IN/OUT/ANA/IO)
 *   GPIOPuPd_TypeDef GPIO_PuPd; // �������� (NOPULL/UP)
 *   uint32_t GPIO_PODEna;       // ��©ʹ�� (0:����, 1:��©)
 * } GPIO_InitTypeDef;
 *
 * �ġ����ù�������
 * -----------------------------------------------------------
 * ʹ�� GPIO_PinAFConfig() �������Ÿ��ù���:
 *   - AF4_UART  : ����ͨ��
 *   - AF5_SPI   : SPI�ӿ�
 *   - AF6_I2C   : I2C����
 *   - AF10_CAN  : CAN����
 *
 * �塢������������
 * -----------------------------------------------------------
 * ��ֹ�����޸�����:
 *   GPIO_LockRegister()   : ��������
 *   GPIO_UnlockRegister() : ��������
 *
 * �������ʵ��
 * -----------------------------------------------------------
 * 1. LED����: ������� + ��������
 * 2. ��ť���: ��������
 * 3. I2C����: ��©��� + 4.7k����������
 * 4. ADC�ɼ�: ģ��ģʽ
 *
 * ע��: ʹ��ǰ������GPIOʱ�� (RCC�������)
 * ===========================================================
 */

#ifndef __MYGPIO_INIT_H_
#define __MYGPIO_INIT_H_

#include "lks32mc08x_lib.h"

void GPIO_init(void);

#endif

/*
// ʾ����LED��ʼ������
void LED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // ����1: ���ýṹ�����
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;      // ѡ��GPIO1��Pin0
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   // ���ģʽ
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;// ��������/����
    GPIO_InitStruct.GPIO_PODEna = 0;             // �������

    // ����2: ��ʼ��GPIO
    GPIO_Init(GPIO1, &GPIO_InitStruct);

    // ����3: ��ʼ״̬��Ϊ�͵�ƽ(����LED������GPIO)
    GPIO_ResetBits(GPIO1, GPIO_Pin_0);
}

// ʾ������ť��ʼ������
void Button_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;    // ����ģʽ
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;    // �����ڲ�����
    GPIO_InitStruct.GPIO_PODEna = 0;             // �޹ز���

    GPIO_Init(GPIO0, &GPIO_InitStruct);
}

// ʾ����I2C���ų�ʼ��
void I2C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // SDA������ (PB9)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   // ���ģʽ
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;// �����ڲ�����
    GPIO_InitStruct.GPIO_PODEna = 1;             // ��©���
    GPIO_Init(GPIO1, &GPIO_InitStruct);

    // ���ù�������
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_9, AF6_I2C);
}
*/
