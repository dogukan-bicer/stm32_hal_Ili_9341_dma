/*
 * LCD_Interface_Library.c
 *
 *  Created on: 18 Ara 2021
 *      Author: Kadir AK
 */

/* Private includes ----------------------------------------------------------*/
#include "LCD_Hardware_Library.h"
#include "LCD_Interface_Library.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private  ---------------------------------------------------------*/
volatile int MenuNum = 0;
volatile int SelectItemIndex = 0;
volatile int SelectItemNum = 0;
volatile int MaxSelectItems = 0;
volatile int SelectCase = 0;
volatile int SelectItemValue = 0;
static uint8_t value[9];
static uint8_t result[9];
//static uint8_t time_value[2];
//static uint8_t time_result[2];

//
extern int32_t refValue ,rrmsRefValue;
extern int32_t gasValue ,rrmsGasValue;
extern uint32_t SensorValueFiltered;
//extern FilterTypeDef filterStruct;
//


// SineGraph Graph Variables
int Graph_x1=40,Graph_x2=290,Graph_y1=150,Graph_y2=200;
float Graph_x=0.0, Graph_y=0.0;
int Graph_value=0;
int Graph_k=50;


void DefaultVariablesSet()
{
	// Desktop Variables
	DesktopVariables.Time_Hour 		= 0;
	DesktopVariables.Time_Minute 	= 0;
	DesktopVariables.Alarm_Status 	= 0;
	DesktopVariables.Battery_Status = 0;
	DesktopVariables.Wifi_Status	= 0;
	DesktopVariables.Co2_Unit 		= 0;



		// Co2 Variables
		 Co2.EtCo2Alar_H			= 30;
		Co2.EtCo2Alar_L			= 15;
		Co2.RespAlar_H 			= 30;
		 Co2.RespAlar_L			= 10;
		 Co2.Apnea_Time			= 30;   // sn
		 Co2.Co2_Unit				= 0;	// 0:mmHg	1:%		2:KPA
		Co2.Wave_Scale			= 0;	// 0:54mmHg		1:76mmHg
		Co2.EtCo2Alar_H_EE				= 1;
		Co2.EtCo2Alar_L_EE				= 2;
		Co2.RespAlar_H_EE 				= 3;
		Co2.RespAlar_L_EE				= 4;
		Co2.Apnea_Time_EE				= 5;
		Co2.Co2_Unit_EE					= 6;
		Co2.Wave_Scale_EE				= 7;

		value[0]=Co2.EtCo2Alar_H;

//	}



	// Time Variables
	Time.Year 						= 2020;
	Time.Month						= 12;
	Time.Date						= 25;
	Time.Hour						= 12;
	Time.Minute						= 30;

	// Wifi Variables
	strcpy((char*)&Wifi.Name, "Wifi Adi");		// Wifi Adı
	strcpy((char*)&Wifi.Key,  "Wifi Sifresi");	// Wifi Şifresi
	Wifi.ConnectionStatus			= 0;		// Bağlantı Durumu
	Wifi.Status						= 0;		// Wifi Durumu Açık/Kapalı
	Wifi.DataTransmissionStatus		= 0;		// Wifi Veri gönderim Durumu

	// WarningSystem Variables
	strcpy((char*)&WarningSystem.Warning[1].Name, "Wifi is not connected");		// Alarm Adı
	WarningSystem.Warning[1].Priority		= 5;							// Alarm Önceliği
	WarningSystem.Warning[1].LedColor		= COLOR_RED;					// Alarm Led
	WarningSystem.Warning[1].WarningStatus	= 0;							// Uyarı Durumu
	WarningSystem.Warning[1].Sound        	= 740;							// Alarm Sesi
	WarningSystem.BuzzerStatus  			= 1; 							// Buzzer Durumu
	WarningSystem.SilentModeRemainingTime 	= 0;							// Buzzer Sesizse ise kalan süre

	// Graphic Variables
	Graphic.X1			= 40;
	Graphic.X2			= 290;
	Graphic.Y1			= 120;
	Graphic.Y2			= 220;
	Graphic.Value		= 0;
	Graphic.GraphicArea = Graphic.X2 - Graphic.X1;
	Graphic.Counter 	= Graphic.X1;
	Graphic.Mode 		= 1; 	// 	0: Histogram 1: Line
	Graphic.Unit 		= 0;    //	mmHg % kPA

	// Save Variables
	Save.Mode				= 10;
	Save.RawData			= 20;
	Save.RollingRms			= 30;
	Save.GasConcentration	= 40;
	Save.SaveFrameTime		= 50;

}

void Open_LCD_Set()
{
	// Variable Set
	MenuNum = 0;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 0;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	// Backlight On
	HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_SET);

	LCD_DrawImage(0, 0, logo_denizler.width, logo_denizler.height, logo_denizler.data);
	//int denizler_data=&logo_denizler.data;
	HAL_Delay(1000);
	//EE_Data_Array[0] = 0;
	DefaultVariablesSet();
	Desktop_Menu_Set();
}

void Close_LCD_Set()
{
	// Variable Set
	MenuNum = 0;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 0;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear
	LCD_DrawImage(0, 0, logo_denizler.width, logo_denizler.height, logo_denizler.data);
	HAL_Delay(1000);
	LCD_Fill_Color(COLOR_BLACK); // Screen Clear
	// Backlight Off
	HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_RESET);
}

void Desktop_Menu_Set()
{
	// Variable Set
	MenuNum = 0;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 0;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(0, 5, "22:22", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); 	// Time
	LCD_WriteString(10, 30, "EtCO2", 	Font_16x26, COLOR_GREEN, COLOR_BLACK); 	// EtCO2

	LCD_DrawImage(240, 5, notifications_on.width, 	notifications_on.height, 	notifications_on.data);
	LCD_DrawImage(260, 5, notifications_off.width, 	notifications_off.height, 	notifications_off.data);
	LCD_DrawImage(280, 5, wifi_off.width, 			wifi_off.height, 			wifi_off.data);
	LCD_DrawImage(300, 5, battery_on.width, 		battery_on.height, 			battery_on.data);

	LCD_WriteString(50,  70,"mmHg", Font_11x18, COLOR_WHITE, COLOR_BLACK); 	// mmHg
	LCD_WriteString(50, 100,  "84",	Font_16x26, COLOR_WHITE, COLOR_BLACK); 	// Value 1
	LCD_WriteString(250, 70,  "RR", Font_11x18, COLOR_WHITE, COLOR_BLACK); 	// RR
	LCD_WriteString(250,100,  "18", Font_16x26, COLOR_WHITE, COLOR_BLACK); 	// Value 2

	// Graphic Area
	LCD_DrawLine(29, 125, 29, 225, COLOR_MAGENTA);
	LCD_DrawLine(30, 125, 30, 225, COLOR_MAGENTA);
	LCD_DrawLine(30, 224,300, 224, COLOR_MAGENTA);
	LCD_DrawLine(30, 225,300, 225, COLOR_MAGENTA);
	for(int k=200;k<205;k+=1)
	{
		LCD_DrawLine(40, k,290, k, COLOR_GREEN);
	}
}

void Desktop_Update()
{
	HAL_IWDG_Refresh(&hiwdg);
	if(MenuNum == 0)
	{
		Time_Update();
		Icons_Update();
		Co2_Unit_Update();
		Wave_Scale_Update();
		//EtCo2_Update();
		Resp_Update();
		Variable_Update();
	}
	else
	{
		Variable_Update();
	}
}

//-----------------------------------------------------------------------------------------------------------------------//
//----------------------------- 		  SET FUNCTIONS	 START	    -----------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//
void Main_Menu_Set() 			// MenuNum 1
{
	// Variable Set
	MenuNum = 1;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 6;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(50, 10,  "MAIN MENU", 	Font_16x26, COLOR_LIGHTBLUE, COLOR_BLACK);	// MenuNum 1
	LCD_WriteString(10, 50,  "CO2", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);		// MenuNum 2
	LCD_WriteString(10, 70,  "TIME", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);		// MenuNum 3
	LCD_WriteString(10, 90,  "WIFI", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);		// MenuNum 4
	LCD_WriteString(10, 110, "ALARM", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);		// MenuNum 5
	LCD_WriteString(10, 130, "GRAPH", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);		// MenuNum 6
	LCD_WriteString(10, 150, "SAVE", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);		// MenuNum 7
	LCD_WriteString(10, 170, "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);		// MenuNum 0
}
//-----------------------------------------------------------------------------------------------------------------------//
void Co2_Menu_Set() 			// MenuNum 2
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////menu will read values when first opened
		ee_read(0, 7,result);
	    Co2.Apnea_Time=result[0];
	    Co2.Co2_Unit=result[1];
	    Co2.Wave_Scale=result[2];
	    Co2.EtCo2Alar_H=result[3];
	    Co2.EtCo2Alar_L=result[4];
	    Co2.RespAlar_H=result[5];
	    Co2.RespAlar_L=result[6];


////////////////////////////////////////////////////////////////////////////////////////////////////////////////menu will read values when first opened


	// Variable Set
	char Vrb[10]="     ";
	MenuNum = 2;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 9;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(50, 10,  "CO2 MENU", 		Font_16x26, COLOR_LIGHTBLUE, COLOR_BLACK);
	LCD_WriteString(10, 40,  "EtCO2", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	//EtCO2 -> 1:ALAR H 2:ALAR L
	LCD_WriteString(80, 40,  "ALAR H",			Font_11x18, COLOR_WHITE, COLOR_BLACK);	 	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_H, Vrb); LCD_WriteString(180, 40, Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);
	LCD_WriteString(80, 60,  "ALAR L", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);		memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_L, Vrb);	LCD_WriteString(180, 60, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 80,  "RESP", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	//RESP -> 1:ALAR H 2:ALAR L
	LCD_WriteString(80, 80,  "ALAR H", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);		memset(Vrb, 0, 10); int2char(Co2.RespAlar_H, Vrb);	LCD_WriteString(180, 80,  Vrb,  Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(80, 100, "ALAR L", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);		memset(Vrb, 0, 10); int2char(Co2.RespAlar_L, Vrb);	LCD_WriteString(180, 100, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 120, "APNEA TIME", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	    memset(Vrb, 0, 10);	int2char(Co2.Apnea_Time, Vrb);	LCD_WriteString(180, 120, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 140, "CO2 UNIT",		Font_11x18, COLOR_GREEN, COLOR_BLACK);		memset(Vrb, 0, 10);	int2char(Co2.Co2_Unit, Vrb);	LCD_WriteString(180, 140, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 160, "ZERO",			Font_11x18, COLOR_GREEN, COLOR_BLACK);		memset(Vrb, 0, 10); int2char(Co2.Wave_Scale, Vrb);	LCD_WriteString(180, 180, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 180, "WAVE SCALE", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 200, "LOAD DEFAULTS",	Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 220, "EXIT", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);
}
//-----------------------------------------------------------------------------------------------------------------------//
void Time_Menu_Set() 			// MenuNum 3
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////menu will read values when first opened
	        Time.Year=flash_read(0x08080800);
	        Time.Month=flash_read(0x08090000);
	        Time.Date=flash_read(0x08090800);
	        Time.Hour=flash_read(0x080A0000);
	        Time.Minute=flash_read(0x080A0800);




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////menu will read values when first opened





	// Variable Set
	char Vrb[10]="     ";
	MenuNum = 3;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 6;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(50, 10,  "TIME MENU",Font_16x26, COLOR_LIGHTBLUE, COLOR_BLACK);
	LCD_WriteString(10, 40,  "YEAR", 	 Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Year,  Vrb); 	LCD_WriteString(80, 40,  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
	LCD_WriteString(10, 60,  "MONTH", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Month, Vrb); 	LCD_WriteString(80, 60,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 80,  "DATE",	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Time.Date,  Vrb); 	LCD_WriteString(80, 80,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 100, "HOUR", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Hour,  Vrb);	LCD_WriteString(80, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 120, "MINUTE", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Minute,Vrb);	LCD_WriteString(80, 120, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 140, "SAVE", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 160, "EXIT", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);
}
//-----------------------------------------------------------------------------------------------------------------------//
void Wifi_Menu_Set() 			// MenuNum 4
{
	// Variable Set
	char Vrb[10]="     ";
	MenuNum = 4;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 5;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(50, 10,  "WIFI MENU", 		Font_16x26, COLOR_LIGHTBLUE, COLOR_BLACK);
	LCD_WriteString(10, 40,  "WIFI NAME",		Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 40, (char*)&Wifi.Name, Font_11x18, COLOR_WHITE, COLOR_BLACK);
	LCD_WriteString(10, 60,  "WIFI KEY", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 60, (char*)&Wifi.Key,  Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 80,  "WIFI CONNECT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 80,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 100, "CLOUD CONNECT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 120, "WIFI", 			Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.Status,Vrb);				LCD_WriteString(160, 120, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 140, "EXIT", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);
}
//-----------------------------------------------------------------------------------------------------------------------//
void Alarm_Menu_Set() 			// MenuNum 5
{
	// Variable Set
	// char Vrb[10]="     ";
	MenuNum = 5;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 2;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(50, 10,  "ALARM MENU", 	Font_16x26, COLOR_LIGHTBLUE, COLOR_BLACK);
	LCD_WriteString(10, 40,  "1",			Font_11x18, COLOR_WHITE, COLOR_BLACK); LCD_WriteString(50, 40, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_WHITE, COLOR_BLACK);
	LCD_WriteString(10, 60,  "2",			Font_11x18, COLOR_GREEN, COLOR_BLACK); LCD_WriteString(50, 60, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 80,  "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
}
//-----------------------------------------------------------------------------------------------------------------------//
void Graph_Menu_Set() 			// MenuNum 6
{
	// Variable Set
	char Vrb[10]="     ";
	MenuNum = 6;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 2;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(50, 10,"GRAPH MENU", 	Font_16x26, COLOR_LIGHTBLUE, COLOR_BLACK);
	LCD_WriteString(10, 40,      "MODE",	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Mode,  Vrb); LCD_WriteString(120, 40,  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
	LCD_WriteString(10, 60,  "CO2 UNIT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Unit,  Vrb); LCD_WriteString(120, 60,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 80,      "EXIT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK);
}
//-----------------------------------------------------------------------------------------------------------------------//
void Save_Menu_Set() 			// MenuNum 7
{
	// Variable Set
	char Vrb[10]="     ";
	MenuNum = 7;
	SelectItemIndex = 0;
	SelectItemNum = 0;
	MaxSelectItems = 4;
	SelectCase = 0;

	LCD_Fill_Color(COLOR_BLACK); // Screen Clear

	LCD_WriteString(50, 10,  "SAVE MENU", 		Font_16x26, COLOR_LIGHTBLUE, COLOR_BLACK);
	LCD_WriteString(10, 40,  "Mode", 			Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.Mode, 			Vrb); LCD_WriteString(200, 40,  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
	LCD_WriteString(10, 60,  "RawData", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RawData, 			Vrb); LCD_WriteString(200, 60,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 80,  "RollingRms", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RollingRms, 		Vrb); LCD_WriteString(200, 80,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 100, "GasConcentration",Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.GasConcentration, Vrb); LCD_WriteString(200, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
	LCD_WriteString(10, 120, "EXIT", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);
}
void Exit_Menu_Set() 			// MenuNum 0
{
	Desktop_Menu_Set();
}
//-----------------------------------------------------------------------------------------------------------------------//
//----------------------------- 		  SET FUNCTIONS	END		    -----------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//
void Event_Select(uint8_t Select)
{
	if 		(SelectCase == 0){	Menu_Select(Select);				}
	else if	(SelectCase == 1){ 	Value_Change_Variables(Select);	}
}

void Menu_Select(uint8_t Select)
{
	if (Select == 0) 	{ SelectItemIndex++; if (SelectItemIndex>MaxSelectItems)SelectItemIndex=0;				}// Down
	else if(Select == 1){ SelectItemIndex--; if (SelectItemIndex<0)				SelectItemIndex=MaxSelectItems; }//	Up

	switch (MenuNum)
	{
	case 0: {										break;} // Desktop
	case 1:	{ Main_Menu_Select(SelectItemIndex);	break;}
	case 2:	{ Co2_Menu_Select(SelectItemIndex);		break;}
	case 3:	{ Time_Menu_Select(SelectItemIndex);	break;}
	case 4:	{ Wifi_Menu_Select(SelectItemIndex);	break;}
	case 5:	{ Alarm_Menu_Select(SelectItemIndex);	break;}
	case 6:	{ Graph_Menu_Select(SelectItemIndex);	break;}
	case 7:	{ Save_Menu_Select(SelectItemIndex);	break;}
	}
}
void Value_Change_Variables(uint8_t Select)
{
	switch (MenuNum)
	{
		case 0:
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{  break;}
			case 1:	{  break;}
			case 2:	{  break;}
			}
			break;}
		case 1: // Main Menus Set Open
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{  break;}
			case 1:	{  break;}
			case 2:	{  break;}
			}
			break;}
		case 2:	// Value Change of CO2 Variables
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{ Co2_EtCo2Alar_H_Set_Open(Select); break;}
			case 1:	{ Co2_EtCo2Alar_L_Set_Open(Select); break;}
			case 2:	{ Co2_RespAlar_H_Set_Open(Select);	break;}
			case 3:	{ Co2_RespAlar_L_Set_Open(Select); 	break;}
			case 4:	{ Co2_Apnea_Set_Open(Select);		break;}
			case 5: { Co2_Co2_Unit_Set_Open(Select);	break;}
			case 6:	{									break;}	// ZERO
			case 7: { Co2_Wave_Scale_Set_Open(Select);	break;}
			case 8:	{									break;}	// Load Defaults
			case 9:	{ Exit_Menu_Set();					break;}
			}
		break;}
		case 3: // Value Change of Time Variables
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{ Time_Year_Set_Open(Select);		break;}
			case 1:	{ Time_Month_Set_Open(Select);		break;}
			case 2:	{ Time_Date_Set_Open(Select);		break;}
			case 3:	{ Time_Hour_Set_Open(Select);		break;}
			case 4:	{ Time_Minute_Set_Open(Select);		break;}
			}
		break;}
		case 4: // Value Change of Wifi Variables
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{   break;}
			case 1:	{   break;}
			case 2:	{  	break;}
			}
		break;}
		case 5: // Value Change of Alarm Variables
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{   break;}
			case 1:	{   break;}
			case 2:	{  	break;}
			}
		break;}
		case 6: // Value Change of Graph Variables
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{   break;}
			case 1:	{   break;}
			case 2:	{  	break;}
			}
		break;}
		case 7: // Value Change of Save Variables
		{
			switch (SelectItemIndex) // 0: Down 1: Up 2: Set
			{
			case 0:	{   break;}
			case 1:	{   break;}
			case 2:	{  	break;}
			}
		break;}
	}
}

void Select_Event_Set()
{
	if (SelectCase == 0)			//------------------------------------------------------------------------------//
	{
		switch (MenuNum)
		{
		case 0:
		{
			Main_Menu_Set();
		break;}
		case 1: //	Main Menu Select Item Set
		{
			switch (SelectItemIndex)
			{
			case 0:	{Co2_Menu_Set();				break;}
			case 1:	{Time_Menu_Set();				break;}
			case 2:	{Wifi_Menu_Set();				break;}
			case 3:	{Alarm_Menu_Set();				break;}
			case 4:	{Graph_Menu_Set();				break;}
			case 5:	{Save_Menu_Set();				break;}
			case 6:	{Exit_Menu_Set();				break;}
			}
		break;}
		case 2: //	Co2 Menu Set
		{
			switch (SelectItemIndex)// 0: Down 1: Up 2: Set
			{
			case 0:	{ Co2_EtCo2Alar_H_Set_Open(2);	break;}
			case 1:	{ Co2_EtCo2Alar_L_Set_Open(2); 	break;}
			case 2:	{ Co2_RespAlar_H_Set_Open(2);  	break;}
			case 3:	{ Co2_RespAlar_L_Set_Open(2);  	break;}
			case 4:	{ Co2_Apnea_Set_Open(2);	   	break;}
			case 5:	{ Co2_Co2_Unit_Set_Open(2);	   	break;}
			case 6:	{							   	break;}
			case 7:	{ Co2_Wave_Scale_Set_Open(2);  	break;}
			case 8:	{							   	break;}
			case 9:	{ Exit_Menu_Set();	   		   	break;}
			}
		break;}
		case 3: //	Time Menu Set
		{
			switch (SelectItemIndex)// 0: Down 1: Up 2: Set
			{
			case 0:	{ Time_Year_Set_Open(2);		break;}
			case 1:	{ Time_Month_Set_Open(2);		break;}
			case 2:	{ Time_Date_Set_Open(2);		break;}
			case 3:	{ Time_Hour_Set_Open(2);		break;}
			case 4:	{ Time_Minute_Set_Open(2);		break;}
			case 5:	{								break;}
			case 6:	{ Exit_Menu_Set();				break;}
			}
		break;}
		case 4: //	Wifi Menu Set
		{
			switch (SelectItemIndex)// 0: Down 1: Up 2: Set
			{
			case 0:	{ 								break;}
			case 1:	{ 								break;}
			case 2:	{ 								break;}
			case 3:	{ 								break;}
			case 4:	{ 								break;}
			case 5:	{ Exit_Menu_Set();				break;}
			}
		break;}
		case 5:  //	Alarm Menu Set
		{
			switch (SelectItemIndex)// 0: Down 1: Up 2: Set
			{
			case 0:	{ 								break;}
			case 1:	{ 								break;}
			case 2:	{ Exit_Menu_Set();				break;}
			}
		break;}
		case 6: //	Graph Menu Set
		{
			switch (SelectItemIndex)// 0: Down 1: Up 2: Set
			{
			case 0:	{ 								break;}
			case 1:	{ 								break;}
			case 2:	{ Exit_Menu_Set();				break;}
			}
		break;}
		case 7: //	Save Menu Set
		{
			switch (SelectItemIndex)// 0: Down 1: Up 2: Set
			{
			case 0:	{ 								break;}
			case 1:	{ 								break;}
			case 3:	{ 								break;}
			case 4:	{ Exit_Menu_Set();				break;}
			}
		break;}
		}
	}else if(SelectCase == 1)		//------------------------------------------------------------------------------//
	{
		switch (MenuNum)
		{
		case 0:
		{
		break;}
		case 1: //	Main Menu Set Close
		{
		break;}
		case 2: //	Co2 Menu Set Close
		{
			switch (SelectItemIndex)
			{
			case 0:	{ Co2_EtCo2Alar_H_Set_Close();	break;}
			case 1:	{ Co2_EtCo2Alar_L_Set_Close();	break;}
			case 2:	{ Co2_RespAlar_H_Set_Close();	break;}
			case 3:	{ Co2_RespAlar_L_Set_Close();	break;}
			case 4:	{ Co2_Apnea_Set_Close();		break;}
			case 5:	{ Co2_Co2_Unit_Set_Close();		break;}
			case 6:	{								break;} // Zero
			case 7:	{ Co2_Wave_Scale_Set_Close();	break;}
			case 8:	{								break;} // Load Defaults
			case 9:	{ Exit_Menu_Set();				break;}
			}
		break;}
		case 3: //	Time Menu Set Close
		{
			switch (SelectItemIndex)
			{
			case 0:	{ Time_Year_Set_Close();  		break;}
			case 1:	{ Time_Month_Set_Close();		break;}
			case 2:	{ Time_Date_Set_Close();		break;}
			case 3:	{ Time_Hour_Set_Close();		break;}
			case 4:	{ Time_Minute_Set_Close();		break;}
			case 5:	{								break;} // Save
			case 6:	{ Exit_Menu_Set();				break;}	// Exit
			}
		break;}
		}
	}									//------------------------------------------------------------------------------//
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  MAIN MENU FUNCTIONS START  --------------------------
// ------------------------------------------------------------------------------------------------------
void Main_Menu_Select(uint8_t Index)
{
	// Variable Set
	MenuNum = 1;
	SelectItemNum = Index;

	switch (SelectItemNum)
	{
	case 0: {
		LCD_WriteString(10, 170, "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 50,  "CO2", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);  // MenuNum 2
		LCD_WriteString(10, 70,  "TIME", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 1: {
		LCD_WriteString(10, 50,  "CO2", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 70,  "TIME", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);	// MenuNum 3
		LCD_WriteString(10, 90,  "WIFI", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	break;}
	case 2:	{
		LCD_WriteString(10, 70,  "TIME", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 90,  "WIFI", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);	// MenuNum 4
		LCD_WriteString(10, 110, "ALARM", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	break;}
	case 3:	{
		LCD_WriteString(10, 90,  "WIFI", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 110, "ALARM", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);	// MenuNum 5
		LCD_WriteString(10, 130, "GRAPH", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	break;}
	case 4:	{
		LCD_WriteString(10, 110, "ALARM", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 130, "GRAPH", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);	// MenuNum 6
		LCD_WriteString(10, 150, "SAVE", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	break;}
	case 5:	{
		LCD_WriteString(10, 130, "GRAPH", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 150, "SAVE", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);	// MenuNum 7
		LCD_WriteString(10, 170, "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	break;}
	case 6:	{
		LCD_WriteString(10, 150, "SAVE", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 170, "EXIT", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);  // MenuNum 0
		LCD_WriteString(10, 50,  "CO2", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	break;}
	}
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  MAIN MENU FUNCTIONS END   --------------------------
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  CO2 MENU FUNCTIONS START  --------------------------
// ------------------------------------------------------------------------------------------------------


void Co2_Menu_Select(uint8_t Index)
{
	// Variable Set
	MenuNum = 2;
	SelectItemNum = Index;
	char Vrb[10]="          ";

	switch (SelectItemNum)
	{
	case 0:	{
		LCD_WriteString(10, 40,  "EtCO2", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	//EtCO2 -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 40,  "ALAR H",			Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_H, Vrb); LCD_WriteString(180, 40, Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(80, 60,  "ALAR L", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_L, Vrb);	LCD_WriteString(180, 60, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 220, "EXIT", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 1:	{
		LCD_WriteString(10, 40,  "EtCO2", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	//EtCO2 -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 40,  "ALAR H",			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_H, Vrb); LCD_WriteString(180, 40, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(80, 60,  "ALAR L", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_L, Vrb);	LCD_WriteString(180, 60, Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 80,  "RESP", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	//RESP -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 80,  "ALAR H", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Co2.RespAlar_H, Vrb);	LCD_WriteString(180, 80,  Vrb,  Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 2:	{
		LCD_WriteString(10, 40,  "EtCO2", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	//EtCO2 -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 40,  "ALAR H",			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_H, Vrb); LCD_WriteString(180, 40, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(80, 60,  "ALAR L", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_L, Vrb);	LCD_WriteString(180, 60, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 80,  "RESP", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	//RESP -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 80,  "ALAR H", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Co2.RespAlar_H, Vrb);	LCD_WriteString(180, 80,  Vrb,  Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(80, 100, "ALAR L", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Co2.RespAlar_L, Vrb);	LCD_WriteString(180, 100, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 3:	{
		LCD_WriteString(10, 80,  "RESP", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	//RESP -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 80,  "ALAR H", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Co2.RespAlar_H, Vrb);	LCD_WriteString(180, 80,  Vrb,  Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(80, 100, "ALAR L", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Co2.RespAlar_L, Vrb);	LCD_WriteString(180, 100, Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 120, "APNEA TIME", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.Apnea_Time, Vrb);	LCD_WriteString(180, 120, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 4:	{
		LCD_WriteString(10, 80,  "RESP", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	//RESP -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 80,  "ALAR H", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Co2.RespAlar_H, Vrb);	LCD_WriteString(180, 80,  Vrb,  Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(80, 100, "ALAR L", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Co2.RespAlar_L, Vrb);	LCD_WriteString(180, 100, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 120, "APNEA TIME", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.Apnea_Time, Vrb);	LCD_WriteString(180, 120, Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 140, "CO2 UNIT",		Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.Co2_Unit, Vrb);	LCD_WriteString(180, 140, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 5:	{
		LCD_WriteString(10, 120, "APNEA TIME", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.Apnea_Time, Vrb);	LCD_WriteString(180, 120, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 140, "CO2 UNIT",		Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.Co2_Unit, Vrb);	LCD_WriteString(180, 140, Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 160, "ZERO",			Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 6:	{
		LCD_WriteString(10, 140, "CO2 UNIT",		Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.Co2_Unit, Vrb);	LCD_WriteString(180, 140, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 160, "ZERO",			Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 180, "WAVE SCALE", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);  memset(Vrb, 0, 10); int2char(Co2.Wave_Scale, Vrb);	LCD_WriteString(180, 180, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 7:	{
		LCD_WriteString(10, 160, "ZERO",			Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 180, "WAVE SCALE", 		Font_11x18, COLOR_WHITE, COLOR_BLACK);  memset(Vrb, 0, 10); int2char(Co2.Wave_Scale, Vrb);	LCD_WriteString(180, 180, Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 200, "LOAD DEFAULTS",	Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 8:	{
		LCD_WriteString(10, 180, "WAVE SCALE", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);  memset(Vrb, 0, 10); int2char(Co2.Wave_Scale, Vrb);	LCD_WriteString(180, 180, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 200, "LOAD DEFAULTS",	Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 220, "EXIT", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 9:	{
		LCD_WriteString(10, 200, "LOAD DEFAULTS",	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 220, "EXIT", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 40,  "EtCO2", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	//EtCO2 -> 1:ALAR H 2:ALAR L
		LCD_WriteString(80, 40,  "ALAR H",			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_H, Vrb); LCD_WriteString(180, 40, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(80, 60,  "ALAR L", 			Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_L, Vrb);	LCD_WriteString(180, 60, Vrb,	Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	}
}


void Co2_EtCo2Alar_H_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0) 		{ Co2.EtCo2Alar_H++; if (Co2.EtCo2Alar_H > 60) Co2.EtCo2Alar_H = 0;
		LCD_WriteString(180, 40,    "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.EtCo2Alar_H, Vrb);
		LCD_WriteString(180, 40, 	  Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1) 	{ Co2.EtCo2Alar_H--;if (Co2.EtCo2Alar_H <= 0) Co2.EtCo2Alar_H = 60;
		LCD_WriteString(180, 40,    "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.EtCo2Alar_H, Vrb);
		LCD_WriteString(180, 40,      Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2) 	{
		LCD_WriteString(80,  40, "ALAR H", 	Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(180, 40,    "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.EtCo2Alar_H, Vrb);
		LCD_WriteString(180, 40, 	  Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Co2_EtCo2Alar_H_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(80,  40, "ALAR H", 	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.EtCo2Alar_H, Vrb);
	LCD_WriteString(180, 40,      Vrb, 	Font_11x18, COLOR_WHITE, COLOR_BLACK);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
		            value[3]=Co2.EtCo2Alar_H;
				  	ee_write(0,2,value);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write

}

void Co2_EtCo2Alar_L_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0) 		{ Co2.EtCo2Alar_L++;	if (Co2.EtCo2Alar_L > 60)	Co2.EtCo2Alar_L = 0;
		LCD_WriteString(180, 60,   "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.EtCo2Alar_L, Vrb);
		LCD_WriteString(180, 60,     Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1) 	{ Co2.EtCo2Alar_L--;	if (Co2.EtCo2Alar_L <= 0) 	Co2.EtCo2Alar_L = 60;
		LCD_WriteString(180, 60,   "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.EtCo2Alar_L, Vrb);
		LCD_WriteString(180, 60,     Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2) 	{
		LCD_WriteString(80,  60, "ALAR L", 	Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(180, 60,    "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.EtCo2Alar_L, Vrb);
		LCD_WriteString(180, 60,      Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Co2_EtCo2Alar_L_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(80,  60, "ALAR L", 	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.EtCo2Alar_L, Vrb);
	LCD_WriteString(180, 60,      Vrb, 	Font_11x18, COLOR_WHITE, COLOR_BLACK);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
		            value[4]=Co2.EtCo2Alar_L;
				  	ee_write(0,2,value);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write

}

void Co2_RespAlar_H_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0)		{ Co2.RespAlar_H++;	if (Co2.RespAlar_H > 60)	Co2.RespAlar_H = 0;
		LCD_WriteString(180, 80,   "   ",	Font_11x18, COLOR_RED, COLOR_BLACK);memset(Vrb, 0, 10);	int2char(Co2.RespAlar_H, Vrb);
		LCD_WriteString(180, 80,     Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{ Co2.RespAlar_H--;	if (Co2.RespAlar_H <= 0)	Co2.RespAlar_H = 60;
		LCD_WriteString(180, 80,   "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.RespAlar_H, Vrb);
		LCD_WriteString(180, 80,     Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)	{
		LCD_WriteString(80,  80, "ALAR H", 	Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(180, 80,    "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.RespAlar_H, Vrb);
		LCD_WriteString(180, 80,      Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Co2_RespAlar_H_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(80,  80, "ALAR H", Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.RespAlar_H, Vrb);
	LCD_WriteString(180, 80,      Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write

	                    value[5]=Co2.RespAlar_H;
					  	ee_write(0,2,value);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write

}

void Co2_RespAlar_L_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0)		{ Co2.RespAlar_L++;	if (Co2.RespAlar_L > 60)	Co2.RespAlar_L = 0;
		LCD_WriteString(180, 100,   "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.RespAlar_L, Vrb);
		LCD_WriteString(180, 100,     Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{ Co2.RespAlar_L--;	if (Co2.RespAlar_L <= 0)	Co2.RespAlar_L = 60;
		LCD_WriteString(180, 100,   "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.RespAlar_L, Vrb);
		LCD_WriteString(180, 100,     Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)	{
		LCD_WriteString(80,  100, "ALAR L", Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(180, 100,    "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.RespAlar_L, Vrb);
		LCD_WriteString(180, 100,      Vrb,	Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Co2_RespAlar_L_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(80,  100, "ALAR L",	Font_11x18, COLOR_WHITE, COLOR_BLACK); 	memset(Vrb, 0, 10); int2char(Co2.RespAlar_L, Vrb);
	LCD_WriteString(180, 100,      Vrb,	Font_11x18, COLOR_WHITE, COLOR_BLACK);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
			            value[6]=Co2.RespAlar_L;
					  	ee_write(0,2,value);

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write


}

void Co2_Apnea_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0) 		{ Co2.Apnea_Time++;	if (Co2.Apnea_Time > 60)	 Co2.Apnea_Time = 0;
		LCD_WriteString(180, 120,        "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Apnea_Time, Vrb);
		LCD_WriteString(180, 120,          Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{ Co2.Apnea_Time--;	if (Co2.Apnea_Time <= 0)	Co2.Apnea_Time = 60;
		LCD_WriteString(180, 120,        "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Apnea_Time, Vrb);
		LCD_WriteString(180, 120,          Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2) 	{
		LCD_WriteString(10,  120, "APNEA TIME", Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(180, 120,       "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Apnea_Time, Vrb);
		LCD_WriteString(180, 120,         Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}


}

void Co2_Apnea_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10, 120, "APNEA TIME", 	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Apnea_Time, Vrb);
	LCD_WriteString(180, 120,		  Vrb, 	Font_11x18, COLOR_WHITE, COLOR_BLACK);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
			  	value[0]=Co2.Apnea_Time;
			  	ee_write(0,2,value);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
}

void Co2_Co2_Unit_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0)		{ Co2.Co2_Unit++; if (Co2.Co2_Unit > 60) 	Co2.Co2_Unit = 0;
		LCD_WriteString(180, 140, "   ",		Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Co2_Unit, Vrb);
		LCD_WriteString(180, 140,   Vrb,  		Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{ Co2.Co2_Unit--; if (Co2.Co2_Unit <= 0)	Co2.Co2_Unit = 60;
		LCD_WriteString(180, 140, "   ", 		Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Co2_Unit, Vrb);
		LCD_WriteString(180, 140,   Vrb, 		Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)	{
		LCD_WriteString(10,  140, "CO2 UNIT",	Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(180, 140,      "   ",	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Co2_Unit, Vrb);
		LCD_WriteString(180, 140,        Vrb,	Font_11x18, COLOR_RED, COLOR_BLACK);
	}

}

void Co2_Co2_Unit_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10,  140, "CO2 UNIT",	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.Co2_Unit, Vrb);
	LCD_WriteString(180, 140,       Vrb, 	Font_11x18, COLOR_WHITE, COLOR_BLACK);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
			  	value[1]=Co2.Co2_Unit;
			  	ee_write(0,2,value);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
}

void Co2_Wave_Scale_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0)		{Co2.Wave_Scale++;	if (Co2.Wave_Scale > 5) 	Co2.Wave_Scale = 0;
		LCD_WriteString(180, 180, "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.Wave_Scale, Vrb);
		LCD_WriteString(180, 180,   Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1) 	{ Co2.Wave_Scale--;	if (Co2.Wave_Scale <= 0)	Co2.Wave_Scale = 5;
		LCD_WriteString(180, 180, "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.Wave_Scale, Vrb);
		LCD_WriteString(180, 180,   Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)	{
		LCD_WriteString(10,  180, "WAVE SCALE", Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(180, 180, 		 "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Co2.Wave_Scale, Vrb);
		LCD_WriteString(180, 180,          Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Co2_Wave_Scale_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10, 180, "WAVE SCALE", Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Co2.Wave_Scale, Vrb);
	LCD_WriteString(180, 180, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
	            value[2]=Co2.Wave_Scale;
			  	ee_write(0,2,value);



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write

}

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  CO2 MENU FUNCTIONS END   ---------------------------
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  TIME MENU FUNCTIONS START  -------------------------
// ------------------------------------------------------------------------------------------------------

void Time_Menu_Select(uint8_t Index)
{
	// Variable Set
	MenuNum = 3;
	SelectItemNum = Index;
	char Vrb[10]="          ";

	switch (SelectItemNum)
	{
	case 0:	{
		LCD_WriteString(10,  40, "YEAR", 	Font_11x18, COLOR_WHITE, COLOR_BLACK);  memset(Vrb, 0, 10); int2char(Time.Year,  Vrb); 	LCD_WriteString(80, 40, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10,  60, "MONTH", 	Font_11x18, COLOR_GREEN, COLOR_BLACK);  memset(Vrb, 0, 10); int2char(Time.Month, Vrb); 	LCD_WriteString(80, 60, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 160, "EXIT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 1:	{
		LCD_WriteString(10,  40, "YEAR", 	Font_11x18, COLOR_GREEN, COLOR_BLACK);  memset(Vrb, 0, 10);	int2char(Time.Year,  Vrb);	 LCD_WriteString(80, 40, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10,  60, "MONTH", 	Font_11x18, COLOR_WHITE, COLOR_BLACK);  memset(Vrb, 0, 10);	int2char(Time.Month, Vrb);	LCD_WriteString(80, 60, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10,  80, "DATE", 	Font_11x18, COLOR_GREEN, COLOR_BLACK);  memset(Vrb, 0, 10); int2char(Time.Date,  Vrb); 	LCD_WriteString(80, 80, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 2:
	{
		LCD_WriteString(10,  60,  "MONTH", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Month, Vrb); 	LCD_WriteString(80, 60,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10,  80,   "DATE",	 Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Time.Date,  Vrb); 	LCD_WriteString(80, 80,  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 100,   "HOUR", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Hour,  Vrb);	LCD_WriteString(80, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 3:
	{
		LCD_WriteString(10,  80,   "DATE",	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10); int2char(Time.Date,  Vrb); 	LCD_WriteString(80, 80,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 100,   "HOUR", 	 Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Hour,  Vrb);	LCD_WriteString(80, 100, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 120, "MINUTE", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Minute,Vrb);	LCD_WriteString(80, 120, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 4:
	{
		LCD_WriteString(10, 100,   "HOUR", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Hour,  Vrb);	LCD_WriteString(80, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 120, "MINUTE", 	 Font_11x18, COLOR_WHITE, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Minute,Vrb);	LCD_WriteString(80, 120, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 140,   "SAVE", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 5:
	{
		LCD_WriteString(10, 120, "MINUTE", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);	memset(Vrb, 0, 10);	int2char(Time.Minute,Vrb);	LCD_WriteString(80, 120, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 140,   "SAVE", 	 Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 160,   "EXIT", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 6:
	{
		LCD_WriteString(10, 140,   "SAVE", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 160,   "EXIT", 	 Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10,  40,   "YEAR", 	 Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Year,  Vrb); 	LCD_WriteString(80, 40,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	}
}



void Time_Year_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0) 		{ Time.Year++; 	if (Time.Year > 2030)	Time.Year = 2020;
		LCD_WriteString(80, 40, "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Year, Vrb);
		LCD_WriteString(80, 40,   Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1) 	{ Time.Year--;	if (Time.Year < 2000)	Time.Year = 2020;
		LCD_WriteString(80, 40, "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Year, Vrb);
		LCD_WriteString(80, 40,  Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2) 	{
		LCD_WriteString(10, 40, "YEAR", Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(80, 40,  "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Year, Vrb);
		LCD_WriteString(80, 40,    Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Time_Year_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10, 40, "YEAR", Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Year, Vrb);
	LCD_WriteString(80, 40,    Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
	flash_write(0x08080800,Time.Year,257);
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
}

void Time_Month_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0) 		{ Time.Month++;	if (Time.Month > 12)	Time.Month = 1;
		LCD_WriteString(80, 60, "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Month, Vrb);
		LCD_WriteString(80, 60,   Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{ Time.Month--;	if (Time.Month <= 0)	Time.Month = 12;
		LCD_WriteString(80, 60, "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Month, Vrb);
		LCD_WriteString(80, 60,   Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)	{
		LCD_WriteString(10, 60, "MONTH",Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(80, 60,   "   ",Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Month, Vrb);
		LCD_WriteString(80, 60,     Vrb,Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Time_Month_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10, 60, "MONTH", Font_11x18, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10);
	int2char(Time.Month, Vrb);
	LCD_WriteString(80, 60, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);


	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
		flash_write(0x08090000,Time.Month,258);
	////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

}

void Time_Date_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0)		{ Time.Date++;	if (Time.Date > 30)	Time.Date = 1;
		LCD_WriteString(80, 80, "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Date, Vrb);
		LCD_WriteString(80, 80,   Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{ Time.Date--;	if (Time.Date <= 0)	Time.Date = 30;
		LCD_WriteString(80, 80, "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Date, Vrb);
		LCD_WriteString(80, 80,   Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)	{
		LCD_WriteString(10, 80, "DATE", Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(80, 80,  "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Date, Vrb);
		LCD_WriteString(80, 80,    Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Time_Date_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10, 80, "DATE", Font_11x18, COLOR_WHITE, COLOR_BLACK); 	memset(Vrb, 0, 10);	int2char(Time.Date, Vrb);
	LCD_WriteString(80, 80,    Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write
//		            value[8]=Time.Date;
//				  	ee_write(0, 2,value);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////flash write



	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
		flash_write(0x08090800,Time.Date,259);
	////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////


}

void Time_Hour_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0)		{ Time.Hour++;if (Time.Hour > 24)Time.Hour = 0;
		LCD_WriteString(80, 100, "   ",  Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Hour, Vrb);
		LCD_WriteString(80, 100,   Vrb,  Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{ Time.Hour--;if (Time.Hour <= 0)Time.Hour = 24;
		LCD_WriteString(80, 100, "   ",  Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Hour, Vrb);
		LCD_WriteString(80, 100,   Vrb,  Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)	{
		LCD_WriteString(10, 100, "HOUR", Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(80, 100,  "   ", Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Hour, Vrb);
		LCD_WriteString(80, 100,    Vrb, Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Time_Hour_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10, 100, "HOUR", Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Time.Hour, Vrb);
	LCD_WriteString(80, 100,    Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);

	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
		flash_write(0x080A0000,Time.Hour,260);
	////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

}

void Time_Minute_Set_Open(uint8_t Select)
{
	char Vrb[10]="          ";
	SelectCase = 1;

	if (Select == 0){Time.Minute++;	if (Time.Minute > 60)Time.Minute = 0;
		LCD_WriteString(80, 120,    "   ",	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Minute, Vrb);
		LCD_WriteString(80, 120,      Vrb,	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 1)	{Time.Minute--;if (Time.Minute <= 0)Time.Minute = 60;
		LCD_WriteString(80, 120,    "   ", 	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Time.Minute, Vrb);
		LCD_WriteString(80, 120,      Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}else if (Select == 2)
	{
		LCD_WriteString(10, 120, "MINUTE", 	Font_11x18, COLOR_RED, COLOR_BLACK);
		LCD_WriteString(80, 120,    "   ",	Font_11x18, COLOR_RED, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Time.Minute, Vrb);
		LCD_WriteString(80, 120,      Vrb, 	Font_11x18, COLOR_RED, COLOR_BLACK);
	}
}

void Time_Minute_Set_Close()
{
	char Vrb[10]="          ";
	SelectCase = 0;

	LCD_WriteString(10, 120, "MINUTE", Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10); int2char(Time.Minute, Vrb);
	LCD_WriteString(80, 120, 	  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);


	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
		flash_write(0x080A0800,Time.Minute,261);
	////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

}

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  TIME MENU FUNCTIONS END ----------------------------
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  WIFI MENU FUNCTIONS START  -------------------------
// ------------------------------------------------------------------------------------------------------

void Wifi_Menu_Select(uint8_t Index)
{
	// Variable Set
	MenuNum = 4;
	SelectItemNum = Index;
	char Vrb[10]="          ";

	switch (Index)
	{
	case 0:	{
		LCD_WriteString(10, 40,  "WIFI NAME",		Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 40, (char*)&Wifi.Name, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 60,  "WIFI KEY", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 60, (char*)&Wifi.Key,  Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 140, "EXIT", 			Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 1:	{
		LCD_WriteString(10, 40,  "WIFI NAME",		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 40, (char*)&Wifi.Name, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 60,  "WIFI KEY", 		Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 60, (char*)&Wifi.Key,  Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 80,  "WIFI CONNECT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 80,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 2:	{
		LCD_WriteString(10, 60,  "WIFI KEY", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 60, (char*)&Wifi.Key,  Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 80,  "WIFI CONNECT", 	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 80,  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 100, "CLOUD CONNECT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 3:	{
		LCD_WriteString(10, 80,  "WIFI CONNECT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 80,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 100, "CLOUD CONNECT", 	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 100, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 120, "WIFI", 			Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.Status,Vrb);				LCD_WriteString(160, 120, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 4:	{
		LCD_WriteString(10, 100, "CLOUD CONNECT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.ConnectionStatus,  Vrb); 	LCD_WriteString(160, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 120, "WIFI", 			Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.Status,Vrb);				LCD_WriteString(160, 120, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 140, "EXIT", 			Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 5:	{
		LCD_WriteString(10, 120, "WIFI", 			Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Wifi.Status,Vrb);				LCD_WriteString(160, 120, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 140, "EXIT", 			Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 40,  "WIFI NAME",		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	LCD_WriteString(160, 40, (char*)&Wifi.Name, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	}
}


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  WIFI MENU FUNCTIONS END ----------------------------
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  ALARM MENU FUNCTIONS START  -------------------------
// ------------------------------------------------------------------------------------------------------

void Alarm_Menu_Select(uint8_t Index)
{
	// Variable Set
	MenuNum = 5;
	SelectItemNum = Index;
	// char Vrb[10]="          ";

	switch (Index)
	{
	case 0:	{
		LCD_WriteString(10, 40,  "1",			Font_11x18, COLOR_WHITE, COLOR_BLACK); LCD_WriteString(50, 40, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 60,  "2",			Font_11x18, COLOR_GREEN, COLOR_BLACK); LCD_WriteString(50, 60, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 80, "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 1:	{
		LCD_WriteString(10, 40,  "1",			Font_11x18, COLOR_GREEN, COLOR_BLACK); LCD_WriteString(50, 40, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 60,  "2",			Font_11x18, COLOR_WHITE, COLOR_BLACK); LCD_WriteString(50, 60, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 80, "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK);  break;}
	case 2:	{
		LCD_WriteString(10, 40,  "1",			Font_11x18, COLOR_GREEN, COLOR_BLACK); LCD_WriteString(50, 40, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 60,  "2",			Font_11x18, COLOR_GREEN, COLOR_BLACK); LCD_WriteString(50, 60, (char*)&WarningSystem.Warning[1].Name, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 80, "EXIT", 		Font_11x18, COLOR_WHITE, COLOR_BLACK); break;}
	}
}

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  ALARM MENU FUNCTIONS END ----------------------------
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  GRAPH MENU FUNCTIONS START  -------------------------
// ------------------------------------------------------------------------------------------------------

void Graph_Menu_Select(uint8_t Index)
{
	// Variable Set
	MenuNum = 6;
	SelectItemNum = Index;
	char Vrb[10]="          ";

	switch (Index)
	{
	case 0:	{
		LCD_WriteString(10, 40,  "MODE",		Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Mode,  Vrb); LCD_WriteString(120, 40,  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 60,  "CO2 UNIT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Unit,  Vrb); LCD_WriteString(120, 60,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 80, "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 1:	{
		LCD_WriteString(10, 40,  "MODE",		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Mode,  Vrb); LCD_WriteString(120, 40,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 60,  "CO2 UNIT", 	Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Unit,  Vrb); LCD_WriteString(120, 60,  Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 80, "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 2:	{
		LCD_WriteString(10, 40,  "MODE",		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Mode,  Vrb); LCD_WriteString(120, 40,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 60,  "CO2 UNIT", 	Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Graphic.Unit,  Vrb); LCD_WriteString(120, 60,  Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 80, "EXIT", 		Font_11x18, COLOR_WHITE, COLOR_BLACK); break;}
	}
}


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  GRAPH MENU FUNCTIONS END ----------------------------
// ------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  SAVE MENU FUNCTIONS START  -------------------------
// ------------------------------------------------------------------------------------------------------

void Save_Menu_Select(uint8_t Index)
{
	// Variable Set
	MenuNum = 7;
	SelectItemNum = Index;
	char Vrb[10]="          ";

	switch (Index)
	{
	case 0:
	{
		LCD_WriteString(10,  40,       "Mode", 		Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.Mode,       Vrb); LCD_WriteString(200, 40, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10,  60,    "RawData", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RawData,    Vrb); LCD_WriteString(200, 60, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 120,       "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 1:
	{
		LCD_WriteString(10,  40,       "Mode", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.Mode,       Vrb); LCD_WriteString(200, 40, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10,  60,    "RawData", 		Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RawData,    Vrb); LCD_WriteString(200, 60, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10,  80, "RollingRms", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RollingRms, Vrb);	LCD_WriteString(200, 80, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 2:
	{
		LCD_WriteString(10,  60,    "RawData", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RawData,    Vrb); LCD_WriteString(200, 60, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10,  80, "RollingRms", 		Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RollingRms, Vrb);	LCD_WriteString(200, 80, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 100, "GasConcentration",Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.GasConcentration, Vrb); LCD_WriteString(200, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 3:
	{
		LCD_WriteString(10,  80, "RollingRms", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.RollingRms,       Vrb); LCD_WriteString(200, 80, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 100, "GasConcentration",Font_11x18, COLOR_WHITE, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.GasConcentration, Vrb); LCD_WriteString(200, 100, Vrb, Font_11x18, COLOR_WHITE, COLOR_BLACK);
		LCD_WriteString(10, 120, 	   "EXIT", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); break;}
	case 4:
	{
		LCD_WriteString(10,  40,       "Mode", 		Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.Mode,        		Vrb); LCD_WriteString(200, 40, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 100, "GasConcentration",Font_11x18, COLOR_GREEN, COLOR_BLACK); memset(Vrb, 0, 10);	int2char(Save.GasConcentration, Vrb); LCD_WriteString(200, 100, Vrb, Font_11x18, COLOR_GREEN, COLOR_BLACK);
		LCD_WriteString(10, 120, 	   "EXIT", 		Font_11x18, COLOR_WHITE, COLOR_BLACK); break;}
	}
}



// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------  SAVE MENU FUNCTIONS END    -------------------------
// ------------------------------------------------------------------------------------------------------


void int2char(int num, char str[])
{
	char lstr[30];
	int cnt = 0;
	int div = 10;
	int j = 0;
	while( num >= div)
	{
		lstr[cnt] = num % div + 0x30;
		num /= 10;
		cnt++;
	}
	lstr[cnt] = num + 0x30;
	for(j= cnt ; j >=0;j--)
	{
		str[cnt-j] = lstr[j];
	}
}


// Drawing a Sine Graph
void SineGraph()
{
	HAL_IWDG_Refresh(&hiwdg);
	if (MenuNum==0)
	{
		if (Graph_k>Graph_x2)
		{
			Graph_k=Graph_x1;
			// Clear points
			LCD_DrawLine(Graph_x1, Graph_y1, Graph_x1, Graph_y2, COLOR_BLACK);
			LCD_DrawLine(Graph_x1-1, Graph_y1, Graph_x1-1, Graph_y2, COLOR_BLACK);
		}
		Graph_x+=0.1;
		Graph_y = sin(Graph_x);
		Graph_value=(int)((Graph_y2+Graph_y1)/2-(Graph_y*(Graph_y2-Graph_y1)/2));
		// Draw points
		LCD_DrawPixel(Graph_k-1, Graph_value, COLOR_GREEN);
		// Clear points
		LCD_DrawLine(Graph_k, Graph_y1, Graph_k, Graph_y2, COLOR_BLACK);
		LCD_DrawLine(Graph_k+1, Graph_y1, Graph_k+1, Graph_y2, COLOR_BLACK);
		Graph_k++;
	}
}

void Graph(int32_t val, uint16_t color)
{
	uint8_t value = 0;
	if (MenuNum==0)
	{
		if (val>10000) 		{ val = 10000;}
		else if (val<1000) { val = 1000;}
		value = MAP(val, 1000, 10000, 0, 50);
		if (Graphic.Counter>Graphic.X2)
		{
			Graphic.Counter=Graphic.X1;
			// Clear points
			LCD_DrawLine(Graphic.X1, Graphic.Y1, Graphic.X1, Graphic.Y1, COLOR_BLACK);
		}
		// Draw points
		if (Graphic.Mode == 1) {
			LCD_DrawLine(Graphic.Counter, (Graphic.Y2 - value), Graphic.Counter, Graphic.Y2-1, color);
			//LCD_DrawRectangle(Graphic.Counter, (Graphic.Y2 - value), Graphic.Counter, Graphic.Y2-1, color);

		}else
		{
			LCD_DrawPixel(Graphic.Counter, (Graphic.Y2 - value), COLOR_GREEN);

		}
		// Clear points
		LCD_DrawLine(Graphic.Counter + 1, Graphic.Y1, Graphic.Counter + 1 , Graphic.Y2-1, COLOR_BLACK);
		Graphic.Counter+=1;
	}
}

void Graphs(int32_t refVal, uint16_t refColor, int32_t gasVal, uint16_t gasColor)
{
	uint8_t value1 = 0,value2 = 0;
	if (MenuNum==0)
	{
		if (refVal>20000) 		{ refVal = 20000;}
		else if (refVal<0) 		{ refVal = 0;}
		value1 = MAP(refVal, 0, 20000, 0, 100);
		if (gasVal>20000)		{ gasVal = 20000;}
		else if (gasVal<0) 		{ gasVal = 0;}
		value2 = MAP(gasVal, 0, 20000, 0, 100);

		if (Graphic.Counter>Graphic.X2)
		{
			Graphic.Counter=Graphic.X1;
			// Clear points
			LCD_DrawLine(Graphic.X1, Graphic.Y1, Graphic.X1, Graphic.Y1, COLOR_BLACK);
		}
		// Draw points
		if (Graphic.Mode == 1)
		{
			//if ((120 - value1)>100) {	value1 = 100; } else{ value1 = 120 - value1; }
			LCD_DrawLine(Graphic.Counter, (Graphic.Y2 - (value1)), Graphic.Counter, Graphic.Y2, refColor);
			//LCD_DrawLine(Graphic.Counter, (Graphic.Y2 + value2), Graphic.Counter, Graphic.Y2, gasColor);
		}else
		{
			//if ((120 - value1)>100) {	value1 = 100; } else{ value1 = value1 - 120; }
			LCD_DrawPixel(Graphic.Counter, (Graphic.Y2 - (value1)), refColor);
			//LCD_DrawPixel(Graphic.Counter, (Graphic.Y2 + value2), gasColor);
		}
		// Clear points
		LCD_DrawLine(Graphic.Counter + 1, Graphic.Y1, Graphic.Counter + 1 , Graphic.Y2, COLOR_BLACK);
		Graphic.Counter+=1;

	}
}
void Time_Update()
{

}

void Icons_Update()
{

}

void Co2_Unit_Update()
{

}

void Wave_Scale_Update()
{

}

void EtCo2_Update(int32_t val)
{
	// Variable Set
	char Vrb[10]="     ";

	if (Graphic.Counter >= Graphic.X2 || Graphic.Counter <= Graphic.X1 + 10) {
	LCD_WriteString(82, 100, "  ", Font_16x26, COLOR_WHITE, COLOR_BLACK); } memset(Vrb, 0, 10); int2char(val, Vrb);
    LCD_WriteString(50, 100,  Vrb, Font_16x26, COLOR_WHITE, COLOR_BLACK);
}

void Resp_Update()
{
	// Variable Set
	char Vrb[10]="     ";

	if (Graphic.Counter >= Graphic.X2 || Graphic.Counter <= Graphic.X1 + 10) {
	LCD_WriteString(282, 100, "  ", Font_16x26, COLOR_WHITE, COLOR_BLACK); } memset(Vrb, 0, 10); int2char(Graphic.Counter, Vrb);
	LCD_WriteString(250, 100,  Vrb, Font_16x26, COLOR_WHITE, COLOR_BLACK);
}

void Variable_Update()
{
	// Variable Set
	char Vrb[10]="     ";

	memset(Vrb, 0, 10); int2char(MenuNum, 			Vrb); LCD_WriteString(5,  230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10);	int2char(SelectItemIndex, 	Vrb); LCD_WriteString(15, 230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10);	int2char(SelectItemNum,		Vrb); LCD_WriteString(25, 230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10);	int2char(MaxSelectItems, 	Vrb); LCD_WriteString(35, 230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10);	int2char(SelectCase, 		Vrb); LCD_WriteString(45, 230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
//
	memset(Vrb, 0, 10); int2char(gasValue, 			Vrb); LCD_WriteString(60, 230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10); int2char(refValue, 			Vrb); LCD_WriteString(100,230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10); int2char(rrmsGasValue,	 	Vrb); LCD_WriteString(140,230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10); int2char(rrmsRefValue, 		Vrb); LCD_WriteString(180,230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
	memset(Vrb, 0, 10); int2char(SensorValueFiltered,Vrb);LCD_WriteString(220,230, Vrb, Font_7x10, COLOR_WHITE, COLOR_BLACK);
//

}





