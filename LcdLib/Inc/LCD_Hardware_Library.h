/*
 * LCD_Hardware_Library.h
 *
 *  Created on: 18 Ara 2021
 *      Author: Kadir AK
 */

#ifndef INC_LCD_HARDWARE_LIBRARY_H_
#define INC_LCD_HARDWARE_LIBRARY_H_

//List of includes
#include "main.h"
#include "LCD_Fonts.h"

/* choose a Hardware SPI port to use. */
#define LCD_SPI_PORT  		hspi2
extern SPI_HandleTypeDef 	LCD_SPI_PORT;

/**
 * if you predefined pin names in CubeMX,
 * you could find them in main.h
 * and use them below
 */

/***** Define the pins tp connect *****/
#define LCD_RST_PORT 		LRST_GPIO_Port	// GPIOC
#define LCD_RST_PIN 		LRST_Pin		// GPIO_PIN_6
#define LCD_DC_PORT 		LDC_GPIO_Port	// GPIOC
#define LCD_DC_PIN 			LDC_Pin			// GPIO_PIN_7
#define LCD_CS_PORT 		LCS_GPIO_Port	// GPIOB
#define LCD_CS_PIN 			LCS_Pin			// GPIO_PIN_12

/***** Use if need backlight control *****/
#define LCD_BLK_PORT		LBL_GPIO_Port	// GPIOA
#define LCD_BLK_PIN			LBL_Pin 		// GPIO_PIN_4



/***** SPI Configuration *****
 *  SPI_MOSI_PORT GPIOB     SPI_MOSI_PIN GPIO_PIN_15   	Mode: Pull-up
 *  SPI_MOSI_PORT GPIOB     SPI_MISO_PIN GPIO_PIN_14	Mode: Pull-up
 *  SPI_SCK_PORT  GPIOB     SPI_SCK_PIN  GPIO_PIN_13
 *  SPI Clock = 72Mhz
 *  SPI Mode  = Half-Duplex Master
 *  Data Size = 8 Bits
 *  Baud Rate = 36 Mbits/s
 */

/* Comment one to use another one.
 * two parameters can be choosed
 * 135x240(0.96 inch) and 240x240(1.3inch)
 * X_SHIFT&Y_SHIFT are used to correct different display's resolution
 */

/* Choose a type you are using */
#define USING_240X320


/* Choose a display rotation you want to use: (0-3) */
//#define LCD_ROTATION 1 			// Left-Right
#define LCD_ROTATION 2			// Up-Down
//#define LCD_ROTATION 3			// Right-Left
//#define LCD_ROTATION 4				// Down-Up

#ifdef USING_240X320

	#define LCD_PIXEL_COUNT	240*320		// 76.800 pixel

		#if LCD_ROTATION == 1
			#define X_SHIFT 0
			#define Y_SHIFT 0
			#define LCD_WIDTH 	240
			#define LCD_HEIGHT 	320
		#elif LCD_ROTATION == 2
			#define X_SHIFT 0
			#define Y_SHIFT 0
			#define LCD_WIDTH 	320
			#define LCD_HEIGHT 	240
		#elif LCD_ROTATION == 3
			#define X_SHIFT 0
			#define Y_SHIFT 0
			#define LCD_WIDTH 	240
			#define LCD_HEIGHT 	320
		#elif LCD_ROTATION == 4
			#define X_SHIFT 0
			#define Y_SHIFT 0
			#define LCD_WIDTH 	320
			#define LCD_HEIGHT 	240
		#endif

#endif

/**
 *Color of pen
 *If you want to use another color, you can choose one in RGB565 format.
 */

#define COLOR_WHITE 				0xFFFF
#define COLOR_BLACK 				0x0000
#define COLOR_NAVY            		0x000F
#define COLOR_BLUE 					0x001F
#define COLOR_RED 					0xF800
#define COLOR_MAGENTA 				0xF81F
#define COLOR_GREEN 				0x07E0
#define COLOR_CYAN 					0x7FFF
#define COLOR_YELLOW 				0xFFE0
#define COLOR_GRAY 					0X8430
#define COLOR_BRED 					0XF81F
#define COLOR_GRED					0XFFE0
#define COLOR_GBLUE 				0X07FF
#define COLOR_BROWN 				0XBC40
#define COLOR_BRRED		 			0XFC07
#define COLOR_DARKBLUE 				0X01CF
#define COLOR_LIGHTBLUE 			0X7D7C
#define COLOR_GRAYBLUE 				0X5458

#define COLOR_LIGHTGREEN 			0X841F
#define COLOR_LGRAY 				0XC618
#define COLOR_LGRAYBLUE 			0XA651
#define COLOR_LBBLUE 				0X2B12

/* Control Registers and constant codes */
											//Command Description
#define LCD_NOP     				0x00	//1 NOP (0x00)
#define LCD_SWRESET 				0x01	//2 Software Reset (0x01)
#define LCD_RDDID   				0x04	//3 Read Display Identification Information (0x04)
#define LCD_RDDST   				0x09	//4 Read Display Status (0x09)
	//5 Read Display Power Mode (0x0A)
	//6 Read Display MADCTL (0x0B)
	//7 Read Display Pixel Format (0x0C)
	//8 Read Display Image Mode (0x0D)
	//9 Read Display Signal Mode (0x0E)
	//10 Read Display Signal Mode (0x0F)
#define LCD_SLPIN   				0x10	//11 Sleep In (0x10)
#define LCD_SLPOUT  				0x11	//12 Sleep Out (0x11)
#define LCD_PTLON   				0x12	//13 Partial Mode On (0x12)
#define LCD_NORON   				0x13	//14 Normal Display Mode On (0x13)
#define LCD_INVOFF  				0x20	//15 Display Inversion Off (0x20)
#define LCD_INVON   				0x21	//16 Display Inversion On (0x21)
#define LCD_GAMMA			    	0x26	//17 Gamma Set (0x26)
#define LCD_DISPOFF 				0x28	//18 Display Off (0x28)
#define LCD_DISPON  				0x29	//19 Display On (0x29)
#define LCD_CASET   				0x2A	//20 Column Address Set (0x2A)
#define LCD_RASET  					0x2B	//21 Page Address Set (0x2B)
#define LCD_RAMWR   				0x2C	//22 Memory Write (0x2C)
	//23 Color Setting fro 4K, 65K and 262K (0x2D)
#define LCD_RAMRD   				0x2E	//24 Memory Read (0x2E)
#define LCD_PTLAR   				0x30	//25 Partial Area (0x30)
#define LCD_VSCRDEF  				0x33	//26 Vertical Scrolling Definition (0x33)
#define LCD_TEARING_OFF				0x34	//27 Tearing Effect Line Off (0x34)
#define LCD_TEARING_ON				0x35	//28 Tearing Effect Line On (0x35)
#define LCD_MADCTL  				0x36	//29 Memory Access Control (0x36)
#define LCD_VSCRSADD 				0x37	//30 Vertical Scrolling Start Address (0x37)
#define LCD_IDMOFF  				0x38	//31 Idle Mode Off (0x38)
#define LCD_IDMON   				0x39	//32 Idle Mode On (0x39)
#define LCD_COLMOD  				0x3A	//33 Interface Pixel Format (0x3A)
#define LCD_FRC				    	0xB1	//37 Frame Rate Control (0xIn normal mode/Full colors) (0xB1)
	//38 Frame Rate Control(0xIn Idle mode/8-colors) (0xB2)
	//39 Frame Rate Control(0xIn Partial mode/full colors) (0xB3)
#define LCD_DISPLAY_INVERSION		0xB4	//40 Display Inversion Control (0xB4)
#define LCD_BPC						0xB5	//41 RGB Interface Blanking Porch setting (0xB5)
#define LCD_DFC				 		0xB6	//43 Display Fuction set 5 (0xB6)
#define LCD_Entry_Mode_Set			0xB7	//42 Source Driver Direction Control (0xB7)
	//43 Gate Driver Direction Control (0xB8)
#define LCD_POWER1					0xC0	//44 Power_Control 1 (0xC0)
#define LCD_POWER2					0xC1	//45 Power_Control 2 (0xC1)
	//46 Power_Control 3 (0xC2)
	//47 Power_Control 4 (0xC3)
	//48 Power_Control 5 (0xC4)
#define LCD_VCOM1					0xC5	//49 VCOM_Control 1 (0xC5)
#define LCD_VCOM2					0xC7	//50 VCOM Offset Control (0xC7)
	//51 Write ID4 Value (0xD3)
	//52 NV Memory Function Controller(0x1) (0xD5)
	//53 NV Memory Function Controller(0x2) (0xD6)
	//54 NV Memory Function Controller(0x3) (0xD7)
	//55 Read ID1 (0xDA)
	//56 Read ID2 (0xDB)
	//57 Read ID3 (0xDC)
#define LCD_PGAMMA					0xE0	//58 Positive Gamma Correction Setting (0xE0)
#define LCD_NGAMMA					0xE1	//59 Negative Gamma Correction Setting (0xE1)
#define LCD_3GAMMA_EN				0xF2	//60 GAM_R_SEL (0xF2)

//Orther Command
#define LCD_MEMCONTROL        		0x36

#define LCD_MADCTL_BGR 				0x08
#define LCD_MADCTL_MH  				0x04

#define LCD_WRDISBV  				0x51
#define LCD_WRCTRLD  				0x53
#define LCD_WRCACE   				0x55
#define LCD_WRCABCMB 				0x5E

#define LCD_RGB_INTERFACE   		0xB0
#define LCD_POWSAVE    				0xBC
#define LCD_DLPOFFSAVE 				0xBD

#define LCD_POWERA					0xCB
#define LCD_POWERB					0xCF

#define LCD_DTCA					0xE8
#define LCD_DTCB					0xEA
#define LCD_POWER_SEQ				0xED

#define LCD_INTERFACE				0xF6
#define LCD_PRC				   		0xF7

/**
 * Memory Data Access Control Register (0x36H)
 * MAP:     D7  D6  D5  D4  D3  D2  D1  D0
 * param:   MY  MX  MV  ML  RGB MH  -   -
 *
 */

/* Page Address Order ('0': Top to Bottom, '1': the opposite) */
#define LCD_MADCTL_MY  0x80
/* Column Address Order ('0': Left to Right, '1': the opposite) */
#define LCD_MADCTL_MX  0x40
/* Page/Column Order ('0' = Normal Mode, '1' = Reverse Mode) */
#define LCD_MADCTL_MV  0x20
/* Line Address Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define LCD_MADCTL_ML  0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define LCD_MADCTL_RGB 0x00

#define LCD_240x320_XSTART 0
#define LCD_240x320_YSTART 0

#define ST_CMD_DELAY   0x80

#define LCD_RDID1   0xDA
#define LCD_RDID2   0xDB
#define LCD_RDID3   0xDC
#define LCD_RDID4   0xDD

/* Advanced options */
/**
 * Caution: Do not operate these settings
 * You know what you are doing
 */

#define LCD_COLOR_MODE_16bit 0x55    //  RGB565 (16bit)
#define LCD_COLOR_MODE_18bit 0x66    //  RGB666 (18bit)

/* Basic operations */
#define LCD_RST_Clr() HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_Set() HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)

#define LCD_DC_Clr() HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_Set() HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET)

#define LCD_Select() HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_UnSelect() HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)

#define ABS(x) ((x) > 0 ? (x) : -(x))

/* Basic functions. */

void LCD_Init(void);
void LCD_SetRotation(uint8_t m);
void LCD_Fill_Color(uint16_t color);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color);
void LCD_DrawPixel_4px(uint16_t x, uint16_t y, uint16_t color);

/* Graphical functions. */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
void LCD_InvertColors(uint8_t invert);

/* Text functions. */
void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void LCD_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor);

/* Extented Graphical functions. */
void LCD_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void LCD_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void LCD_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

/* Command functions */
void LCD_TearEffect(uint8_t tear);

/* Simple test function. */
void LCD_Test(void);

/* Other functions */
#if !defined(USING_240X320)
	#error 	You should at least choose one display resolution!
#endif

#ifndef LCD_ROTATION
    #error You should at least choose a display rotation!
#endif


#endif /* INC_LCD_HARDWARE_LIBRARY_H_ */
