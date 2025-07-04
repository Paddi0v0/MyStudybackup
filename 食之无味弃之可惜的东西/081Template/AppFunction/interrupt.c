/******************************************************************************
 * �жϺ��������ܽ�����ļ�˳��
 *
 * | �жϺ���               | ��Ҫ����                     | �ؼ����� |
 * |------------------------|------------------------------|--------------------------|
 * | PWRDN_IRQHandler      | ��ԴǷѹ����                 | ���õ�Դ������ |
 * | ADC0_IRQHandler       | ADC0ת����ɴ���             | ���ADC�жϱ�־ | |
 *UART0_IRQHandler      | UART0ͨ�Ŵ����գ�          | �޲��� | |
 *MCPWM_IRQHandler      | �������PWM����              | ���MCPWM�жϱ�־ | |
 *HALL_IRQHandler       | ���������������գ�         | �޲��� | |
 *UTIMER0_IRQHandler    | ��ʱ��0�жϴ���              | �����ʱ���жϱ�־ | |
 *UTIMER1_IRQHandler    | ��ʱ��1�жϴ����գ�        | �޲��� | |
 *UTIMER2_IRQHandler    | ��ʱ��2�жϴ����գ�        | �޲��� | |
 *UTIMER3_IRQHandler    | ��ʱ��3�жϴ����գ�        | �޲��� | |
 *ENCODER0_IRQHandler   | ������0�����գ�            | �޲��� | |
 *ENCODER1_IRQHandler   | ������1�����գ�            | �޲��� | |
 *CMP_IRQHandler        | �Ƚ��������գ�             | �޲��� | |
 *UART1_IRQHandler      | UART1ͨ�Ŵ����գ�          | �޲��� | |
 *SysTick_Handler       | ϵͳ���Ķ�ʱ�������գ�     | �޲��� | |
 *SleepTimer_IRQHandler | ���߻��Ѵ���                 | ��ѭ�������޸ģ� | |
 *GPIO_IRQHandler       | GPIO�жϴ����գ�           | �޲��� | |
 *I2C0_IRQHandler       | I2C0ͨ�Ŵ����գ�           | �޲��� | |
 *SPI0_IRQHandler       | SPI0ͨ�Ŵ����գ�           | �޲��� |
 *
 * ˵����
 * 1. ��"���գ�"�ĺ�����Ҫ����ʵ�ʹ�����Ӵ���
 * 2. SleepTimer�е���ѭ����Ҫ�滻Ϊʵ�ʻ��Ѵ���
 * 3. ���Ӧ��ͨ��ֻ��SysTick��GPIO�ж�
 ******************************************************************************/

#include "hardware_config.h"

// ��ԴǷѹ�жϴ���
void PWRDN_IRQHandler(void) {
  /*
   * �����ԴǷѹ�¼�
   * SYS_VolSelModuleIRQ() �ǵ�Դ����ģ����жϴ�����
   * ���ܣ�
   * 1. ��⵱ǰ��Դ״̬��5V/3.3V��
   * 2. ִ����Ӧ�ĵ�Դ���ϴ���
   * 3. �����Դ�жϱ�־λ
   * 4. ���ܴ���ϵͳ��λ��͹���ģʽ
   */
  // SYS_VolSelModuleIRQ();
}

// ADC0�жϴ���
void ADC0_IRQHandler(void) {
  /*
   * ���ADC0�жϱ�־λ
   * BIT1 | BIT0 ��ӦADC0���жϱ�־λ
   * ����˵����
   * 1. ADC0_IF ��ADC0�жϱ�־�Ĵ���
   * 2. BIT0 = 0x01 ��ʾת������жϱ�־
   * 3. BIT1 = 0x02 ��ʾת�������жϱ�־
   * 4. ͨ��д1�����־λ��д1���㣩
   */
  // ADC0_IF |= BIT1 | BIT0;
}

// UART0�жϴ����գ�
void UART0_IRQHandler(void) {}

// MCPWM�жϴ���
void MCPWM_IRQHandler(void) {
  /*
   * ���MCPWM�жϱ�־λ
   * BIT1 | BIT0 ��ӦMCPWM���жϱ�־λ
   * ����˵����
   * 1. MCPWM_IF �ǵ������PWM�жϱ�־�Ĵ���
   * 2. BIT0 = 0x01 ��ʾPWM���ڽ����ж�
   * 3. BIT1 = 0x02 ��ʾPWM���ϱ����ж�
   * 4. ֱ�Ӹ�ֵ�����־λ����ͬ��ADC��д1���㣩
   */
  // MCPWM_IF = BIT1 | BIT0;
}

// HALL�жϴ����գ�
void HALL_IRQHandler(void) {}

// TIMER0�жϴ���
void UTIMER0_IRQHandler(void) {
  /*
   * �����ʱ��0�жϱ�־λ
   * TIMER_IF_ZERO �Ƕ�ʱ��0���жϱ�־λ
   * ����˵����
   * 1. UTIMER_IF �Ƕ�ʱ���жϱ�־�Ĵ���
   * 2. TIMER_IF_ZERO �Ƕ�ʱ��0�ļ��������־
   * 3. ͨ��д1�����־λ��д1���㣩
   * 4. ʵ��Ӧ���������ִ�ж�ʱ����
   */
  // UTIMER_IF |= TIMER_IF_ZERO;
}

// TIMER1�жϴ����գ�
void UTIMER1_IRQHandler(void) {}

// TIMER2�жϴ����գ�
void UTIMER2_IRQHandler(void) {}

// TIMER3�жϴ����գ�
void UTIMER3_IRQHandler(void) {}

// ENCODER0�жϴ����գ�
void ENCODER0_IRQHandler(void) {}

// ENCODER1�жϴ����գ�
void ENCODER1_IRQHandler(void) {}

// �Ƚ����жϴ����գ�
void CMP_IRQHandler(void) {}

// UART1�жϴ����գ�
void UART1_IRQHandler(void) {}

// ϵͳ�δ�ʱ�жϴ����գ�
void SysTick_Handler(void) {}

// ���߻����жϴ���
void SleepTimer_IRQHandler(void) {
  /*
   * ���߻����жϴ���
   * ��ǰʵ��Ϊ��ѭ���������ʵ�������޸ģ�
   * ��ȷ������
   * 1. ������߶�ʱ���жϱ�־
   * 2. ִ��ϵͳ���ѳ�ʼ��
   * 3. �ָ�ϵͳʱ�Ӻ�����
   * 4. ����������ִ��
   *
   * ���棺��ǰ��ѭ���ᵼ��ϵͳ����
   * ��ʱ���������ڵ��Ի���������
   */
  /*
  while (1)
      ;
       */
}

// GPIO�жϴ����գ�
void GPIO_IRQHandler(void) {}

// I2C0�жϴ����գ�
void I2C0_IRQHandler(void) {}

// SPI0�жϴ����գ�
void SPI0_IRQHandler(void) {}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR
 * **********************/
/* ------------------------------END OF FILE------------------------------------
 */
