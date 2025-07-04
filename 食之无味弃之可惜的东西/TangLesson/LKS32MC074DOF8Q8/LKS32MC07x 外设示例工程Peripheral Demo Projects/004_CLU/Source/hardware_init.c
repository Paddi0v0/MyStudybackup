/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_init.c
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ����ʼ������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2015��11��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
#include "LKS32MC07x_lib.h" // Device header

void CLU_init(void);
void GPIO_init(void);
void SoftDelay(u32 cnt);
/*******************************************************************************
 �������ƣ�    void Hardware_init(void)
 ����������    Ӳ�����ֳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq(); /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;
    FLASH_CFG |= 0x00080000; /* enable prefetch */
    IWDG_DISABLE();          /* �رտ��Ź�*/

    CLU_init();
    GPIO_init();
    SoftDelay(100);       /* �ȴ�Ӳ����ʼ�����*/
    SYS_WR_PROTECT = 0x0; /* �ر�ϵͳ�Ĵ���д����*/
    __enable_irq();       /* �������ж� */
}
/*******************************************************************************
 �������ƣ�    void Clock_Init(void)
 ����������    ʱ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/4/17      V1.1           Howlet Li          ������ʱ�������ȴ�PLL�ȶ�
 *******************************************************************************/
void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;  /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 = BIT15;     /* BIT15:PLLPDN ��PLL */
    SoftDelay(100);           /* ��ʱ100us, �ȴ�PLL�ȶ� 21.4.17*/
    SYS_CLK_CFG = 0x000001ff; /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff��Ӧ96Mʱ�� */
    SYS_WR_PROTECT = 0;       /* �ر�ϵͳ�Ĵ���д����*/
}

/*******************************************************************************
 �������ƣ�    void SystemInit(void)
 ����������    Ӳ��ϵͳ��ʼ��������ʱ�ӳ�ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          ����
 *******************************************************************************/
void SystemInit(void)
{
    Clock_Init(); /* ʱ�ӳ�ʼ�� */
}

void CLU_init(void)
{
    CLU_InitTypeDef cluconfig;

    CLU_StructInit(&cluconfig);

    cluconfig.EN = ENABLE;                        ///< ģ��ʹ�� 0�ر�ģ�� 1��ģ��
    cluconfig.FIE = DISABLE;                      ///< �½����ж�ʹ��
    cluconfig.RIE = DISABLE;                      ///< �������ж�ʹ��
    cluconfig.MXA = CLU_MXA_CLU0_P1_0;            ///< A���븴��ѡ�� ��ʽ CLU_MXA_CLUa_b aȡֵ��ΧΪ[0 1 2 3] ��Ӧ��ͬ��CLU b��ʾѡ��ͬ�������ź�
    cluconfig.MXB = CLU_MXB_CLU0_P0_15;           ///< B���븴��ѡ�� ��ʽ CLU_MXB_CLUa_b aȡֵ��ΧΪ[0 1 2 3] ��Ӧ��ͬ��CLU b��ʾѡ��ͬ�������ź�
    cluconfig.FN = CLU_FN_NOTA_B | CLU_FN_A_NOTB; ///< ��ֵ�� ��ʽ CLU_FN_x xȡֵ��ΧΪ[A NOTA A_NOTB ...]��ӦΪ������ʽ�µ���С�� ���� y=a+bc+!c ��ʾΪ CLU_FN_A|CLU_FN_B_C|CLU_FN_NOTC
    cluconfig.OUTSEL = 1;                         ///< ���ѡ�� 0: D ��������� 1: LUT ���
    cluconfig.OEN = 1;                            ///< ���ʹ�� 0: �ر� 1: ʹ��
    cluconfig.CLKINV = 0;                         ///< D ������ʱ�ӵ�ƽ 0: ͬ�� 1: ����
    cluconfig.CLKSEL = 0;                         ///< D ������ʱ��ѡ�� 0x0: ��λ���� 0x1: MXA ���� 0x2: ALTCLK0 0x3: ALTCLK1
    CLU_Init(CLU, CLU_MODEL_CLU0, &cluconfig);
}
void GPIO_init(void)
{
    // CLO���
    GPIO_InitTypeDef gpioconfig;
    gpioconfig.GPIO_Pin = GPIO_Pin_14;    // Ҫ���õ�����
    gpioconfig.GPIO_Mode = GPIO_Mode_OUT; // GPIOģʽ
    gpioconfig.GPIO_PuPd = DISABLE;       // ����ʹ��
    gpioconfig.GPIO_PODEna = DISABLE;     // ��©ʹ��
    GPIO_Init(GPIO0, &gpioconfig);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_14, 12); // ����ΪCL

    // CLO�����������źţ�������gpio���һ����������źŽ��в���
    gpioconfig.GPIO_Pin = GPIO_Pin_0;    // Ҫ���õ�����
    gpioconfig.GPIO_Mode = GPIO_Mode_IN; // GPIOģʽ
    gpioconfig.GPIO_PuPd = DISABLE;      // ����ʹ��
    gpioconfig.GPIO_PODEna = DISABLE;    // ��©ʹ��
    GPIO_Init(GPIO1, &gpioconfig);

    gpioconfig.GPIO_Pin = GPIO_Pin_15;   // Ҫ���õ�����
    gpioconfig.GPIO_Mode = GPIO_Mode_IN; // GPIOģʽ
    gpioconfig.GPIO_PuPd = DISABLE;      // ����ʹ��
    gpioconfig.GPIO_PODEna = DISABLE;    // ��©ʹ��
    GPIO_Init(GPIO0, &gpioconfig);

    GPIO1->POE |= BIT0;
    GPIO0->POE |= BIT15;
}

/*******************************************************************************
 �������ƣ�    void SoftDelay(void)
 ����������    �����ʱ����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;

    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
    }
}
