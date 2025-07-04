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
 * 修改日期：2021年8月27日
 * 版 本 号：V 1.0
 * 修 改 人：李鹏
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
 其它说明：    本实验使用凌鸥081的demo板进行低功耗实验， 本实验唤醒时间设置为4S，进入休
               待中断执行， 所以在执行__WFI();之前，先使能中断。
														 
 运行效果：    将程序烧录到demo板后，使用万用表电流档测量电源输入电流值。按下START按键后，
              芯片进入低功耗模式，此时KEIL无法烧录程序，再使用万用表测量此时电源输入电源
			  值，对比发现电流值有明显降低。进入休眠间隔4S后LED1电平翻转，电源输入电流恢
			  复正常。

 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
  2022/4/14     V1.0           HuangMG            创建
 *******************************************************************************/
int main(void)
{
	u8 key = 0;
	Hardware_init();           /* 硬件初始化 */
	SYS_SetWakeInterval(SYS_WakeInterval_4s);   //Utimer 4秒唤醒
	for(;;)
	{
		key = GPIO_KEY_Scan(0);
		switch (key)
		{
			case start_PRES:
					 EnterSleep();   /*开启睡眠模式（休眠函数不能放入休眠中断内）*/
				break;
			case stop_PRES:	   
					 
				break;		
		}
		 SoftDelay(0xffff);
	}
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
