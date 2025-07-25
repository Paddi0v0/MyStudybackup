/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： Main.c
 * 文件标识：
 * 内容摘要： 工程主代码
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： 李鹏
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：2021年8月25日
 * 版 本 号：V 1.0
 * 修 改 人：HMG
 * 修改内容：创建
 *
 *******************************************************************************/
#include "hardware_config.h"

/*******************************************************************************
 函数名称：    int main(void)
 功能描述：    主程序入口
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    测试说明：本实验配置4组PWM输出，MCPWM_CH0(P1.4\P1.5),MCPWM_CH1(P1.6\P1.7)
              MCPWM_CH2(P1.8\P1.9),MCPWM_CH3(P1.10\P1.11)
			  通电运行。
			   运行效果为：程序烧录后，对应的MCPWM口会产生四组中心互补的PWM波。
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/2/25
 *******************************************************************************/

int main(void)
{
	Hardware_init(); /* 硬件初始化 */
	while (1)
	{
		SoftDelay(0x10000);
	}
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
