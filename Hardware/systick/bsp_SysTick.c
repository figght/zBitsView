/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SysTick ϵͳ�δ�ʱ��1ms�жϺ�����,�ж�ʱ����������ã�
  *          ���õ��� 1us 10us 1ms �жϡ�     
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-MINI STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_SysTick.h"

static volatile uint32_t _Sys_TimeStapVal_ = 0;
static volatile uint32_t TimingDelay = 0;
 
/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick
  * @param  ��
  * @retval ��
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}



/**
  * @brief   ms��ʱ����,1msΪһ����λ
  * @param  
  *		@arg nTime: Delay_ms( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1ms = 1ms
  * @retval  ��
  */
void Delay_ms(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}

/**
  * @brief   1s��ʱ����
  * @param  
  *		@arg nTime: Delay_s( 1 ) ��ʵ�ֵ���ʱΪ 1 000* 1ms = 1s
  * @retval  ��
  */
void Delay_s(unsigned int ms)
{ 
	unsigned char i;
	for(i=0;i<ms;i++)
		Delay_ms(1000);
}


/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

//SysTick�жϺ�������,ÿ�ε���,������ʱ�����һ
void _TimeStap_APP()
{
	_Sys_TimeStapVal_++;
}

//��ȡ�ӿ��������ڵ�ʱ���,ÿ�����һ
uint32_t	BSP_GetMilliTimeStap()
{
	return (uint32_t)_Sys_TimeStapVal_;
}

static int SYSCLK = 0;

//����ϵͳʱ���������
void BSP_TimeStapInit(void)
{
	/* Enable Cortex-M3's DWT CYCCNT reg. */
	DEM_CR         |= (uint32_t)DEM_CR_TRCENA;                
	DWT_CYCCNT      = (uint32_t)0u;
	DWT_CR         |= (uint32_t)DWT_CR_CYCCNTENA;
	
	SYSCLK = SystemCoreClock; // ���浱ǰϵͳ��ʱ�����ڣ�eg. 72,000,000(72MHz). 
}

//��ȡ�ӿ�����ĳʱ������ʱ���ź���(72Mhzʱ��59.652s������)
uint32_t BSP_GetTimeStap(void)
{
	return ((uint32_t)DWT_CYCCNT);
}
 
// ΢����ʱ
void Delay_us(uint32_t us)
{
  uint32_t ticks_start,ticks_delay, ticks_end;
  
  ticks_start = DWT_CYCCNT;
  
  if ( !SYSCLK )
    BSP_TimeStapInit();
  
  ticks_delay = ( us * ( SYSCLK / (1000*1000) ) ); // ��΢��������ɵδ���          
  
  ticks_end = ticks_start + ticks_delay;
  
  if ( ticks_end > ticks_start )
  {
    while( DWT_CYCCNT < ticks_end );
  }
  else // �����������ת
  {
    while( DWT_CYCCNT >= ticks_end ); // ��ת���ֵ�����ticks_endС
    while( DWT_CYCCNT < ticks_end );
  }
}

/*********************************************END OF FILE**********************/
