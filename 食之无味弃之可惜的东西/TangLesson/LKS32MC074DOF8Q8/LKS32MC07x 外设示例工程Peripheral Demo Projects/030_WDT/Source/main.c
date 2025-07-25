/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： Main.c
 * 文件标识：
 * 内容摘要： 工程主代码
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet Li
 * 完成日期： 2020年8月5日
 *
 * 修改记录1：
 * 修改日期：2020年8月16日
 * 版 本 号：V 1.0
 * 修 改 人：Howlet Li
 * 修改内容：创建
 *
 *******************************************************************************/
#include "lks32mc07x_lib.h"
#include "hardware_init.h"

/*******************************************************************************
 函数名称：    int main(void)
 功能描述：    主程序入口
 输入参数：    无         
 输出参数：    无
 返 回 值：    无
 其它说明：    实验配置看门狗2S进行复位，P0.6作为指示IO，在硬件初始化函数内进行IO翻转，
              翻转次数为1000次，这样当看门狗2S内不进行喂狗，则芯片会发生复位，那么我们
			        就可以通过P0.6的电平翻转进行观察，即可判断芯片复位了。
			        如果在main函数500ms喂狗，那么P0.6就是上电翻转1000次，随后就不在翻转。
 
 运行效果：    程序烧录凌鸥LKS32MC071最小系统板，通过示波器测量P0.6电平可以看到有一
              段时间会有PWM发出，然后IO电平固定。如果将mian函数中喂狗函数不屏蔽，则IO电
			        平一直保持，反之屏蔽喂狗语句，则P0.6每间隔2S左右的时间电平进行翻转。

 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2023/2/26     V1.0          cakezhang             创建
 *******************************************************************************/
int main(void)
{
	Hardware_init(); /* 硬件初始化 */
	for (;;)
	{
		IWDG_Feed();//看门狗喂狗
		SoftDelay(0xFFFFF);     
	}
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

