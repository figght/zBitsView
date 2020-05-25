#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "stdio.h"

#define CLK_CALU(x)						((uint32_t) (0x01<<((((uint32_t)(x)&0xff00)>>8)/4)))
#ifndef MIN
#define MIN(x, y) ((((int32_t)(x)) < ((int32_t)(y))) ? (x) : (y))
#endif //min
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif //max
#ifndef ABS
#define ABS(X) ((X) > 0 ? (X) : -(X))
#endif

#endif //_MAIN_H_


