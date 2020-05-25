/*
 * Copyright (c) 2020 XuPeng Zhang <zxp1428@126.com>
 * All rights reserved
 */

#include "zBitsView.h"

static ViewListTypeDef *gpv, *gpvl;     
static CursorTypeDef *gpc;

//�����α������
void cursorSet(uint16_t layer, uint8_t row, uint16_t start)
{  
	gpc->currViewIndex = layer;
	gpc->currRow = row;
	gpc->startRow = start;
}

//��ʼ���α�
void cursorInit(CursorTypeDef * c)
{
	gpc = c;
	cursorSet(0,0,0);
	gpc->keyval = gpc->rowNum = 0;
}

//������
void cursorHandleKey(void)
{
	uint16_t temp = 0,returnFlag = 0; 
#if DISPLAY_MAXLINES == 1  
  int8_t gpc_currRow = gpc->currRow;
#endif  
	
	switch(gpc->keyval)
	{
		case KEY_ENTER:
			gpc->keyval = KEY_NONE;
      
      temp = gpv->list[gpc->currRow+gpc->startRow].enterViewIndex;
      if(temp != VIEW_NONE)    
      {
        //�ڴ���ת�Ľ������¼��״̬
        gpvl[temp].startRow = gpc->startRow;
        gpvl[temp].currRow = gpc->currRow;
        gpvl[temp].parentViewIndex = gpc->currViewIndex;
                
        cursorSet(temp,0,0);
      }
			break;
		case KEY_HOME://�ص�������
			gpc->keyval = KEY_NONE;
			cursorSet(0,0,0);
			break;
		case KEY_RETURN:
			gpc->keyval = KEY_NONE;
		
			//����ǰ�����е�״ֵ̬д�뵽������
			gpc->currRow = gpv->currRow;
			gpc->startRow = gpv->startRow;
			gpc->currViewIndex = gpv->parentViewIndex;
      
      gpv->currRow = gpv->startRow= gpv->parentViewIndex = 0;										//����(�Ǳ���)
      returnFlag = 0xa5a5;    //��־λ
			break;
		case KEY_DOWN:
			gpc->keyval = KEY_NONE;
#if DISPLAY_MAXLINES == 1
      if(gpc->startRow < (gpc->rowNum -1))
        gpc_currRow++;
#else
      if(gpc->currRow < MIN(DISPLAY_MAXLINES-1,gpc->rowNum-1))
        gpc->currRow++;
#endif      
			break;
		case KEY_UP:
			gpc->keyval = KEY_NONE;
#if DISPLAY_MAXLINES == 1    
      if(gpc->startRow > 0)
        gpc_currRow--;
#else
			if(gpc->currRow > 0)
				gpc->currRow--;
#endif    
			break;
		case KEY_ADD:
			gpc->keyval = KEY_NONE;
			temp = gpc->currRow+gpc->startRow;
			if(gpv->list[temp].handler != NULL)
				gpv->list[temp].handler(temp, gpv->list[temp].text, KEY_ADD);
			break;
		case KEY_SUB:
			gpc->keyval = KEY_NONE;
			temp = gpc->currRow+gpc->startRow;
			if(gpv->list[temp].handler != NULL)
				gpv->list[temp].handler(temp, gpv->list[temp].text, KEY_SUB);
			break;	
		default:
			break;
	}
  
  if(returnFlag == 0)//��ֹ���淵��ʱ��startRow��currRow�ı�
  {
#if DISPLAY_MAXLINES == 1       
    if(gpc_currRow == DISPLAY_MAXLINES && gpc->startRow < (gpc->rowNum) )
    {
      gpc->currRow = 0;
      gpc->startRow++;
    }else if(gpc_currRow == -1 && gpc->startRow > 0)
    {
      gpc->currRow = 0;
      gpc->startRow--;
    }
#else 
    if(gpc->currRow == (DISPLAY_MAXLINES-1) && gpc->startRow<(gpc->rowNum-DISPLAY_MAXLINES))
    {
      gpc->startRow++;
      gpc->currRow--;
    }else	if(gpc->currRow == 0 && (gpc->startRow > 0))
    {
      gpc->startRow--;
      gpc->currRow++;
    }
#endif
  }
}

void cursorKeySet(unsigned char key)
{
  gpc->keyval = key;
}

unsigned char cursorKeyGet(void)
{
  return (unsigned char)gpc->keyval;
}

void parseView(void)
{
	uint16_t i,loopLimit;
	
	//��ʾ   
  for(i = DISPLAY_STARTROW;i<(DISPLAY_STARTROW+DISPLAY_MAXLINES);i++)
    LCD_ClearLine(i);
#if (DISPLAY_MAXLINES != 1) || (SELECT_ROW_STYLE == 2)
	LCD_DrawArrow(0,gpc->currRow+DISPLAY_STARTROW);
#endif  
	loopLimit = (gpc->rowNum)<DISPLAY_MAXLINES?gpc->rowNum:(DISPLAY_MAXLINES+gpc->startRow);	//��ȡ���ѭ������
	for(i = gpc->startRow;i<loopLimit;i++)
	{
#if DISPLAY_MAXLINES == 1      
    if(gpv->list[i].handler != NULL)
			gpv->list[i].handler(gpc->currRow+gpc->startRow, gpv->list[i].text, KEY_NONE);
		else
			LCD_String(0,i-gpc->startRow+DISPLAY_STARTROW,gpv->list[i].text);
#else  
		if(gpv->list[i].handler != NULL)
			gpv->list[i].handler(gpc->currRow+gpc->startRow, gpv->list[i].text, KEY_NONE);
		else
    {
#if SELECT_ROW_STYLE == 1        
			LCD_String(1,i-gpc->startRow+DISPLAY_STARTROW,gpv->list[i].text);
#else
      if(i == gpc->currRow+gpc->startRow)
        LCD_String_Inverse(0,i-gpc->startRow+DISPLAY_STARTROW,gpv->list[i].text);
      else
        LCD_String(0,i-gpc->startRow+DISPLAY_STARTROW,gpv->list[i].text);
#endif
    }
#endif    
	} 
}

void View_Init(ViewListTypeDef * v, CursorTypeDef * c)
{	
	gpvl = v;
	cursorInit(c);
}

void View_Loop(void)
{
	gpv = &gpvl[gpc->currViewIndex];
  gpc->rowNum = gpv->lengthOfList;
  if(gpc->keyval != 0)
    cursorHandleKey();	//������
  parseView();//����ͼ��
}

