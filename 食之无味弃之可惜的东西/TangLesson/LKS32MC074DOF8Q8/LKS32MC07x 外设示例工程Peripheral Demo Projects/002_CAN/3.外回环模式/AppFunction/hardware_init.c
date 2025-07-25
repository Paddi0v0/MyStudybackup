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
    
    FLASH_CFG |= 0x00080000;          /* enable prefetch */
    IWDG_DISABLE();                   /* 关闭看门狗*/
	  GPIO_init();                      /* GPIO初始化*/
    CAN_Init(0x0B,0x02,0x05,0x08);    //CAN波特率500Khz
	  ID0_Filter(0x234,0x000,1);         //开启ID0接收滤波，接收id=0x02，扩展帧
//	  ID1_Filter(0x03,0x000,0);         //开启ID1接收滤波，接收id=0x03，标准帧
//	  ID2_Filter(0x04,0x000,0);         //开启ID2接收滤波，接收id=0x04，标准帧
//	  ID3_Filter(0x05,0x000,1);         //开启ID3接收滤波，接收id=0x05，扩展帧
	  SoftDelay(100);                   /* 等待硬件初始化完毕*/
	  SYS_WR_PROTECT = 0x0;             /* 关闭系统寄存器写操作*/
/*************************************************************************/	
	  CAN_CFG_STAT |= BIT6;             /* 开启外回环模式*/
	  CAN_RCTRL |= BIT7;                /* 开启外回环模式时需要开启《自应答》功能*/
/*************************************************************************/	
	  NVIC_SetPriority(CAN0_IRQn, 0);   //CAN 中断优先级配置
    NVIC_EnableIRQ(CAN0_IRQn);        //使能 CAN 定时器中断
    __enable_irq();                   /* 开启总中断 */
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
    GPIO_StructInit(&GPIO_InitStruct);        //初始化结构体
		
		GPIO_StructInit(&GPIO_InitStruct);        //初始化结构体
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; //GPIO输入模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
	
		GPIO_StructInit(&GPIO_InitStruct);         //初始化结构体
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //GPIO输出模式	
	  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIO0,GPIO_PinSource_6,AF10_CAN);
		GPIO_PinAFConfig(GPIO0,GPIO_PinSource_7,AF10_CAN);
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




