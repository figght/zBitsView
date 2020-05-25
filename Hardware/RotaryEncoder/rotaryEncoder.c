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
//	char EncType=0; //���ñ���������: 0Ϊһ��λһ����,1Ϊ����λһ����

	if(Enc0==0)EncOld=(Read_RoyEcd_CLK()?0x02:0x00)+(Read_RoyEcd_DAT()?0x01:0x00),Enc0=1;  //��ס���ε���ʱ��������״̬
	EncNow=(Read_RoyEcd_CLK()?0x02:0x00)+(Read_RoyEcd_DAT()?0x01:0x00);        //��������IO��ǰ״̬��ϳ�16���Ƶ�0x00|0x01|0x02|0x03
	if(EncNow == EncOld)
		return(0);                                //��������ݺ�ԭ��������һ��(û��ת��)��ֱ�ӷ���0

	if(EncOld==0x00&&EncNow==0x02||EncOld==0x03&&EncNow==0x01)
		EncX=EncNow;         //00-10|11-01
	if(EncOld==0x00&&EncX==0x02&&EncNow==0x03||EncOld==0x03&&EncX==0x01&&EncNow==0x00)//00-10-11|11-01-00��ת
	{
			EncOld=EncNow,EncX=0;
			if(EncType==1)return('R');   //����λһ����
			else       //һ��λһ����
			{
					if(Enc1==0)Enc1=1;
					else
					{
							//Delayms(60);       //��ʱ������ת������(�������ڶ�ʱ����)
							Enc1=0;
							return('R');
					}
			}
	}

	if(EncOld==0x00&&EncNow==0x01||EncOld==0x03&&EncNow==0x02)EncX=EncNow;         //00-01|11-10
	if(EncOld==0x00&&EncX==0x01&&EncNow==0x03||EncOld==0x03&&EncX==0x02&&EncNow==0x00)//00-01-11|11-10-00��ת
	{
			EncOld=EncNow,EncX=0;
			if(EncType==1)return('L');   //����λһ����
			else           //һ��λһ����
			{
					if(Enc1==0)Enc1=1;
					else
					{
							//Delayms(60);       //��ʱ������ת������(�������ڶ�ʱ����)
							Enc1=0;
							return('L');
					}
			}
	}
	return(0);           //û����ȷ���뷵��0
}


/**
* ��ת������������⺯��
*/
uint8_t Read_RoyEcd_SW()
{
	return GPIO_ReadInputDataBit(RoyEcd_SW_PORT,RoyEcd_SW_PIN);
}

