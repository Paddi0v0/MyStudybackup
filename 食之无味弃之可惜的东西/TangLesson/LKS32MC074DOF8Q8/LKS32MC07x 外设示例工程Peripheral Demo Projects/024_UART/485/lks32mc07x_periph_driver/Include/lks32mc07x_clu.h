/**
 * @file
 * @brief        CLU�⺯��ͷ�ļ�
 * @author       YangZJ
 * @date         2023-02-27
 *  */
#ifndef __LKS32MC07x_CLU_H
#define __LKS32MC07x_CLU_H
///< Includes ------------------------------------------------------------------
#include "lks32mc07x.h"
#include "lks32mc07x_sys.h"
#include "string.h"

typedef struct
{
    __IO u32 EN;  ///< 0x00 CL0 ʹ�ܼĴ���
    __IO u32 IE;  ///< 0x04 CL0 �ж�ʹ�ܼĴ���
    __IO u32 IF;  ///< 0x08 CL0 �жϱ�־�Ĵ���
    __IO u32 OUT; ///< 0x0C CL0 ����Ĵ���
    __IO u32 MX;  ///< 0x10 CL0 �����·��ѡ�Ĵ���
    __IO u32 FN;  ///< 0x14 CL0 ����ѡ��Ĵ���
    __IO u32 CF;  ///< 0x18 CL0 ���üĴ���
} CLU_TypeDef;

#ifndef CLU
#define CLU ((CLU_TypeDef *)CL0_BASE)
#endif

typedef struct
{
    u32 EN;     ///< ģ��ʹ�� 0�ر�ģ�� 1��ģ��
    u32 FIE;    ///< �½����ж�ʹ��
    u32 RIE;    ///< �������ж�ʹ��
    u32 MXA;    ///< A���븴��ѡ�� ��ʽ CLU_MXA_CLUa_b aȡֵ��ΧΪ[0 1 2 3] ��Ӧ��ͬ��CLU b��ʾѡ��ͬ�������ź�
    u32 MXB;    ///< B���븴��ѡ�� ��ʽ CLU_MXB_CLUa_b aȡֵ��ΧΪ[0 1 2 3] ��Ӧ��ͬ��CLU b��ʾѡ��ͬ�������ź�
    u32 FN;     ///< ��ֵ�� ��ʽ CLU_FN_x xȡֵ��ΧΪ[A NOTA A_NOTB ...]��ӦΪ������ʽ�µ���С�� ���� y=a+bc+!c ��ʾΪ CLU_FN_A|CLU_FN_B_C|CLU_FN_NOTC
    u32 OUTSEL; ///< ���ѡ�� 0: D ��������� 1: LUT ���
    u32 OEN;    ///< ���ʹ�� 0: �ر� 1: ʹ��
    u32 CLKINV; ///< D ������ʱ�ӵ�ƽ 0: ͬ�� 1: ����
    u32 CLKSEL; ///< D ������ʱ��ѡ�� 0x0: ��λ���� 0x1: MXA ���� 0x2: ALTCLK0 0x3: ALTCLK1
} CLU_InitTypeDef;

#define CLU_MODEL_CLU0 BIT0 ///< CLU0 CL0������4��CLU ������BIT0 BIT1 BIT2 BIT3������ �����޸�
#define CLU_MODEL_CLU1 BIT1 ///< CLU1
#define CLU_MODEL_CLU2 BIT2 ///< CLU2
#define CLU_MODEL_CLU3 BIT3 ///< CLU3

#define CLU_MXA_CLU0_CLU0OUT 0       ///< ʹ�� CLU0OUT ��ΪCLU0_A������
#define CLU_MXA_CLU0_CLU1OUT 1       ///< ʹ�� CLU1OUT ��ΪCLU0_A������
#define CLU_MXA_CLU0_CLU2OUT 2       ///< ʹ�� CLU2OUT ��ΪCLU0_A������
#define CLU_MXA_CLU0_CLU3OUT 3       ///< ʹ�� CLU3OUT ��ΪCLU0_A������
#define CLU_MXA_CLU0_Timer0ch0ovfl 4 ///< ʹ�� Timer0ch0ovfl ��ΪCLU0_A������
#define CLU_MXA_CLU0_Timer1ch0ovfl 5 ///< ʹ�� Timer1ch0ovfl ��ΪCLU0_A������
#define CLU_MXA_CLU0_PWM0 6          ///< ʹ�� PWM0 ��ΪCLU0_A������
#define CLU_MXA_CLU0_PWM1 7          ///< ʹ�� PWM1 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P0_0 8          ///< ʹ�� P0_0 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P0_3 9          ///< ʹ�� P0_3 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P0_5 10         ///< ʹ�� P0_5 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P0_7 11         ///< ʹ�� P0_7 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P2_12 12        ///< ʹ�� P2_12 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P0_12 13        ///< ʹ�� P0_12 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P0_14 14        ///< ʹ�� P0_14 ��ΪCLU0_A������
#define CLU_MXA_CLU0_P1_0 15         ///< ʹ�� P1_0 ��ΪCLU0_A������

#define CLU_MXA_CLU1_CLU0OUT 0       ///< ʹ�� CLU0OUT ��ΪCLU1_A������
#define CLU_MXA_CLU1_CLU1OUT 1       ///< ʹ�� CLU1OUT ��ΪCLU1_A������
#define CLU_MXA_CLU1_CLU2OUT 2       ///< ʹ�� CLU2OUT ��ΪCLU1_A������
#define CLU_MXA_CLU1_CLU3OUT 3       ///< ʹ�� CLU3OUT ��ΪCLU1_A������
#define CLU_MXA_CLU1_Timer0ch1ovfl 4 ///< ʹ�� Timer0ch1ovfl ��ΪCLU1_A������
#define CLU_MXA_CLU1_Timer1ch1ovfl 5 ///< ʹ�� Timer1ch1ovfl ��ΪCLU1_A������
#define CLU_MXA_CLU1_PWM2 6          ///< ʹ�� PWM2 ��ΪCLU1_A������
#define CLU_MXA_CLU1_PWM3 7          ///< ʹ�� PWM3 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P0_0 8          ///< ʹ�� P0_0 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P0_2 9          ///< ʹ�� P0_2 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P0_5 10         ///< ʹ�� P0_5 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P0_6 11         ///< ʹ�� P0_6 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P2_12 12        ///< ʹ�� P2_12 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P0_11 13        ///< ʹ�� P0_11 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P0_14 14        ///< ʹ�� P0_14 ��ΪCLU1_A������
#define CLU_MXA_CLU1_P0_15 15        ///< ʹ�� P0_15 ��ΪCLU1_A������

#define CLU_MXA_CLU2_CLU0OUT 0       ///< ʹ�� CLU0OUT ��ΪCLU2_A������
#define CLU_MXA_CLU2_CLU1OUT 1       ///< ʹ�� CLU1OUT ��ΪCLU2_A������
#define CLU_MXA_CLU2_CLU2OUT 2       ///< ʹ�� CLU2OUT ��ΪCLU2_A������
#define CLU_MXA_CLU2_CLU3OUT 3       ///< ʹ�� CLU3OUT ��ΪCLU2_A������
#define CLU_MXA_CLU2_Timer2ch0ovfl 4 ///< ʹ�� Timer2ch0ovfl ��ΪCLU2_A������
#define CLU_MXA_CLU2_Timer3ch0ovfl 5 ///< ʹ�� Timer3ch0ovfl ��ΪCLU2_A������
#define CLU_MXA_CLU2_PWM0 6          ///< ʹ�� PWM0 ��ΪCLU2_A������
#define CLU_MXA_CLU2_PWM2 7          ///< ʹ�� PWM2 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P2_8 8          ///< ʹ�� P2_8 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P1_13 9         ///< ʹ�� P1_13 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P1_15 10        ///< ʹ�� P1_15 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P1_10 11        ///< ʹ�� P1_10 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P2_1 12         ///< ʹ�� P2_1 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P2_4 13         ///< ʹ�� P2_4 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P2_6 14         ///< ʹ�� P2_6 ��ΪCLU2_A������
#define CLU_MXA_CLU2_P2_14 15        ///< ʹ�� P2_14 ��ΪCLU2_A������

#define CLU_MXA_CLU3_CLU0OUT 0       ///< ʹ�� CLU0OUT ��ΪCLU3_A������
#define CLU_MXA_CLU3_CLU1OUT 1       ///< ʹ�� CLU1OUT ��ΪCLU3_A������
#define CLU_MXA_CLU3_CLU2OUT 2       ///< ʹ�� CLU2OUT ��ΪCLU3_A������
#define CLU_MXA_CLU3_CLU3OUT 3       ///< ʹ�� CLU3OUT ��ΪCLU3_A������
#define CLU_MXA_CLU3_Timer2ch1ovfl 4 ///< ʹ�� Timer2ch1ovfl ��ΪCLU3_A������
#define CLU_MXA_CLU3_Timer3ch1ovfl 5 ///< ʹ�� Timer3ch1ovfl ��ΪCLU3_A������
#define CLU_MXA_CLU3_PWM1 6          ///< ʹ�� PWM1 ��ΪCLU3_A������
#define CLU_MXA_CLU3_PWM3 7          ///< ʹ�� PWM3 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P2_8 8          ///< ʹ�� P2_8 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P3_9 9          ///< ʹ�� P3_9 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P1_15 10        ///< ʹ�� P1_15 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P2_0 11         ///< ʹ�� P2_0 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P2_1 12         ///< ʹ�� P2_1 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P2_3 13         ///< ʹ�� P2_3 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P2_6 14         ///< ʹ�� P2_6 ��ΪCLU3_A������
#define CLU_MXA_CLU3_P2_13 15        ///< ʹ�� P2_13 ��ΪCLU3_A������

#define CLU_MXB_CLU0_CLU0OUT 0 ///< ʹ�� CLU0OUT ��ΪCLU0_B������
#define CLU_MXB_CLU0_CLU1OUT 1 ///< ʹ�� CLU1OUT ��ΪCLU0_B������
#define CLU_MXB_CLU0_CLU2OUT 2 ///< ʹ�� CLU2OUT ��ΪCLU0_B������
#define CLU_MXB_CLU0_CLU3OUT 3 ///< ʹ�� CLU3OUT ��ΪCLU0_B������
#define CLU_MXB_CLU0_ADBUSY0 4 ///< ʹ�� ADBUSY0 ��ΪCLU0_B������
#define CLU_MXB_CLU0_ADBUSY1 5 ///< ʹ�� ADBUSY1 ��ΪCLU0_B������
#define CLU_MXB_CLU0_PWM3 6    ///< ʹ�� PWM3 ��ΪCLU0_B������
#define CLU_MXB_CLU0_SPIMOSI 7 ///< ʹ�� SPIMOSI ��ΪCLU0_B������
#define CLU_MXB_CLU0_P0_2 8    ///< ʹ�� P0_2 ��ΪCLU0_B������
#define CLU_MXB_CLU0_P0_4 9    ///< ʹ�� P0_4 ��ΪCLU0_B������
#define CLU_MXB_CLU0_P0_6 10   ///< ʹ�� P0_6 ��ΪCLU0_B������
#define CLU_MXB_CLU0_P2_11 11  ///< ʹ�� P2_11 ��ΪCLU0_B������
#define CLU_MXB_CLU0_P0_11 12  ///< ʹ�� P0_11 ��ΪCLU0_B������
#define CLU_MXB_CLU0_P0_13 13  ///< ʹ�� P0_13 ��ΪCLU0_B������
#define CLU_MXB_CLU0_P0_15 14  ///< ʹ�� P0_15 ��ΪCLU0_B������
#define CLU_MXB_CLU0_P2_7 15   ///< ʹ�� P2_7 ��ΪCLU0_B������

#define CLU_MXB_CLU1_CLU0OUT 0 ///< ʹ�� CLU0OUT ��ΪCLU1_B������
#define CLU_MXB_CLU1_CLU1OUT 1 ///< ʹ�� CLU1OUT ��ΪCLU1_B������
#define CLU_MXB_CLU1_CLU2OUT 2 ///< ʹ�� CLU2OUT ��ΪCLU1_B������
#define CLU_MXB_CLU1_CLU3OUT 3 ///< ʹ�� CLU3OUT ��ΪCLU1_B������
#define CLU_MXB_CLU1_ADBUSY0 4 ///< ʹ�� ADBUSY0 ��ΪCLU1_B������
#define CLU_MXB_CLU1_ADBUSY1 5 ///< ʹ�� ADBUSY1 ��ΪCLU1_B������
#define CLU_MXB_CLU1_PWM1 6    ///< ʹ�� PWM1 ��ΪCLU1_B������
#define CLU_MXB_CLU1_SPIMISO 7 ///< ʹ�� SPIMISO ��ΪCLU1_B������
#define CLU_MXB_CLU1_P0_3 8    ///< ʹ�� P0_3 ��ΪCLU1_B������
#define CLU_MXB_CLU1_P0_4 9    ///< ʹ�� P0_4 ��ΪCLU1_B������
#define CLU_MXB_CLU1_P0_7 10   ///< ʹ�� P0_7 ��ΪCLU1_B������
#define CLU_MXB_CLU1_P2_11 11  ///< ʹ�� P2_11 ��ΪCLU1_B������
#define CLU_MXB_CLU1_P0_12 12  ///< ʹ�� P0_12 ��ΪCLU1_B������
#define CLU_MXB_CLU1_P0_13 13  ///< ʹ�� P0_13 ��ΪCLU1_B������
#define CLU_MXB_CLU1_P1_0 14   ///< ʹ�� P1_0 ��ΪCLU1_B������
#define CLU_MXB_CLU1_P2_7 15   ///< ʹ�� P2_7 ��ΪCLU1_B������

#define CLU_MXB_CLU2_CLU0OUT 0 ///< ʹ�� CLU0OUT ��ΪCLU2_B������
#define CLU_MXB_CLU2_CLU1OUT 1 ///< ʹ�� CLU1OUT ��ΪCLU2_B������
#define CLU_MXB_CLU2_CLU2OUT 2 ///< ʹ�� CLU2OUT ��ΪCLU2_B������
#define CLU_MXB_CLU2_CLU3OUT 3 ///< ʹ�� CLU3OUT ��ΪCLU2_B������
#define CLU_MXB_CLU2_PWM1 4    ///< ʹ�� PWM1 ��ΪCLU2_B������
#define CLU_MXB_CLU2_PWM3 5    ///< ʹ�� PWM3 ��ΪCLU2_B������
#define CLU_MXB_CLU2_I2CSDA 6  ///< ʹ�� I2CSDA ��ΪCLU2_B������
#define CLU_MXB_CLU2_SPICLK 7  ///< ʹ�� SPICLK ��ΪCLU2_B������
#define CLU_MXB_CLU2_P3_9 8    ///< ʹ�� P3_9 ��ΪCLU2_B������
#define CLU_MXB_CLU2_P1_14 9   ///< ʹ�� P1_14 ��ΪCLU2_B������
#define CLU_MXB_CLU2_P2_0 10   ///< ʹ�� P2_0 ��ΪCLU2_B������
#define CLU_MXB_CLU2_P1_11 11  ///< ʹ�� P1_11 ��ΪCLU2_B������
#define CLU_MXB_CLU2_P2_3 12   ///< ʹ�� P2_3 ��ΪCLU2_B������
#define CLU_MXB_CLU2_P2_5 13   ///< ʹ�� P2_5 ��ΪCLU2_B������
#define CLU_MXB_CLU2_P2_13 14  ///< ʹ�� P2_13 ��ΪCLU2_B������
#define CLU_MXB_CLU2_P2_15 15  ///< ʹ�� P2_15 ��ΪCLU2_B������

#define CLU_MXB_CLU3_CLU0OUT 0 ///< ʹ�� CLU0OUT ��ΪCLU3_B������
#define CLU_MXB_CLU3_CLU1OUT 1 ///< ʹ�� CLU1OUT ��ΪCLU3_B������
#define CLU_MXB_CLU3_CLU2OUT 2 ///< ʹ�� CLU2OUT ��ΪCLU3_B������
#define CLU_MXB_CLU3_CLU3OUT 3 ///< ʹ�� CLU3OUT ��ΪCLU3_B������
#define CLU_MXB_CLU3_PWM0 4    ///< ʹ�� PWM0 ��ΪCLU3_B������
#define CLU_MXB_CLU3_PWM2 5    ///< ʹ�� PWM2 ��ΪCLU3_B������
#define CLU_MXB_CLU3_I2CSCL 6  ///< ʹ�� I2CSCL ��ΪCLU3_B������
#define CLU_MXB_CLU3_SPICSN 7  ///< ʹ�� SPICSN ��ΪCLU3_B������
#define CLU_MXB_CLU3_P1_13 8   ///< ʹ�� P1_13 ��ΪCLU3_B������
#define CLU_MXB_CLU3_P1_14 9   ///< ʹ�� P1_14 ��ΪCLU3_B������
#define CLU_MXB_CLU3_P1_10 10  ///< ʹ�� P1_10 ��ΪCLU3_B������
#define CLU_MXB_CLU3_P1_11 11  ///< ʹ�� P1_11 ��ΪCLU3_B������
#define CLU_MXB_CLU3_P2_4 12   ///< ʹ�� P2_4 ��ΪCLU3_B������
#define CLU_MXB_CLU3_P2_5 13   ///< ʹ�� P2_5 ��ΪCLU3_B������
#define CLU_MXB_CLU3_P2_14 14  ///< ʹ�� P2_14 ��ΪCLU3_B������
#define CLU_MXB_CLU3_P2_15 15  ///< ʹ�� P2_15 ��ΪCLU3_B������

#define CLU_FN_0 0x0               ///< CLU��ֵ�����С�� 0        ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_1 0xff              ///< CLU��ֵ�����С�� 1        ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A 0xf0              ///< CLU��ֵ�����С�� a        ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA 0x0f           ///< CLU��ֵ�����С�� !a       ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_B 0xcc              ///< CLU��ֵ�����С�� b        ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTB 0x33           ///< CLU��ֵ�����С�� !b       ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_C 0xaa              ///< CLU��ֵ�����С�� c        ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTC 0x55           ///< CLU��ֵ�����С�� !c       ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_B 0xc0            ///< CLU��ֵ�����С�� a&b      ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_NOTB 0x03      ///< CLU��ֵ�����С�� !a&!b    ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_B 0x0c         ///< CLU��ֵ�����С�� !a&b     ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_NOTB 0x30         ///< CLU��ֵ�����С�� a&!b     ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_C 0xa0            ///< CLU��ֵ�����С�� a&c      ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_NOTC 0x05      ///< CLU��ֵ�����С�� !a&!c    ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_C 0x0a         ///< CLU��ֵ�����С�� !a&c     ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_NOTC 0x50         ///< CLU��ֵ�����С�� a&!c     ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_B_C 0x88            ///< CLU��ֵ�����С�� b&c      ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTB_NOTC 0x11      ///< CLU��ֵ�����С�� !b&!c    ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTB_C 0x22         ///< CLU��ֵ�����С�� !b&c     ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_B_NOTC 0x44         ///< CLU��ֵ�����С�� b&!c     ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_NOTB_NOTC 0x01 ///< CLU��ֵ�����С�� !a&!b&!c ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_NOTB_C 0x02    ///< CLU��ֵ�����С�� !a&!b&c  ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_B_NOTC 0x04    ///< CLU��ֵ�����С�� !a&b&!c  ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_NOTA_B_C 0x08       ///< CLU��ֵ�����С�� !a&b&c   ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_NOTB_NOTC 0x10    ///< CLU��ֵ�����С�� a&!b&!c  ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_NOTB_C 0x20       ///< CLU��ֵ�����С�� a&!b&c   ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_B_NOTC 0x40       ///< CLU��ֵ�����С�� a&b&!c   ע����С��֮��ֻ���� | ��C���԰�λ��
#define CLU_FN_A_B_C 0x80          ///< CLU��ֵ�����С�� a&b&c    ע����С��֮��ֻ���� | ��C���԰�λ��

void CLU_Init(CLU_TypeDef *CLUx,u8 CLU_MODEL_CLUx, CLU_InitTypeDef *CLU_InitStruct);
void CLU_StructInit(CLU_InitTypeDef *CLU_InitStruct);

void CLU_Enable(CLU_TypeDef *CLUx);
void CLU_Disable(CLU_TypeDef *CLUx);

u8 CLU_ReadOutVolue(CLU_TypeDef *CLUx,u8 CLU_MODEL_CLUx);
#endif /*__LKS32MC08x_CLU_H */
