/*
 * Graphics.c
 *
 *  Created on: 2015Äê7ÔÂ25ÈÕ
 *      Author: SL
 */

#include "Graphics.h"

// color management

inline lcd_color color_convert(color w_color)
{
    lcd_color color;
    unsigned char red, green, blue;
	red = (w_color >> 16) & 0xFF;
	green = (w_color >> 8) & 0xFF;
	blue = w_color & 0xFF;
	color = ((red >> 3) << 11) + ((green >> 2) << 5) + (blue >> 3);
	return color;
}

inline void draw_point(point point, color color){
	if((color >> 24) != 0){
		return;
	}

	LCD_DrawPoint(point.x, point.y, color_convert(color));
}

static inline void draw_point_inner(alt_u16 x, alt_u16 y, color color){
	if((color >> 24) != 0){
		return;
	}

	LCD_DrawPoint(x, y, color_convert(color));
}

inline void fill_area(bound bound, color color){
	if((color >> 24) != 0){
		return;
	}

	if(bound.x.min > bound.x.max || bound.y.min > bound.y.max){
		return;
	}

	LCD_Color_Fill(bound.x.min, bound.y.min, bound.x.max, bound.y.max, color_convert(color));
}

inline void draw_Hline(range x, int y, color c_line){
	if((c_line >> 24) != 0){
		return;
	}

    LCD_Color_Fill(x.min, y, x.max, y, color_convert(c_line));
}

inline void draw_Vline(int x, range y, color c_line){
	if((c_line >> 24) != 0){
		return;
	}

	LCD_Color_Fill(x, y.min, x, y.max, color_convert(c_line));
}

void draw_line(point point1, point point2, color wcolor,int line_width)
{
	if((wcolor >> 24) != 0){
		return;
	}

    int i, j;
    int x_length, y_length, abs_x_length, abs_y_length;
    int x1, y1, x2, y2;
    float temp;

    x1 = point1.x;
    y1 = point1.y;
    x2 = point2.x;
    y2 = point2.y;

    x_length = x2 - x1;
    y_length = y2 - y1;
    abs_x_length = abs(x_length);
    abs_y_length = abs(y_length);

    if(abs_x_length > abs_y_length){
        for(i = 0;i <= abs_x_length;i++){
            temp = (float)i * y_length/x_length;
            if(x_length > 0)
            {
                for(j = 0;j < line_width;j++)
                	draw_point_inner(x1 + i, y1 + temp + j, wcolor);
            }else{
                for(j = 0;j < line_width;j++)
                	draw_point_inner(x1 - i, y1 - temp + j, wcolor);
            }
        }
    }else if(abs_y_length != 0){
        for(i = 0;i <= abs_y_length;i++){
            temp = (float)i * x_length/y_length;
            if(y_length > 0)
            {
                for(j = 0;j < line_width;j++)
                	draw_point_inner(x1 + temp + j, y1 + i, wcolor);
            }else{
                for(j = 0;j < line_width;j++)
                	draw_point_inner(x1 - temp + j, y1 - i, wcolor);
            }
        }
    }
}

void draw_circle(point center,int circle_r,color circle_color)
{
	if((circle_color >> 24) != 0){
		return;
	}

    int i,temp,y_0,y_1;
    int center_x, center_y;

    center_x = center.x;
    center_y = center.y;

    temp = center_y + circle_r;
    for(i = 0;i <= (int)circle_r*0.7071+1;i++)
    {
        y_0 = abs(pow(temp-center_y,2) + pow(i,2) - pow(circle_r,2));
        y_1 = abs(pow(temp-center_y-1,2) + pow(i,2) - pow(circle_r,2));
        if(y_0 < y_1){
        	draw_point_inner(center_x+i,temp*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x+i,(2*center_y - temp)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-i,temp*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-i,(2*center_y - temp)*1.078 - 0.078*center_y, circle_color);

        	draw_point_inner(temp-center_y+center_x,(center_y+i)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(temp-center_y+center_x,(center_y-i)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-temp+center_y,(center_y+i)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-temp+center_y,(center_y-i)*1.078 - 0.078*center_y, circle_color);
        }else{
        	draw_point_inner(center_x+i,(temp-1)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x+i,(2*center_y - temp+1)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-i,(temp-1)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-i,(2*center_y - temp+1)*1.078 - 0.078*center_y, circle_color);

        	draw_point_inner(temp-1-center_y+center_x,(center_y+i)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(temp-1-center_y+center_x,(center_y-i)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-temp+1+center_y,(center_y+i)*1.078 - 0.078*center_y, circle_color);
        	draw_point_inner(center_x-temp+1+center_y,(center_y-i)*1.078 - 0.078*center_y, circle_color);
            temp--;
        }
    }
}
