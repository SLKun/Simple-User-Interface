#ifndef SIMPLE_USER_INTERFACE_INCLUDES_H_
#define SIMPLE_USER_INTERFACE_INCLUDES_H_

#include "de0-nano.h"
#include "Graphics.h"
#include "Driver/TouchScreen/TSDriver.h"
#include "Fonts.h"

// the max quantity number item of widget
#define MAX_LIST_LENGTH 128

// Message ID
#define WIDGET_INIT          0
#define WIDGET_TOUCHED       1
#define WIDGET_SWITCH_TOUCH  2
#define DATA_REFRESH         3
#define WIDGET_REDRAW		 4
#define WIDGET_HIDE			 5

//  TouchScreen Config
#define GUI_TOUCH_SWAP_XY 0
#define GUI_TOUCH_MIRROR_X 0
#define GUI_TOUCH_MIRROR_Y 1
#define GUI_TOUCH_AD_LEFT 0
#define GUI_TOUCH_AD_RIGHT 2700
#define GUI_TOUCH_AD_TOP 0
#define GUI_TOUCH_AD_BOTTOM 1500
#define GUI_TOUCH_XSIZE 800
#define GUI_TOUCH_YSIZE 480

// text align
#define H_ALIGN_LEFT   0x02  //0000 0010
#define H_ALIGN_CENTER 0x03  //0000 0011
#define H_ALIGN_RIGHT  0x01  //0000 0001
#define V_ALIGN_TOP    0x20  //0010 0000
#define V_ALIGN_CENTER 0x30  //0011 0000
#define V_ALIGN_BOTTOM 0x10  //0001 0000

// Some Default Define
#define BUTTON_CHOOSED        0x6d6c6d//0x4d93e7//0x6d6c6d
#define BUTTON_UNCHOOSED      0xfbfbfb//0x006ec6
#define BUTTON_TEXT_UNCHOOSED 0x6d6c6d//0x060606
#define BUTTON_TEXT_CHOOSED   0xfbfbfb//0xfbfbfb

// Base Typedef

typedef alt_u8 align;

// messages

typedef struct _messages
{
    int dest;
    int msgid;
    void *data;
} messages;

// Defination of widget

typedef struct _widget
{
    int id;                  //To Distinguish different widget
    frame frame;                //relatively position
    bound bound;                //absolutely position
    char *data;                 //the text that showed
    void (*callback)(messages);   //the callback function
    int ishided;
    union {
    	struct {
    		int isholded;
    		char *text;
    	}button;				// for some button
    	char *text;				// for text field
    	void *ptr;				// general use
    } state_info;
} widget;

// Use for generating User Interface From Array

typedef struct _widget_info
{
    int id;                    //To Distinguish different widget
    int x;                        //the position of left upside corner
    int y;
    int width;                    //the size of this widget
    int height;
    char *data;                   //the text that showed
    void (*callback)(messages);   //the callback function
} widget_info;

//global widget table

typedef struct _widget_table
{
    alt_u32 length;               //the length of the list
    widget list[MAX_LIST_LENGTH]; //the list
} widget_table;


/* ATTENTION
 * when using text field, if you need change text.
 * maybe you can malloc a memory, and then let widget.data pointer to it.
 * what's more, when using message.data, when sending, use malloc, at receiving, you must free it!
 * be very casual when using malloc, you must remember to FREE it!
 */

// button with text, frame and background, generally with size 24 font
ret_code draw_general_button(bound bound, char *text, color c_background, color c_frame, color c_font);
// text field normal
ret_code draw_general_textfield(bound bound, char *text, alt_u16 size, align align, color c_background, color c_font);
// text field will part fresh
ret_code draw_general_textfield_part_refresh(bound bound, char *last_text, char* text,alt_u16 size, color c_background, color c_font);
// text field with word warp
ret_code draw_general_multi_textfield_none_LF(bound bound, char *text, alt_u16 size, color c_background, color c_font);
// text field with LF and word warp
ret_code draw_general_multi_textfield(bound bound, char *text, alt_u16 size, color c_background, color c_font);
// simple way to create user interface
ret_code create_user_interface_from_array(const widget_info *pawidget, int length);
// manually create a widget
ret_code create_widget(int id, int x, int y, int width, int height, char *data, void (*callback)(messages));
// get widget object from a id -> use for get a widget
widget* get_widget_from_id(int id);
// send the message to widget
void send_message(messages message);
// touch screen routine to handle touch event
void touch_screen_routine();
// hide a widget
inline void hide_widget(int id);
// show a widget
inline void show_widget(int id);
// send WIDGET_SWITCH_TOUCH message
inline void switch_touch(int id);
// notify the data has been refresh
inline void data_refresh(int id, void *data);
// init the SUI
void SUI_Init(color c_background);

#endif /*SIMPLE_USER_INTERFACE_INCLUDES_H_*/
