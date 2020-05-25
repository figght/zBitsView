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

/*************************�û�������***************************/

/*********���ͷ�ļ�************/
#include "nokia5110.h"
/*******************************/

/******������ʾ�����궨��*******/
//static uint8_t keyCode[][8] = {
//  {0,1,2,3,4,5,6,7},
//  {0,1,2,3,4,5,6,7},
//  {0,1,2,3,4,5,6,7},
//};

#define DISPLAY_STARTROW            1           //�ӵڼ��п�ʼ��ʾ
#define DISPLAY_MAXLINES            5           //��ʾ����[1~��Ļ�����ʾ����]

#if 1                                           //ѡ��ĳ��ʱ����ʾЧ��
#define SELECT_ROW_STYLE            1           //��ѡ������ǰ��ռ��һ���ַ���ʾ��ͷ
#else
#define SELECT_ROW_STYLE            2           //��ѡ���н��з�ɫ����
#endif 

#define LCD_ClearLine(y)            LCD_ClearLine(y)                //���ĳһ�к���(һ��ʵ����ʹĳһ��ȫ����ʾ�ո�)
#define LCD_DrawArrow(x,y)          LCD_ShowChar(x,y,'>')           //����ͷ����
#define LCD_String(x,y,string)      LCD_ShowString(x,y,string)      //��ʾ�ַ�������
#define LCD_String_Inverse(x,y,str) LCD_ShowString_Inverse(x,y,str) //��ɫ�����y�д�x���굽��β������
/*******************************/

/**************************************************************/

#define ROW_LENGTH(x)			          ((uint16_t)(sizeof(x)/sizeof(RowListTypeDef)))
#define VIEW_MEMBER_FORMAT(x)       {x,ROW_LENGTH(x),0,0,0}
#define VIEW_MEMBER_LENGTH(x)		    ((uint16_t)(sizeof(x)/sizeof(ViewListTypeDef)))	
	
#define KEY_NONE						        0						//û�а��°���
#define KEY_ENTER					          1						//����<ȷ��>��
#define KEY_RETURN					        2						//����<����>��(������һ��)
#define KEY_HOME						        3						//����<��ҳ>��
#define KEY_DOWN						        4						//����<��>��
#define KEY_UP							        5						//����<��>��
#define KEY_ADD					            6						//����<��>��
#define KEY_SUB						          7						//����<��>��
          
#define VIEW_NONE                   UINT16_MAX  //��ĳһ��Ԫ��û��ָ��Ľ���ʱ,��enterViewIndex��Ա�洢��ֵ

/**
  * @brief  ��Ԫ�ص��õĺ���,������ʾ��Ļ�������ض�����(��KEY_ADD��KEY_SUB)
  * @param  index: ָ��˺�����RowListTypeDef�������е��±�
  * @param  p: ָ��ǰRowListTypeDefԪ�ص�textָ��ָ����ַ���
  * @param  key: �����°�����ֵ���ڵ���6(KEY_ADD),����βλ��Ƿ�0ֵ(KEY_NONE);��С��6,����0(KEY_NONE)
  * @retval None
  */
typedef void(*HandlerFunc)(uint16_t index, char* p, uint8_t key);   

//�α�,ֻ��Ҫ����һ������    ==> 8�ֽ�(byte)
typedef struct {
	uint8_t currRow;			                        //��ǰָ��Ԫ��
	uint8_t keyval;				                        //��¼����
  uint16_t currViewIndex;		                    //��ǰָ���
	uint16_t startRow;	                        	//��Ļ��һ����ʾ����Ԫ������
	uint16_t rowNum;		                        	//��¼��ǰ�����Ԫ����
}CursorTypeDef;

//ÿ�����65536����Ԫ��   ==> 12�ֽ�(byte)
typedef struct{
	uint16_t enterViewIndex;	                    //����ȷ������ת�Ľ���[0~65534(VIEW_NONE-1)]
//  uint16_t reserve;                           //����,��չ��/////////////////  
	char * text;					                        //��ǰ����ʾ���ı�
	HandlerFunc handler;	                        //��ʾ����
}RowListTypeDef;

//�����65535��   ==> 12�ֽ�(byte)
//��ʹ��VIEW_MEMBER_FORMAT(x)�궨������
typedef struct {
	const RowListTypeDef * const list;            //ָ��ǰ����ָ�����Ԫ��
	uint16_t lengthOfList;	                      //ָ�����Ԫ�صĳ���
	uint16_t parentViewIndex;	                    //��Ԫ��������ı��
	uint16_t startRow;			                      //��¼����һ��ʱ�Ŀ�ʼ������
	uint8_t currRow;  			                      //��¼����һ��ʱ��������
//  uint8_t reserve;                           //����,��չ��/////////////////  
}ViewListTypeDef;

/**
  * @brief  ��ʼ��UI
  * @param  v: ���������ָ��
  * @param  c: �α��ָ��
  * @retval None
  */
void View_Init(ViewListTypeDef * v, CursorTypeDef * c);

/**
  * @brief  ����ѭ��,���ڳ�����ѭ����
  * @param  None
  * @retval None
  */
void View_Loop(void);

#ifdef __cplusplus
}
#endif 

#endif //_ZBITSVIEW_H_
