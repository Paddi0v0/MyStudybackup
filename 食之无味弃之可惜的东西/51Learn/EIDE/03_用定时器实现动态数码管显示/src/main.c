#include <STC89C5xRC.H>

// �ö�ʱ��ʵ�ֶ�̬�������ʾ
typedef unsigned char u8;
u8 code nums[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F}; // ����������ܶ����
u8 code LEDs[] = {0Xe3, 0Xe7, 0Xeb, 0Xef, 0Xf3, 0Xf7, 0Xfb, 0Xff};

void Timer0_Init()
{
    TMOD &= 0XF0; // �����ʱ��0��ģʽλ
    TMOD |= 0X01; // ���ö�ʱ��Ϊģʽ1(16λ��ʱ��)
}

void Delay_10ms(unsigned int count)
{
    static const u8 TH0_VAL = 0XDC;
    static const u8 TL0_VAL = 0X00;
    unsigned int i;
    TR0 = 0; // �رն�ʱ��
    TF0 = 0; // �����ʱ����־λ
    TH0 = TH0_VAL;
    TL0 = TL0_VAL;
    TR0 = 1; // ������ʱ��
    for (i = 0; i < count; i++)
    {
        while (!TF0)
            ;
        TH0 = TH0_VAL;
        TL0 = TL0_VAL;
        TF0 = 0;
    }
    TR0 = 0; // �رն�ʱ������ռ����Դ
}
/*
#define LEDS_LEN 8
#define NUMS_LEN 10

void main()
{
    u8 i, num_index = 0;
    Timer0_Init();
    while (1)
    {
        // ɨ��8������ܣ�ÿ������ʾ��ǰnum_index��Ӧ������
        for (i = 0; i < LEDS_LEN; i++)
        {
            P0 = 0x00;            // ��Ӱ
            P2 &= 0xe3;           // ����P2.2~P2.4
            P2 |= LEDs[i];        // ѡ�е�iλ
            P0 = nums[num_index]; // ��ʾ��ǰ����
            Delay_10ms(1);        // ɨ���ٶ�
        }
        // �л�����һ������
        num_index++;
        if (num_index >= NUMS_LEN) num_index = 0;
        Delay_10ms(1); // ���������л��ٶ�
    }
}
*/

void main()
{
    u8 i;
    Timer0_Init();
    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            P0 = 0x00;     // �رն�ѡ(��Ӱ) P0 &= 0x00; ʵ��Ч���ȼ��� P0 = 0x00;  ����P0 &= 0x00����дû����
            P2 &= 0xe3;    // �ر�λѡ  ��Ȼ����ֻ����P22,P23,P24ΪΪ0��,����P2 = LEDs[0]ֱ�Ӹ�ֵ��,���������P2Ҫ�ĳ�|=
            P2 |= LEDs[i]; // ������ǰλѡ
            P0 = nums[i];  // ���ö�ѡ
            Delay_10ms(50);
        }
    }
}
