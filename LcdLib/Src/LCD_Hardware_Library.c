/*
 * LCD_Hardware_Library.c
 *
 *  Created on: 18 Ara 2021
 *      Author: Kadir AK
 */

#include "LCD_Hardware_Library.h"
#include "LCD_Interface_Library.h"
/**
 * @brief Write command to LCD controller
 * @param cmd -> command to write
 * @return none
 */
static void LCD_WriteCommand(uint8_t cmd)
{
	LCD_Select();
	LCD_DC_Clr();
	HAL_SPI_Transmit_DMA(&LCD_SPI_PORT, &cmd, sizeof(cmd));
	LCD_UnSelect();
}

/**
 * @brief Write data to LCD controller
 * @param buff -> pointer of data buffer
 * @param buff_size -> size of the data buffer
 * @return none
 */
static void LCD_WriteData(uint8_t *buff, size_t buff_size)
{
	LCD_Select();
	LCD_DC_Set();

	// split data in small chunks because HAL can't send more than 64K at once

	while (buff_size > 0) {
		uint16_t chunk_size = buff_size > 65535 ? 65535 : buff_size;
		HAL_SPI_Transmit_DMA(&LCD_SPI_PORT,buff, chunk_size);
		while(HAL_SPI_GetState(&LCD_SPI_PORT) != HAL_SPI_STATE_READY);
		buff += chunk_size;
		buff_size -= chunk_size;
	}

	LCD_UnSelect();
}
/**
 * @brief Write data to LCD controller, simplify for 8bit data.
 * data -> data to write
 * @return none
 */
static void LCD_WriteSmallData(uint8_t data)
{
	LCD_Select();
	LCD_DC_Set();
	HAL_SPI_Transmit_DMA(&LCD_SPI_PORT, &data, sizeof(data));
	LCD_UnSelect();
}

/**
 * @brief Set the rotation direction of the display
 * @param m -> rotation parameter(please refer it in LCD.h)
 * @return none
 */
void LCD_SetRotation(uint8_t m)
{
	HAL_IWDG_Refresh(&hiwdg);
	LCD_WriteCommand(LCD_MADCTL);	// MADCTL
	switch(m)
	{
		case 1:
			LCD_WriteSmallData(LCD_MADCTL_MY | LCD_MADCTL_BGR);
			break;
		case 2:
			LCD_WriteSmallData(LCD_MADCTL_MV | LCD_MADCTL_BGR);
			break;
		case 3:
			LCD_WriteSmallData(LCD_MADCTL_MX | LCD_MADCTL_BGR);
			break;
		case 4:
			LCD_WriteSmallData(LCD_MADCTL_MX | LCD_MADCTL_MY | LCD_MADCTL_MV | LCD_MADCTL_BGR);
			break;
		default:
			LCD_WriteSmallData(LCD_MADCTL_MY | LCD_MADCTL_BGR);
			break;
	}
}

/**
 * @brief Set address of DisplayWindow
 * @param xi&yi -> coordinates of window
 * @return none
 */
static void LCD_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	LCD_Select();
	uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
	uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

	/* Column Address set */
	HAL_IWDG_Refresh(&hiwdg);
	LCD_WriteCommand(LCD_CASET);
	{
		uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
		LCD_WriteData(data, sizeof(data));
	}

	/* Row Address set */
	HAL_IWDG_Refresh(&hiwdg);
	LCD_WriteCommand(LCD_RASET);
	{
		uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
		LCD_WriteData(data, sizeof(data));
	}
	/* Write to RAM */
	HAL_IWDG_Refresh(&hiwdg);
	LCD_WriteCommand(LCD_RAMWR);
	LCD_UnSelect();
}

/**
 * @brief Initialize LCD controller
 * @param none
 * @return none
 */
void LCD_Init(void)
{
	// Backlight On
	HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_SET);
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(50);

	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);  //Turn LCD ON

	LCD_WriteCommand		(LCD_SWRESET); 			// software reset comand
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(100);
	LCD_WriteCommand 		(LCD_DISPOFF); 			// display off
	//------------power control------------------------------
	LCD_WriteCommand 		(LCD_POWER1); 			// power control
	LCD_WriteSmallData  	(0x26); 				// GVDD = 4.75v
	LCD_WriteCommand 		(LCD_POWER2); 			// power control
	LCD_WriteSmallData  	(0x11); 				// AVDD=VCIx2, VGH=VCIx7, VGL=-VCIx3
	//--------------VCOM-------------------------------------
	LCD_WriteCommand 		(LCD_VCOM1); 			// vcom control
	LCD_WriteSmallData  	(0x35);					// Set the VCOMH voltage (0x35 = 4.025v)
	LCD_WriteSmallData  	(0x3e); 				// Set the VCOML voltage (0x3E = -0.950v)
	LCD_WriteCommand 		(LCD_VCOM2); 			// vcom control
	LCD_WriteSmallData 	 	(0xbe);

	//------------memory access control------------------------
	LCD_WriteCommand 		(LCD_MADCTL); 			// memory access control
	LCD_WriteSmallData		(0x48);

	LCD_WriteCommand 		(LCD_COLMOD); 			// pixel format set
	LCD_WriteSmallData  	(0x55); 				// 16bit /pixel

	LCD_WriteCommand		(LCD_FRC);
	LCD_WriteSmallData		(0);
	LCD_WriteSmallData		(0x1F);
	//-------------ddram ----------------------------
	LCD_WriteCommand 		(LCD_CASET); 			// column set
	LCD_WriteSmallData  	(0x00); 				// x0_HIGH---0
	LCD_WriteSmallData  	(0x00); 				// x0_LOW----0
	LCD_WriteSmallData   	(0x00); 				// x1_HIGH---240
	LCD_WriteSmallData   	(0xEF); 				// x1_LOW----240
	LCD_WriteCommand 		(LCD_RASET);			// page address set
	LCD_WriteSmallData   	(0x00); 				// y0_HIGH---0
	LCD_WriteSmallData   	(0x00); 				// y0_LOW----0
	LCD_WriteSmallData   	(0x01); 				// y1_HIGH---320
	LCD_WriteSmallData  	(0x3F); 				// y1_LOW----320

	LCD_WriteCommand 		(LCD_TEARING_OFF); 		// tearing effect off
	//LCD_write_cmd(LCD1_TEARING_ON); 				// tearing effect on
	//LCD_write_cmd(LCD1_DISPLAY_INVERSION); 		// display inversion
	LCD_WriteCommand 		(LCD_Entry_Mode_Set); 	// entry mode set
	// Deep Standby Mode: OFF
	// Set the output level of gate driver G1-G320: Normal display
	// Low voltage detection: Disable
	LCD_WriteSmallData   	(0x07);
	//-----------------display------------------------
	LCD_WriteCommand 		(LCD_DFC); 				// display function control
	//Set the scan mode in non-display area
	//Determine source/VCOM output in a non-display area in the partial display mode
	LCD_WriteSmallData   	(0x0a);
	//Select whether the liquid crystal type is normally white type or normally black type
	//Sets the direction of scan by the gate driver in the range determined by SCN and NL
	//Select the shift direction of outputs from the source driver
	//Sets the gate driver pin arrangement in combination with the GS bit to select the optimal scan mode for the module
	//Specify the scan cycle interval of gate driver in non-display area when PTG to select interval scan
	LCD_WriteSmallData   	(0x82);
	// Sets the number of lines to drive the LCD at an interval of 8 lines
	LCD_WriteSmallData  	(0x27);
	LCD_WriteSmallData   	(0x00); 				// clock divisor

	LCD_WriteCommand 		(LCD_SLPOUT); 			// sleep out
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(100);
	LCD_WriteCommand 		(LCD_DISPON); 			// display on
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(100);
	LCD_WriteCommand 		(LCD_RAMWR); 			// memory write
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(5);

	LCD_SetRotation(LCD_ROTATION);
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(50);
	LCD_Fill_Color(COLOR_BLACK);				//	Fill with Black.
}

/**
 * @brief Fill the DisplayWindow with single color
 * @param color -> color to Fill with
 * @return none
 */
void LCD_Fill_Color(uint16_t color)
{
	LCD_DrawImage(0, 0,320, 240, Dark_screen.data);//	Fill with Black.
}

/**
 * @brief Draw a Pixel
 * @param x&y -> coordinate to Draw
 * @param color -> color of the Pixel
 * @return none
 */
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x < 0) || (x >= LCD_WIDTH) ||
		 (y < 0) || (y >= LCD_HEIGHT))	return;

	LCD_SetAddressWindow(x, y, x, y);
	uint8_t data[] = {color >> 8, color & 0xFF};
	LCD_Select();
	LCD_WriteData(data, sizeof(data));
	LCD_UnSelect();
}

/**
 * @brief Fill an Area with single color
 * @param xSta&ySta -> coordinate of the start point
 * @param xEnd&yEnd -> coordinate of the end point
 * @param color -> color to Fill with
 * @return none
 */
void LCD_Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
	if ((xEnd < 0) || (xEnd >= LCD_WIDTH) ||
		 (yEnd < 0) || (yEnd >= LCD_HEIGHT))	return;
	LCD_Select();
	uint16_t i, j;
	LCD_SetAddressWindow(xSta, ySta, xEnd, yEnd);
	for (i = ySta; i <= yEnd; i++)
		for (j = xSta; j <= xEnd; j++) {
			uint8_t data[] = {color >> 8, color & 0xFF};
			LCD_WriteData(data, sizeof(data));
		}
	LCD_UnSelect();
}

/**
 * @brief Draw a big Pixel at a point
 * @param x&y -> coordinate of the point
 * @param color -> color of the Pixel
 * @return none
 */
void LCD_DrawPixel_4px(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x <= 0) || (x > LCD_WIDTH) ||
		 (y <= 0) || (y > LCD_HEIGHT))	return;
	LCD_Select();
	LCD_Fill(x - 1, y - 1, x + 1, y + 1, color);
	LCD_UnSelect();
}

/**
 * @brief Draw a line with single color
 * @param x1&y1 -> coordinate of the start point
 * @param x2&y2 -> coordinate of the end point
 * @param color -> color of the line to Draw
 * @return none
 */
void LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
        uint16_t color) {
	uint16_t swap;
    uint16_t steep = ABS(y1 - y0) > ABS(x1 - x0);
    if (steep) {
		swap = x0;
		x0 = y0;
		y0 = swap;

		swap = x1;
		x1 = y1;
		y1 = swap;
        //_swap_int16_t(x0, y0);
        //_swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
		swap = x0;
		x0 = x1;
		x1 = swap;

		swap = y0;
		y0 = y1;
		y1 = swap;
        //_swap_int16_t(x0, x1);
        //_swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = ABS(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            LCD_DrawPixel(y0, x0, color);
        } else {
            LCD_DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**
 * @brief Draw a Rectangle with single color
 * @param xi&yi -> 2 coordinates of 2 top points.
 * @param color -> color of the Rectangle line
 * @return none
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	LCD_Select();
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
	LCD_UnSelect();
}

/**
 * @brief Draw a circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle line
 * @return  none
 */
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	LCD_Select();
	LCD_DrawPixel(x0, y0 + r, color);
	LCD_DrawPixel(x0, y0 - r, color);
	LCD_DrawPixel(x0 + r, y0, color);
	LCD_DrawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		LCD_DrawPixel(x0 + x, y0 + y, color);
		LCD_DrawPixel(x0 - x, y0 + y, color);
		LCD_DrawPixel(x0 + x, y0 - y, color);
		LCD_DrawPixel(x0 - x, y0 - y, color);

		LCD_DrawPixel(x0 + y, y0 + x, color);
		LCD_DrawPixel(x0 - y, y0 + x, color);
		LCD_DrawPixel(x0 + y, y0 - x, color);
		LCD_DrawPixel(x0 - y, y0 - x, color);
	}
	LCD_UnSelect();
}

/**
 * @brief Draw an Image on the screen
 * @param x&y -> start point of the Image
 * @param w&h -> width & height of the Image to Draw
 * @param data -> pointer of the Image array
 * @return none
 */
void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)
{
	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
		return;
	if ((x + w - 1) >= LCD_WIDTH)
		return;
	if ((y + h - 1) >= LCD_HEIGHT)
		return;

	LCD_Select();
	LCD_SetAddressWindow(x, y, x + w - 1, y + h - 1);
	LCD_WriteData((uint8_t *)data, sizeof(uint16_t) * w * h);
	LCD_UnSelect();
}

/**
 * @brief Invert Fullscreen color
 * @param invert -> Whether to invert
 * @return none
 */
void LCD_InvertColors(uint8_t invert)
{
	LCD_Select();
	LCD_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
	LCD_UnSelect();
}

/**
 * @brief Write a char
 * @param  x&y -> cursor of the start point.
 * @param ch -> char to write
 * @param font -> fontstyle of the string
 * @param color -> color of the char
 * @param bgcolor -> background color of the char
 * @return  none
 */
void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
	uint32_t i, b, j;
	LCD_Select();
	LCD_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);

	for (i = 0; i < font.height; i++) {
		b = font.data[(ch - 32) * font.height + i];
		for (j = 0; j < font.width; j++) {
			if ((b << j) & 0x8000) {
				uint8_t data[] = {color >> 8, color & 0xFF};
				LCD_WriteData(data, sizeof(data));
			}
			else {
				uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
				LCD_WriteData(data, sizeof(data));
			}
		}
	}
	LCD_UnSelect();
}

/**
 * @brief Write a string
 * @param  x&y -> cursor of the start point.
 * @param str -> string to write
 * @param font -> fontstyle of the string
 * @param color -> color of the string
 * @param bgcolor -> background color of the string
 * @return  none
 */
void LCD_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor)
{
	LCD_Select();
	while (*str) {
		if (x + font.width >= LCD_WIDTH) {
			x = 0;
			y += font.height;
			if (y + font.height >= LCD_HEIGHT) {
				break;
			}

			if (*str == ' ') {
				// skip spaces in the beginning of the new line
				str++;
				continue;
			}
		}
		LCD_WriteChar(x, y, *str, font, color, bgcolor);
		x += font.width;
		str++;
	}
	LCD_UnSelect();
}

/**
 * @brief Draw a filled Rectangle with single color
 * @param  x&y -> coordinates of the starting point
 * @param w&h -> width & height of the Rectangle
 * @param color -> color of the Rectangle
 * @return  none
 */
void LCD_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	LCD_Select();
	uint8_t i;

	/* Check input parameters */
	if (x >= LCD_WIDTH ||
		y >= LCD_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= LCD_WIDTH) {
		w = LCD_WIDTH - x;
	}
	if ((y + h) >= LCD_HEIGHT) {
		h = LCD_HEIGHT - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		LCD_DrawLine(x, y + i, x + w, y + i, color);
	}
	LCD_UnSelect();
}

/**
 * @brief Draw a Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the lines
 * @return  none
 */
void LCD_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	LCD_Select();
	/* Draw lines */
	LCD_DrawLine(x1, y1, x2, y2, color);
	LCD_DrawLine(x2, y2, x3, y3, color);
	LCD_DrawLine(x3, y3, x1, y1, color);
	LCD_UnSelect();
}

/**
 * @brief Draw a filled Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the triangle
 * @return  none
 */
void LCD_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	LCD_Select();
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
			yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
			curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	}
	else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	}
	else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
	else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		LCD_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
	LCD_UnSelect();
}

/**
 * @brief Draw a Filled circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle
 * @return  none
 */
void LCD_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	LCD_Select();
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	LCD_DrawPixel(x0, y0 + r, color);
	LCD_DrawPixel(x0, y0 - r, color);
	LCD_DrawPixel(x0 + r, y0, color);
	LCD_DrawPixel(x0 - r, y0, color);
	LCD_DrawLine(x0 - r, y0, x0 + r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		LCD_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
		LCD_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

		LCD_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
		LCD_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
	}
	LCD_UnSelect();
}


/**
 * @brief Open/Close tearing effect line
 * @param tear -> Whether to tear
 * @return none
 */
void LCD_TearEffect(uint8_t tear)
{
	LCD_Select();
	LCD_WriteCommand(tear ? 0x35 /* TEON */ : 0x34 /* TEOFF */);
	LCD_UnSelect();
}


/**
 * @brief A Simple test function for LCD
 * @param  none
 * @return  none
 */
void LCD_Test(void)
{
	LCD_Fill_Color(COLOR_WHITE);
	HAL_Delay(500);
	LCD_WriteString(10, 20, "Speed Test", Font_11x18, COLOR_RED, COLOR_WHITE);
	HAL_Delay(500);
	LCD_Fill_Color(COLOR_CYAN);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_RED);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_BLUE);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_GREEN);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_YELLOW);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_BROWN);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_DARKBLUE);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_MAGENTA);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_LIGHTGREEN);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_LGRAY);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_LBBLUE);
    HAL_Delay(250);
	LCD_Fill_Color(COLOR_WHITE);
	HAL_Delay(250);

	LCD_WriteString(10, 10, "Font test.", Font_16x26, COLOR_GBLUE, COLOR_WHITE);
	LCD_WriteString(10, 50, "Hello Steve!", Font_7x10, COLOR_RED, COLOR_WHITE);
	LCD_WriteString(10, 75, "Hello Steve!", Font_11x18, COLOR_YELLOW, COLOR_WHITE);
	LCD_WriteString(10, 100, "Hello Steve!", Font_16x26, COLOR_MAGENTA, COLOR_WHITE);
	HAL_Delay(1000);

	LCD_Fill_Color(COLOR_RED);
	LCD_WriteString(10, 10, "Rect./Line.", Font_11x18, COLOR_YELLOW, COLOR_RED);
	LCD_DrawRectangle(30, 30, 100, 100, COLOR_WHITE);
	HAL_Delay(1000);

	LCD_Fill_Color(COLOR_RED);
	LCD_WriteString(10, 10, "Filled Rect.", Font_11x18, COLOR_YELLOW, COLOR_RED);
	LCD_DrawFilledRectangle(30, 30, 50, 50, COLOR_WHITE);
	HAL_Delay(1000);


	LCD_Fill_Color(COLOR_RED);
	LCD_WriteString(10, 10, "Circle.", Font_11x18, COLOR_YELLOW, COLOR_RED);
	LCD_DrawCircle(60, 60, 25, COLOR_WHITE);
	HAL_Delay(1000);

	LCD_Fill_Color(COLOR_RED);
	LCD_WriteString(10, 10, "Filled Cir.", Font_11x18, COLOR_YELLOW, COLOR_RED);
	LCD_DrawFilledCircle(60, 60, 25, COLOR_WHITE);
	HAL_Delay(1000);

	LCD_Fill_Color(COLOR_RED);
	LCD_WriteString(10, 10, "Triangle", Font_11x18, COLOR_YELLOW, COLOR_RED);
	LCD_DrawTriangle(30, 30, 30, 70, 60, 40, COLOR_WHITE);
	HAL_Delay(1000);

	LCD_Fill_Color(COLOR_RED);
	LCD_WriteString(10, 10, "Filled Tri", Font_11x18, COLOR_YELLOW, COLOR_RED);
	LCD_DrawFilledTriangle(30, 30, 30, 70, 60, 40, COLOR_WHITE);
	HAL_Delay(1000);
}

