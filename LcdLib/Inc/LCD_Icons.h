/*
 * LCD_Icons.h
 *
 *  Created on: Dec 16, 2021
 *      Author: Kadir AK
 */

#ifndef INC_LCD_ICONS_H_
#define INC_LCD_ICONS_H_

#include "stdint.h"

typedef struct {
    const uint16_t width;
    uint16_t height;
    const uint16_t *data;
} IconsDef;

//Icon lib.

extern IconsDef notifications_off;
extern IconsDef notifications_on;
extern IconsDef wifi_off;
extern IconsDef battery_on;
extern IconsDef device_on;
extern IconsDef Dark_screen;
extern IconsDef logo_denizler;


#endif /* INC_LCD_ICONS_H_ */
