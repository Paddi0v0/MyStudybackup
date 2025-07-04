#include "mygpio_init.h"
#include "lks32mc08x_lib.h"

void GPIO_init(void)
{
    /*
����:
����LED1 --> P0.6����,LED2 --> P0.7����,LED3 --> P0.3����.
LED1��LED2ԭ��ͼ���Ǹߵ�ƽ����,LED3ԭ��ͼ�ǵ͵�ƽ����.
�ڵ�·�����LED1��LED2Ϊ�ߵ�ƽ�ſ��Ե���,���Բ�������Ϊ��©���,��Ϊ��©���Ĭ������ǵ͵�ƽ.
��LED3�ĵ�·����ǵ͵�ƽ����,���Լȿ������Ϊ��©ģʽ,Ҳ��������Ϊ����ģʽ,��Ϊ����ģʽ�ȿ�������ߵ�ƽ,Ҳ��������͵�ƽ
����LED1��LED2����Ϊ�������,LED3����Ϊ��©���.
*/
    GPIO_InitTypeDef GPIO_InitStruct;

    // ����LED1(P0.6)��LED2(P0.7) - �������
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��

    // ����LED1��LED2���ߵ�ƽ������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // ͬʱ����P0.6��P0.7
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;          // ���ģʽ
    // GPIO_PuPd���������ã���������ѡ��: ������ GPIO_PuPd_NOPULL, ����
    // GPIO_PuPd_UP��LKS32 ϵ�в�֧���ڲ������� �����ģʽ������/��©���£�PUE
    // λ����Ӱ��ʵ�������ƽ����ƽ�� PDO �Ĵ���������
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_PODEna = 0;    // 0�ǽ��ÿ�©������������������,1�����ÿ�©���
    GPIO_Init(GPIO0, &GPIO_InitStruct); // Ӧ������

    // ��������LED3(P0.3) - ��©���
    GPIO_StructInit(&GPIO_InitStruct);            // ���³�ʼ���ṹ��
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;        // ֻ����P0.3
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;    // ���ģʽ
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ������
    GPIO_InitStruct.GPIO_PODEna = 1;              // ��©���
    GPIO_Init(GPIO0, &GPIO_InitStruct);           // Ӧ������
}
void Led_12on(void)
{
    GPIO_SetBits(GPIO0, GPIO_Pin_6 | GPIO_Pin_7);
}
void Led_12off(void)
{
    GPIO_ResetBits(GPIO0, GPIO_Pin_6 | GPIO_Pin_7);
}
void Led_3on(void)
{
    GPIO_ResetBits(GPIO0, GPIO_Pin_3);
}
void Led_3off(void)
{
    GPIO_SetBits(GPIO0, GPIO_Pin_3);
}
