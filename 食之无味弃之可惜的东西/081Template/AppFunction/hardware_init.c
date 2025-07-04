#include "hardware_init.h"
#include "lks32mc08x_lib.h"
#include "mygpio_init.h"
#include "mytimer_init.h"

void Hardware_init(void)
{
    // Ĭ��д��
    __disable_irq();         /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83; /*ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000; /* enable prefetch ��FLASHԤȡ����ʹ��*/

    GPIO_init(); /* GPIO��ʼ�� */
    

    SoftDelay(100);       /* �ȴ�Ӳ����ʼ�����*/
    SYS_WR_PROTECT = 0x0; /*�ر�ϵͳ�Ĵ���д����*/
#if POWER_MODE
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0); /*MCU��Դ�ж�ʹ�ܺ���*/
#else
    SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3); /*MCU��Դ�ж�ʹ�ܺ���*/
#endif
    __enable_irq();
}

void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83; /* ���д��������������PLL */
    SYS_AFE_REG5 |= BIT15;   /* BIT15:PLLPDN */
#if POWER_MODE
    SYS_VolSelModule(MCU_POWER_5v0);
#else
    SYS_VolSelModule(MCU_POWER_3v3);
#endif
    SoftDelay(100); /* �ȴ�PLL�ȶ����˲��������Զ�ʹ��д������ */

    /* ��Ҫ�ٴν��д����������ʱ�ӼĴ��� */
    SYS_WR_PROTECT = 0x7a83;  /* ���д��������������ʱ�� */
    SYS_CLK_CFG = 0x000001ff; /* ѡ��PLL��Ϊʱ��Դ����Ƶϵ��0xFF=96MHz */
    SYS_WR_PROTECT = 0x0;     /* ����ʹ��д���� */
}

void SystemInit(void)
{
    Clock_Init(); /* ʱ�ӳ�ʼ�� */
}

void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt; // volatile ��ֹ�������Ż�

    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP(); // �ղ���ָ�����1��ʱ�����ڣ�
    }
}

/*
*/
