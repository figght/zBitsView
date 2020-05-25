#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

#define  DWT_CR      *(uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(uint32_t *)0xE0001004
#define  DEM_CR      *(uint32_t *)0xE000EDFC
#define  DBGMCU_CR   *(uint32_t *)0xE0042004

#define  DEM_CR_TRCENA                   (1 << 24)

#define  DWT_CR_CYCCNTENA                (1 <<  0)

//����ϵͳ��શ�ʱ��
void SysTick_Init(void);
//����ϵͳʱ���������
void BSP_TimeStapInit(void);
void Delay_us(uint32_t us);
void Delay_ms(unsigned int ms);
void Delay_s(unsigned int ms);
void TimingDelay_Decrement(void);
void _TimeStap_APP(void);
//��ȡ�ӿ��������ڵ�ʱ���,ÿ�����һ
uint32_t BSP_GetMilliTimeStap(void);
//��ȡ�ӿ��������ھ�����ʱ���źŸ���[72Mhzʱ��59.652s������]
uint32_t BSP_GetTimeStap(void);

#endif /* __SYSTICK_H */
