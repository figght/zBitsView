#include "main.h"
#include "nokia5110.h"
#include "rotaryEncoder.h"
#include "multi_button.h"
#include "zBitsView.h"

//////////////////////////////////////////////////////////////////////////

void RowHandler(uint16_t index, char *p, uint8_t key)
{
	static uint16_t cnt = 5;
	char str[50];
	
	switch(key)
	{
		case KEY_NONE:
		sprintf(str,"%s %d",p,(int)cnt);
		LCD_ShowString(1,1,str);
			break;
		case KEY_ADD:
			cnt++;
			break;
		case KEY_SUB:
			cnt--;
			break;
		default:
			break;
	}
}

const RowListTypeDef rowListHome[] = {  
	//{.enterViewIndex  |  .x  |  .text  |  .handler},
//	{1,"home",RowHandler},
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

CursorTypeDef cursor;
//////////////////////////////////////////////////////////////////////////

Button RoyEcdButton;

void LCD_ShowInfo(void);
double calu_mult10(double radix,uint16_t power);																//计算某数乘以power次10后的结果

void SW_SingleClick_Handler(void * btn)
{
	cursor.keyval = KEY_ENTER;
}

void SW_LongStart_Handler(void * btn)
{
	//Button * p = (Button *)btn;
	cursor.keyval = KEY_RETURN;
}
 
uint8_t gpiobool = 1;
uint16_t gcnt = 0;

int main(void)
{
	uint32_t timestap = 0,lastTimeStap = 0; 
	uint8_t EventCnt = 0,rotaryval = 0;
	
  SysTick_Init();
	BSP_TimeStapInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);

  LCD_init();
  LCD_clear();
  RotaryEncoder_Init();
	
	button_init(&RoyEcdButton,Read_RoyEcd_SW,RoyEcd_SW_True);
	button_attach(&RoyEcdButton,BTN_SINGLE_CLICK,SW_SingleClick_Handler);
	button_attach(&RoyEcdButton,BTN_LONG_RRESS_START,SW_LongStart_Handler);
	button_start(&RoyEcdButton);
  
	View_Init(menu,&cursor);
		
  while(1)
  {
		timestap = BSP_GetMilliTimeStap();
		
		rotaryval = ReadRotaryEncoder();
		if(rotaryval == ROTARY_LEFT)
		{
			cursor.keyval = KEY_UP;
		}else if(rotaryval == ROTARY_RIGHT)
		{
			cursor.keyval = KEY_DOWN;
		}
    		
		if(timestap - lastTimeStap >= 5)																						//每5ms执行一次
		{
			lastTimeStap = timestap;
			button_ticks();																														//5ms执行一次扫描按键程序
			EventCnt++;
			if(EventCnt%20 == 1)																											//每40*5=200ms刷新一次屏幕
			{
        //ITM_SendChar('A');
				LCD_ShowInfo(); 
			}
		}
  }
}

void LCD_ShowInfo(void)
{
	char showbuff[50],i;
	uint32_t ts;
	  
	ts = BSP_GetMilliTimeStap()/1000;

	sprintf(showbuff,"Time:%3d:%02d:%02d",(ts/60/60)%999,(ts/60)%60,ts%60);				//显示开机时间"Time:HHH:MM:SS"
	LCD_ShowString(0,0,showbuff);
	View_Loop();
}  


