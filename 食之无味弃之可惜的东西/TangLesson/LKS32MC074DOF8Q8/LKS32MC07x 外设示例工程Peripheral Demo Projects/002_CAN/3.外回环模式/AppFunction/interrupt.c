/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： interrupt.c
 * 文件标识：
 * 内容摘要： 中断服务程序文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2020年8月5日
 *
 * 修改记录1：
 * 修改日期：2020年8月5日
 * 版 本 号：V 1.0
 * 修 改 人：Howlet
 * 修改内容：创建
 *
 *******************************************************************************/
#include "lks32mc07x_lib.h"


/*******************************************************************************
 函数名称：    void ADC0_IRQHandler(void)
 功能描述：    ADC0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void ADC1_IRQHandler(void)
{
    ADC1_IF |= BIT1 | BIT0;
}


/*******************************************************************************
 函数名称：    void MCPWM_IRQHandler(void)
 功能描述：    MCPWM中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void MCPWM_IRQHandler(void)
{
    MCPWM0_IF0 = BIT1 | BIT0;    
}


/*******************************************************************************
 函数名称：    void HALL_IRQHandler(void)
 功能描述：    HALL中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void HALL0_IRQHandler(void)
{
    HALL->INFO = HALL->INFO;    // 清标志位
}


/*******************************************************************************
 函数名称：    void TIMER0_IRQHandler(void)
 功能描述：    TIMER0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void TIMER0_IRQHandler(void)
{
    /* 时基500us */
    UTIMER0_IF = BIT2;
}

/*******************************************************************************
 函数名称：    void TIMER1_IRQHandler(void)
 功能描述：    TIMER1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void TIMER1_IRQHandler(void)
{

}

/*******************************************************************************
 函数名称：    void TIMER2_IRQHandler(void)
 功能描述：    TIMER2中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void TIMER2_IRQHandler(void)
{

}

/*******************************************************************************
 函数名称：    void TIMER3_IRQHandler(void)
 功能描述：    TIMER3中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void TIMER3_IRQHandler(void)
{

}
/*******************************************************************************
 函数名称：    void ENCODER0_IRQHandler(void)
 功能描述：    ENCODER1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void ENCODER0_IRQHandler(void)
{

}

/*******************************************************************************
 函数名称：    void ENCODER0_IRQHandler(void)
 功能描述：    ENCODER1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void ENCODER1_IRQHandler(void)
{

}




/*******************************************************************************
 函数名称：    void CMP_IRQHandler(void)
 功能描述：    比较器中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void CMP_IRQHandler(void)
{
    CMP_IF = BIT0 | BIT1;
}



/*******************************************************************************
 函数名称：    void UART0_IRQHandler(void)
 功能描述：    UART0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void UART0_IRQHandler(void)
{
}

/*******************************************************************************
 函数名称：    void UART1_IRQHandler(void)
 功能描述：    UART1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void UART1_IRQHandler(void)
{
  

}

/*******************************************************************************
 函数名称：    void SysTick_Handler(void)
 功能描述：    系统滴答定时中断
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void SysTick_Handler(void)
{

}

/*******************************************************************************
 函数名称：    void SleepTimer_IRQHandler(void)
 功能描述：    休眠唤醒中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void SleepTimer_IRQHandler(void)
{
    while(1);
}

/*******************************************************************************
 函数名称：    void GPIO_IRQHandler(void)
 功能描述：    GPIO中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void GPIO_IRQHandler(void)
{

}

void QEP0_IRQHandler(void)
{
}
void QEP1_IRQHandler(void)
{
}
/*******************************************************************************
 函数名称：    void I2C0_IRQHandler(void)
 功能描述：    I2C0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void I2C0_IRQHandler(void)
{

}

/*******************************************************************************
 函数名称：    void SPI0_IRQHandler(void)
 功能描述：    SPI0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void SPI0_IRQHandler(void)
{

}

void DSP0_IRQHandler(void)
{
}
void ADC0_IRQHandler(void)
{
}
void MCPWM0_IRQHandler(void)
{
}

void MCPWM1_IRQHandler(void)
{
}

void SIF0_IRQHandler  (void)
{
}
void WAKE_IRQHandler  (void)
{
}
void SW_IRQHandler    (void)
{
}
void PWRDN_IRQHandler (void)
{
}
void CL0_IRQHandler   (void)
{
}

/*******************************************************************************
 函数名称：    void CAN_IRQHandler(void)
 功能描述：    CAN中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2023/2/13      V1.0          HuangMG            创建
 *******************************************************************************/
void CAN0_IRQHandler(void)
{
  if(CAN_RTIF & BIT3)/*PTB 发送中断标志*/
	{
		CAN_RTIF = BIT3;
	}
	if(CAN_RTIF & BIT2)/*STB 发送中断标志*/
	{
		CAN_RTIF = BIT2; 
	}
	/*****************接收设备一直无应答，则取消重发发送*****************/
 if(CAN_ERRINT & BIT0)/*取消发送中断标志*/
 {
	 CAN_ERRINT = BIT0;
	if(CAN_TECNT > 127)   /*被动错误*/
	{
		if(CAN_EALCAP & 0x80) /* 应答错误*/
		{
//		   CAN_TCMD |= BIT3; /*取消PTB发送*/
		}
	}
 }
 if(CAN_RTIF & BIT7)/*接收到有效帧标志*/
 {
     CAN_RTIF = BIT7;
	   CAN_Receive_Msg(&( can_par.id),&( can_par.len),&(can_par.ide),&(can_par.rtr),can_par.RX) ;
 }
 
 if(CAN_RTIF & BIT4)
 {
	  CAN_ERRINT = BIT4; /*被动错误中断标志*/
 }
 
 if(CAN_RTIF & BIT5)
 {
   CAN_RTIF = BIT5;
 }
 if(CAN_RTIF & BIT6)
 {
   CAN_RTIF = BIT6;
 }
  if(CAN_RTIF & BIT1)
 {
   CAN_RTIF = BIT1;
 }
 
  CAN_RTIF = 0xff;
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */


