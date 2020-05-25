#include "nokia5110.h"
#include "fonts.h"

/*--------------------------5110驱动------------------------------*/
/********************写字节**************************/
void LCD_write_byte_cmd(unsigned char dt)  //写字节
{
	unsigned char i;
	
	LCD5110_CE_CLR();    //5110片选有效，允许输入数据
	LCD5110_DC_CLR();      //DC为低电平写指令，高电平写数据
	for(i=0;i<8;i++)   //传送8bit数据
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
	LCD5110_CE_SET();     //禁止5110
	LCD5110_Din_SET();
}

void LCD_write_byte_data(unsigned char dt)
{
	unsigned char i;
	
	LCD5110_CE_CLR();    //5110片选有效，允许输入数据
	LCD5110_DC_SET(); 	//DC为低电平写指令，高电平写数据
	for(i=0;i<8;i++)   //传送8bit数据
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
	LCD5110_CE_SET();     //禁止5110
	LCD5110_Din_SET();
}

static void Nokia5110_GPIO_Init(void)
{
	GPIO_InitTypeDef			GPIO_InitStruct;
	
		//要用到仿真口PA15  所以关掉JTAG仿真功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	  //启动AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //配置JTAG-DP Disabled and SW-DP Enabled 
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); //JTAG and SWD 全部禁用
	
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

/*******************初始化********************************************/
void LCD_init(void)  //初始化（command=0，写指令）
{
	Nokia5110_GPIO_Init();
	
	//Delay_ms(10);
	
  LCD5110_RST_CLR();   
  Delay_ms(1);
  LCD5110_RST_SET(); 
	
  LCD_write_byte_cmd(0x21);//初始化Lcd,功能设定使用扩充指令
  LCD_write_byte_cmd(0xc7);//设定液晶偏置电压(5v)，5110接5V时偏置电压改为（0xbb,0）,接3.3V时改为（0xc0,0）
  LCD_write_byte_cmd(0x20);//使用基本指令
  LCD_write_byte_cmd(0x0c);//设定显示模式，正常显示
}

/*********************设置初始坐标************************************/
void LCD_set_XY(unsigned char X, unsigned char Y)  //设置LCD坐标，X:0-83 Y:0-5
{
	LCD_write_byte_cmd(0x40 | Y);// column  //列
	LCD_write_byte_cmd(0x80 | X);// row   //行
}
/**********************清屏函数***************************************/
void LCD_clear(void)    //LCD清屏函数 
{
  unsigned char t;
  unsigned char k;
  LCD_set_XY(0,0);   //设置RAM起始地址
  for(t=0;t<6;t++)
  {
    for(k=0;k<84;k++)
    {
      LCD_write_byte_data(0x00);    //写数据
    }
  }
}

void LCD_ClearLine(unsigned char y)
{
  LCD_set_XY(0,y);   //设置RAM起始地址
  for(y=0;y<84;y++)
  {
    LCD_write_byte_data(0x00);    //写数据
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

//在特定位置画点
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




