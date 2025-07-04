/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� i2c_driver.c
 * �ļ���ʶ��
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� YangZJ
 * ������ڣ� 220826
 *
 *******************************************************************************/
#include "i2c_driver.h"
// �⺯���ӿ�
void i2c_DriverInit(I2C_TypeDef *I2Cx, DMA_RegTypeDef *DMA_CHx, u32 BaudRate, u8 ADDR); // I2C�������ʼ��
void i2c_DriverMasterRx(u8 addr, u8 *data, u8 len);                                     // ��ģʽ����
void i2c_DriverMasterTx(u8 addr, u8 *data, u8 len);                                     // ��ģʽ����
void i2c_DriverMasterSetRxCallback(void (*fun)(u8));                                    // ��ģʽ������ɻص�����
void i2c_DriverMasterSetTxCallback(void (*fun)(u8));                                    // ��ģʽ������ɻص�����
void i2c_DriverMasterWait(void);                                                        // ��ģʽ�ȴ��������
void i2c_DriverSlaveSetRxCallback(void (*fun)(void));                                   // ��ģʽ������ɻص�����
void i2c_DriverSlaveSetTxCallback(void (*fun)(void));                                   // ��ģʽ������ɻص�����
void i2c_DriverSlaveSetTxData(u8 *data, u8 len);                                        // ��ģʽ���ô����͵�����
void i2c_DriverSlaveGetRxData(u8 *data, u8 len);                                        // ��ģʽ��ȡ�ս��յ�����

// I2C���������
static void i2c_DriverVoidFunctionU8(u8 state); // �ջص�����
static void i2c_DriverVoidFunctionVoid(void);   // �ջص�����

static void (*i2c_DriverMasterRxData)(u8 state) = i2c_DriverVoidFunctionU8; // ��ģʽ������ɻص�����ָ��
static void (*i2c_DriverMasterTxData)(u8 state) = i2c_DriverVoidFunctionU8; // ��ģʽ������ɻص�����ָ��
static void (*i2c_DriverSlaveRxData)(void) = i2c_DriverVoidFunctionVoid;    // ��ģʽ������ɻص�����ָ��
static void (*i2c_DriverSlaveTxData)(void) = i2c_DriverVoidFunctionVoid;    // ��ģʽ������ɻص�����ָ��
static I2C_TypeDef *i2c_Driver_I2Cx;                                        // I2C�Ĵ���ָ��
static DMA_RegTypeDef *i2c_Driver_DMA_CHx;                                  // DMAͨ���Ĵ���ָ��
static u8 i2c_Driver_Slave_Rx_Data[64] = {0};                               // ��ģʽ�������ݻ���
static u8 i2c_Driver_Slave_Tx_Data[64] = {0};                               // ��ģʽ�������ݻ���
static u8 i2c_Driver_Slave_Addr;                                            // ��ģʽ������ַ
static u8 i2c_Driver_Slave_Flg = 0;                                         // 1 ����  2 ����
static u8 i2c_Driver_Master_Flg = 0;                                        // 1 ����  2 ����
static volatile u8 i2c_Driver_Run_Flg = 1;                                  // 1 æµ  2 ����
static volatile u8 i2c_DriverNeedSize;                                      // i2c����Ҫ���ͻ���ܵ����ݣ����ڴ������ݳ���32λ�����
static volatile u8 i2c_DriverContinuousModeFlg;                             // 1 ����ģʽ  0 ��ͨģʽ

/*
    I2C������   ��ʼ��
    �������
                I2Cx        ָ��I2Cģ���ָ��
                DMA_CHx     ָ��DMAĳһ��ͨ����ָ��
                BaudRate    ��Ϊ�������͵Ĳ�����
                ADDR        ��Ϊ�ӻ��ĵ�ַ��7λ,0-127
*/
void i2c_DriverInit(I2C_TypeDef *I2Cx, DMA_RegTypeDef *DMA_CHx, u32 BaudRate, u8 ADDR)
{
    ADDR = ADDR & 0x7f;
    i2c_Driver_I2Cx = I2Cx;
    i2c_Driver_DMA_CHx = DMA_CHx;
    i2c_Driver_Slave_Addr = (u8)(ADDR << 1);
    SYS_ModuleClockCmd(SYS_Module_I2C0, ENABLE);
    SYS_SoftResetModule(SYS_Module_I2C0);

    SYS_WR_PROTECT = 0x7A83; // ���ϵͳ�Ĵ���д����
    SYS_CLK_DIV0 = ((u32)48000000 + ((BaudRate * 17) >> 1)) / (BaudRate * 17) - 1;
    SYS_WR_PROTECT = 0; // ���ϵͳ�Ĵ���д����

    i2c_Driver_I2Cx->ADDR = ADDR | BIT8;
    i2c_Driver_I2Cx->CFG = 0xf3;
    i2c_Driver_I2Cx->BCR = BIT5 | BIT7;
    DMA0_CTRL = 0x01; // DMAģ��ʹ��
    i2c_Driver_DMA_CHx->REN = BIT14;
    i2c_Driver_DMA_CHx->CTMS = 32;
    i2c_Driver_Run_Flg = 0;
}
/*
    I2C������   ��ģʽ��������
    �������
                addr     �ӻ��ĵ�ַ
                data     �����͵����������ڵĵ�ַ
                len      �������ݵĳ���
*/
void i2c_DriverMasterTx(u8 addr, u8 *data, u8 len)
{
    if (i2c_Driver_Run_Flg == 1)
    {
    }
    else
    {
        i2c_Driver_Run_Flg = 1;
        while (i2c_Driver_I2Cx->MSCR & BIT3)
            ; // �ȴ�I2C����

        i2c_Driver_I2Cx->SCR = 0;
        len = (len) ? len : 1;                                                  // �������ƣ�1-32
        len = (len > 32) ? 32 : len;                                            // �������ƣ�1-32
        i2c_Driver_I2Cx->ADDR = (i2c_Driver_I2Cx->ADDR & 0x80) | (addr & 0x7f); // ���豸��ַ
        i2c_Driver_I2Cx->SCR = BIT2;                                            // I2C���䷽��
        i2c_Driver_DMA_CHx->CCR = 0;                                            // I2C�������ݳ���
        i2c_Driver_DMA_CHx->CTMS = len;                                         // DMA���˵����ݳ��ȣ�ʵ�ʴ������ݳ���ΪI2C->BSIZE+1
        i2c_Driver_DMA_CHx->SADR = (u32)data;                                   // DMAԴ��ַ
        i2c_Driver_DMA_CHx->DADR = (u32) & (i2c_Driver_I2Cx->DATA);             // DMAĿ�ĵ�ַ
        i2c_Driver_DMA_CHx->REN = BIT15;
        i2c_Driver_DMA_CHx->CCR = BIT6 | BIT1 | BIT0;                           // ����DMA���䷽��ʹ��DMA
        do
        {
            while (i2c_Driver_I2Cx->MSCR & BIT3) // �ȴ�I2C����
            {
            }
            i2c_Driver_I2Cx->MSCR = 1;                  // ����I2C��������
            while ((i2c_Driver_I2Cx->MSCR & BIT3) == 0) // �ȴ�I2C��start�ź�
            {
            }
        } while ((i2c_Driver_I2Cx->MSCR & BIT2) == 0); // ���û���������ߣ���������������
        i2c_Driver_Master_Flg = 1;
        i2c_Driver_Run_Flg = 0;
    }
}
/*
    I2C������   ��ģʽ��������
    �������
                addr     �ӻ��ĵ�ַ
                data     �����յ����������ڵĵ�ַ
                len      �������ݵĳ���
*/
void i2c_DriverMasterRx(u8 addr, u8 *data, u8 len)
{
    if (i2c_Driver_Run_Flg == 1)
    {
    }
    else
    {
        i2c_Driver_Run_Flg = 1;
        if (i2c_DriverContinuousModeFlg == 1)
        {
        }
        else
        {
            while (i2c_Driver_I2Cx->MSCR & BIT3)
                ; // �ȴ�I2C����
        }
        i2c_Driver_I2Cx->SCR = 0;
        len = (len) ? len : 1; // �������ƣ�1-32
        if (len > 32)
        {
            i2c_DriverNeedSize = len - 32;
            len = 32; // �������ƣ�1-32
        }
        else
        {
            i2c_DriverNeedSize = 0;
            len = len; // �������ƣ�1-32
        }
        i2c_Driver_I2Cx->ADDR = (i2c_Driver_I2Cx->ADDR & 0x80) | (addr & 0x7f); // ���豸��ַ
        i2c_Driver_I2Cx->SCR = BIT4;                                            // I2C���䷽��
        i2c_Driver_DMA_CHx->CCR = 0;                                            // I2C�������ݳ���
        i2c_Driver_DMA_CHx->CTMS = len;                                         // DMA���˵����ݳ��ȣ�ʵ�ʴ������ݳ���ΪI2C->BSIZE+1
        i2c_Driver_DMA_CHx->SADR = (u32) & (i2c_Driver_I2Cx->DATA);             // DMAԴ��ַ
        i2c_Driver_DMA_CHx->DADR = (u32)data;                                   // DMAĿ�ĵ�ַ
        i2c_Driver_DMA_CHx->REN = BIT14;
        i2c_Driver_DMA_CHx->CCR = BIT4 | BIT1 | BIT0;                           // ����DMA���䷽��ʹ��DMA
        if (i2c_DriverContinuousModeFlg == 1)
        {
        }
        else
        {
            do
            {
                while (i2c_Driver_I2Cx->MSCR & BIT3) // �ȴ�I2C����
                {
                }
                i2c_Driver_I2Cx->MSCR = BIT0;               // ����I2C��������
                while ((i2c_Driver_I2Cx->MSCR & BIT3) == 0) // �ȴ�I2C��start�ź�
                {
                }
            } while ((i2c_Driver_I2Cx->MSCR & BIT2) == 0); // ���û���������ߣ���������������
        }
        i2c_Driver_Master_Flg = 2;
        i2c_Driver_Run_Flg = 0;
    }
}
/*
    I2C������ ��ģʽ�ȴ��������
*/
void i2c_DriverMasterWait(void)
{
    while (i2c_Driver_I2Cx->MSCR & BIT3)
        ; // �ȴ�I2C����
}
/*
    I2C������   ���ô�ģʽ���ͳ����ݺ�Ļص�����
    �������
                fun     ������ɺ���Ҫִ�еĻص�������Ĭ�Ͽպ��������Բ�����
*/
void i2c_DriverSlaveSetRxCallback(void (*fun)(void))
{
    i2c_DriverSlaveRxData = fun;
}
/*
    I2C������   ���ô�ģʽ���������ݺ�Ļص�����
    �������
                fun     ������ɺ���Ҫִ�еĻص�������Ĭ�Ͽպ��������Բ�����
*/
void i2c_DriverSlaveSetTxCallback(void (*fun)(void))
{
    i2c_DriverSlaveTxData = fun;
}
/*
    I2C������   ���ô�ģʽ�����͵�����
    �������
                data    ԭ���ݵ�ַ
                len     ԭ���ݳ���
*/
void i2c_DriverSlaveSetTxData(u8 *data, u8 len)
{
    u8 i;
    for (i = 0; i < len; i++)
    {
        i2c_Driver_Slave_Tx_Data[i] = data[i];
    }
}
/*
    I2C������   ��ȡ��ģʽ���յ�������
    �������
                data    ԭ���ݵ�ַ
                len     ԭ���ݳ���
*/
void i2c_DriverSlaveGetRxData(u8 *data, u8 len)
{
    u8 i;
    for (i = 0; i < len; i++)
    {
        data[i] = i2c_Driver_Slave_Rx_Data[i];
    }
}

/*
    I2C������   ������ģʽ������ɻص�����
    �������
                fun    ������ɺ���Ҫִ�еĻص�������Ĭ�Ͽպ��������Բ�����
*/
void i2c_DriverMasterSetRxCallback(void (*fun)(u8))
{
    i2c_DriverMasterRxData = fun;
}
/*
    I2C������   ������ģʽ������ɻص�����
    �������
                fun    ������ɺ���Ҫִ�еĻص�������Ĭ�Ͽպ��������Բ�����
*/
void i2c_DriverMasterSetTxCallback(void (*fun)(u8))
{
    i2c_DriverMasterTxData = fun;
}

static void i2c_DriverSlaveTx(void)
{
    i2c_Driver_DMA_CHx->CCR = 0;    // I2C�������ݳ���
    i2c_Driver_DMA_CHx->CTMS = 255; // DMA���˵����ݳ��ȣ�ʵ�ʴ������ݳ���ΪI2C->BSIZE+1
    if (i2c_DriverContinuousModeFlg)
    {
        i2c_Driver_DMA_CHx->SADR = (u32)i2c_Driver_Slave_Tx_Data + 32; // DMAԴ��ַ
    }
    else
    {
        i2c_Driver_DMA_CHx->SADR = (u32)i2c_Driver_Slave_Tx_Data; // DMAԴ��ַ
    }
    i2c_Driver_DMA_CHx->DADR = (u32) & (i2c_Driver_I2Cx->DATA); // DMAĿ�ĵ�ַ
    i2c_Driver_DMA_CHx->REN = BIT15;
    i2c_Driver_DMA_CHx->CCR = BIT6 | BIT1 | BIT0;               // ����DMA���䷽��ʹ��DMA
    i2c_Driver_DMA_CHx->REN |= BIT15;
}
static void i2c_DriverSlaveRx(void)
{
    i2c_Driver_DMA_CHx->CCR = 0;                                // �ر�DMA
    i2c_Driver_DMA_CHx->CTMS = 255;                             // DMA���˵����ݳ��ȣ�ʵ�ʴ������ݳ���ΪI2C->BSIZE+1
    i2c_Driver_DMA_CHx->SADR = (u32) & (i2c_Driver_I2Cx->DATA); // DMAԴ��ַ
    i2c_Driver_DMA_CHx->DADR = (u32)i2c_Driver_Slave_Rx_Data;   // DMAĿ�ĵ�ַ
    i2c_Driver_DMA_CHx->REN = BIT14;
    i2c_Driver_DMA_CHx->CCR = BIT4 | BIT1 | BIT0;               // ����DMA���䷽��ʹ��DMA
}
u32 callbackstate = 0;
void I2C0_IRQHandler(void);
u8 flg;
void I2C0_IRQHandler(void)
{
    u8 i2c_stop_flg = 0;
    u32 scr = 0;

    if (flg == 1)
    {
        flg = 0;
        GPIO_SetBits(GPIO0, GPIO_Pin_5);
    }
    else
    {
        flg = 1;
        GPIO_ResetBits(GPIO0, GPIO_Pin_5);
    }
    if (i2c_Driver_I2Cx->SCR & BIT1)
    {
        if (i2c_Driver_I2Cx->SCR & BIT3)
        {
            if (i2c_Driver_Master_Flg == 1) // ��ģʽ����
            {
                callbackstate = 1;
            }
            else if (i2c_Driver_Master_Flg == 2) // ��ģʽ����
            {
                callbackstate = 2;
            }
            i2c_Driver_Master_Flg = 0;
        }
    }
    if (i2c_Driver_I2Cx->SCR & BIT0)
    {

        if (i2c_Driver_I2Cx->MSCR & BIT2)
        {
            if (i2c_Driver_Master_Flg == 1)
            {
                callbackstate = 3;
            }
            else if (i2c_Driver_Master_Flg == 2)
            {
                if (i2c_DriverNeedSize != 0)
                {
                    i2c_DriverContinuousModeFlg = 1;
                    i2c_DriverMasterRx(0, (u8 *)i2c_Driver_DMA_CHx->DADR, i2c_DriverNeedSize);
                    i2c_DriverContinuousModeFlg = 0;
                    return;
                }
                callbackstate = 4;
            }
            i2c_Driver_Master_Flg = 0;
        }
        else
        {

            if ((i2c_Driver_I2Cx->SCR & (BIT5 | BIT3)) == 0)
            {
                i2c_DriverContinuousModeFlg = 1;
                i2c_DriverSlaveTx();
                i2c_DriverContinuousModeFlg = 0;
                scr = BIT4 | BIT2; // I2C���䷽��
            }
        }
    }
    if ((i2c_Driver_I2Cx->MSCR & BIT2) == 0)
    {
        if (i2c_Driver_I2Cx->SCR & BIT3)
        {
            if ((i2c_Driver_I2Cx->DATA & 0xfe) == i2c_Driver_Slave_Addr)
            {
                if (i2c_Driver_I2Cx->DATA & BIT0)
                {
                    i2c_DriverSlaveTx();
                    scr = BIT4 | BIT2; // I2C���䷽��
                    i2c_Driver_Slave_Flg = 1;
                }
                else
                {
                    i2c_DriverSlaveRx();
                    scr = BIT4; // I2C���䷽��
                    i2c_Driver_Slave_Flg = 2;
                }
            }
        }
    }
    if (i2c_Driver_I2Cx->SCR & BIT5)
    {
        i2c_stop_flg = 1;
        if (i2c_Driver_Slave_Flg == 1)
        {
            i2c_Driver_Slave_Flg = 0;
            callbackstate = 5;
        }
        else if (i2c_Driver_Slave_Flg == 2)
        {
            i2c_Driver_Slave_Flg = 0;
            callbackstate = 6;
        }
    }
    i2c_Driver_I2Cx->SCR = scr;
    if (i2c_stop_flg == 1)
    {
        switch (callbackstate)
        {
        case 0:
            break;
        case 1:
            callbackstate = 0;
            i2c_DriverMasterTxData(0);
            break;
        case 2:
            callbackstate = 0;
            i2c_DriverMasterRxData(0);
            break;
        case 3:
            callbackstate = 0;
            i2c_DriverMasterTxData(1);
            break;
        case 4:
            callbackstate = 0;
            i2c_DriverMasterRxData(1);
            break;
        case 5:
            callbackstate = 0;
            i2c_DriverSlaveTxData();
            break;
        case 6:
            callbackstate = 0;
            i2c_DriverSlaveRxData();
            break;
        default:
            callbackstate = 0;
            break;
        }
    }
}
static void i2c_DriverVoidFunctionU8(u8 state)
{
    if (state)
    {
    }
}
static void i2c_DriverVoidFunctionVoid(void)
{
}
