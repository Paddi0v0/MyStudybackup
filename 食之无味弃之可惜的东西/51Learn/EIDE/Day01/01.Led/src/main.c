#include <intrins.h>
#include <STC89C5xRC.H>
// ��ʱ������������λ1ms��������11.0592MHz����
void Delay1ms(unsigned int ms) {
  unsigned int i, j;
  for (i = ms; i > 0; i--)
    for (j = 120; j > 0; j--)
      ; // ��ʱ1ms�Ļ�����λ��11.0592MHz�� 
}

void main() {
  while (1) {
    P20 = 0;       // LED��
    Delay1ms(500); // ��ʱ500msP20 = 1;       // LED��
    Delay1ms(500); // ��ʱ500ms for (i = 0; i < 10; i++) {   P20 = 0;       //
                   // LED�� Delay1ms(500); // ��ʱ500ms
    P20 = 1;       // LED��
  }
}

