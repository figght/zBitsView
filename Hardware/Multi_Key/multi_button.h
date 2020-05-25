/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */
 
#ifndef _MULTI_BUTTON_H_
#define _MULTI_BUTTON_H_

#include "stdint.h"
#include "string.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    5	//ms		//执行按键检测函数的时间间隔
#define DEBOUNCE_TICKS    3	//MAX 8
#define SHORT_TICKS       (300 /TICKS_INTERVAL)	//
#define LONG_TICKS        (1000 /TICKS_INTERVAL)	//长按最小时间


typedef void (*BtnCallback)(void*);

typedef enum {
	BTN_PRESS_DOWN = 0,		//按下即为
	BTN_PRESS_UP,
	BTN_PRESS_REPEAT,
	BTN_SINGLE_CLICK,
	BTN_DOUBLE_CLICK,
	BTN_LONG_RRESS_START,
	BTN_LONG_PRESS_HOLD,
	BTN_number_of_event,	//7
	BTN_NONE_PRESS
}PressEvent;

typedef struct Button {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3; 		//
	uint8_t  active_level : 1;		//记录上次检测时按键的状态
	uint8_t  button_level : 1;		//按下时按键的状态
	uint8_t  (*hal_button_Level)(void);
	BtnCallback  cb[BTN_number_of_event];
	struct Button* next;
}Button;

#ifdef __cplusplus  
extern "C" {  
#endif  

void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);//每个Button模组执行需要146~147个时钟,大约2.028us

#ifdef __cplusplus
} 
#endif

#endif
