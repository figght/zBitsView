#include "rotaryEncoder.h"

void RotaryEncoder_Init(void)
{
	GPIO_InitTypeDef				GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(	CLK_CALU(RoyEcd_PORT) |
													CLK_CALU(RoyEcd_SW_PORT),ENABLE);
	
	GPIO_InitStruct.GPIO_Pin =	RoyEcd_CLK_PIN |
															RoyEcd_DAT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RoyEcd_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = RoyEcd_SW_PIN;
	GPIO_Init(RoyEcd_SW_PORT,&GPIO_InitStruct);
}

unsigned char ReadRotaryEncoder(void)
{
	static char Enc0=0,Enc1=0;
	static unsigned char EncOld,EncX=0;
	unsigned char EncNow;
//	char EncType=0; //设置编码器类型: 0为一定位一脉冲,1为两定位一脉冲

	if(Enc0==0)EncOld=(Read_RoyEcd_CLK()?0x02:0x00)+(Read_RoyEcd_DAT()?0x01:0x00),Enc0=1;  //记住初次调用时编码器的状态
	EncNow=(Read_RoyEcd_CLK()?0x02:0x00)+(Read_RoyEcd_DAT()?0x01:0x00);        //根据两个IO当前状态组合成16进制的0x00|0x01|0x02|0x03
	if(EncNow == EncOld)
		return(0);                                //如果新数据和原来的数据一样(没有转动)就直接返回0

	if(EncOld==0x00&&EncNow==0x02||EncOld==0x03&&EncNow==0x01)
		EncX=EncNow;         //00-10|11-01
	if(EncOld==0x00&&EncX==0x02&&EncNow==0x03||EncOld==0x03&&EncX==0x01&&EncNow==0x00)//00-10-11|11-01-00右转
	{
			EncOld=EncNow,EncX=0;
			if(EncType==1)return('R');   //两定位一脉冲
			else       //一定位一脉冲
			{
					if(Enc1==0)Enc1=1;
					else
					{
							//Delayms(60);       //延时降低旋转灵敏度(不能用在定时器中)
							Enc1=0;
							return('R');
					}
			}
	}

	if(EncOld==0x00&&EncNow==0x01||EncOld==0x03&&EncNow==0x02)EncX=EncNow;         //00-01|11-10
	if(EncOld==0x00&&EncX==0x01&&EncNow==0x03||EncOld==0x03&&EncX==0x02&&EncNow==0x00)//00-01-11|11-10-00左转
	{
			EncOld=EncNow,EncX=0;
			if(EncType==1)return('L');   //两定位一脉冲
			else           //一定位一脉冲
			{
					if(Enc1==0)Enc1=1;
					else
					{
							//Delayms(60);       //延时降低旋转灵敏度(不能用在定时器中)
							Enc1=0;
							return('L');
					}
			}
	}
	return(0);           //没有正确解码返回0
}


/**
* 旋转编码器按键检测函数
*/
uint8_t Read_RoyEcd_SW()
{
	return GPIO_ReadInputDataBit(RoyEcd_SW_PORT,RoyEcd_SW_PIN);
}

