
#include "lks32mc07x_lib.h" // Device header
#include "delay.h"
#define I2C I2C0
static uint8_t addr = 0x24;                                              // ��ַ����Ϊ0x24ʱ �����������͵����� ��ַΪ0x25ʱ ��������������
static uint8_t state = 0;                                                // 0 �޶��� 1���� 2���� 3 �趨��ǰ��ַ
static __attribute__((at(0x7c00))) volatile const uint8_t romdata[1024]; // �̶�romdata[511]Ϊ0 ��������rom�������Ƿ�����
static volatile uint8_t romdatabuffer[512];                              // ������ �յ������ݻ��ȴ浽����
static uint16_t romdatap = 0;                                            // ������ָ�� 0-511ѭ��
static uint16_t RxCnt = 0;                                               // ���յ������ݼ���
void eraseSector(uint32_t addr)
{
    SYS_WR_PROTECT = 0x7a83; /* ����дʹ��*/
    SYS_FLSE = 0x8fca;
    FLASH_CFG |= BIT31;
    FLASH_ADDR = addr;
    FLASH_ERASE = 0x7654dcba;
    FLASH_CFG &= ~BIT31;
    SYS_FLSE = 0;
}
void programSector(uint32_t* flashadd2,uint32_t* startaddr )
{
    int16_t i;
    FLASH_CFG |= BIT27;
    SYS_WR_PROTECT = 0x7a83; /* ����дʹ��*/
    SYS_FLSP = 0x8F35;
    for(i=0;i<128;i++)
    {
        if(*startaddr != *flashadd2) // ֻ�����ݲ�һ����ʱ�����Ҫ����д����
        {
            FLASH_ADDR = (uint32_t)flashadd2;
            FLASH_WDATA = *startaddr;
        }
        startaddr ++;
        flashadd2 ++;
    }
    SYS_FLSP = 0;
    FLASH_CFG &= ~BIT27;
}
/*******************************************************************************
 �������ƣ�    void RomDataInit(void)
 ����������    ���ݳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/04/17      V1.0         yangzj            ����
 *******************************************************************************/
void RomDataInit(void) // �ϵ��ʼ��buffer
{
        __disable_irq();
    if (romdata[511] != 0) // ��������Ƿ����任
    {
        if (romdata[1023] != 0) // �����е���������Ϊ0
        {
            for (int16_t i = 0; i < 512; i++)
            {
                romdatabuffer[i] = 0xff;
            }
            romdatabuffer[511] = 0;
        }
        else
        {
            eraseSector((uint32_t)romdata);
            programSector((uint32_t*)romdata,(uint32_t*)(romdata+512));
        }
    }
    for (int16_t i = 0; i < 512; i++)
    {
        romdatabuffer[i] = romdata[i];
    }
        __enable_irq();
}
/*******************************************************************************
 �������ƣ�    void RomDataSave(void)
 ����������    д���ݵ�flash��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2024/04/17      V1.0         yangzj            ����
 *******************************************************************************/
void RomDataSave(void)
{
    uint16_t flg = 0;
    uint32_t *p = (uint32_t *)((u32)romdatabuffer);
    uint32_t *p1 = (uint32_t *)(romdata);
    for (int16_t i = 0; i < 128; i++)
    {
        if (p[i] != p1[i])
        {
            flg = 1;
        }
    }
    if (flg == 1) // ��������Ƿ���Ҫ����
    {
        // д���� ��д�뵽�ڶ���������Ȼ����д�뵽��һ������
        // �����д��Ĺ����з����ϵ��λ�������������һ��������������������
        
        // ��������ʱ����Ҫ��ʱ�ص�i2c
        I2C->CFG &= ~BIT0;
        __disable_irq();
        romdatabuffer[511] = 0;
        eraseSector((uint32_t)romdata + 512);
        programSector((uint32_t*)(romdata+512),(uint32_t*)romdatabuffer);
        eraseSector((uint32_t)romdata);
        programSector((uint32_t*)(romdata),(uint32_t*)romdatabuffer);
        I2C->CFG |= BIT0;
        __enable_irq();
    }
}
/*******************************************************************************
 �������ƣ�    void I2C_IRQHandler(void)
 ����������    I2C�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/11/15      V1.0        yangzj        ����
 *******************************************************************************/
void I2C0_IRQHandler(void)
{
    if (I2C->SCR & BIT3) // �յ���ַ
    {
        uint8_t data;
        data = I2C->DATA;
        if ((data & 0xfe) == addr)
        {
            if (data & 1) // ��������������
            {
                state = 2;
                I2C->DATA = romdatabuffer[romdatap]; // ����������ʱ��Ҫ��ǰ׼��������
                romdatap = (romdatap + 1) & 511;
                I2C->SCR = BIT2 | BIT4;
            }
            else // ��������������������
            {
                state = 3;
                I2C->SCR = BIT4;
            }
        }
        else
        {
            I2C->SCR = 0;
            state = 0;
        }
    }
    else
    {
        if (I2C->SCR & BIT0)
        {
            if (state == 1) // ��������������������
            {
                RxCnt ++;
                romdatabuffer[romdatap] = I2C->DATA;
                romdatap = (romdatap + 1) & 511;
                I2C->SCR = BIT4;
            }
            else if (state == 2) // ��������������
            {
                I2C->DATA = romdatabuffer[romdatap];
                romdatap = (romdatap + 1) & 511;
                I2C->SCR = BIT2;
            }
            else if (state == 3) // ���������������ĵ�ַ
            {
                romdatap = I2C->DATA;
                romdatap = romdatap*2; // ��ַ��Χ0-255 ӳ�䵽0-512
                I2C->SCR = BIT4;
                state = 1;
                RxCnt = 0;
            }
            else
            {
                I2C->SCR = 0;
            }
        }
        else if (I2C->SCR & BIT5) // ����stop�¼� �������
        {
            I2C->SCR = 0;
            if (state == 1)
            {
                if(RxCnt > 1) // �����յ���һ�������ٴ��������߼�
                {
                    RomDataSave();
                }
            }
            state = 0;
        }
        else
        {
            I2C->SCR = 0;
        }
    }
}
