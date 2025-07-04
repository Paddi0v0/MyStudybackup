#ifndef HARDINIT_H_
#define HARDINIT_H_
#include "lks32mc08x.h"

#define MCU_POWER_3v3  0 //MCU 3.3V����
#define MCU_POWER_5v0  1 //MCU 5V����

#define POWER_MODE     MCU_POWER_5v0 


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

void Clock_Init(void);
void SystemInit(void);

void InitCom(void);
void SYS_VolSelModuleIRQ(void);
void SYS_VolSelModuleEnableIRQ(uint32_t Vol);
#endif
