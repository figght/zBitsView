#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

#define  DWT_CR      *(uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(uint32_t *)0xE0001004
#define  DEM_CR      *(uint32_t *)0xE000EDFC
#define  DBGMCU_CR   *(uint32_t *)0xE0042004

#define  DEM_CR_TRCENA                   (1 << 24)

#define  DWT_CR_CYCCNTENA                (1 <<  0)

//开启系统嘀嗒定时器
void SysTick_Init(void);
//开启系统时间戳计数器
void BSP_TimeStapInit(void);
void Delay_us(uint32_t us);
void Delay_ms(unsigned int ms);
void Delay_s(unsigned int ms);
void TimingDelay_Decrement(void);
void _TimeStap_APP(void);
//获取从开机到现在的时间戳,每毫秒加一
uint32_t BSP_GetMilliTimeStap(void);
//获取从开机到现在经历的时钟信号个数[72Mhz时钟59.652s溢出溢出]
uint32_t BSP_GetTimeStap(void);

#endif /* __SYSTICK_H */
