/*
 * font_library.c
 *
 *  Created on: 2015-5-4
 *      Author: FengPL
 */

#include "Fonts.h"

void draw_string(point origin, char *string, int size, color font_color){
	int i = 0;	//Avoid the string is too long and maybe no end sign
	char *ptr_char = string;
	point origin_c;
	while(*ptr_char != '\0' && i++ < 128){
		//get the index and row of font
		int index;
		switch(size){
		case 48: index = 0;  break;
		case 40: index = 1;  break;
		case 24: index = 2;  break;
		case 16: index = 3;  break;
		case 12: index = 4;  break;
		default: index = -1;
		}

		//draw the char
		origin_c.x = origin.x + (ptr_char - string) * _font[index].y_size/2;
		origin_c.y = origin.y;
		if(index != -1){
			draw_char(origin_c, index, *ptr_char, font_color);
		}else{
			printf("unsupported font size._%d\n", size);
		}
		ptr_char++;
	}
}

void draw_char(point origin, int index, char letter, color n_color)
{
	int i, j, font_size;   point point;
	Fonts *font = _font[index].font + (letter - 32) * _font[index].column;
	font_size = _font[index].x_size * _font[index].y_size;

	for(i = 0, j = 0; i < font_size; i++){
		if((*(font + i/8) << i%8) & 0x80){
			point.x = origin.x + j;
			point.y = origin.y;
			draw_point(point, n_color);
		}
		j++;
		if(j > font_size / _font[index].y_size - 1){
			origin.y++;
			j = 0;
		}
	}
}
