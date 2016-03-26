/*
 * Graphics.h
 *
 *  Created on: 2015Äê7ÔÂ25ÈÕ
 *      Author: SL
 */

 /*
 *
 *     The Interface between Driver Layer and User Layer
 *     It provides the color management and the basic 2D drawing Function.
 *
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "de0-nano.h"
#include "Driver/LCD/lcd.h"

// LCD Color Convert Setting
typedef alt_u16 lcd_color;

#define LCD_BLACK 0x0000
#define LCD_WHITE 0xFFFF
#define LCD_RED   0xF000
#define LCD_GREEN 0x0F00
#define LCD_BLUE  0x00F0

// Inner 32-bit color
typedef alt_u32 color; //32-bit color

#define TRANSPARENT 0xFF000000
#define BLACK       0x00000000
#define WHITE       0x00FFFFFF
#define RED         0x00FF0000
#define GREEN       0x0000FF00
#define BLUE        0x000000FF

// Basic Graphics Typedef

typedef struct _point   //point
{
    int x;
    int y;
} point;

typedef struct _size    //size
{
    int width;
    int height;
} size;

typedef struct _range   //a range of a number
{
    int min;
    int max;
} range;

typedef struct _frame   //the relatively positon of the widget
{
    point origin;
    size size;
} frame;

typedef struct _bound   //the absolutely position of the widget
{
    range x;
    range y;
} bound;

inline lcd_color color_convert(color w_color);

inline void draw_point(point point, color color);
inline void fill_area(bound bound, color color);
inline void draw_Hline(range x, int y, color c_line);
inline void draw_Vline(int x, range y, color c_line);
void draw_line(point point1, point point2, color wcolor,int line_width);
void draw_circle(point center,int circle_r,color circle_color);

#endif /* GRAPHICS_H_ */
