/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_wake.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� wake����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0    <td>HuangMG    <td>����
 * </table>
 */

#ifndef __lks32mc07x_WAKE__
#define __lks32mc07x_WAKE__
#include "lks32mc07x.h"


/** 
 *@brief �͹���ʱ�������м����
 */
typedef struct
{
  uint32_t AFE_REG5_RECORD;
  uint32_t CLK_CFG_RECORD;
	
}WAKE_InitTypeDef;

/** 
 *@brief �͹���IO���ѵ�ƽö�ٳ�Ա
 */
typedef enum
{
	WAKETRIG_LOW,  /**����IO�͵�ƽ����*/
	WAKETRIG_HIGH, /**����IO�ߵ�ƽ����*/
}enumWakeTrigLevel;



/*���߻��Ѽ��ʱ��*/
#define SYS_WakeInterval_025s  0x00     /**< ���߻��Ѽ��ʱ��0.25s*/  
#define SYS_WakeInterval_05s   0x01     /**< ���߻��Ѽ��ʱ��0.5s*/
#define SYS_WakeInterval_1s    0x02     /**< ���߻��Ѽ��ʱ��1s*/
#define SYS_WakeInterval_2s    0x03     /**< ���߻��Ѽ��ʱ��2s*/
#define SYS_WakeInterval_4s    0x04     /**< ���߻��Ѽ��ʱ��4s*/
#define SYS_WakeInterval_8s    0x05     /**< ���߻��Ѽ��ʱ��8s*/
#define SYS_WakeInterval_16s   0x06     /**< ���߻��Ѽ��ʱ��16s*/
#define SYS_WakeInterval_32s   0x07     /**< ���߻��Ѽ��ʱ��32s*/

/*�͹��Ļ���IO�˲�*/
#define IOWK_FLT_EN    0x02 /**< ʹ��IO�˲�*/
#define IOWK_FLT_DI    0x00 /**< �ر�IO�˲�*/

/*�͹��Ļ���IOѡ��*/
#define WAKEIO_P0_0	     BIT0   /**< P0.0 ��Ϊ����IO*/
#define WAKEIO_P0_2	     BIT1   /**< P0.2 ��Ϊ����IO*/
#define WAKEIO_P0_6	     BIT2   /**< P0.6 ��Ϊ����IO*/
#define WAKEIO_P0_11	   BIT3   /**< P0.11��Ϊ����IO*/
#define WAKEIO_P0_14     BIT4   /**< P0.14��Ϊ����IO*/
#define WAKEIO_P2_4	     BIT5   /**< P2.4 ��Ϊ����IO*/
#define WAKEIO_P2_7	     BIT6   /**< P2.7 ��Ϊ����IO*/
#define WAKEIO_P2_15	   BIT7   /**< P2.15 ��Ϊ����IO*/
#define WAKEIO_CLUOUT0   BIT8   /**< CLUOUT0 ��Ϊ����ʹ��*/
#define WAKEIO_CLUOUT1	 BIT9   /**< CLUOUT1 ��Ϊ����ʹ��*/
#define WAKEIO_CLUOUT2   BIT10  /**< CLUOUT2 ��Ϊ����ʹ��*/
#define WAKEIO_CLUOUT3	 BIT11  /**< CLUOUT3 ��Ϊ����ʹ��*/


/*���ߡ�����*/
void SYS_EVTCLRSleep(void);
void SYS_FallSleep(void);
void SYS_SetWakeInterval(uint32_t rth , uint32_t wth , uint8_t ena);
void Switch2PLL(void);
void Switch2HRC(void);
void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, uint32_t flt , uint8_t ena);






#endif




