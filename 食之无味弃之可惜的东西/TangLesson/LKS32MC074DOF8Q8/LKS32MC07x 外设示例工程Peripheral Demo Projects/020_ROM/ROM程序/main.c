#include <lks32mc07x.h>
void ROM_ENABLE(void);
s16 ( lks32_sin)(u16 hAngle);
    
s16 ( lks32_cos)(u16 hAngle);
volatile u16 i123;
volatile u16 j;
volatile u16 k;
volatile u16 l;
typedef  s16 (*sin_fun)(u16 angle);
volatile sin_fun lks32_sin1;
volatile sin_fun lks32_cos1;
volatile int user_key;

// ˵�� ��
// �����rom�ĳ����ʱ����Ҫ��֤�����������������Ҫ�õ��ĳ�������һ�ε�
// �ã��Ա�֤��������ѳ���ŵ�rom���棬ͬʱ��Ҫ��֤rom��û�ж�����Ӧ�ó��������
// ���û�������rom�ڵĳ����ʱ�򣬿���ʹ�ú���е��ã�Ҳ����ʹ�ú���ָ��
typedef  void (*void_fun)(void);
#define rom_RESET ((void_fun)(u32 *)(0+1))
volatile int32_t initflg;
int main()
{
    j=lks32_sin(i123++);
    k=lks32_cos(i123++);
    initflg = 0x12345678;
    NVIC_SystemReset();
    for(;;);
}
void SystemInit()
{
}
