/**
 * @file
 * @defgroup i2c
 * @brief        i2c�⺯��
 * @author       YangZJ
 * @date         2023-01-02
 * @{
 */
#include "lks32mc07x_i2c.h"
#include "lks32mc07x_sys.h"
#include "lks32mc07x_dma.h"

/**
 * @brief    I2C��ʼ��
 * @param this I2C���ýṹ��
 * @par ���¼�¼  V1.0  2021/11/10  YangZJ  ����
 */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *this)
{
    SYS_ModuleClockCmd(SYS_Module_I2C0, ENABLE);
    SYS_SoftResetModule(SYS_Module_I2C0);

    {
        u32 BaudRate;
        BaudRate = ((u32)48000000 + ((this->BaudRate * 17) >> 1)) / (this->BaudRate * 17) - 1;
        SYS_WR_PROTECT = 0x7A83; // ���ϵͳ�Ĵ���д����
        SYS_CLK_DIV0 = BaudRate;
        SYS_WR_PROTECT = 0; // ���ϵͳ�Ĵ���д����
    }
    I2Cx->ADDR = (this->ADRCMP << 7);
    I2Cx->CFG = (this->IE << 7) | (this->TC_IE << 6) |
                (this->BUS_ERR_IE << 5) | (this->STOP_IE << 4) |
                (this->MST_MODE << 1) | (this->SLV_MODE);
    I2Cx->SCR = 0;
    I2Cx->DATA = 0;
    I2Cx->MSCR = 0;
    I2Cx->BCR = (this->BURST_NACK << 7) | (this->BURST_ADDR_CMP << 6) |
                (this->BUSRT_EN << 5);
}
/**
 * @brief    I2C�ṹ���ʼ��
 * @param this I2C���ýṹ��
 * @par ���¼�¼  V1.0  2021/11/10  YangZJ  ����
 */
void I2C_StructInit(I2C_InitTypeDef *this)
{
    this->ADRCMP = DISABLE;         //  I2C Ӳ����ַ�Ƚ�ʹ�ܿ��أ�ֻ���� DMA ģʽ�¿�������Ч��
    this->MST_MODE = DISABLE;       //  I2C ��ģʽʹ��
    this->SLV_MODE = DISABLE;       //  I2C ��ģʽʹ��
    this->BUSRT_EN = DISABLE;       //  I2C DMA����ʹ��
    this->BaudRate = 100000;        //  I2C ������
    this->IE = DISABLE;             //  I2C �ж�ʹ��
    this->TC_IE = DISABLE;          //  I2C ���ݴ�������ж�ʹ��
    this->BUS_ERR_IE = DISABLE;     //  I2C ���ߴ����¼��ж�ʹ��
    this->STOP_IE = DISABLE;        //  I2C STOP �¼��ж�ʹ��
    this->BURST_NACK = DISABLE;     //  I2C ���䣬NACK �¼��ж�ʹ��
    this->BURST_ADDR_CMP = DISABLE; //  I2C ���䣬Ӳ����ַƥ���ж�ʹ��
}
/**
 * @brief    ��I2C����״̬
 * @param this I2C���ýṹ��
 * @par ���¼�¼  V1.0  2021/11/10  YangZJ  ����
 */
u8 I2c_ReadBusState(I2C_TypeDef *I2Cx, u16 n)
{
    if (I2Cx->SCR & n)
    {
        return ENABLE;
    }
    else
    {
        return DISABLE;
    }
}
/**
 * @brief    I2C����״̬��λ
 * @param this I2C���ýṹ��
 * @par ���¼�¼  V1.0  2021/11/10  YangZJ  ����
 */
void I2c_ClearBusState(I2C_TypeDef *I2Cx, u16 n)
{
    I2Cx->SCR &= ~n;
}
/**
 * @brief    I2C��������
 * @par ���¼�¼  V1.0  2016/05/21  YangZJ  ����
 */
void I2C_SendData(I2C_TypeDef *I2Cx, u8 n)
{
    I2Cx->DATA = n;
}

/**
 * @brief    I2C������������
 * @param I2C_TypeDef *I2Cx
 * @return ����������
 * @par ���¼�¼  V1.0  2016/05/21  YangZJ  ����
 */
u8 I2C_ReadData(I2C_TypeDef *I2Cx)
{
    return (u8)I2Cx->DATA;
}

/**
 * @brief    I2C����ж�
 �����ı�
 ���������
 * @par ���¼�¼  V1.0  2022/08/30  YangZJ  ����
*/
void I2C_ClearIRQFlag(I2C_TypeDef *I2Cx, u16 I2C_IF_x)
{
    switch (I2C_IF_x)
    {
    case I2C_IF_TC: // ��������ж�
        I2Cx->SCR &= ~BIT0;
        break;
    case I2C_IF_BUS_ERR: // ���ߴ����ж�
        I2Cx->SCR &= ~BIT7;
        break;
    case I2C_IF_STOP: // STOP�¼��ж�
        I2Cx->SCR &= ~BIT5;
        break;
    case I2C_IF_NACK: // NACK�¼��ж�
        I2Cx->SCR &= ~BIT1;
        break;
    case I2C_IF_ADDR_CMP: // Ӳ����ַ�Ƚ��ж�

        break;
    default:
        break;
    }
}
/**
 * @brief    I2C��ȡ��ǰ�ж�״̬
 �����ı�
 ���������
 * @par ���¼�¼  V1.0  2022/08/30  YangZJ  ����
*/
u8 I2C_GetIRQFlag(I2C_TypeDef *I2Cx, u16 I2C_IF_x)
{
    switch (I2C_IF_x)
    {
    case I2C_IF_TC: // ��������ж�
        if ((I2Cx->CFG & BIT7) == 0)
        {
            return 0;
        }
        if ((I2Cx->CFG & BIT6) == 0)
        {
            return 0;
        }
        if (I2Cx->SCR & BIT0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_BUS_ERR: // ���ߴ����ж�
        if ((I2Cx->CFG & BIT7) == 0)
        {
            return 0;
        }
        if ((I2Cx->CFG & BIT5) == 0)
        {
            return 0;
        }
        if (I2Cx->SCR & BIT7)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_STOP: // STOP�¼��ж�
        if ((I2Cx->CFG & BIT7) == 0)
        {
            return 0;
        }
        if ((I2Cx->CFG & BIT4) == 0)
        {
            return 0;
        }
        if (I2Cx->SCR & BIT5)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_NACK: // NACK�¼��ж�
        if ((I2Cx->CFG & BIT7) == 0)
        {
            return 0;
        }
        if ((I2Cx->BCR & BIT7) == 0)
        {
            return 0;
        }
        if (I2Cx->SCR & BIT1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_ADDR_CMP: // Ӳ����ַƥ���ж�
        if ((I2Cx->CFG & BIT7) == 0)
        {
            return 0;
        }
        if ((I2Cx->BCR & BIT6) == 0)
        {
            return 0;
        }
        if (I2Cx->SCR & BIT3)
        {
            if ((u32)(I2Cx->DATA >> 1) == (I2Cx->ADDR & 0x7f))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    default:
        return 0;
    }
}
/**
 * @brief    I2C����ж�
 * @param I2C_TypeDef *I2Cx
 * @par ���¼�¼  V1.0  2022/08/30  YangZJ  ����
 */
u8 I2C_GetFlag(I2C_TypeDef *I2Cx, u16 I2C_IF_x)
{
    switch (I2C_IF_x)
    {
    case I2C_IF_TC: // ��������ж�
        if (I2Cx->SCR & BIT0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_BUS_ERR: // ���ߴ����ж�
        if (I2Cx->SCR & BIT7)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_STOP: // STOP�¼��ж�
        if (I2Cx->SCR & BIT5)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_NACK: // NACK�¼��ж�
        if (I2Cx->SCR & BIT1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case I2C_IF_ADDR_CMP: // Ӳ����ַƥ���ж�
        if (I2Cx->SCR & BIT3)
        {
            if ((u32)(I2Cx->DATA >> 1) == (I2Cx->ADDR & 0x7f))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    default:
        return 0;
    }
}

/**
 * @brief    I2Cģ��ʹ��
 * @param I2C_TypeDef *I2Cx
 * @par ���¼�¼  V1.0  2022/08/30  YangZJ  ����
 */
void I2C_Enable(I2C_TypeDef *I2Cx)
{
    if (I2Cx == I2C0)
    {
        SYS_ModuleClockCmd(SYS_Module_I2C0, ENABLE);
    }
}
/**
 * @brief    I2Cģ��ر�
 * @param I2C_TypeDef *I2Cx
 * @par ���¼�¼  V1.0  2022/08/30  YangZJ  ����
 */
void I2C_Disable(I2C_TypeDef *I2Cx)
{
    if (I2Cx == I2C0)
    {
        SYS_ModuleClockCmd(SYS_Module_I2C0, DISABLE);
    }
}

/**
 * @brief    ����I2Cģ��Ĳ�����
 * @param BaudRate ������
 * @par ���¼�¼  V1.0  2022/08/30  YangZJ  ����
 */
void I2C_SetBaudRate(u32 BaudRate)
{
    u32 div;
    div = ((u32)48000000 + ((BaudRate * 17) >> 1)) / (BaudRate * 17) - 1;
    SYS_WR_PROTECT = 0x7A83;
    SYS_CLK_DIV0 = div;
    SYS_WR_PROTECT = 0;
}

/*! @} */
