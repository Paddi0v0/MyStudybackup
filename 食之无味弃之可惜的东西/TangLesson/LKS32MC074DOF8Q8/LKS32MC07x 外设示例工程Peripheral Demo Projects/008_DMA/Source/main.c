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
 * 修改日期：2022年02月22日
 * 版 本 号：V 1.0
 * 修 改 人：ywx
 * 修改内容：创建
 *
 *******************************************************************************/
#include "hardware_init.h"
/*******************************************************************************
 函数名称：    int main(void)
 功能描述：    主程序入口
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：     实验要求：通过DMA将内存数据搬运到uart外设，由串口发送到上位机,通过串口助手查看		   
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 *******************************************************************************/
int main(void)
{
	Hardware_init();
	while (1)
	{
		SoftDelay(0XFFFFF);				  
	}
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
