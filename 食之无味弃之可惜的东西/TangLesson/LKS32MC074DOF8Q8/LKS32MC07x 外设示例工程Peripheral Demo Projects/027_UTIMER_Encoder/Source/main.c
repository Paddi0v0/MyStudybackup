/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： Main.c
 * 文件标识：
 * 内容摘要： 工程主代码
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： HuangMG
 * 完成日期： 2023年3月2日
 *
 *******************************************************************************/
#include "hardware_init.h"

void Hardware_init(void);
/*******************************************************************************
 函数名称：    int main(void)
 功能描述：    主程序入口
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    使用//配置P2.11(QEP0_T1) ,P2.12(QEP0_T2)和P0.13（QEP0_Z）,编码器配置
               counting on T1计数模式，TH=65535。
               
 
 运行效果：    将编码器三根线接入上面三个IO口，然后选择编码器，debug查看QEP_CNT计数值。
							
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2023/2/20      V1.0          HuangMG            创建
 *******************************************************************************/
u32 QEP_CNT = 0;   /*编码器计数值*/
int main(void)
{
    Hardware_init();                         //硬件初始化
    for(;;)
    {
			QEP_CNT = TIM_QEP_GetCount(QEP0);
			SoftDelay(100);
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

