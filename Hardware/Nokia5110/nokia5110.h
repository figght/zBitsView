#ifndef _NOKIA5110_H_
#define _NOKIA5110_H_

#include "main.h"

#define LCD_NOKIA5110_ROW							14
#define LCD_NOKIA5110_COL							6

//若想使用翻转阳码字体,则将此行取消注释,否则,注释之
#define LCD_NOKIA5110_USE_INVERSION		1

#ifdef LCD_NOKIA5110_USE_INVERSION
#define LCD_INVERSION_BIT							0x10000
#endif //LCD_NOKIA5110_USE_INVERSION

#define LCD5110_RST_PIN								GPIO_Pin_9
#define LCD5110_CE_PIN 	              GPIO_Pin_7
#define LCD5110_DC_PIN	              GPIO_Pin_5
#define LCD5110_Din_PIN	              GPIO_Pin_3
#define LCD5110_Clk_PIN								GPIO_Pin_1	

#define LCD5110_RST_SET()							GPIO_SetBits(GPIOB,LCD5110_RST_PIN)
#define LCD5110_CE_SET() 							GPIO_SetBits(GPIOB,LCD5110_CE_PIN )
#define LCD5110_DC_SET()							GPIO_SetBits(GPIOB,LCD5110_DC_PIN	)
#define LCD5110_Din_SET()							GPIO_SetBits(GPIOB,LCD5110_Din_PIN)
#define LCD5110_Clk_SET()							GPIO_SetBits(GPIOB,LCD5110_Clk_PIN)
		
#define LCD5110_RST_CLR()							GPIO_ResetBits(GPIOB,LCD5110_RST_PIN)	
#define LCD5110_CE_CLR() 							GPIO_ResetBits(GPIOB,LCD5110_CE_PIN )
#define LCD5110_DC_CLR()							GPIO_ResetBits(GPIOB,LCD5110_DC_PIN	)
#define LCD5110_Din_CLR()							GPIO_ResetBits(GPIOB,LCD5110_Din_PIN)
#define LCD5110_Clk_CLR()							GPIO_ResetBits(GPIOB,LCD5110_Clk_PIN)


void LCD_init(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_clear(void);
void LCD_ClearLine(unsigned char y);
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c);
void LCD_write_zi(unsigned char row, unsigned char page,unsigned char c);
void LCD_ShowChar(unsigned char x, unsigned char y, char ch);
void LCD_ShowChar_Inverse(unsigned char x, unsigned char y, char ch);
void LCD_ShowString(unsigned char x, unsigned char y, char *p);
void LCD_ShowString_Inverse(unsigned char x, unsigned char y, char *p);
//void LCD_ShowChinese(unsigned char x,unsigned char y,unsigned char ch);
void LCD_Draw(unsigned char x,unsigned char y);

#ifdef LCD_NOKIA5110_USE_INVERSION
void LCD_ShowStringInversion(unsigned char x, unsigned char y, char *p,unsigned int inver);
#endif //#ifdef LCD_NOKIA5110_USE_INVERSION

#endif //_NOKIA5110_H_
