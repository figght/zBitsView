#ifndef _ROTARYENCODER_H_
#define _ROTARYENCODER_H_

#include "main.h"

#define EncType									1		 //设置编码器类型: 0为一定位一脉冲,1为两定位一脉冲		

#define RoyEcd_PORT							GPIOB
#define RoyEcd_CLK_PIN					GPIO_Pin_2
#define RoyEcd_DAT_PIN					GPIO_Pin_0
#define RoyEcd_SW_PORT					GPIOC
#define RoyEcd_SW_PIN						GPIO_Pin_13

#define Read_RoyEcd_CLK()				(GPIO_ReadInputDataBit(RoyEcd_PORT,RoyEcd_CLK_PIN) == Bit_RESET)
#define Read_RoyEcd_DAT()				(GPIO_ReadInputDataBit(RoyEcd_PORT,RoyEcd_DAT_PIN) == Bit_RESET)
//#define Read_RoyEcd_SW()				(GPIO_ReadInputDataBit(RoyEcd_SW_PORT,RoyEcd_SW_PIN) == Bit_RESET)

#define RoyEcd_SW_True					Bit_RESET
#define RoyEcd_SW_False					(!RoyEcd_SW_True)

#define ROTARY_LEFT								'L'
#define ROTARY_RIGHT							'R'

void RotaryEncoder_Init(void);
unsigned char ReadRotaryEncoder(void);
uint8_t Read_RoyEcd_SW(void);

#endif //_ROTARYENCODER_H_
