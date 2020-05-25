/*
 * Copyright (c) 2020 XuPeng Zhang <zxp1428@126.com>
 * All rights reserved
 */

#ifndef _ZBITSVIEW_H_
#define _ZBITSVIEW_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif 

/*************************用户定义区***************************/

/*********添加头文件************/
#include "nokia5110.h"
/*******************************/

/******更改显示函数宏定义*******/
//static uint8_t keyCode[][8] = {
//  {0,1,2,3,4,5,6,7},
//  {0,1,2,3,4,5,6,7},
//  {0,1,2,3,4,5,6,7},
//};

#define DISPLAY_STARTROW            1           //从第几行开始显示
#define DISPLAY_MAXLINES            5           //显示行数[1~屏幕最大显示行数]

#if 1                                           //选中某行时的显示效果
#define SELECT_ROW_STYLE            1           //在选中行最前面占用一个字符显示箭头
#else
#define SELECT_ROW_STYLE            2           //将选中行进行反色处理
#endif 

#define LCD_ClearLine(y)            LCD_ClearLine(y)                //清除某一行函数(一般实现是使某一行全部显示空格)
#define LCD_DrawArrow(x,y)          LCD_ShowChar(x,y,'>')           //画箭头函数
#define LCD_String(x,y,string)      LCD_ShowString(x,y,string)      //显示字符串函数
#define LCD_String_Inverse(x,y,str) LCD_ShowString_Inverse(x,y,str) //反色输出第y行从x坐标到行尾的所有
/*******************************/

/**************************************************************/

#define ROW_LENGTH(x)			          ((uint16_t)(sizeof(x)/sizeof(RowListTypeDef)))
#define VIEW_MEMBER_FORMAT(x)       {x,ROW_LENGTH(x),0,0,0}
#define VIEW_MEMBER_LENGTH(x)		    ((uint16_t)(sizeof(x)/sizeof(ViewListTypeDef)))	
	
#define KEY_NONE						        0						//没有按下按键
#define KEY_ENTER					          1						//按下<确定>键
#define KEY_RETURN					        2						//按下<返回>键(返回上一层)
#define KEY_HOME						        3						//按下<首页>键
#define KEY_DOWN						        4						//按下<下>键
#define KEY_UP							        5						//按下<上>键
#define KEY_ADD					            6						//按下<加>键
#define KEY_SUB						          7						//按下<减>键
          
#define VIEW_NONE                   UINT16_MAX  //当某一行元素没有指向的界面时,其enterViewIndex成员存储此值

/**
  * @brief  行元素调用的函数,用于显示屏幕及处理特定按键(在KEY_ADD和KEY_SUB)
  * @param  index: 指向此函数的RowListTypeDef在数组中的下标
  * @param  p: 指向当前RowListTypeDef元素的text指针指向的字符串
  * @param  key: 若按下按键的值大于等于6(KEY_ADD),则此形参会是非0值(KEY_NONE);若小于6,则传入0(KEY_NONE)
  * @retval None
  */
typedef void(*HandlerFunc)(uint16_t index, char* p, uint8_t key);   

//游标,只需要定义一个即可    ==> 8字节(byte)
typedef struct {
	uint8_t currRow;			                        //当前指向元素
	uint8_t keyval;				                        //记录按键
  uint16_t currViewIndex;		                    //当前指向层
	uint16_t startRow;	                        	//屏幕第一行显示的行元素索引
	uint16_t rowNum;		                        	//记录当前层的行元素数
}CursorTypeDef;

//每层最多65536个行元素   ==> 12字节(byte)
typedef struct{
	uint16_t enterViewIndex;	                    //按下确定键跳转的界面[0~65534(VIEW_NONE-1)]
//  uint16_t reserve;                           //保留,扩展用/////////////////  
	char * text;					                        //当前行显示的文本
	HandlerFunc handler;	                        //显示函数
}RowListTypeDef;

//最多有65535层   ==> 12字节(byte)
//可使用VIEW_MEMBER_FORMAT(x)宏定义数组
typedef struct {
	const RowListTypeDef * const list;            //指向当前层所指向的行元素
	uint16_t lengthOfList;	                      //指向的行元素的长度
	uint16_t parentViewIndex;	                    //本元素所属层的标号
	uint16_t startRow;			                      //记录在上一层时的开始行索引
	uint8_t currRow;  			                      //记录在上一层时的行索引
//  uint8_t reserve;                           //保留,扩展用/////////////////  
}ViewListTypeDef;

/**
  * @brief  初始化UI
  * @param  v: 界面数组的指针
  * @param  c: 游标的指针
  * @retval None
  */
void View_Init(ViewListTypeDef * v, CursorTypeDef * c);

/**
  * @brief  界面循环,放在程序主循环中
  * @param  None
  * @retval None
  */
void View_Loop(void);

#ifdef __cplusplus
}
#endif 

#endif //_ZBITSVIEW_H_
