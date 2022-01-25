/*
 * LCD_Interface_Library.h
 *
 *  Created on: Dec 16, 2021
 *      Author: Kadir AK
 */

#ifndef INC_LCD_INTERFACE_LIBRARY_H_
#define INC_LCD_INTERFACE_LIBRARY_H_


/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "string.h"
#include "LCD_Icons.h"
#include "LCD_Hardware_Library.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

// Values Displayed on the Capnography Device  -  Kapnografi Cihazında Gösterilen Değerler
/* 	. . . . . . . . . . . .		TERIMLER		. . . . . . . . . . . .
 *  MEASUREMENTS(ÖLÇÜMLER):
 *		CO2 kısmi basıncı ve solunum hızı (CO2 partial pressure and respiratory rate)
 *  Respiration Rate (RR) :
 *		3-150bpm(Beats per minute) Soluk sayısı.
 *		İki nefesten sonra görüntülenir ve her nefeste güncellenir.
 *      RR Ölçüm aralığı: 0-150rpm
 *	End-tidal Carbon Dioxide (EtCO2):
 *		Kantitatif(Quantitative) EtCO2 her nefeste güncellenir (mmHg,%,kPa olarak görüntülenir)
	    Normal Ventilasyon 	. . .  	EtCo2 35-40 mmHg
		Hiperventilasyon	. . .  	EtCo2 30-35 mmHg
		Hipoventilasyon  	. . .	EtCo2 > 45  mmHg
		EtCo2 Ölçüm aralığı: 0-150mmHg
 *	Capnogram:
 *		Grafik (saniyede CO2 değerleri taraması)
 *  Alarm Limits:
 *		Alarm limitleri ve görsel alarm durumu göstergesi, aktif alarmları iki dakika sessize alma
 *  Power Button:
 *		15 saniyede tam hassasiyete ısınma süresi.
 *		10 saniye içinde görüntülenen dalga biçimi, 2 dakika içinde özellikleri karşılar (25°C oda sıcaklığında)
 *	Airway Adapter:
 *		Yetişkin/pediatrik ve bebek boyutlarında mevcuttur
 *
 *	CO2 ölçüm doğruluğunun kayması
 *		Kısa sapma: 4 saatte 1 mmHg'den az sapma
 *	    Uzun sapma: 120 saat içinde ölçüm doğruluğu gereksinimlerini karşılamayacaktır
 *	CO2 noise:
 *		%5 CO2'de 1 mmHg'den az gürültü RMS
 *
 *
 *
 *	ARALIKLAR(RANGES):
 *		CO2 . . . 0-99 mmHg
 *		CO2 . . . 0-9.9 kPa
 *  DOĞRULUK-STANDART KOŞULLAR(ACCURACY STANDARD CONDITIONS):
 *  	CO2 . . . 0-40 mmHg +2 mmHg; 41-99 mmHg 6% of reading
 *		CO2 . . . 0-5.3 kPa +0.3 kPa; 5.4-9.9 kPa 6% of reading
 *  PİLLER(BATTERIES):
 *		Tip. . . 2 (iki) AAA Alkalin veya Lityum
 *	ÇEVRESEL(ENVIRONMENTAL):
 * 		Çevresel çalışma koşulları ...
 *  FİZİKSEL ÖZELLİKLER (PHYSICAL CHARACTERISTICS):
 *		Fiziksel özellikleri(Boyut,Ağırlık)
 *	ADAPTER INFORMATION(ADAPTÖR BİLGİLERİ)
 *		Dead space adult/pediatric 	. . . 6 ml
 *		Dead space infant    		. . . 1 m
 *
 *
 *	ALARM DURUMLARI(ALARM SITUATIONS)
 *		Düşüp pil alarmı
 *		Solunum hızı alarmı (RR)
 *		Aşırı limit alarımları
 *		Adaptör Yok
 *  	Tıkalı Adaptör
 *		Nefes Yok (Apne)
 *		Ayarlanabilir Yüksek ve Düşük EtCO2 alarmı için sesli ve görsel alarm sistemi
 *
 *	ICONS
 *		Akciğer/min  Dk soluk sayısı (RR)
 *		Mute/Clear Mute
 * 		Enable Wifi
 *
 */


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

// Desktop Variables
typedef struct {
	volatile uint8_t 	Time_Hour;
	volatile uint8_t	Time_Minute;
	volatile uint8_t 	Alarm_Status;
	volatile uint8_t 	Battery_Status;
	volatile uint8_t 	Wifi_Status;
	volatile uint8_t 	Co2_Unit;
} DesktopVariablesDef;
volatile DesktopVariablesDef DesktopVariables;

// Co2 Variables
typedef struct {
	volatile uint8_t 	EtCo2Alar_H;
	volatile uint8_t 	EtCo2Alar_L;
	volatile uint8_t 	RespAlar_H;
	volatile uint8_t 	RespAlar_L;
	volatile uint8_t 	Apnea_Time; 	// sn
	volatile uint8_t 	Co2_Unit;  		// 0:mmHg	1:%		2:KPA
	volatile uint8_t 	Wave_Scale; 	// 0:54mmHg		1:76mmHg
	volatile uint32_t 	EtCo2Alar_H_EE;
	volatile uint32_t 	EtCo2Alar_L_EE;
	volatile uint32_t 	RespAlar_H_EE;
	volatile uint32_t 	RespAlar_L_EE;
	volatile uint32_t 	Apnea_Time_EE;
	volatile uint32_t 	Co2_Unit_EE;
	volatile uint32_t 	Wave_Scale_EE;
} Co2VariablesDef;
volatile Co2VariablesDef Co2;

// Time Variables
typedef struct {
	volatile uint16_t 	Year;					// Yıl
	volatile uint8_t  	Month;					// Ay
	volatile uint8_t  	Date;					// Tarih
	volatile uint8_t  	Hour;					// Saat
	volatile uint8_t  	Minute;					// Wifi Adı
} TimeVariablesDef;
volatile TimeVariablesDef Time;

// Wifi Variables
typedef struct {
    volatile char     		Name[50];					// Wifi Adı
    volatile char   		Key[50];					// Wifi Şifresi
    volatile uint16_t   	ConnectionStatus;			// Bağlantı Durumu
    volatile uint8_t  		Status;						// Wifi Durumu Açık/Kapalı
    volatile uint16_t  		DataTransmissionStatus;		// Wifi Veri gönderim Durumu
} WifiVariablesDef;
volatile WifiVariablesDef Wifi;

// WarningSystem Variables
typedef struct {
    volatile char     		Name[50];					// Alarm Adı
    volatile uint8_t  		Priority;					// Alarm Önceliği
    volatile uint16_t   	LedColor;					// Alarm Led
    volatile uint8_t  		WarningStatus;				// Uyarı Durumu
    volatile uint16_t  		Sound;						// Alarm Sesi
} WarningVariablesDef;

typedef struct {
	volatile WarningVariablesDef 	Warning[10];				// Alarm Bilgileri
    volatile uint8_t  				BuzzerStatus;  				// Buzzer Durumu
    volatile uint16_t  				SilentModeRemainingTime;	// Buzzer Sesizse ise kalan süre
} WarningSystemVariablesDef;
volatile WarningSystemVariablesDef WarningSystem;

// Graphic Variables
typedef struct {
    volatile uint16_t 	X1;					//	... point
    volatile uint16_t 	X2;					//	... point
    volatile uint16_t 	Y1;					//	... point
    volatile uint16_t 	Y2;					//	... point
    volatile uint16_t 	Value;				//	... point
    volatile uint16_t 	GraphicArea;		//	... point
    volatile uint16_t 	Counter;			//	... point
    volatile uint8_t  	Mode;				//	0: Histogram 1: Line
    volatile uint8_t    Unit;				//  mmHg % kPA
} GraphicVariablesDef;
volatile GraphicVariablesDef Graphic;

// Save Variables
typedef struct {
	volatile uint8_t 	Mode;				//	0:Disabled 	1:Enable
	volatile uint8_t  	RawData;			//	0:Pasif		1:Aktif
	volatile uint8_t  	RollingRms;			//	0:Pasif		1:Aktif
	volatile uint8_t  	GasConcentration;	//	0:Pasif		1:Aktif
	volatile uint16_t  	SaveFrameTime;		//	... ms
} SaveVariablesDef;
volatile SaveVariablesDef Save;

/* Private function prototypes -----------------------------------------------*/
// Set Functions
void DefaultVariablesSet();
void Desktop_Menu_Set();
void Open_LCD_Set();
void Close_LCD_Set();
void Main_Menu_Set(); 			// MenuNum 1
void Co2_Menu_Set();			// MenuNum 2
void Time_Menu_Set();			// MenuNum 3
void Wifi_Menu_Set();			// MenuNum 4
void Alarm_Menu_Set();			// MenuNum 5
void Graph_Menu_Set();			// MenuNum 6
void Save_Menu_Set();			// MenuNum 7
void Exit_Menu_Set();			// MenuNum 0

// Update Event Functions  ???????
void Desktop_Update();
void Time_Update();
void Icons_Update();
void Co2_Unit_Update();
void Wave_Scale_Update();
void EtCo2_Update(int32_t val);
void Resp_Update();

void Variable_Update();

// Events Functions
void Select_Event_Set();
void Event_Select(uint8_t Select);
void Value_Change_Variables (uint8_t Select);
void Menu_Select(uint8_t Select);
void Main_Menu_Select(uint8_t Index); 			// MenuNum 1
void Co2_Menu_Select(uint8_t Index);			// MenuNum 2
void Time_Menu_Select(uint8_t Index);			// MenuNum 3
void Wifi_Menu_Select(uint8_t Index);			// MenuNum 4
void Alarm_Menu_Select(uint8_t Index);			// MenuNum 5
void Graph_Menu_Select(uint8_t Index);			// MenuNum 6
void Save_Menu_Select(uint8_t Index);			// MenuNum 7


// Co2 Events Functions
void Co2_EtCo2Alar_H_Set_Open(uint8_t Select);	// SelectItemNum 0
void Co2_EtCo2Alar_L_Set_Open(uint8_t Select);	// SelectItemNum 1
void Co2_RespAlar_H_Set_Open(uint8_t Select);	// SelectItemNum 2
void Co2_RespAlar_L_Set_Open(uint8_t Select);	// SelectItemNum 3
void Co2_Apnea_Set_Open(uint8_t Select);		// SelectItemNum 4
void Co2_Co2_Unit_Set_Open(uint8_t Select);		// SelectItemNum 5
void Co2_Zero_Set_Open(uint8_t Select);			// SelectItemNum 6
void Co2_Wave_Scale_Set_Open(uint8_t Select);	// SelectItemNum 7
void Co2_Load_Defaults_Set_Open(uint8_t Select);// SelectItemNum 8
void Co2_EtCo2Alar_H_Set_Close();				// SelectItemNum 0
void Co2_EtCo2Alar_L_Set_Close();				// SelectItemNum 1
void Co2_RespAlar_H_Set_Close();				// SelectItemNum 2
void Co2_RespAlar_L_Set_Close();				// SelectItemNum 3
void Co2_Apnea_Set_Close();						// SelectItemNum 4
void Co2_Co2_Unit_Set_Close();					// SelectItemNum 5
void Co2_Zero_Set_Close();						// SelectItemNum 6
void Co2_Wave_Scale_Set_Close();				// SelectItemNum 7
void Co2_Load_Defaults_Set_Close();				// SelectItemNum 8

// Time Events Functions
void Time_Year_Set_Open(uint8_t Select);		// SelectItemNum 0
void Time_Month_Set_Open(uint8_t Select);		// SelectItemNum 1
void Time_Date_Set_Open(uint8_t Select);		// SelectItemNum 2
void Time_Hour_Set_Open(uint8_t Select);		// SelectItemNum 3
void Time_Minute_Set_Open(uint8_t Select);		// SelectItemNum 4
void Time_Save_Set_Open(uint8_t Select);		// SelectItemNum 5
void Time_Year_Set_Close();						// SelectItemNum 0
void Time_Month_Set_Close();					// SelectItemNum 1
void Time_Date_Set_Close();						// SelectItemNum 2
void Time_Hour_Set_Close();						// SelectItemNum 3
void Time_Minute_Set_Close();					// SelectItemNum 4
void Time_Save_Set_Close();						// SelectItemNum 5

// Wifi Events Functions
void Wifi_Set_Open(uint8_t Select);				// SelectItemNum 0
void Wifi_Set_Close();							// SelectItemNum 0

// Alarm Events Functions
void Alarm_1_Set_Open(uint8_t Select);			// SelectItemNum 0
void Alarm_2_Set_Open(uint8_t Select);			// SelectItemNum 1
void Alarm_3_Set_Open(uint8_t Select);			// SelectItemNum 2
void Alarm_4_Set_Open(uint8_t Select);			// SelectItemNum 3
void Alarm_5_Set_Open(uint8_t Select);			// SelectItemNum 4
void Alarm_6_Set_Open(uint8_t Select);			// SelectItemNum 5
void Alarm_1_Set_Close();						// SelectItemNum 0
void Alarm_2_Set_Close();						// SelectItemNum 1
void Alarm_3_Set_Close();						// SelectItemNum 2
void Alarm_4_Set_Close();						// SelectItemNum 3
void Alarm_5_Set_Close();						// SelectItemNum 4
void Alarm_6_Set_Close();						// SelectItemNum 5

// Graph Events Functions
void Graph_Mode_Set_Open(uint8_t Select);		// SelectItemNum 0
void Graph_Co2_Unit_Set_Open(uint8_t Select);	// SelectItemNum 1
void Graph_Mode_Set_Close();					// SelectItemNum 0
void Graph_Co2_Set_Close();						// SelectItemNum 1

// Save Events Functions
void Save_Set_Open(uint8_t Select);					// SelectItemNum 0
void Save_RawData_Set_Open(uint8_t Select);			// SelectItemNum 1
void Save_RollingRMS_Set_Open(uint8_t Select);		// SelectItemNum 2
void Save_GasConcentration_Set_Open(uint8_t Select);// SelectItemNum 3
void Save_SaveTime_Set_Open(uint8_t Select);		// SelectItemNum 4
void Save_Set_Close();								// SelectItemNum 0
void Save_RawData_Set_Close();						// SelectItemNum 1
void Save_RollingRMS_Set_Close();					// SelectItemNum 2
void Save_GasConcentration_Set_Close();				// SelectItemNum 3
void Save_SaveTime_Set_Close();						// SelectItemNum 4

void int2char(int num, char str[]);

void SineGraph();
void Graph(int32_t val, uint16_t color);
void Graphs(int32_t val1, uint16_t color1, int32_t val2, uint16_t color2);


#endif /* INC_LCD_INTERFACE_LIBRARY_H_ */
