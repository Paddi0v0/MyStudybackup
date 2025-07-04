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
#include "hardware_init.h"
#include "lks32mc07x_lib.h"

void Hardware_init(void);

/*******************************************************************************
 函数名称：    int main(void)
 功能描述：    主程序入口
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    配置ADC0单段采样模式，分别基于3.6V和7.2V两个量程对通道9进行采样，
               对比几样结果存入数组中ADCSample[]
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2023/3/9      V1.0           Zhujie             创建
 *******************************************************************************/

u32 Cnt = 0;
s16 ADCSampleValue = 0;
s16 CurrentTempature = 0;
int main(void)
{
    Hardware_init();      /* 硬件初始化 */
	  PWMOutputs(ENABLE);
    for(;;)
    {
			  Cnt ++;
			  if(Cnt < 100000)
				{
				    GPIO_SetBits (GPIO0 ,GPIO_Pin_6);
					  GPIO_ResetBits (GPIO0 ,GPIO_Pin_7);
				}
				else if(Cnt < 200000)
				{
				    GPIO_ResetBits (GPIO0 ,GPIO_Pin_6);
					  GPIO_SetBits (GPIO0 ,GPIO_Pin_7);
				}
				else
				{
				    Cnt = 0;
				}
				ADCSampleValue = ADC_GetConversionValue(ADC1,DAT1);
        CurrentTempature = GetCurrentTemperature(ADCSampleValue) ;
			  SoftDelay(10);
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

