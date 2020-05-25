/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SysTick 系统滴答时钟1ms中断函数库,中断时间可自由配置，
  *          常用的有 1us 10us 1ms 中断。     
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_SysTick.h"

static volatile uint32_t _Sys_TimeStapVal_ = 0;
static volatile uint32_t TimingDelay = 0;
 
/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}



/**
  * @brief   ms延时程序,1ms为一个单位
  * @param  
  *		@arg nTime: Delay_ms( 1 ) 则实现的延时为 1 * 1ms = 1ms
  * @retval  无
  */
void Delay_ms(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}

/**
  * @brief   1s延时程序
  * @param  
  *		@arg nTime: Delay_s( 1 ) 则实现的延时为 1 000* 1ms = 1s
  * @retval  无
  */
void Delay_s(unsigned int ms)
{ 
	unsigned char i;
	for(i=0;i<ms;i++)
		Delay_ms(1000);
}


/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

//SysTick中断函数调用,每次调用,将毫秒时间戳加一
void _TimeStap_APP()
{
	_Sys_TimeStapVal_++;
}

//获取从开机到现在的时间戳,每毫秒加一
uint32_t	BSP_GetMilliTimeStap()
{
	return (uint32_t)_Sys_TimeStapVal_;
}

static int SYSCLK = 0;

//开启系统时间戳计数器
void BSP_TimeStapInit(void)
{
	/* Enable Cortex-M3's DWT CYCCNT reg. */
	DEM_CR         |= (uint32_t)DEM_CR_TRCENA;                
	DWT_CYCCNT      = (uint32_t)0u;
	DWT_CR         |= (uint32_t)DWT_CR_CYCCNTENA;
	
	SYSCLK = SystemCoreClock; // 保存当前系统的时钟周期，eg. 72,000,000(72MHz). 
}

//获取从开机到某时经历的时钟信号数(72Mhz时钟59.652s溢出溢出)
uint32_t BSP_GetTimeStap(void)
{
	return ((uint32_t)DWT_CYCCNT);
}
 
// 微秒延时
void Delay_us(uint32_t us)
{
  uint32_t ticks_start,ticks_delay, ticks_end;
  
  ticks_start = DWT_CYCCNT;
  
  if ( !SYSCLK )
    BSP_TimeStapInit();
  
  ticks_delay = ( us * ( SYSCLK / (1000*1000) ) ); // 将微秒数换算成滴答数          
  
  ticks_end = ticks_start + ticks_delay;
  
  if ( ticks_end > ticks_start )
  {
    while( DWT_CYCCNT < ticks_end );
  }
  else // 计数溢出，翻转
  {
    while( DWT_CYCCNT >= ticks_end ); // 翻转后的值不会比ticks_end小
    while( DWT_CYCCNT < ticks_end );
  }
}

/*********************************************END OF FILE**********************/
