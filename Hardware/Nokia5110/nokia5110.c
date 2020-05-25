#include "nokia5110.h"
#include "fonts.h"

/*--------------------------5110����------------------------------*/
/********************д�ֽ�**************************/
void LCD_write_byte_cmd(unsigned char dt)  //д�ֽ�
{
	unsigned char i;
	
	LCD5110_CE_CLR();    //5110Ƭѡ��Ч��������������
	LCD5110_DC_CLR();      //DCΪ�͵�ƽдָ��ߵ�ƽд����
	for(i=0;i<8;i++)   //����8bit����
	{
		if(dt&0x80)
			LCD5110_Din_SET();
		else
			LCD5110_Din_CLR();
		dt=dt<<1;  
		LCD5110_Clk_CLR();   
		LCD5110_Clk_SET();
	} 
	LCD5110_DC_SET(); 
	LCD5110_CE_SET();     //��ֹ5110
	LCD5110_Din_SET();
}

void LCD_write_byte_data(unsigned char dt)
{
	unsigned char i;
	
	LCD5110_CE_CLR();    //5110Ƭѡ��Ч��������������
	LCD5110_DC_SET(); 	//DCΪ�͵�ƽдָ��ߵ�ƽд����
	for(i=0;i<8;i++)   //����8bit����
	{
		if(dt&0x80)
			LCD5110_Din_SET();
		else
			LCD5110_Din_CLR();
		dt=dt<<1;  
		LCD5110_Clk_CLR();   
		LCD5110_Clk_SET();
	} 
	LCD5110_DC_SET(); 
	LCD5110_CE_SET();     //��ֹ5110
	LCD5110_Din_SET();
}

static void Nokia5110_GPIO_Init(void)
{
	GPIO_InitTypeDef			GPIO_InitStruct;
	
		//Ҫ�õ������PA15  ���Թص�JTAG���湦��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	  //����AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //����JTAG-DP Disabled and SW-DP Enabled 
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); //JTAG and SWD ȫ������
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = 	LCD5110_RST_PIN	|
															LCD5110_CE_PIN	|
															LCD5110_DC_PIN	|
															LCD5110_Din_PIN	|
															LCD5110_Clk_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

/*******************��ʼ��********************************************/
void LCD_init(void)  //��ʼ����command=0��дָ�
{
	Nokia5110_GPIO_Init();
	
	//Delay_ms(10);
	
  LCD5110_RST_CLR();   
  Delay_ms(1);
  LCD5110_RST_SET(); 
	
  LCD_write_byte_cmd(0x21);//��ʼ��Lcd,�����趨ʹ������ָ��
  LCD_write_byte_cmd(0xc7);//�趨Һ��ƫ�õ�ѹ(5v)��5110��5Vʱƫ�õ�ѹ��Ϊ��0xbb,0��,��3.3Vʱ��Ϊ��0xc0,0��
  LCD_write_byte_cmd(0x20);//ʹ�û���ָ��
  LCD_write_byte_cmd(0x0c);//�趨��ʾģʽ��������ʾ
}

/*********************���ó�ʼ����************************************/
void LCD_set_XY(unsigned char X, unsigned char Y)  //����LCD���꣬X:0-83 Y:0-5
{
	LCD_write_byte_cmd(0x40 | Y);// column  //��
	LCD_write_byte_cmd(0x80 | X);// row   //��
}
/**********************��������***************************************/
void LCD_clear(void)    //LCD�������� 
{
  unsigned char t;
  unsigned char k;
  LCD_set_XY(0,0);   //����RAM��ʼ��ַ
  for(t=0;t<6;t++)
  {
    for(k=0;k<84;k++)
    {
      LCD_write_byte_data(0x00);    //д����
    }
  }
}

void LCD_ClearLine(unsigned char y)
{
  LCD_set_XY(0,y);   //����RAM��ʼ��ַ
  for(y=0;y<84;y++)
  {
    LCD_write_byte_data(0x00);    //д����
  }
}

void LCD_ShowChar(unsigned char x, unsigned char y, char ch)
{
	unsigned char i;
  
  LCD_set_XY(x*6, y);
	ch -= ' ';
	
	for(i = 0;i<6;i++)
	{
		LCD_write_byte_data(FontsAnscii6X8[ch][i]);
	}
}

void LCD_ShowChar_Inverse(unsigned char x, unsigned char y, char ch)
{
	unsigned char i;
  
  LCD_set_XY(x*6, y);
	ch -= ' ';
	
	for(i = 0;i<6;i++)
	{
		LCD_write_byte_data(~FontsAnscii6X8[ch][i]);
	}
}

static void ShowChar(char ch)
{
	unsigned char i;
	ch -= ' ';
	
	for(i = 0;i<6;i++)
	{
		LCD_write_byte_data(FontsAnscii6X8[ch][i]);
	}
}

void LCD_ShowString(unsigned char x, unsigned char y, char *p)
{
	LCD_set_XY(x*6, y);
	
	while(*p)
	{
		ShowChar(*p++);
	}
}

static void ShowChar_Inverse(char ch)
{
	unsigned char i;
	ch -= ' ';
	
	for(i = 0;i<6;i++)
	{
		LCD_write_byte_data(~FontsAnscii6X8[ch][i]);
	}
}

void LCD_ShowString_Inverse(unsigned char x, unsigned char y, char *p)
{
  uint8_t cnt = 0;
	LCD_set_XY(x*6, y);
	
	while(*p)
	{
    cnt++;
		ShowChar_Inverse(*p++);
	}
  while(cnt++ < LCD_NOKIA5110_ROW)
  {
    ShowChar_Inverse(' ');
  }
}

#ifdef LCD_NOKIA5110_USE_INVERSION

static void LCD_ShowCharInverion(char ch)
{
	unsigned char i;
	ch -= ' ';
	
	for(i = 0;i<6;i++)
	{
		LCD_write_byte_data(FontAnsciiInver6X8[ch][i]);
	}
}

void LCD_ShowStringInversion(unsigned char x, unsigned char y, char *p,unsigned int inver)
{
	unsigned char i;
	
	LCD_set_XY(x*6, y);
	
	for(i = 0;p[i];i++)
	{
		if((inver<<i)&0x8000)
		{
			LCD_ShowCharInverion(p[i]);
		}
		else
		{
			ShowChar(p[i]);
		}
	}
}

#endif //#ifdef LCD_NOKIA5110_USE_INVERSION

//���ض�λ�û���
void LCD_Draw(unsigned char x,unsigned char y)
{
	
	LCD_set_XY(x*6, y);
	
	for(x = 0;x<6;x++)
	{
		LCD_write_byte_data(FontsAnscii6X8[95][x]);
	}
}

//void LCD_ShowChinese(unsigned char x,unsigned char y,unsigned char ch)
//{
//	unsigned char i;
//	
//	LCD_set_XY(x*6, y);
//	for(i=0; i<12;i++)
//	{
//		LCD_write_byte_data(FontsChinese12X12[ch][i]);
//	}
//	
//	LCD_set_XY(x*6, y+1);
//	for(i=12; i<24;i++)
//	{
//		LCD_write_byte_data(FontsChinese12X12[ch][i]);
//	}
//}




