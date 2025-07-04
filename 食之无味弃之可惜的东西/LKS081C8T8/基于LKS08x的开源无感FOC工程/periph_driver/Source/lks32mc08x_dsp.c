/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc08x_dsp.c
 * �ļ���ʶ��
 * ����ժҪ�� DSPЭ������ģʽ��������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� YangZJ
 * ������ڣ� 2022��3��10��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2022��3��10��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�YangZJ
 * �޸����ݣ�����
 *
 *******************************************************************************/
#include "lks32mc08x.h"
#include "lks32mc08x_sys.h"
#include "lks32mc08x_dsp.h"
static volatile u8 dsp_flg; // ��־λ�������жϳ����Ƿ񱻴��
/*******************************************************************************
 �������ƣ�    s16 lks08x_dsp_sin(s16 val)
 ����������    ʹ��08x��DSP��������
 ���������    u16  val     �Ƕ�0-65535��Ӧ0-360��
 �� �� ֵ��    s16 result  ������Q15��ʽ
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
stru_Dsp_sincos lks08x_dsp_sin_cos(s16 val)
{
    stru_Dsp_sincos result;
    
    u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_SC = BIT2 | BIT1;
        DSP_CORDIC_THETA = (u32)val;
        result.hSin = (s16)DSP_CORDIC_SIN;
        result.hCos = (s16)DSP_CORDIC_COS;
    } while (dsp_flg_old != dsp_flg);
    return result;
}
/*******************************************************************************
 �������ƣ�    s16 lks08x_dsp_sin(s16 val)
 ����������    ʹ��08x��DSP��������
 ���������    u16  val     �Ƕ�0-65535��Ӧ0-360��
 �� �� ֵ��    s16 result  ������Q15��ʽ
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
s16 lks08x_dsp_sin(s16 val)
{
    s16 result;
    u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_SC = BIT2 | BIT1;
        DSP_CORDIC_THETA = (u32)val;
        result = (s16)DSP_CORDIC_SIN;
    } while (dsp_flg_old != dsp_flg);
    return result;
}
/*******************************************************************************
 �������ƣ�    s16 lks08x_dsp_cos(s16 val)
 ����������    ʹ��08x��DSP��������
 ���������    u16  val    �Ƕ�0-65535��Ӧ0-360��
 �� �� ֵ��    s16  result ������Q15��ʽ
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
s16 lks08x_dsp_cos(s16 val)
{
    s16 result;
    u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_SC = BIT2 | BIT1;
        DSP_CORDIC_THETA = (u32)val;
        result = (s16)DSP_CORDIC_COS;
    } while (dsp_flg_old != dsp_flg);
    return result;
}
/*******************************************************************************
 �������ƣ�    u16 lks08x_dsp_rms(s16 a,s16 b)
 ����������    ʹ��08x��DSP���������
 ���������    s16 a,b
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
u16 lks08x_dsp_rms(s16 a, s16 b)
{
    u16 c;
    u16 result;
    u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        //�ȴ�������
        //�ڼ���Ĺ������ж������Ƿ�������
        a = (a > 0) ? a : -a;
        b = (b > 0) ? b : -b;
        c = (u16)(a + b);
        SYS_SoftResetModule(SYS_Module_DSP);
        if (c > 32767) // �����ݿ��������ʱ�򽵵�1λ���Ȳ����¿�ʼ����
        {
            DSP_CORDIC_X = (u32)(a >> 1);
            DSP_CORDIC_Y = (u32)(b >> 1);
            result = (u16)DSP_CORDIC_MOD;
            result <<= 1;
        }
        else // ��������
        {
            DSP_CORDIC_X = (u32)a;
            DSP_CORDIC_Y = (u32)b;
            result = (u16)DSP_CORDIC_MOD;
        }
    } while (dsp_flg_old != dsp_flg);
    return result;
}
/*******************************************************************************
 �������ƣ�    u16 lks08x_dsp_arctan(s16 x,s16 y)
 ����������    ʹ��08x��DSP���㷴����
 ���������    s16  x,y
 �� �� ֵ��    ������ �Ƕ�0-65535��Ӧ0-360��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
u16 lks08x_dsp_arctan(s16 x, s16 y)
{
    u16 a;
    u8 i;
    u16 result = 0;
    u8 dsp_flg_old;
    a = (u16)((x > 0) ? x : -x);
    a = (u16)((y > 0) ? a + y : a - y);
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        for (i = 0; i < 16; i++)
        {
            if ((a & (0x8000 >> i)) != 0)
            {
                if (i == 0)
                {
                    // 08x��DSP�ڼ���Ƕ�ʱ���뱣֤(x*x+y*y)<32767
                    SYS_SoftResetModule(SYS_Module_DSP);
                    DSP_CORDIC_X = (u32)x >> 1;
                    DSP_CORDIC_Y = (u32)y >> 1;
                }
                else
                {
                    // 08x��DSP�ڼ���atanʱΪ��߾��ȣ�Ӧ�����ܵķŵ�X��Y�ķ�ֵ
                    SYS_SoftResetModule(SYS_Module_DSP);
                    DSP_CORDIC_X = (u32)x << (i - 1);
                    DSP_CORDIC_Y = (u32)y << (i - 1);
                }
                result = (u16)DSP_CORDIC_ARCTAN;
                break;
            }
        }
    } while (dsp_flg_old != dsp_flg);
    return result;
}
/*******************************************************************************
 �������ƣ�    s32 lks08x_dsp_div(s32 a,s16 b)
 ����������    ʹ��08x��DSP�������
 ���������    s32  a         ������
               s16  b         ����
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
s32 lks08x_dsp_div(s32 a, s16 b)
{
    s32 c;
    s32 result;
    u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_DID = (u32)a;
        DSP_DIS = (u32)b;
        if (b == (s16)0x8000)
        {
            c = -a - 1;
            result = (c >> 15) + 1;
        }
        else if (b == 0)
        {
            result = 0;
        }
        else
        {
            if (a != (s16)0x80000000)
            {
                result = (s32)DSP_QUO;
            }
            else
            {
                a >>= 1;
                DSP_DID = (u32)a;
                DSP_DIS = (u32)b;
                result = ((s32)DSP_QUO)<< 1;
            }
        }
    } while (dsp_flg_old != dsp_flg);
    return result;
}
/*******************************************************************************
 �������ƣ�    s32 lks08x_dsp_mod(s32 a,s16 b)
 ����������    ʹ��08x��DSP��ģ
 ���������    s32  a         ������
               s16  b         ����
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
s32 lks08x_dsp_mod(s32 a, s16 b)
{
    s32 result;
    u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_DID = (u32)a;
        DSP_DIS = (u32)b;
        if (b == 0)
        {
            result = a;
        }
        else
        {
            result = (s32)DSP_REM;
        }
    } while (dsp_flg_old != dsp_flg);
    return result;
}
/*******************************************************************************
 �������ƣ�    u16 lks08x_dsp_sqrt(u32 val)
 ����������    ʹ��08x��DSP��ƽ��
 ���������    u32 val ��������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022.03.10    V1.0           YangZJ            ����
 *******************************************************************************/
u16 lks08x_dsp_sqrt(u32 val)
{
    u16 result;
    u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_RAD = val;
        result = (u16)DSP_SQRT;
    } while (dsp_flg_old != dsp_flg);
    return result;
}
