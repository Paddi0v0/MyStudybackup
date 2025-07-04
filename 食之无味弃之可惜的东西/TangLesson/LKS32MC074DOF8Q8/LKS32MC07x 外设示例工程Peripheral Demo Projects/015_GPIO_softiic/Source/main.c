#include "lks32mc07x_lib.h"
uint8_t writetable[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint8_t readtable[10] = {0};
uint8_t writetable1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint8_t readtable1[10] = {0};
/**
 * @brief  ��ʱ����
 * @param  cnt: ��ʱʱ��
 */
void SoftDelay(u32 cnt);
/**
 * @brief  ���ֽ�д
 * @param  flashaddr: д���ַ
 * @param  data: д������
 */
void eeprom_write_one(uint16_t flashaddr, uint8_t data);
/**
 * @brief  ���ֽ�д
 * @param  flashaddr: д���ַ
 * @param  data: д������
 * @param  len: д�볤��
 */
void eeprom_write(uint16_t flashaddr, uint8_t *data, uint32_t len);
/**
 * @brief  ���ֽڶ�
 * @param  flashaddr: ��ȡ��ַ
 * @return ��ȡ����
 */
uint8_t eeprom_read_one(uint16_t flashaddr);
/**
 * @brief  ���ֽڶ�
 * @param  flashaddr: ��ȡ��ַ
 * @param  data: ��ȡ����
 * @param  len: ��ȡ����
 */
void eeprom_read(uint16_t flashaddr, uint8_t *data, uint32_t len);
/**
 * @brief  ������
 */
int main(void)
{
    SYS_ModuleClockCmd(SYS_Module_GPIO, ENABLE);
    GPIO0->PIE |= BIT3 | BIT4;
    GPIO0->POE |= BIT3 | BIT4;
    GPIO0->PODE |= BIT4; // sdaʹ�ÿ�©�����sclʹ�����������scl���ʹ�ÿ�©��������������յ����ţ�

    for (;;)
    {
        uint16_t i;
        // ���ֽ�д��
        for (i = 0; i < 10; i++)
        {
            eeprom_write_one(i, writetable[i]);
            SoftDelay(0xffff); // д����ɺ���Ҫ�ȴ���Լ10ms����
        }
        for (i = 0; i < 10; i++)
        {
            readtable[i] = eeprom_read_one(i);
        }
        SoftDelay(0xfffff);

        // ���ֽ�д��
        eeprom_write(0x200, writetable1, 10); // ���ֽ�д��
        SoftDelay(0xffff);                    // д����ɺ���Ҫ�ȴ���Լ10ms����
        eeprom_read(0x200, readtable1, 10);
        SoftDelay(0xfffff);
    }
}
// ����IIC����
#define SCL_IO GPIO0, GPIO_Pin_3
#define SDA_IO GPIO0, GPIO_Pin_4
// ����IIC��ַ
#define ADDR 0xa0

// IIC����
static void iicdelay(void);
static void start(void);          // ����IIC��ʼ�ź�
static void stop(void);           // ����IICֹͣ�ź�
static uint8_t waitAck(void);     // �ȴ�Ӧ���źŵ���
static void ack(void);            // ����ACKӦ��
static void nack(void);           // ������ACKӦ��
static void send(uint8_t txd);    // IIC����һ���ֽ�
static uint8_t read(uint8_t ack); // ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint32_t softiic_errflg = 0;

/**
 * @brief ���ֽ�д
 * @param flashaddr: д���ַ
 * @param data: д������
 */
void eeprom_write_one(uint16_t flashaddr, uint8_t data)
{
    uint8_t ack;
    start();
    send(ADDR);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    send(flashaddr >> 8);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    send(flashaddr & 0xff);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    send(data);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    stop();
}
/**
 * @brief ���ֽ�д
 * @param flashaddr: д���ַ
 * @param data: д������
 * @param len: д�볤��
 */
void eeprom_write(uint16_t flashaddr, uint8_t *data, uint32_t len)
{
    uint8_t ack;
    start();
    send(ADDR);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    send(flashaddr >> 8);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    send(flashaddr & 0xff);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        send(data[i]);
        ack = waitAck();
        if (ack == 1)
        {
            softiic_errflg++;
            stop();
            return;
        }
    }
    stop();
}
/**
 * @brief ���ֽڶ�
 * @param flashaddr: ��ȡ��ַ
 * @return ��ȡ����
 */
uint8_t eeprom_read_one(uint16_t flashaddr)
{
    uint8_t ack;
    uint8_t rev = 0;
    start();
    send(ADDR);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return rev;
    }
    send(flashaddr >> 8);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return rev;
    }
    send(flashaddr & 0xff);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return rev;
    }
    start();
    send(ADDR | 1);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return rev;
    }
    rev = read(0);
    stop();
    return rev;
}
/**
 * @brief ���ֽڶ�
 * @param flashaddr: ��ȡ��ַ
 * @param data: ��ȡ����
 * @param len: ��ȡ����
 */
void eeprom_read(uint16_t flashaddr, uint8_t *data, uint32_t len)
{
    uint8_t ack;
    start();
    send(ADDR);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    send(flashaddr >> 8);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    send(flashaddr & 0xff);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    start();
    send(ADDR | 1);
    ack = waitAck();
    if (ack == 1)
    {
        softiic_errflg++;
        stop();
        return;
    }
    uint32_t i;
    for (i = 0; i < len - 1; i++)
    {
        data[i] = read(1);
    }
    data[len - 1] = read(0); // ���һ�ζ�ȡ��Ҫ����nack
    stop();
    return;
}
/**
 * @brief  gpio����͵�ƽ
 * @param  gpiox: GPIOx
 * @param  pinx: GPIO_Pin_x
 */
static void gpio_out0(GPIO_TypeDef *gpiox, uint16_t pinx)
{
    gpiox->BRR = pinx;
}
/**
 * @brief  gpio����ߵ�ƽ
 * @param  gpiox: GPIOx
 * @param  pinx: GPIO_Pin_x
 */
static void gpio_out1(GPIO_TypeDef *gpiox, uint16_t pinx)
{
    gpiox->BSRR = pinx;
}
/**
 * @brief  gpio��ȡ��ƽ
 * @param  gpiox: GPIOx
 * @param  pinx: GPIO_Pin_x
 * @return ��ƽ
 */
static uint16_t gpio_read(GPIO_TypeDef *gpiox, uint16_t pinx)
{
    return gpiox->PDI & pinx;
}
/**
 * @brief  ����IIC��ʼ�ź�
 */
static void start(void)
{
    gpio_out1(SCL_IO);
    gpio_out1(SDA_IO);
    iicdelay();
    gpio_out0(SDA_IO);
    iicdelay();
    gpio_out0(SCL_IO);
}
/**
 * @brief  ����IICֹͣ�ź�
 */
static void stop(void)
{
    gpio_out0(SCL_IO);
    gpio_out0(SDA_IO); // STOP:when CLK is high DATA change form low to high
    iicdelay();
    gpio_out1(SCL_IO);
    gpio_out1(SDA_IO);
    iicdelay();
}
/**
 * @brief  �ȴ�Ӧ���źŵ���
 * @return 1������Ӧ��ʧ��
 *        0������Ӧ��ɹ�
 */
static uint8_t waitAck(void)
{
    uint8_t ucErrTime = 0;
    gpio_out1(SDA_IO); // SDA����Ϊ����
    gpio_out1(SDA_IO);
    iicdelay();
    gpio_out1(SCL_IO);
    iicdelay();
    while (gpio_read(SDA_IO))
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            stop();
            return 1;
        }
    }
    gpio_out0(SCL_IO); // ʱ�����0
    return 0;
}
/**
 * @brief  ����ACKӦ��
 */
static void ack(void)
{
    gpio_out0(SCL_IO);
    gpio_out1(SDA_IO);
    gpio_out0(SDA_IO);
    iicdelay();
    gpio_out1(SCL_IO);
    iicdelay();
    gpio_out0(SCL_IO);
}
/**
 * @brief  ������ACKӦ��
 */
static void nack(void)
{
    gpio_out0(SCL_IO);
    gpio_out1(SDA_IO);
    gpio_out1(SDA_IO);
    iicdelay();
    gpio_out1(SCL_IO);
    iicdelay();
    gpio_out0(SCL_IO);
}
/**
 * @brief  IIC����һ���ֽ�
 * @param  txd: ���͵��ֽ�
 */
static void send(uint8_t txd)
{
    uint8_t t;
    gpio_out1(SDA_IO);
    gpio_out0(SCL_IO); // ����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++)
    {
        if (((txd & 0x80) >> 7))
        {
            gpio_out1(SDA_IO);
        }
        else
        {
            gpio_out0(SDA_IO);
        }
        txd <<= 1;
        iicdelay(); // ��TEA5767��������ʱ���Ǳ����
        gpio_out1(SCL_IO);
        iicdelay();
        gpio_out0(SCL_IO);
        //        iicdelay(4);
    }
}
/**
 * @brief  ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
 * @param  ack: 1������ACK
 *            0������nACK
 * @return ��ȡ������
 */
static uint8_t read(uint8_t tack)
{
    unsigned char i, receive = 0;
    gpio_out1(SDA_IO); // SDA����Ϊ����
    for (i = 0; i < 8; i++)
    {
        gpio_out0(SCL_IO);
        iicdelay();
        gpio_out1(SCL_IO);
        receive <<= 1;
        if (gpio_read(SDA_IO))
            receive++;
        iicdelay();
    }
    if (!tack)
    {
        nack(); // ����nACK
    }
    else
    {
        ack(); // ����ACK
    }
    return receive;
}
/**
 * @brief  ��ʱ����
 * @param  cnt: ��ʱʱ��
 */
static void iicdelay(void)
{
    uint32_t cnt = 100;
    for (; cnt > 0; cnt--)
    {
        __NOP();
    }
}
