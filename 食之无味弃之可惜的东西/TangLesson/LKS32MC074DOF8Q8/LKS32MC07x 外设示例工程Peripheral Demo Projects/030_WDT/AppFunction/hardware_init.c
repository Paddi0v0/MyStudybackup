/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： hardware_init.c
 * 文件标识：
 * 内容摘要： 硬件初始化代码
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet Li
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *******************************************************************************/
#include "basic.h"
#include "lks32mc07x_sys.h"
#include "hardware_init.h"


/*******************************************************************************
 函数名称：    void Hardware_init(void)
 功能描述：    硬件部分初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();                  /* 关闭中断 中断总开关 */
    SYS_WR_PROTECT = 0x7a83;
    IWDG_DISABLE();                   /* 关闭看门狗*/
    GPIO_init();    	                /* GPIO初始化 */
    out_start_signal();               /* 翻转指示IO*/
	  IWDG_init();  
	  SoftDelay(100);                   /* 等待硬件初始化完毕*/                    
	__enable_irq();                     /* 开启总中断 */
}


/*******************************************************************************
 函数名称：    void Clock_Init(void)
 功能描述：    时钟配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建

 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/4/17      V1.1           Howlet Li          增加延时函数，等待PLL稳定
 *******************************************************************************/
void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;   /* 解除系统寄存器写保护 */
    SYS_AFE_REG5 = BIT15;      /* BIT15:PLLPDN 开PLL */
    SoftDelay(100);            /* 延时100us, 等待PLL稳定 21.4.17*/
    SYS_CLK_CFG = 0x000001ff;  /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff对应96M时钟 */
    SYS_WR_PROTECT = 0;        /* 关闭系统寄存器写操作*/
}

/*******************************************************************************
 函数名称：    void Reg_Clr(void)
 功能描述：    寄存器清零
 输入参数：    addr ：需要清零寄存器的起始地址 
               nSize：需要清零的寄存器个数
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/3/14      V1.0           Howlet Li          创建
 *******************************************************************************/
void Reg_Clr(u32 addr, u8 nSize)
{
   while(nSize --)
   {
      REG32(addr) = 0;
      addr += 4;
   }
}


/*******************************************************************************
 函数名称：    void SystemInit(void)
 功能描述：    硬件系统初始化，调用时钟初始化函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          创建
 *******************************************************************************/
void SystemInit(void)
{
    Clock_Init();  /* 时钟初始化 */
}






/*******************************************************************************
 函数名称：    void GPIO_init(void)
 功能描述：    GPIO硬件初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/

void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
}

/*******************************************************************************
 函数名称：    void out_start_signal(void)
 功能描述：    程序运行启动指示
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/11/15      V1.0        HuangMG        创建
 *******************************************************************************/
void out_start_signal(void)
{
    int i;
    for(i=0;i<1000;i++)
    {
        Invers_GPIO(GPIO0,GPIO_Pin_6);
		SoftDelay(1000);
    }
}

/*******************************************************************************
 函数名称：    uint8_t Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
 功能描述：    GPIO电平反转函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/14     V1.0          HuangMG             创建
 *******************************************************************************/
uint8_t Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    u16 reg;
    reg = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin); //读GPIO电平
    if (reg)
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
        return 0;
    }
    else
    {
        GPIO_SetBits(GPIOx, GPIO_Pin);
        return 1;
    }
}

/*******************************************************************************
 函数名称：    void IWDG_init(void)
 功能描述：    看门狗初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/26    V1.0           YangZJ            创建
 *******************************************************************************/
void IWDG_init(void)
{
    IWDG_InitTypeDef IWDG_InitStruct;
    IWDG_StrutInit(&IWDG_InitStruct);
    IWDG_InitStruct.WDG_EN  = ENABLE;      //使能看门狗
    IWDG_InitStruct.RTH     = SECOND2IWDGCNT(2.0);//设置看门狗
    IWDG_InitStruct.DWK_EN  = DISABLE;     // 深度休眠定时唤醒关闭   
    IWDG_InitStruct.WTH     = 0;           // 看门狗定时唤醒时间（21位计数器，但低12恒位0）
    IWDG_Init(&IWDG_InitStruct);
    IWDG_ENABLE();                         // 开启看门狗
}
/*******************************************************************************
 函数名称：    void UART_init(void)
 功能描述：    UART0寄存器配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void UART_init(void)
{
  
}
/*******************************************************************************
 函数名称：    void SoftDelay(void)
 功能描述：    软件延时函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;

    for(t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
    }
}
