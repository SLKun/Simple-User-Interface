/*
 * fonts.h
 *
 *  Created on: 2015-7-31
 *      Author: SL
 */

/*
 *
 *     It Provides the ASCII Fonts and the simple function to draw string.
 *
 */

#ifndef FONT_LIBRARY_H_
#define FONT_LIBRARY_H_

#include "de0-nano.h"
#include "Graphics.h"

typedef const unsigned char Fonts;

typedef struct _font_info{
	Fonts *font;
	alt_u16 x_size;
    alt_u16 y_size;
	alt_u16 row;
	alt_u16 column;
}font_info;

extern Fonts ascii_48[];
extern Fonts ascii_40[];
extern Fonts ascii_24[];
extern Fonts ascii_16[];
extern Fonts ascii_12[];

static const font_info _font[] = {
        {(Fonts *)ascii_48, 24, 48, 96, 144}, //0
        {(Fonts *)ascii_40, 24, 40, 96, 120}, //1
        {(Fonts *)ascii_24, 16, 24, 96, 48},  //2
        {(Fonts *)ascii_16,  8, 16, 96, 16},  //3
        {(Fonts *)ascii_12,  6, 12, 96, 12}   //4
};

void draw_string(point origin, char *string, int size, color font_color);
void draw_char(point origin, int index, char letter, color n_color);

#endif /* FONT_LIBRARY_H_ */
