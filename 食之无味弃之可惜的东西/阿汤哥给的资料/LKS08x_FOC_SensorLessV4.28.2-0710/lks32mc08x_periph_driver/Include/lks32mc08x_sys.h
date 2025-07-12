/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_sys.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� sys����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��05��21�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */

#ifndef __LKS32MC08x_SYS_H
#define __LKS32MC08x_SYS_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
#include <stdbool.h>

/** �͹���IO���ѵ�ƽö�ٳ�Ա*/ 
typedef enum
{
	WAKETRIG_LOW, /**����IO�͵�ƽ����*/ 
	WAKETRIG_HIGH,/**����IO�ߵ�ƽ����*/ 
}enumWakeTrigLevel;

/** 
 *@brief �͹���ʱ�������м����
 */
typedef struct
{
    uint32_t AFE_REG5_RECORD;
    uint32_t AFE_REG6_RECORD;
	uint32_t CLK_CFG_RECORD;
}WAKE_InitTypeDef;

/** 
 *@brief ϵͳ��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
	uint32_t PLL_SrcSel;                      /**<PLL�ź�����Դѡ��RCʱ�ӻ��ⲿ���� @see SYS_PLLSRSEL_RCH @see SYS_PLLSRSEL_CRYSTAL*/

	uint32_t PLL_DivSel;                      /**<ѡ��8��ʱ�������У��ĸ��������ʱ��*/
	uint32_t PLL_ReDiv;                       /**<PLL�ٷ�Ƶ��1��Ƶ��4��Ƶ*/

	uint32_t Clk_DivSPI;                      /**<I2Cʱ�ӷ�Ƶ*/
	uint32_t Clk_DivUART;                     /**<UARTʱ�ӷ�Ƶ*/
	uint32_t Clk_FEN;                         /**<ģ��ʱ��ʹ��*/

	uint32_t WDT_Ena;                         /**<ʹ�ܿ��Ź�*/
	uint32_t PORFilter_Ena;                   /**<ʹ��POR�˲�*/
	
}SYS_InitTypeDef;


/*PLL�ź�����Դѡ��*/
#define SYS_PLLSRSEL_RCH        0x00  /**<ʹ��4MHz RCʱ��*/
#define SYS_PLLSRSEL_CRYSTAL    0x01  /**<ʹ�þ���ʱ��*/

/*PLL�ٷ�Ƶ����*/
#define SYS_PLLREDIV_4			0x00  /**<��PLL��Ƶ��������/4*/
#define SYS_PLLREDIV_1			0x01  /**<��PLL��Ƶ�����ϲ��ٷ�Ƶ*/

/*SPIʱ�ӷ�Ƶ*/
#define SYS_Clk_SPIDiv1          0 /**< SPIʱ�ӷ�Ƶ��ռʱδʹ��*/
#define SYS_Clk_SPIDiv2          1 /**< SPIʱ�ӷ�Ƶ��ռʱδʹ��*/
#define SYS_Clk_SPIDiv4          2 /**< SPIʱ�ӷ�Ƶ��ռʱδʹ��*/
#define SYS_Clk_SPIDiv8          3 /**< SPIʱ�ӷ�Ƶ��ռʱδʹ��*/

/*UARTʱ�ӷ�Ƶ*/
#define SYS_Clk_UARTDiv1         0 /**< UARTʱ�ӷ�Ƶ��ռʱδʹ��*/
#define SYS_Clk_UARTDiv2         1 /**< UARTʱ�ӷ�Ƶ��ռʱδʹ��*/
#define SYS_Clk_UARTDiv4         2 /**< UARTʱ�ӷ�Ƶ��ռʱδʹ��*/
#define SYS_Clk_UARTDiv8         3 /**< UARTʱ�ӷ�Ƶ��ռʱδʹ��*/

/*����ģ��λ����*/
#define SYS_Module_I2C           BIT0 /**< I2Cģ��*/
#define SYS_Module_HALL          BIT1 /**< HALLģ��*/
#define SYS_Module_TIMER         BIT2 /**< TIMERģ��*/
#define SYS_Module_MCPWM         BIT3 /**< MCPWMģ��*/
#define SYS_Module_UART0         BIT4 /**< UART0ģ��*/
#define SYS_Module_UART1         BIT5 /**< UART1ģ��*/
#define SYS_Module_DSP           BIT6 /**< DSPģ��*/
#define SYS_Module_CAN           BIT7 /**< CANģ��*/
#define SYS_Module_SPI           BIT8 /**< SPIģ��*/

/*ģ��ģ�鶨��*/
#define SYS_AnalogModule_ADC     BIT0  /**<ADCģ��*/
#define SYS_AnalogModule_OPA0    BIT2  /**<OPA0ģ��*/
#define SYS_AnalogModule_OPA1    BIT3  /**<OPA1ģ��*/
#define SYS_AnalogModule_OPA2    BIT4  /**<OPA2ģ��*/
#define SYS_AnalogModule_OPA3    BIT5  /**<OPA3ģ��*/
#define SYS_AnalogModule_CMP0    BIT6  /**<CMP0ģ��*/
#define SYS_AnalogModule_CMP1    BIT7  /**<CMP1ģ��*/
#define SYS_AnalogModule_BGP     BIT8  /**<BGPģ��*/
#define SYS_AnalogModule_RCH     BIT10 /**<RCHģ��*/
#define SYS_AnalogModule_DAC     BIT12 /**<DACģ��*/
#define SYS_AnalogModule_TMP     BIT13 /**<�¶ȴ�����ģ��*/
#define SYS_AnalogModule_XTAL    BIT14 /**<����ģ��*/
#define SYS_AnalogModule_PLL     BIT15 /**<PLLģ��*/

/*���Ź���ʱʱ��*/
#define SYS_WD_TimeOut2s  0         /**<���Ź���ʱʱ��2s��λ*/
#define SYS_WD_TimeOut4s  1         /**<���Ź���ʱʱ��4s��λ*/
#define SYS_WD_TimeOut8s  2         /**<���Ź���ʱʱ��8s��λ*/
#define SYS_WD_TimeOut64s 3         /**<���Ź���ʱʱ��64s��λ*/

/*��λ�ź�Դ����*/
#define SYS_RstSrc_LPOR        0x01     /**<��λ�ź�Դ���ڲ�1.5V��Դ����λ*/
#define SYS_RstSrc_HPOR        0x02     /**<��λ�ź�Դ���ڲ�3.3V��Դ����*/
#define SYS_RstSrc_KEYBOARD    0x04     /**<��λ�ź�Դ��������λ*/
#define SYS_RstSrc_WDT         0x08     /**<��λ�ź�Դ��WDT��λ*/

/*���߻��Ѽ��ʱ��*/
#define SYS_WakeInterval_025s  0x00     /**<���߻��Ѽ��ʱ��0.25s*/    
#define SYS_WakeInterval_05s   0x01     /**<���߻��Ѽ��ʱ��0.5s*/
#define SYS_WakeInterval_1s    0x02     /**<���߻��Ѽ��ʱ��1s*/
#define SYS_WakeInterval_2s    0x03     /**<���߻��Ѽ��ʱ��2s*/
#define SYS_WakeInterval_4s    0x04     /**<���߻��Ѽ��ʱ��4s*/
#define SYS_WakeInterval_8s    0x05     /**<���߻��Ѽ��ʱ��8s*/
#define SYS_WakeInterval_16s   0x06     /**<���߻��Ѽ��ʱ��16s*/
#define SYS_WakeInterval_32s   0x07     /**<���߻��Ѽ��ʱ��32s*/

/*�͹��Ļ���IOѡ��*/
#define WAKEIO_P0_0	 BIT0  /**<ϵͳ�͹���ʱʹ��P0.0��Ϊ����IO*/
#define WAKEIO_P0_1	 BIT1  /**<ϵͳ�͹���ʱʹ��P0.1��Ϊ����IO*/
#define WAKEIO_P1_0	 BIT2  /**<ϵͳ�͹���ʱʹ��P1.0��Ϊ����IO*/
#define WAKEIO_P1_1	 BIT3  /**<ϵͳ�͹���ʱʹ��P1.1��Ϊ����IO*/

#define MCU_POWER_3v3  0 //MCU 3.3V����
#define MCU_POWER_5v0  1 //MCU 5V����

#define POWER_MODE         MCU_POWER_5v0 

#ifndef POWER_4V5
#define POWER_4V5   0x00    /*��Դ��������ֵΪ4.5V*/   
#endif
#ifndef POWER_4V2
#define POWER_4V2   0x01    /*��Դ��������ֵΪ4.2V*/ 
#endif
#ifndef POWER_3V9
#define POWER_3V9   0x02    /*��Դ��������ֵΪ3.9V*/
#endif
#ifndef POWER_3V6
#define POWER_3V6   0x03    /*��Դ��������ֵΪ3.6V*/ 
#endif


/*ϵͳ����*/
void SYS_Init(SYS_InitTypeDef* SYS_InitStruct);
void SYS_StructInit(SYS_InitTypeDef* SYS_InitStruct);

/*��λԴ*/
uint32_t SYS_GetRstSource(void);
void SYS_ClearRst(void);

/*���Ź�*/
void SYS_WatchDogCmd(FuncState state);
void SYS_FeedDog(uint32_t timeOut);

/*POR�˲�*/
//void SYS_PORFilterCmd(FuncState state);

/*���ߡ����ѡ�ʱ���л�*/
void SYS_FallSleep(void);
void SYS_SetWakeInterval(uint32_t nInterval);
void Switch2PLL(void);
void Switch2HRC(void);
void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, uint8_t ena);

/*����8·ʱ��ʹ��*/
void SYS_ModuleClockCmd(uint32_t nModule, FuncState state);

/*����ģ��ģ��ʱ��ʹ��*/
void SYS_AnalogModuleClockCmd(uint32_t nModule, FuncState state);
    
/*��λ��ģ��*/
void SYS_SoftResetModule(uint32_t nModule);

/*IAP_MODE*/
//�������IAP_MODE������λ�Ժ�

/*DSPģ���ʼ��*/
void DSP_Init(void);

/* MCU�����ѹ�ȶ��жϺ���*/
void SYS_VolSelModule(uint32_t Vol);

/* MCU�����ѹ�ȶ��ж��жϺ���*/
void SYS_VolSelModuleIRQ(void);

/* MCU�����ѹ�ȶ��ж��ж�ʹ�ܺ���*/
void SYS_VolSelModuleEnableIRQ(uint32_t Vol);
#endif /*__lks32mc08x_SYS_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
