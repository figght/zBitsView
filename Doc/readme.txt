# 屏幕菜单

- 时间: 2020-3-18
- 作者: 张续鹏   

## 功能

实现了一个多级菜单控件

## 简介

1. 编写环境：Keil μVision 5
2. 硬件设备：STM32F103C8T6、Nokia 5110屏幕，EC11旋转编码器
3. 本界面控件理论上不限制屏幕、不限制单片机型号（未验证）
4. 移植方便

## 代码分析

### 数据结构

1. 行元素结构体

```c
typedef struct{
	uint16_t enterViewIndex;//按下确定键跳转的界面
	char * text;			//当前行显示的文本
	HandlerFunc handler;	//显示函数
}RowListTypeDef;
```

数组最大长度：65536
每个元素占用12字节
因为此数据不需要修改，所以可使用`const`或`code(C51)`修饰

HandlerFunc是函数指针，此函数即可作为行元素的显示函数，又可作为按键处理函数，其类型为：

> ```c
> typedef void(*HandlerFunc)(uint16_t index, char* p, uint8_t key);   
> ```
>
> 三个形参的作用分别是:
>
> > @param  index: 指向此函数的RowListTypeDef在数组中的下标
> >
> > @param  p: 指向当前RowListTypeDef元素的text指针指向的字符串
> >
> > @param  key: 若按下按键的值大于等于6(KEY_ADD),则此形参会是非0值(KEY_NONE);若小于6,则传入0(KEY_NONE)

2. 界面结构体

```c
typedef struct {
	const RowListTypeDef * const list;//指向当前层所指向的行元素
	uint16_t lengthOfList;		//指向的行元素的长度
	uint16_t parentViewIndex;	//本元素所属层的标号
	uint16_t startRow; //记录在上一层时的开始行索引
	uint8_t currRow;   //记录在上一层时的行索引
}ViewListTypeDef;
```

数组最大长度：65535（0~65534）	——	65535代表RowListTypeDef元素没有指向的ViewListTypeDef元素

每个元素占用12字节

定义ViewListTypeDef型数组是可以使用`VIEW_MEMBER_FORMAT(x)`帮助编写；如：

> ```C
> ViewListTypeDef menu[] = {
> 	VIEW_MEMBER_FORMAT(rowListHome),
> 	VIEW_MEMBER_FORMAT(rowListSettingRoot),
> 	VIEW_MEMBER_FORMAT(rowListView1),
> 	VIEW_MEMBER_FORMAT(rowListView2),
> 	VIEW_MEMBER_FORMAT(rowListView3),
> VIEW_MEMBER_FORMAT(rowListView1_1),
> };
> ```
>
> 其中`VIEW_MEMBER_FORMAT`宏定义为
>
> ```c
> #define ROW_LENGTH(x)			          ((uint16_t)(sizeof(x)/sizeof(RowListTypeDef)))
> #define VIEW_MEMBER_FORMAT(x)       {x,ROW_LENGTH(x),0,0,0}
> ```

3. 游标结构体

```c
//游标,只需要定义一个即可    ==> 8字节(byte)
typedef struct {
	uint8_t currRow;			                        //当前指向元素
	uint8_t keyval;				                        //记录按键
  uint16_t currViewIndex;		                    //当前指向层
	uint16_t startRow;	                        	//屏幕第一行显示的行元素索引
	uint16_t rowNum;		                        	//记录当前层的行元素数
}CursorTypeDef;
```

### 函数作用

本控件函数很少，只有两个，即：

1. `void View_Init(ViewListTypeDef * v, CursorTypeDef * c)`

此函数的作用是初始化界面控件：将用户定义好的`ViewListTypeDef`数组的地址和`CursorTypeDef`地址初始化到控件

2. `void View_Loop(void)`

此函数作用是在处理界面数据。注意：**需要将此函数放入主循环中，每隔一段时间调用一次**

> 间隔时间典型值是100ms。
>
> 注意：并不是本控件消耗的时间多，而是屏幕驱动程序消耗的时间太多，本人的屏幕驱动是模拟的SPI时序而不是单片机硬件SPI，故屏幕驱动消耗的时间太多。控件每次需要不到1000个机器周期，而驱动程序是其上百倍。
>
> 若使用硬件外设驱动屏幕，则可以将间隔时间适当调小一点，同时注意不要低于屏幕刷新周期。

### 界面设计

设计界面时只需要定义几个数组即可。

首先定义`RowListTypeDef`类型数组，根据界面数定义数组个数，根据每个界面包含的行元素数定义每个数组的长度。

然后定义`ViewListTypeDef`类型数组，定义一个即可，数组长度是界面数决定的。

例如，设计这样一个界面：

> ```mermaid
> graph LR
> 
> home{home}
> root{root}
> v1{view1}
> v2{view2}
> v3{view3}
> v1_1{view1-1}
> 
> rowhome(rowHome)
> home-->rowhome
> rowhome-.->root
> 
> r1(row1)
> r2(row2)
> r3(row3)
> root-->r1
> root-->r2
> root-->r3
> r1-.->v1
> r2-.->v2
> r3-.->v3
> r1_1-.->v1_1
> 
> r1_1(row1-1)
> r1_2(row1-2)
> r1_3(row1-3)
> r1_4(row1-4)
> r1_5(row1-5)
> r1_6(row1-6)
> r1_7(row1-7)
> r1_8(row1-8)
> r1_9(row1-9)
> v1-->r1_1 
> v1-->r1_2
> v1-->r1_3
> v1-->r1_4 
> v1-->r1_5
> v1-->r1_6
> v1-->r1_7 
> v1-->r1_8
> v1-->r1_9
> 
> 
> r2_1(row2-1)
> r2_2(row2-2)
> r2_3(row2-3)
> r2_4(row2-4)
> r2_5(row2-5)
> r2_6(row2-6)
> r2_7(row2-7)
> r2_8(row2-8)
> v2-->r2_1 
> v2-->r2_2
> v2-->r2_3
> v2-->r2_4 
> v2-->r2_5
> v2-->r2_6
> v2-->r2_7 
> v2-->r2_8
> 
> r3_1(row3-1)
> r3_2(row3-2)
> r3_3(row3-3)
> r3_4(row3-4)
> r3_5(row3-5)
> r3_6(row3-6)
> r3_7(row3-7)
> r3_8(row3-8)
> r3_9(row3-9)
> r3_10(row3-10)
> r3_11(row3-11)
> r3_12(row3-12)
> r3_13(row3-13)
> r3_14(row3-14)
> r3_15(row3-15)
> v3-->r3_1
> v3-->r3_2
> v3-->r3_3
> v3-->r3_4
> v3-->r3_5
> v3-->r3_6
> v3-->r3_7
> v3-->r3_8
> v3-->r3_9
> v3-->r3_10
> v3-->r3_11
> v3-->r3_12
> v3-->r3_13
> v3-->r3_14
> v3-->r3_15
> 
> r1_1_1(row1-1-1)
> r1_1_2(row1-1-2)
> r1_1_3(row1-1-3)
> r1_1_4(row1-1-4)
> r1_1_5(row1-1-5)
> r1_1_6(row1-1-6)
> r1_1_7(row1-1-7)
> r1_1_8(row1-1-8)
> v1_1-->r1_1_1
> v1_1-->r1_1_2
> v1_1-->r1_1_3
> v1_1-->r1_1_4
> v1_1-->r1_1_5
> v1_1-->r1_1_6
> v1_1-->r1_1_7
> v1_1-->r1_1_8
> 
> ```

则需要这样定义数组

```c
const RowListTypeDef rowListHome[] = {  
	//{.enterViewIndex  |  .x  |  .text  |  .handler},
  {1,"home",NULL},
};

const RowListTypeDef rowListSRoot[] = {  
	//{.enterViewIndex  |  .x  |  .text  |  .handler},
	{2,"Row 1",NULL},
	{3,"Row 2",NULL},
	{4,"Row 3",NULL},
};

const RowListTypeDef rowListView1[] = {  
	//{.enterViewIndex  |  .x  |  .text  |  .handler},
	{5,"Row 1-1",NULL}, 
	{VIEW_NONE,"Row 1-2",NULL},
	{VIEW_NONE,"Row 1-3",NULL},
	{VIEW_NONE,"Row 1-4",NULL},
	{VIEW_NONE,"Row 1-5",NULL},
	{VIEW_NONE,"Row 1-6",NULL},
  {VIEW_NONE,"Row 1-7",NULL},
  {VIEW_NONE,"Row 1-8",NULL},
  {VIEW_NONE,"Row 1-9",NULL},
};

const RowListTypeDef rowListView2[] = {  
	//{.enterViewIndex  |  .x  |  .text  |  .handler},
	{VIEW_NONE,"Row 2-1",NULL},
	{VIEW_NONE,"Row 2-2",NULL},
	{VIEW_NONE,"Row 2-3",NULL},
	{VIEW_NONE,"Row 2-4",NULL},
	{VIEW_NONE,"Row 2-5",NULL},
  {VIEW_NONE,"Row 2-6",NULL},
  {VIEW_NONE,"Row 2-7",NULL},
  {VIEW_NONE,"Row 2-8",NULL},
};

const RowListTypeDef rowListView3[] = {  
	//{.enterViewIndex  |  .x  |  .text  |  .handler},
	{VIEW_NONE,"Row 3-1",NULL},
	{VIEW_NONE,"Row 3-2",NULL},
	{VIEW_NONE,"Row 3-3",NULL},
	{VIEW_NONE,"Row 3-4",NULL},
	{VIEW_NONE,"Row 3-5",NULL},
	{VIEW_NONE,"Row 3-6",NULL},
  {VIEW_NONE,"Row 3-7",NULL},
  {VIEW_NONE,"Row 3-8",NULL},
  {VIEW_NONE,"Row 3-9",NULL},
  {VIEW_NONE,"Row 3-10",NULL},
  {VIEW_NONE,"Row 3-11",NULL},
  {VIEW_NONE,"Row 3-12",NULL},
  {VIEW_NONE,"Row 3-13",NULL},
  {VIEW_NONE,"Row 3-14",NULL},
  {VIEW_NONE,"Row 3-15",NULL},
};

const RowListTypeDef rowListView1_1[] = {  
	//{.enterViewIndex  |  .x  |  .text  |  .handler},
	{VIEW_NONE,"Row 1-1-1",NULL}, 
	{VIEW_NONE,"Row 1-1-2",NULL},
	{VIEW_NONE,"Row 1-1-3",NULL},
	{VIEW_NONE,"Row 1-1-4",NULL},
	{VIEW_NONE,"Row 1-1-5",NULL},
  {VIEW_NONE,"Row 1-1-6",NULL},
	{VIEW_NONE,"Row 1-1-7",NULL},
  {VIEW_NONE,"Row 1-1-8",NULL},
};

ViewListTypeDef menu[] = {
	//.currIndex | .parentViewIndex | .list | .lengthOfList | .display
	VIEW_MEMBER_FORMAT(rowListHome),
	VIEW_MEMBER_FORMAT(rowListSRoot),
	VIEW_MEMBER_FORMAT(rowListView1),
	VIEW_MEMBER_FORMAT(rowListView2),
	VIEW_MEMBER_FORMAT(rowListView3),
  VIEW_MEMBER_FORMAT(rowListView1_1),
};
```

### 程序格式

程序需要定义一个全局变量游标`CursorTypeDef`，如：

```c
CursorTypeDef cursor;
```

然后在main函数中调用控件初始化程序`View_Init`

```c
View_Init(menu,&cursor);
```

在程序主循环中每隔一段时间调用程序`View_Loop`.例如每隔100ms调用一次

当有按键按下时,只需要根据按键的不同给`cursor.keyval`变量赋不同的值即可.例如:

```c
rotaryval = ReadRotaryEncoder();
if(rotaryval == ROTARY_LEFT)
{
    cursor.keyval = KEY_UP;
}else if(rotaryval == ROTARY_RIGHT)
{
    cursor.keyval = KEY_DOWN;
}
```

其中按键值有以下:

```c
#define KEY_NONE						        0						//没有按下按键
#define KEY_ENTER					          1						//按下<确定>键
#define KEY_RETURN					        2						//按下<返回>键(返回上一层)
#define KEY_HOME						        3						//按下<首页>键
#define KEY_DOWN						        4						//按下<下>键
#define KEY_UP							        5						//按下<上>键
#define KEY_ADD					            6						//按下<加>键
#define KEY_SUB						          7						//按下<减>键
```





