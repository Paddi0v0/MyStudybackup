#include <STC89C5xRC.H>
#include <intrins.h>


/*
void Delay1ms(unsigned int n) {
    while(n--) {
        unsigned char i = 3, j = 215;  // 
        do {
            while(--j);
        } while(--i);
    }
}
*/
bit flag_1ms = 0;

void Timer0_Init() {
    TMOD |= 0x02;     // ��ʱ��0��ģʽ2��8λ�Զ���װ��
    TH0 = 0xFD;       // ��ֵ��11.0592MHz �� 1ms��
    TL0 = 0xFD;
    ET0 = 1;          // ���ж�
    EA = 1;
    TR0 = 1;          // ������ʱ��
}

void Delay1ms(unsigned int n) {
    unsigned int i;
    for(i = 0; i < n; i++) {
        flag_1ms = 0;
        while(!flag_1ms);  // �ȴ�1ms
    }
}

// �жϷ������
void Timer0_ISR(void) interrupt 1 {
    flag_1ms = 1;
}

void main()
{
    while (1)
    {
        P20 = ~P20;
        Delay1ms(500);
        // Code
    }
}

