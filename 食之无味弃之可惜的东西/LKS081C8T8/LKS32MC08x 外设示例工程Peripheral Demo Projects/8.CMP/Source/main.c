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
 * 修改日期：2015年11月5日
 * 版 本 号：V 1.0
 * 修 改 人：李鹏
 * 修改内容：创建
 *
 *******************************************************************************/
#include "hardware_config.h"

/*******************************************************************************
 函数名称：    int main(void)
 功能描述：    该程序主要用于比较P2.1输出电压是否大于基准电压值，如果大于CMP1产生中断LED1
               亮，如果P2.1电压一直高于基准电压，则CMP1一直处于中断状态，LED1始终亮，此时
							 按键不起作用。 CMP1_OUT口输出CMP1的结果电压。
							 如果不想P2.1电压一直高于基准电压，MP1一直处于中断状态，则可以关断CMP1中断，
							 此时CMP1_OUT口输出正常。
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    测试说明：
			        运行效果为：用杜邦线将LED2和OC脚相连，按START键后，LED2亮和LED1亮，这时按下
							STOP，LED1和LED2不变化断开LED2和OC连接，按下STOP，LED1和LED2灭。
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5     V1.0            HuangMG           创建
 *******************************************************************************/

int main(void)
{
	u8 key = 0;
	Hardware_init();   /* 硬件初始化 */
	//初始化所有LED全灭
	GPIO_ResetBits(GPIO0,GPIO_Pin_6);
	GPIO_ResetBits(GPIO0,GPIO_Pin_7); 
	GPIO_SetBits(GPIO0,GPIO_Pin_3);
	while (1)
	{
		key = GPIO_KEY_Scan(0);
		switch (key)
		{
		case start_PRES:
			GPIO_SetBits(GPIO0,GPIO_Pin_7);
			break;

		case stop_PRES:
			GPIO_ResetBits(GPIO0,GPIO_Pin_6);
		  GPIO_ResetBits(GPIO0,GPIO_Pin_7);
			break;
		}
		SoftDelay(0xFF);
	}
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
